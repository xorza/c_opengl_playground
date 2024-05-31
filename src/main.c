#include "window.h"
#include "renderer.h"

#include "gl_includes.h"
#include "sdl_includes.h"

#include <stdio.h>
#include <inttypes.h>
#include <time.h>


int main(void) {
    struct Window window;
    create_window(&window);

    struct timespec start_time;
    clock_gettime(CLOCK_REALTIME, &start_time);

    struct FrameInfo frame_info = {
            .frame_number = 0,
            .time = {0, 0},
            .delta_time = {0, 0}
    };

    struct Renderer renderer;
    create_renderer(&renderer);

    while (process_events(&window)) {
        if (!window.redraw) {
            continue;
        }

        window.redraw = false;
        update_frame_info(&frame_info, start_time);

        render(&renderer, &window, &frame_info);

        SDL_GL_SwapWindow(window.sdl_window);
    }

    destroy_renderer(&renderer);
    destroy_window(&window);

    return 0;
}