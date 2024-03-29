#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <ctime>
#include "../utils/log.h"

// https://learnopengl-cn.github.io/01%20Getting%20started/03%20Hello%20Window
namespace getting_started
{
    namespace hello_window
    {
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
            if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)))
            {
                LOG_E("Failed to initialize GLAD");
                glfwTerminate();
                return -1;
            }
            else
            {
                LOG_I("GLVersion: %s GLVendor: %s", glGetString(GL_VERSION), glGetString(GL_VENDOR));
            }
            glViewport(0, 0, 800, 600);
            glfwSetFramebufferSizeCallback(window, [](GLFWwindow *win, int width, int height) -> void {
                glViewport(0, 0, width, height);
            });

            while (!glfwWindowShouldClose(window))
            {

                if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                {
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                    LOG_I("ESCAPE pressed, exiting");
                }

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                // drawing here

                glfwSwapBuffers(window);
                glfwPollEvents();
            }
            glfwTerminate();
            return 0;
        }
    } // namespace hello_window
} // namespace getting_started
