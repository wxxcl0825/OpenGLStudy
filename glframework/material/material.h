#pragma once

#include "../core.h"

enum class MaterialType {
    PhongMaterial, WhiteMaterial
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

protected:
    MaterialType mType;
};