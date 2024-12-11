#include <iostream>

#include "glframework/core.h"
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "glframework/shader.h"
#include "glframework/texture.h"
#include "glframework/geometry.h"

#include "application/camera/camera.h"
#include "application/camera/orthographicCamera.h"
#include "application/camera/perspectiveCamera.h"

#include "application/camera/cameraControl.h"
#include "application/camera/gameCameraControl.h"
#include "application/camera/trackBallCameraControl.h"

#include "glframework/material/material.h"
#include "glframework/material/phongMaterial.h"
#include "glframework/mesh.h"

// GLuint vao;
Shader* shader = nullptr;
// Texture* grassTexture = nullptr;
// Texture* landTexture = nullptr;
// Texture* noiseTexture = nullptr;
Texture* texture = nullptr;
glm::mat4 transform(1.0);
glm::mat4 viewMatrix(1.0f);
glm::mat4 orthoMatrix(1.0f);
glm::mat4 perspectiveMatrix(1.0f);

PerspectiveCamera* camera = nullptr;
// OrthographicCamera* camera = nullptr;
// TrackBallCameraControl* cameraControl = nullptr;
GameCameraControl* cameraControl = nullptr;

Geometry* geometry = nullptr;

// 平行光: 光线方向 + 强度(由颜色表示)
glm::vec3 lightDirection = glm::vec3(-1.0f, -1.0f, -1.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 ambientColor = glm::vec3(0.1f);

// 高光反射强度(高光亮度)
float specularIntensity = 0.5f;

void OnResize(int width, int height) {
    GL_CALL(glViewport(0, 0, width, height));
}

void onKey(int key, int action, int mods) {
    cameraControl->onKey(key, action, mods);
}

void onMouse(int button, int action, int mods) {
    double x = 0, y = 0;
    app->getCursorPosition(&x, &y);
    cameraControl->onMouse(button, action, x, y);
}

void onCursor(double xpos, double ypos) {
    cameraControl->onCursor(xpos, ypos);
}

void onScroll(double offset) {
    cameraControl->onScroll(offset);
}

void prepareCamera() {
    camera = new PerspectiveCamera(60.0f, (float)(app->getWidth()) / (float)(app->getHeight()), 0.1f, 1000.0f);
    // cameraControl = new TrackBallCameraControl();
    cameraControl = new GameCameraControl();
    cameraControl->setCamera(camera);
    cameraControl->setSensitivity(0.2f);
}

void prepare() {
    auto geometry = Geometry::createSphere(3.0f);

    auto material = new PhongMaterial();
    material->mShiness = 32.0f;
    material->mDiffuse = new Texture("assets/textures/noir.png", 0);

    auto mesh = new Mesh(geometry, material);
}

void render() {
    // 清理画布
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // 渲染操作
    // 绑定program(选择材质)
    shader->begin();

    // 切换贴图, 切换Shader Uniform变量, 切换vao, 绘制
    texture->bind();
    // shader->setFloat("time", glfwGetTime());    // vs, fs变量重名时, 合二为一
    // shader->setVector3("uColor", 0.3, 0.4, 0.5);
    // shader->setInt("grassSampler", 0);
    // shader->setInt("landSampler", 1);
    // shader->setInt("noiseSampler", 2);
    shader->setInt("sampler", 0);
    // shader->setFloat("width", texture->getWidth());
    // shader->setFloat("height", texture->getHeight());
    shader->setMatrix4x4("modelMatrix", transform);
    shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
    shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

    // 光源相关参数
    shader->setVector3("lightDirection", lightDirection);
    shader->setVector3("lightColor", lightColor);
    shader->setFloat("specularIntensity", specularIntensity);
    shader->setVector3("ambientColor", ambientColor);

    shader->setVector3("cameraPosition", camera->mPosition);

    // 计算法线矩阵并下放
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(transform));
    shader->setMatrix3x3("normalMatrix", glm::mat3(normalMatrix));

    // 绑定vao(选择几何信息)
    glBindVertexArray(geometry->getVao());

    // 发出绘制指令
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    shader->end();
}

int main() {    
    if (!app->init(800, 600))
        return -1;

    app->setResizeCallback(OnResize);
    app->setKeyBoardCallback(onKey);
    app->setMouseCallback(onMouse);
    app->setCursorCallback(onCursor);
    app->setScrollCallback(onScroll);

    // 设置openGL视口并清理颜色
    glViewport(0, 0, 800, 600);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // 设置用于Clear时的颜色, 以便在Clear时将整个画布设置为该颜色

    prepare();
    prepareCamera();

    // 执行窗体循环
    while (app->update()) {
        cameraControl->update();
        render();
    }

    app->destroy();
    // delete grassTexture;
    // delete landTexture;
    delete texture;
    return 0;
}