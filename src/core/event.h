#pragma once

enum class SystemEventType {
    MOTION_ENABLED,
    MOTION_DISABLED,
    MOTION_MOVE_STARTED,
    MOTION_MOVE_PAUSED,
    MOTION_MOVE_RESUMED,
    MOTION_MOVE_CANCELLED,
    MOTION_MOVE_COMPLETED,

    INPUTS_ENABLED,
    INPUTS_DISABLED,
    INPUTS_BUTTON_PRESSED,
    INPUTS_BUTTON_RELEASED,
    INPUTS_TEAM_CHANGED,
    INPUTS_STRATEGY_CHANGED,
    INPUTS_STARTER_ARMED,
    INPUTS_STARTER_UNARMED,
    INPUTS_STARTER_PULLED,

    LIDAR_ENABLED,
    LIDAR_DISABLED,
    LIDAR_OBSTACLE_DETECTED,
    LIDAR_NOTHING_DETECTED,

    

};

class Event {
public:
    virtual ~Event() = default;
    virtual EventType getType() const = 0;
};

class PositionUpdateEvent : public Event {
public:
    EventType getType() const override { return EventType::POSITION_UPDATE; }
    // Other members...
};