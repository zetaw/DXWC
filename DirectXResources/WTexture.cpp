#include "WTexture.h"


WTexture::WTexture(){
	mTexture = nullptr;
	mSamplerLinear = nullptr;
}


WTexture::~WTexture(){
	mTexture->Release();
	mTexture = nullptr;
	mSamplerLinear->Release();
	mSamplerLinear = nullptr;
}

void WTexture::init(WCHAR * szFileName){
	ID3D11Device *pd3dDevice = DXUTGetD3D11Device();
	DX::ThrowIfFailed(D3DX11CreateShaderResourceViewFromFileW(pd3dDevice, szFileName, nullptr, nullptr, &mTexture, nullptr));
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	DX::ThrowIfFailed(pd3dDevice->CreateSamplerState(&sampDesc, &mSamplerLinear));
}

ID3D11ShaderResourceView * const WTexture::getShaderResourceView(){
	return mTexture;
}

ID3D11SamplerState * const WTexture::getSamplerState(){
	return mSamplerLinear;
}
