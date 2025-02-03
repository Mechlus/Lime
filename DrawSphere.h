#pragma once
#include "irrlicht.h"
#include <vector>

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;

#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923

inline void genSphere(vector3df center, std::vector<S3DVertex>& vertices, std::vector<u16>& indices,
    float radius, float height, u32 rings, u32 sectors, SColor col) {
    float const R = 1.0f / (rings - 1);
    float const S = 1.0f / (sectors - 1);
    float halfHeight = height * 0.5f;
    u32 baseIndex = vertices.size();

    for (u32 r = 0; r < rings / 2; ++r) {
        for (u32 s = 0; s < sectors; ++s) {
            float y = -sin(-M_PI_2 + M_PI * r * R);
            float x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
            float z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

            vector3df position = vector3df(x * radius, (y * radius) + halfHeight, z * radius) + center;
            vertices.push_back(S3DVertex(position, vector3df(x, y, z), col, vector2df(s * S, r * R)));
        }
    }

    for (u32 r = 0; r <= 1; ++r) {
        for (u32 s = 0; s < sectors; ++s) {
            float y = (r == 0) ? halfHeight : -halfHeight;
            float x = cos(2 * M_PI * s * S) * radius;
            float z = sin(2 * M_PI * s * S) * radius;

            vector3df position = vector3df(x, y, z) + center;
            vertices.push_back(S3DVertex(position, vector3df(x, 0, z), col, vector2df(s * S, r * R)));
        }
    }

    for (u32 r = 0; r < rings / 2; ++r) {
        for (u32 s = 0; s < sectors; ++s) {
            float y = sin(-M_PI_2 + M_PI * r * R);
            float x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
            float z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

            vector3df position = vector3df(x * radius, (y * radius) - halfHeight, z * radius) + center;
            vertices.push_back(S3DVertex(position, vector3df(x, y, z), col, vector2df(s * S, r * R)));
        }
    }

    u32 ringCount = rings + 2;
    for (u32 r = 0; r < ringCount - 1; ++r) {
        for (u32 s = 0; s < sectors - 1; ++s) {
            u16 i1 = baseIndex + r * sectors + s;
            u16 i2 = baseIndex + r * sectors + (s + 1) % sectors;
            u16 i3 = baseIndex + (r + 1) * sectors + s;

            indices.push_back(i1); indices.push_back(i2);
            indices.push_back(i1); indices.push_back(i3);
        }
    }
}

inline void renderSphere(vector3df pos, float rad = 5.0f, float height = 5.0f, u32 rings = 20, u32 sectors = 20, SColor col = SColor()) {
    std::vector<S3DVertex> vertices;
    std::vector<u16> indices;

    genSphere(pos, vertices, indices, rad, height, rings, sectors, col);

    driver->drawVertexPrimitiveList(vertices.data(), vertices.size(),
        indices.data(), indices.size() / 2,
        EVT_STANDARD, EPT_LINES, EIT_16BIT);
}