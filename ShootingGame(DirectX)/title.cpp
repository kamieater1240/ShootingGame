#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "title.h"
#include "main.h"
#include "game.h"
#include "fade.h"
#include "sound.h"
#include "texture.h"
#include "sprite.h"
#include "input.h"
#include "mydirect3d.h"

static int titleTexID;
static int startTexID, startGlowTexID;
static int exitTexID, exitGlowTexID;
static int easyTexID, easyGlowTexID;
static int hardTexID, hardGlowTexID;

//Menu
bool inMenu, difficultyChoose;
int  selected;

void titleInit() {
	Texture_SetLoadFile("Assets/Textures/title.png", 1280, 900);
	Texture_SetLoadFile("Assets/Textures/start.png", 452, 142);
	Texture_SetLoadFile("Assets/Textures/start_glow.png", 470, 162);
	Texture_SetLoadFile("Assets/Textures/exit.png", 311, 142);
	Texture_SetLoadFile("Assets/Textures/exit_glow.png", 330, 162);
	Texture_SetLoadFile("Assets/Textures/easy.png", 452, 142);
	Texture_SetLoadFile("Assets/Textures/easy_glow.png", 470, 162);
	Texture_SetLoadFile("Assets/Textures/hard.png", 452, 142);
	Texture_SetLoadFile("Assets/Textures/hard_glow.png", 470, 162);
	Texture_Load();
	titleTexID = Texture_GetID("Assets/Textures/title.png");
	startTexID = Texture_GetID("Assets/Textures/start.png");
	startGlowTexID = Texture_GetID("Assets/Textures/start_glow.png");
	exitTexID = Texture_GetID("Assets/Textures/exit.png");
	exitGlowTexID = Texture_GetID("Assets/Textures/exit_glow.png");
	easyTexID = Texture_GetID("Assets/Textures/easy.png");
	easyGlowTexID = Texture_GetID("Assets/Textures/easy_glow.png");
	hardTexID = Texture_GetID("Assets/Textures/hard.png");
	hardGlowTexID = Texture_GetID("Assets/Textures/hard_glow.png");

	inMenu = true;
	difficultyChoose = false;
	selected = 0;

	PlaySound(SOUND_LABEL_TITLEBGM);
}

void titleUninit() {
	StopSound(SOUND_LABEL_TITLEBGM);
}

void titleUpdate() {
	if (Keyboard_IsTrigger(DIK_UP)) {			//Up
		selected--;
		if (selected < 0)
			selected = 1;

		PlaySound(SOUND_LABEL_SE_SELECT);
	}
	if (Keyboard_IsTrigger(DIK_DOWN)) {			//Down
		selected++;
		if (selected > 1)
			selected = 0;

		PlaySound(SOUND_LABEL_SE_SELECT);
	}
	if (Keyboard_IsTrigger(DIK_RETURN)) {		//Press Enter
		PlaySound(SOUND_LABEL_SE_OK);

		if (inMenu) {
			if (selected == 0) {				//Enter choose difficulty phase
				inMenu = false;
				difficultyChoose = true;
			}
			else if (selected == 1) {			//Exit the game

			}
		}
		else if (difficultyChoose) {
			if (selected == 0) {				//Easy Mode
				setGameDifficulty(10);
			}
			else if (selected == 1) {			//Hard Mode
				setGameDifficulty(100);
			}
			Fade(SCENE_GAME);
		}
	}
	if (Keyboard_IsTrigger(DIK_ESCAPE)) {		//Press ESC

	}
}

void titleDraw() {
	Sprite_Draw(titleTexID, SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);

	if (inMenu) {
		if (selected == 0) {
			Sprite_Draw(startGlowTexID, 1050.f, 500.f);
			Sprite_Draw(exitTexID, 1050.f, 700.f);
		}
		else if (selected == 1) {
			Sprite_Draw(startTexID, 1050.f, 500.f);
			Sprite_Draw(exitGlowTexID, 1050.f, 700.f);
		}
	}
	else if (difficultyChoose) {
		if (selected == 0) {
			Sprite_Draw(easyGlowTexID, 1050.f, 500.f);
			Sprite_Draw(hardTexID, 1050.f, 700.f);
		}
		else if (selected == 1) {
			Sprite_Draw(easyTexID, 1050.f, 500.f);
			Sprite_Draw(hardGlowTexID, 1050.f, 700.f);
		}
	}
}