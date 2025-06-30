#pragma once

#include <ostream>

namespace tifo
{
    class Vector3f
    {
    public:
        float x;
        float y;
        float z;

        Vector3f(float x, float y, float z);

        Vector3f();

        Vector3f(const Vector3f& other) = default;

        ~Vector3f() = default;

        Vector3f& operator+=(const Vector3f& rhs);

        Vector3f& operator-=(const Vector3f& rhs);

        Vector3f& operator*=(float rhs);

        friend std::ostream& operator<<(std::ostream& lhs, const Vector3f& rhs);

        float dot(const Vector3f& rhs) const;

        float magnitude() const;

        Vector3f normalize() const;

        Vector3f cross(const Vector3f& rhs) const;

        Vector3f hadamard(const Vector3f& rhs) const;
    };

    Vector3f operator+(const Vector3f& lhs, const Vector3f& rhs);

    Vector3f operator*(const Vector3f& lhs, float rhs);

    Vector3f operator*(float lhs, const Vector3f& rhs);

    Vector3f operator-(const Vector3f& lhs, const Vector3f& rhs);

    Vector3f operator-(const Vector3f& rhs);
} // namespace tifo