#pragma once

#include <SDL2/SDL.h>
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")

#include <SDL2/SDL2_gfxPrimitives.h>

#include "app.h"

/*! \file
    \brief Definitions for \ref app.h for the case of SDL2 library
*/

namespace SDL2App
{

class LastError : public ILastError
{
public:
    virtual uint64_t code() override
    {
        return -1;
    }
    virtual std::string message() override
    {
        return std::string{SDL_GetError()};
    };
};

class Window : public IWindow
{
public:
    SDL_Window* _window;
    SDL_Renderer *_renderer;

    virtual bool init(const Rect& rect) override
    {
        _window = SDL_CreateWindow(
                      "Vektor",
                      rect._lt._x,
                      rect._lt._y,
                      rect._rb._x - rect._lt._x,
                      rect._rb._y - rect._lt._y,
                      SDL_WINDOW_SHOWN
                  );
        if (_window == nullptr)
            return false;

        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
        if (_renderer == nullptr)
            return false;

        return true;
    }
    virtual void done() override
    {
        if(_renderer)
            SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
        if(_window)
            SDL_DestroyWindow(_window);
        _window = nullptr;
    }

    virtual bool render(const Shapes& shapes) override
    {
        for(auto s : shapes) {
            bool success;
            switch(s->type()) {
            case ShapeType::line:
                success = render(*(dynamic_cast<ShapeLine*>(s)));
                break;
            case ShapeType::poly_line:
                success = render(*(dynamic_cast<ShapePolyLine*>(s)));
                break;
            case ShapeType::rect:
                success = render(*(dynamic_cast<ShapeRect*>(s)));
                break;
            case ShapeType::circle:
                success = render(*(dynamic_cast<ShapeCircle*>(s)));
                break;
            default:
                success = true;
                break;
            }
            if(!success)
                return false;
        }
        return true;
    }
    virtual bool update() override
    {
        SDL_RenderPresent(_renderer);
        return true;
    }

    Window() : _window(nullptr), _renderer(nullptr) {}

