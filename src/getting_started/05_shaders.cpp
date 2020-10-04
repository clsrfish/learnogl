#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "../utils/log.h"

namespace getting_started
{
    namespace shaders
    {
        float vertices[] = {
            0.5f, 0.5f, 0.0f,   // 左上角
            0.5f, -0.5f, 0.0f,  // 右下角
            -0.5f, -0.5f, 0.0f, // 左下角
            -0.5f, 0.5f, 0.0f}; // 左上角

        unsigned short indices[] = {
            0, 1, 3,  // 第一个三角形
            1, 2, 3}; // 第二个三角形

        constexpr const char *kVertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main() {
            gl_Position = vec4(aPos, 1.0);
        }
        )";

        constexpr const char *kFragShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        uniform vec4 uColor;

        void main() {
            FragColor = uColor;
        }
        )";
        int main()
        {
            LOG_I(__FILENAME__);
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
            LOG_D("Running on macOS, enable forward compatibilty.");
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
            GLFWwindow *window = glfwCreateWindow(800, 600, __FILENAME__, nullptr, nullptr);
            if (window == nullptr)
            {
                LOG_E("Failed to create GLFW Window");
                glfwTerminate();
                return -1;
            }
            glfwMakeContextCurrent(window);
            if (!gladLoadGLLoader(((GLADloadproc)glfwGetProcAddress)))
            {
                LOG_E("Failed to initialize GLAD loader");
                glfwTerminate();
                return -1;
            }
            else
            {
                LOG_I("GLVersion: %s GLVendor: %s", glGetString(GL_VERSION), glGetString(GL_VENDOR));
            }
            glViewport(0, 0, 800, 600);
            glfwSetFramebufferSizeCallback(window, [](GLFWwindow *, int w, int h) -> void {
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
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
            glEnableVertexAttribArray(0);

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

            while (!glfwWindowShouldClose(window))
            {
                if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                {
                    LOG_I("ESCAPE pressed, exiting.");
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                }
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

                glUseProgram(shaderProgram);
                float timeValue = glfwGetTime();
                float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
                int uColor = glGetUniformLocation(shaderProgram, "uColor");
                glUniform4f(uColor, 0.0f, greenValue, 0.0f, 1.0f);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
                glBindVertexArray(0);

                glfwSwapBuffers(window);
                glfwPollEvents();
            }

            glfwTerminate();
            return 0;
        }

    } // namespace shaders

} // namespace getting_started
