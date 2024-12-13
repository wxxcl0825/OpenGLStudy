#include "renderer.h"
#include "../material/phongMaterial.h"
#include "../material/whiteMaterial.h"

#include <string>

Renderer::Renderer() {
    mPhongShader = new Shader("assets/shaders/phong.vert", "assets/shaders/phong.frag");
    mWhiteShader = new Shader("assets/shaders/white.vert", "assets/shaders/white.frag");
    mDepthShader = new Shader("assets/shaders/depth.vert", "assets/shaders/depth.frag");
}

Renderer::~Renderer() {

}

void Renderer::render(Scene* scene, Camera* camera, DirectionalLight* dirLight, AmbientLight* ambLight) {    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderObject(scene, camera, dirLight, ambLight);
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
        case MaterialType::DepthMaterial:
            result = mDepthShader;
            break;
        default:
            break;
    }

    return result;
}

void Renderer::setClearColor(glm::vec3 color) {
    glClearColor(color.r, color.g, color.b, 1.0f);
}

void Renderer::renderObject(Object* object, Camera* camera, DirectionalLight* dirLight, AmbientLight* ambLight) {
    if (object->getType() == ObjectType::Mesh) {
        auto mesh = (Mesh *) object;
        auto geometry = mesh->mGeometry;
        auto material = mesh->mMaterial;

        // 设置深度检测
        if (material->mDepthTest) {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(material->mDepthFunc);
        } else
            glDisable(GL_DEPTH_TEST);

        if (material->mDepthWrite)
            glDepthMask(GL_TRUE);
        else
            glDepthMask(GL_FALSE);


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
                // shader->setInt("specularMaskSampler", 1);
                // phongMat->mSpecularMask->bind();

                // MVP变换
                shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
                shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
                shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

                // 光源
                shader->setVector3("directionalLight.direction", dirLight->mDirection);
                shader->setVector3("directionalLight.color", dirLight->mColor);
                shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);
                shader->setVector3("ambientColor", ambLight->mColor);
                shader->setFloat("shiness", phongMat->mShiness);

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
            case MaterialType::DepthMaterial: {
                shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
                shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
                shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());
                shader->setFloat("near", camera->mNear);
                shader->setFloat("far", camera->mFar);
                }
                break;
            default:
                break;
        }

        glBindVertexArray(geometry->getVao());
        glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);

        shader->end();
    }

    auto children = object->getChildren();
    for (auto child: children) {
        renderObject(child, camera, dirLight, ambLight);
    }
}