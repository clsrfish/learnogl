//
//  10_instancing.cpp
//
//  Created by Clsrfish on 11/10/2020
//

#include <iostream>
#include <sstream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../model_loading/model.h"
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

        Camera camera(glm::vec3(0.0f, 0.0f, 200.0f));

        bool everMoved = false;
        double lastCursorX = 0.0;
        double lastCursorY = 0.0;
        void processCursorMove(GLFWwindow *window, double xpos, double ypos)
        {
            if (!everMoved)
            {
                lastCursorX = xpos;
                lastCursorY = ypos;
                everMoved = true;
            }
            double xoffset = xpos - lastCursorX;
            double yoffset = lastCursorY - ypos;
            lastCursorX = xpos;
            lastCursorY = ypos;
            camera.ProcessMouseMovement(xoffset, yoffset);
        }

        void processMouseScroll(GLFWwindow *window, double xoffset, double yoffset)
        {
            camera.ProcessMouseScroll(yoffset);
        }

        float lastFrameTime = -1.0f;
        void processKeyboard(GLFWwindow *window)
        {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                LOG_I("Escape pressed, exiting.");
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
            float currentFrameTime = glfwGetTime();
            if (lastFrameTime < 0.0f)
            {
                lastFrameTime = currentFrameTime;
            }
            float deltaTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            {
                camera.ProcessKeyboard(FORWARD, deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            {
                camera.ProcessKeyboard(BACKWARD, deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            {
                camera.ProcessKeyboard(LEFT, deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            {
                camera.ProcessKeyboard(RIGHT, deltaTime);
            }
        }

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
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(window, processCursorMove);
            glfwSetScrollCallback(window, processMouseScroll);

            std::vector<glm::mat4> modelMatrices;
            float radius = 150.0f;
            float offset = 25.0f;
            srand(glfwGetTime());
            unsigned int amount = 10000;
            for (unsigned int i = 0; i < amount; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                // 1. translate
                float angle = (float)i / (float)amount * 360.0f;
                float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
                float x = sin(angle) * radius + displacement;
                displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
                float y = displacement * 0.4f;
                displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
                float z = cos(angle) * radius + displacement;
                model = glm::translate(model, glm::vec3(x, y, z));

                // 2. scale
                float scale = (rand() % 20) / 100.0f + 0.05;
                model = glm::scale(model, glm::vec3(scale));

                // 3. rotate
                float rotAngle = (rand() % 360);
                model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

                modelMatrices.push_back(model);
            }

            // load models
            model_loading::Model planet("assets/planet/planet.obj");
            model_loading::Model rock("assets/rock/rock.obj");

            unsigned int buffer;
            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
            for (unsigned int i = 0; i < rock.Meshes.size(); i++)
            {
                unsigned int VAO = rock.Meshes[i].VAO;
                glBindVertexArray(VAO);
                glBindBuffer(GL_ARRAY_BUFFER, buffer);
                auto vec4Size = sizeof(glm::vec4);
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)0);
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(vec4Size));
                glEnableVertexAttribArray(5);
                glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(2 * vec4Size));
                glEnableVertexAttribArray(6);
                glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(3 * vec4Size));

                glVertexAttribDivisor(3, 1);
                glVertexAttribDivisor(4, 1);
                glVertexAttribDivisor(5, 1);
                glVertexAttribDivisor(6, 1);
                glBindVertexArray(0);
            }

            // load shaders
            Shader modelShader("shaders/advanced_gl/10/model.vs", "shaders/advanced_gl/10/model.fs");
            Shader rockShader("shaders/advanced_gl/10/rock.vs", "shaders/advanced_gl/10/model.fs");

            // glfwSwapInterval(1);
            while (!glfwWindowShouldClose(window))
            {
                processKeyboard(window);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glEnable(GL_DEPTH_TEST);

                if (!modelShader.Use())
                {
                    break;
                }
                glm::mat4 view = camera.GetViewMatrix();
                glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / screenHeight, 0.1f, 1000.0f);

                modelShader.SetMatrix4("u_View", glm::value_ptr(view));
                modelShader.SetMatrix4("u_Projection", glm::value_ptr(projection));

                // draw planet
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
                model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
                modelShader.SetMatrix4("u_Model", glm::value_ptr(model));
                planet.Draw(modelShader);

                rockShader.Use();
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, rock.loadedTextures[0].id);
                rockShader.SetMatrix4("u_View", glm::value_ptr(view));
                rockShader.SetMatrix4("u_Projection", glm::value_ptr(projection));
                rockShader.SetInt("u_TextureDiffuse0", 0);
                // draw meteories
                for (unsigned int i = 0; i < rock.Meshes.size(); i++)
                {
                    glBindVertexArray(rock.Meshes[i].VAO);
                    glDrawElementsInstanced(GL_TRIANGLES, rock.Meshes[i].indices.size(), GL_UNSIGNED_INT, (void *)0, amount);
                }

                glfwSwapBuffers(window);
                glfwPollEvents();
            }

            glfwTerminate();
            return 0;
        }
    } // namespace instancing
} // namespace advanced_ogl