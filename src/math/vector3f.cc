#include "vector3f.hh"

#include <cmath>

namespace tifo
{
    Vector3f::Vector3f()
        : Vector3f(0, 0, 0)
    {}

    Vector3f::Vector3f(float x, float y, float z)
        : x(x)
        , y(y)
        , z(z)
    {}

    Vector3f& Vector3f::operator+=(const Vector3f& rhs)
    {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        return *this;
    }

    Vector3f& Vector3f::operator-=(const Vector3f& rhs)
    {
        this->x -= rhs.x;
        this->y -= rhs.y;
        this->z -= rhs.z;
        return *this;
    }

    Vector3f& Vector3f::operator*=(float rhs)
    {
        this->x *= rhs;
        this->y *= rhs;
        this->z *= rhs;
        return *this;
    }

    float Vector3f::dot(const Vector3f& rhs) const
    {
        return this->x * rhs.x + this->y * y + this->z * rhs.z;
    }

    float Vector3f::magnitude() const
    {
        return std::sqrt(this->dot(*this));
    }

    Vector3f Vector3f::normalize() const
    {
        return *this * (1 / this->magnitude());
    }

    Vector3f Vector3f::cross(const Vector3f& rhs) const
    {
        return { this->y * rhs.z - this->z * rhs.y, this->z * rhs.x - this->x * rhs.z,
                 this->x * rhs.y - this->y * rhs.x };
    }

    Vector3f Vector3f::hadamard(const Vector3f& rhs) const
    {
        return { this->x * rhs.x, this->y * rhs.y, this->z * rhs.z };
    }

    std::ostream& operator<<(std::ostream& lhs, const Vector3f& rhs)
    {
        return lhs << "Vector3f(" << rhs.x << ";" << rhs.y << ";" << rhs.z << ")";
    }

    Vector3f operator+(const Vector3f& lhs, const Vector3f& rhs)
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
    }

    Vector3f operator*(const Vector3f& lhs, float rhs)
    {
        return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
    }

    Vector3f operator*(float lhs, const Vector3f& rhs)
    {
        return rhs * lhs;
    }

    Vector3f operator-(const Vector3f& lhs, const Vector3f& rhs)
    {
        return lhs + (-rhs);
    }

    Vector3f operator-(const Vector3f& rhs)
    {
        return (-1) * rhs;
    }
} // namespace tifo