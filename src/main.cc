#include <iostream>
#include <tiffio.h>
#include <random>
#include <numbers>
#include <filesystem>
#include <unistd.h>

#include "math/matrix4f.hh"
#include "image/image.hh"
#include "interpolator/nearest_neighbor_interpolator.hh"
#include "transform/rigid_transform.hh"
#include "metric/mean_squared_error_metric.hh"
#include "optimizer/regular_gradient_descent_optimizer.hh"

tifo::Image load_tiff_3d(const std::string& filename)
{
    TIFF* tif = TIFFOpen(filename.c_str(), "r");
    if (!tif)
    {
        throw std::runtime_error("Cannot open TIFF file: " + filename);
    }

    uint32_t width = 0, height = 0;
    std::vector<std::vector<uint8_t>> slices_data;

    do
    {
        uint32_t w, h;
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);

        if (width == 0)
        {
            width = w;
        }
        if (height == 0)
        {
            height = h;
        }
        if (w != width || h != height)
        {
            throw std::runtime_error("Inconsistent slice dimensions");
        }

        std::vector<uint8_t> buffer(w * h);
        for (uint32_t row = 0; row < h; row++)
        {
            TIFFReadScanline(tif, &buffer[row * w], row, 0);
        }
        slices_data.push_back(std::move(buffer));
    } while (TIFFReadDirectory(tif));

    TIFFClose(tif);

    tifo::Image image(width, height, slices_data.size());
    for (uint32_t slice = 0; slice < slices_data.size(); ++slice)
    {
        const auto& buffer = slices_data[slice];
        for (uint32_t row = 0; row < height; ++row)
        {
            for (uint32_t col = 0; col < width; ++col)
            {
                image.set_pixel(row, col, slice, buffer[row * width + col]);
            }
        }
    }

    return image;
}

void save_tiff_3d(const std::string& filename, const tifo::Image& image)
{
    TIFF* tif = TIFFOpen(filename.c_str(), "w");
    if (!tif)
    {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    uint32_t width = image.get_width();
    uint32_t height = image.get_height();
    uint32_t depth = image.get_depth();

    for (uint32_t slice = 0; slice < depth; ++slice)
    {
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, height);

        std::vector<uint8_t> buffer(width);
        for (uint32_t row = 0; row < height; ++row)
        {
            for (uint32_t col = 0; col < width; ++col)
            {
                buffer[col] = image.get_pixel(row, col, slice);
            }
            if (TIFFWriteScanline(tif, buffer.data(), row, 0) < 0)
            {
                TIFFClose(tif);
                throw std::runtime_error("Failed to write scanline");
            }
        }

        if (slice < depth - 1)
        {
            if (!TIFFWriteDirectory(tif))
            {
                TIFFClose(tif);
                throw std::runtime_error("Failed to write directory");
            }
        }
    }

    TIFFClose(tif);
}

tifo::RigidTransform random_rigid_transform()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    // Translation value between [-5, 5]
    std::uniform_real_distribution translation_dist(-5.f, 5.f);

    // Rotation angle between [-π/12, π/12] radians (~ -7.5° à 7.5°)
    constexpr float PI = std::numbers::pi_v<float>;
    std::uniform_real_distribution rotation_dist(PI / 12.f, PI / 12.f);

    tifo::Vector3f random_translation(translation_dist(gen), translation_dist(gen), translation_dist(gen));
    tifo::Vector3f random_rotation(rotation_dist(gen), rotation_dist(gen), rotation_dist(gen));

    return { random_translation, random_rotation };
}

int main(int argc, char* argv[])
{
    float learning_rate = 1e-1f;
    int max_iterations = 1000;
    float tolerance = 1e-4f;
    float step = 1e-2f;
    bool save_steps = false;

    int opt;
    while ((opt = getopt(argc, argv, "g:i:t:s:S")) != -1)
    {
        switch (opt)
        {
        case 'g':
            learning_rate = std::stof(optarg);
            break;
        case 'i':
            max_iterations = std::stoi(optarg);
            break;
        case 't':
            tolerance = std::stof(optarg);
            break;
        case 's':
            step = std::stof(optarg);
            break;
        case 'S':
            save_steps = true;
            break;
        default:
            std::cerr << "Usage: " << argv[0]
                      << " [-g learning_rate] [-i max_iterations] [-t tolerance] [-s step] [-S] <FIXED_IMAGE> "
                         "<MOVING_IMAGE> <OUTPUT_IMAGE>"
                      << std::endl;
            return 1;
        }
    }

    if (argc - optind < 3)
    {
        std::cerr << "Invalid arguments. Example call: " << argv[0]
                  << " [-g learning_rate] [-i max_iterations] [-t tolerance] [-s step] [-S] <FIXED_IMAGE> "
                     "<MOVING_IMAGE> <OUTPUT_IMAGE>"
                  << std::endl;
        return 1;
    }

    const std::filesystem::path fixed_img_path(argv[optind]);
    const std::filesystem::path moving_img_path(argv[optind + 1]);
    const std::filesystem::path output_img_path(argv[optind + 2]);

    tifo::Image fixed_image = load_tiff_3d(fixed_img_path);
    tifo::Image moving_img = load_tiff_3d(moving_img_path);

    tifo::MeanSquaredErrorMetric mse_metric;
    tifo::RigidTransform transform = random_rigid_transform();
    tifo::RegularGradientDescentOptimizer optimizer(learning_rate);
    optimizer.set_max_iterations(max_iterations);
    optimizer.set_tolerance(tolerance);
    optimizer.set_step(step);

    // Configure callback if requested
    if (save_steps)
    {
        const std::string base = output_img_path.stem().string();
        const std::string ext = output_img_path.extension().string();
        const std::string parent = output_img_path.parent_path().string();

        optimizer.set_iteration_callback([base, ext, parent](int iteration, const tifo::Image& transformed_img) {
            std::ostringstream filename;
            filename << parent << "/" << base << "_" << iteration << ext;
            save_tiff_3d(filename.str(), transformed_img);
        });
    }

    optimizer.optimize(&transform, &mse_metric, fixed_image, moving_img);

    tifo::NearestNeighborInterpolator interpolator;
    tifo::Image transformed_img = transform.apply_img(moving_img, interpolator);
    save_tiff_3d(output_img_path, transformed_img);

    return 0;
}
