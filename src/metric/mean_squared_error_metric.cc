#include "mean_squared_error_metric.hh"

namespace tifo
{
    float MeanSquaredErrorMetric::compare(const Image& img1, const Image& img2) const
    {
        double sum = 0.0f;
        for (uint32_t slice = 0; slice < img1.get_depth(); ++slice)
        {
            for (uint32_t row = 0; row < img1.get_height(); ++row)
            {
                for (uint32_t col = 0; col < img1.get_width(); ++col)
                {
                    const auto first = img1.get_pixel(row, col, slice) / 255.;
                    const auto second = img2.get_pixel(row, col, slice) / 255.;
                    const double diff = first - second;
                    sum += diff * diff;
                }
            }
        }
        const auto mse = static_cast<float>(sum / static_cast<double>(img1.size()));
        return 1.f / (1.f + mse);
    }
} // namespace tifo