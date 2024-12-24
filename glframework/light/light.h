#pragma once

#include "../core.h"

class Light {
public:
  Light();
  ~Light();

public:
  glm::vec3 mColor{1.0f};
  float mSpecularIntensity{1.0f};
};