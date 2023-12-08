// Released under the MIT licence.
// See LICENCE.txt for details.

#include "../Controller.h"

int ControllerBackend_Init(void)
{
	return 0;
}

void ControllerBackend_Deinit(void)
{
	
}

int ControllerBackend_GetJoystickStatus(int **buttons, unsigned int *button_count, short **axes, unsigned int *axis_count)
{
	(void)buttons;
	(void)button_count;
	(void)axes;
	(void)axis_count;

	return 0;
}
