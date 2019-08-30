#ifndef SCOREBOARD_H_
#define SCOREBOARD_H_

enum SCOREDATATYPE{
	HIGH_SCORE,
	CURRENT_SCORE,
	LIFE_SCORE,
	POWER_SCORE
};

void scoreBoardInit();
void scoreBoardDraw();
void setScoreData(SCOREDATATYPE type, int val);
int  getScoreData(SCOREDATATYPE type);

#endif // !SCOREBOARD_H_

