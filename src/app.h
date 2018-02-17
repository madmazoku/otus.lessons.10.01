#pragma once

#include "geometry.h"
#include "shape.h"
#include "event.h"

/*! \file
    \brief Interfaces to system libraries required for GUI application
*/

//! \brief interface allow obtain some last error info from library
class ILastError
{
public:
//! \brief last error code
    virtual uint64_t code() = 0;
//! \brief last error message
    virtual std::string message() = 0;
};

//! \brief interface to window which can render some \ref IShape sequence and display it
class IWindow
{
public:
//! \brief make window with needed size and place
    virtual bool init(const Rect&) = 0;
//! \brief free resources
    virtual void done() = 0;

//! \brief render shapes sequence
    virtual bool render(const Shapes&) = 0;
//! \brief display currently rendered image
    virtual bool update() = 0;
};

//! \brief interface to event processing
class IEventLoop
{
public:
//! \brief do nothing untill new event appears
    virtual bool wait_event() = 0;
//! \brief take events from queue and ask \ref IEventHandler to process it. Return \ref IEventHandler which must be used for next event
    virtual IEventHandler* process_events(IEventHandler*) = 0;
};

//! \brief interface to initialize system library and obtain general data and other interfaces
class IApp
{
public:
//! \brief initialize system libraries
    virtual bool init() = 0;
//! \brief free resources
    virtual void done() = 0;

//! \brief utility whicth return max rect window can have
    virtual Rect max_window_rect() = 0;

//! \brief return \ref ILastError interface
    virtual ILastError* last_error() = 0;
//! \brief return \ref IWindow interface
    virtual IWindow* window() = 0;
//! \brief return \ref IEventLoop interface
    virtual IEventLoop* event_loop() = 0;
};
