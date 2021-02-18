#include "hzpch.h"
#include "WindowsWindow.h"
// When we create the Windows window it calls the constructor which calls the Init() function
// It sets up all our data from properties, initializes GLFW if it hasn't yet

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"

#include <glad/glad.h>

namespace Hazel {
	static bool s_GLFWInitialized = false;

	// On Error, print it
	static void GLFWErrorCallback(int error, const char* description) {
		HZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	// Create (properties)
	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	// Constructor
	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	// Destructor
	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	// Init (properties) - initializes GLFW and creates openGL context
	void WindowsWindow::Init(const WindowProps& props) {
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		HZ_CORE_INFO("Creating window {} ({}, {})", props.Title, props.Width, props.Height);

		// Only initialize GLFW once, when we initialize window
		// But we might create multiple windows
		if (!s_GLFWInitialized) {
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			HZ_CORE_ASSERT(success, "Could not initiate GLFW!");
			// Set error callback function
			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

		// Creating the glfw window, make context current, set a window user pointer
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);

		// Initializing Glad
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callback for window resize event, using a lambda function
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			// Get the window data
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			// Update data
			data.Width = width;
			data.Height = height;
			// Create the event with given parameters
			WindowResizeEvent event(width, height);
			// Dispatch the created event
			data.EventCallback(event);
		});

		// Set GLFW callback for window close event, using a lambda function
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			// Get the window data, create event and dispatch it
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		// Set GLFW callback for key event, using a lambda function
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			// Get the window data
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			// Create right event and call the callback function
			switch (action) {
				case GLFW_PRESS: {
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		// Set GLFW callback for mouse button event, using a lambda function
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			// Get the window data
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			// Create right event and call the callback function
			switch (action) {
				case GLFW_PRESS: {
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		// Set GLFW callback for mouse scroll event, using a lambda function
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			// Get the window data, create event and dispatch it
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		// Set GLFW callback for cursor position event, using a lambda function
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			// Get the window data, create event and dispatch it
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	} // void WindowsWindow::Init(const WindowProps& props)

	// Shutdown
	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(m_Window);
	}
	
	// OnUpdate
	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	// SetVSync
	void WindowsWindow::SetVSync(bool enabled) {
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	// IsVSync
	bool WindowsWindow::IsVSync() const {
		return m_Data.VSync;
	}
}
