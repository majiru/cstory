// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#pragma once

#include "WindowsWrapper.h"

typedef struct DIRECTINPUTSTATUS DIRECTINPUTSTATUS;
struct DIRECTINPUTSTATUS
{
	BOOL bLeft;
	BOOL bRight;
	BOOL bUp;
	BOOL bDown;
	BOOL bButton[32]; // 32 is the number of buttons in DirectInput's `DIJOYSTATE` struct
};

void ReleaseDirectInput(void);
BOOL InitDirectInput(void);
BOOL GetJoystickStatus(DIRECTINPUTSTATUS *status);
BOOL ResetJoystickStatus(void);
