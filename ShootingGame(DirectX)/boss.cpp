#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "boss.h"
#include "main.h"
#include "game.h"
#include "player.h"
#include "sprite.h"
#include "texture.h"
#include "sound.h"
#include "mydirect3d.h"
#include "debug_font.h"

//==========boss parameters==========//
static int g_boss_textureID;
static int g_bossShot_textureID;

//���W
D3DXVECTOR2 boss_position, prev_boss_position;

//moving pattern now
int move_pattern;
int prev_move_pattern;
//shooting pattern now
int shot_pattern;
int prev_shot_pattern;

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

//Boss hp
int bossHp;
int prevBossHp;

//Invincible flag
bool bossInvincible;

//Appear flag
bool bossAppears;

//Difficulty
GAME_DIFFICULTY difficulty;
//===================================//


void bossInit() {
	boss_position = prev_boss_position = { 400.f, -100.f };

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
	prev_move_pattern = 0;
	shot_pattern = 0;
	prev_shot_pattern = 0;

	nextMoveX = 0;
	nextMoveY = 180;

	waitCount = 0;
	bossShotCount = 0;

	waitFlag = false;
	p3_state = 0;

	boss_damageFlag = false;
	boss_aliveFlag = true;
	boss_shotFlag = false;

	bossHp = BOSS_HP;
	prevBossHp = bossHp;

	bossInvincible = false;

	bossAppears = false;

	difficulty = getGameDifficulty();
}

void bossDraw() {

	if (boss_aliveFlag) {
		//Draw Boss
		if (!boss_damageFlag) {		//Normal Phase
			Sprite_Draw(g_boss_textureID, boss_position.x, boss_position.y);
		}
		else {						//Get Damage Phase 

		}
	}

	//Draw Boss' bullets
	for (int i = 0; i < BOSS_SHOTNUM; i++) {
		if (bossShots[i].flag) {
			switch (bossShots[i].pattern)
			{
			case 0:
			case 1:
				Sprite_Draw(g_bossShot_textureID, bossShots[i].x, bossShots[i].y, 0, 44, bossShots[i].width, bossShots[i].height, bossShots[i].x, bossShots[i].y, bossShots[i].radian + D3DX_PI / 2.f);
				break;
			case 2:
				Sprite_Draw(g_bossShot_textureID, bossShots[i].x, bossShots[i].y, 0, 82, 42, 42, bossShots[i].x, bossShots[i].y, bossShots[i].radian + D3DX_PI / 2.f);
				break;
			default: break;
			}
		}
	}

	boss_damageFlag = false;
}

