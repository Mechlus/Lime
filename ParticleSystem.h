#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "Vector2D.h"
#include "StaticMesh.h"
#include "Texture.h"
#include <string>
#include <vector>

#include "Compatible3D.h"

class ParticleSystem : public Compatible3D {
public:
    irr::scene::IParticleSystemSceneNode* ps;

    ParticleSystem();

    Vector3D getPosition();
    void setPosition(const Vector3D& pos);

    Vector3D getRotation();
    void setRotation(const Vector3D& rot);

    Vector3D getScale();
    void setScale(const Vector3D& scale);

    void setEmitter(int i, sol::table params);
    void addAffector(int i, sol::table params);
    void removeAffectors();
    void removeParticles();

    void setParticleGlobalBehavior(bool track);

    bool getActive();
    void setActive(bool active);

    void setMaterialTexture(const Texture& tex);
    void setMaterialFlag(int i, bool enable);
    void setMaterialType(int i);

    bool getDebug();
    void setDebug(bool visible);

    bool getVisible();
    void setVisible(bool enable);

    void destroy();

    irr::scene::ISceneNode* getNode() const override { return ps; }
};

void bindParticleSystem();