#pragma once
#include "interpolator.hh"

namespace tifo
{

    class TrilinearInterpolator : public Interpolator
    {
    public:
        ~TrilinearInterpolator() override = default;

        std::optional<uint8_t> interpolate(const Image& img, const Vector3f& pixel_pos) const override;
    };
} // namespace tifo
