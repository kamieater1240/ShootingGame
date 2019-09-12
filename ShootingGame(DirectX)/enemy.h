#ifndef ENEMY_H_
#define ENEMY_H_

#define ENEMY_NUM  26
#define ENEMY_SNUM 50

#include "main.h"

struct ENEMY {
	//���W
	D3DXVECTOR2 enemy_position;

	//�摜�T�C�Y
	float width, height;

	//�o���A��~�A�A�ҁA���˃^�C�~���O
	int in_time, stop_time, shot_time, out_time;
	
	//�G�̎��
	int type;
	//�e�̎��
	int shotType;
	//�ړ��p�^�[��
	int move_pattern;
	//�V���b�g�p�^�[��
	int shot_pattern;

	//Health Point
	int hp;
	//�����Ă���A�C�e���^�C�v
	int item;

	//�G���o�����Ă���̃J�E��
	int count;

	//Control how many bullets an enemy shoots once
	int bulletNum;
	//Shooting bullet's radian
	double shootingRadian;

	//�G���Ńt���O
	bool deadFlag;
	//�G�N���X���Ńt���O
	bool endFlag;

	//�e�\����
	E_SHOT shot[ENEMY_SNUM];
	//�V���b�g�����Ă�悤�ɂȂ������̃t���O
	bool sFlag;
	//�V���b�g���łĂ�悤�ɂȂ��Ă���̃J�E���g
	int sCount;
};

struct ENEMYDATA {
	int type;				//�G���
	int shotType;			//�e���
	int move_pattern;		//�ړ��p�^�[��
	int shot_pattern;		//���˃p�^�[��
	int in_time;			//�o������
	int stop_time;			//��~����
	int shot_time;			//�e���ˎ���
	int out_time;			//�A�Ҏ���
	int x;					//x���W
	int y;					//y���W
	int speed;				//�e�X�s�[�h
	int hp;					//HP
	int item;				//�A�C�e��
};

void enemyInit();
void enemyUpdate();
void enemyDraw();
void enemyMove();
void enemyShot();
void readENEMYDATA();
bool checkOutOfRange(ENEMY enemy);
bool checkEnemyShotOutOfRange(ENEMY enemy, int index);
void getEnemyPosition(int index, float *x, float *y);
bool getEnemyPositions(int index, float *x, float *y);
bool getEnemyShotPosition(int enemyIndex, int shotIndex, float *x, float *y);
void setEnemyShotFlag(int enemyIndex, int shotIndex, bool flag);
int  getEnemyShotType(int enemyIndex);
void setEnemyDeadFlag(int index);
bool getEnemyDeadFlag(int index);
int  getEnemyInTime(int index);
int  getEnemyItemType(int index);
void setEnemyHp(int index, int num);
int  getEnemyHP(int index);
int  getEnemyLeft();
void setEnemyLeft(int num);

#endif // !ENEMY_H_


