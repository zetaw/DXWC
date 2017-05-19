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
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	ID3D11PixelShader *m_pixelShader;
	ID3DBlob *m_blob;
};

