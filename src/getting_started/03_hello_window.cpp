#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <ctime>

#ifdef Windows
// Windows下文件目录层级是'\\'
#define __FILENAME__ (strrchr(__FILE__, '\\') + 1)
#elif Linux
// Linux下文件目录层级是'/'
#define __FILENAME__ (strrchr(__FILE__, '/') + 1)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') + 1)
#endif

// https://learnopengl-cn.github.io/01%20Getting%20started/03%20Hello%20Window
namespace
{
    int main()
    {
        std::cout << __FILENAME__ << std::endl;
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
        std::cout << "Running on macOS, enable forward compatibilty." << std::endl;
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

        GLFWwindow *window = glfwCreateWindow(800, 600, __FILENAME__, nullptr, nullptr);
        if (window == nullptr)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            glfwTerminate();
            return -1;
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
} // namespace

void helloWindow()
{
    main();
}
