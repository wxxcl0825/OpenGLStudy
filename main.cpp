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

#include "glframework/light/directionalLight.h"
#include "glframework/light/ambientLight.h"

#include "glframework/renderer/renderer.h"

Renderer* renderer = nullptr;
Camera* camera = nullptr;
CameraControl* cameraControl = nullptr;

std::vector<Mesh*> meshes{};
DirectionalLight* dirLight = nullptr;
AmbientLight* ambLight = nullptr;

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
    renderer = new Renderer();
    
    auto geometry = Geometry::createSphere(1.5f);

    auto material1 = new PhongMaterial();
    material1->mShiness = 10.0f;
    material1->mDiffuse = new Texture("assets/textures/noir.png", 0);

    auto material2 = new PhongMaterial();
    material2->mShiness = 32.0f;
    material2->mDiffuse = new Texture("assets/textures/land.jpg", 0);

    auto mesh1 = new Mesh(geometry, material1);
    auto mesh2 = new Mesh(geometry, material2);
    mesh2->setPosistion(glm::vec3(3.0f, 0, 0));

    meshes.push_back(mesh1);
    meshes.push_back(mesh2);

    dirLight = new DirectionalLight();
    ambLight = new AmbientLight();
    ambLight->mColor = glm::vec3(0.1f);
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
        meshes[0]->rotateX(0.1f);
        meshes[0]->rotateY(1.0f);

        cameraControl->update();
        renderer->render(meshes, camera, dirLight, ambLight);
    }

    app->destroy();
    return 0;
}