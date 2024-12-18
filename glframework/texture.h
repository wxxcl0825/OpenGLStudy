#pragma once

#include "../glframework/core.h"
#include <string>
#include <map>

class Texture {
public:
    static Texture* createTexture(const std::string& path, unsigned int unit);
    static Texture* createTexture(const std::string& path, unsigned int unit, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn);
    static Texture* createColorAttachment(unsigned int width, unsigned int height, unsigned int unit);
    static Texture* createDepthStencilAttachment(unsigned int width, unsigned int height, unsigned int unit);

    Texture();
    Texture(const std::string& path, unsigned int unit);
    Texture(const std::vector<std::string>& paths, unsigned int unit);
    Texture(unsigned int unit, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn);
    Texture(unsigned int width, unsigned int height, unsigned int unit); // 空纹理
    ~Texture();

    void bind();

    void setUnit(unsigned int unit) { mUnit = unit; }
    int getWidth() const { return mWidth; }
    int getHeight() const { return mHeight; }
    GLuint getTexture() const { return mTexture; }

private:
    GLuint mTexture{0};
    int mWidth{0};
    int mHeight{0};
    unsigned int mUnit{0};  // 使用: GL_TEXTURE0 + mUnit
    unsigned int mTextureTarget{GL_TEXTURE_2D};  // 纹理类型

    static std::map<std::string, Texture*> mTextureCache;   // 纹理缓存(静态) 路径 -> 纹理
};