#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "wrapper/checkError.h"
#include "application/Application.h"
#include "glframework/shader.h"

GLuint vao;
Shader* shader = nullptr;

void OnResize(int width, int height) {
    GL_CALL(glViewport(0, 0, width, height));
}

void onKey(int key, int action, int mods) {
    // if (key == GLFW_KEY_W) {

    // }

    // if (action == GLFW_PRESS) {

    // }

    // if (action == GLFW_RELEASE) {

    // }

    // if (mods == GLFW_MOD_CONTROL) {

    // }

    // if (mods == GLFW_MOD_SHIFT) {

    // }
    std::cout << "Key Pressed: " << key << std::endl;
    std::cout << "action: " << action << std::endl;
    std::cout << "mods: " << mods << std::endl;
}

void prepareSingleBuffer() {
    // 为数据各自生成一个vbo, 为分开的vbo各自填充数据
    float positions[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    float colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    GLuint posVbo = 0, colorVbo = 0;
    GL_CALL(glGenBuffers(1, &posVbo));
    GL_CALL(glGenBuffers(1, &colorVbo));
    
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorVbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW));

    // 生成vao, 并分别加入vao
    GLuint vao = 0;
    GL_CALL(glGenVertexArrays(1, &vao));
    GL_CALL(glBindVertexArray(vao));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));  // 绑定vbo, 下面属性描述才能与该vbo相关
    GL_CALL(glEnableVertexAttribArray(0));  // vao的0号位
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0));  // 压入属性描述

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorVbo));
    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0));

    GL_CALL(glBindVertexArray(0));  // vao解绑
}

void prepareInterleavedBuffer() {
    // 将所有数据放在一个vbo里
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    GLuint vbo = 0;
    GL_CALL(glGenBuffers(1, &vbo));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    GL_CALL(glGenVertexArrays(1, &vao));
    GL_CALL(glBindVertexArray(vao));

    // glBindBuffer(GL_ARRAY_BUFFER, vbo);  //  已绑定

    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));

    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));

    GL_CALL(glBindVertexArray(0));
}

void prepareVAOForGLTriangles() {
    float positions[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.8f, 0.8f, 0.0f,
        0.8f, 0.0f, 0.0f,
    };

    GLuint posVbo;
    glGenBuffers(1, &posVbo);

    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    glBindVertexArray(0);
}

void prepareVAO() {
    float positions[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };

    float colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
    };

    GLuint posVbo;
    glGenBuffers(1, &posVbo);
    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    GLuint colorVbo;
    glGenBuffers(1, &colorVbo);
    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // 绑定vbo, ebo, 加入描述信息
    glBindBuffer(GL_ARRAY_BUFFER, posVbo); // 可以省略
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);  // 该语句会寻找当前已绑定的vbo, 故只需在vbo绑定后即可(vbo绑定要在vao写入数据之前)

    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0); 

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // 不可省略, 要在vao绑定后才能把ebo绑给绑过的vao(ebo绑定要在vao绑定之后)

    glBindVertexArray(0);   // 解绑VAO, 由于部分设置依赖于当前绑定的VAO, 防止VAO被篡改
}

void prepareShader() {
    shader = new Shader("assets/shaders/vertex.vert", "assets/shaders/fragment.frag");
}

void render() {
    // 清理画布
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

    // 渲染操作
    // 绑定program(选择材质)
    shader->begin();

    shader->setFloat("time", glfwGetTime());    // vs, fs变量重名时, 合二为一

    // 绑定vao(选择几何信息)
    glBindVertexArray(vao);

    // 发出绘制指令
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    
    shader->end();
}

int main() {
    if (!app->init(800, 600))
        return -1;

    app->setResizeCallback(OnResize);
    app->setKeyBoardCallBack(onKey);

    // 设置openGL视口并清理颜色
    glViewport(0, 0, 800, 600);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // 设置用于Clear时的颜色, 以便在Clear时将整个画布设置为该颜色

    prepareShader();
    prepareVAO();

    // 执行窗体循环
    while (app->update()) {
        render();
    }

    app->destroy();
    return 0;
}