#pragma once
#include "core.h"

#include <vector>


enum class ObjectType {
    Object,
    Mesh,
    Scene
};

class Object {
public:
    Object();
    ~Object();

    void setPosition(glm::vec3 pos);

    // 增量变换
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);

    void setAngleX(float angle);
    void setAngleY(float angle);
    void setAngleZ(float angle);

    void setScale(glm::vec3 scale);

    // 目的: 获取modelmatrix
    glm::mat4 getModelMatrix() const;
    glm::vec3 getPosition() const { return mPosition; }

    void addChild(Object* obj);
    std::vector<Object*> getChildren();
    Object* getParent();
    ObjectType getType() const { return mType; }

protected:
    glm::vec3 mPosition{0.0f};  // 世界坐标系

    // Unity旋转标准: pitch yaw roll(采用局部坐标系)
    float mAngleX{0.0f};
    float mAngleY{0.0f};
    float mAngleZ{0.0f};

    glm::vec3 mScale{1.0f};

    std::vector<Object*> mChildren{};
    Object* mParent{nullptr};

    ObjectType mType;
};