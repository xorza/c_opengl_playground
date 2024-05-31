#define GLEW_STATIC 1

#include <GL/glew.h>

#define SDL_MAIN_HANDLED 1

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>

#include <stdio.h>

struct Window {
    SDL_Window *sdl_window;
    SDL_GLContext gl_context;
    int width;
    int height;
    bool redraw;
};

struct Window create_window() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    int width = 800;
    int height = 600;

    const SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
    SDL_Window *const sdl_window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,
                                                    height,
                                                    window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(sdl_window);

    SDL_GL_MakeCurrent(sdl_window, gl_context);
    glewInit();

    { // Print OpenGL version and profile
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

    struct Window window = {sdl_window, gl_context, width, height, true};
    return window;
}

void destroy_window(struct Window *const window) {
    SDL_GL_DeleteContext(window->gl_context);
    SDL_DestroyWindow(window->sdl_window);
    SDL_Quit();
}

bool process_events(struct Window *const window) {
    SDL_Event event;

    if (SDL_WaitEvent(&event) == 0) {
        return false;
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
                    case SDL_WINDOWEVENT_RESIZED : {
                        window->width = event.window.data1;
                        window->height = event.window.data2;
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

int main(void) {
    struct Window window = create_window();

    while (process_events(&window)) {
        if (!window.redraw) {
            continue;
        }

        window.redraw = false;
        glViewport(0, 0, window.width, window.height);
        glClearColor(0.1f, 0.05f, 0.1f, 1.01f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window.sdl_window);
    }

    destroy_window(&window);

    return 0;
}
