#include "MeshBuffer.h"

using namespace irr;
using namespace core;
using namespace scene;

MeshBuffer::MeshBuffer() {
	buffer = &irr::scene::SMeshBuffer();
}

MeshBuffer::~MeshBuffer() {
    currentIndex = 0;
	buffer->drop();
}

void MeshBuffer::pushFace(const Vector3D& v1, const Vector3D& v2, const Vector3D& v3,
                          const Vector3D& n1, const Vector3D& n2, const Vector3D& n3,
                          const Vector2D& uvw1, const Vector2D& uvw2, const Vector2D& uvw3,
                          const Vector3D& rgb, int opacity) {
    if (!buffer) return;

    buffer->Vertices.reallocate(buffer->Vertices.size() + 3);
    buffer->Vertices.set_used(buffer->Vertices.size() + 3);

    buffer->Vertices[currentIndex] = S3DVertex(v1, n1, c1, uv1);
    buffer->Vertices[currentIndex + 1] = S3DVertex(v2, n2, c2, uv2);
    buffer->Vertices[currentIndex + 2] = S3DVertex(v3, n3, c3, uv3);

    buffer->Indices.reallocate(buffer->Indices.size() + 3);
    buffer->Indices.set_used(buffer->Indices.size() + 3);

    buffer->Indices[currentIndex] = currentIndex;
    buffer->Indices[currentIndex + 1] = currentIndex + 1;
    buffer->Indices[currentIndex + 2] = currentIndex + 2;

    currentIndex += 3;
}

void MeshBuffer::clear() {
    if (buffer) {
        buffer->drop();
        currentIndex = 0;
        buffer = &irr::scene::SMeshBuffer();
    }
}

void MeshBuffer::destroy() {
    if (buffer) {
        buffer->drop();
        currentIndex = 0;
    }
}

void MeshBuffer::recalculateBoundingBox() {
    if (buffer) buffer->recalculateBoundingBox();
}

irr::scene::SMeshBuffer* MeshBuffer::getBuffer() {
	return buffer;
}

