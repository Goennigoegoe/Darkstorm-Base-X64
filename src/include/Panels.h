#pragma once

typedef struct CScreenSize_t
{
	int iScreenHeight;
	int iScreenWidth;

} CScreenSize;

typedef void(__fastcall* PaintTraverse_t)(void*, unsigned int, bool, bool);
PaintTraverse_t oPaintTraverse = nullptr;
void __fastcall Hooked_PaintTraverse(void* pPanels, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
void Intro();

extern CScreenSize gScreenSize;
