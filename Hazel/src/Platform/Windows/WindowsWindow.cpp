#include "hzpch.h"
#include "WindowsWindow.h"
// When we create the Windows window it calls the constructor which calls the Init() function
// It sets up all our data from properties, initializes GLFW if it hasn't yet

namespace Hazel {
	static bool s_GLFWInitialized = false;

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

	// Init (properties)
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

			s_GLFWInitialized = true;
		}

		// Creating the glfw window, make context current, set a window user pointer
		// 
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

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
