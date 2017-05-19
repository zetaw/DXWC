#include "DXUT.h"
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
	DX::ThrowIfFailed(CompileShaderFromFile(szFileName, szEntryPoint, "ps_4_0", &m_blob));
	DX::ThrowIfFailed(pd3dDevice->CreatePixelShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &m_pixelShader));
	return S_OK;
}

ID3D11PixelShader * WPSShader::getShader(){
	return m_pixelShader;
}

ID3DBlob * WPSShader::getBlob(){
	return m_blob;
}

HRESULT WPSShader::CompileShaderFromFile(WCHAR * szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob ** ppBlobOut){
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr)){
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

