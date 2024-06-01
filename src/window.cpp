#include "window.hpp"

#include <new>

int create_window(Window *const window) {
    new(window) Window{};

    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");


    window->width = 800;
    window->height = 600;

    const auto window_flags = static_cast< SDL_WindowFlags>(SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    window->sdl_window = SDL_CreateWindow(
            "Window",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            window->width, window->height,
            window_flags
    );
    window->gl_context = SDL_GL_CreateContext(window->sdl_window);

    SDL_GL_MakeCurrent(window->sdl_window, window->gl_context);
    glewInit();

    {   // Print OpenGL version and profile
        auto version = glGetString(GL_VERSION);
        fprintf(stdout, "OpenGL version: %s\n", version);

        int contextProfile = 0;
        glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &contextProfile);
        if ((contextProfile & GL_CONTEXT_CORE_PROFILE_BIT) != 0) {
            fprintf(stdout, "Core profile\n");
        } else {
            fprintf(stdout, "Not a core profile\n");
        }
    }

    window->redraw = true;

    return 0;
}

Window::~Window() {
    SDL_GL_DeleteContext(this->gl_context);
    SDL_DestroyWindow(this->sdl_window);
    SDL_Quit();
}

bool process_events(Window *const window, bool wait) {
    SDL_Event event;


    if (wait) {
        if (SDL_WaitEvent(&event) == 0) {
            return false;
        }
    } else {
        if (SDL_PollEvent(&event) == 0) {
            return true;
        }
    }

    int has_event = 1;
    while (has_event != 0) {
        switch (event.type) {
            case SDL_QUIT : {
                window->redraw = false;
                return false;
            }

            case SDL_WINDOWEVENT: {
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_MAXIMIZED:
                    case SDL_WINDOWEVENT_RESTORED:
                    case SDL_WINDOWEVENT_RESIZED : {
                        SDL_GetWindowSize(window->sdl_window, &window->width, &window->height);

                        window->redraw = true;
                    }

                }
            }

            default: {
            }
        }

        has_event = SDL_PollEvent(&event);
    }

    return true;
}

void update_frame_info(
        struct FrameInfo *const frame_info,
        const struct timespec start_time
) {
    frame_info->frame_number++;

    struct timespec current_time{};
    clock_gettime(CLOCK_REALTIME, &current_time);

    struct timespec time_since_start = {
            .tv_sec = current_time.tv_sec - start_time.tv_sec,
            .tv_nsec = current_time.tv_nsec - start_time.tv_nsec
    };
    struct timespec delta_time = {
            .tv_sec = time_since_start.tv_sec - frame_info->ts_time.tv_sec,
            .tv_nsec = time_since_start.tv_nsec - frame_info->ts_time.tv_nsec
    };

    frame_info->ts_time = time_since_start;
    frame_info->ts_delta_time = delta_time;


    frame_info->time = static_cast<float>(
            static_cast<double>(time_since_start.tv_sec) + time_since_start.tv_nsec / 1.0e9
    );
    frame_info->delta = static_cast<float>(
            static_cast<double>(delta_time.tv_sec) + delta_time.tv_nsec / 1.0e9
    );

//    printf("Frame number: %u\n", frame_info->frame_number);
//    printf("Time since start: %llu s %lu ns\n", frame_info->time.tv_sec, frame_info->time.tv_nsec);
//    printf("Delta time: %llu s %lu ns\n", frame_info->delta_time.tv_sec, frame_info->delta_time.tv_nsec);
}
