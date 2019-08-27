﻿#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "player.h"
#include "main.h"
#include "sprite.h"
#include "texture.h"
#include "mydirect3d.h"
#include "input.h"
#include "enemy.h"
#include "tama.h"
#include "debug_font.h"

//player parameter
static D3DXVECTOR2 g_player_position;
static float g_player_speed;
static int g_player_textureID;
static int g_player_centerTexID;
//移動係数
static float move;
//横方向と縦方向のカウント数。
int xcount, ycount;
//生きてるかどうかのフラグ
int life;
bool damageFlag;
bool endFlag;
//When get damage, counter that counts the frame when player sparks
int damageCounter;

//Slow motion
bool isMovingSlow;
//Move direction
bool isMovingRight, isMovingLeft;
//弾
SHOT shot[PSHOT_NUM];
//カウント
int shotCount;
//Shooting CD
float shootingCD;
//Power
int power;

void playerInit() {

	Texture_SetLoadFile("Assets/Textures/player.png", 512, 512);
	Texture_SetLoadFile("Assets/Textures/kShot.png", 512, 512);
	Texture_Load();

	g_player_position = D3DXVECTOR2(PLAYER_INITX, PLAYER_INITY);
	g_player_speed = PLAYER_DEFAULT_SPEED;
	g_player_textureID = Texture_GetID("Assets/Textures/player.png");
	g_player_centerTexID = Texture_GetID("Assets/Textures/kShot.png");

	move = 1.f;
	shootingCD = 1.f;
	power = 1;
	tamaInit();

	life = 1;
	damageFlag = false;
	endFlag = false;
	damageCounter = 0;

	isMovingSlow = false;
	isMovingRight = false;
	isMovingLeft = false;

	//フラグを全部falseにしとく
	//グラフィックハンドルと画像のサイズを代入しとく
	for (int i = 0; i < PSHOT_NUM; ++i) {
		shot[i].flag = false;
		shot[i].gh = Texture_GetID("Assets/Textures/kShot.png");
		shot[i].width = 17;
		shot[i].height = 21;
		shot[i].type = 0;
	}
	shotCount = 0;

}

void playerUninit() {

}

void playerUpdate() {

	//移動
	playerMove();

	//発射
	playerShot();
	++shotCount;

	//境界判定
	if (!damageFlag) {
		g_player_position.x = max(g_player_position.x, 30.f + PLAYER_WIDTH / 2.f);
		g_player_position.x = min(g_player_position.x, 775.f - PLAYER_WIDTH / 2.f);
		g_player_position.y = max(g_player_position.y, 25.f + PLAYER_HEIGHT / 2.f);
		g_player_position.y = min(g_player_position.y, SCREEN_HEIGHT - 25.f - PLAYER_HEIGHT / 2.f);
	}
}

void playerDraw() {

	//Draw Bullets
	for (int i = 0; i < PSHOT_NUM; ++i) {
		if (shot[i].flag) {
			Sprite_Draw(shot[i].gh, shot[i].x, shot[i].y, 0, 215, shot[i].width, shot[i].height, shot[i].x, shot[i].y, shot[i].radian + D3DX_PI / 2.f);
		}
	}

	//Draw Player
	if (damageFlag) {	//Get Damage Phase
		if (damageCounter == 0) //Set the player a little below the initial position
			g_player_position = D3DXVECTOR2(PLAYER_INITX, PLAYER_INITY + 158);
		if (damageCounter > 20) {
			if (damageCounter % 2 == 0)
				Sprite_Draw(g_player_textureID, g_player_position.x, g_player_position.y, 0, 0, 100, 100, 140);
		}
		damageCounter++;
		if (damageCounter == 80) {
			damageFlag = false;
			damageCounter = 0;
		}
	}
	else {				//Normal Phase
		if (!isMovingLeft && !isMovingRight)
			Sprite_Draw(g_player_textureID, g_player_position.x, g_player_position.y, 0, 0, 100, 100);
		else if (isMovingLeft)
			Sprite_Draw(g_player_textureID, g_player_position.x, g_player_position.y, 0, 100, 100, 100);
		else if (isMovingRight)
			Sprite_Draw(g_player_textureID, g_player_position.x, g_player_position.y, 0, 200, 100, 100);
		if (isMovingSlow)
			Sprite_Draw(g_player_centerTexID, g_player_position.x, g_player_position.y, 0, 0, 20, 20);
	}

	//陰陽玉
	showTama();

	DebugFont_Draw(32, 32, "x = %.2f, y = %.2f", g_player_position.x, g_player_position.y);
}

