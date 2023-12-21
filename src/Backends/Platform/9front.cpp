// Released under the MIT licence.
// See LICENCE.txt for details.

#include "../Misc.h"

#include "../../Attributes.h"

#include <thread.h>
#include <draw.h>
#include <memdraw.h>
#include <keyboard.h>

static int keyboard_state[BACKEND_KEYBOARD_TOTAL];

int Mark_Rune(int *kbd, Rune r)
{
	if(r >= 'a' && r <= 'z'){
		r -= 'a';
		return kbd[BACKEND_KEYBOARD_A + r] = 1;
	}
	if(r >= '0' && r <= '9'){
		r -= '0';
		return kbd[BACKEND_KEYBOARD_0 + r] = 1;
	}

	switch(r){
	case KF|1:
	case KF|2:
	case KF|3:
	case KF|4:
	case KF|5:
	case KF|6:
	case KF|7:
	case KF|8:
	case KF|9:
	case KF|10:
	case KF|11:
	case KF|12:
		r -= (KF|1);
		return kbd[BACKEND_KEYBOARD_F1 + r] = 1;

	case Kleft:
		return kbd[BACKEND_KEYBOARD_LEFT] = 1;
	case Kright:
		return kbd[BACKEND_KEYBOARD_RIGHT] = 1;
	case Kup:
		return kbd[BACKEND_KEYBOARD_UP] = 1;
	case Kdown:
		return kbd[BACKEND_KEYBOARD_DOWN] = 1;
	case Kesc:
		return kbd[BACKEND_KEYBOARD_ESCAPE] = 1;
	case '`':
		return kbd[BACKEND_KEYBOARD_BACK_QUOTE] = 1;
	case '\t':
		return kbd[BACKEND_KEYBOARD_TAB] = 1;
	case Kshift:
		return kbd[BACKEND_KEYBOARD_LEFT_SHIFT] = 1;
	case Kctl:
		return kbd[BACKEND_KEYBOARD_LEFT_CTRL] = 1;
	case ' ':
		return kbd[BACKEND_KEYBOARD_SPACE] = 1;
	case '\n':
		return kbd[BACKEND_KEYBOARD_ENTER] = 1;
	case Kbs:
		return kbd[BACKEND_KEYBOARD_BACKSPACE] = 1;
	case '-':
		return kbd[BACKEND_KEYBOARD_MINUS] = 1;
	case '=':
		return kbd[BACKEND_KEYBOARD_EQUALS] = 1;
	case '[':
		return kbd[BACKEND_KEYBOARD_LEFT_BRACKET] = 1;
	case ']':
		return kbd[BACKEND_KEYBOARD_RIGHT_BRACKET] = 1;
	case '\\':
		return kbd[BACKEND_KEYBOARD_BACK_SLASH] = 1;
	case ';':
		return kbd[BACKEND_KEYBOARD_SEMICOLON] = 1;
	case '\'':
		return kbd[BACKEND_KEYBOARD_APOSTROPHE] = 1;
	case ',':
		return kbd[BACKEND_KEYBOARD_COMMA] = 1;
	case '.':
		return kbd[BACKEND_KEYBOARD_PERIOD] = 1;
	case '/':
		return kbd[BACKEND_KEYBOARD_FORWARD_SLASH] = 1;
	}
	return 0;

	/* Sorry!
		BACKEND_KEYBOARD_CAPS_LOCK,
		BACKEND_KEYBOARD_LEFT_ALT,
		BACKEND_KEYBOARD_RIGHT_ALT,
		BACKEND_KEYBOARD_RIGHT_CTRL,
		BACKEND_KEYBOARD_RIGHT_SHIFT,
	*/
}

void Key_Proc(void*)
{
	char buf[128], *s;
	int kbfd, n;
	Rune r;

	threadsetname("keyproc");
	kbfd = open("/dev/kbd", OREAD);
	if(kbfd < 0)
		sysfatal("open: %r");
	for(;;){
		n = read(kbfd, buf, sizeof buf-1);
		if(n <= 0)
			return;
		buf[n-1] = 0;
		for(s = buf; s < buf+n; s++){
			if(*s != 'k' && *s != 'K'){
				for(; *s != '\0'; s++)
					;
				continue;
			}

			memset(keyboard_state, 0, sizeof keyboard_state);
			for(s++; *s && s < buf+n;){
				s += chartorune(&r, s);
				Mark_Rune(keyboard_state, r);
			}
		}
	}
}

int Backend_Init(void (*drag_and_drop_callback)(const char *path), void (*window_focus_callback)(int focus))
{
	(void)drag_and_drop_callback;
	(void)window_focus_callback;

	proccreate(Key_Proc, nil, 8192);

	return 1;
}

void Backend_Deinit(void)
{
}

void Backend_PostWindowCreation(void)
{
	
}

int Backend_GetPaths(char **module_path, char **data_path)
{
	*module_path = "/sys/games/lib/cstory";
	*data_path = "/sys/games/lib/cstory/data";

	return 1;
}

void Backend_HideMouse(void)
{
	
}

void Backend_SetWindowIcon(const unsigned char *rgb_pixels, size_t width, size_t height)
{
	(void)rgb_pixels;
	(void)width;
	(void)height;
}

void Backend_SetCursor(const unsigned char *rgba_pixels, size_t width, size_t height)
{
	(void)rgba_pixels;
	(void)width;
	(void)height;
}

void Backend_EnableDragAndDrop(void)
{	
}


int Backend_SystemTask(int active)
{
	return 1;
}

void Backend_GetKeyboardState(int *out)
{
	memcpy(out, keyboard_state, sizeof keyboard_state);
}

void Backend_ShowMessageBox(const char *title, const char *message)
{
	Backend_PrintInfo("ShowMessageBox - '%s' - '%s'\n", title, message);
}

ATTRIBUTE_FORMAT_PRINTF(1, 2) void Backend_PrintError(const char *format, ...)
{
	va_list argumentList;
	va_start(argumentList, format);
	fputs("ERROR: ", stderr);
	vfprintf(stderr, format, argumentList);
	fputc('\n', stderr);
	va_end(argumentList);
}

ATTRIBUTE_FORMAT_PRINTF(1, 2) void Backend_PrintInfo(const char *format, ...)
{
	va_list argumentList;
	va_start(argumentList, format);
	fputs("INFO: ", stdout);
	vfprintf(stdout, format, argumentList);
	fputc('\n', stdout);
	va_end(argumentList);
}

#define Nmsec 1000000ULL

unsigned long Backend_GetTicks(void)
{
	return npe_nanosec() / Nmsec;
}

void Backend_Delay(unsigned int ticks)
{
	npe_nsleep((uvlong)ticks * Nmsec);
}
