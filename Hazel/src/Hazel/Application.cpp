#include "hzpch.h"
#include "Application.h"
// Defining Run() function of the Application class

#include <GLFW/glfw3.h>

namespace Hazel {

// Bind function (x) as event callback function
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	// Constructor
	Application::Application() {
		// Create a window
		m_Window = std::unique_ptr<Window>(Window::Create());
		// Bind OnEvent function as event callback fn
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	// Destructor
	Application::~Application() {}

	// PushLayer
	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
	}

	// PushOverlay
	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
	}

	// OnEvent
	void Application::OnEvent(Event& e) {
		// Create event dispatcher and bind the right function for specific event
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		// Log all events
		// HZ_CORE_TRACE("{0}", e);

		// Go backwards trough the layer stack and pass events
		// If the event has already been handled dont continue
		// So if the overlay handles an event that is it, other layers wont get it
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(e);
			if (e.IsHandled())
				break;
		}
	}

	// Run
	void Application::Run() {
		while (m_Running) {
			// Set a pink window
			glClearColor(1, 1, 0, 1);
			//glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			// Call OnUpdate functions in all layers
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			// Call on update every frame
			m_Window->OnUpdate();
		}
	}

	// On Window Close event function
	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}
