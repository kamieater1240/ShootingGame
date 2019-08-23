#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include "enemy.h"
#include "main.h"
#include "sprite.h"
#include "texture.h"
#include "mydirect3d.h"
#include "input.h"
#include "game.h"
#include "debug_font.h"

static int g_enemy_textureID;
static int g_enemyShot_textureID;

ENEMY enemy[ENEMY_NUM];
//ENEMY enemy;
ENEMYDATA data[ENEMY_NUM];

void enemyInit() {
	Texture_SetLoadFile("Assets/Textures/enemy.png", 512, 512);
	Texture_SetLoadFile("Assets/Textures/bullet.png", 1024, 1024);
	Texture_Load();
	g_enemy_textureID = Texture_GetID("Assets/Textures/enemy.png");
	g_enemyShot_textureID = Texture_GetID("Assets/Textures/bullet.png");

	//Read enemy data from file
	readENEMYDATA();
}

void readENEMYDATA() {
	FILE *fp;
	char buf[100];
	int c;
	int col = 1;
	int row = 0;

	memset(buf, 0, sizeof(buf));
	fp = fopen("ENEMYDATA1.csv", "r");

	//ヘッダ読み飛ばす
	while (1) {
		while (1) {
			c = fgetc(fp);

			//末尾ならループを抜ける。
			if (c == EOF)
				goto out;

			//カンマか改行でなければ、文字としてつなげる
			if (c != ',' && c != '\n')
				strcat(buf, (const char*)&c);
			//カンマか改行ならループ抜ける。
			else
				break;
		}
		//ここに来たということは、1セル分の文字列が出来上がったということ
		switch (col) {
			//1列目は敵種類を表す。atoi関数で数値として代入。
		case  1: data[row].type = atoi(buf); break;
		case  2: data[row].shotType = atoi(buf); break;
		case  3: data[row].move_pattern = atoi(buf); break;
		case  4: data[row].shot_pattern = atoi(buf); break;
		case  5: data[row].in_time = atoi(buf); break;
		case  6: data[row].stop_time = atoi(buf); break;
		case  7: data[row].shot_time = atoi(buf); break;
		case  8: data[row].out_time = atoi(buf); break;
		case  9: data[row].x = atoi(buf); break;
		case 10: data[row].y = atoi(buf); break;
		case 11: data[row].speed = atoi(buf); break;
		case 12: data[row].hp = atoi(buf); break;
		case 13: data[row].item = atoi(buf); break;
		default: break;
		}
		//バッファを初期化
		memset(buf, 0, sizeof(buf));
		//列数を足す
		col++;

		//もし読み込んだ文字が改行だったら列数を初期化して行数を増やす
		if (c == '\n') {
			col = 1;
			row++;
		}
	}

out:
	//敵クラス生成
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i].width = 64.f;
		enemy[i].height = 64.f;

		enemy[i].type = data[i].type;
		enemy[i].shotType = data[i].shotType;

		enemy[i].move_pattern = data[i].move_pattern;
		enemy[i].shot_pattern = data[i].shot_pattern;

		enemy[i].enemy_position.x = data[i].x;
		enemy[i].enemy_position.y = data[i].y;

		enemy[i].in_time = data[i].in_time;
		enemy[i].stop_time = data[i].stop_time;
		enemy[i].shot_time = data[i].shot_time;
		enemy[i].out_time = data[i].out_time;

		enemy[i].hp = data[i].hp;
		enemy[i].item = data[i].item;

		//弾の初期化
		for (int j = 0; j < ENEMY_SNUM; j++) {
			enemy[i].shot[j].flag = false;
			enemy[i].shot[j].width = 56;
			enemy[i].shot[j].height = 56;
			enemy[i].shot[j].pattern = data[i].shot_pattern;
			enemy[i].shot[j].speed = data[i].speed;
			enemy[i].shot[j].x = enemy[i].enemy_position.x;
			enemy[i].shot[j].y = enemy[i].enemy_position.y;
		}

		enemy[i].count = 0;
		enemy[i].sCount = 0;

		enemy[i].deadFlag = false;
		enemy[i].endFlag = false;
		enemy[i].sFlag = false;
	}
}

void enemyUpdate() {
	enemyMove();
	enemyShot();
	//enemy.count++;
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i].count++;
	}
}

void enemyDraw() {
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (g_FrameCount > enemy[i].in_time) {
			//Draw enemy's bullets
			for (int j = 0; j < ENEMY_SNUM; j++) {
				if (enemy[i].shot[j].flag) {
					Sprite_Draw(g_enemyShot_textureID, enemy[i].shot[j].x, enemy[i].shot[j].y, 0, 262, enemy[i].shot[j].width, enemy[i].shot[j].height);
				}
			}

			//Draw Enemy
			int patternX = enemy[i].count / 10 % 3;
			if (!enemy[i].deadFlag) {
				Sprite_Draw(g_enemy_textureID, enemy[i].enemy_position.x, enemy[i].enemy_position.y, patternX * enemy[i].width, 0, enemy[i].width, enemy[i].height);
			}
		}
	}
}

void enemyMove() {
	for (int i = 0; i < ENEMY_NUM; i++) {
		//出てきてから止まる時間までの間なら下に移動
		if (enemy[i].in_time < g_FrameCount && g_FrameCount < enemy[i].stop_time) {
			enemy[i].enemy_position.y += 2.f;
		}
		//帰還時間を過ぎたら戻る。
		else if (g_FrameCount > enemy[i].out_time) {
			enemy[i].enemy_position.y -= 2.f;
			if (enemy[i].enemy_position.y < -40) {
				enemy[i].endFlag = true;
			}
		}
	}
}

void enemyShot() {
	for (int i = 0; i < ENEMY_NUM; i++) {
		//発射タイミングになったら、フラグを立てる
		if (enemy[i].shot_time == g_FrameCount) {
			enemy[i].sFlag = true;
		}

		if (enemy[i].sFlag) {
			switch (enemy[i].shot_pattern) {
			case 0:
				if (enemy[i].sCount % 20 == 0 && enemy[i].sCount <= 80) {
					for (int j = 0; j < ENEMY_SNUM; j++) {
						if (!enemy[i].shot[j].flag) {
							enemy[i].shot[j].flag = true;
							enemy[i].shot[j].x = enemy[i].enemy_position.x;
							enemy[i].shot[j].y = enemy[i].enemy_position.y;
							break;
						}
					}
				}
				break;
			default: break;
			}

			//How many bullets are left on screen
			int shotLeft = 0;

			//Move the bullets
			for (int j = 0; j < ENEMY_SNUM; j++) {
				if (enemy[i].shot[j].flag) {
					enemy[i].shot[j].y += enemy[i].shot[j].speed;

					//弾の境界判定
					if (enemy[i].shot[j].x < -20 || enemy[i].shot[j].x > 420 || enemy[i].shot[j].y < -20 || enemy[i].shot[j].y > 500) {
						enemy[i].shot[j].flag = false;
						continue;
					}
					shotLeft++;
				}
			}

			//When shotLeft == 0 means there are no bullets left
			//And when deadFlag is true at the same time, destroy the enemy
			if (shotLeft == 0 && enemy[i].deadFlag) {
				enemy[i].endFlag = true;
			}

			enemy[i].sCount++;
		}
	}
}