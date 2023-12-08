// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#pragma once

#define RGB(r,g,b) ((r) | ((g) << 8) | ((b) << 16))

typedef int BOOL;

#define FALSE 0
#define TRUE 1

typedef struct RECT RECT;
struct RECT
{
	long left;
	long top;
	long right;
	long bottom;
};
