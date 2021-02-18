#include <Hazel.h>

// An example of layer
class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
		: Layer("Example"){}

	void OnUpdate() override {
		// Log OnUpdate event
		HZ_INFO("ExampleLayer::Update");
	}

	void OnEvent(Hazel::Event& event) override {
		// Log all events this layer gets
		HZ_TRACE("{0}", event);
	}
};

class Sandbox : public Hazel::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Hazel::ImGuiLayer());
	}
	~Sandbox() {

	}
};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}
