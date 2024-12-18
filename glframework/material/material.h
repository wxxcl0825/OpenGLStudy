#pragma once

#include "../core.h"

enum class MaterialType {
    PhongMaterial, WhiteMaterial, DepthMaterial
};

class Renderer;

class Material {
friend Renderer;
public:
    Material();
    ~Material();

    // 深度检测
    bool mDepthTest{ true };
    GLenum mDepthFunc{ GL_LESS };
    bool mDepthWrite { true };

    bool mPolygonOffset{false};
    unsigned int mPolygonOffsetType{GL_POLYGON_OFFSET_FILL};
    float mFactor{0.0f};
    float mUnit{0.0f};

    // stencil
    // glEnable(GL_STENCIL_TEST);
    // glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    // glStencilMask(0xFF);  // 保证模版缓冲可清理
    // glStencilFunc(GL_ALWAYS, 1, 0xFF);  // 如何测试 
    bool mStencilTest{false};

    unsigned int mSFail{GL_KEEP};   // 模板测试失败
    unsigned int mZFail{GL_KEEP};   // 深度测试失败
    unsigned int mZPass{GL_KEEP};   // 模版&深度测试通过

    // ref & mStencilFuncMask 匹配 val & mStencilFuncMask 做测试, 写入权限由mStencilMask控制
    unsigned int mStencilMask{0xFF}; // 模版写入控制

    unsigned int mStencilFunc{GL_ALWAYS}; // 模版测试函数
    int mStencilRef{0}; // 模版测试参考值
    unsigned int mStencilFuncMask{0xFF};

protected:
    MaterialType mType;
};