#pragma once
#include "../core.h"
#include "../mesh.h"
#include "../light/directionalLight.h"
#include "../light/ambientLight.h"
#include "../light/pointLight.h"
#include "../light/spotLight.h"
#include "../../application/camera/camera.h"
#include "../shader.h"
#include "../scene.h"


#include <vector>

class Renderer {
public:
    Renderer();
    ~Renderer();

    // 调用后渲染一帧
    void render(
        Scene* scene,
        Camera* camera,
        DirectionalLight* dirLight,
        AmbientLight* ambLight);

    void renderObject(Object* object, Camera* camera, DirectionalLight* dirLight, AmbientLight* ambLight); // 渲染一个物体(及其子节点)

    void setClearColor(glm::vec3 color);

private:
    Shader* mPhongShader{nullptr};
    Shader* mWhiteShader{nullptr};

    Shader* pickShader(MaterialType type);
};