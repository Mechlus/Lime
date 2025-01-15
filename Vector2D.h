#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include "LuaLime.h"

#include <string>
#include <cmath>
#define PI 3.14159265358979323846

struct Vector2D
{
public:
	float x, y;

	Vector2D() : x(0), y(0) {}
	Vector2D(float x, float y) : x(x), y(y) {}
    Vector2D(float x) : x(x), y(x) {}

    // add
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    // sub
    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    // mult
    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    // divide
    Vector2D operator/(float scalar) const {
        return Vector2D(x / scalar, y / scalar);
    }

    // length
    float length() const {
        return std::sqrt(x * x + y * y);
    }

    // normalize
    Vector2D normalize() const {
        return normalizeRange(0, 1);
    }

    // normalize to range
    Vector2D normalizeRange(float min = 0, float max = 0) const {
        float len = length();

        if (len > 0) {
            float normX = x / len;
            float normY = y / len;

            float scale = max - min;
            normX = normX * scale + min;
            normY = normY * scale + min;

            return Vector2D(normX, normY);
        }

        return Vector2D(0.0f, 0.0f);
    }

    // dot product
    float dot(const Vector2D& other) const {
        return (x * other.x) + (y * other.y);
    }

    // distance
    float distance(const Vector2D& other = Vector2D(0,0)) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    // rotate
    Vector2D rotate(float rad = 0) const {
        float cosTheta = std::cos(rad);
        float sinTheta = std::sin(rad);

        return Vector2D(
            x * cosTheta - y * sinTheta,
            x * sinTheta + y * cosTheta
        );
    }

    // angle between two vector2ds
    float angle(const Vector2D& other = Vector2D(0,0)) const {
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
    bool operator==(const Vector2D& other) const {
        return x == other.x && y == other.y;
    }

    // not equal
    bool operator!=(const Vector2D& other) const {
        return !(x == other.x && y == other.y);
    }

    // to string
    std::string toString() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
};

inline void bindVector2D() {
    sol::usertype<Vector2D> bind_type = lua->new_usertype<Vector2D>("Vector2D",
        sol::constructors<Vector2D(), Vector2D(float, float), Vector2D(float)>(),
        sol::meta_function::addition, &Vector2D::operator+,
        sol::meta_function::subtraction, &Vector2D::operator-,
        sol::meta_function::multiplication, &Vector2D::operator*,
        sol::meta_function::division, &Vector2D::operator/,
        sol::meta_function::equal_to, &Vector2D::operator==,
        
        "x", &Vector2D::x,
        "y", &Vector2D::y);
    
    bind_type["length"] = &Vector2D::length;
    bind_type["normalize"] = &Vector2D::normalize;
    bind_type["normalizeToRange"] = &Vector2D::normalizeRange;
    bind_type["dot"] = &Vector2D::dot;
    bind_type["distance"] = &Vector2D::distance;
    bind_type["angle"] = &Vector2D::angle;
    bind_type["rotate"] = &Vector2D::rotate;
    bind_type["toStr"] = &Vector2D::toString;
}