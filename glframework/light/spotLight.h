#pragma once

#include "light.h"
#include "../object.h"

class SpotLight : public Light, public Object {
public:
    SpotLight();
    ~SpotLight();

    glm::vec3 mTargetDirection{-1.0f};
    float mInnerAngle{0.0f};
    float mOuterAngle{0.0f};
};