#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "game.h"
#include "main.h"
#include "mydirect3d.h"
#include "system_timer.h"
#include "explosion.h"
#include "input.h"
#include "sprite.h"
#include "texture.h"
#include "player.h"
#include "enemy.h"
#include "item.h"
#include "boss.h"
#include "collision.h"
#include "scoreboard.h"
#include "effectpdead.h"
#include "sound.h"
#include "debug_font.h"

int g_FrameCount;				//フレームカウンター
int g_FPSBaseFrameCount;		//FPS計測用フレームカウンター
double g_FPSBaseTime;			//FPS計測用時間
float g_FPS;					//FPS

static int g_BG_textureID;
static int g_gameBG_textureID;
static GAME_DIFFICULTY g_game_difficulty;
float gameBG1Y, gameBG2Y;

void gameInit() {
	//==============================================System Initialization===============================================//
	DebugFont_Initialize();

	g_FrameCount = g_FPSBaseFrameCount = 0;
	g_FPSBaseTime = SystemTimer_GetTime();
	g_FPS = 0.0;

	Texture_SetLoadFile("Assets/Textures/background1.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	Texture_SetLoadFile("Assets/Textures/gameBG.png", GAME_WIDTH, GAME_HEIGHT);
	Texture_Load();

	g_BG_textureID = Texture_GetID("Assets/Textures/background1.png");
	g_gameBG_textureID = Texture_GetID("Assets/Textures/gameBG.png");

	gameBG1Y = SCREEN_HEIGHT / 2.f;
	gameBG2Y = (SCREEN_HEIGHT / 2.f) - Texture_GetWidth(g_gameBG_textureID);

	LPDIRECT3DDEVICE9 myDevice = MyDirect3D_GetDevice();
	//==================================================================================================================//

	playerInit();
	enemyInit();
	bossInit();
	itemInit();
	scoreBoardInit();
	pDeadEffectInit();
	explosionInit();

	PlaySound(SOUND_LABEL_BOSSBGM);
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

	gameBG1Y += 5.f;
	gameBG2Y += 5.f;
	if (gameBG1Y >= (SCREEN_HEIGHT / 2.f) + Texture_GetWidth(g_gameBG_textureID))
		gameBG1Y = (SCREEN_HEIGHT / 2.f) - Texture_GetWidth(g_gameBG_textureID);
	if (gameBG2Y >= (SCREEN_HEIGHT / 2.f) + Texture_GetWidth(g_gameBG_textureID))
		gameBG2Y = (SCREEN_HEIGHT / 2.f) - Texture_GetWidth(g_gameBG_textureID);

	playerUpdate();
	enemyUpdate();
	bossMove();
	itemUpdate();
	checkCollisionAll();
	checkBossCollision();
	if (pDeadEffectGetFlag())
		pDeadEffectUpdate();
	explosionUpdate();

}

void gameDraw() {

	Sprite_Draw(g_gameBG_textureID, 50.f + GAME_WIDTH / 2.f, gameBG1Y);
	Sprite_Draw(g_gameBG_textureID, 50.f + GAME_WIDTH / 2.f, gameBG2Y);

	itemDraw();
	playerDraw();
	enemyDraw();
	bossDraw();
	if (pDeadEffectGetFlag())
		pDeadEffectDraw();
	explosionDraw();

	Sprite_Draw(g_BG_textureID, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
	scoreBoardDraw();
}

void setGameDifficulty(GAME_DIFFICULTY difficulty) {
	g_game_difficulty = difficulty;
}

GAME_DIFFICULTY getGameDifficulty() {
	return g_game_difficulty;
}