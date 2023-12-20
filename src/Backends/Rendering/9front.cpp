// Released under the MIT licence.
// See LICENCE.txt for details.

#include "../Rendering.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <draw.h>
#include <memdraw.h>

#include "../Misc.h"
#include "Window/Software.h"
#include "../../Attributes.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct RenderBackend_Surface
{
	Image *i;
	Image *mask;
	int dirty;
} RenderBackend_Surface;

typedef struct RenderBackend_GlyphAtlas
{
	Memimage *m;
	Image *color;
	int dirty;
	Image *cache;
} RenderBackend_GlyphAtlas;

static RenderBackend_Surface framebuffer;

static RenderBackend_GlyphAtlas *glyph_atlas;
static RenderBackend_Surface *glyph_destination_surface;

enum {
	scale = 2,
};

RenderBackend_Surface* RenderBackend_Init(const char *window_title, size_t width, size_t height, int fullscreen)
{
	framebuffer.i = allocimage(display, Rect(0, 0, width*scale, height*scale), screen->chan, 0, DBlue);
	if(framebuffer.i == nil)
		sysfatal("could not alloc screen");

	return &framebuffer;
}

void RenderBackend_Deinit(void)
{
	freeimage(framebuffer.i);
}

void RenderBackend_DrawScreen(void)
{
	draw(screen, screen->r, framebuffer.i, nil, ZP);
	flushimage(display, 1);
}

RenderBackend_Surface* RenderBackend_CreateSurface(size_t width, size_t height, int render_target)
{
	(void)render_target;

	RenderBackend_Surface *surface = mallocz(sizeof(RenderBackend_Surface), 1);

	if (surface == NULL)
		return NULL;

	surface->i = allocimage(display, Rect(0, 0, width*scale, height*scale), screen->chan, 0, DGreen);

	return surface;
}

void RenderBackend_FreeSurface(RenderBackend_Surface *surface)
{
	freeimage(surface->i);
	freeimage(surface->mask);
	free(surface);
}

int RenderBackend_IsSurfaceLost(RenderBackend_Surface *surface)
{
	(void)surface;

	return 0;
}

void RenderBackend_RestoreSurface(RenderBackend_Surface *surface)
{
	(void)surface;
}

void RenderBackend_UploadSurfaceScale(RenderBackend_Surface *surface, const unsigned char *pixels, size_t width, size_t height)
{
	Image *row;
	Rectangle r, r2;
	int i, j, k, l;
	uchar *buf, *d;

	r = Rect(0, 0, width*scale, 1);
	r2 = Rect(0, 0, width*scale, scale);
	row = allocimage(display, r, BGR24, 1, DYellow);
	buf = malloc(width*scale*3);

	for(i = 0; i < height; i++){
		d = buf;
		for(j = 0; j < width; j++)
		for(k = 0; k < scale; k++){
			*d++ = pixels[(i*width*3) + j*3 + 0];
			*d++ = pixels[(i*width*3) + j*3 + 1];
			*d++ = pixels[(i*width*3) + j*3 + 2];
		}

		loadimage(row, r, buf, width*scale*3);
		draw(surface->i, r2, row, nil, ZP);
		r2.min.y += scale;
		r2.max.y += scale;
	}

	freeimage(row);
	free(buf);
	surface->dirty = 1;
}

void RenderBackend_UploadSurface(RenderBackend_Surface *surface, const unsigned char *pixels, size_t width, size_t height)
{
	Image *i;
	Rectangle r;

	if(scale != 1){
		RenderBackend_UploadSurfaceScale(surface, pixels, width, height);
		return;
	}

	r = Rect(0, 0, width, height);
	i = allocimage(display, r, BGR24, 0, DYellow);
	loadimage(i, r, pixels, width*height*3);
	draw(surface->i, r, i, nil, ZP);
	freeimage(i);
	surface->dirty = 1;
}

void RenderBackend_CalcMask(RenderBackend_Surface *s)
{
	Memimage *m;
	Rectangle r;
	int x, y, w, h;
	ulong *lp;
	ulong mask;

	switch(s->i->chan){
	case XRGB32:
	case ARGB32:
	case XBGR32:
	case ABGR32:
		mask = 0xFFFFFF;
		break;
	case RGBA32:
		mask = 0xFFFFFF00;
		break;
	default:
		/* TODO? */
		sysfatal("< 32 bit screen color channel");
		return;
	}
	r = s->i->r;
	w = Dx(r);
	h = Dy(r);
	m = allocmemimage(r, s->i->chan);
	unloadimage(s->i, r, m->data->bdata, h*w*(s->i->depth/8));
	for(y = 0; y < h; y++)
	for(x = 0; x < w; x++){
		lp = m->data->base + y*w + x;
		if((*lp & mask) == 0)
			*lp = 0x00;
		else
			*lp = 0xFFFFFFFF;
	}
	freeimage(s->mask);
	s->mask = allocimage(display, r, screen->chan, 0, DBlue);
	loadimage(s->mask, r, m->data->bdata, h*w*(s->i->depth/8));
	freememimage(m);
	s->dirty = 0;
}

