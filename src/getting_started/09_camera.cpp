#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <algorithm>
#include "../gl/shader.h"
#include "../utils/log.h"
#include "../gl/gl_utils.h"

namespace camera
{
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    unsigned short indices[] = {
        0, 1, 3,  // 第一个三角形
        1, 2, 3}; // 第二个三角形

    int screenWidth = 800;
    int screenHeight = 600;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float lastFrameTime = -1.0f;

    void processInput(GLFWwindow *window)
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
        float cameraSpeed = 2.5f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraPos += cameraSpeed * cameraFront;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            cameraPos -= cameraSpeed * cameraFront;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
        }
    }

    bool firstMove = false;
    float lastCursorX = 0.0f;
    float lastCursorY = 0.0f;
    float pitch = 0.0f;
    float yaw = -90.0f;

    void processCursorMove(GLFWwindow *window, double x, double y)
    {
        LOG_I("Cursor moving: %.3f, %.3f", x, y);
        if (firstMove)
        {
            lastCursorX = x;
            lastCursorY = y;
            firstMove = false;
        }
        // x increases from left to right
        // y increases from top to bottom
        float offsetX = x - lastCursorX;
        float offsetY = lastCursorY - y;
        lastCursorX = x;
        lastCursorY = y;

        offsetX *= 0.05f;
        offsetY *= 0.05f;

        yaw += offsetX;
        pitch += offsetY;

        pitch = std::max(-89.9f, pitch);
        pitch = std::min(89.9f, pitch);

        // new cameraFront
        glm::vec3 front;
        front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front.y = sin(glm::radians(pitch));
        front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        cameraFront = glm::normalize(front);
    }

    constexpr float MIN_FOV = 1.0f;
    constexpr float MAX_FOV = 60.0f;
    float fov = 45.0f;
    void processMouseScroll(GLFWwindow *window, double x, double y)
    {
        LOG_I("Mouse scrolling: fov=%.3f y=%.3f", fov, y);
        if (fov >= MIN_FOV && fov <= MAX_FOV)
        {
            fov -= y;
        }
        fov = std::max(MIN_FOV, fov);
        fov = std::min(MAX_FOV, fov);
    }

    int cameraImpl()
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
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_E("Failed to initialize GLAD loader");
            glfwTerminate();
            return -1;
        }
        else
        {
            printGLInfo();
        }
        glViewport(0, 0, screenWidth, screenHeight);
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow *, int w, int h) {
            screenWidth = w;
            screenHeight = h;
            glViewport(0, 0, w, h);
        });
        glfwSetCursorPosCallback(window, processCursorMove);
        glfwSetScrollCallback(window, processMouseScroll);

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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        unsigned int containerTex;
        glGenTextures(1, &containerTex);
        glBindTexture(GL_TEXTURE_2D, containerTex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int width, height, channels;
        unsigned char *data = stbi_load("assets/container.jpg", &width, &height, &channels, 0);
        if (data == nullptr)
        {
            LOG_E("Failed to load image: %s", "assets/container.jpg");
            glfwTerminate();
            return -1;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        unsigned int faceTex;
        glGenTextures(1, &faceTex);
        glBindTexture(GL_TEXTURE_2D, faceTex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        data = stbi_load("assets/awesomeface.png", &width, &height, &channels, 0);
        if (data == nullptr)
        {
            LOG_E("Failed to load image: %s", "assets/awesomeface.png");
            glfwTerminate();
            return -1;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        data = nullptr;

        auto shader = new Shader("shaders/getting_started/08/shader.vs", "shaders/getting_started/08/shader.fs");

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

        while (!glfwWindowShouldClose(window))
        {
            processInput(window);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glEnable(GL_DEPTH_TEST);
            if (!shader->Use())
            {
                break;
            }
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, containerTex);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, faceTex);
            shader->SetInt("texture0", 0);
            shader->SetInt("texture1", 1);

            glBindVertexArray(VAO);
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(fov), static_cast<float>(screenWidth) / screenHeight, 0.1f, 100.0f);
            shader->SetMatrix4("uView", glm::value_ptr(view));
            shader->SetMatrix4("uProjection", glm::value_ptr(projection));
            for (unsigned int i = 0; i < 10; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.0f, 0.0f));

                shader->SetMatrix4("uModel", glm::value_ptr(model));

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }

} // namespace camera

namespace getting_started
{
    void camera()
    {
        camera::cameraImpl();
    }
} // namespace getting_started
