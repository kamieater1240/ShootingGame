#ifndef BOSS_H_
#define BOSS_H_

#define BOSS_SHOTNUM 100

void bossInit();
void bossDraw();
void bossMove();
void bossMoveInit(float nextX, float nextY, int state);
void bossAppear();
void bossMovePattern1();
void bossMovePattern2();
void bossMovePattern3();
void bossShot();
void bossShotPattern1();
void bossShotPattern2();
void bossShotPattern3();
int  shotSearch();

void setBossDamageFlag();
void setBossFlag(bool flag);
bool getBossFlag();

#endif // !BOSS_H_

