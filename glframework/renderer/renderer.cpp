#include "renderer.h"
#include "../material/phongMaterial.h"
#include "../material/whiteMaterial.h"

#include <string>

Renderer::Renderer() {
    mPhongShader = new Shader("assets/shaders/phong.vert", "assets/shaders/phong.frag");
    mWhiteShader = new Shader("assets/shaders/white.vert", "assets/shaders/white.frag");
}

Renderer::~Renderer() {

}

void Renderer::render(const std::vector<Mesh *> &meshes, Camera *camera,
            const std::vector<SpotLight*>& spotLights, AmbientLight *ambLight) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto mesh: meshes) {
        auto geometry = mesh->mGeometry;
        auto material = mesh->mMaterial;

        Shader* shader = pickShader(material->mType);
        // uniform 更新
        shader->begin();
        // TODO: 这段设计不合理, 应仔细处理Material和Shader的关系
        switch (material->mType) {
            case MaterialType::PhongMaterial: {
                // 类型转换
                PhongMaterial* phongMat = (PhongMaterial*) material;
                // diffuse贴图
                shader->setInt("sampler", 0);   // 纹理采样器 -> 纹理单元
                // 纹理 -> 纹理单元 (需要获取mesh->mMaterial->mDiffuse)
                phongMat->mDiffuse->bind();
                
                // 高光蒙版更新
                shader->setInt("specularMaskSampler", 1);
                phongMat->mSpecularMask->bind();

                // MVP变换
                shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
                shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
                shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

                // 光源
                for (int i = 0; i < spotLights.size(); i++) {
                    auto spotLight = spotLights[i];
                    std::string baseName = "spotLights[";
                    baseName.append(std::to_string(i));
                    baseName.append("]");

                    shader->setVector3(baseName + ".position", spotLight->getPosition());
                    shader->setVector3(baseName + ".color", spotLight->mColor);
                    shader->setVector3(baseName + ".targetDirection", spotLight->mTargetDirection);
                    shader->setFloat(baseName + ".innerLine", glm::cos(glm::radians(spotLight->mInnerAngle)));
                    shader->setFloat(baseName + ".outerLine", glm::cos(glm::radians(spotLight->mOuterAngle)));
                    shader->setFloat(baseName + ".specularIntensity", spotLight->mSpecularIntensity);
                    shader->setVector3("ambientColor", ambLight->mColor);
                    shader->setFloat("shiness", phongMat->mShiness);
                }

                // 相机
                shader->setVector3("cameraPosition", camera->mPosition);

                // 法线矩阵
                glm::mat4 normalMatrix = glm::transpose(glm::inverse(mesh->getModelMatrix()));
                shader->setMatrix3x3("normalMatrix", glm::mat3(normalMatrix));
                }
                break;
            case MaterialType::WhiteMaterial: {
                shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
                shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
                shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());
                }
                break;
            default:
                continue;
        }

        glBindVertexArray(geometry->getVao());
        glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);

        shader->end();
    }
}

Shader* Renderer::pickShader(MaterialType type) {
    Shader* result = nullptr;

    switch (type) {
        case MaterialType::PhongMaterial:
            result = mPhongShader;
            break;
        case MaterialType::WhiteMaterial:
            result = mWhiteShader;
            break;
        default:
            break;
    }

    return result;
}