#include "trilinear_interpolator.hh"

#include <algorithm>
#include <cmath>

namespace tifo
{
    std::optional<uint8_t> TrilinearInterpolator::interpolate(const Image& img, const Vector3f& pixel_pos) const
    {
        const float x = pixel_pos.x;
        const float y = pixel_pos.y;
        const float z = pixel_pos.z;

        const int x0 = static_cast<int>(std::floor(x));
        const int y0 = static_cast<int>(std::floor(y));
        const int z0 = static_cast<int>(std::floor(z));
        const int x1 = x0 + 1;
        const int y1 = y0 + 1;
        const int z1 = z0 + 1;

        const float dx = x - static_cast<float>(x0);
        const float dy = y - static_cast<float>(y0);
        const float dz = z - static_cast<float>(z0);

        // Get 8 corners around pixel
        float c000,c100,c010,c110,c001,c101,c011,c111;
        try
        {
            c000 = static_cast<float>(img.get_pixel(x0, y0, z0));
            c100 = static_cast<float>(img.get_pixel(x1, y0, z0));
            c010 = static_cast<float>(img.get_pixel(x0, y1, z0));
            c110 = static_cast<float>(img.get_pixel(x1, y1, z0));
            c001 = static_cast<float>(img.get_pixel(x0, y0, z1));
            c101 = static_cast<float>(img.get_pixel(x1, y0, z1));
            c011 = static_cast<float>(img.get_pixel(x0, y1, z1));
            c111 = static_cast<float>(img.get_pixel(x1, y1, z1));
        }
        catch (const std::out_of_range&)
        {
            return std::nullopt;
        }

        // Interpolation along x
        const float c00 = c000 * (1 - dx) + c100 * dx;
        const float c10 = c010 * (1 - dx) + c110 * dx;
        const float c01 = c001 * (1 - dx) + c101 * dx;
        const float c11 = c011 * (1 - dx) + c111 * dx;

        // Interpolation along y
        const float c0 = c00 * (1 - dy) + c10 * dy;
        const float c1 = c01 * (1 - dy) + c11 * dy;

        // Interpolation along z
        const float c = c0 * (1 - dz) + c1 * dz;

        // Clamp
        uint8_t color = static_cast<uint8_t>(std::clamp(c, 0.0f, 255.0f));
        return std::make_optional(color);
    }
} // namespace tifo