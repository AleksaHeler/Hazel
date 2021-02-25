#include <Hazel.h>

//#include "imgui/imgui.h"

// An example of layer
class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
		: Layer("Example") {}

	void OnUpdate() override {
		// Log OnUpdate event
		// HZ_INFO("ExampleLayer::Update");
		// Log Tab key press
		if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
			HZ_TRACE("TAB IS PRESSED! (poll)");
	}

	virtual void OnImGuiRender() override
	{
		//ImGui::Begin("Test");
		//ImGui::Text("Hello World");
		//ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override {
		// Log all events this layer gets
		// HZ_TRACE("{0}", event);
		// Log all key pressed events as a character (unprintable keys will be garbage)
		if (event.GetEventType() == Hazel::EventType::KeyPressed) {
			Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
			if (e.GetKeyCode() == HZ_KEY_TAB)
				HZ_TRACE("Tab key is pressed! (event)");
			else
				HZ_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public Hazel::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {

	}
};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}
