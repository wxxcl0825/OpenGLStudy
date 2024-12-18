#pragma once

#include "material.h"
#include "../texture.h"

class ScreenMaterial : public Material {
public:
    ScreenMaterial();
    ~ScreenMaterial();

    Texture* mScreenTexture{nullptr};
};