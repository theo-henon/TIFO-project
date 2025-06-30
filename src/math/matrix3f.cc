#include "matrix3f.hh"

#include <cmath>

namespace tifo
{
    Matrix3f::Matrix3f()
        : data_()
    {
        data_.fill(0);
    }

    float Matrix3f::get_value(unsigned row, unsigned col) const
    {
        return data_.at(3 * row + col);
    }

    void Matrix3f::set_value(unsigned row, unsigned col, float value)
    {
        data_.at(3 * row + col) = value;
    }

    const std::array<float, 9>& Matrix3f::get_data() const
    {
        return data_;
    }

    const float* Matrix3f::get_data_ptr() const
    {
        return data_.data();
    }

    Matrix3f Matrix3f::transpose() const
    {
        Matrix3f result;
        for (unsigned row = 0; row < 3; ++row)
        {
            for (unsigned col = 0; col < 3; ++col)
            {
                result.set_value(row, col, get_value(col, row));
            }
        }
        return result;
    }

    Matrix3f Matrix3f::diagonal(float d1, float d2, float d3)
    {
        Matrix3f result;
        result.set_value(0, 0, d1);
        result.set_value(1, 1, d2);
        result.set_value(2, 2, d3);
        return result;
    }

    Matrix3f Matrix3f::identity()
    {
        return diagonal(1.f, 1.f, 1.f);
    }

    Matrix3f Matrix3f::rotate(float rx, float ry, float rz)
    {
        const float cos_x = std::cos(rx), sin_x = std::sin(rx);
        const float cos_y = std::cos(ry), sin_y = std::sin(ry);
        const float cos_z = std::cos(rz), sin_z = std::sin(rz);

        Matrix3f rot_x = diagonal(1, cos_x, cos_x);
        rot_x.set_value(1, 2, -sin_x);
        rot_x.set_value(2, 1, sin_x);

        Matrix3f rot_y = diagonal(cos_y, 1, cos_y);
        rot_y.set_value(0, 2, sin_y);
        rot_y.set_value(2, 0, -sin_y);

        Matrix3f rot_z = diagonal(cos_z, cos_z, 1);
        rot_z.set_value(0, 1, -sin_z);
        rot_z.set_value(1, 0, sin_z);

        return rot_x * rot_y * rot_z;
    }

    Matrix3f Matrix3f::rotate(const Vector3f& vec)
    {
        return rotate(vec.x, vec.y, vec.z);
    }

    Matrix3f& Matrix3f::operator*=(const Matrix3f& rhs)
    {
        for (unsigned i = 0; i < 3; ++i)
        {
            for (unsigned j = 0; j < 3; ++j)
            {
                float sum = 0.f;
                for (unsigned k = 0; k < 3; ++k)
                {
                    sum += this->get_value(i, k) * rhs.get_value(k, j);
                }
                this->set_value(i, j, sum);
            }
        }
        return *this;
    }

    Matrix3f operator*(const Matrix3f& lhs, const Matrix3f& rhs)
    {
        Matrix3f lhs1(lhs);
        return lhs1 *= rhs;
    }

    Vector3f operator*(const Matrix3f& lhs, const Vector3f& rhs)
    {
        Vector3f result;
        result.x = lhs.get_value(0, 0) * rhs.x + lhs.get_value(0, 1) * rhs.y + lhs.get_value(0, 2) * rhs.z;
        result.y = lhs.get_value(1, 0) * rhs.x + lhs.get_value(1, 1) * rhs.y + lhs.get_value(1, 2) * rhs.z;
        result.z = lhs.get_value(2, 0) * rhs.x + lhs.get_value(2, 1) * rhs.y + lhs.get_value(2, 2) * rhs.z;
        return result;
    }

    std::ostream& operator<<(std::ostream& os, const Matrix3f& rhs)
    {
        for (unsigned i = 0; i < 3; ++i)
        {
            for (unsigned j = 0; j < 3; ++j)
            {
                if (const float value = rhs.get_value(i, j); std::abs(value) < 1e-3)
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