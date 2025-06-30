#include "nearest_neighbor_interpolator.hh"

#include <cmath>

namespace tifo
{
    std::optional<uint8_t> NearestNeighborInterpolator::interpolate(const Image& img, const Vector3f& pixel_pos) const
    {
        uint32_t pixel_idx = std::round(pixel_pos.x);
        uint32_t pixel_idy = std::round(pixel_pos.y);
        uint32_t pixel_idz = std::round(pixel_pos.z);

        try
        {
            return std::make_optional(img.get_pixel(pixel_idx, pixel_idy, pixel_idz));
        }
        catch (const std::out_of_range& oor_ex)
        {
            return std::nullopt;
        }
    }
} // namespace tifo