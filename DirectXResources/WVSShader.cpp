#include "DXUT.h"
#include "WVSShader.h"


WVSShader::WVSShader(){
	m_vertexShader = nullptr;
	m_blob = nullptr;
}


WVSShader::~WVSShader(){
	m_vertexShader->Release();
	m_vertexShader = nullptr;
	m_blob->Release();
	m_blob = nullptr;
	m_inputLayout->Release();
	m_inputLayout = nullptr;
}

HRESULT WVSShader::Compile(WCHAR * szFileName, LPCSTR szEntryPoint){
	ID3D11Device* pd3dDevice = DXUTGetD3D11Device();
	DX::ThrowIfFailed(CompileShaderFromFile(szFileName, szEntryPoint, "vs_4_0", &m_blob));
	DX::ThrowIfFailed(pd3dDevice->CreateVertexShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &m_vertexShader));
	return S_OK;
}

ID3D11VertexShader * WVSShader::getShader(){
	return m_vertexShader;
}

ID3DBlob * WVSShader::getBlob(){
	return m_blob;
}

HRESULT WVSShader::createInputLayout(D3D11_INPUT_ELEMENT_DESC * inputElemnt, size_t size){
	ID3D11Device* pd3dDevice = DXUTGetD3D11Device();
	ID3D11DeviceContext * pd3dImmediateContext = DXUTGetD3D11DeviceContext();
	DX::ThrowIfFailed(pd3dDevice->CreateInputLayout(inputElemnt, size, m_blob->GetBufferPointer(), m_blob->GetBufferSize(), &m_inputLayout));
	return S_OK;
}

ID3D11InputLayout * WVSShader::getInputLayout(){
	return m_inputLayout;
}

HRESULT WVSShader::CompileShaderFromFile(WCHAR * szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob ** ppBlobOut){
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
