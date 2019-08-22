#ifndef PLAYER_H_
#define PLAYER_H_

#define PLAYER_WIDTH	100
#define PLAYER_HEIGHT	100
#define PLAYER_DEFAULT_SPEED 5.0f

void playerInit();
void playerUninit();
void playerUpdate();
void playerDraw();
void playerMove();
void playerShot();

#endif // !PLAYER_H_
