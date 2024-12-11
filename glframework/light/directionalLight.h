#include "Light.h"

class DirectionalLight : public Light
{
public:
    DirectionalLight();
    ~DirectionalLight();

public:
   glm::vec3 mDirection{-1.0f};

};