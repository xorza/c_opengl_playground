#include "main.hpp"

#include "window.hpp"
#include "renderer.hpp"


#include <cstdio>
#include <cstdint>
#include <ctime>


int run() {
    Window window{};
    create_window(&window);

    SDL_GL_SetSwapInterval(0);

    struct timespec start_time{};
    clock_gettime(CLOCK_REALTIME, &start_time);

    struct FrameInfo frame_info = {
            .frame_number = 0,
            .ts_time = {0, 0},
            .ts_delta_time = {0, 0},
            .time = 0.0f,
            .delta = 0.0f,
    };

    Renderer renderer = Renderer{};
    create_renderer(&renderer);


    while (process_events(&window, false)) {
        if (!window.redraw) {
//            continue;
        }

        window.redraw = false;
        update_frame_info(&frame_info, start_time);

        render(&renderer, &window, &frame_info);

        SDL_GL_SwapWindow(window.sdl_window);
    }


    return 0;
}