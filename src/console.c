#include "console.h"
#include "mmio.h"
#include "tbfont.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static uint32_t FONT_SIZE = 8;
static unsigned char *pFont = (unsigned char*) tb_font;

#define CONSOLE_X 80
#define CONSOLE_Y 35

static uint8_t text_console[CONSOLE_X][CONSOLE_Y];
static color_t text_color[CONSOLE_X][CONSOLE_Y];

static int xConsolePos = 0;
static int yConsolePos = 0;

static color_t FOREGROUND_COLOR = {0, 0xff, 0},
               BACKGROUND_COLOR = {0, 0, 0};


static color_t ansi_colors[16] = {
    /* black */
    {
        0x00,
        0x00,
        0x00,
    },
    /* red */
    {
        0xaa,
        0x00,
        0x00,
    },
    /* green */
    {
        0x00,
        0xaa,
        0x00,
    },
    /* brown */
    {
        0xaa,
        0x55,
        0x22,
    },
    /* blue */
    {
        0x00,
        0x00,
        0xaa,
    },
    /* purple */
    {
        0xaa,
        0x00,
        0xaa,
    },
    /* cyan */
    {
        0x00,
        0xaa,
        0xaa,
    },
    /* grey */
    {
        0xaa,
        0xaa,
        0xaa,
    },
    /* dark grey */
    {
        0x7d,
        0x7d,
        0x7d,
    },
    /* bright red */
    {
        0xff,
        0x7d,
        0x7d,
    },
    /* bright green */
    {
        0x00,
        0xff,
        0x00,
    },
    /* yellow */
    {
        0xff,
        0xff,
        0x00,
    },
    /* bright blue */
    {
        0x00,
        0x00,
        0xff,
    },
    /* pink */
    {
        0xff,
        0x00,
        0xff,
    },
    /* bright cyan */
    {
        0x00,
        0xff,
        0xff,
    },
    /* white */
    {
        0xff,
        0xff,
        0xff,
    },
};


int console_putchar (color_t foreColor, color_t backColor,
                     int ch, int xPos, int yPos) {
    int x, y;
    for (y = 0; y < FONT_SIZE; y++) {
        for (x = 0; x < FONT_SIZE; x++) {
            if (pFont[ch * FONT_SIZE + y] & 1 << (FONT_SIZE-1 - x)) {
                SetPixel (x + xPos, y + yPos, FOREGROUND_COLOR);
            } else {
                SetPixel (x + xPos, y + yPos, BACKGROUND_COLOR);
            }
        }
    }
    return 0;
}


int console_write (const char *buffer, int nLength) {
    
    char escape_code[20];
    
    int bufferIdx, escapeIdx;
    
    for (bufferIdx = 0; bufferIdx < nLength; bufferIdx++) {
        
        switch (buffer[bufferIdx]) {
            case '\r':
                yConsolePos++;
                break;
            
            case '\n':
                xConsolePos = 0;
                yConsolePos++;
                break;
                
            case '\b':
                xConsolePos--;
                if (xConsolePos < 0) {
                    xConsolePos = 0;
                }
                break;
                
            default:
                text_console [xConsolePos][yConsolePos] = buffer[bufferIdx];
                text_color [xConsolePos][yConsolePos] = FOREGROUND_COLOR;
                xConsolePos++;
                break;
        }
        
        // New Line
        if (xConsolePos > CONSOLE_X) {
            xConsolePos = 0;
            yConsolePos++;
        }
        
        // Scroll
        if (yConsolePos > CONSOLE_Y) {
            
        }
    }
    
    console_push ();
    
    return nLength;
}


int console_push () {
    int x, y;
    
    if (!FrameBufferIsInitialized ()) {
        return -1;
    }
    
    for (x = 0; x < CONSOLE_X; x++) {
        for (y = 0; y < CONSOLE_Y; y++) {
            if (text_console [x][y] != ' ') {
                console_putchar (FOREGROUND_COLOR, BACKGROUND_COLOR,
                                 text_console [x][y], x*FONT_SIZE, y*FONT_SIZE);
            }
        }
    }
    
    return framebuffer_push ();
}


void console_init () {
    console_clear ();
    console_home ();
    
    framebuffer_push ();
}


int console_clear () {
    int x, y;
    
    ClearFrameBuffer (MakeColor(0, 0, 0));
    
    for (x = 0; x < CONSOLE_X; x++) {
        for (y = 0; y < CONSOLE_Y; y++) {
            text_console [x][y] = ' ';
            text_color [x][y] = BACKGROUND_COLOR;
        }
    }
    
    return 0;
}


int console_home () {
    xConsolePos = 0;
    yConsolePos = 0;
    
    return 0;
}