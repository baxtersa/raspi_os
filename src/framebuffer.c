#include "framebuffer.h"
#include "drivers/vcio.h"
#include <stdio.h>

static frame_buffer_t* pFrameBuffer = (frame_buffer_t*)M_BOX_BASE;

color_t colorBackground, colorForeground;

frame_buffer_t* GetFrameBuffer(void) {
  return pFrameBuffer;
}

void FrameBufferInit(void) {
  // Initialize screen size members
  pFrameBuffer->m_uWidth = 800;
  pFrameBuffer->m_uHeight = 600;
  pFrameBuffer->m_uDepth = 24;

  pFrameBuffer->m_uXOffset = pFrameBuffer->m_uYOffset = 0;

  MailboxWrite(M_BOX_CHAN_FRAME_BUFFER, (uint32_t)pFrameBuffer + 0x40000000);

  MailboxRead(M_BOX_CHAN_FRAME_BUFFER, (uint32_t*)pFrameBuffer);
}

color_t MakeColor(uint8_t red, uint8_t green, uint8_t blue) {
  return (color_t) {
    .red = red,
      .green = green,
      .blue = blue,
  };
}

static int setPixel(frame_buffer_t* pFrameBuffer, uint16_t x_pixel, uint16_t y_pixel, color_t color);

void ClearFrameBuffer(color_t color) {
  uint16_t x, y;;

  frame_buffer_t* pFrameBuffer = GetFrameBuffer();
  if (!pFrameBuffer) {
    // Trap here if we don't have a valid frame buffer
    return;
  }

  for (y = 0; y != pFrameBuffer->m_uVirtualHeight; y++) {
    for (x = 0; x != pFrameBuffer->m_uVirtualWidth; x++) {
      if (setPixel(pFrameBuffer, x, y, color) < 0) {
	// Trap here if we fail to set a pixel color
	continue;
      }
    }
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

void SetBackgroundColor(color_t color) {
  colorBackground = color;
}

void SetForegroundColor(color_t color) {
  colorForeground = color;
}
