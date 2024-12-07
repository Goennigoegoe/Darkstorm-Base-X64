#pragma once

#ifndef CLIENT_H
#define CLIENT_H

#include "SDK.h"

bool __fastcall Hooked_CreateMove(void* ClientMode, float input_sample_frametime, CUserCmd* pCommand);
void InitCreateMove();

#endif
