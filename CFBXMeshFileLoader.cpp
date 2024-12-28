#include "CFBXMeshFileLoader.h"
#include <SMesh.h>
#include <iostream>

namespace irr {
    namespace scene {

        CFBXMeshFileLoader::CFBXMeshFileLoader(ISceneManager* smgr)
            : sceneManager(smgr) {}

        CFBXMeshFileLoader::~CFBXMeshFileLoader() {}

        bool CFBXMeshFileLoader::isALoadableFileExtension(const io::path& filename) const {
            return filename.endsWith(".fbx"); // Only FBX files are supported
        }

        IAnimatedMesh* CFBXMeshFileLoader::createMesh(io::IReadFile* file) {
            if (!file) {
                std::cerr << "Error: Null file pointer." << std::endl;
                return nullptr;
            }

            // Load file into memory
            std::vector<char> buffer(file->getSize());
            file->read(buffer.data(), buffer.size());

            // Parse FBX with Assimp
            const aiScene* scene = importer.ReadFileFromMemory(
                buffer.data(), buffer.size(),
                aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices
            );

            if (!scene) {
                std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
                return nullptr;
            }

            return convertAssimpMesh(scene);
        }

        IAnimatedMesh* CFBXMeshFileLoader::convertAssimpMesh(const aiScene* scene) {
            SMesh* irrMesh = new SMesh();

            for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
                aiMesh* mesh = scene->mMeshes[i];
                SMeshBuffer* buffer = new SMeshBuffer();

                // Convert vertices
                for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
                    buffer->Vertices.push_back(video::S3DVertex(
                        mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z,
                        0, 1, 0, video::SColor(255, 255, 255, 255),
                        mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][j].x : 0,
                        mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][j].y : 0
                    ));
                }

                // Convert indices
                for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
                    aiFace& face = mesh->mFaces[j];
                    if (face.mNumIndices == 3) { // Triangles only
                        buffer->Indices.push_back(face.mIndices[0]);
                        buffer->Indices.push_back(face.mIndices[1]);
                        buffer->Indices.push_back(face.mIndices[2]);
                    }
                }

                buffer->recalculateBoundingBox();
                irrMesh->addMeshBuffer(buffer);
                buffer->drop();
            }

            irrMesh->recalculateBoundingBox();
            return irrMesh;
        }

    } // namespace scene
} // namespace irr
