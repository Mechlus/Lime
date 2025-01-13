#pragma once

#include "irrlicht.h"
#include "IrrManagers.h"
#include "Texture.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "LuaLime.h"

class Material
{
public:
	irr::video::SMaterial mat;

    Material() {
        mat.UseMipMaps = false;
    }

	Material(const Material& other) {
		mat = other.mat;
	}
    Material(Texture& tex) {
        setTexture(tex, 0);
    }

	int getAntiAliasing() {
		return mat.AntiAliasing;
	}

	void setAntiAliasing(int i) {
		mat.AntiAliasing = i;
	}

	int getMaterialType() {
		return mat.MaterialType;
	}

	void setMaterialType(int i) {
        mat.MaterialType = (irr::video::E_MATERIAL_TYPE)i;
	}

    bool getFog() {
        return mat.FogEnable;
    }

    void setFog(bool i) {
        mat.FogEnable = i;
    }

    bool getBackface() {
        return mat.BackfaceCulling;
    }

    void setBackface(bool i) {
        mat.BackfaceCulling = i;
    }

    bool getFrontface() {
        return mat.FrontfaceCulling;
    }

    void setFrontface(bool i) {
        mat.FrontfaceCulling = i;
    }

    bool getWireframe() {
        return mat.Wireframe;
    }

    void setWireframe(bool i) {
        mat.Wireframe = i;
    }

    bool getLighting() {
        return mat.Lighting;
    }

    void setLighting(bool i) {
        //mat.Lighting = i; Deprecated for xEffects
    }

    Vector3D getDiffuseColor() {
        return Vector3D(mat.DiffuseColor.getRed(), mat.DiffuseColor.getGreen(), mat.DiffuseColor.getBlue());
    }

    void setDiffuseColor(Vector3D& color) {
        mat.DiffuseColor.set(1, color.x / 255.0f, color.y / 255.0f, color.z / 255.0f);
    }

    Vector3D getAmbientColor() {
        return Vector3D(mat.AmbientColor.getRed(), mat.AmbientColor.getGreen(), mat.AmbientColor.getBlue());
    }

    void setAmbientColor(Vector3D& color) {
        mat.AmbientColor.set(1, color.x/255.0f, color.y/255.0f, color.z/255.0f);
    }

    Vector3D getEmissiveColor() {
        return Vector3D(mat.EmissiveColor.getRed(), mat.EmissiveColor.getGreen(), mat.EmissiveColor.getBlue());
    }

    void setEmissiveColor(Vector3D& color) {
        mat.EmissiveColor.set(1, color.x / 255.0f, color.y / 255.0f, color.z / 255.0f);
    }

    Vector3D getSpecularColor() {
        return Vector3D(mat.SpecularColor.getRed(), mat.SpecularColor.getGreen(), mat.SpecularColor.getBlue());
    }

    void setSpecularColor(Vector3D& color) {
        mat.SpecularColor.set(1, color.x / 255.0f, color.y / 255.0f, color.z / 255.0f);
    }

    bool getGourad() {
        return mat.GouraudShading;
    }

    void setGourad(bool i) {
        mat.GouraudShading = i;
    }

    bool getZWrite() {
        return mat.ZWriteEnable;
    }

    void setZWrite(bool i) {
        mat.ZWriteEnable = i;
    }

    bool getPointCloud() {
        return mat.PointCloud;
    }

    void setPointCloud(bool i) {
        mat.PointCloud = i;
    }

    float getShine() {
        return mat.Shininess;
    }

    void setShine(float i) {
        mat.Shininess = i;
    }

    bool getBiFilter() {
        return mat.getFlag(irr::video::EMF_BILINEAR_FILTER);
    }

    void setBiFilter(bool i) {
        mat.setFlag(irr::video::EMF_BILINEAR_FILTER, i);
    }

    bool getTriFilter() {
        return mat.getFlag(irr::video::EMF_TRILINEAR_FILTER);
    }

