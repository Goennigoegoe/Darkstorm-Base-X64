#include "include/SDK.h"
#include "include/Panels.hpp"
#include "include/CDrawManager.h"

#include <iostream>
#include <string>

CScreenSize gScreenSize;

//===================================================================================
typedef void(__fastcall* PaintTraverse_t)(void*, int);
PaintTraverse_t oPaintTraverse = nullptr;
void __fastcall Hooked_PaintTraverse( void* rcx, int mode )
{
	/*try
	{
		//VMTManager& hook = VMTManager::GetHook(pPanels); //Get a pointer to the instance of your VMTManager with the function GetHook.
		//hook.GetMethod<void(__thiscall*)(PVOID, unsigned int, bool, bool)>(gOffsets.iPaintTraverseOffset)(pPanels, vguiPanel, forceRepaint, allowForce); //Call the original.

		static unsigned int vguiMatSystemTopPanel;

    std::cout << "Bruh\n";

		if (vguiMatSystemTopPanel == NULL)
		{
			const char* szName = gInts.Panels->GetName(vguiPanel);
			if( szName[0] == 'M' && szName[3] == 'S' ) //Look for MatSystemTopPanel without using slow operations like strcmp or strstr.
			{
				vguiMatSystemTopPanel = vguiPanel;
				Intro();
			}
		}
		
		if ( vguiMatSystemTopPanel == vguiPanel ) //If we're on MatSystemTopPanel, call our drawing code.
		{
			if( gInts.Engine->IsDrawingLoadingImage() || !gInts.Engine->IsInGame( ) || !gInts.Engine->IsConnected() || gInts.Engine->Con_IsVisible( ) || ( ( GetAsyncKeyState(VK_F12) || gInts.Engine->IsTakingScreenshot( ) ) ) )
			{
				return; //We don't want to draw at the menu.
			}

			//This section will be called when the player is not at the menu game and can see the screen or not taking a screenshot.
			gDrawManager.DrawString( (gScreenSize.iScreenWidth / 2) - 55, 200, gDrawManager.dwGetTeamColor(3), "Welcome to Darkstorm"); //Remove this if you want.

			//Test ESP code.

			CBaseEntity* pBaseLocalEnt = gInts.EntList->GetClientEntity(me);  //Grab the local player's entity.

			if (pBaseLocalEnt == NULL) //Always check for null pointers.
				return;

			Vector vecWorld, vecScreen; //Setup the Vectors.

			pBaseLocalEnt->GetWorldSpaceCenter(vecWorld); //Get the center of the player.

			if ( gDrawManager.WorldToScreen(vecWorld, vecScreen) ) //If the player is visble.
			{
				gDrawManager.DrawString( vecScreen.x, vecScreen.y, 0xFFFFFFFF, "You" ); //Draw on the player.
			}

      oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);
		}
	}
	catch(...)
	{
		Log::Fatal("Failed PaintTraverse");
	}*/

  // Replaced with engine vgui paint
  oPaintTraverse(rcx, mode);

  static bool FirstInitialize = true;
  if (FirstInitialize)
  {
    Intro();
    FirstInitialize = false;
  }

  if(mode & PAINT_UIPANELS)
  {
    gInts.Surface->StartDrawing();
    {
      gDrawManager.DrawString( (gScreenSize.iScreenWidth / 2) - 55, 200, gDrawManager.dwGetTeamColor(3), "Welcome to Darkstorm");
      
      if(gInts.Engine->IsInGame( ) || gInts.Engine->IsConnected())
      {
        CBaseEntity* pBaseLocalEnt = gInts.EntList->GetClientEntity(me)->As<CBaseEntity>();  //Grab the local player's entity.

        if (pBaseLocalEnt == NULL) //Always check for null pointers.
          return;

        Vector vecWorld, vecScreen; //Setup the Vectors.

        pBaseLocalEnt->GetWorldSpaceCenter(vecWorld); //Get the center of the player.

        if ( gDrawManager.WorldToScreen(vecWorld, vecScreen) ) //If the player is visble.
        {
          gDrawManager.DrawString( vecScreen.x, vecScreen.y, 0xFFFFFFFF, "You" ); //Draw on the player.
        }
      }
    }
    gInts.Surface->FinishDrawing();
  }
}

