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

    Material* mGlobalMaterial{nullptr}; // 全局材质

private:
    Shader* mPhongShader{nullptr};
    Shader* mWhiteShader{nullptr};
    Shader* mDepthShader{nullptr};
    Shader* mOpacityMaskShader{nullptr};

    Shader* pickShader(MaterialType type);
    void setDepthState(Material* material);
    void setPolygonOffsetState(Material* material);
    void setStencilState(Material* material);
    void setBlendState(Material* material);

    // 透明物体绘制策略: 先绘制不透明物体，再从后往前绘制透明物体
    // 每一帧绘制前需要清空
    std::vector<Mesh*> mOpacityObjects{};
    std::vector<Mesh*> mTransparentObjects{};

    void projectObject(Object* object); // 判断物体透明/不透明, 并放入对应队列中
};