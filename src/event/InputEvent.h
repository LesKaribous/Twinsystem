#pragma once
#include "event/Event.h"
#include <sstream>

namespace TwinSystem
{
    class InputEvent : public Event{
	public:
		InputEvent(std::string str, bool state) : _str(str), _state(state) {}

		inline bool state() const { return _state; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputEvent: " << _str << " : " << _state;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::InputEvent; } 
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "InputEvent"; }

		EVENT_CLASS_CATEGORY(EventCategoryInput)
	private:
		bool _state;
        std::string _str;
	};
} // namespace TwinSystem
