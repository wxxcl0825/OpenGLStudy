#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "../wrapper/checkError.h"
#include <iostream>

std::map<std::string, Texture*> Texture::mTextureCache{};

Texture* Texture::createTexture(const std::string& path, unsigned int unit) {
    auto iter = mTextureCache.find(path);
    if (iter != mTextureCache.end())
        return iter->second;
    auto texture = new Texture(path, unit);
    mTextureCache[path] = texture;

    return texture;
}

Texture* Texture::createTexture(const std::string& path, unsigned int unit, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn) {
    auto iter = mTextureCache.find(path);
    if (iter != mTextureCache.end())
        return iter->second;
    auto texture = new Texture(unit, dataIn, widthIn, heightIn);
    mTextureCache[path] = texture;

    return texture;
}

Texture* Texture::createColorAttachment(unsigned int width, unsigned int height, unsigned int unit) {
    return new Texture(width, height, unit);
}

Texture* Texture::createDepthStencilAttachment(unsigned int width, unsigned int height, unsigned int unit) {
    Texture* dsTex = new Texture();

    unsigned int depthStencil;
    GL_CALL(glGenTextures(1, &depthStencil));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, depthStencil));

    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL)); // 写入空数据

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    dsTex->mTexture = depthStencil;
    dsTex->mWidth = width;
    dsTex->mHeight = height;
    dsTex->mUnit = unit;

    return dsTex;
}

Texture::Texture() {

}

Texture::Texture(const std::string& path, unsigned int unit) {
    mUnit = unit;
    
    int channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &mWidth, &mHeight, &channels, STBI_rgb_alpha);

    GL_CALL(glGenTextures(1, &mTexture));
    GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));   // 先激活纹理单元, 后续绑定纹理将绑定在对应的纹理单元上
    // 一旦激活, 关不掉(不能使用glBindTexture(GL_TEXTURE_2D, 0), 否则将删掉当前纹理单元的纹理对象)
    GL_CALL(glBindTexture(GL_TEXTURE_2D, mTexture));
    
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data)); // 传输时开辟显存
    // glGenerateMipmap(GL_TEXTURE_2D);    // Mipmap自动生成
    stbi_image_free(data);

    // 设置纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //  所需像素数 > 纹理像素数: linear过滤, 平滑图像
    // GL_NEAREST: 在mipmap上的采样方式   MIPMAP_LINEAR: 在mipmap层次上做线性插值(叠加方法)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  //  所需像素数 < 纹理像素数: nearest过滤, 增强变化

    // 设置纹理包裹方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // u 方向
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);   // v 方向
}

// 右左上下后前
Texture::Texture(const std::vector<std::string>& paths, unsigned int unit) {
    mUnit = unit;
    mTextureTarget = GL_TEXTURE_CUBE_MAP;

    stbi_set_flip_vertically_on_load(false);  // cubemap 无需翻转y轴

    GL_CALL(glGenTextures(1, &mTexture));
    GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
    GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture));

    int width, height, channels;
    for (int i = 0; i < paths.size(); i++) {
        unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (data) {
            GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
            stbi_image_free(data);
        } else {
            std::cerr << "Cubemap texture failed to load at path: " << paths[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(mTextureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(mTextureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(mTextureTarget, GL_TEXTURE_WRAP_R, GL_REPEAT);
}

Texture::Texture(unsigned int unit, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn) {
    mUnit = unit;
    
    int channels;
    stbi_set_flip_vertically_on_load(true);

    // 计算整张图片的大小
    // Assimp规定 png, jpg 的 height = 0, width代表图片大小
    uint32_t dataInSize = 0;
    if (!heightIn)
        dataInSize = widthIn;
    else
        dataInSize = widthIn * heightIn * 4;    // 统一认为数据格式是RGBA

    unsigned char* data = stbi_load_from_memory(dataIn, dataInSize, &mWidth, &mHeight, &channels, STBI_rgb_alpha);

    GL_CALL(glGenTextures(1, &mTexture));
    GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, mTexture));
    
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::Texture(unsigned int width, unsigned int height, unsigned int unit) {
    mWidth = width;
    mHeight = height;
    mUnit = unit;

    GL_CALL(glGenTextures(1, &mTexture));
    GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, mTexture));

    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

Texture::~Texture() {
    if (mTexture != 0) {
        glDeleteTextures(1, &mTexture);
    }
}

void Texture::bind() {
    GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));  //  先切换纹理单元
    glBindTexture(mTextureTarget, mTexture);
}