#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "DrawSphere.h"
#include <string>
#include <vector>

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;

class Hitbox : public ISceneNode {
public:
    // Constructor
    Hitbox(ISceneNode* parent, ISceneManager* smgr, s32 id)
        : ISceneNode(parent, smgr, id) {
    }

    virtual void OnRegisterSceneNode() override {
        SceneManager->registerNodeForRendering(this, scene::ESNRP_TRANSPARENT);
        ISceneNode::OnRegisterSceneNode();
    }

    virtual void render() override {

    }

    // Access bounding box (required override)
    virtual const aabbox3d<f32>& getBoundingBox() const override {
        static aabbox3d<f32> dummyBox(vector3df(-1, -1, -1), vector3df(1, 1, 1));
        return dummyBox;
    }

    virtual u32 getMaterialCount() const override {
        return 0;
    }

    virtual SMaterial& getMaterial(u32 i) override {
        static SMaterial dummyMaterial;
        return dummyMaterial;
    }

    bool getActive() const;
    void setActive(bool visible);

    bool getVisibility() const;
    void setVisibility(bool visible);

    Vector3D getPosition();
    void setPosition(const Vector3D& pos);

    Vector3D getRotation();
    void setRotation(const Vector3D& rot);

    Vector3D getScale();
    void setScale(const Vector3D& scale);

    Vector3D getColor();
    void setColor(const Vector3D& color);

    int getID();
    void setID(int i);

private:
};