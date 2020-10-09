//
//  03_model.cpp
//
//  Created by Clsrfish on 08/10/2020
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "./model.h"
#include "../gl/shader.h"
#include "../gl/camera.h"
#include "../gl/gl_utils.h"
#include "../utils/log.h"

namespace model_loading
{
    namespace model
    {

        float lightVertices[] = {
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,

            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,

            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,

            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f};

        int screenWidth = 800;
        int screenHeight = 600;

        Camera camera;

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

            // light vao
            unsigned int lightVAO;
            glGenVertexArrays(1, &lightVAO);
            glBindVertexArray(lightVAO);

            unsigned int lightVBO;
            glGenBuffers(1, &lightVBO);
            glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
            glEnableVertexAttribArray(0);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            model_loading::Model nanosuit("assets/nanosuit/nanosuit.obj");
            Shader nanosuitShader("shaders/model_loading/shader.vs", "shaders/model_loading/shader.fs");
            Shader lightShader("shaders/model_loading/light.vs", "shaders/model_loading/light.fs");

            // positions of the point lights
            glm::vec3 pointLightPositions[] = {
                glm::vec3(-3.7f, 0.2f, 2.0f),
                glm::vec3(3.3f, -0.0f, -4.0f),
            };

            glfwSwapInterval(1);
            while (!glfwWindowShouldClose(window))
            {
                processKeyboard(window);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glEnable(GL_DEPTH_TEST);
                glm::mat4 view = camera.GetViewMatrix();
                glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(screenWidth) / screenHeight, 0.1f, 100.0f);

                if (nanosuitShader.Use())
                {
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(0.0f, -4.0f, 0.0f)); // translate it down so it's at the center of the scene
                    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));      // it's a bit too big for our scene, so scale it down
                    nanosuitShader.SetMatrix4("u_Model", glm::value_ptr(model));
                    nanosuitShader.SetMatrix4("u_View", glm::value_ptr(view));
                    nanosuitShader.SetMatrix4("u_Projection", glm::value_ptr(projection));

                    nanosuitShader.SetVec3("u_ViewPos", camera.Position.x, camera.Position.y, camera.Position.z);

                    // point light
                    for (unsigned int i = 0; i < 2; i++)
                    {
                        auto position = pointLightPositions[i];
                        std::stringstream pointLightNameBuilder;
                        pointLightNameBuilder << "u_PointLights[" << i << "].";
                        auto pointLightName = pointLightNameBuilder.str();
                        nanosuitShader.SetVec3(pointLightName + "position", position.x, position.y, position.z);
                        nanosuitShader.SetVec3(pointLightName + "ambient", 0.05f, 0.05f, 0.05f);
                        nanosuitShader.SetVec3(pointLightName + "diffuse", 0.8f, 0.8f, 0.8f);
                        nanosuitShader.SetVec3(pointLightName + "specular", 1.0f, 1.0f, 1.0f);
                        nanosuitShader.SetFloat(pointLightName + "constant", 1.0f);
                        nanosuitShader.SetFloat(pointLightName + "linear", 0.06f);
                        nanosuitShader.SetFloat(pointLightName + "quadratic", 0.024f);
                    }
                    nanosuit.Draw(nanosuitShader);
                }

                if (lightShader.Use())
                {
                    glBindVertexArray(lightVAO);
                    lightShader.SetMatrix4("u_View", glm::value_ptr(view));
                    lightShader.SetMatrix4("u_Projection", glm::value_ptr(projection));
                    for (unsigned int i = 0; i < 2; i++)
                    {
                        auto position = pointLightPositions[i];
                        glm::mat4 model = glm::mat4(1.0f);
                        model = glm::translate(model, position);
                        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
                        lightShader.SetMatrix4("u_Model", glm::value_ptr(model));
                        glDrawArrays(GL_TRIANGLES, 0, 36);
                    }
                }

                glfwSwapBuffers(window);
                glfwPollEvents();
            }
            glfwTerminate();
            return 0;
        }
    } // namespace model
} // namespace model_loading