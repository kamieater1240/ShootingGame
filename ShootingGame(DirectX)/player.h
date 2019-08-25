#ifndef PLAYER_H_
#define PLAYER_H_

#define PLAYER_WIDTH	100
#define PLAYER_HEIGHT	100
#define PLAYER_DEFAULT_SPEED 5.0f

struct SHOT {
	bool flag;				//弾が発射中かどうか
	double x;				//x座標
	double y;				//y座標
	int gh;					//グラフィックハンドル
	int width, height;		//画像の幅と高さ
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

#endif // !PLAYER_H_
