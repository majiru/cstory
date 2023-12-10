// Released under the MIT licence.
// See LICENCE.txt for details.

#include "../Software.h"

#include <stddef.h>
#include <stdlib.h>
#include <thread.h>
#include <draw.h>
#include <memdraw.h>

static Memimage *buffer[2];
static int curbuffer = 0;
extern Channel *drawreq;

int WindowBackend_Software_CreateWindow(const char *window_title, size_t screen_width, size_t screen_height, int fullscreen)
{
	(void)window_title;
	(void)fullscreen;

	buffer[0] = allocmemimage(Rect(0, 0, screen_width, screen_height), BGR24);
	buffer[1] = allocmemimage(Rect(0, 0, screen_width, screen_height), BGR24);
	if(buffer[0] == nil || buffer[1] == nil)
		sysfatal("allocmemimage: %r");
	return 1;
}

void WindowBackend_Software_DestroyWindow(void)
{
}

unsigned char* WindowBackend_Software_GetFramebuffer(size_t *pitch)
{
	Memimage *b;
	b = buffer[curbuffer];
	curbuffer = !curbuffer;
	*pitch = Dx(b->r)*(b->depth/8);

	return b->data->bdata;
}

void WindowBackend_Software_Display(void)
{
	/*
	 * If we're not ready for another frame
	 * drop it. Prevents performance problems
	 * resulting in slowdown of game logic.
	 */
	nbsend(drawreq, &buffer[curbuffer]);
}

void WindowBackend_Software_HandleWindowResize(size_t w, size_t h)
{
	freememimage(buffer[0]);
	freememimage(buffer[1]);
	buffer[0] = allocmemimage(Rect(0, 0, w, h), BGR24);
	buffer[1] = allocmemimage(Rect(0, 0, w, h), BGR24);
}
