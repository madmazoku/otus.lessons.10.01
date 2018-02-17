// otus.lessons.10.01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <spdlog/spdlog.h>

#include <chrono>
#include <ctime>
#include <boost/lexical_cast.hpp>

#include "sdl2_app.h"
#include "vektor.h"

#include <memory>

/*! \file
    \brief vektor application entry function

    Shortcuts:
    -    Add PolyLine: 'P'
        -    Add and move new point: Left Mouse Button down not in existent point
        -    Select and move existent point: Left Mouse Button down in existent point
        -    Finish and discard PolyLine: Escape
        -     Finish and store PolyLine: Return
    -    Store scene to stdout: 'W'
    -    Replace scene from stdin: 'R'

*/

int main(int argc, char** argv)
{
    auto console = spdlog::stdout_logger_mt("console");
    console->info("Wellcome!");

    console->info("Shortcuts:");
    console->info("\tAdd PolyLine: 'P'");
    console->info("\t\tAdd and move new point: Left Mouse Button down not in existent point");
    console->info("\t\tSelect and move existent point: Left Mouse Button down in existent point");
    console->info("\t\tFinish and discard PolyLine: Escape");
    console->info("\t\tFinish and store PolyLine: Return");
    console->info("\tStore scene to stdout: 'W'");
    console->info("\tReplace scene from stdin: 'R'");

    SDL2App::App app;
    if(!app.init()) {
        console->error("App init Error: {0} {1}", app.last_error()->code(), app.last_error()->message());
        app.done();
        return 1;
    }

    Rect rect = app.max_window_rect();
    Point size = rect.size();

    if(size._x < 512)
        size._x = 512;
    if(size._y < 256)
        size._y = 256;

    size._x >>= 2;
    size._y >>= 2;
    rect._lt._x += size._x;
    rect._lt._y += size._y;
    rect._rb._x -= size._x;
    rect._rb._y -= size._y;

    IWindow* window = app.window();
    if(!window->init(rect)) {
        console->error("Window init Error: {0} {1}", app.last_error()->code(), app.last_error()->message());
        window->done();
        delete window;
        app.done();
        return 1;
    }

    VektorState vektor_state(rect.size());
    vektor_state._event_handlers[EventHandlerType::main] = new MainEventHandler(vektor_state);
    vektor_state._event_handlers[EventHandlerType::poly_line] = new PolyLineEventHandler(vektor_state);
    vektor_state._buttons.push_back(new ButtonLine(Point(30, 30), 25));
    vektor_state._buttons.push_back(new ButtonPolyLine(Point(90, 30), 25));
    vektor_state._buttons.push_back(new ButtonRect(Point(150, 30), 25));
    vektor_state._buttons.push_back(new ButtonCircle(Point(210, 30), 25));
    vektor_state._buttons.push_back(new ButtonWrite(Point(270, 30), 25));
    vektor_state._buttons.push_back(new ButtonRead(Point(330, 30), 25));

    IEventHandler* evh = vektor_state._event_handlers[EventHandlerType::main];

    auto start = std::chrono::system_clock::now();
    size_t count = 0;
    auto last = start;
    size_t last_count = count;
    bool run = true;

    while(true) {
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> full_elapsed = end - start;
        std::chrono::duration<double> last_elapsed = end - last;

        if (!run || last_elapsed.count() >= 1) {
            int frames = count - last_count;
            double fps = ((double)frames) / last_elapsed.count();

            console->info("[{0} / {1}] fps: {2}", full_elapsed.count(), count, fps);

            last = end;
            last_count = count;
        }
        ++count;

        evh = app.event_loop()->process_events(evh);
        if(evh == nullptr)
            break;
        if(vektor_state.render(window)) {
            console->error("Window render Error: {0} {1}", app.last_error()->code(), app.last_error()->message());
            window->done();
            delete window;
            app.done();
            return 1;
        }
        if(!window->update()) {
            console->error("Window update Error: {0} {1}", app.last_error()->code(), app.last_error()->message());
            window->done();
            delete window;
            app.done();
            return 1;
        }
        if(!app.event_loop()->wait_event()) {
            console->error("Event loop wait Error: {0} {1}", app.last_error()->code(), app.last_error()->message());
            window->done();
            delete window;
            app.done();
            return 1;
        }
    }

    window->done();
    delete window;

    app.done();

    console->info("Goodby!");

    return 0;
}