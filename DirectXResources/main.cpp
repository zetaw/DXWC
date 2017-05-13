#include <DXUT.h>

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow ){
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
	DXUTInit(true, true, NULL);
	DXUTSetCursorSettings(true, true);
	DXUTCreateWindow(L"Applications on DirectX11");

	DXUTCreateDevice(D3D_FEATURE_LEVEL_11_0, true, 800, 600);
    DXUTMainLoop(); 

    return DXUTGetExitCode();
}


