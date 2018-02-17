// otus.lessons.10.01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <spdlog/spdlog.h>

#include <SDL2/SDL.h>
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")

#include <chrono>
#include <ctime>
#include <boost/lexical_cast.hpp>

#include "sdl2_app.h"
#include "vektor.h"

#include <memory>

int main(int argc, char** argv)
{
    auto console = spdlog::stdout_logger_mt("console");
    console->info("Wellcome!");

    SDL2App::App app;
    if(!app.init()) {
        console->error("App init Error: {0} {1}", app.last_error()->code(), app.last_error()->message());
        app.done();
        return 1;
    }

    Rect rect = app.max_window_rect();
    Point size = rect.size();

    if(size._x == 0)
        size._x = 128;
    if(size._y == 0)
        size._y = 128;

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
/*    
    auto console = spdlog::stdout_logger_mt("console");
    console->info("Wellcome!");

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        console->error("SDL_Init Error: {1}", SDL_GetError());
        return 1;
    }

    SDL_DisplayMode display_mode;
    if (SDL_GetCurrentDisplayMode(0, &display_mode) != 0) {
        console->error("SDL_CreateWindow Error: {1}", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    long width = display_mode.w >> 1;
    long height = display_mode.h >> 1;

    SDL_Window *win = SDL_CreateWindow(
        "Vektor",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );
    if (win == nullptr) {
        console->error("SDL_CreateWindow Error: {1}", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (win == nullptr) {
        console->error("SDL_CreateRenderer Error: {1}", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    auto start = std::chrono::system_clock::now();
    size_t count = 0;
    auto last = start;
    size_t last_count = count;
    bool run = true;

    SDL_Event event;
    while (run) {
        auto loop_start = std::chrono::system_clock::now();
        ++count;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect rect;
        rect.x = 5;
        rect.y = 5;
        rect.w = width - 10;
        rect.h = height - 10;
        SDL_SetRenderDrawColor(renderer, 0x7f, 0x7f, 0x7f, 255);
        SDL_RenderDrawRect(renderer, &rect);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                run = false;
            }
        }

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> full_elapsed = end - start;
        std::chrono::duration<double> last_elapsed = end - last;
        std::chrono::duration<double> loop_elapsed = end - loop_start;

        if (!run || last_elapsed.count() >= 1) {
            int frames = count - last_count;
            double fps = ((double)frames) / last_elapsed.count();

            SDL_SetWindowTitle(win, ("Vektor! FPS: " + boost::lexical_cast<std::string>(fps)).c_str());

            console->info("[{0} / {1}] fps: {2}", full_elapsed.count(), count, fps);

            last = end;
            last_count = count;
        }

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    console->info("Goodby!");

    return 0;
*/
}