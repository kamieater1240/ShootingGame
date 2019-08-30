#ifndef MAIN_H_
#define MAIN_H_

#define SCREEN_WIDTH			1280
#define SCREEN_HEIGHT			900
#define GAME_WIDTH				700
#define GAME_HEIGHT				850

#define PLAYER_INITX			400
#define PLAYER_INITY			800

#define PSHOT_NUM				100
#define PSHOT_SPEED				14

#define TAMA_MAXNUM				4
#define TAMA_INITX				50
#define TAMA_INITY				30
#define TAMA_SHAKE				15

#define ITEM_NUM				100

#define PLAYER_COLLISIONRAD		10
#define ENEMY_COLLISIONRAD		32
#define ITEM_COLLISIONRAD		16

#define PSHOT_COLLISIONRAD		10
#define ESHOT0_COLLISIONRAD		25
#define ESHOT1_COLLISIONRAD		16
#define ESHOT2_COLLISIONRAD		25

//position       color            texture
#define FVF_VERTEX2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FPS_MEASUREMENT_TIME 1.0

typedef struct Vertex2d_tag {
	// x, y, z, w			座標変換済み頂点の利用　=> RHWに1.0fを設定
	D3DXVECTOR4 position;	//座標, include in d3dx9.h
	//Color
	D3DCOLOR color;
	//uv
	D3DXVECTOR2 uv;			//texture coordinate
}Vertex2d;

static bool Init(HWND hWnd);
static void Uninit(void);
static void Update();
static void Draw(void);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // !MAIN_H_