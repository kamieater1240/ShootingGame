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
#include "player.h"
#include "enemy.h"
#include "debug_font.h"

int g_FrameCount;				//フレームカウンター
int g_FPSBaseFrameCount;		//FPS計測用フレームカウンター
double g_FPSBaseTime;			//FPS計測用時間
float g_FPS;					//FPS

static int g_BG_textureID;
static int g_gameBG_textureID;
bool inTitlePhrase, inGamePhrase;

void gameInit() {
	//==============================================System Initialization===============================================//
	DebugFont_Initialize();
	SystemTimer_Initialize();
	SystemTimer_Start();

	g_FrameCount = g_FPSBaseFrameCount = 0;
	g_FPSBaseTime = SystemTimer_GetTime();
	g_FPS = 0.0;

	inTitlePhrase = true;
	inGamePhrase = false;

	Texture_SetLoadFile("Assets/Textures/background.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	Texture_SetLoadFile("Assets/Textures/gameBG.png", GAME_WIDTH, GAME_HEIGHT);
	Texture_Load();

	g_BG_textureID = Texture_GetID("Assets/Textures/background.png");
	g_gameBG_textureID = Texture_GetID("Assets/Textures/gameBG.png");

	LPDIRECT3DDEVICE9 myDevice = MyDirect3D_GetDevice();

	//Texture setting
	myDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	//可以控制polygon vertex的alpha值來讓texture變透明
	myDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	myDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	//透明を設定できるセッティング
	myDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	myDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	myDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//==================================================================================================================//

	playerInit();
	enemyInit();
}

void gameUninit() {

}

void gameUpdate() {

	g_FrameCount++;
	double time = SystemTimer_GetTime();
	float timeDelta = time - g_FPSBaseTime;

	if (time - g_FPSBaseTime >= FPS_MEASUREMENT_TIME) {
		g_FPS = (float)((g_FrameCount - g_FPSBaseFrameCount) / (time - g_FPSBaseTime));
		g_FPSBaseTime = time;
		g_FPSBaseFrameCount = g_FrameCount;
	}
	
	playerUpdate();
	enemyUpdate();

	/*if (inTitlePhrase) {

	}
	else if (!inTitlePhrase && inGamePhrase) {

	}*/

}

void gameDraw() {
	
	Sprite_Draw(g_BG_textureID, SCREEN_WIDTH/2.f, SCREEN_HEIGHT/2.f);
	Sprite_Draw(g_gameBG_textureID, 50.f + GAME_WIDTH/2.f, SCREEN_HEIGHT/2.f);

	playerDraw();
	enemyDraw();
}