#include "hzpch.h"
#include "WindowsInput.h"

#include "Hazel/Application.h"
#include <GLFW/glfw3.h>

namespace Hazel {

	Input* Input::s_Instance = new WindowsInput();

	// IsKeyPressedImpl
	bool WindowsInput::IsKeyPressedImpl(int keycode) {
		// Get native GLFW window, get key state and return true if pressed
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	// IsMouseButtonPressedImpl
	bool WindowsInput::IsMouseButtonPressedImpl(int button) {
		// Get native GLFW window, get mouse button state and return true if pressed
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	// GetMousePositionImpl
	std::pair<float, float> WindowsInput::GetMousePositionImpl() {
		// Get native GLFW window, get mouse pos and return the pair
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	// GetMouseXImpl
	float WindowsInput::GetMouseXImpl() {
		// Get native GLFW window, get mouse pos and return X pos
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		// Or instead of above, we can just use (in C++17):
		//auto [xpos, ypos] = GetMousePositionImpl();
		return (float)xpos;
	}

	// GetMouseYImpl
	float WindowsInput::GetMouseYImpl() {
		// Get native GLFW window, get mouse pos and return Y pos
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		// Or instead of above, we can just use:
		//auto [xpos, ypos] = GetMousePositionImpl();
		return (float)ypos;
	}
}