    bool render(const ShapeLine& shape)
    {
        bool success = true;
        if(shape._state._fg._a != 0x00) {
            success = success && lineRGBA(
                          _renderer,
                          shape._p1._x, shape._p1._y, shape._p2._x, shape._p2._y,
                          shape._state._fg._r, shape._state._fg._g, shape._state._fg._b, shape._state._fg._a
                      ) == 0;
        }
        return success;
    }
    bool render(const ShapePolyLine& shape)
    {
        bool success = true;
        if(shape._pts.size() < 2)
            return success;

        Sint16* vx = new Sint16[shape._pts.size()];
        Sint16* vy = new Sint16[shape._pts.size()];
        {
            size_t i = 0;
            for(auto p : shape._pts) {
                vx[i] = p._x;
                vy[i] = p._y;
                ++i;
            }
        }
        if(shape._state._bg._a != 0x00) {
            if(shape._pts.size() > 2 && shape._closed)
                success = success && filledPolygonRGBA(
                              _renderer,
                              vx, vy, shape._pts.size(),
                              shape._state._fg._r, shape._state._fg._g, shape._state._fg._b, shape._state._fg._a
                          ) == 0;
        }
        if(shape._state._fg._a != 0x00) {
            if(shape._pts.size() > 2 && shape._closed)
                success = success && polygonRGBA(
                              _renderer,
                              vx, vy, shape._pts.size(),
                              shape._state._fg._r, shape._state._fg._g, shape._state._fg._b, shape._state._fg._a
                          ) == 0;
            else
                for(size_t i = 1; success && i < shape._pts.size(); ++i)
                    success = success && lineRGBA(
                                  _renderer,
                                  vx[i-1], vy[i-1], vx[i], vy[i],
                                  shape._state._fg._r, shape._state._fg._g, shape._state._fg._b, shape._state._fg._a
                              ) == 0;
        }
        delete[] vy;
        delete[] vx;
        return success;
    }
    bool render(const ShapeRect& shape)
    {
        bool success = true;
        if(shape._state._bg._a != 0x00) {
            success = success && boxRGBA(
                          _renderer,
                          shape._r._lt._x, shape._r._lt._y, shape._r._rb._x, shape._r._rb._y,
                          shape._state._bg._r, shape._state._bg._g, shape._state._bg._b, shape._state._bg._a
                      ) == 0;
        }
        if(shape._state._fg._a != 0x00) {
            success = success && rectangleRGBA(
                          _renderer,
                          shape._r._lt._x, shape._r._lt._y, shape._r._rb._x, shape._r._rb._y,
                          shape._state._fg._r, shape._state._fg._g, shape._state._fg._b, shape._state._fg._a
                      ) == 0;
        }
        return success;
    }
    bool render(const ShapeCircle& shape)
    {
        bool success = true;
        if(shape._state._bg._a != 0x00) {
            success = success && filledCircleRGBA(
                          _renderer,
                          shape._o._x, shape._o._y, shape._r,
                          shape._state._fg._r, shape._state._fg._g, shape._state._fg._b, shape._state._fg._a
                      ) == 0;
        }
        if(shape._state._fg._a != 0x00) {
            success = success && circleRGBA(
                          _renderer,
                          shape._o._x, shape._o._y, shape._r,
                          shape._state._fg._r, shape._state._fg._g, shape._state._fg._b, shape._state._fg._a
                      ) == 0;
        }
        return success;
    }
};

class EventLoop : public IEventLoop
{
public:
    virtual bool wait_event() override
    {
        return SDL_WaitEvent(NULL) == 1;
    }
    virtual IEventHandler* process_events(IEventHandler* evh) override
    {
        IEventHandler* evh_current = evh;
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            IEvent* ev;
            switch(event.type) {
            case SDL_KEYDOWN:
                ev = new EventKey(event.key.keysym.scancode, false);
                break;
            case SDL_KEYUP:
                ev = new EventKey(event.key.keysym.scancode, true);
                break;
            case SDL_MOUSEMOTION:
                ev = new EventMouseMove(Point(event.motion.x, event.motion.y));
                break;
            case SDL_MOUSEBUTTONDOWN:
                ev = new EventMouseButton(Point(event.button.x, event.button.y), get_mb(event), false);
                break;
            case SDL_MOUSEBUTTONUP:
                ev = new EventMouseButton(Point(event.button.x, event.button.y), get_mb(event), true);
                break;
            case SDL_QUIT:
                ev = new EventQuit;
                break;
            default:
                ev = nullptr;
                break;
            }
            if(ev != nullptr) {
                IEventHandler* evh_next = evh_current->process(ev);
                delete ev;
                if(evh_current != evh_next) {
                    evh_current->leave();
                    if(evh_next != nullptr)
                        evh_next->enter();
                }
                evh_current = evh_next;
            }
        }
        return evh_current;
    }

    EventMouseButton::MouseButton get_mb(const SDL_Event& event)
    {
        switch(event.button.button) {
        case SDL_BUTTON_LEFT:
            return EventMouseButton::MouseButton::Left;
        case SDL_BUTTON_RIGHT:
            return EventMouseButton::MouseButton::Right;
        default:
            return EventMouseButton::MouseButton::unknown;
        }
    }
};

class App : public IApp
{
public:
    LastError _last_error;
    EventLoop _event_loop;

    virtual bool init() override
    {
        return SDL_Init(SDL_INIT_VIDEO) == 0;
    }
    virtual void done() override
    {
        SDL_Quit();
    }

    virtual Rect max_window_rect() override
    {
        Rect rect(0, 0, 0, 0);

        SDL_DisplayMode display_mode;
        if (SDL_GetCurrentDisplayMode(0, &display_mode) != 0)
            return rect;

        rect._rb._x = display_mode.w;
        rect._rb._y = display_mode.h;

        return rect;
    }

    virtual ILastError* last_error() override
    {
        return &_last_error;
    }

    virtual IWindow* window() override
    {
        return new SDL2App::Window;
    }

    virtual IEventLoop* event_loop() override
    {
        return &_event_loop;
    }
};

}