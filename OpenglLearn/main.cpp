//
//  main.cpp
//  OpenglLearn
//
//  Created by liuhaifeng02 on 2023/12/30.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
 
#include <iostream>
#include <filesystem>
#include <fstream>

#include "render/scene.hpp"
#include "camera.hpp"
 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
 
// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;    // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

// static API
static std::vector<std::string> g_supportExtensions;
static void GetSupportExtensions() {
    if (!g_supportExtensions.empty())
        return;
    GLint n, i;
    glGetIntegerv(GL_NUM_EXTENSIONS, &n);
    for (i = 0; i < n; i++) {
        std::string extension = (char*)glGetStringi(GL_EXTENSIONS, i);
        std::cout << "extension = " << extension << std::endl;
        g_supportExtensions.push_back(extension);
    }
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
 
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        const char* description;
        glfwGetError(&description);
        printf("create window failed, code = %s\n", description);
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    GetSupportExtensions();
    Camera::GetCamera().setPosition({0,0,20});
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
 
        Scene::getScene().update();
        // render
        Scene::getScene().drawShadow();
        Scene::getScene().draw();
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
 
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
 
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
 
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
 
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    float yaw = Camera::GetCamera().getYaw();
    yaw += xoffset;
    float pitch = Camera::GetCamera().getPitch();
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    
    Camera::GetCamera().setPitch(pitch);
    Camera::GetCamera().setYaw(yaw);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    float fov = Camera::GetCamera().getFov();
    fov += (float)yoffset;
    if (fov < 3.0f)
        fov = 3.0f;
    if (fov > 90.0f)
        fov = 90.0f;
    printf("yoffset = %f\n", yoffset);
    
    Camera::GetCamera().setFov(fov);
    return;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        std::cout << "Key pressed: " << key << std::endl;
    }
    else if (action == GLFW_RELEASE)
    {
        if (key == GLFW_KEY_C) {
            // 读取当前帧缓冲的像素数据
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            unsigned char* pixels = new unsigned char[width * height * 3]; // 假设使用 RGB 颜色格式
            glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, pixels);

            // 写入图像数据到文件
            std::string rootPath = "/Users/liuhaifeng/personal/OpenglLearnMac/OpenglLearn/screen_picture/";
            std::ofstream imageFile(rootPath + "output_image.bmp", std::ios::binary);
            if (imageFile.is_open())
            {
                // BMP 文件头
                char bmpHeader[54] = {0};
                bmpHeader[0] = 'B';
                bmpHeader[1] = 'M';
                int imageSize = width * height * 3;
                int fileSize = imageSize + 54;
                bmpHeader[2] = fileSize & 0xFF;
                bmpHeader[3] = (fileSize >> 8) & 0xFF;
                bmpHeader[4] = (fileSize >> 16) & 0xFF;
                bmpHeader[5] = (fileSize >> 24) & 0xFF;
                bmpHeader[10] = 54;
                
                // 图像信息头
                bmpHeader[14] = 40;
                bmpHeader[18] = width & 0xFF;
                bmpHeader[19] = (width >> 8) & 0xFF;
                bmpHeader[20] = (width >> 16) & 0xFF;
                bmpHeader[21] = (width >> 24) & 0xFF;
                bmpHeader[22] = height & 0xFF;
                bmpHeader[23] = (height >> 8) & 0xFF;
                bmpHeader[24] = (height >> 16) & 0xFF;
                bmpHeader[25] = (height >> 24) & 0xFF;
                bmpHeader[26] = 1;
                bmpHeader[28] = 24;
                
                // 写入文件头
                imageFile.write(bmpHeader, 54);
                
                // 写入图像数据
                imageFile.write(reinterpret_cast<char*>(pixels), imageSize);
                
                // 关闭文件
                imageFile.close();
            }
            else
            {
                std::cerr << "Failed to open image file for writing" << std::endl;
            }
            
            
            // 释放内存
            delete[] pixels;
        }
        std::cout << "Key released: " << key << std::endl;
    }
}
