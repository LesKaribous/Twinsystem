#pragma once
#include "Event.h"
#include <sstream>
#include "math/Geometry.h"

namespace TwinSystem{

    class BoolChangedEvent : public Event{
	public:
		BoolChangedEvent(std::string str, bool value) : _str(str), _state(value) {}

		inline bool GetValue() const { return _state; }

		std::string ToString() const override{
			std::stringstream ss;
			ss << "BoolChangedEvent: " << _str << " : " << _state ? "true" : "false";
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::BoolChangedEvent; } 
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "BoolChangedEvent"; }

		EVENT_CLASS_CATEGORY(EventCategoryMonitoring);

	private:
		bool _state;
        std::string _str;
	};



    class IntChangedEvent : public Event{
	public:
		IntChangedEvent(std::string str, int value) : _str(str), _state(value) {}

		inline int GetValue() const { return _state; }

		std::string ToString() const override{
			std::stringstream ss;
			ss << "IntChangedEvent: " << _str << " : " << _state;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::IntChangedEvent; } 
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "IntChangedEvent"; }

		EVENT_CLASS_CATEGORY(EventCategoryMonitoring);

	private:
		int _state;
        std::string _str;
	};


    class FloatChangedEvent : public Event{
	public:
		FloatChangedEvent(std::string str, float value) : _str(str), _state(value) {}

		inline float GetValue() const { return _state; }

		std::string ToString() const override{
			std::stringstream ss;
			ss << "FloatChangedEvent: " << _str << " : " << _state;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::FloatChangedEvent; } 
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "FloatChangedEvent"; }

		EVENT_CLASS_CATEGORY(EventCategoryMonitoring);

	private:
		float _state;
        std::string _str;
	};



    class Vec3ChangedEvent : public Event{
	public:
		Vec3ChangedEvent(std::string str, Vec3 value) : _str(str), _state(value) {}

		inline Vec3 GetValue() const { return _state; }

		std::string ToString() const override{
			std::stringstream ss;
			ss << "Vec3ChangedEvent: " << _str << " : {" << _state.a << ", "<< _state.b << ", " << _state.c << "}";
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::Vec3ChangedEvent; } 
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "Vec3ChangedEvent"; }

		EVENT_CLASS_CATEGORY(EventCategoryMonitoring);

	private:
		Vec3 _state;
        std::string _str;
	};


    template<typename T>
    class ValueTracker {
    public:
        ValueTracker() {}

        void SetValue(T newValue) {
            _hasChanged = false;
            if (newValue != _value) {
                _value = newValue;
                OnValueChanged();
            }
        }

        T& GetValue() {
            return _value;
        }

        bool HasChanged(){
            return(_hasChanged);
        };

    private:
        void OnValueChanged(){
            _hasChanged = true;
        }

        T _value;
        bool _hasChanged;
    };
        
} // namespace TwinSystem

    