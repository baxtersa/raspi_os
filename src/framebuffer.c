#include "framebuffer.h"
#include "drivers/vcio.h"
#include <stdio.h>

#define SCREEN_SIZE 800*600*3

static frame_buffer_t frameBuffer;
static unsigned char pScreenPixels [SCREEN_SIZE];

static int g_initialized = 0;

color_t colorBackground, colorForeground;

frame_buffer_t* GetFrameBuffer (void) {
    return &frameBuffer;
}

void FrameBufferInit (void) {
    int rc;  
    
    // Initialize screen size members
    frameBuffer.m_uWidth = frameBuffer.m_uVirtualWidth = 800;
    frameBuffer.m_uHeight = frameBuffer.m_uVirtualHeight = 600;
    frameBuffer.m_uPitch = 0;
    frameBuffer.m_uDepth = 24;
    
    frameBuffer.m_uXOffset = frameBuffer.m_uYOffset = 0;
    
    frameBuffer.m_pBuffer = 0;
    frameBuffer.m_uSize = 0;
    
    rc = MailboxWrite (M_BOX_CHAN_FRAME_BUFFER, (uint32_t) (&frameBuffer) + M_BOX_EMPTY);
    
    if (rc < 0) {
        while (1) {
            // TRAP!!! We failed to write to our mailbox
        }
    }

    rc = MailboxRead (M_BOX_CHAN_FRAME_BUFFER);
    
    if (rc < 0) {
        while (1) {
            // TRAP!!! We faile to read from our mailbox
        }
    }
    
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
    
    frame_buffer_t* pFrameBuffer = GetFrameBuffer ();
    if (!pFrameBuffer) {
	// Trap here if we don't have a valid frame buffer
	return;
    }
    
    for (y = 0; y != pFrameBuffer->m_uHeight; y++) {
	HorizontalLine (color, 0, pFrameBuffer->m_uWidth, y);
    }
}

static int setPixel (uint16_t x_pixel, uint16_t y_pixel, color_t color) {
    frame_buffer_t* pFrameBuffer = GetFrameBuffer ();
    // Return an error if we try to set a pixel out of bounds
    if (x_pixel > pFrameBuffer->m_uVirtualWidth ||
        y_pixel > pFrameBuffer->m_uVirtualHeight) {
        return -1;
    }
    
    uint8_t *fb = pScreenPixels;

    uint32_t uOffset = x_pixel * 3;

    fb [(y_pixel*pFrameBuffer->m_uPitch) + (uOffset) + 0] = color.red;
    fb [(y_pixel*pFrameBuffer->m_uPitch) + (uOffset) + 1] = color.green;
    fb [(y_pixel*pFrameBuffer->m_uPitch) + (uOffset) + 2] = color.blue;
    
    return 0;
}

void HorizontalLine (color_t color, int x0, int x1, int y) {
    int x;
    
    for (x = x0; x < x1; x++) {
        if (setPixel (x, y, color) < 0) {
            // Break here if we fail to set a pixel
            continue;
        }
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
    frame_buffer_t* pFrameBuffer = GetFrameBuffer ();
    int x;
    int r, g, b;

    for (x=0; x < pFrameBuffer->m_uWidth; x++) {
        r = x*255;
        g = x % 2 == 0 ?
            x*255 :
            255 * (pFrameBuffer->m_uWidth - x);
        b = 255 * (pFrameBuffer->m_uWidth - x);
        VerticalLine (MakeColor (r, g, b), 0,
                  pFrameBuffer->m_uHeight, x);
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
