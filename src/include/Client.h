#pragma once

#include "SDK.h"

typedef bool(__fastcall* CreateMove_t)(void* ClientMode, float input_sample_frametime, CUserCmd* pCommand);
CreateMove_t oCreateMove = nullptr;
bool __fastcall Hooked_CreateMove(void* ClientMode, float input_sample_frametime, CUserCmd* pCommand);
