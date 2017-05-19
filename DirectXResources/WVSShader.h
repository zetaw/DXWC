#pragma once
#include"Macro.h"


class WVSShader{
public:
	WVSShader();
	~WVSShader();
	HRESULT Compile(WCHAR* szFileName, LPCSTR szEntryPoint);
	ID3D11VertexShader* getShader();
	ID3DBlob* getBlob();
	HRESULT createInputLayout(D3D11_INPUT_ELEMENT_DESC *inputElemnt, size_t size);
	ID3D11InputLayout* getInputLayout();
private:
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	ID3D11VertexShader *m_vertexShader;
	ID3DBlob *m_blob;
	ID3D11InputLayout *m_inputLayout;
};

