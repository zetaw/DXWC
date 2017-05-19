#include "Macro.h"
#include "WPSShader.h"

WPSShader::WPSShader(){
	m_blob = nullptr;
	m_pixelShader = nullptr;
}

WPSShader::~WPSShader(){
	m_pixelShader->Release();
	m_pixelShader = nullptr;
	m_blob->Release();
	m_blob = nullptr;
}


HRESULT WPSShader::Compile(WCHAR * szFileName, LPCSTR szEntryPoint){
	ID3D11Device* pd3dDevice = DXUTGetD3D11Device();
	DX::ThrowIfFailed(DX::CompileShaderFromFile(szFileName, szEntryPoint, "ps_4_0", &m_blob));
	DX::ThrowIfFailed(pd3dDevice->CreatePixelShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &m_pixelShader));
	return S_OK;
}

ID3D11PixelShader * WPSShader::getShader(){
	return m_pixelShader;
}

ID3DBlob * WPSShader::getBlob(){
	return m_blob;
}


