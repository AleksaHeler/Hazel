#pragma once
// Defining a window interface that needs to be implemented per platform
// WindowProps structure
// Window class

// Precompiled headers
#include "hzpch.h"

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel {

	// Window properties structure
	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Hazel Engine",
				unsigned int width = 1280,
				unsigned int height = 720)
			: Title(title), Width(width), Height(height) {}
	};

	// Interface presenting a desktop system based Window
	// No data, no functions - pure virtual stuff
	// Because this has to be implemented per platform
	class HAZEL_API Window {
	public:
		// This is just a standard function which returns void and takes Event as an argument
		using EventCallbackFn = std::function<void(Event&)>;

		// Virtual destructor
		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		// Getters
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}
