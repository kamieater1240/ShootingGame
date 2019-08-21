#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "mydirect3d.h"
#include "main.h"

static LPDIRECT3D9 g_D3D = NULL;
LPDIRECT3DDEVICE9 g_pDevice = NULL;
D3DPRESENT_PARAMETERS d3dpp = {};

bool MyDirect3D_Init(HWND hWnd) {

	g_D3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!g_D3D)
	{
		::MessageBox(0, "Direct3DCreate9() - FAILED", 0, 0);
		return false;
	}

	//裏画面のバッファ設定
	/*
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
	d3dpp.Windowed = TRUE;								//Windowed or Full Screen
	d3dpp.EnableAutoDepthStencil = TRUE;				//デプスバッファ、ステンシルバッファ
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;			//デプス16Bit
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT; //Also have _ONE, _IMMEDIATE
	*/

	d3dpp.BackBufferWidth = 640;
	d3dpp.BackBufferHeight = 480;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	/*g_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_D3D == NULL) {
		MessageBox(NULL, "Missing g_D3D, Direct3DCreate9(D3D_SDK_VERSION)", "Missing", MB_OK | MB_ICONINFORMATION);
	}*/

	HRESULT hr = g_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pDevice);
	if (FAILED(hr)) {	//hrが失敗だったら真
		MessageBox(NULL, "hr failure", "Failure", MB_OK | MB_ICONINFORMATION);
		return false;
	}

	return true;
}

void MyDirect3D_Uninit() {

}

LPDIRECT3DDEVICE9 MyDirect3D_GetDevice() {
	return g_pDevice;
}