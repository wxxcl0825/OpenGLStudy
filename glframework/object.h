#pragma once
#include "core.h"

class Object {
public:
    Object();
    ~Object();

    void setPosistion(glm::vec3 pos);

    // 增量变换
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);

    void setScale(glm::vec3 scale);

    // 目的: 获取modelmatrix
    glm::mat4 getModelMatrix();

protected:
    glm::vec3 mPosition{0.0f};  // 世界坐标系

    // Unity旋转标准: pitch yaw roll(采用局部坐标系)
    float mAngleX{0.0f};
    float mAngleY{0.0f};
    float mAngleZ{0.0f};

    glm::vec3 mScale{1.0f};
};