#pragma once

#include "transform.hh"
#include "math/vector3f.hh"
#include "math/matrix3f.hh"
#include "math/matrix4f.hh"

namespace tifo
{
    class RigidTransform : public Transform
    {
    public:
        RigidTransform(const Vector3f& translation, const Vector3f& rotation);
        ~RigidTransform() override = default;

        Vector3f apply(const Vector3f& vec) const override;
        Vector3f apply(const Vector3f& vec, const Vector3f& center) const;
        Image apply_img(const Image& img, const Interpolator& interpolator) const override;
        Transform* inverse() const override;
        std::vector<float> get_parameters() const override;
        void set_parameters(const std::vector<float>& parameters) override;
        void update() override;
        void print(std::ostream& os) override;

    private:
        Vector3f translation_;
        Vector3f rotation_;
        Matrix4f transform_matrix_;
    };
} // namespace tifo
