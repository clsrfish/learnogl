#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>
#include "../utils/log.h"

namespace textures
{
    float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 左上
    };

    unsigned short indices[] = {
        0, 1, 3,  // 第一个三角形
        1, 2, 3}; // 第二个三角形

    constexpr const char *kVertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        layout (location = 2) in vec2 aTexCoord;

        out vec3 color;
        out vec2 texCoord;
        void main() {
            gl_Position = vec4(aPos, 1.0);
            color = aColor;
            texCoord = aTexCoord;
        }
    )";

    constexpr const char *kFragShaderSource = R"(
        #version 330 core

        in vec3 color;
        in vec2 texCoord;
        uniform sampler2D uTex;

        out vec4 FragColor;

        void main() {
            FragColor = texture(uTex, texCoord) * vec4(color, 1.0);
        }
    )";
    int texturesImpl()
    {
        LOG_I(__FILENAME__);
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
        GLFWwindow *window = glfwCreateWindow(800, 600, __FILENAME__, nullptr, nullptr);
        if (window == nullptr)
        {
            LOG_E("Failed to create GLFW window");
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader(((GLADloadproc)glfwGetProcAddress)))
        {
            LOG_E("Failed to initialize GLAD");
            glfwTerminate();
            return -1;
        }
        glViewport(0, 0, 800, 600);
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow *, int w, int h) {
            glViewport(0, 0, w, h);
        });

        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        unsigned int EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(sizeof(float) * 6));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // init vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &kVertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        int compileStatus;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus <= 0)
        {
            int infoLength;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLength);
            char *logBuffer = new char[infoLength];
            glGetShaderInfoLog(vertexShader, infoLength, nullptr, logBuffer);
            LOG_E("ERROR::SHADER::VERTEX::COMPILATION_FAILED::%s", logBuffer);
            glfwTerminate();
            return -1;
        }

        unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShader, 1, &kFragShaderSource, nullptr);
        glCompileShader(fragShader);
        glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus <= 0)
        {
            int infoLength;
            glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &infoLength);
            char *logBuffer = new char[infoLength];
            glGetShaderInfoLog(fragShader, infoLength, nullptr, logBuffer);
            LOG_E("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED::%s", logBuffer);
            glfwTerminate();
            return -1;
        }

        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragShader);
        glLinkProgram(shaderProgram);
        int linkStatus;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
        if (linkStatus <= 0)
        {
            int infoLength;
            glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLength);
            char *logBuffer = new char[infoLength];
            glGetProgramInfoLog(shaderProgram, infoLength, nullptr, logBuffer);
            LOG_E("ERROR::PROGRAM::LINK_FAILED::%s", logBuffer);
            glfwTerminate();
            return -1;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragShader);

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int width, height;
        char *textureFileName = "assets/container.jpg";
        auto data = stbi_load(textureFileName, &width, &height, nullptr, 0);
        if (data == nullptr)
        {
            LOG_E("Failed to load texture: %s", textureFileName);
            glfwTerminate();
            return -1;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);

        while (!glfwWindowShouldClose(window))
        {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            glUseProgram(shaderProgram);
            glBindTexture(GL_TEXTURE_2D, texture);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void *)0);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }
} // namespace textures

namespace getting_started
{
    void textures()
    {
        textures::texturesImpl();
    }
} // namespace getting_started
