#include "orthographicCamera.h"
#include <cmath>

OrthographicCamera::OrthographicCamera(float l, float r, float t, float b, float n, float f) {
    mLeft = l;
    mRight = r;
    mTop = t;
    mBottom = b;
    mNear = n;
    mFar = f;
}

OrthographicCamera::~OrthographicCamera() {

}

glm::mat4 OrthographicCamera::getProjectionMatrix() {
    float scale = std::pow(2.0, mScale);
    return glm::ortho(mLeft, mRight, mBottom, mTop, mNear, mFar);
}

