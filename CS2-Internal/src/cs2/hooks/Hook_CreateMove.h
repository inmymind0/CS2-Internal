#pragma once
#include "cs2/classes/CCSGOInput.h"

typedef double(__fastcall* CreateMoveFn)(CCSGOInput* pThis, unsigned int slot, CUserCmd* pCmd);

extern CreateMoveFn oCreateMove;

double __fastcall hkCreateMove(CCSGOInput* pThis, unsigned int slot, CUserCmd* pCmd);
