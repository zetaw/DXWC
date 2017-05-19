#include "Macro.h"
#include "WEffect.h"

WEffect::WEffect(){
	mEffect = nullptr;
}

void WEffect::Compile(WCHAR * szFileName){
	ID3DBlob* compiledShader = nullptr;
	DX::ThrowIfFailed(DX::CompileShaderFromFile(szFileName, 0, "fx_4_0", &compiledShader));
	DX::ThrowIfFailed(D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, DXUTGetD3D11Device(), &mEffect));
	compiledShader->Release();
}

ID3DX11Effect * const WEffect::getEffect(){
	return mEffect;
}

WEffect::~WEffect(){
	mEffect->Release();
	mEffect = nullptr;
}
