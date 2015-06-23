#ifndef CONSOLE_H
#define CONSOLE_H

#include "framebuffer.h"

int console_putchar (color_t foreColor, color_t backColor,
                     int ch, int xPos, int yPos);
int console_write (const char *buffer, int nLength);

void console_init ();
int console_clear ();
int console_home ();

int console_push();

#endif