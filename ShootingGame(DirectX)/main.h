#ifndef MAIN_H_
#define MAIN_H_

#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	900
#define GAME_WIDTH		700
#define GAME_HEIGHT		850
#define PSHOT_NUM		100
#define PSHOT_SPEED		14

//position       color            texture
#define FVF_VERTEX2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FPS_MEASUREMENT_TIME 1.0

typedef struct Vertex2d_tag {
	// x, y, z, w			���W�ϊ��ςݒ��_�̗��p�@=> RHW��1.0f��ݒ�
	D3DXVECTOR4 position;	//���W, include in d3dx9.h
	//Color
	D3DCOLOR color;
	//uv
	D3DXVECTOR2 uv;			//texture coordinate
}Vertex2d;

struct SHOT {
	bool flag;				//�e�����˒����ǂ���
	double x;				//x���W
	double y;				//y���W
	int gh;					//�O���t�B�b�N�n���h��
	int width, height;		//�摜�̕��ƍ���
};


static bool Init(HWND hWnd);
static void Uninit(void);
static void Update();
static void Draw(void);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // !MAIN_H_