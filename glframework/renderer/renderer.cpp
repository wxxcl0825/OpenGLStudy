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
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilMask(0xFF);
    glDisable(GL_BLEND); // 默认关闭, 颜色混合开销大, 且无需清理
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
        setDepthState(material);
        // 设置Polygon Offset
        setPolygonOffsetState(material);
        // 设置模版测试
        setStencilState(material);
        // 设置颜色混合
        setBlendState(material);


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

                // 透明度
                shader->setFloat("opacity", material->mOpacity);
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

void Renderer::setDepthState(Material* material) {
    if (material->mDepthTest) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(material->mDepthFunc);
    } else
        glDisable(GL_DEPTH_TEST);

    if (material->mDepthWrite)
        glDepthMask(GL_TRUE);
    else
        glDepthMask(GL_FALSE);
}

void Renderer::setPolygonOffsetState(Material* material) {
    // 目的: 消除zFighting
    // glEnable(GL_POLYGON_OFFSET_FILL); // FILL: 面片; LINE: 线
    // glPolygonOffset(1.0f, 0.0f); // factor: 深度值关于屏幕像素变化越快, 增加的z值越大(远处), offset越大; units: 将深度值增加几个基本单元
    if (material->mPolygonOffset) {
        glEnable(material->mPolygonOffsetType);
        glPolygonOffset(material->mFactor, material->mUnit);
    } else {
        glDisable(GL_POLYGON_OFFSET_FILL);
        glDisable(GL_POLYGON_OFFSET_LINE);
    }
}

void Renderer::setStencilState(Material* material) {
    if (material->mStencilTest) {
        glEnable(GL_STENCIL_TEST);
        glStencilOp(material->mSFail, material->mZFail, material->mZPass);
        glStencilMask(material->mStencilMask);
        glStencilFunc(material->mStencilFunc, material->mStencilRef, material->mStencilFuncMask);
    } else {
        glDisable(GL_STENCIL_TEST);
    }
}

void Renderer::setBlendState(Material* material) {
    if (material->mBlend) {
        glEnable(GL_BLEND);
        glBlendFunc(material->mSFactor, material->mDFactor);
    } else {
        glDisable(GL_BLEND);
    }
}