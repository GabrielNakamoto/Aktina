#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

template<typename T>
class vec3
{
public:
    T x, y, z;

    vec3<T>()  : x(), y(), z() {};
    explicit vec3<T>(T s)  : x(s), y(s), z(s) {};
    vec3<T>(T x, T y, T z)  : x(x), y(y), z(z) {};
    vec3<T>(const vec3<T> &v)  : x(v.x), y(v.y), z(v.z) {};

    vec3 operator-() const
    {
        return vec3( -x, -y, -z );
    }

    vec3<T>& operator+=(const vec3<T> &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;

        return *this;
    }

    vec3<T>& operator*=(T s)
    {
        x *= s;
        y *= s;
        z *= s;

        return *this;
    }

    double length() const
    {
        return std::sqrt(dot(*this,*this));
    }

    vec3<T>& normalize()
    {
        double len = this->length();

        if(len == 0) return *this;

        double inv_len = 1.0 / len;

        return (*this *= inv_len);
    }
};

template<typename T>
inline float dot(const vec3<T> &l, const vec3<T> &r)
{
    return      l.x * r.x +
                l.y * r.y +
                l.z * r.z;
}


template<typename T>
inline vec3<T> operator+(const vec3<T> &l, const vec3<T> &r)
{
    return vec3(l.x + r.x,
                l.y + r.y,
                l.z + r.z);
}

template<typename T>
inline vec3<T> operator-(const vec3<T> &l, const vec3<T> &r)
{
    return vec3(l.x - r.x,
                l.y - r.y,
                l.z - r.z);
}

template<typename T>
inline vec3<T> operator*(const vec3<T> &l, const vec3<T> &r)
{
    return vec3(l.x * r.x,
                l.y * r.y,
                l.z * r.z);
}

template<typename T>
inline vec3<T> operator/(const vec3<T> &v, float s)
{
    return vec3(v.x / s,
                v.y / s,
                v.z / s);
}

template<typename T>
inline vec3<T> operator/(float s, const vec3<T> &v)
{
    return v / s;
}

template<typename T>
inline vec3<T> operator*(const vec3<T> &v, float s)
{
    return vec3(v.x * s,
                v.y * s,
                v.z * s );
}

template<typename T>
inline vec3<T> operator*(float s, const vec3<T> &v)
{
    return v * s;
}

template<typename T>
inline vec3<T> normalized(const vec3<T> &v)
{
    vec3<T> ret(v);

    double len = ret.length();

    if(len == 0) return ret;

    double inv_len = 1. / len;

    return (ret *= inv_len);
}


typedef vec3<float> vec3f;
typedef vec3<int> vec3i;

#endif
