#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "boss.h"
#include "main.h"
#include "player.h"
#include "sprite.h"
#include "texture.h"
#include "mydirect3d.h"
#include "debug_font.h"

//==========boss parameters==========//
static int g_boss_textureID;
static int g_bossShot_textureID;

//ç¿ïW
D3DXVECTOR2 boss_position, prev_boss_position;

//moving pattern now
int move_pattern;
//shooting pattern now
int shot_pattern;

//bullets
E_SHOT bossShots[BOSS_SHOTNUM];

//total angle and the angle to raise
int angle, angleRaise;

//the amount to move horizontal
int horizontalRaise;
int waitCount;
bool waitFlag;

//the distance left to the next destination
float nextMoveX, nextMoveY;
int p3_state;

//damage flag, set to true when get any damage
bool boss_damageFlag;

//alive flag
bool boss_aliveFlag;

//shot flag
bool boss_shotFlag;
int bossShotCount;
//===================================//


void bossInit() {
	boss_position = prev_boss_position = { 200.f, -100.f };

	Texture_SetLoadFile("Assets/Textures/boss.png", 240, 240);
	Texture_SetLoadFile("Assets/Textures/bullet.png", 1024, 1024);
	Texture_Load();
	g_boss_textureID = Texture_GetID("Assets/Textures/boss.png");
	g_bossShot_textureID = Texture_GetID("Assets/Textures/bullet.png");

	//Initialize the bullets
	for (int i = 0; i < BOSS_SHOTNUM; i++) {
		bossShots[i].flag = false;
		bossShots[i].pattern = 0;
		bossShots[i].radian = 0;
		bossShots[i].speed = 0;
		bossShots[i].width = 40;
		bossShots[i].height = 36;
		bossShots[i].x = 0;
		bossShots[i].y = 0;
	}

	angleRaise = 2;
	horizontalRaise = 2;
	angle = 0;
	move_pattern = 0;
	shot_pattern = 0;

	nextMoveX = 0;
	nextMoveY = 180;

	waitCount = 0;
	bossShotCount = 0;

	waitFlag = false;
	p3_state = 0;

	boss_damageFlag = false;
	boss_aliveFlag = true;
	boss_shotFlag = false;
}

void bossDraw() {

	//Draw Boss' bullets
	for (int i = 0; i < BOSS_SHOTNUM; i++) {
		if (bossShots[i].flag) {
			Sprite_Draw(g_bossShot_textureID, bossShots[i].x, bossShots[i].y, 0, 44, bossShots[i].width, bossShots[i].height, bossShots[i].x, bossShots[i].y, bossShots[i].radian + D3DX_PI / 2.f);
		}
	}

	//Draw Boss
	if (!boss_damageFlag) {		//Normal Phase
		Sprite_Draw(g_boss_textureID, boss_position.x, boss_position.y);
	}
	else {						//Get Damage Phase 

	}

	boss_damageFlag = false;
}

void bossMove() {
	switch (move_pattern)
	{
	case 0:
		bossAppear();
		break;
	case 1:
		bossMovePattern1();
		break;
	case 2:
		bossMovePattern2();
		break;
	case 3:
		bossMovePattern3();
		break;
	default: break;
	}

	if (boss_shotFlag)
		bossShot();
}

void bossMoveInit(float nextX, float nextY, int state) {
	prev_boss_position.x = boss_position.x;
	prev_boss_position.y = boss_position.y;

	nextMoveX = nextX - boss_position.x;
	nextMoveY = nextY - boss_position.y;

	angle = 0;

	p3_state = state;
}

void bossAppear() {
	float temp;

	angle += 2;

	temp = sin(angle*D3DX_PI / 180.f);

	boss_position.x = 200.f;
	boss_position.y = prev_boss_position.y + temp * nextMoveY;

	//When angle == 90, sin(angle) equals 1, so it means the boss has arrived its destination
	if (angle == 90) {
		move_pattern = 1;
		angle = 0;
		boss_shotFlag = true;
	}
}

