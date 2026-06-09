#pragma once

#include "cs2/classes/CCSGOInput.h"

void __fastcall hkValidateInput(CCSGOInput* pInput, int unk);

using ValidateInputFn = void(__fastcall*)(CCSGOInput*, int);
extern ValidateInputFn oValidateInput;
