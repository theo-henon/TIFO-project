#pragma once

#include "image/image.hh"
#include "math/vector3f.hh"
#include "interpolator/interpolator.hh"

namespace tifo
{
    class Transform
    {
    public:
        virtual ~Transform() = default;

        virtual Vector3f apply(const Vector3f& vec) const = 0;
        virtual Image apply_img(const Image& img, const Interpolator& interpolator) const = 0;
        virtual Transform* inverse() const = 0;
        virtual std::vector<float> get_parameters() const = 0;
        virtual void set_parameters(const std::vector<float>& parameters) = 0;
        virtual void update() = 0;
        virtual void print(std::ostream& os) = 0;
    };
} // namespace tifo