    void setTriFilter(bool i) {
        mat.setFlag(irr::video::EMF_TRILINEAR_FILTER, i);
    }

    bool getAnsiFilter() {
        return mat.getFlag(irr::video::EMF_ANISOTROPIC_FILTER);
    }

    void setAnsiFilter(bool i) {
        mat.setFlag(irr::video::EMF_ANISOTROPIC_FILTER, i);
    }

    Vector2D getPan(int i) {
        if (i < 0 || i >= irr::video::MATERIAL_MAX_TEXTURES) {
            return Vector2D();
        }
        if (mat.getTexture(i) == nullptr) {
            return Vector2D();
        }

        auto& matT = mat.getTextureMatrix(i);
        return Vector2D(matT.getTranslation().X, matT.getTranslation().Y);
    }

    void setPan(Vector2D scroll, int i)
    {
        if (i < 0 || i >= irr::video::MATERIAL_MAX_TEXTURES)
            return;

        auto texture = mat.getTexture(i);
        if (!texture)
            return;

        irr::core::matrix4 matT = mat.getTextureMatrix(i);
        matT.setTextureTranslate(scroll.x, scroll.y);
        mat.setTextureMatrix(i, matT);
    }

    int getWrapU(int layer) {
        if (layer >= 0 && layer < irr::video::MATERIAL_MAX_TEXTURES && mat.getTexture(layer) != nullptr) {
            return mat.TextureLayer[layer].TextureWrapU;
        }
        return 0;
    }

    void setWrapU(int layer, int i) {
        if (layer >= 0 && layer < irr::video::MATERIAL_MAX_TEXTURES && mat.getTexture(layer) != nullptr) {
            mat.TextureLayer[layer].TextureWrapU = i;
        }
    }

    int getWrapV(int layer) {
        if (layer >= 0 && layer < irr::video::MATERIAL_MAX_TEXTURES && mat.getTexture(layer) != nullptr) {
            return mat.TextureLayer[layer].TextureWrapV;
        }
        return 0;
    }

    void setWrapV(int layer, int i) {
        if (layer >= 0 && layer < irr::video::MATERIAL_MAX_TEXTURES && mat.getTexture(layer) != nullptr) {
            mat.TextureLayer[layer].TextureWrapV = i;
        }
    }

    int getWrapUV(int layer) {
        if (layer >= 0 && layer < irr::video::MATERIAL_MAX_TEXTURES && mat.getTexture(layer) != nullptr) {
            return mat.TextureLayer[layer].TextureWrapU; // Just gives back U, not both U and V. Is this a problem?
        }
        return 0;
    }

    void setWrapUV(int layer, int i) {
        if (layer >= 0 && layer < irr::video::MATERIAL_MAX_TEXTURES && mat.getTexture(layer) != nullptr) {
            mat.TextureLayer[layer].TextureWrapU = i;
            mat.TextureLayer[layer].TextureWrapV = i;
        }
    }

    int getOpacity() {
        return mat.DiffuseColor.getAlpha();
    }

    void setOpacity(int i) {
        if (i < 0) {
            i = 0;
        } else if(i > 255) {
            i = 255;
        }

        mat.DiffuseColor.setAlpha(i);
        mat.AmbientColor.setAlpha(i);
        mat.EmissiveColor.setAlpha(i);
        mat.SpecularColor.setAlpha(i);
        // not entirely sure if this works so test this
    }

    bool getMipMaps() {
        return mat.UseMipMaps;
    }

    void setMipMaps(bool i) {
        mat.UseMipMaps = i;
    }

    int getZComparison() {
        return mat.ZBuffer;
    }

    void setZComparison(int i) {
        mat.ZBuffer = i;
    }

    void setTexture(Texture& tex, int slot) {
        if (tex.texture) {
            mat.setTexture(slot, tex.texture);
        }
    }

    bool getTexture(int slot) {
        // get texture path at slot or return new Texture obj?
        return false;
    }

