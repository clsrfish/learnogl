//
//  main.cpp
//
//  Created by Clsrfish on 15/10/2020
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./game.hpp"
#include "./resource_manager.hpp"
#include "../utils/log.h"
#include "../gl/gl_utils.h"

namespace breakout
{
    int screenWidth = 800;
    int screenHeight = 600;
    Game Breakout(screenWidth, screenHeight);

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
        GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "Breakout", nullptr, nullptr);
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
        glfwSetKeyCallback(window, [](GLFWwindow *win, int key, int scancode, int action, int mode) {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(win, GLFW_TRUE);
            }
            if (key > 0 && key < 1024)
            {
                Breakout.Keys[key] = (action == GLFW_PRESS || action == GLFW_REPEAT);
            }
        });
        GLClearError();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Breakout.Init();
        glfwSwapInterval(1);
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;
        while (!glfwWindowShouldClose(window))
        {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            glfwPollEvents();

            Breakout.ProcessInput(deltaTime);
            Breakout.Update(deltaTime);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            Breakout.Render();

            glfwSwapBuffers(window);
        }

        return 0;
    }
} // namespace breakout