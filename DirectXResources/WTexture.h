#pragma once
#include "Macro.h"
//This Class is ONLY FOR TEXTURE 2D
class WTexture{
public:
	WTexture();
	~WTexture();
	void init(WCHAR* szFileName);
	void init(UCHAR* buffer,UINT width,UINT height);
	ID3D11ShaderResourceView* const getShaderResourceView();
	ID3D11SamplerState* const getSamplerState();
private:
	ID3D11ShaderResourceView* mTexture;
	ID3D11SamplerState* mSamplerLinear;
};

