//
//  10_instancing.cpp
//
//  Created by Clsrfish on 11/10/2020
//

#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../gl/gl_utils.h"
#include "../gl/shader.h"
#include "../gl/camera.h"
#include "../utils/log.h"

namespace advanced_ogl
{
    namespace instancing
    {
        int screenWidth = 800;
        int screenHeight = 600;

        float quadVertices[] = {
            // 位置          // 颜色
            -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
            0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
            -0.05f, -0.05f, 0.0f, 0.0f, 1.0f,

            -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
            0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
            0.05f, 0.05f, 0.0f, 1.0f, 1.0f};

        int main()
        {
            LOG_I(__FILENAME__);
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
            GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, __FILENAME__, nullptr, nullptr);
            if (window == nullptr)
            {
                LOG_E("Failed to create GLFW window");
                glfwTerminate();
                return -1;
            }
            glfwMakeContextCurrent(window);
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                LOG_E("Failed to initialize GLAD loader");
                glfwTerminate();
                return -1;
            }
            printGLInfo();
            glViewport(0, 0, screenWidth, screenHeight);
            glfwSetFramebufferSizeCallback(window, [](GLFWwindow *, int w, int h) {
                screenWidth = w;
                screenHeight = h;
                glViewport(0, 0, w, h);
            });

            // quad
            unsigned int quadVAO, quadVBO;
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            // translations
            glm::vec2 translations[100];
            int index = 0;
            float offset = 0.1f;
            for (int y = -10; y < 10; y += 2)
            {
                for (int x = -10; x < 10; x += 2)
                {
                    glm::vec2 translation;
                    translation.x = x / 10.0f + offset;
                    translation.y = y / 10.0f + offset;
                    translations[index++] = translation;
                }
            }
            unsigned int offsetVBO;
            glGenBuffers(1, &offsetVBO);
            glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *)(0));
            glEnableVertexAttribArray(2);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glVertexAttribDivisor(2, 1); // the 2nd attribute update every 1 draw call

            glBindVertexArray(0);

            Shader quadShader("shaders/advanced_gl/10/instance.vs", "shaders/advanced_gl/10/instance.fs");

            while (!glfwWindowShouldClose(window))
            {
                if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                {
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                    continue;
                }
                glClear(GL_COLOR_BUFFER_BIT);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

                quadShader.Use();
                glBindVertexArray(quadVAO);
                glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);

                glfwSwapBuffers(window);
                glfwPollEvents();
            }

            return 0;
        }
    } // namespace instancing
} // namespace advanced_ogl