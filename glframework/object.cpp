#include "object.h"
#include <iostream>

Object::Object() {
    mType = ObjectType::Object;
}

Object::~Object() {

}

void Object::setPosition(glm::vec3 pos) {
    mPosition = pos;
}

void Object::rotateX(float angle) {
    mAngleX += angle;
}

void Object::rotateY(float angle) {
    mAngleY += angle;
}

void Object::rotateZ(float angle) {
    mAngleZ += angle;
}


void Object::setAngleX(float angle) {
    mAngleX = angle;
}

void Object::setAngleY(float angle) {
    mAngleY = angle;
}

void Object::setAngleZ(float angle) {
    mAngleZ = angle;
}

void Object::setScale(glm::vec3 scale) {
    mScale = scale;
}

glm::mat4 Object::getModelMatrix() const {
    glm::mat4 parentModelMatrix{1.0f};
    if (mParent != nullptr)
        parentModelMatrix = mParent->getModelMatrix();

    // 缩放、旋转、平移
    glm::mat4 transform{1.0f};
    transform = glm::scale(transform, mScale);
    
    transform = glm::rotate(transform, glm::radians(mAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(mAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(mAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));

    transform = parentModelMatrix * glm::translate(glm::mat4(1.0f), mPosition) * transform;

    return transform;
}

void Object::addChild(Object* obj) {
    auto iter = std::find(mChildren.begin(), mChildren.end(), obj);
    if (iter != mChildren.end()) {
        std::cerr << "Duplicate child added" << std::endl;
        return;
    }
    mChildren.push_back(obj);
    obj->mParent = this;
}

std::vector<Object*> Object::getChildren() {
    return mChildren;
}

Object* Object::getParent() {
    return mParent;
}