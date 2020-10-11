//
//  05_framebuffers.cpp
//
//  Created by Clsrfish on 11/10/2020
//

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
    namespace framebuffers
    {
        float cubeVertices[] = {
            // positions          // texture Coords
            // Back face
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-right
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // bottom-right
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-right
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // top-left
            // Front face
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // bottom-right
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // top-right
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // top-right
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  // top-left
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
            // Left face
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // top-right
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // top-left
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom-right
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // top-right
                                             // Right face
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // top-left
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // bottom-right
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-right
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // bottom-right
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // top-left
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // bottom-left
            // Bottom face
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  // top-left
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // bottom-left
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // bottom-left
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom-right
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
            // Top face
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // bottom-right
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // top-right
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // bottom-right
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f   // bottom-left
        };

        float planeVertices[] = {
            // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
            5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
            -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
            -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,

            5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
            5.0f, -0.5f, -5.0f, 2.0f, 2.0f,
            -5.0f, -0.5f, -5.0f, 0.0f, 2.0f};

        float quadVertices[] = {
            // positions   // texCoords
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f};

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

            // cube
            unsigned int cubeVAO, cubeVBO;
            glGenVertexArrays(1, &cubeVAO);
            glBindVertexArray(cubeVAO);
            glGenBuffers(1, &cubeVBO);
            glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(0));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(sizeof(float) * 3));
            glEnableVertexAttribArray(1);
            glBindVertexArray(0);
            // plane
            unsigned int planeVAO, planeVBO;
            glGenVertexArrays(1, &planeVAO);
            glBindVertexArray(planeVAO);
            glGenBuffers(1, &planeVBO);
            glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(0));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(sizeof(float) * 3));
            glEnableVertexAttribArray(1);
            glBindVertexArray(0);

            // quad
            unsigned int quadVAO, quadVBO;
            glGenVertexArrays(1, &quadVAO);
            glBindVertexArray(quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)(0));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)(sizeof(float) * 2));
            glEnableVertexAttribArray(1);
            glBindVertexArray(0);

            unsigned int cubeTexture = TextureFromFile("assets/container.jpg");
            unsigned int floorTexture = TextureFromFile("assets/metal.png");

            Shader shader("shaders/advanced_gl/05/shader.vs", "shaders/advanced_gl/05/shader.fs");
            Shader quadShader("shaders/advanced_gl/05/frame.vs", "shaders/advanced_gl/05/frame.fs");

            // frame buffers
            unsigned int frameBuffer;
            glGenFramebuffers(1, &frameBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
            // color attachments
            unsigned int frameBufferTex;
            glGenTextures(1, &frameBufferTex);
            glBindTexture(GL_TEXTURE_2D, frameBufferTex);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTex, 0);
            // depth and stencil attachments
            unsigned int frameBufferRBO;
            glGenRenderbuffers(1, &frameBufferRBO);
            glBindRenderbuffer(GL_RENDERBUFFER, frameBufferRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frameBufferRBO);

            // check
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                LOG_E("Framebuffer is not complete!");
                glfwTerminate();
                return -1;
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glm::vec3 cubePositions[] = {
                glm::vec3(-1.0f, 0.0f, -1.0f),
                glm::vec3(2.0f, 0.0f, 0.0f)};

            glfwSwapInterval(1);
            while (!glfwWindowShouldClose(window))
            {
                processKeyboard(window);
                glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glEnable(GL_DEPTH_TEST);

                glEnable(GL_CULL_FACE);
                glFrontFace(GL_CCW);
                glCullFace(GL_BACK);

                if (!shader.Use())
                {
                    break;
                }
                glm::mat4 view = camera.GetViewMatrix();
                glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / screenHeight, 0.1f, 100.0f);

                shader.SetMatrix4("u_View", glm::value_ptr(view));
                shader.SetMatrix4("u_Projection", glm::value_ptr(projection));

                // floor
                glBindVertexArray(planeVAO);
                glBindTexture(GL_TEXTURE_2D, floorTexture);
                shader.SetMatrix4("u_Model", static_cast<const float *>(glm::value_ptr(glm::mat4(1.0f))));
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);

                // cubes
                glBindVertexArray(cubeVAO);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, cubeTexture);
                for (unsigned int i = 0; i < 2; i++)
                {
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, cubePositions[i]);
                    shader.SetMatrix4("u_Model", glm::value_ptr(model));
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }

                // render texture from frameBuffer
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                quadShader.Use();
                glBindVertexArray(quadVAO);
                glDisable(GL_DEPTH_TEST);
                glBindTexture(GL_TEXTURE_2D, frameBufferTex);
                glDrawArrays(GL_TRIANGLES, 0, 6);

                glfwSwapBuffers(window);
                glfwPollEvents();
            }
            return 0;
        }
    } // namespace framebuffers
} // namespace advanced_ogl