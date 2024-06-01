#pragma once

#include "utils/gl_includes.hpp"
#include "utils/sdl_includes.hpp"
#include "utils/nocopy.hpp"

#include <cstdio>
#include <cstdint>
#include <ctime>


struct FrameInfo {
    uint32_t frame_number;

    timespec ts_time;
    timespec ts_delta_time;

    float time;
    float delta;
};

struct Window {
    NOCOPY(Window)

    SDL_Window *sdl_window;
    SDL_GLContext gl_context;
    int width;
    int height;
    bool redraw;

    Window() = default;

    ~Window();
};


int create_window(Window *window);

bool process_events(Window *window, bool wait);

void update_frame_info(
        struct FrameInfo *frame_info,
        struct timespec start_time
);