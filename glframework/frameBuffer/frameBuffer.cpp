#include "frameBuffer.h"
#include <iostream>

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height) {
    mWeight = width;
    mHeight = height;

    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    // 创建颜色附件
    mColorAttachment = Texture::createColorAttachment(width, height, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment->getTexture(), 0);
    // 创建深度&模板附件
    mDepthStencilAttachment = Texture::createDepthStencilAttachment(width, height, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthStencilAttachment->getTexture(), 0);
    // 检查FBO完整性
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);   // 解绑
}

FrameBuffer::~FrameBuffer() {
    if (mFBO != 0)
        glDeleteFramebuffers(1, &mFBO);
    if (mColorAttachment != nullptr)
        delete mColorAttachment;
    if (mDepthStencilAttachment != nullptr)
        delete mDepthStencilAttachment;
}