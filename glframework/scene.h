#pragma once
#include "core.h"
#include "object.h"

class Scene: public Object {    // 继承Object以容纳子节点
public:
    Scene();
    ~Scene();
};