void RenderBackend_Blit(RenderBackend_Surface *source_surface, const RenderBackend_Rect *rect, RenderBackend_Surface *destination_surface, long x, long y, int colour_key)
{
	Rectangle r, r2;

	r = Rect(rect->left*scale, rect->top*scale, rect->right*scale, rect->bottom*scale);
	r2 = Rect(x*scale, y*scale, (x*scale+Dx(r)), (y*scale+Dy(r)));

	if(colour_key && source_surface->dirty)
		RenderBackend_CalcMask(source_surface);

	draw(destination_surface->i, r2, source_surface->i, colour_key ? source_surface->mask : nil, r.min);
	destination_surface->dirty = 1;
}

void RenderBackend_ColourFill(RenderBackend_Surface *surface, const RenderBackend_Rect *rect, unsigned char red, unsigned char green, unsigned char blue)
{
	Rectangle r;
	Image *color;

	r = Rect(rect->left*scale, rect->top*scale, rect->right*scale, rect->bottom*scale);
	color = allocimage(display, Rect(0, 0, 1, 1), BGR24, 1, (red<<24)|(green<<16)|(blue<<8)|0xFF);
	draw(surface->i, r, color, nil, ZP);
	freeimage(color);
	surface->dirty = 1;
}

RenderBackend_GlyphAtlas* RenderBackend_CreateGlyphAtlas(size_t width, size_t height)
{
	RenderBackend_GlyphAtlas *atlas = mallocz(sizeof(RenderBackend_GlyphAtlas), 1);
	atlas->m = allocmemimage(Rect(0, 0, width*scale, height*scale), screen->chan);
	atlas->dirty = 1;
	return atlas;
}

void RenderBackend_DestroyGlyphAtlas(RenderBackend_GlyphAtlas *atlas)
{
	freememimage(atlas->m);
	freeimage(atlas->cache);
	free(atlas);
}

void RenderBackend_UploadGlyph(RenderBackend_GlyphAtlas *atlas, size_t x, size_t y, const unsigned char *pixels, size_t width, size_t height, size_t pitch)
{
	uchar *s, *d, *od;
	ulong dw;
	int ix, iy, k;

	dw = Dx(atlas->m->r);
	for(iy = 0; iy < height; iy++, y++){
		s = pixels + iy*pitch;
		od = d = byteaddr(atlas->m, Pt(x*scale, y*scale));

		for(ix = 0; ix < width; ix++, s++)
		for(k = 0; k < scale; k++){
			*d++ = *s;
			*d++ = *s;
			*d++ = *s;
			*d++ = 0xFF;
		}

		for(k = 1; k < scale; k++)
			memcpy(byteaddr(atlas->m, Pt(x*scale, y*scale+k)), od, d - od);
	}
	atlas->dirty = 1;
}

void RenderBackend_PrepareToDrawGlyphs(RenderBackend_GlyphAtlas *atlas, RenderBackend_Surface *destination_surface, unsigned char red, unsigned char green, unsigned char blue)
{
	int x, y, w;

	glyph_atlas = atlas;
	glyph_destination_surface = destination_surface;

	freeimage(atlas->color);
	atlas->color = allocimage(display, Rect(0, 0, 1, 1), screen->chan, 1, (red<<24)|(green<<16)|(blue<<8)|0xFF);
}

void RenderBackend_DrawGlyph(long x, long y, size_t glyph_x, size_t glyph_y, size_t glyph_width, size_t glyph_height)
{
	Rectangle r, cr;
	Point p;

	r = Rect(x*scale, y*scale, x*scale+glyph_width*scale, y*scale+glyph_height*scale);
	p = Pt(glyph_x*scale, glyph_y*scale);
	if(glyph_atlas->dirty){
		cr = glyph_atlas->m->r;
		freeimage(glyph_atlas->cache);
		glyph_atlas->cache = allocimage(display, cr, screen->chan, 0, DYellow);
		loadimage(glyph_atlas->cache, cr, glyph_atlas->m->data->bdata, Dx(cr)*Dy(cr)*(glyph_atlas->m->depth/8));
		glyph_atlas->dirty = 0;
	}
	draw(glyph_destination_surface->i, r, glyph_atlas->color, glyph_atlas->cache, p);
	glyph_destination_surface->dirty = 1;
}

void RenderBackend_HandleRenderTargetLoss(void)
{
}

void RenderBackend_HandleWindowResize(size_t width, size_t height)
{
}
