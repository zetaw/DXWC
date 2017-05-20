#pragma once
#include "Macro.h"
#include <tuple>
#include <ft2build.h>
#include "WBuffer.h"
#include "WTexture.h"
#include "WVSShader.h"
#include "WPSShader.h"
#include <unordered_map>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace WFontStruct{
	struct FontVertex{
		XMFLOAT3 pos;
		XMFLOAT4 color;
		XMFLOAT2 tex;
	};
}
typedef std::tuple<float, float, float, float, ID3D11ShaderResourceView*> CharBitmapType;

class WFont{
public:
	WFont();
	~WFont();
	void Init();
	ID3D11ShaderResourceView* getShaderResourceView(wchar_t);
	void Render(std::wstring& text);
	void OnSizeChanged(const D2D1_RECT_F& size);
	void StartPoint(XMFLOAT2& pt);
	void setFontColor(const XMFLOAT4& color);
	void setFontName(const std::string& fontname);
	void setFontSize(UINT fontsize);
	void setFont(const std::string& fontname, const UINT fontsize);
protected:
	void UpdateFont();
	void Free();
private:
	FT_Face mFTFace;
	FT_GlyphSlot mFTSlot;
	UINT mCharWidth, mCharHeight;
	WVSShader *mVertexShader;
	WPSShader *mPixelShader;
	WBuffer<WFontStruct::FontVertex> *mVertexBuffer;
	WBuffer<WORD> *mIndexBuffer;
	WTexture *mCharTexture;
	ID3D11ShaderResourceView* mView;
	float mFontSize;
	bool mIsAntiAliased;

	float mAscender;
	float mDescender;
	float mFontHeight;
	float mAddance;
	float mToplineHeigh;
	std::string mFontName;

	std::unordered_map<wchar_t, CharBitmapType>   mCharTextMap;
	D2D1_RECT_F mWindowRect;    // 保存渲染目标的窗口大小  
	D2D1_RECT_F  mRendArear;     // 渲染区域  
	XMFLOAT2  mPointPen;  // 开始渲染位置  
	WFontStruct::FontVertex mFontVertex[4]; // 字体所处的矩形区域  
	XMFLOAT4 mFontColor;     // 字体颜色  
};

