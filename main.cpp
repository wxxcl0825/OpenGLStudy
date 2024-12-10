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

void doRotationTransform() {
    transform = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));  // 角度为float, 弧度
}

void doTranslationTransform() {
    transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
}

void doScaleTransform() {
    transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 1.0f));
}

void doTransform() {
    float angle = 1.0f;
    /* 
        glm叠加变换相对于本地坐标系
        1. 旋转变换: 相对于图形中心
        2. 移动变换: 相对于缩放大小
    */
    transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
}

float angle = 0.0f;
void doRotation() {
    angle += 2.0f;
    transform = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0, 0.0, 1.0)); 
}

void prepareVAO() {
    geometry = Geometry::createBox(6.0f);
}

void prepareShader() {
    shader = new Shader("assets/shaders/vertex.vert", "assets/shaders/fragment.frag");
}

void prepareTexture() {
    // grassTexture = new Texture("assets/textures/grass.jpg", 0);
    // landTexture = new Texture("assets/textures/land.jpg", 1);
    // noiseTexture = new Texture("assets/textures/noise.png", 2);
    texture = new Texture("assets/textures/noir.png", 0);
}

void prepareCamera() {
    camera = new PerspectiveCamera(60.0f, (float)(app->getWidth()) / (float)(app->getHeight()), 0.1f, 1000.0f);
    // cameraControl = new TrackBallCameraControl();
    cameraControl = new GameCameraControl();
    cameraControl->setCamera(camera);
    cameraControl->setSensitivity(0.2f);
}

void prepareState() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // glDepthFunc(GL_GREATER);
    // glClearDepth(0.0f);
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
    shader->setMatrix4x4("transform", transform);
    shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
    shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

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

    prepareShader();
    prepareVAO();
    prepareTexture();
    prepareCamera();
    prepareState();

    // 执行窗体循环
    while (app->update()) {
        cameraControl->update();
        render();
        // doTransform();
    }

    app->destroy();
    // delete grassTexture;
    // delete landTexture;
    delete texture;
    return 0;
}