#include "cameraControl.h"

CameraControl::CameraControl() {
    
}

CameraControl::~CameraControl() {

}

void CameraControl::onKey(int key, int action, int mods) {
    if (action == GLFW_REPEAT) 
        return;
    bool pressed = action == GLFW_PRESS;
    mKeyMap[key] = pressed;
}

void CameraControl::onMouse(int button, int action, float xpos, float ypos) {
    bool pressed = action == GLFW_PRESS;
    if (pressed)
        mCurrentX = xpos, mCurrentY = ypos;
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            mLeftMouseDown = pressed;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            mRightMouseDown = pressed;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            mMiddleMouseDown = pressed;
            break;
    }
}

void CameraControl::onCursor(double xpos, double ypos) {

}


void CameraControl::onScroll(double offset) {
    
}

void CameraControl::update() {
    
}