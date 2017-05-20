#include "Macro.h"
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

void WTexture::init(UCHAR * buffer, UINT width, UINT height){
	ID3D11Device *pd3dDevice = DXUTGetD3D11Device();
	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = buffer;
	subData.SysMemPitch = width;
	subData.SysMemSlicePitch = height;

	ID3D11Texture2D*      Tex2D;
	D3D11_TEXTURE2D_DESC  Tex2Dtdesc;

	Tex2Dtdesc.Width = width;
	Tex2Dtdesc.Height = height;
	Tex2Dtdesc.MipLevels = 1;
	Tex2Dtdesc.ArraySize = 1;

	Tex2Dtdesc.SampleDesc.Count = 1;
	Tex2Dtdesc.SampleDesc.Quality = 0;
	Tex2Dtdesc.Usage = D3D11_USAGE_DEFAULT;
	Tex2Dtdesc.Format = DXGI_FORMAT_A8_UNORM;
	Tex2Dtdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	Tex2Dtdesc.CPUAccessFlags = 0;
	Tex2Dtdesc.MiscFlags = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = Tex2Dtdesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = Tex2Dtdesc.MipLevels;
	viewDesc.Texture2D.MostDetailedMip = 0;

	DX::ThrowIfFailed(pd3dDevice->CreateTexture2D(&Tex2Dtdesc, &subData, &Tex2D));
	DX::ThrowIfFailed(pd3dDevice->CreateShaderResourceView(Tex2D, &viewDesc, &mTexture));
	SAFE_RELEASE(Tex2D);
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
