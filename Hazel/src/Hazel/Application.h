#pragma once
// Defining Application class, which contains function Run()
// Declaring function CreateApplication() which will be defined in Client

#include "Core.h"

namespace Hazel {

	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in Client
	Application* CreateApplication();
}