    int getID() {
        return mat.ID;
    }

    void setID(int i) {
        mat.ID = i;
    }

    void setTextureScale(Vector2D scale, int i) {
        if (i < 0 || i >= irr::video::MATERIAL_MAX_TEXTURES)
            return;

        auto texture = mat.getTexture(i);
        if (!texture)
            return;

        irr::core::matrix4 matT = mat.getTextureMatrix(i);
        matT.setTextureScale(scale.x, scale.y);
        mat.setTextureMatrix(i, matT);
    }

    Vector2D getTextureScale(int i) {
        // no function for this in irrlicht yet
        return Vector2D();
    }

    void setMaterialFlag(int i, bool enable) {
        mat.setFlag((irr::video::E_MATERIAL_FLAG)i, enable);
    }
};

void bindMaterial() {
    sol::usertype<Material> bind_type = lua->new_usertype<Material>("Material",
		sol::constructors<Material(), Material(const Texture& tex), Material(const Material& other)>(),

		"type", sol::property(&Material::getMaterialType, &Material::setMaterialType),
        "fog", sol::property(&Material::getFog, &Material::setFog),
        "backfaceCulling", sol::property(&Material::getBackface, &Material::setBackface),
        "frontfaceCulling", sol::property(&Material::getFrontface, &Material::setFrontface),
		"antiAliasing", sol::property(&Material::getAntiAliasing, &Material::setAntiAliasing),
        "wireframe", sol::property(&Material::getWireframe, &Material::setWireframe),
        "lighting", sol::property(&Material::getLighting, &Material::setLighting),
        "diffuseColor", sol::property(&Material::getDiffuseColor, &Material::setDiffuseColor),
        //"ambientColor", sol::property(&Material::getAmbientColor, &Material::setAmbientColor), Makes all lighting and colors not work?
        "emissiveColor", sol::property(&Material::getEmissiveColor, &Material::setEmissiveColor),
        "specularColor", sol::property(&Material::getSpecularColor, &Material::setSpecularColor),
        "gouradShading", sol::property(&Material::getGourad, &Material::setGourad),
        "zWrite", sol::property(&Material::getZWrite, &Material::setZWrite),
        "zComparison", sol::property(&Material::getZComparison, &Material::setZComparison),
        "pointCloud", sol::property(&Material::getPointCloud, &Material::setPointCloud),
        "bilinearFiltering", sol::property(&Material::getBiFilter, &Material::setBiFilter),
        "trilinearFiltering", sol::property(&Material::getTriFilter, &Material::setTriFilter),
        "anisotropicFiltering", sol::property(&Material::getAnsiFilter, &Material::setAnsiFilter),
        "shininess", sol::property(&Material::getShine, &Material::setShine),
        "opacity", sol::property(&Material::getOpacity, &Material::setOpacity),
        "mipmaps", sol::property(&Material::getMipMaps, &Material::setMipMaps),
        "textureWrap", sol::property(&Material::getWrapUV, &Material::setWrapUV),
        "textureWrapU", sol::property(&Material::getWrapU, &Material::setWrapU),
        "textureWrapV", sol::property(&Material::getWrapV, &Material::setWrapV),
        "ID", sol::property(&Material::getID, &Material::setID)
    );

    bind_type["getTextureTranslation"] = &Material::getPan;
    bind_type["setTextureTranslation"] = &Material::setPan;
    bind_type["setTextureUVWrapping"] = &Material::setWrapUV;
    bind_type["setTextureUWrapping"] = &Material::setWrapU;
    bind_type["setTextureVWrapping"] = &Material::setWrapV;
    bind_type["setTexture"] = &Material::setTexture;

    bind_type["setTextureScale"] = &Material::setTextureScale;
    bind_type["getTextureScale"] = &Material::setTextureScale;

    bind_type["setMaterialFlag"] = &Material::setMaterialFlag;
}