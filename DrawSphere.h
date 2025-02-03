#pragma once
#include "irrlicht.h"
#include <string>
#include <vector>

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;

// Drawing debug sphere
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923

inline void genSphere(vector3df center, std::vector<S3DVertex>& vertices, std::vector<u16>& indices, float radius, u32 rings, u32 sectors, SColor col) {
    float const R = 1.0f / (rings - 1);
    float const S = 1.0f / (sectors - 1);

    for (u32 r = 0; r < rings; ++r) {
        for (u32 s = 0; s < sectors; ++s) {
            float y = sin(-M_PI_2 + M_PI * r * R);
            float x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
            float z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

            vector3df position = vector3df(x * radius, y * radius, z * radius) + center;

            vertices.push_back(S3DVertex(position, vector3df(x, y, z), col, vector2df(s * S, r * R)));
        }
    }

    for (u32 r = 0; r < rings - 1; ++r) {
        for (u32 s = 0; s < sectors - 1; ++s) {
            u16 i1 = r * sectors + s;
            u16 i2 = r * sectors + (s + 1);
            u16 i3 = (r + 1) * sectors + s;

            indices.push_back(i1); indices.push_back(i2);
            indices.push_back(i1); indices.push_back(i3);
        }
    }
}

inline void renderSphere(vector3df pos, float rad = 5.0f, u32 rings = 20, u32 sectors = 20, SColor col = SColor()) {
    std::vector<S3DVertex> vertices;
    std::vector<u16> indices;

    genSphere(pos, vertices, indices, rad, rings, sectors, col);

    driver->drawVertexPrimitiveList(vertices.data(), vertices.size(),
        indices.data(), indices.size() / 2,
        EVT_STANDARD, EPT_LINES, EIT_16BIT);
}

