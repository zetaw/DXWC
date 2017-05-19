#pragma once
#include "Macro.h"
class WTexture{
public:
	WTexture();
	~WTexture();
	void init(WCHAR* szFileName);
	ID3D11ShaderResourceView* const getShaderResourceView();
	ID3D11SamplerState* const getSamplerState();
private:
	ID3D11ShaderResourceView* mTexture;
	ID3D11SamplerState* mSamplerLinear;
};

