#pragma once
#include <string>
#include <cmath>

#define PI 3.14159265358979323846

class Vector3D
{
public:
    float x, y, z;

    Vector3D() : x(0), y(0), z(0) {}
    Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}

    // add
    Vector3D operator+(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }

    // sub
    Vector3D operator-(const Vector3D& other) const {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    // mult
    Vector3D operator*(float scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    // divide
    Vector3D operator/(float scalar) const {
        return Vector3D(x / scalar, y / scalar, z / scalar);
    }

    // length
    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // normalize
    Vector3D normalize() const {
        return normalizeRange(0, 1);
    }

    // normalize to range
    Vector3D normalizeRange(float min, float max) const {
        float len = length();

        if (len > 0) {
            float normX = x / len;
            float normY = y / len;
            float normZ = z / len;

            float scale = max - min;
            normX = normX * scale + min;
            normY = normY * scale + min;
            normZ = normZ * scale + min;

            return Vector3D(normX, normY, normZ);
        }

        return Vector3D(0.0f, 0.0f, 0.0f);
    }

    // dot product
    float dot(const Vector3D& other = Vector3D(0,0,0)) const {
        return (x * other.x) + (y * other.y) + (z * other.z);
    }

    // distance
    float distance(const Vector3D& other = Vector3D(0,0,0)) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }

    // rotate
    Vector3D rotate(const Vector3D& rot = Vector3D(0,0,0)) const {
        // rotation is a vector3d of radian angles
        // X
        float cosX = std::cos(rot.x);
        float sinX = std::sin(rot.x);
        float newY = y * cosX - z * sinX;
        float newZ = y * sinX + z * cosX;

        // Y
        float cosY = std::cos(rot.y);
        float sinY = std::sin(rot.y);
        float newX = x * cosY + newZ * sinY;
        newZ = -x * sinY + newZ * cosY;

        // Z
        float cosZ = std::cos(rot.z);
        float sinZ = std::sin(rot.z);
        float finalX = newX * cosZ - newY * sinZ;
        float finalY = newX * sinZ + newY * cosZ;

        // uses euler angle approach, prone to gimbal lock
        return Vector3D(finalX, finalY, newZ);
    }

    // convert vector3d to degrees
    Vector3D deg() const {
        return Vector3D(x * 180 / PI, y * 180 / PI, z * 180 / PI);
    }

    // convert vector3d to radians
    Vector3D rad() const {
        return Vector3D(x * PI / 180, y * PI / 180, z * PI / 180);
    }

    // angle between two vector3ds
    float angle(const Vector3D& other = Vector3D(0,0,0)) const {
        float thisDot = dot(other);
        float lenProduct = length() * other.length();

        if (lenProduct == 0) {
            return 0.0f;
        }

        float cosTheta = thisDot / lenProduct;
        cosTheta = std::fmax(-1.0f, std::fmin(1.0f, cosTheta));
        float angleRad = std::acos(cosTheta);

        return angleRad;
    }

    // equal
    bool operator==(const Vector3D& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    // not equal
    bool operator!=(const Vector3D& other) const {
        return !operator==(other);
    }

    // to string
    std::string toString() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }
};

inline void bindVector3D() {
    sol::usertype<Vector3D> bind_type = lua->new_usertype<Vector3D>("Vector3D",
        sol::constructors<Vector3D(), Vector3D(float, float, float)>(),
        sol::meta_function::addition, &Vector3D::operator+,
        sol::meta_function::subtraction, &Vector3D::operator-,
        sol::meta_function::multiplication, &Vector3D::operator*,
        sol::meta_function::division, &Vector3D::operator/,
        sol::meta_function::equal_to, &Vector3D::operator==,

        "x", &Vector3D::x,
        "y", &Vector3D::y,
        "z", &Vector3D::z
    ); 

    bind_type["length"] = &Vector3D::length;
    bind_type["normalize"] = &Vector3D::normalize;
    bind_type["normalizeToRange"] = &Vector3D::normalizeRange;
    bind_type["dot"] = &Vector3D::dot;
    bind_type["distance"] = &Vector3D::distance;
    bind_type["toDegrees"] = &Vector3D::deg;
    bind_type["toRadians"] = &Vector3D::rad;
    bind_type["rotate"] = &Vector3D::rotate;
    bind_type["angle"] = &Vector3D::angle;
    bind_type["toStr"] = &Vector3D::toString;
}