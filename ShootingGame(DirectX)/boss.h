#ifndef BOSS_H_
#define BOSS_H_

#define BOSS_HP 500

void bossInit();
void bossDraw();
void bossMove();
void bossMoveInit(float nextX, float nextY, int state);
void bossMoveToDefault();
void bossAppear();
void bossMovePattern1();
void bossMovePattern2();
void bossMovePattern3();
void setBossMovePattern(int pattern);
void bossShot();
void bossShotPattern1();
void bossShotPattern2();
void bossShotPattern3();
void setBossShotPattern(int pattern);
int  shotSearch();
bool bossShotRangeCheck(int index);

void setBossDamageFlag();
void setBossFlag(bool flag);
bool getBossFlag();
bool getBossInvincibleFlag();

void getBossPosition(float* x, float *y);
bool getBossShotPosition(int index, float *x, float *y, int *type);
void setBossShotFlag(int index, bool flag);

int  setBossHp(int damage);
int  getPrevBossHp();

void setBossDamageSetting();


#endif // !BOSS_H_

