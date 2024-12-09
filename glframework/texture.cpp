#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "../wrapper/checkError.h"

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

    stbi_image_free(data);

    // 设置纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //  所需像素数 > 纹理像素数: linear过滤, 平滑图像
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  //  所需像素数 < 纹理像素数: nearest过滤, 增强变化

    // 设置纹理包裹方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // u 方向
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);   // v 方向
}


Texture::~Texture() {
    if (mTexture != 0) {
        glDeleteTextures(1, &mTexture);
    }
}

void Texture::bind() {
    GL_CALL(glActiveTexture(GL_TEXTURE0 + mUnit));  //  先切换纹理单元
    glBindTexture(GL_TEXTURE_2D, mTexture);
}