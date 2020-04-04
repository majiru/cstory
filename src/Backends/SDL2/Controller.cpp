#include "../Controller.h"
#include "Controller.h"

#include <stddef.h>
#include <stdio.h>

#include "SDL.h"

#include "../../WindowsWrapper.h"

#define DEADZONE 10000;

static SDL_Joystick *joystick;

BOOL ControllerBackend_Init(void)
{
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	return TRUE;
}

void ControllerBackend_Deinit(void)
{
	if (joystick != NULL)
	{
		SDL_JoystickClose(joystick);
		joystick = NULL;
	}

	SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
}

BOOL ControllerBackend_GetJoystickStatus(JOYSTICK_STATUS *status)
{
	if (joystick == NULL)
		return FALSE;

	const size_t button_limit = sizeof(status->bButton) / sizeof(status->bButton[0]);

	const Sint16 joystick_x = SDL_JoystickGetAxis(joystick, 0);
	const Sint16 joystick_y = SDL_JoystickGetAxis(joystick, 1);

	status->bLeft = joystick_x < -DEADZONE;
	status->bRight = joystick_x > DEADZONE;
	status->bUp = joystick_y < -DEADZONE;
	status->bDown = joystick_y > DEADZONE;

	int total_buttons = SDL_JoystickNumButtons(joystick);
	int total_axes = SDL_JoystickNumAxes(joystick);
	int total_hats = SDL_JoystickNumHats(joystick);

	unsigned int buttons_done = 0;

	for (int i = 0; i < total_buttons; ++i)
	{
		status->bButton[buttons_done] = SDL_JoystickGetButton(joystick, i);

		if (++buttons_done >= button_limit)
			break;
	}

	for (int i = 0; i < total_axes; ++i)
	{
		Sint16 axis = SDL_JoystickGetAxis(joystick, i);

		status->bButton[buttons_done] = axis < -DEADZONE;

		if (++buttons_done >= button_limit)
			break;

		status->bButton[buttons_done] = axis > DEADZONE;

		if (++buttons_done >= button_limit)
			break;
	}

	for (int i = 0; i < total_axes; ++i)
	{
		Uint8 hat = SDL_JoystickGetHat(joystick, i);

		status->bButton[buttons_done] = hat == SDL_HAT_UP || hat == SDL_HAT_LEFTUP || hat == SDL_HAT_RIGHTUP;

		if (++buttons_done >= button_limit)
			break;

		status->bButton[buttons_done] = hat == SDL_HAT_RIGHT || hat == SDL_HAT_RIGHTUP || hat == SDL_HAT_RIGHTDOWN;

		if (++buttons_done >= button_limit)
			break;

		status->bButton[buttons_done] = hat == SDL_HAT_DOWN || hat == SDL_HAT_LEFTDOWN || hat == SDL_HAT_RIGHTDOWN;

		if (++buttons_done >= button_limit)
			break;

		status->bButton[buttons_done] = hat == SDL_HAT_LEFT || hat == SDL_HAT_LEFTUP || hat == SDL_HAT_LEFTDOWN;

		if (++buttons_done >= button_limit)
			break;
	}

	// Blank the buttons that do not
	for (size_t i = buttons_done; i < button_limit; ++i)
		status->bButton[i] = FALSE;

	return TRUE;
}

BOOL ControllerBackend_ResetJoystickStatus(void)
{
	if (joystick == NULL)
		return FALSE;

	return TRUE;
}

void ControllerBackend_JoystickConnect(Sint32 joystick_id)
{
	printf("Joystick #%d connected - %s\n", joystick_id, SDL_JoystickNameForIndex(joystick_id));

	if (joystick == NULL)
	{
		joystick = SDL_JoystickOpen(joystick_id);

		if (joystick != NULL)
			printf("Joystick #%d selected\n", joystick_id);
	}
}

void ControllerBackend_JoystickDisconnect(Sint32 joystick_id)
{
	if (joystick_id == SDL_JoystickInstanceID(joystick))
	{
		printf("Joystick #%d disconnected\n", joystick_id);
		joystick = NULL;
	}
}