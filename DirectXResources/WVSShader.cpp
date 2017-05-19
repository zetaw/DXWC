#include "Macro.h"
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
	DX::ThrowIfFailed(DX::CompileShaderFromFile(szFileName, szEntryPoint, "vs_4_0", &m_blob));
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
