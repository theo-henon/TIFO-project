#pragma once

#include <array>

#include <ostream>

#include "vector3f.hh"
#include "matrix3f.hh"

namespace tifo
{
    class Matrix4f
    {
    public:
        Matrix4f();
        Matrix4f(const Matrix3f& matrix3f);

        float get_value(unsigned row, unsigned col) const;

        void set_value(unsigned row, unsigned col, float value);

        const std::array<float, 16>& get_data() const;

        const float* get_data_ptr() const;

        Matrix4f transpose() const;

        static Matrix4f identity();

        static Matrix4f diagonal(float d1, float d2, float d3, float d4);

        static Matrix4f translate(float x, float y, float z);

        static Matrix4f translate(const Vector3f& vec);

        static Matrix4f rotate(float rx, float ry, float rz);

        static Matrix4f rotate(const Vector3f& vec);

        Matrix4f& operator*=(const Matrix4f& rhs);

        friend Matrix4f operator*(const Matrix4f& lhs, const Matrix4f& rhs);

        friend Vector3f operator*(const Matrix4f& lhs, const Vector3f& rhs);

        friend std::ostream& operator<<(std::ostream& os, const Matrix4f& rhs);

    private:
        std::array<float, 16> data_;
    };
} // namespace tifo