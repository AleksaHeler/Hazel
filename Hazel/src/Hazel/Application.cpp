#include "Application.h"
// Defining Run() function of the Application class

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Log.h"

namespace Hazel {

	Application::Application() {}

	Application::~Application() {}

	void Application::Run() {
		WindowResizeEvent e(1280, 720);
		HZ_TRACE(e);

		while (true);
	}
}
