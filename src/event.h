#pragma once

#include "geometry.h"

/*! \file
    \brief Declaration for Event descriptor and some definitions
*/

//! \brief supported event types
enum class EventType {
    unknown,
    key,
    mouse_move,
    mouse_button,
    quit
};

//! \brief base event descriptor
class IEvent
{
public:
//! \brief event descriptor type
    virtual EventType type() = 0;
};
//! \brief sequence of event descriptors
using Events = std::vector<IEvent*>;

//! \brief Event handler
class IEventHandler
{
public:
//! \brief process event descriptor and respond with new event handler which must process next event in queue or nullptr if we need to finish processing
    virtual IEventHandler* process(IEvent*) = 0;
//! \brief must be called before new sequence of event will be processed by this event handler
    virtual void enter() = 0;
//! \brief must be called after sequence of event was processed by this event handler
    virtual void leave() = 0;
};

// specific events

//! \brief Event descriptor for events related to key pressing
class EventKey : public IEvent
{
public:
    virtual EventType type() final { return EventType::key; }

//! \brief scancode
    uint16_t _vk;
    bool _up;

    EventKey() : _vk(0), _up(false) {}
    EventKey(uint16_t vk, bool up) : _vk(vk), _up(up) {}
};

//! \brief Event descriptor for events related to mouse motion
class EventMouseMove : public IEvent
{
public:
    virtual EventType type() final { return EventType::mouse_move; }

    Point _cursor;

    EventMouseMove() : _cursor(0,0) {}
    EventMouseMove(const Point& cursor) : _cursor(cursor) {}
};

//! \brief Event descriptor for events related to pressing mouse button
class EventMouseButton : public IEvent
{
public:
    virtual EventType type() final { return EventType::mouse_button; }

    enum class MouseButton {
        unknown,
        Left,
        Right,
    };

    Point _cursor;
    MouseButton _mb;
    bool _up;

    EventMouseButton() : _cursor(0, 0), _mb(MouseButton::unknown), _up(false) {}
    EventMouseButton(const Point& cursor, MouseButton mb, bool up) : _cursor(cursor), _mb(mb), _up(up) {}
};

//! \brief Event descriptor for event related to finishing application
class EventQuit : public IEvent
{
public:
    virtual EventType type() final { return EventType::quit; }
};

