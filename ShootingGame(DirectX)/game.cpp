#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "game.h"
#include "main.h"
#include "mydirect3d.h"
#include "system_timer.h"
#include "input.h"
#include "sprite.h"
#include "texture.h"

int g_FrameCount;				//フレームカウンター
int g_FPSBaseFrameCount;		//FPS計測用フレームカウンター
double g_FPSBaseTime;			//FPS計測用時間
float g_FPS;					//FPS

void gameInit() {
	
	SystemTimer_Initialize();
	SystemTimer_Start();

	g_FrameCount = g_FPSBaseFrameCount = 0;
	g_FPSBaseTime = SystemTimer_GetTime();
	g_FPS = 0.0;

	inTitlePhrase = true;
	inGamePhrace = false;

	LPDIRECT3DDEVICE9 myDevice = MyDirect3D_GetDevice();

	//Texture setting
	myDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	//可以控制polygon vertex的alpha值來讓texture變透明
	myDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	myDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	//g_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);	//可以讓texture只顯示polygon color

	//透明を設定できるセッティング
	myDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	myDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	myDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Texture_SetLoadFile("Asset/Texture/endSign.png", 720, 720);		//ID 0
	//Texture_Load();
}

void gameUninit() {

}

void gameUpdate() {

	g_FrameCount++;
	double time = SystemTimer_GetTime();

	if (time - g_FPSBaseTime >= FPS_MEASUREMENT_TIME) {
		g_FPS = (float)((g_FrameCount - g_FPSBaseFrameCount) / (time - g_FPSBaseTime));
		g_FPSBaseTime = time;
		g_FPSBaseFrameCount = g_FrameCount;
	}
	
	if (inTitlePhrase) {

	}
	else if (!inTitlePhrase && inGamePhrace) {

	}

}

void gameDraw() {
	LPDIRECT3DDEVICE9 myDevice = MyDirect3D_GetDevice();
	
	/*if (g_FrameCount < 1750) {
		//Draw BackGround
		myDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, BackGround, sizeof(Vertex2d));

		//Draw field
		for (int i = 0; i < 2; i++) {
			myDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, sportsField[i], sizeof(Vertex2d));
		}
		myDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, grassField, sizeof(Vertex2d));

		//Draw track lines
		for (int i = 0; i < 8; i++) {
			myDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, trackLines[i], sizeof(Vertex2d));
		}
		for (int i = 0; i < 4; i++) {
			myDevice->DrawPrimitiveUP(D3DPT_LINELIST, 2, farTrackLines[i], sizeof(Vertex2d));
		}

		animationDraw();
	}

	if (g_FrameCount >= 1750) {
		myDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, blackCover, sizeof(Vertex2d));

		if (g_FrameCount > 1800)
			Sprite_Draw(6, 640, 360);
	}*/



}