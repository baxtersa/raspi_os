#include "framebuffer.h"
#include "drivers/vcio.h"
#include <stdio.h>

static frame_buffer_t* pFrameBuffer = (frame_buffer_t*)M_BOX_BASE;
static int g_initialized = 0;

color_t colorBackground, colorForeground;

frame_buffer_t* GetFrameBuffer(void) {
  return pFrameBuffer;
}

void FrameBufferInit(void) {
  int rc;  

  // Initialize screen size members
  pFrameBuffer->m_uWidth = pFrameBuffer->m_uVirtualWidth = 800;
  pFrameBuffer->m_uHeight = pFrameBuffer->m_uVirtualHeight = 600;
  pFrameBuffer->m_uPitch = 0;
  pFrameBuffer->m_uDepth = 24;

  pFrameBuffer->m_uXOffset = pFrameBuffer->m_uYOffset = 0;

  rc = MailboxWrite (M_BOX_CHAN_FRAME_BUFFER, (uint32_t)pFrameBuffer + M_BOX_EMPTY);

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

color_t MakeColor(uint8_t red, uint8_t green, uint8_t blue) {
  return (color_t) {
    .red = red,
      .green = green,
      .blue = blue,
  };
}

int FrameBufferIsInitialized (void) {
  return g_initialized;
}

static int setPixel(frame_buffer_t* pFrameBuffer, uint16_t x_pixel, uint16_t y_pixel, color_t color);

void ClearFrameBuffer(color_t color) {
  uint16_t x, y;;

  frame_buffer_t* pFrameBuffer = GetFrameBuffer();
  if (!pFrameBuffer) {
    // Trap here if we don't have a valid frame buffer
    return;
  }

  for (y = 0; y != pFrameBuffer->m_uHeight; y++) {
    HorizontalLine (color, 0, pFrameBuffer->m_uWidth, y);
  }
}

static int setPixel(frame_buffer_t* pFrameBuffer, uint16_t x_pixel, uint16_t y_pixel, color_t color) {
  // Return an error if we try to set a pixel out of bounds
  if (x_pixel > pFrameBuffer->m_uVirtualWidth ||
      y_pixel > pFrameBuffer->m_uVirtualHeight) {
    return -1;
  }

  uint32_t offset = (x_pixel + (y_pixel + pFrameBuffer->m_uVirtualWidth)) * 
    (pFrameBuffer->m_uDepth / 8);

  *((uint8_t*) pFrameBuffer->m_pBuffer + offset) = color.red;
  *((uint8_t*) pFrameBuffer->m_pBuffer + offset + 1) = color.green;
  *((uint8_t*) pFrameBuffer->m_pBuffer + offset + 2) = color.blue;

  return 0;
}

void HorizontalLine (color_t color, int x0, int x1, int y) {
  int x;
  
  for (x = x0; x < x1; x++) {
    if (setPixel (pFrameBuffer, x, y, color) < 0) {
      // Break here if we fail to set a pixel
      continue;
    }
  }
}

void VerticalLine (color_t color, int y0, int y1, int x) {
  int y;

  for (y = y0; y < y1; y++) {
    setPixel (pFrameBuffer, x, y, color);
  }
}

void SetBackgroundColor(color_t color) {
  colorBackground = color;
}

void SetForegroundColor(color_t color) {
  colorForeground = color;
}
