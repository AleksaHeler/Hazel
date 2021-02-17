#include "hzpch.h"
#include "Application.h"
// Defining Run() function of the Application class

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Log.h"

#include <GLFW/glfw3.h>

namespace Hazel {

	// Constructor
	Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	// Destructor
	Application::~Application() {}

	// Run
	void Application::Run() {
		while (m_Running) {
			// Set a pink window
			glClearColor(1, 1, 0, 1);
			//glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			// Call on update every frame
			m_Window->OnUpdate();
		}
	}
}
