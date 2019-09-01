#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "collision.h"
#include "enemy.h"
#include "player.h"
#include "item.h"
#include "scoreboard.h"
#include "game.h"
#include "main.h"
#include "mydirect3d.h"

bool circleCollsion(const Circle* pA, const Circle* pB) {
	return D3DXVec2LengthSq(&(pB->position - pA->position)) < (pA->radius + pB->radius)*(pA->radius + pB->radius);
}

void checkCollisionAll() {
	float pPositionX, pPositionY;
	float ePositionX, ePositionY;
	float pBulletPositionX, pBulletPositionY;
	float eBulletPositionX, eBulletPositionY;

	//Check the collision between player's bullets and the enemies
	for (int i = 0; i < PSHOT_NUM; i++) {
		if (getPlayerShotPosition(i, &pBulletPositionX, &pBulletPositionY)) {
			for (int j = 0; j < ENEMY_NUM; j++) {
				if (g_FrameCount > getEnemyInTime(j) && !getEnemyDeadFlag(j)) {
					getEnemyPosition(j, &ePositionX, &ePositionY);
					Circle pBullet, enemyObj;
					pBullet.position.x = pBulletPositionX;
					pBullet.position.y = pBulletPositionY;
					pBullet.radius = PSHOT_COLLISIONRAD;
					enemyObj.position.x = ePositionX;
					enemyObj.position.y = ePositionY;
					enemyObj.radius = ENEMY_COLLISIONRAD;

					if (circleCollsion(&pBullet, &enemyObj)) {
						//if enemy was hit, set deadFlag to true
						setEnemyDeadFlag(j);
						//set the bullet that hit the enemy to false
						setPlayerShotFlag(i, false);
						//play enemy destroy sound effect

						//get points
						setScoreData(CURRENT_SCORE, 100);
						//item appears
						for (int k = 0; k < ITEM_NUM; k++) {
							if (!getItemFlag(k)) {
								setItemFlag(k, ePositionX, ePositionY, getEnemyItemType(j));
								break;
							}
						}
					}
				}
			}
		}
	}

	//player's collision between enemy's bulltes or the items
	if (!getPlayerDamageFlag()) {
		getPlayerPosition(&pPositionX, &pPositionY);

		//Check the collision between enemies' bullets and the player
		for (int i = 0; i < ENEMY_NUM; i++) {
			for (int j = 0; j < ENEMY_SNUM; j++) {
				//Check if the bullet is used, if it is used then get its position
				if (getEnemyShotPosition(i, j, &eBulletPositionX, &eBulletPositionY)) {
					Circle eBullet, playerObj;
					eBullet.position.x = eBulletPositionX;
					eBullet.position.y = eBulletPositionY;
					switch (getEnemyShotType(i)) {
					case 0:
						eBullet.radius = ESHOT0_COLLISIONRAD;
						break;
					case 1:
						eBullet.radius = ESHOT1_COLLISIONRAD;
						break;
					case 2:
						eBullet.radius = ESHOT2_COLLISIONRAD;
						break;
					default: break;
					}
					playerObj.position.x = pPositionX;
					playerObj.position.y = pPositionY;
					playerObj.radius = PLAYER_COLLISIONRAD;

					if (circleCollsion(&eBullet, &playerObj)) {
						//if the player was hit, set player's damage flag to true
						setPlayerDamageFlag();
						//set the bullet that hit the player to false
						setEnemyShotFlag(i, j, false);
						//play player destroy sound effect
					}
				}
			}
		}

		//Check the collision between player and the items
		float itemX, itemY;
		for (int i = 0; i < ITEM_NUM; i++) {
			if (getItemFlag(i)) {
				getItemPosition(i, &itemX, &itemY);
				Circle playerObj, itemObj;
				playerObj.position.x = pPositionX;
				playerObj.position.y = pPositionY;
				playerObj.radius = PLAYER_COLLISIONRAD + 10;
				itemObj.position.x = itemX;
				itemObj.position.y = itemY;
				itemObj.radius = ITEM_COLLISIONRAD;

				if (circleCollsion(&playerObj, &itemObj)) {
					switch (getItemType(i)) {
					case 0:			//Get power
						upgradePlayerPower(1);
						break;
					case 1:			//Get point
						setScoreData(CURRENT_SCORE, 256);
						break;
					case 2:			//Get 1up
						setPlayerLife();
						break;
					default: break;
					}

					DestroyItem(i);
				}
			}
		}
	}

	//get and set player's life and power
	setScoreData(LIFE_SCORE, getPlayerLife());
	setScoreData(POWER_SCORE, getPlayerPower());
}