void bossMovePattern1() {

	boss_position.x = 200.f;

	angle += angleRaise;
	boss_position.y = 80.f + sin(angle*D3DX_PI / 180.f) * BOSS_SHAKE;

	if (angle == 90)
		angleRaise = -2;
	else if (angle == -90)
		angleRaise = 2;
}

void bossMovePattern2() {
	if (!waitFlag) {
		boss_position.x += horizontalRaise;

		if (boss_position.x == 70) {
			horizontalRaise = 2;
			waitFlag = true;
		}
		else if (boss_position.x == 330) {
			horizontalRaise = -2;
			waitFlag = true;
		}
	}

	if (waitFlag) {
		waitCount++;
		if (waitCount == 20) {
			waitFlag = false;
			waitCount = 0;
		}
	}
}

void bossMovePattern3() {
	float temp;

	angle += 2;

	temp = sin(angle*D3DX_PI / 180.f);

	boss_position.x = prev_boss_position.x + temp * nextMoveX;
	boss_position.y = prev_boss_position.y + temp * nextMoveY;

	if (angle == 90) {
		if (p3_state == 0)
			bossMoveInit(70.f, 80.f, 1);
		else if (p3_state == 0)
			bossMoveInit(200.f, 160.f, 2);
		else
			bossMoveInit(330.f, 80.f, 0);
	}
}

void bossShot() {
	//How much bullets did the boss shot
	int num = 0;
	//The index of the bullet that is not used
	int index;

	bool shotCountFlag = false;

	float playerX, playerY;
	static float trad;

	if (!boss_damageFlag) {
		getPlayerPosition(&playerX, &playerY);

		if (bossShotCount == 0)
			trad = atan2(playerY - boss_position.y, playerX - boss_position.x);

		//Set the bullets
		switch (shot_pattern)
		{
		case 0:
			if (bossShotCount % 5 == 0 && bossShotCount <= 15) {
				while ((index = shotSearch()) != -1) {
					bossShots[index].pattern = 0;
					bossShots[index].speed = 6;

					if (num == 0)
						bossShots[index].radian = trad - (10 * D3DX_PI / 180.f);
					else if (num == 1)
						bossShots[index].radian = trad - (5 * D3DX_PI / 180.f);
					else if (num == 2)
						bossShots[index].radian = trad;
					else if (num == 3)
						bossShots[index].radian = trad + (5 * D3DX_PI / 180.f);
					else if (num == 4)
						bossShots[index].radian = trad + (10 * D3DX_PI / 180.f);

					num++;

					if (num == 5)
						break;
				}
			}
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		default: break;
		}

		for (int i = 0; i < BOSS_SHOTNUM; i++) {
			if (bossShots[i].flag) {
				switch (bossShots[i].pattern)
				{
				case 0:
					bossShots[i].x += cos(bossShots[i].radian) * bossShots[i].speed;
					bossShots[i].y += sin(bossShots[i].radian) * bossShots[i].speed;

					if (bossShotCount == 40 && shotCountFlag == false)
						shotCountFlag = true;
					break;
				case 1:
					break;
				case 2:
					break;
				case 3:
					break;
				default: break;
				}
			}
		}

		bossShotCount++;

		if (shotCountFlag) {
			bossShotCount = 0;
		}
	}
}

void bossShotPattern1() {

}

void bossShotPattern2() {

}

void bossShotPattern3() {

}

int shotSearch() {

	for (int i = 0; i < BOSS_SHOTNUM; i++) {
		if (!bossShots[i].flag) {
			bossShots[i].flag = true;
			bossShots[i].x = boss_position.x;
			bossShots[i].y = boss_position.y;
			return i;
		}
	}

	return -1;
}


void setBossDamageFlag() {

}

void setBossFlag(bool flag) {

}

bool getBossFlag() {
	return true;
}
