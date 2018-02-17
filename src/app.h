#pragma once

#include "geometry.h"
#include "shape.h"
#include "event.h"

class ILastError {
public:
    virtual uint64_t code() = 0;
    virtual std::string message() = 0;
};

class IWindow {
public:
    virtual bool init(const Rect&) = 0;
    virtual void done() = 0;

    virtual bool render(const Shapes&) = 0;
    virtual bool update() = 0;
};

class IEventLoop {
public:
    virtual bool wait_event() = 0;
    virtual IEventHandler* process_events(IEventHandler*) = 0;
};

class IApp {
public:

    virtual bool init() = 0;
    virtual void done() = 0;

    virtual Rect max_window_rect() = 0;

    virtual ILastError* last_error() = 0;
    virtual IWindow* window() = 0;
    virtual IEventLoop* event_loop() = 0;
};