void playerMove() {
	if (!damageFlag) {
		isMovingLeft = false;
		isMovingRight = false;

		if (Keyboard_IsPress(DIK_LEFT) || Keyboard_IsPress(DIK_RIGHT)) {

			if (Keyboard_IsPress(DIK_LEFT)) {
				isMovingLeft = true;
				isMovingRight = false;
			}
			else {
				isMovingLeft = false;
				isMovingRight = true;
			}

			if (Keyboard_IsPress(DIK_UP) || Keyboard_IsPress(DIK_DOWN))
				move = 0.71f;
			else
				move = 1.0f;
		}
		else if (Keyboard_IsPress(DIK_UP) || Keyboard_IsPress(DIK_DOWN)) {
			move = 1.0f;
		}

		if (Keyboard_IsPress(DIK_LSHIFT)) {
			move *= 0.5f;
			isMovingSlow = true;
		}
		else
			isMovingSlow = false;

		if (Keyboard_IsPress(DIK_LEFT))
			g_player_position.x -= PLAYER_DEFAULT_SPEED * move;
		if (Keyboard_IsPress(DIK_RIGHT))
			g_player_position.x += PLAYER_DEFAULT_SPEED * move;
		if (Keyboard_IsPress(DIK_UP))
			g_player_position.y -= PLAYER_DEFAULT_SPEED * move;
		if (Keyboard_IsPress(DIK_DOWN))
			g_player_position.y += PLAYER_DEFAULT_SPEED * move;
	}
	else {
		g_player_position.y -= 2.f;
	}
}

void playerShot() {

	//Control the how many lines of bullets being shot
	int num = 0;

	if (!damageFlag) {
		//キーが押されててかつ、5ループに一回発射
		if (Keyboard_IsPress(DIK_Z) && shotCount % 5 == 0) {
			for (int i = 0; i < PSHOT_NUM; ++i) {
				if (shot[i].flag == false) {
					if (power < 5) {
						shot[i].flag = true;
						shot[i].x = g_player_position.x;
						shot[i].y = g_player_position.y;
						shot[i].radian = -D3DX_PI / 2.f;
						shot[i].type = 0;
						break;
					}
					else if (power >= 5 && power < 10) {
						if (num == 0) {
							shot[i].flag = true;
							shot[i].x = g_player_position.x;
							shot[i].y = g_player_position.y;
							shot[i].radian = -D3DX_PI / 2.f;
							shot[i].type = 0;
						}
						else if (num == 1) {
							shot[i].flag = true;
							shot[i].x = g_player_position.x;
							shot[i].y = g_player_position.y;
							shot[i].radian = -D3DX_PI / 2.f - 0.12f;
							shot[i].type = 0;
						}
						else if (num == 2) {
							shot[i].flag = true;
							shot[i].x = g_player_position.x;
							shot[i].y = g_player_position.y;
							shot[i].radian = -D3DX_PI / 2.f + 0.12;
							shot[i].type = 0;
						}

						num++;
						if (num == 3)
							break;
					}
					else if (power == 10) {
						if (num == 0) {
							shot[i].flag = true;
							shot[i].x = g_player_position.x;
							shot[i].y = g_player_position.y;
							shot[i].radian = -D3DX_PI / 2.f;
							shot[i].type = 0;
						}
						else if (num == 1) {
							shot[i].flag = true;
							shot[i].x = g_player_position.x;
							shot[i].y = g_player_position.y;
							shot[i].radian = -D3DX_PI / 2.f - 0.12f;
							shot[i].type = 0;
						}
						else if (num == 2) {
							shot[i].flag = true;
							shot[i].x = g_player_position.x;
							shot[i].y = g_player_position.y;
							shot[i].radian = -D3DX_PI / 2.f + 0.12;
							shot[i].type = 0;
						}
						else if (num > 2) {
							tamaShotSet(i);
						}

						num++;
						if (num == 5)
							break;
					}
				}
			}
		}
	}

	//nearest enemy's radian
	float tRadian;
	//nearest enemy's index and position
	int eIndex;
	float eX, eY;
	eIndex = searchNearbyEnemy();

	//弾を移動させる処理
	for (int i = 0; i < PSHOT_NUM; ++i) {
		//発射してる弾だけ
		if (shot[i].flag) {
			if (shot[i].type == 0) {
				shot[i].x += cos(shot[i].radian) * PSHOT_SPEED;
				shot[i].y += sin(shot[i].radian) * PSHOT_SPEED;
			}
			else if (shot[i].type == 1) {
				//There are no enemies
				if (eIndex == -1)
					tRadian = -D3DX_PI / 2.f;
				else {
					getEnemyPositions(eIndex, &eX, &eY);
					tRadian = atan2(eY - shot[i].y, eX - shot[i].x);
				}

				shot[i].radian = tRadian;
				shot[i].x += cos(shot[i].radian) * PSHOT_SPEED;
				shot[i].y += sin(shot[i].radian) * PSHOT_SPEED;
			}

			//画面の外にはみ出したらフラグを戻す
			if (checkShotOutOfRange(i)) {
				shot[i].flag = false;
			}
		}
	}
}

