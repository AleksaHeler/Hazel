#include "hzpch.h"
#include "Application.h"
// Defining Run() function of the Application class

#include "Hazel/Log.h"

#include <glad/glad.h>

#include "Input.h"

namespace Hazel {

// Bind function (x) as event callback function
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	// Singleton
	Application* Application::s_Instance = nullptr;

	// Constructor
	Application::Application() {
		// Singleton
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Create a window
		m_Window = std::unique_ptr<Window>(Window::Create());

		// Bind OnEvent function as event callback fn
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	// Destructor
	Application::~Application() {
	
	}

	// PushLayer
	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	// PushOverlay
	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
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
			// Set window color
			glClearColor(0.416, 0.051, 0.769, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			// Call OnUpdate functions in all layers
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			// Log mouse position
			// auto [x, y] = Input::GetMousePosition();
			// HZ_CORE_TRACE("{0}, {1}", x, y);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

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
