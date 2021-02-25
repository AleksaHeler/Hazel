#include "hzpch.h"
#include "LayerStack.h"

namespace Hazel {

	// Constructor
	LayerStack::LayerStack() { }

	// Destructor
	LayerStack::~LayerStack() {
		// Delete all layers
		for (Layer* layer : m_Layers)
			delete layer;
	}

	// PushLayer - layers get pushed in the first half of the list
	// When it is pushed it will be as a last layer, but not in front overlays
	void LayerStack::PushLayer(Layer* layer) {
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}
	
	// PushOverlay - overlays get pushed in the second half of the list
	// Because overlays should allways be rendered last, they get pushed at the end
	void LayerStack::PushOverlay(Layer* overlay) {
		m_Layers.emplace_back(overlay);
	}

	// PopLayer
	void LayerStack::PopLayer(Layer* layer) {
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	// PopOverlay
	void LayerStack::PopOverlay(Layer* overlay) {
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
		}
	}
}
