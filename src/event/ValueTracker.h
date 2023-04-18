#pragma once
#include "Event.h"
#include "math/Geometry.h"
#include "debug/Console.h"

namespace TwinSystem{

    class BoolChangedEvent : public Event{
	public:
		BoolChangedEvent(String str, bool value) : _str(str), _state(value) {}

		inline bool GetValue() const { return _state; }

		String ToString() const override{
			String ss;
			ss += GetName() + _str + String(" : {") + String(_state) + "}";
			return ss;
		}

		static EventType GetStaticType() { return EventType::BoolChangedEvent; } 
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "BoolChangedEvent"; }

		EVENT_CLASS_CATEGORY(EventCategoryMonitoring);

	private:
		bool _state;
        String _str;
	};



    class IntChangedEvent : public Event{
	public:
		IntChangedEvent(String str, int value) : _str(str), _state(value) {}

		inline int GetValue() const { return _state; }

		String ToString() const override{
			String ss;
			ss += GetName() + _str + String(" : {") + String(_state) + "}";
			return ss;
		}

		static EventType GetStaticType() { return EventType::IntChangedEvent; } 
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "IntChangedEvent"; }

		EVENT_CLASS_CATEGORY(EventCategoryMonitoring);

	private:
		int _state;
        String _str;
	};


    class FloatChangedEvent : public Event{
	public:
		FloatChangedEvent(String str, float value) : _str(str), _state(value) {}

		inline float GetValue() const { return _state; }

		String ToString() const override{
			String ss;
			ss += GetName() + _str + String(" : {") + String(_state) + "}";
			return ss;
		}

		static EventType GetStaticType() { return EventType::FloatChangedEvent; } 
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "FloatChangedEvent"; }

		EVENT_CLASS_CATEGORY(EventCategoryMonitoring);

	private:
		float _state;
        String _str;
	};



    class Vec3ChangedEvent : public Event{
	public:
		Vec3ChangedEvent(String str, Vec3 value) : _str(str), _state(value) {}

		inline Vec3 GetValue() const { return _state; }

		String ToString() const override{
			String ss;
			ss += GetName() + _str + String(" : {") + String(_state.a) + ", "+ _state.b + ", " + _state.c + "}";
			return ss;
		}

		static EventType GetStaticType() { return EventType::Vec3ChangedEvent; } 
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "Vec3ChangedEvent"; }

		EVENT_CLASS_CATEGORY(EventCategoryMonitoring);

	private:
		Vec3 _state;
        String _str;
	};


    template<typename T>
    class ValueTracker {
    public:
        ValueTracker() {}

        void SetValue(T newValue) {
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

    