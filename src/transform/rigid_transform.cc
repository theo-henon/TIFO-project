#include "rigid_transform.hh"

namespace tifo
{
    RigidTransform::RigidTransform(const Vector3f& translation, const Vector3f& rotation)
        : translation_(translation)
        , rotation_(rotation)
    {
        this->update();
    }

    Vector3f RigidTransform::apply(const Vector3f& vec) const
    {
        return apply(vec, {});
    }

    Vector3f RigidTransform::apply(const Vector3f& vec, const Vector3f& center) const
    {
        return transform_matrix_ * vec;
    }

    Image RigidTransform::apply_img(const Image& img, const Interpolator& interpolator) const
    {
        auto* inverse_transform = inverse();
        Image transformed_img(img);
        // Vector3f moving_img_center = { static_cast<float>(img.get_width()) / 2.f,
        //                                      static_cast<float>(img.get_height()) / 2.f,
        //                                      static_cast<float>(img.get_depth()) / 2.f };
        for (uint32_t slice = 0; slice < transformed_img.get_depth(); ++slice)
        {
            for (uint32_t row = 0; row < transformed_img.get_height(); ++row)
            {
                for (uint32_t col = 0; col < transformed_img.get_width(); ++col)
                {
                    tifo::Vector3f dest_pos(static_cast<float>(col), static_cast<float>(row),
                                            static_cast<float>(slice));

                    tifo::Vector3f source_pos = inverse_transform->apply(dest_pos);
                    // std::cout << dest_pos << " <- " << source_pos << std::endl;
                    auto opt_color = interpolator.interpolate(img, source_pos);
                    if (opt_color.has_value())
                    {
                        transformed_img.set_pixel(col, row, slice, opt_color.value());
                    }
                }
            }
        }
        delete inverse_transform;
        return transformed_img;
    }

    Transform* RigidTransform::inverse() const
    {
        RigidTransform* inverse_transform = new RigidTransform(-translation_, rotation_);
        inverse_transform->transform_matrix_ = transform_matrix_;
        return inverse_transform;
    }

    std::vector<float> RigidTransform::get_parameters() const
    {
        return { translation_.x, translation_.y, translation_.z, rotation_.x, rotation_.y, rotation_.z };
    }

    void RigidTransform::set_parameters(const std::vector<float>& parameters)
    {
        translation_.x = parameters[0];
        translation_.y = parameters[1];
        translation_.z = parameters[2];
        rotation_.x = parameters[3];
        rotation_.y = parameters[4];
        rotation_.z = parameters[5];
        update();
    }

    void RigidTransform::update()
    {
        this->transform_matrix_ = Matrix4f::rotate(rotation_) * Matrix4f::translate(translation_);
    }

    void RigidTransform::print(std::ostream& os)
    {
        os << "T=" << translation_ << " R=" << rotation_;
    }

} // namespace tifo