void bossMove() {
	if (boss_aliveFlag) {
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
		case 4:
			bossMoveToDefault();
			break;
		default: break;
		}
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

void bossMoveToDefault() {
	float temp;

	angle += 2;

	temp = sin(angle*D3DX_PI / 180);

	boss_position.x = prev_boss_position.x + temp * nextMoveX;
	boss_position.y = prev_boss_position.y + temp * nextMoveY;

	if (angle == 90) {
		setBossMovePattern(prev_move_pattern + 1);
		setBossShotPattern(prev_shot_pattern + 1);
		bossInvincible = false;

		if (move_pattern == 3) {
			bossMoveInit(400, 160, 2);
		}
	}
}

void bossAppear() {
	float temp;

	angle += 2;

	temp = sin(angle*D3DX_PI / 180.f);

	boss_position.x = 400.f;
	boss_position.y = prev_boss_position.y + temp * nextMoveY;

	//When angle == 90, sin(angle) equals 1, so it means the boss has arrived its destination
	if (angle == 90) {
		move_pattern = 1;
		angle = 0;
		boss_shotFlag = true;
	}
}

void bossMovePattern1() {

	boss_position.x = 400.f;

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

		if (boss_position.x == 170) {
			horizontalRaise = 2;
			waitFlag = true;
		}
		else if (boss_position.x == 730) {
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
		else if (p3_state == 1)
			bossMoveInit(200.f, 160.f, 2);
		else
			bossMoveInit(330.f, 80.f, 0);
	}
}

void setBossMovePattern(int pattern) {
	prev_move_pattern = move_pattern;
	move_pattern = pattern;
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
		if (boss_aliveFlag) {
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

						if (difficulty == DIFFICULTY_EASY) {			//Easy Mode
							if (num == 0)
								bossShots[index].radian = trad - (20 * D3DX_PI / 180.f);
							else if (num == 1)
								bossShots[index].radian = trad - (10 * D3DX_PI / 180.f);
							else if (num == 2)
								bossShots[index].radian = trad;
							else if (num == 3)
								bossShots[index].radian = trad + (10 * D3DX_PI / 180.f);
							else if (num == 4)
								bossShots[index].radian = trad + (20 * D3DX_PI / 180.f);

							num++;
							if (num == 5)
								break;
						}
						else if (difficulty == DIFFICULTY_HARD) {	//Hard Mode
							if (num == 0)
								bossShots[index].radian = trad - (24 * D3DX_PI / 180.f);
							else if (num == 1)
								bossShots[index].radian = trad - (16 * D3DX_PI / 180.f);
							else if (num == 2)
								bossShots[index].radian = trad - (8 * D3DX_PI / 180.f);
							else if (num == 3)
								bossShots[index].radian = trad;
							else if (num == 4)
								bossShots[index].radian = trad + (8 * D3DX_PI / 180.f);
							else if (num == 5)
								bossShots[index].radian = trad + (16 * D3DX_PI / 180.f);
							else if (num == 6)
								bossShots[index].radian = trad + (24 * D3DX_PI / 180.f);

							num++;
							if (num == 7)
								break;
						}
					}
					//Play enemy shot SE
					if (bossShotCount == 0)
						PlaySound(SOUND_LABEL_SE_ESHOT);
				}
				break;
			case 1:
				int interval;
				if (difficulty == DIFFICULTY_EASY)
					interval = 20;
				else if (DIFFICULTY_HARD)
					interval = 10;
				if (bossShotCount % interval == 0) {
					trad = atan2(playerY - boss_position.y, playerX - boss_position.x);
					while ((index = shotSearch()) != -1) {
						bossShots[index].pattern = 1;
						bossShots[index].speed = 1;
						bossShots[index].radian = trad + ((rand() % 120) * D3DX_PI / 180.f) - (60 * D3DX_PI / 180.f);

						num++;
						if (num == 2)
							break;
					}
					//Play enemy shot SE
					PlaySound(SOUND_LABEL_SE_ESHOT);
				}
				break;
			case 2:
				int tamaNum;
				if (difficulty == DIFFICULTY_EASY)
					tamaNum = 10;
				else if (DIFFICULTY_HARD)
					tamaNum = 20;
				if (bossShotCount % 30 == 0) {
					trad = atan2(playerY - boss_position.y, playerX - boss_position.x);
					while ((index = shotSearch()) != -1) {
						bossShots[index].pattern = 2;
						bossShots[index].speed = 2;
						bossShots[index].radian = trad + num * ((360 / tamaNum) * D3DX_PI / 180.f);

						num++;
						if (num == tamaNum)
							break;
					}
					//Play enemy shot SE
					PlaySound(SOUND_LABEL_SE_ESHOT);
				}
				break;
			case 3:
				break;
			default: break;
			}
		}

		//Move the bullets
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
					bossShots[i].x += cos(bossShots[i].radian) * bossShots[i].speed;
					bossShots[i].y += sin(bossShots[i].radian) * bossShots[i].speed;
					break;
				case 2:
					bossShots[i].x += cos(bossShots[i].radian) * bossShots[i].speed;
					bossShots[i].y += sin(bossShots[i].radian) * bossShots[i].speed;
					break;
				case 3:
					break;
				default: break;
				}
			}

			if (bossShotRangeCheck(i)) {
				bossShots[i].flag = false;
			}
		}

		bossShotCount++;

		if (shotCountFlag) {
			bossShotCount = 0;
		}
	}
}

void setBossShotPattern(int pattern) {
	prev_shot_pattern = shot_pattern;
	shot_pattern = pattern;
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

bool bossShotRangeCheck(int index) {
	if (bossShots[index].x < 50 || bossShots[index].x > 850 || bossShots[index].y < -10 || bossShots[index].y > 900)
		return true;
	else
		return false;
}


void setBossDamageFlag() {

}

void setBossFlag(bool flag) {
	boss_aliveFlag = flag;
}

bool getBossFlag() {
	return boss_aliveFlag;
}

bool getBossInvincibleFlag() {
	return bossInvincible;
}

void setBossAppearFlag(bool flag) {
	bossAppears = flag;
}

bool getBossAppearFlag() {
	return bossAppears;
}

void getBossPosition(float* x, float *y) {
	*x = boss_position.x;
	*y = boss_position.y;
}

bool getBossShotPosition(int index, float *x, float *y, int *type) {
	if (bossShots[index].flag) {
		*x = bossShots[index].x;
		*y = bossShots[index].y;
		*type = bossShots[index].type;
		return true;
	}
	else
		return false;
}

void setBossShotFlag(int index, bool flag) {
	bossShots[index].flag = flag;
}

int setBossHp(int damage) {
	prevBossHp = bossHp;
	bossHp -= damage;
	if (bossHp < 0)
		bossHp = 0;
	return bossHp;
}

int  getPrevBossHp() {
	return prevBossHp;
}

void setBossDamageSetting() {
	prev_boss_position.x = boss_position.x;
	prev_boss_position.y = boss_position.y;

	nextMoveX = 400.f - boss_position.x;
	nextMoveY = 160.f - boss_position.y;

	angle = 0;

	bossInvincible = true;

	setBossMovePattern(4);
	setBossShotPattern(4);
}