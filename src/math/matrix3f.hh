#pragma once

#include <array>
#include <ostream>

#include "vector3f.hh"

namespace tifo
{
    class Matrix3f
    {
    public:
        Matrix3f();

        float get_value(unsigned row, unsigned col) const;

        void set_value(unsigned row, unsigned col, float value);

        const std::array<float, 9>& get_data() const;

        const float* get_data_ptr() const;

        Matrix3f transpose() const;

        static Matrix3f diagonal(float d1, float d2, float d3);

        static Matrix3f identity();

        static Matrix3f rotate(float rx, float ry, float rz);

        static Matrix3f rotate(const Vector3f& vec);

        Matrix3f& operator*=(const Matrix3f& rhs);

        friend Matrix3f operator*(const Matrix3f& lhs, const Matrix3f& rhs);

        friend Vector3f operator*(const Matrix3f& lhs, const Vector3f& rhs);

        friend std::ostream& operator<<(std::ostream& os, const Matrix3f& rhs);

    private:
        std::array<float, 9> data_;
    };
} // namespace tifo
