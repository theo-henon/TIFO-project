#include "nearest_neighbor_interpolator.hh"

#include <cmath>

namespace tifo
{
    std::optional<uint8_t> NearestNeighborInterpolator::interpolate(const Image& img, const Vector3f& pixel_pos) const
    {
        const auto pixel_idx = static_cast<uint32_t>(std::round(pixel_pos.x));
        const auto pixel_idy = static_cast<uint32_t>(std::round(pixel_pos.y));
        const auto pixel_idz = static_cast<uint32_t>(std::round(pixel_pos.z));

        try
        {
            const uint8_t color = img.get_pixel(pixel_idx, pixel_idy, pixel_idz);
            return std::make_optional(color);
        }
        catch (const std::out_of_range&)
        {
            return std::nullopt;
        }
    }
} // namespace tifo