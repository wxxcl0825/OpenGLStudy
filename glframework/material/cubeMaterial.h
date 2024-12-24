#pragma once

#include "material.h"
#include "../texture.h"

class CubeMaterial : public Material {
public:
    CubeMaterial();
    ~CubeMaterial();
public:
    Texture* mDiffuse{nullptr};
};