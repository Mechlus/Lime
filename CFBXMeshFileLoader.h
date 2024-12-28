#ifndef C_FBX_MESH_FILE_LOADER_H_INCLUDED
#define C_FBX_MESH_FILE_LOADER_H_INCLUDED

#include <IMeshLoader.h>
#include <ISceneManager.h>
#include "IrrAssimp.h"

namespace irr {
    namespace scene {

        class CFBXMeshFileLoader : public IMeshLoader {
        public:
            CFBXMeshFileLoader(ISceneManager* smgr, IrrAssimpImport* assimpImporter);
            CFBXMeshFileLoader(ISceneManager* smgr);
            virtual ~CFBXMeshFileLoader();

            // Overrides for IMeshLoader
            virtual bool isALoadableFileExtension(const io::path& filename) const override;
            virtual IAnimatedMesh* createMesh(io::IReadFile* file) override;

        private:
            ISceneManager* sceneManager;
            IrrAssimpImport* assimpImporter;
        };

    } // namespace scene
} // namespace irr

#endif // C_FBX_MESH_FILE_LOADER_H_INCLUDED
