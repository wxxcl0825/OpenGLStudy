#pragma once
#include "../core.h"

class Tools {
public:
    // 传入矩阵 解构其中的位置信息 旋转信息XYZ 缩放信息
    static void decompose(glm::mat4 matrix, glm::vec3& position, glm::vec3& eulerAngle, glm::vec3& scale);
};