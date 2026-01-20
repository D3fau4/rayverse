int main(int argc, char** argv) {  
    app_state_t* app_state = &global_app_state;  
    /* Initialize the logger */
    dc_log_init();
    
    dc_log_printf("Welcome to Rayverse on Dreamcast!\n");
    dc_log_printf("Build date: %s %s\n\n", __DATE__, __TIME__);

    app_state->running = true;

    while(app_state->running) {
        usleep(50000);  /* 50ms */
    }

    return 0;
}