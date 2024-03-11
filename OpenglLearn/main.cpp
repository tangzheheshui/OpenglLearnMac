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
#include <time.h>

#include "render/scene.hpp"
#include "camera.hpp"
#include "input.hpp"
#include "core/taskQueue.h"
 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

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
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetInputMode(window, GLFW_CURSOR_NORMAL, GLFW_CURSOR_DISABLED);
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    InputProcessMng::setFrameBufferSize(width, height);
    
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

    auto start = std::chrono::high_resolution_clock::now();
    
    TaskQueue& taskQueue = TaskQueue::instance();
    std::vector<std::thread> workers;
    for (int i = 0; i < 5; ++i) {
        workers.emplace_back([&taskQueue]() {
            taskQueue.executeTasks();
        });
    }
    
    auto a = Scene::getScene();
    
    while (!taskQueue.is_empty()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "load time = " << duration.count() << std::endl;
    
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
 
        Scene::getScene().update();
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << "frame_time1 = " << duration.count() << std::endl;
        
        // render
        Scene::getScene().drawShadow();
        
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::cout << "frame_time2 = " << duration.count() << std::endl;
        
        Scene::getScene().draw();
        
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::cout << "frame_time3 = " << duration.count() << std::endl;
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::cout << "frame_time = " << duration.count() << std::endl;
        start = end;
    }
 
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
 
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    
    for (auto &work : workers) {
        work.detach();
    }
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
    InputProcessMng::processScroll(xoffset, yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        //std::cout << "Key pressed: " << key << std::endl;
    }
    else if (action == GLFW_RELEASE)
    {
        InputProcessMng::processKeyRelease(key);
    }
}

// 鼠标点击回调函数
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        InputProcessMng::processMouseLeftKeyUp(xpos, ypos);
    }
}
