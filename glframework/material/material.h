#pragma once

#include "../core.h"

enum class MaterialType {
    PhongMaterial, WhiteMaterial, DepthMaterial
};

class Renderer;

class Material {
friend Renderer;
public:
    Material();
    ~Material();

    // 深度检测
    bool mDepthTest{ true };
    GLenum mDepthFunc{ GL_LESS };
    bool mDepthWrite { true };

    bool mPolygonOffset{false};
    unsigned int mPolygonOffsetType{GL_POLYGON_OFFSET_FILL};
    float mFactor{0.0f};
    float mUnit{0.0f};

protected:
    MaterialType mType;
};