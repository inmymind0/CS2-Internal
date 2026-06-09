#pragma once

#include "cs2/classes/CCSGOInput.h"

using SetViewAnglesFn = void(__fastcall*)(CCSGOInput*, int32_t, QAngle_t&);
extern SetViewAnglesFn oSetViewAngles;
