#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>
#include "../gl/shader.h"
#include "../utils/log.h"
#include "../gl/gl_utils.h"

namespace getting_started
{
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
            else
            {
                printGLInfo();
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

            const char *src = nullptr;
            Shader *shader = new Shader("shaders/getting_started/06/shader.vs", "shaders/getting_started/06/shader.fs");

            stbi_set_flip_vertically_on_load(1);
            unsigned int containerTexture;
            glGenTextures(1, &containerTexture);
            glBindTexture(GL_TEXTURE_2D, containerTexture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            int width, height, channels;
            char *textureFileName = "assets/container.jpg";
            auto data = stbi_load(textureFileName, &width, &height, &channels, 0);
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

            unsigned int faceTexture;
            glGenTextures(1, &faceTexture);
            glBindTexture(GL_TEXTURE_2D, faceTexture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            textureFileName = "assets/awesomeface.png";
            data = stbi_load(textureFileName, &width, &height, &channels, 0);
            if (data == nullptr)
            {
                LOG_E("Failed to load texture: %s", textureFileName);
                glfwTerminate();
                return -1;
            }
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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

                if (!shader->Use())
                {
                    break;
                }

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, containerTexture);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, faceTexture);
                shader->SetInt("texture0", 0);
                shader->SetInt("texture1", 1);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void *)0);

                glfwSwapBuffers(window);
                glfwPollEvents();
            }

            glfwTerminate();
            return 0;
        }
    } // namespace textures

} // namespace getting_started
