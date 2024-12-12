#include "renderer.h"
#include "../material/phongMaterial.h"
#include "../material/whiteMaterial.h"

Renderer::Renderer() {
    mPhongShader = new Shader("assets/shaders/phong.vert", "assets/shaders/phong.frag");
    mWhiteShader = new Shader("assets/shaders/white.vert", "assets/shaders/white.frag");
}

Renderer::~Renderer() {

}

void Renderer::render(const std::vector<Mesh *> &meshes, Camera *camera,
            PointLight *pointLight, AmbientLight *ambLight) {
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
                shader->setVector3("lightPosition", pointLight->getPosition());
                shader->setVector3("lightColor", pointLight->mColor);
                shader->setFloat("specularIntensity", pointLight->mSpecularIntensity);
                shader->setVector3("ambientColor", ambLight->mColor);
                shader->setFloat("shiness", phongMat->mShiness);
                shader->setFloat("k1", pointLight->mK1);
                shader->setFloat("k2", pointLight->mK2);
                shader->setFloat("kc", pointLight->mKc);

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