#pragma once

#include "gl_includes.hpp"
#include "sdl_includes.hpp"
#include "nocopy.hpp"

#include <cstdio>
#include <cstdint>
#include <ctime>


struct FrameInfo {
    uint32_t frame_number;

    timespec time;
    timespec delta_time;
};

struct Window {
    NO_COPY(Window)

    SDL_Window *sdl_window;
    SDL_GLContext gl_context;
    int width;
    int height;
    bool redraw;

    Window() = default;

    ~Window();
};


int create_window(Window *window);

bool process_events(Window *window);

void update_frame_info(
        struct FrameInfo *frame_info,
        struct timespec start_time
);