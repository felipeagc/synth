#pragma once

#include <math.h>

struct Vec3
{
    union
    {
        struct
        {
            float x, y, z;
        };
        float v[3];
    };

    Vec3()
    {
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
    }

    Vec3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float &operator[](size_t index)
    {
        return this->v[index];
    }

    const float &operator[](size_t index) const
    {
        return this->v[index];
    }

    Vec3 operator+(const Vec3 &rhs)
    {
        return Vec3(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
    }

    void operator+=(const Vec3 &rhs)
    {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
    }
};

static float dot(const Vec3 &lhs, const Vec3 &rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
