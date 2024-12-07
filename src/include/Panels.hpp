#pragma once

#ifndef PANELS_H
#define PANELS_H

typedef struct CScreenSize_t
{
	int iScreenHeight;
	int iScreenWidth;

} CScreenSize;

//typedef void(__fastcall* PaintTraverse_t)(void*, unsigned int, bool, bool);
//PaintTraverse_t oPaintTraverse = nullptr;

//void(__fastcall*)(void*, unsigned int, bool, bool) oPaintTraverse = nullptr;
void __fastcall Hooked_PaintTraverse(void* pPanels, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
void Intro();
void InitPaintTraverseHook();

extern CScreenSize gScreenSize;

#endif
