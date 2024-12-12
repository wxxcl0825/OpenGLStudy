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

protected:
    MaterialType mType;
};