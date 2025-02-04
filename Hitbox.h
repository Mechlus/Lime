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

#include "Compatible3D.h"

class Hitbox : public ISceneNode, public Compatible3D {
public:
    irr::scene::ITriangleSelector* selector;
    irr::scene::SMeshBuffer* meshBuffer;

    // Constructor
    Hitbox(ISceneNode* parent, ISceneManager* smgr, s32 id, f32 radius, f32 height)
        : ISceneNode(parent, smgr, id), meshBuffer(nullptr) {
        this->disregardVisibility = true; // Registers collision regardless of visibility
        reconstruct(radius, height);
    }

    virtual void OnRegisterSceneNode() override {
        SceneManager->registerNodeForRendering(this, scene::ESNRP_TRANSPARENT);
        ISceneNode::OnRegisterSceneNode();
    }

    virtual void render() override {
        if (!meshBuffer) return;
        video::IVideoDriver* driver = SceneManager->getVideoDriver();
        driver->setMaterial(meshBuffer->Material);
        driver->drawVertexPrimitiveList(meshBuffer->Vertices.pointer(), meshBuffer->Vertices.size(),
            meshBuffer->Indices.pointer(), meshBuffer->Indices.size() / 3,
            EVT_STANDARD, EPT_TRIANGLES, EIT_16BIT);
    }

    virtual const aabbox3d<f32>& getBoundingBox() const override {
        return meshBuffer ? meshBuffer->BoundingBox : aabbox3d<f32>();
    }

    virtual u32 getMaterialCount() const override {
        return 1;
    }

    virtual SMaterial& getMaterial(u32 i) override {
        return meshBuffer ? meshBuffer->Material : *(new SMaterial());
    }

    void reconstruct(f32 radius, f32 height);

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

    void destroy() {
        if (meshBuffer)
            meshBuffer->drop();
        if (selector)
            selector->drop();
    }

    void reconstruct(f32 radius, f32 height);

    bool getCollision();
    void setCollision(bool enable); // React to raypicks etc.

    bool pointInside(const Vector3D& point);
    bool lineInside(const Vector3D& start, const Vector3D& end);

    sol::table getIntersectors(); // Returns table of structs containing the ID and team ID that are intersecting this hitbox.

    float resolution = 1.0f; // Resolution of spheric check for capsule collision
    float displayResolution = 1.0f; // Scalar resolution for debug view
    int teamID = 0; // Hitboxes with the same team ID will ignore one another
    bool ignoreHitboxes = false; // Set to true if this hitbox is for attack animations or any instance where this hitbox does not need to be interacted with by other hitboxes (sword swing would be true, body would be false)

    /*
    Hitboxes work as such:
    When created, they are added to a list under a root node that is iterated on update. These hitboxes then check amongst hitboxes of a different team ID along the length of the capsule
    if two hitboxes are overlapping. Starting with a simple AABB check, the hitboxes then follow up with Irrlicht's .getLengthSQ() function to see if a sphere check is within another capsule.
    Example: const f32 distanceSquared = (candidateCollisionPoint - ray.start).getLengthSQ();
    */

    // TO-DO: Add a sin/cos table for instant look up, much like Kaze's SM64 optimization so making all of the capsules is much faster.
private:
};