#pragma once

#include "../image/image.hh"
#include "math/vector3f.hh"

#include <optional>

namespace tifo
{
    class Interpolator
    {
    public:
        virtual ~Interpolator() = default;

        /**
         *
         * @param pixel_pos The coordinates of the pixel inside the image
         * @return the interpolated color of the image
         */
        virtual std::optional<uint8_t> interpolate(const Image& img, const Vector3f& pixel_pos) const = 0;
    };
} // namespace tifo
