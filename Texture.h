#pragma once

#include <string>
#include "irrlicht.h"
#include "IrrManagers.h"
#include "Vector2D.h"

class Texture {
public:
    irr::video::ITexture* texture;
    std::string path;

    Texture();
    Texture(const std::string imgpath);

    bool load(const std::string& imgpath);
    std::string getPath() const;
    void keyColor(const Vector2D& pos);
    bool saveTexture(std::string path);
};

void bindTexture();