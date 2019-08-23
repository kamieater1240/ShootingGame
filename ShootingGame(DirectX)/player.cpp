#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "player.h"
#include "main.h"
#include "sprite.h"
#include "texture.h"
#include "mydirect3d.h"
#include "input.h"
#include "debug_font.h"

//player parameter
static D3DXVECTOR2 g_player_position;
static float g_player_speed;
static int g_player_textureID;
//移動係数
static float move;
//横方向と縦方向のカウント数。
int xcount, ycount;
//生きてるかどうかのフラグ
bool life;
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

void playerInit() {

	Texture_SetLoadFile("Assets/Textures/player.png", 512, 512);
	Texture_SetLoadFile("Assets/Textures/kShot.png", 512, 512);
	//Texture_SetLoadFile("Assets/Textures/bullet.png", 1024, 1024);
	Texture_Load();

	//g_player_position = D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - PLAYER_HEIGHT);
	g_player_position = D3DXVECTOR2(100.f, 100.f);
	g_player_speed = PLAYER_DEFAULT_SPEED;
	g_player_textureID = Texture_GetID("Assets/Textures/player.png");

	move = 1.f;
	shootingCD = 1.f;
	life = true;
	isMovingSlow = false;
	isMovingRight = false;
	isMovingLeft = false;

	//フラグを全部falseにしとく
	//グラフィックハンドルと画像のサイズを代入しとく
	for (int i = 0; i < PSHOT_NUM; ++i) {
		shot[i].flag = false;
		//shot[i].gh = Texture_GetID("Assets/Textures/bullet.png");
		shot[i].gh = Texture_GetID("Assets/Textures/kShot.png");
		shot[i].width = 17;
		shot[i].height = 21;
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
	g_player_position.x = max(g_player_position.x, 30.f + PLAYER_WIDTH / 2.f);
	g_player_position.x = min(g_player_position.x, 775.f - PLAYER_WIDTH / 2.f);
	g_player_position.y = max(g_player_position.y, 25.f + PLAYER_HEIGHT / 2.f);
	g_player_position.y = min(g_player_position.y, SCREEN_HEIGHT - 25.f - PLAYER_HEIGHT / 2.f);
}

void playerDraw() {

	//Draw Bullets
	for (int i = 0; i < PSHOT_NUM; ++i) {
		if (shot[i].flag) {
			Sprite_Draw(shot[i].gh, shot[i].x, shot[i].y, 0, 215, shot[i].width, shot[i].height);
		}
	}

	//Draw Player
	if (!isMovingLeft && !isMovingRight)
		Sprite_Draw(g_player_textureID, g_player_position.x, g_player_position.y, 0, 0, 100, 100);
	else if (isMovingLeft)
		Sprite_Draw(g_player_textureID, g_player_position.x, g_player_position.y, 0, 100, 100, 100);
	else if (isMovingRight)
		Sprite_Draw(g_player_textureID, g_player_position.x, g_player_position.y, 0, 200, 100, 100);
	if (isMovingSlow)
		Sprite_Draw(1, g_player_position.x, g_player_position.y, 0, 0, 20, 20);

	DebugFont_Draw(32, 32, "x = %.2f, y = %.2f", g_player_position.x, g_player_position.y);
}

void playerMove() {

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

void playerShot() {

	//キーが押されててかつ、5ループに一回発射
	if (Keyboard_IsPress(DIK_Z) && shotCount % 5 == 0) {
		for (int i = 0; i < PSHOT_NUM; ++i) {
			if (shot[i].flag == false) {
				shot[i].flag = true;
				shot[i].x = g_player_position.x;
				shot[i].y = g_player_position.y;
				break;
			}
		}
	}

	//弾を移動させる処理
	for (int i = 0; i < PSHOT_NUM; ++i) {
		//発射してる弾だけ
		if (shot[i].flag) {
			shot[i].y -= PSHOT_SPEED;
			//画面の外にはみ出したらフラグを戻す
			if (shot[i].y < -10) {
				shot[i].flag = false;
			}
		}
	}
}