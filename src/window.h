#pragma once

#include "gl_includes.h"
#include "sdl_includes.h"

#include <stdio.h>
#include <inttypes.h>
#include <time.h>


struct Window {
    SDL_Window *sdl_window;
    SDL_GLContext gl_context;
    int width;
    int height;
    bool redraw;
};

struct FrameInfo {
    uint32_t frame_number;

    struct timespec time;
    struct timespec delta_time;
};

int create_window(struct Window *window);

void destroy_window(struct Window *window);

bool process_events(struct Window *window);

void update_frame_info(
        struct FrameInfo *frame_info,
        struct timespec start_time
);