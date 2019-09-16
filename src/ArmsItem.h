#pragma once

#include "WindowsWrapper.h"

// "Arms" is a synonym of "weapon" here
// "Code" means "ID" here
// "Num" often means "ammo" here

/// Weapon struct
struct ARMS
{
	/// ID of the weapon
	int code;

	int level;
	int exp;

	/// Maximum ammunition
	int max_num;

	/// Current ammunition
	int num;
};

struct ITEM
{
	/// ID of the item
	int code;
};

// Limits for the amount of weapons and items
#define ARMS_MAX 8
#define ITEM_MAX 0x20

extern int gArmsEnergyX;

extern int gSelectedArms;
extern int gSelectedItem;

extern ARMS gArmsData[ARMS_MAX];
extern ITEM gItemData[ITEM_MAX];

/// Clear the gArmsData array, reverting it to the default state (no weapons)
void ClearArmsData();

/// Clear the gItemData array, reverting it to the default state (no items)
void ClearItemData();

/// Add code to the weapon array, setting max_num as the max ammo, or find code and add max_num to its ammo. Fails if no space is available and the weapon isn't already present
BOOL AddArmsData(long code, long max_num);

/// Remove code from the weapon array. Fails if code is not found.
BOOL SubArmsData(long code);

/// Replace code1 with code2, setting max_num as its max ammo. Fails if code1 is not found.
BOOL TradeArms(long code1, long code2, long max_num);

/// Add code to the item array. Fails if no space is left
BOOL AddItemData(long code);

/// Remove code from the item array. Fails if code was not found.
BOOL SubItemData(long code);

// Inventory loop
int CampLoop();
BOOL CheckItem(long a);
BOOL CheckArms(long a);
BOOL UseArmsEnergy(long num);
BOOL ChargeArmsEnergy(long num);
void FullArmsEnergy();
int RotationArms();
int RotationArmsRev();
void ChangeToFirstArms();
