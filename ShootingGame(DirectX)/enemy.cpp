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

//ENEMY enemy[ENEMY_NUM];
ENEMY enemy;
ENEMYDATA data[ENEMY_NUM];

void enemyInit() {
	Texture_SetLoadFile("Assets/Textures/enemy.png", 512, 512);
	Texture_SetLoadFile("Assets/Textures/bullet.png", 512, 512);
	Texture_Load();
	g_enemy_textureID = Texture_GetID("Assets/Textures/enemy.png");
	g_enemyShot_textureID = Texture_GetID("Assets/Textures/bullet.png");

	//readENEMYDATA();
	//enemy = new ENEMY();

	enemy.width = 64.f;
	enemy.height = 64.f;
	enemy.type = 0;
	enemy.shotType = 0;
	enemy.move_pattern = 0;
	enemy.shot_pattern = 0;
	enemy.enemy_position.x = 50.f;
	enemy.enemy_position.y = -50.f;

	enemy.in_time = 120;
	enemy.stop_time = 180;
	enemy.shot_time = 181;
	enemy.out_time = 360;

	//弾の初期化
	for (int i = 0; i < ENEMY_SNUM; i++) {
		enemy.shot[i].flag = false;
		enemy.shot[i].width = 56;
		enemy.shot[i].height = 56;
		enemy.shot[i].pattern = 0;
		enemy.shot[i].speed = 4;
		enemy.shot[i].x = enemy.enemy_position.x;
		enemy.shot[i].y = enemy.enemy_position.y;
	}

	enemy.count = 0;
	enemy.sCount = 0;

	enemy.deadFlag = false;
	enemy.endFlag = false;
	enemy.sFlag = false;
}

void readENEMYDATA() {
	FILE *fp;
	char buf[100];
	int c;
	int col = 1;
	int row = 0;

	memset(buf, 0, sizeof(buf));
	fp = fopen("ENEMYDATA.csv", "r");

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

	}
}

void enemyUpdate() {
	enemyMove();
	enemyShot();
	enemy.count++;
}

void enemyDraw() {
	//Draw enemy's bullets
	for (int i = 0; i < ENEMY_SNUM; i++) {
		if (enemy.shot[i].flag) {
			Sprite_Draw(g_enemyShot_textureID, enemy.shot[i].x, enemy.shot[i].y, 0, 262, enemy.shot[i].width, enemy.shot[i].height);
		}
	}

	//Draw Enemy
	int patternX = enemy.count / 10 % 3;
	if (!enemy.deadFlag) {
		Sprite_Draw(g_enemy_textureID, enemy.enemy_position.x, enemy.enemy_position.y, patternX * enemy.width, 0, enemy.width, enemy.height);
	}
}

void enemyMove() {
	//出てきてから止まる時間までの間なら下に移動
	if (enemy.in_time < g_FrameCount && g_FrameCount < enemy.stop_time) {
		enemy.enemy_position.y += 2.f;
	}
	//帰還時間を過ぎたら戻る。
	else if (g_FrameCount > enemy.out_time) {
		enemy.enemy_position.y -= 2.f;
		if (enemy.enemy_position.y < -40) {
			enemy.endFlag = true;
		}
	}
}

void enemyShot() {
	//発射タイミングになったら、フラグを立てる
	if (enemy.shot_time == g_FrameCount) {
		enemy.sFlag = true;
	}

	if (enemy.sFlag) {
		switch (enemy.shot_pattern) {
		case 0:
			if (enemy.sCount % 20 == 0 && enemy.sCount <= 80) {
				for (int i = 0; i < ENEMY_SNUM; i++) {
					if (!enemy.shot[i].flag) {
						enemy.shot[i].flag = true;
						enemy.shot[i].x = enemy.enemy_position.x;
						enemy.shot[i].y = enemy.enemy_position.y;
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
		for (int i = 0; i < ENEMY_SNUM; i++) {
			if (enemy.shot[i].flag) {
				enemy.shot[i].y += enemy.shot[i].speed;

				//弾の境界判定
				if (enemy.shot[i].x < -20 || enemy.shot[i].x > 420 || enemy.shot[i].y < -20 || enemy.shot[i].y > 500) {
					enemy.shot[i].flag = false;
					continue;
				}
				shotLeft++;
			}
		}

		//When shotLeft == 0 means there are no bullets left
		//And when deadFlag is true at the same time, destroy the enemy
		if (shotLeft == 0 && enemy.deadFlag) {
			enemy.endFlag = true;
		}

		enemy.sCount++;
	}
}