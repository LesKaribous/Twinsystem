#pragma once

#include "core/Core.h"

namespace TwinSystem{
	enum class EventType
	{
		None = 0,
		BoolChangedEvent,
		IntChangedEvent,
		FloatChangedEvent,
		Vec3ChangedEvent,
		SystemEvent,
        SensorEvent,
		InputEvent,
		MoveEvent,
        ActuatorEvent,
        EmergencyStop,
        BatteryLow,
        MatchFinished,
        RemoteCommand
	};

	enum EventCategory
	{
		None = 0,
		EventCategorySystem     = BIT(0),
        EventCategoryInput      = BIT(1),
		EventCategoryUI         = BIT(2),
		EventCategoryLidar      = BIT(3),
		EventCategoryMatch      = BIT(4),
		EventCategorySensor     = BIT(5),
		EventCategoryActuator   = BIT(6),
		EventCategoryMotion     = BIT(7),
		EventCategoryMonitoring = BIT(8)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }


#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }


	class Event
	{
	public:
		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual String ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};


    class EventDispatcher{
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}
		
		// F will be deduced by the compiler
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

}
