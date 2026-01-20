bool dc_init_opengl(app_state_t* app_state) {
    SDL_GLContext gl_context = SDL_GL_CreateContext(app_state->sdl.window);

    char* version_string = (char*)glGetString(GL_VERSION);
    dc_log_printf("OpenGL supported version: %s\n", version_string);

    SDL_GL_MakeCurrent(app_state->sdl.window, gl_context);

    SDL_GL_SetSwapInterval(1); // Enable vsync
    app_state->vsync_enabled = true;

    return true;
}