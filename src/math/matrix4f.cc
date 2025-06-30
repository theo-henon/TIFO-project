#include "matrix4f.hh"

#include <algorithm>
#include <cmath>

namespace tifo
{
    Matrix4f::Matrix4f()
        : data_()
    {
        data_.fill(0.f);
    }

    Matrix4f::Matrix4f(const Matrix3f& matrix3f)
        : Matrix4f()
    {
        for (unsigned row = 0; row < 3; ++row)
        {
            for (unsigned col = 0; col < 3; ++col)
            {
                set_value(row, col, matrix3f.get_value(row, col));
            }
        }
    }

    float Matrix4f::get_value(unsigned row, unsigned col) const
    {
        return data_.at(4 * row + col);
    }

    void Matrix4f::set_value(const unsigned row, const unsigned col, const float value)
    {
        data_.at(4 * row + col) = value;
    }

    const std::array<float, 16>& Matrix4f::get_data() const
    {
        return data_;
    }

    const float* Matrix4f::get_data_ptr() const
    {
        return data_.data();
    }

    Matrix4f Matrix4f::transpose() const
    {
        Matrix4f result;
        for (unsigned row = 0; row < 4; ++row)
        {
            for (unsigned col = 0; col < 4; ++col)
            {
                result.set_value(row, col, get_value(col, row));
            }
        }
        return result;
    }

    Matrix4f Matrix4f::identity()
    {
        Matrix4f result;
        for (size_t i = 0; i < 4; ++i)
        {
            for (size_t j = 0; j < 4; ++j)
            {
                if (i == j)
                {
                    result.data_[i * 4 + j] = 1.f;
                }
            }
        }
        return result;
    }

    Matrix4f Matrix4f::diagonal(const float d1, const float d2, const float d3, const float d4)
    {
        Matrix4f result;
        result.set_value(0, 0, d1);
        result.set_value(1, 1, d2);
        result.set_value(2, 2, d3);
        result.set_value(3, 3, d4);
        return result;
    }

    Matrix4f Matrix4f::translate(const float x, const float y, const float z)
    {
        Matrix4f result = identity();
        result.set_value(0, 3, x);
        result.set_value(1, 3, y);
        result.set_value(2, 3, z);
        return result;
    }

    Matrix4f Matrix4f::translate(const Vector3f& vec)
    {
        return translate(vec.x, vec.y, vec.z);
    }

    Matrix4f Matrix4f::rotate(const float rx, const float ry, const float rz)
    {
        const float cos_x = std::cos(rx), sin_x = std::sin(rx);
        const float cos_y = std::cos(ry), sin_y = std::sin(ry);
        const float cos_z = std::cos(rz), sin_z = std::sin(rz);

        Matrix4f rot_x = diagonal(1, cos_x, cos_x, 1);
        rot_x.set_value(1, 2, -sin_x);
        rot_x.set_value(2, 1, sin_x);

        Matrix4f rot_y = diagonal(cos_y, 1, cos_y, 1);
        rot_y.set_value(0, 2, sin_y);
        rot_y.set_value(2, 0, -sin_y);

        Matrix4f rot_z = diagonal(cos_z, cos_z, 1, 1);
        rot_z.set_value(0, 1, -sin_z);
        rot_z.set_value(1, 0, sin_z);

        return rot_x * rot_y * rot_z;
    }

    Matrix4f Matrix4f::rotate(const Vector3f& vec)
    {
        return rotate(vec.x, vec.y, vec.z);
    }

    Matrix4f& Matrix4f::operator*=(const Matrix4f& rhs)
    {
        for (size_t i = 0; i < 4; ++i)
        {
            for (size_t j = 0; j < 4; ++j)
            {
                float sum = 0.f;
                for (size_t k = 0; k < 4; ++k)
                {
                    sum += data_[i * 4 + k] * rhs.data_[k * 4 + j];
                }
                data_[i * 4 + j] = sum;
            }
        }
        return *this;
    }

    Matrix4f operator*(const Matrix4f& lhs, const Matrix4f& rhs)
    {
        Matrix4f lhs1(lhs);
        return lhs1 *= rhs;
    }

    Vector3f operator*(const Matrix4f& lhs, const Vector3f& rhs)
    {
        float x = lhs.get_value(0, 0) * rhs.x + lhs.get_value(0, 1) * rhs.y + lhs.get_value(0, 2) * rhs.z
            + lhs.get_value(0, 3);
        float y = lhs.get_value(1, 0) * rhs.x + lhs.get_value(1, 1) * rhs.y + lhs.get_value(1, 2) * rhs.z
            + lhs.get_value(1, 3);
        float z = lhs.get_value(2, 0) * rhs.x + lhs.get_value(2, 1) * rhs.y + lhs.get_value(2, 2) * rhs.z
            + lhs.get_value(2, 3);
        return { x, y, z };
    }

    std::ostream& operator<<(std::ostream& os, const Matrix4f& rhs)
    {
        for (size_t i = 0; i < 4; ++i)
        {
            for (size_t j = 0; j < 4; ++j)
            {
                const float value = rhs.data_[i * 4 + j];
                const float abs_value = std::abs(value);

                if (abs_value < 1e-3)
                {
                    os << 0;
                }
                else
                {
                    os << value;
                }
                os << " ";
            }
            os << '\n';
        }
        return os;
    }
} // namespace tifo