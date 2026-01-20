#define FPS 30
#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 240

int main(int argc, char** argv) {  
    app_state_t* app_state = &global_app_state;  
    /* Initialize the logger */
    dc_log_init();
    
    dc_log_printf("Welcome to Rayverse on Dreamcast!\n");
    dc_log_printf("Build date: %s %s\n\n", __DATE__, __TIME__);

    SDL_SetHint(SDL_HINT_VIDEO_DOUBLE_BUFFER, "1"); // SDL2 defaults to double buffering, this shuts it off
    SDL_SetHint(SDL_HINT_DC_VIDEO_MODE, "SDL_DC_TEXTURED_VIDEO");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        dc_log_printf("SDL could not initialize! SDL Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Rayverse", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        dc_log_printf("Window could not be created! SDL Error: %s", SDL_GetError());
        return 3;
    }

    {
        s32 gl_w, gl_h;
        SDL_GL_GetDrawableSize(window, &gl_w, &gl_h);
        s32 window_w, window_h;
        SDL_GetWindowSize(window, &window_w, &window_h);
        app_state->client_width = WINDOW_WIDTH;
        app_state->client_height = WINDOW_HEIGHT;
        app_state->display_scale_factor = (float) gl_w / (float) window_w;
        app_state->display_points_per_pixel = (float) window_w / (float) gl_w;
        dc_log_printf("Display scale factor: %f\n", app_state->display_scale_factor);
        dc_log_printf("Display points per pixel: %f\n", app_state->display_points_per_pixel);
        dc_log_printf("Window size: %dx%d\n", window_w, window_h);
        dc_log_printf("Drawable size: %dx%d\n", gl_w, gl_h);
    }

    dc_init_opengl(app_state);

    app_state->sdl.window = window;
    app_state->running = true;
    
    while(app_state->running) {
        usleep(50000);  /* 50ms */
    }

    return 0;
}