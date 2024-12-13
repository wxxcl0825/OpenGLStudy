#pragma once

#include "../glframework/core.h"
#include <string>
#include <map>

class Texture {
public:
    static Texture* createTexture(const std::string& path, unsigned int unit);
    static Texture* createTexture(const std::string& path, unsigned int unit, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn);

    Texture(const std::string& path, unsigned int unit);
    Texture(unsigned int unit, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn);
    ~Texture();

    void bind();

    void setUnit(unsigned int unit) { mUnit = unit; }
    int getWidth() const { return mWidth; }
    int getHeight() const { return mHeight; }

private:
    GLuint mTexture{0};
    int mWidth{0};
    int mHeight{0};
    unsigned int mUnit{0};  // 使用: GL_TEXTURE0 + mUnit

    static std::map<std::string, Texture*> mTextureCache;   // 纹理缓存(静态) 路径 -> 纹理
};