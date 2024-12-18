#pragma once

#include "material.h"
#include "../texture.h"

class OpacityMaskMaterial : public Material {
public:
    OpacityMaskMaterial();
    ~OpacityMaskMaterial();

public:
    Texture *mDiffuse{nullptr};
    Texture *mOpacityMask{nullptr};
    float mShiness{1.0f};
};