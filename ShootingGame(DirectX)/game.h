#ifndef GAME_H_
#define GAME_H_

extern int g_FrameCount;

void gameInit();
void gameUninit();
void gameUpdate();
void gameDraw();
void setGameDifficulty(int difficulty);
int  getGameDifficulty();

#endif // !GAME_H_
