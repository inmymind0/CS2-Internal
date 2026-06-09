#pragma once
#include "cs2/classes/CCSGOInput.h"

typedef double(__fastcall* CreateMoveFn)(CCSGOInput* pThis, unsigned int slot, CUserCmd* pCmd);

extern CreateMoveFn oCreateMove;

double __fastcall hkCreateMove(CCSGOInput* pThis, unsigned int slot, CUserCmd* pCmd);

extern bool g_AntiAimEnabled;
extern int g_AntiAimPitchMode;
extern int g_AntiAimYawMode;
extern float g_AntiAimYawOffset;
extern float g_AntiAimRoll;

extern QAngle_t g_RealAngles;
extern QAngle_t g_FakeAngles;
extern bool g_IsUsing;