#include "rcp_window.hpp"
#include "glad/glad.h"
#include <stdexcept>
#include <iostream>

namespace rcp {
#ifdef _DEBUG
	void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
		GLsizei length, const char* message, const void* userParam) {
        //taken from https://learnopengl.com/In-Practice/Debugging
        // ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

        std::cout << "---------------" << std::endl;
        std::cout << "Debug message (" << id << "): " << message << std::endl;

        switch (source)
        {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
        } std::cout << std::endl;

        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
        } std::cout << std::endl;

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
        } std::cout << std::endl;
        std::cout << std::endl;
	}
#endif
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
        rcp::Window* win = reinterpret_cast<rcp::Window*>(glfwGetWindowUserPointer(window));
        win->key_callback(key, scancode, action, mods);
    }

	Window::Window(int _width, int _height, const char* _title, bool _vsync):width(_width), height(_height), title(_title), vsync(_vsync){
		if (!glfwInit()) {
			throw std::runtime_error("Failed to initialize GLFW");
		}

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

		handle = glfwCreateWindow(width, height, title, NULL, NULL);
		if(!handle){
			glfwTerminate();
			throw std::runtime_error("Failed to create window");
		}

        if (vsync) {
            glfwSwapInterval(1);
        }
        else {
            glfwSwapInterval(0);
        }

		glfwMakeContextCurrent(handle);
        glfwSetWindowUserPointer(handle, this);
        glfwSetKeyCallback(handle, rcp::key_callback);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw std::runtime_error("Failed to initialize glad");
		}
#ifdef _DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

        for (int i = 0; i < keys.size(); i++) {
            keys[i] = false;
        }

        for (int i = 0; i < buttons.size(); i++) {
            buttons[i] = false;
        }
	}

	Window::~Window(){
        glfwDestroyWindow(handle);
        glfwTerminate();
	}

	void Window::pollEvents(){
        glfwPollEvents();
	}

    void Window::swapBuffers(){
        glfwSwapBuffers(handle);
    }

	void Window::clear(){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

    void Window::close(){
        glfwSetWindowShouldClose(handle, GLFW_TRUE);
    }

    void Window::key_callback(int key, int scancode, int action, int mods){
        keys[key] = action;
    }
}