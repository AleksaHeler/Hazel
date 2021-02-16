#pragma once
// Say we have a CreateApplication function already declared somewhere (Client)
// Here we call that function which returns a pointer to Application class
// Then call the Run() function
// And then delete the dynamically allocated Application object

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

// Create applicaion, run it and then delete it
int main(int argc, char** argv) {
	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif // HZ_PLATFORM_WINDOWS

