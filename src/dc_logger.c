#include <dc/video.h>
#include <dc/biosfont.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define CHAR_WIDTH BFONT_THIN_WIDTH
#define CHAR_HEIGHT BFONT_HEIGHT
#define MAX_COLS (SCREEN_WIDTH / CHAR_WIDTH)
#define MAX_ROWS (SCREEN_HEIGHT / CHAR_HEIGHT)
#define BUFFER_SIZE 4096

/* Logger state */
static struct {
    int cursor_x;
    int cursor_y;
    int color;
    int initialized;
} logger_state = { 0, 0, 1, 0 };

/* Initialize the logger */
void dc_log_init() {
    if (!logger_state.initialized) {
        vid_set_mode(DM_640x480, PM_RGB565);
        logger_state.cursor_x = 0;
        logger_state.cursor_y = 0;
        logger_state.color = 1;
        logger_state.initialized = 1;
        dc_log_clear();
    }
}

/* Clear the screen */
void dc_log_clear() {
    vid_clear(0, 0, 0);
    logger_state.cursor_x = 0;
    logger_state.cursor_y = 0;
}

/* Set text color */
void dc_log_set_color(int inverted) {
    logger_state.color = inverted ? 0 : 1;
}

/* Set cursor position */
void dc_log_set_cursor(int x, int y) {
    if (x >= 0 && x < MAX_COLS) {
        logger_state.cursor_x = x;
    }
    if (y >= 0 && y < MAX_ROWS) {
        logger_state.cursor_y = y;
    }
}

/* Scroll the screen up by one line */
static void scroll_up() {
    /* Simple approach: clear and reset to top
       For a real implementation, you'd want to copy the video memory */
    if (logger_state.cursor_y >= MAX_ROWS) {
        /* We've reached the bottom, wrap to top and clear */
        logger_state.cursor_y = 0;
        logger_state.cursor_x = 0;
        
        /* Optional: implement actual scrolling by copying vram_s buffer
           For now, we just wrap around */
    }
}

/* Print a single character */
static void print_char(char c) {
    if (!logger_state.initialized) {
        dc_log_init();
    }
    
    /* Handle special characters */
    if (c == '\n') {
        logger_state.cursor_x = 0;
        logger_state.cursor_y++;
        scroll_up();
        return;
    } else if (c == '\r') {
        logger_state.cursor_x = 0;
        return;
    } else if (c == '\t') {
        /* Tab = 4 spaces */
        int spaces = 4 - (logger_state.cursor_x % 4);
        for (int i = 0; i < spaces; i++) {
            print_char(' ');
        }
        return;
    }
    
    /* Check if we need to wrap to next line */
    if (logger_state.cursor_x >= MAX_COLS) {
        logger_state.cursor_x = 0;
        logger_state.cursor_y++;
        scroll_up();
    }
    
    /* Calculate screen offset */
    int offset = (logger_state.cursor_y * CHAR_HEIGHT * SCREEN_WIDTH) +
                 (logger_state.cursor_x * CHAR_WIDTH);
    
    /* Draw the character */
    if (offset >= 0 && offset < SCREEN_WIDTH * SCREEN_HEIGHT) {
        bfont_draw_thin(vram_s + offset, SCREEN_WIDTH, logger_state.color, c, 0);
    }
    
    /* Move cursor forward */
    logger_state.cursor_x++;
}

/* Print a string without formatting */
void dc_log_print(const char *str) {
    if (!str) return;
    
    if (!logger_state.initialized) {
        dc_log_init();
    }
    
    while (*str) {
        print_char(*str);
        str++;
    }
}

/* Print a formatted string (like printf) */
void dc_log_printf(const char *fmt, ...) {
    char buffer[BUFFER_SIZE];
    va_list args;
    
    if (!fmt) return;
    
    if (!logger_state.initialized) {
        dc_log_init();
    }
    
    va_start(args, fmt);
    vsnprintf(buffer, BUFFER_SIZE, fmt, args);
    va_end(args);
    
    dc_log_print(buffer);
}
