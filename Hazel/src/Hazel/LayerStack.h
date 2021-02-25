#pragma once

#include "Hazel/Core.h"
#include "Layer.h"

#include <vector>

namespace Hazel {

	// A wrapper for layer vector
	// Its not an actual stack because we push things in the middle of it
	// Layers are owned by the layer stack as soon as you give it to layer stack
	class HAZEL_API LayerStack {
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		// Begin/end wrappers
		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		// Division between layers and overlays
		// Overlays should always be at the end to be rendered last
		unsigned int m_LayerInsertIndex = 0;
	};



}