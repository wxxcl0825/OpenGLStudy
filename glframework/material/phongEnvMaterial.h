#pragma once

#include "material.h"
#include "../texture.h"

class PhongEnvMaterial : public Material {
public:
    PhongEnvMaterial();
    ~PhongEnvMaterial();

public:
    Texture *mDiffuse{nullptr};
    Texture *mSpecularMask{nullptr};
    Texture *mEnv{nullptr};
    float mShiness{1.0f};
};