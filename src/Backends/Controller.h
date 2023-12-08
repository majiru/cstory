// Released under the MIT licence.
// See LICENCE.txt for details.

#pragma once

int ControllerBackend_Init(void);
void ControllerBackend_Deinit(void);
int ControllerBackend_GetJoystickStatus(int **buttons, unsigned int *button_count, short **axes, unsigned int *axis_count);
