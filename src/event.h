#pragma once

#include "geometry.h"

enum class EventType {
    unknown,
    key,
    mouse_move,
    mouse_down,
    quit
};

class IEvent {
public:
    virtual EventType type() = 0; 
};
using Events = std::vector<IEvent*>;

// specific events

class EventKey : public IEvent {
public:
    virtual EventType type() final { return EventType::key; } 

    uint16_t _vk;
    bool _up;

    EventKey() : _vk(0), _up(false) {}
    EventKey(uint16_t vk, bool up) : _vk(vk), _up(up) {}
};

class EventMouseMove : public IEvent {
public:
    virtual EventType type() final { return EventType::mouse_move; } 

    Point _cursor;

    EventMouseMove() : _cursor(0,0) {}
    EventMouseMove(const Point& cursor) : _cursor(cursor) {}
};

class EventMouseButton : public IEvent {
public:
    virtual EventType type() final { return EventType::mouse_down; } 

    enum class MouseButton {
        unknown,
        LeftButton,
        RightButton,
    };

    Point _cursor;
    MouseButton _mb;
    bool _up;

    EventMouseButton() : _cursor(0, 0), _mb(MouseButton::unknown), _up(false) {}
    EventMouseButton(const Point& cursor, MouseButton mb, bool up) : _cursor(cursor), _mb(mb), _up(up) {}
};

class EventQuit : public IEvent {
public:
    virtual EventType type() final { return EventType::quit; } 
};

