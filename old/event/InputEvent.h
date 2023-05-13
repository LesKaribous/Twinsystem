#pragma once
#include "event/Event.h"

namespace TwinSystem
{
    class InputEvent : public Event{
	public:
		InputEvent(String str, bool state) : _str(str), _state(state) {}

		inline bool state() const { return _state; }
		
		String ToString() const override{
			String ss;
			ss += GetName() + _str + String(" : {") + String(_state) + "}";
			return ss;
		}

		static EventType GetStaticType() { return EventType::InputEvent; } 
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "InputEvent"; }

		EVENT_CLASS_CATEGORY(EventCategoryInput)
	private:
		bool _state;
        String _str;
	};
} // namespace TwinSystem
