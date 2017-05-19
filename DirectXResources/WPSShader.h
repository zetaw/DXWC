#pragma once
#include "Macro.h"
class WPSShader{
public:
	WPSShader();
	~WPSShader();
	HRESULT Compile(WCHAR* szFileName, LPCSTR szEntryPoint);
	ID3D11PixelShader* getShader();
	ID3DBlob* getBlob();
private:
	ID3D11PixelShader *m_pixelShader;
	ID3DBlob *m_blob;
};

