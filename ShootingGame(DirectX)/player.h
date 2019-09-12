#ifndef PLAYER_H_
#define PLAYER_H_

#define PLAYER_WIDTH	100
#define PLAYER_HEIGHT	100
#define PLAYER_DEFAULT_SPEED 9.0f

struct SHOT {
	bool flag;				//�e�����˒����ǂ���
	double x;				//x���W
	double y;				//y���W
	int gh;					//�O���t�B�b�N�n���h��
	int width, height;		//�摜�̕��ƍ���
	double radian;			//�i�ފp�x
	int type;				//�e�̎��
};

void playerInit();
void playerUninit();
void playerUpdate();
void playerDraw();
void playerMove();
void playerShot();
void getPlayerPosition(float *x, float *y);
bool getPlayerShotPosition(int index, float *x, float *y);
void setPlayerShotFlag(int index, bool flag);
void setPlayerDamageFlag();
bool getPlayerDamageFlag();
bool getPlayerInvincibleFlag();
int  getPlayerLife();
void setPlayerLife();
bool checkShotOutOfRange(int index);
void setPlayerPower(int p);
int  getPlayerPower();
void showTama();
int  searchNearbyEnemy();
void tamaShotSet(int index);

#endif // !PLAYER_H_
