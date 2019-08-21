#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "player.h"
#include "main.h"
#include "sprite.h"
#include "texture.h"
#include "mydirect3d.h"
#include "input.h"


Vertex2d v3[] = {
	{D3DXVECTOR4(100.0f, 500.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 0.0f)},
	{D3DXVECTOR4(300.0f, 500.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 0.0f)},
	{D3DXVECTOR4(100.0f, 700.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(0.0f, 1.0f)},
	{D3DXVECTOR4(300.0f, 700.0f, 0.0f, 1.0f), D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR2(1.0f, 1.0f)}
};

//Texture IDs
int Texture_IDs[100];
int textureNum;

//player parameter
static D3DXVECTOR2 g_player_position;
static float g_player_speed;
static int g_player_textureID;
void playerInit() {

	Texture_SetLoadFile("Asset/Texture/player.png", 256, 256);
	Texture_Load();

	g_player_position = D3DXVECTOR2(SCREEN_WIDTH*0.5f, SCREEN_HEIGHT - PLAYER_HEIGHT);
	g_player_speed = PLAYER_DEFAULT_SPEED;
	g_player_textureID = Texture_GetID("Asset/Texture/player.png");
}

void playerUninit() {

}

void playerUpdate() {
	D3DXVECTOR2 dir(0.0f, 0.0f);

	if (Keyboard_IsPress(DIK_W)) {		//Move Up
		//moveY -= moveSpeed;
		dir.y -= 1.0f;
	}
	if (Keyboard_IsPress(DIK_A)) {		//Move Left
		//moveX -= moveSpeed;
		dir.x -= 1.0f;
	}
	if (Keyboard_IsPress(DIK_S)) {		//Move Down
		//moveY += moveSpeed;
		dir.y += 1.0f;
	}
	if (Keyboard_IsPress(DIK_D)) {		//Move Right
		//moveX += moveSpeed;
		dir.x += 1.0f;
	}
	if (Keyboard_IsPress(DIK_B)) {		//Fire Bullet
		//moveX += moveSpeed;
		//CreateBullet(g_player_position.x + 40.0f, g_player_position.y, { 2.0, 0.0 }, BULLET_NORMAL);
	}

	//位置更新
	D3DXVec2Normalize(&dir, &dir);
	g_player_position += dir * PLAYER_DEFAULT_SPEED; //プレイヤー座標 = プレイヤー座標 + 方向ベクトルX速度(単位ベクトル)
	//angleNow += moveAngle;

	////境界判定
	//nowX = max(nowX, Texture_GetWidth(1) / 2);
	//nowX = min(nowX, SCREEN_WIDTH - Texture_GetWidth(1) / 2);
	//nowY = max(nowY, Texture_GetHeight(1) / 2);
	//nowY = min(nowY, SCREEN_HEIGHT + 100 - Texture_GetHeight(1) / 2);
}

void playerDraw() {

	//DrawPrimitiveUPの引数　=>　図形の描くタイプ　　数　　頂点データ先頭アドレス　　頂点1個分のサイズ

	Sprite_Draw(g_player_textureID, g_player_position.x, g_player_position.y/*, g_player_position.x, g_player_position.y, angleNow, scaleX, scaleY, 1.0f*/);
	
}