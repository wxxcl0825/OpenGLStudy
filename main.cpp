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
#include "glframework/material/whiteMaterial.h"
#include "glframework/mesh.h"

#include "glframework/light/directionalLight.h"
#include "glframework/light/ambientLight.h"
#include "glframework/light/pointLight.h"


#include "glframework/renderer/renderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

Renderer* renderer = nullptr;
Scene* scene = nullptr;
Camera* camera = nullptr;
CameraControl* cameraControl = nullptr;

DirectionalLight* dirLight = nullptr;
AmbientLight* ambLight = nullptr;

glm::vec3 clearColor{};

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
    scene = new Scene();
    
    auto geometry = Geometry::createBox(1.0f);
    auto spGeometry = Geometry::createSphere(1.0f);

    auto material = new PhongMaterial();
    material->mShiness = 16.0f;
    material->mDiffuse = new Texture("assets/textures/box.png", 0);
    material->mSpecularMask = new Texture("assets/textures/sp_mask.png", 1);    // 将纹理贴图绑到1号单元

    auto mesh = new Mesh(geometry, material);
    auto spMesh = new Mesh(spGeometry, material);
    spMesh->setPosition(glm::vec3(2.0f, 0.0f, 0.0f));

    mesh->addChild(spMesh);
    scene->addChild(mesh);

    dirLight = new DirectionalLight();
    dirLight->mDirection = glm::vec3(-1.0f);
    
    ambLight = new AmbientLight();
    ambLight->mColor = glm::vec3(0.1f);
}

void initIMGUI() {
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(app->getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void renderIMGUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Hello, world!");
    ImGui::Text("ChangeColor Demo");
    ImGui::Button("Test Button", ImVec2(40, 20));
    ImGui::ColorEdit3("Clear Color", (float *) &clearColor);
    ImGui::End();

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(app->getWindow(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // 设置用于Clear时的颜色, 以便在Clear时将整个画布设置为该颜色

    prepare();
    prepareCamera();
    initIMGUI();

    // 执行窗体循环
    while (app->update()) {
        cameraControl->update();
        renderer->setClearColor(clearColor);
        renderer->render(scene, camera, dirLight, ambLight);
        renderIMGUI();
    }

    app->destroy();
    return 0;
}