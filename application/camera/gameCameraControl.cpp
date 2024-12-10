#include "gameCameraControl.h"

GameCameraControl::GameCameraControl() {

}

GameCameraControl::~GameCameraControl() {

}

void GameCameraControl::onCursor(double xpos, double ypos) {
    float deltaX = (xpos - mCurrentX) * mSensitivity;
    float deltaY = (ypos - mCurrentY) * mSensitivity;
    if (mRightMouseDown) {
        pitch(-deltaY);
        yaw(-deltaX);
    }

    mCurrentX = xpos;
    mCurrentY = ypos;
}

void GameCameraControl::update() {
    glm::vec3 direction(0.0f);

    auto front = glm::cross(mCamera->mUp, mCamera->mRight);
    auto right = mCamera->mRight;

    if (mKeyMap[GLFW_KEY_W])
        direction += front;
    if (mKeyMap[GLFW_KEY_S])
        direction -= front;
    if (mKeyMap[GLFW_KEY_A])
        direction -= right;
    if (mKeyMap[GLFW_KEY_D])
        direction += right;
    
    if (glm::length(direction)) {
        direction = glm::normalize(direction);
        mCamera->mPosition += direction * mSpeed;
    }
}

void GameCameraControl::pitch(float angle) {
    mPitch += angle;
    if (mPitch > 89.0f || mPitch < -89.0f) {
        mPitch -= angle;
        return;
    }

    auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->mRight);
    mCamera->mUp = glm::vec4(mCamera->mUp, 0.0f) * mat;
}

void GameCameraControl::yaw(float angle) {
    auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    mCamera->mUp = glm::vec4(mCamera->mUp, 0.0f) * mat;
    mCamera->mRight = glm::vec4(mCamera->mRight, 0.0f) * mat;
}