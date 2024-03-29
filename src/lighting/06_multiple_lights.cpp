//
//  06_multiple_lights.cpp
//
//  Created by Clsrfish on 08/10/2020
//
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <algorithm>
#include "../gl/shader.h"
#include "../gl/camera.h"
#include "../utils/log.h"
#include "../gl/gl_utils.h"

namespace lighting
{
    namespace multiple_lights
    {
        float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

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

            unsigned int cubeVAO;
            glGenVertexArrays(1, &cubeVAO);
            glBindVertexArray(cubeVAO);

            // cube
            unsigned int cubeVBO;
            glGenBuffers(1, &cubeVBO);
            glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(sizeof(float) * 3));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(sizeof(float) * 6));
            glEnableVertexAttribArray(2);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // light
            unsigned int lightVAO;
            glGenVertexArrays(1, &lightVAO);
            glBindVertexArray(lightVAO);

            unsigned int lightVBO;
            glGenBuffers(1, &lightVBO);
            glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)0);
            glEnableVertexAttribArray(0);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            unsigned int diffuseMap;
            glGenTextures(1, &diffuseMap);
            glBindTexture(GL_TEXTURE_2D, diffuseMap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            int width, height, channels;
            auto data = stbi_load("assets/container2.png", &width, &height, &channels, 0);
            if (data == nullptr)
            {
                LOG_E("Failed to load diffuse map: %s", "assets/container2.png");
                glfwTerminate();
                return -1;
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
                data = nullptr;
            }

            unsigned int specularMap;
            glGenTextures(1, &specularMap);
            glBindTexture(GL_TEXTURE_2D, specularMap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            data = stbi_load("assets/container2_specular.png", &width, &height, &channels, 0);
            if (data == nullptr)
            {
                LOG_E("Failed to load specular map: %s", "assets/container2_specular.png");
                glfwTerminate();
                return -1;
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
                data = nullptr;
            }

            Shader *cubeShader = new Shader("shaders/lighting/06/cube.vs", "shaders/lighting/06/cube.fs");
            Shader *lightShader = new Shader("shaders/lighting/06/light.vs", "shaders/lighting/06/light.fs");

            glm::vec3 cubePositions[] = {
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 5.0f, -15.0f),
                glm::vec3(-1.5f, -2.2f, -2.5f),
                glm::vec3(-3.8f, -2.0f, -12.3f),
                glm::vec3(2.4f, -0.4f, -3.5f),
                glm::vec3(-1.7f, 3.0f, -7.5f),
                glm::vec3(1.3f, -2.0f, -2.5f),
                glm::vec3(1.5f, 2.0f, -2.5f),
                glm::vec3(1.5f, 0.2f, -1.5f),
                glm::vec3(-1.3f, 1.0f, -1.5f)};
            // positions of the point lights
            glm::vec3 pointLightPositions[] = {
                glm::vec3(0.7f, 0.2f, 2.0f),
                glm::vec3(2.3f, -3.3f, -4.0f),
                glm::vec3(-4.0f, 2.0f, -12.0f),
                glm::vec3(0.0f, 0.0f, -3.0f)};
            glfwSwapInterval(1);
            while (!glfwWindowShouldClose(window))
            {
                processKeyboard(window);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glEnable(GL_DEPTH_TEST);

                glm::mat4 view = camera.GetViewMatrix();
                glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(screenWidth) / screenHeight, 0.1f, 100.0f);
                if (cubeShader->Use())
                {
                    glBindVertexArray(cubeVAO);
                    cubeShader->SetMatrix4("u_View", glm::value_ptr(view));
                    cubeShader->SetMatrix4("u_Projection", glm::value_ptr(projection));
                    cubeShader->SetVec3("u_ViewPos", camera.Position.x, camera.Position.y, camera.Position.z);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, diffuseMap);
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, specularMap);
                    cubeShader->SetInt("u_Material.diffuse", 0);
                    cubeShader->SetInt("u_Material.specular", 1);
                    cubeShader->SetVec3("u_Material.specular", 0.5f, 0.5f, 0.5f);
                    cubeShader->SetFloat("u_Material.shineness", 32.0f);

                    // directional light
                    cubeShader->SetVec3("u_DirLight.direction", -0.2f, -1.0f, -0.3f);
                    cubeShader->SetVec3("u_DirLight.ambient", 0.05f, 0.05f, 0.05f);
                    cubeShader->SetVec3("u_DirLight.diffuse", 0.4f, 0.4f, 0.4f);
                    cubeShader->SetVec3("u_DirLight.specular", 0.5f, 0.5f, 0.5f);
                    // point light
                    for (size_t i = 0; i < 4; i++)
                    {
                        auto position = pointLightPositions[i];
                        std::stringstream pointLightNameBuilder;
                        pointLightNameBuilder << "u_PointLights[" << i << "].";
                        auto pointLightName = pointLightNameBuilder.str();
                        cubeShader->SetVec3(pointLightName + "position", position.x, position.y, position.z);
                        cubeShader->SetVec3(pointLightName + "ambient", 0.05f, 0.05f, 0.05f);
                        cubeShader->SetVec3(pointLightName + "diffuse", 0.8f, 0.8f, 0.8f);
                        cubeShader->SetVec3(pointLightName + "specular", 1.0f, 1.0f, 1.0f);
                        cubeShader->SetFloat(pointLightName + "constant", 1.0f);
                        cubeShader->SetFloat(pointLightName + "linear", 0.09f);
                        cubeShader->SetFloat(pointLightName + "quadratic", 0.032f);
                    }
                    // spotLight
                    cubeShader->SetVec3("u_SpotLight.position", camera.Position.x, camera.Position.y, camera.Position.z);
                    cubeShader->SetVec3("u_SpotLight.direction", camera.Front.x, camera.Front.y, camera.Front.z);
                    cubeShader->SetVec3("u_SpotLight.ambient", 0.0f, 0.0f, 0.0f);
                    cubeShader->SetVec3("u_SpotLight.diffuse", 1.0f, 1.0f, 1.0f);
                    cubeShader->SetVec3("u_SpotLight.specular", 1.0f, 1.0f, 1.0f);
                    cubeShader->SetFloat("u_SpotLight.constant", 1.0f);
                    cubeShader->SetFloat("u_SpotLight.linear", 0.09);
                    cubeShader->SetFloat("u_SpotLight.quadratic", 0.032);
                    cubeShader->SetFloat("u_SpotLight.cutOff", glm::cos(glm::radians(12.5f)));
                    cubeShader->SetFloat("u_SpotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

                    for (size_t i = 0; i < 10; i++)
                    {
                        glm::mat4 model = glm::mat4(1.0f);
                        model = glm::translate(model, cubePositions[i]);
                        float angle = 20.0f * i;
                        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                        cubeShader->SetMatrix4("u_Model", glm::value_ptr(model));
                        glDrawArrays(GL_TRIANGLES, 0, 36);
                    }
                }
                else
                {
                    break;
                }
                if (lightShader->Use())
                {
                    glBindVertexArray(lightVAO);
                    lightShader->SetMatrix4("u_View", glm::value_ptr(view));
                    lightShader->SetMatrix4("u_Projection", glm::value_ptr(projection));
                    for (size_t i = 0; i < 4; i++)
                    {
                        auto position = pointLightPositions[i];
                        glm::mat4 model = glm::mat4(1.0f);
                        model = glm::translate(model, position);
                        model = glm::scale(model, glm::vec3(0.2f));
                        lightShader->SetMatrix4("u_Model", glm::value_ptr(model));
                        glDrawArrays(GL_TRIANGLES, 0, 36);
                    }
                }
                else
                {
                    break;
                }

                glfwSwapBuffers(window);
                glfwPollEvents();
            }
            glfwTerminate();
            return 0;
        }
    } // namespace multiple_lights
} // namespace lighting