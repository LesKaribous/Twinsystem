#pragma once

#include "Event.h"

namespace TwinSystem {
	class SystemUpdateEvent : public Event{
	public:
		SystemUpdateEvent() {}

		EVENT_CLASS_TYPE(SystemUpdate)
		EVENT_CLASS_CATEGORY(EventCategorySystem)
	};
}