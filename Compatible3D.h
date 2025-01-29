#ifndef COMPATIBLE3D_H
#define COMPATIBLE3D_H

#include <irrlicht.h>
#include <sol/sol.hpp>

class Compatible3D {
public:
    virtual ~Compatible3D() = default;

    virtual irr::scene::ISceneNode* getNode() const = 0;

    void setParent(sol::optional<Compatible3D*> parent) {
        irr::scene::ISceneNode* node = getNode();
        if (!node) return;

        node->setParent(*parent ? (*parent)->getNode() : nullptr);
    }
};

void bindCompatible3D() {
    sol::usertype<Compatible3D> bind_type = lua->new_usertype<Compatible3D>("Compatible3D");
    bind_type["setParent"] = &Compatible3D::setParent;
}

#endif // COMPATIBLE3D_H
