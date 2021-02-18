#pragma once
/* --- Main file for the event system ---
* Events in Hazel are currently blocking, meaning when an event occurs it
* immediately gets dispached and must be dealt with righ then and there.
* For the future, a better strategy might be to buffer events in an event
* bus and process them during the "event" part of the update stage.
*/

#include "hzpch.h"
#include "Hazel/Core.h"

namespace Hazel {

	// All the possible event types
	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication = BIT(0), // bit shift by x ("Core.h")
		EventCategoryInput       = BIT(1),
		EventCategoryKeyboard    = BIT(2),
		EventCategoryMouse       = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

// Macros to implement event type and category in specific event classes
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	// Main event class
	class HAZEL_API Event {
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		// Is this event in the given category
		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}
		inline bool IsHandled() {
			return m_Handled;
		}
	protected:
		// Has the event been handled
		bool m_Handled = false;
	};

	// A way for us to despatch events based on their type
	class EventDispatcher {
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event) {}

		template<typename T> 
		bool Dispatch(EventFn<T> func) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	// Output stream operator. Just converts event to string 
	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}
}