void getPlayerPosition(float *x, float *y) {
	*x = g_player_position.x;
	*y = g_player_position.y;
}

bool getPlayerShotPosition(int index, float *x, float *y) {
	if (shot[index].flag) {
		*x = shot[index].x;
		*y = shot[index].y;
		return true;
	}
	else
		return false;
}

void setPlayerShotFlag(int index, bool flag) {
	shot[index].flag = flag;
}

void setPlayerDamageFlag() {
	damageFlag = true;
}

bool getPlayerDamageFlag() {
	return damageFlag;
}

bool checkShotOutOfRange(int index) {
	if (shot[index].y < -10 || shot[index].x < 50 || shot[index].x > 750)
		return true;
	else
		return false;
}

void upgradePlayerPower(int p) {
	power += p;
	if (power > 10)
		power = 10;
}

int  getPlayerPower() {
	return power;
}

void showTama() {
	if (power == 10)
		tamaAll(g_player_position.x, g_player_position.y);
}

int searchNearbyEnemy() {
	int nearEnemyIndex = -1;
	float nearResult;
	float eX, eY, tX, tY;

	for (int i = 0; i < ENEMY_NUM; i++) {
		if (!getEnemyPositions(i, &eX, &eY))
			continue;

		tX = eX - g_player_position.x;
		tY = eY - g_player_position.y;

		if (nearEnemyIndex == -1) {
			nearEnemyIndex = i;
			nearResult = tX * tX + tY * tY;
			continue;
		}

		if (nearResult > tX * tX + tY * tY) {
			nearEnemyIndex = i;
			nearResult = tX * tX + tY * tY;
		}
	}
	return nearEnemyIndex;
}

void tamaShotSet(int index) {
	float tY;
	float tRadian, eX, eY;
	static int toggle = 1;
	int tIndex;

	tY = tamaGetPosition();

	tIndex = searchNearbyEnemy();
	if (tIndex == -1) {
		tRadian = -D3DX_PI / 2.f;
	}
	else {
		getEnemyPositions(tIndex, &eX, &eY);
		tRadian = atan2(eY - tY + TAMA_INITY, eX - g_player_position.x + (toggle*TAMA_INITX));
	}

	shot[index].flag = true;
	shot[index].x = g_player_position.x + TAMA_INITX * toggle;
	shot[index].y = tY + TAMA_INITY;
	shot[index].radian = tRadian;
	shot[index].type = 1;

	if (toggle == 1)
		toggle = -1;
	else
		toggle = 1;
}