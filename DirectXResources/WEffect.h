#pragma once
#include "Macro.h"
class WEffect{
public:
	WEffect();
	void Compile(WCHAR* szFileName);
	ID3DX11Effect* const getEffect();
	~WEffect();
private:
	ID3DX11Effect* mEffect;
};

