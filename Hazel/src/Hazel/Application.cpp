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

		// ---- DRAWING A TRIANGLE ----
		// Vertex array
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);
		// Vertex buffer
		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		// Three points with three dimensions
		// Space is from -1 to 1
		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f, // first point
			 0.5f, -0.5f, 0.0f, // second point (anti-clockwise)
			 0.0f,  0.5f, 0.0f  // third point
		};

		// Send this data to GPU
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Tell OpenGL in what format the data is
		glEnableVertexAttribArray(0);
		// Describing data at index zero, three floats, dont normalize, space between points
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr); 

		// Index buffer
		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		// Tell in what order to connect vertices
		unsigned int indices[3] = { 0, 1, 2 };
		// Send this data to GPU
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		// ----------------------------
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
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			// Draw the triangle we set up in Application constructor
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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
