#ifndef GAME_H_
#define GAME_H_

extern int g_FrameCount;

enum GAME_DIFFICULTY {
	DIFFICULTY_EASY,
	DIFFICULTY_HARD
};

void gameInit();
void gameUninit();
void gameUpdate();
void gameDraw();
void setGameDifficulty(GAME_DIFFICULTY difficulty);
GAME_DIFFICULTY getGameDifficulty();

#endif // !GAME_H_
