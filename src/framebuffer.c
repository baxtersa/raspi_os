#include "framebuffer.h"
#include "drivers/vcio.h"
#include <stdio.h>

#define SCREEN_SIZE 800*600*3

static frame_buffer_t frameBuffer;
static unsigned char pScreenPixels [(uint32_t) SCREEN_SIZE];

static int g_initialized = 0;

color_t colorBackground, colorForeground;

frame_buffer_t GetFrameBuffer (void) {
    return frameBuffer;
}

void FrameBufferInit (void) {
    frame_buffer_t fb_info __attribute__ ((aligned(16)));
    int rc;
    
    // Initialize screen size members
    fb_info.m_uWidth = 640;
    fb_info.m_uVirtualWidth = 480;
    fb_info.m_uHeight = 640;
    fb_info.m_uVirtualHeight = 480;
    fb_info.m_uPitch = 0;
    fb_info.m_uDepth = 24;
    
    fb_info.m_uXOffset = 0;
    fb_info.m_uYOffset = 0;
    
    fb_info.m_pBuffer = 0;
    fb_info.m_uSize = 0;
    
    rc = MailboxWrite (M_BOX_CHAN_FRAME_BUFFER, (uint32_t) (&fb_info) + M_BOX_EMPTY);
    
    if (rc < 0) {
        while (1) {
            // TRAP!!! We failed to write to our mailbox
        }
    }

    rc = MailboxRead (M_BOX_CHAN_FRAME_BUFFER);
    
    if (rc < 0) {
        while (1) {
            // TRAP!!! We failed to read from our mailbox
        }
    }
    
    frameBuffer.m_pBuffer = (uint32_t)fb_info.m_pBuffer;
    frameBuffer.m_uWidth = fb_info.m_uWidth;
    frameBuffer.m_uHeight = fb_info.m_uHeight;
    frameBuffer.m_uPitch = fb_info.m_uPitch;
    frameBuffer.m_uDepth = fb_info.m_uDepth;

    g_initialized = 1;
}

color_t MakeColor (uint8_t red, uint8_t green, uint8_t blue) {
    return (color_t) {
	.red = red,
	    .green = green,
	    .blue = blue,
	    };
}

int FrameBufferIsInitialized (void) {
    return g_initialized;
}

static int setPixel (uint16_t x_pixel, uint16_t y_pixel, color_t color);

void ClearFrameBuffer (color_t color) {
    uint32_t x, y;
    
    frame_buffer_t pFrameBuffer = GetFrameBuffer ();
    
    for (y = 0; y != pFrameBuffer.m_uHeight; y++) {
        HorizontalLine (color, 0, pFrameBuffer.m_uWidth, y);
    }
    
    framebuffer_push ();
}

static int setPixel (uint16_t x_pixel, uint16_t y_pixel, color_t color) {
    frame_buffer_t pFrameBuffer = GetFrameBuffer ();
    // Return an error if we try to set a pixel out of bounds
    if (x_pixel > pFrameBuffer.m_uWidth ||
        y_pixel > pFrameBuffer.m_uHeight) {
        return -1;
    }
    
    uint8_t *fb = pScreenPixels;

    uint32_t uOffset = x_pixel * 3;

    fb [(y_pixel*pFrameBuffer.m_uPitch) + (uOffset) + 0] = color.red;
    fb [(y_pixel*pFrameBuffer.m_uPitch) + (uOffset) + 1] = color.green;
    fb [(y_pixel*pFrameBuffer.m_uPitch) + (uOffset) + 2] = color.blue;
    
    return 0;
}

void HorizontalLine (color_t color, int x0, int x1, int y) {
    int x;
    
    for (x = x0; x < x1; x++) {
        setPixel (x, y, color);
    }
}

void VerticalLine (color_t color, int y0, int y1, int x) {
    int y;
    
    for (y = y0; y < y1; y++) {
        setPixel (x, y, color);
    }
}

void SetBackgroundColor(color_t color) {
    colorBackground = color;
}

void SetForegroundColor(color_t color) {
    colorForeground = color;
}

void GradientFB (void) {
    frame_buffer_t pFrameBuffer = GetFrameBuffer ();
    int x, y;
    int r, g, b;
    int height = pFrameBuffer.m_uHeight;
    int width = pFrameBuffer.m_uWidth;
    
    for (x=0; x < width; x++) {
        for (y=0; y < height; y++) {
            r = x*255 / width - y*255 / height;
            r *= r < 0 ? -1 : 1;
            g = 255 * (width - x) / width - y*255 / height;
            g *= g < 0 ? -1 : 1;
            b = 255 * (width - x) / width - (255 * (height - y)) / height;
            b *= b < 0 ? -1 : 1;
            setPixel (x, y, MakeColor (r, g, b));
        }
    }

    framebuffer_push();
}

void *int_memcpy (void *dest, const void *src, size_t n) {
    int i;

    int *d = dest;
    const int *s = src;

    for (i = 0; i < n / 4; i++) {
        *d = *s;
        d++;
        s++;
    }

    return dest;
}

int framebuffer_push (void) {
    int_memcpy ((uint8_t*) frameBuffer.m_pBuffer,
		pScreenPixels, SCREEN_SIZE);
}
