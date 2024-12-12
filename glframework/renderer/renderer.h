#pragma once
#include "../core.h"
#include "../mesh.h"
#include "../light/directionalLight.h"
#include "../light/ambientLight.h"
#include "../../application/camera/camera.h"
#include "../shader.h"


#include <vector>

class Renderer {
public:
    Renderer();
    ~Renderer();

    // 调用后渲染一帧
    void render(
        const std::vector<Mesh*>& meshes,
        Camera* camera,
        DirectionalLight* dirLight,
        AmbientLight* ambLight);
private:
    Shader* mPhongShader{nullptr};

    Shader* pickShader(MaterialType type);
};