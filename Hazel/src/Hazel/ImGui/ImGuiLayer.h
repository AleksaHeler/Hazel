#pragma once

#include "Hazel/Layer.h"

namespace Hazel {

	// A specific layer where we will render debug things
	// Always drawn last, on top of scene
	class HAZEL_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	private:
		float m_Time = 0.0f;
	};

}