void __fastcall Hooked_Paint(void* rcx, int mode)
{
  oPaintTraverse(rcx, mode);

  static bool FirstInitialize = true;
  if (FirstInitialize)
  {
    Intro();
    FirstInitialize = false;
  }


  if(mode & PAINT_UIPANELS)
  {
    gInts.Surface->StartDrawing();
    {
      gDrawManager.DrawString( (gScreenSize.iScreenWidth / 2) - 55, 200, gDrawManager.dwGetTeamColor(3), "Welcome to Darkstorm");
      
      if(gInts.Engine->IsInGame( ) || gInts.Engine->IsConnected())
      {
        CBaseEntity* pBaseLocalEnt = gInts.EntList->GetClientEntity(me)->As<CBaseEntity>();  //Grab the local player's entity.

        if (pBaseLocalEnt == NULL) //Always check for null pointers.
          return;

        Vector vecWorld, vecScreen; //Setup the Vectors.

        pBaseLocalEnt->GetWorldSpaceCenter(vecWorld); //Get the center of the player.

        //vecWorld = pBaseLocalEnt->GetAbsOrigin();

        if ( gDrawManager.WorldToScreen(vecWorld, vecScreen) ) //If the player is visble.
        {
          gDrawManager.DrawString( vecScreen.x, vecScreen.y, 0xFFFFFFFF, "You" ); //Draw on the player.
        }

        //int LastY = 275;

        for(int i = 1; i < gInts.EntList->GetHighestEntityIndex(); i++)
        {
          CBaseEntity* pEnt = gInts.EntList->GetClientEntity(i)->As<CBaseEntity>();
          if(pEnt == NULL || pEnt->IsDormant())
            continue;

          if(pEnt->entindex() == pBaseLocalEnt->entindex())
            continue;

          if(!pEnt->IsPlayer())
          {
            //std::cout << "Not A Player\n";
            
            //Vector pEntWorld, pEntScreen;
            //pEnt->GetWorldSpaceCenter(pEntWorld);

            //if ( gDrawManager.WorldToScreen(pEntWorld, pEntScreen) )
              //gDrawManager.DrawString( pEntScreen.x, pEntScreen.y, 0xFFFFFFFF, "Not A Player" ); //Draw on the player.


            continue;
          }

          //char* name = pEnt->GetClientClass()->chName;
          //std::string EntText("Entity: ");
          //EntText += name;

          //gDrawManager.DrawString( (gScreenSize.iScreenWidth / 2) - 55, LastY, 0x0000FFFF, pEnt->GetClientClass()->chName );
          //LastY += 15;

          //gDrawManager.DrawString( 50, 50, 0xFFFFFFFF, "Player" ); //Draw on the player.
          
          Vector pEntWorld, pEntScreen;
          pEnt->GetWorldSpaceCenter(pEntWorld);

          if ( gDrawManager.WorldToScreen(pEntWorld, pEntScreen) )
          {
            //printf("Pos: %.2f %.2f", pEntScreen.x, pEntScreen.y);
            gDrawManager.DrawString( pEntScreen.x, pEntScreen.y, 0xFFFFFFFF, "Player" ); //Draw on the player.
          }
        }

        /*std::string Entities("Highest Entity Index: ");
        Entities += std::to_string(gInts.EntList->GetHighestEntityIndex());

        gDrawManager.DrawString( (gScreenSize.iScreenWidth / 2) - 55, 250, 0xFF0000FF, Entities.c_str() );*/
      }
    }
    gInts.Surface->FinishDrawing();
  }
}
//===================================================================================
void Intro( void )
{
	try
	{
		gDrawManager.Initialize(); //Initalize the drawing class.

		Log::Msg("Injection Successful"); //If the module got here without crashing, it is good day.
	}
	catch(...)
	{
		Log::Fatal("Failed Intro");
	}
}

void InitPaintTraverseHook()
{
  MH_CreateHook( find_vfunc<LPVOID>(gInts.EngineVGui, 14), &Hooked_Paint, (void**)(&oPaintTraverse) ); // getvfunc thing might be incorrect but should be correctly done
  //std::cout << "Function called but not doing shit\n";
}
