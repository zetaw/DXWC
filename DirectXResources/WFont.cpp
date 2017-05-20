#include "Macro.h"
#include "WFont.h"
#define INTER_GLYPH_PAD_SPACE 2  
#define INTER_FONT_STEP 4  
#define SPACE_STEP 10.0  

#define FT_POS_COEF  (1.0/64.0)  

static FT_Library ft_lib;
static int Ft_Usage_Count = 0;

WFont::WFont(){
	mFTFace = 0;
	mFTSlot = 0;
	mVertexBuffer = new WBuffer<WFontStruct::FontVertex>();
	mIndexBuffer = new WBuffer<WORD>();
	mVertexShader = new WVSShader();
	mPixelShader = new WPSShader();
	mCharTexture = new WTexture();
	mView = nullptr;

	if (!Ft_Usage_Count++)
		if (FT_Init_FreeType(&ft_lib)){
			std::cerr << "There is some error when Init Library" << std::endl;
			return;
		}
	UpdateFont();
	mWindowRect = {0,(float)DXUTGetWindowHeight(),(float)DXUTGetWindowWidth(),0};
	mFontColor = XMFLOAT4(0, 0, 0, 1);
	mFontVertex[0].tex = XMFLOAT2(0.f, 0.f);
	mFontVertex[1].tex = XMFLOAT2(1.f, 0.f);
	mFontVertex[2].tex = XMFLOAT2(0.f, 1.f);
	mFontVertex[3].tex = XMFLOAT2(1.f, 1.f);

	mPointPen = XMFLOAT2(0, 40);
}

WFont::~WFont(){
	delete mVertexBuffer;
	delete mIndexBuffer;
	delete mVertexShader;
	delete mPixelShader;
}

void WFont::Init(){
	ID3D11Device *pd3dDevice = DXUTGetD3D11Device();
	ID3D11DeviceContext *pd3dImmediateContext = DXUTGetD3D11DeviceContext();

	mVertexShader->Compile(L"Font.fx", "VS");
	mPixelShader->Compile(L"Font.fx", "PS");
	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	mVertexShader->createInputLayout(solidColorLayout, 3);
	WFontStruct::FontVertex points[] = {
		{XMFLOAT3(-0.5f, 0.5f, 0), XMFLOAT4(1.f, 0.f, 0.f, 1.f), XMFLOAT2(0, 0)},
		{XMFLOAT3(0.5f, 0.5f, 0), XMFLOAT4(1.f, 0.f, 0.f, 1.f), XMFLOAT2(1, 0)},
		{XMFLOAT3(-0.5f, -0.5f, 0), XMFLOAT4(1.f, 0.f, 0.f, 1.f), XMFLOAT2(0, 1)},
		{XMFLOAT3(0.5f, -0.5f, 0), XMFLOAT4(1.f, 0.f, 0.f, 1.f), XMFLOAT2(1, 1)},
	};
	mVertexBuffer->CreateBuffer(points, 4, D3D11_BIND_VERTEX_BUFFER,true);
	WORD indices[] = {0,1,2,2,1,3};
	mIndexBuffer->CreateBuffer(indices, 36, D3D11_BIND_INDEX_BUFFER);
}

ID3D11ShaderResourceView* WFont::getShaderResourceView(wchar_t ch){
	if (FT_Load_Char(mFTFace, ch, FT_LOAD_RENDER)) return nullptr;
	mFTSlot = mFTFace->glyph;
	FT_Bitmap& bitmap = mFTSlot->bitmap;
	mCharTexture->init(bitmap.buffer, bitmap.width, bitmap.rows);
	return mCharTexture->getShaderResourceView();
}

void WFont::Render(std::wstring & text){
	ID3D11Device *pd3dDevice = DXUTGetD3D11Device();
	UINT stride = sizeof(WFontStruct::FontVertex);
	UINT offset = 0;
	XMFLOAT2 oldPoint = mPointPen;
	ID3D11DeviceContext *pd3dImmediateContext = DXUTGetD3D11DeviceContext();

	pd3dImmediateContext->IASetInputLayout(mVertexShader->getInputLayout());
	pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	ID3D11Buffer *tempvb=mVertexBuffer->getBuffer();
	pd3dImmediateContext->IASetVertexBuffers(0, 1, &tempvb,&stride,&offset);
	pd3dImmediateContext->IASetIndexBuffer(mIndexBuffer->getBuffer(), DXGI_FORMAT_R32_UINT, 0);
	for (int i = 0; i < 4; i++) mFontVertex[i].color = mFontColor;
	int l = text.length();
	ID3D11SamplerState* mSamplerLinear;
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
	pd3dImmediateContext->PSSetSamplers(0, 1, &mSamplerLinear);
	pd3dImmediateContext->VSSetShader(mVertexShader->getShader(), nullptr, 0);
	pd3dImmediateContext->PSSetShader(mPixelShader->getShader(), nullptr, 0);
	float ClearColor[4] = {0.0f, 0.125f, 0.3f, 1.0f};
	ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
	pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
	ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
	pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);
	for (int i = 0; i < l; i++){

		if (text[i] == L' '){
			float step = SPACE_STEP;
			mPointPen.x=mPointPen.x + step;
			continue;
		}
		if (text[i] == L'\n'){
			mPointPen.x=oldPoint.x;
			mPointPen.y=mPointPen.y + mFontHeight;
			continue;
		}
		if (mCharTextMap.count(text[i])){
			CharBitmapType& chartype = mCharTextMap.at(text[i]);
			mCharWidth = std::get<0>(chartype);
			mCharHeight = std::get<1>(chartype);
			mToplineHeigh = std::get<2>(chartype);
			mAddance = std::get<3>(chartype);
			mView = std::get<4>(chartype);
		}
		else{
			auto it = getShaderResourceView(text[i]);
			if (it){
				mView = it;
				CharBitmapType chartype(mCharWidth, mCharHeight, mToplineHeigh, mAddance, it);
				mCharTextMap[text[i]] = chartype;
			}
		}
		if (mPointPen.x + mCharWidth >= mWindowRect.right - 10){
			mPointPen.x=oldPoint.x;
			mPointPen.y=mPointPen.y + mFontHeight;
		}
		
		mRendArear = {mPointPen.x/(mWindowRect.right-mWindowRect.left),
			(mPointPen.y - mAscender + mToplineHeigh+mCharHeight)/(mWindowRect.top - mWindowRect.bottom),
			(mPointPen.x+mCharWidth) / (mWindowRect.right - mWindowRect.left),
			(mPointPen.y - mAscender + mToplineHeigh) / (mWindowRect.top - mWindowRect.bottom)};
		mFontVertex[0].pos = XMFLOAT3(mRendArear.left, mRendArear.top, 1.0);
		mFontVertex[1].pos = XMFLOAT3(mRendArear.right, mRendArear.top, 1.0);
		mFontVertex[2].pos = XMFLOAT3(mRendArear.left, mRendArear.bottom, 1.0);
		mFontVertex[3].pos = XMFLOAT3(mRendArear.right, mRendArear.bottom, 1.0);
		mPointPen.x=mPointPen.x + mAddance;

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		WFontStruct::FontVertex *data;
		DX::ThrowIfFailed(pd3dImmediateContext->Map(mVertexBuffer->getBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		data = (WFontStruct::FontVertex*)mappedResource.pData;
		for (int j = 0; j < 4; j++) data[j].pos = mFontVertex->pos;
		pd3dImmediateContext->Unmap(mVertexBuffer->getBuffer(), 0);

		pd3dImmediateContext->PSSetShaderResources(0, 1, &mView);
		pd3dImmediateContext->OMSetDepthStencilState(0, 0);
		//pd3dImmediateContext->OMSetBlendState(0,0,0)
		pd3dImmediateContext->DrawIndexed(6, 0, 0);
		
	}
	mPointPen = oldPoint;
}

void WFont::OnSizeChanged(const D2D1_RECT_F & size){ mWindowRect = size; }

void WFont::StartPoint(XMFLOAT2 & pt){ mPointPen = pt; }

void WFont::setFontColor(const XMFLOAT4 & color){ mFontColor = color; }

void WFont::setFontName(const std::string & fontname){
	mFontName = fontname;
	UpdateFont();
}

void WFont::setFontSize(UINT fontsize){
	mFontSize = fontsize;
	UpdateFont();
}

void WFont::setFont(const std::string & fontname, const UINT fontsize){
	mFontName = fontname;
	mFontSize = fontsize;
	UpdateFont();
}

void WFont::UpdateFont(){
	Free();
	if (mFontName.find('.') == -1) mFontName += std::string(".ttf");
	mFontName = std::string("C:\\Windows\\Fonts\\") + mFontName;
	WIN32_FIND_DATAA FindResult;
	if (FindFirstFileA(mFontName.c_str(), &FindResult)==INVALID_HANDLE_VALUE) return;
	FT_Error error;
	if ((error = FT_New_Face(ft_lib,mFontName.c_str(),	0, &mFTFace)) != 0){
		std::string par="ttf Error!";
		throw std::runtime_error(par);
	}

	FT_Select_Charmap(mFTFace, FT_ENCODING_UNICODE);
	FT_Set_Pixel_Sizes(mFTFace, mFontSize, mFontSize);
	if (!mFTFace->charmap){
		FT_Done_Face(mFTFace);
		mFTFace = nullptr;
		throw std::runtime_error("ttf error!");
	}

	unsigned horzdpi = 96;
	unsigned vertdpi = 96;

	float hps = mFontSize * 64;
	float vps = mFontSize * 64;

	if (FT_Set_Char_Size(mFTFace, FT_F26Dot6(hps), FT_F26Dot6(vps), horzdpi, vertdpi)){
		float ptSize_72 = (mFontSize * 72.0f) / vertdpi;
		float best_delta = 99999;
		float best_size = 0;
		for (int i = 0; i < mFTFace->num_fixed_sizes; i++){
			float size = mFTFace->available_sizes[i].size * float(FT_POS_COEF);
			float delta = fabs(size - ptSize_72);
			if (delta < best_delta){
				best_delta = delta;
				best_size = size;
			}
		}

		if ((best_size <= 0) ||	FT_Set_Char_Size(mFTFace, 0, FT_F26Dot6(best_size * 64), 0, 0)){
			throw std::runtime_error("The font cannot be rasterised at a size of points, and cannot be used.");
		}
	}

	if (mFTFace->face_flags & FT_FACE_FLAG_SCALABLE){
		float y_scale = mFTFace->size->metrics.y_scale * float(FT_POS_COEF) * (1.0f / 65536.0f);
		mAscender = mFTFace->ascender * y_scale;
		mDescender = mFTFace->descender * y_scale;
		mFontHeight = mFTFace->height * y_scale;
	}
	else{
		mAscender = mFTFace->size->metrics.ascender * float(FT_POS_COEF);
		mDescender = mFTFace->size->metrics.descender * float(FT_POS_COEF);
		mFontHeight = mFTFace->size->metrics.height * float(FT_POS_COEF);
	}
}

void WFont::Free(){
	if (!mFTFace) return;
	FT_Done_Face(mFTFace);
	mFTFace = nullptr;
	for (auto & m : mCharTextMap){
		auto it = std::get<4>(m.second);
		SAFE_RELEASE(it);
	}
	mCharTextMap.clear();
}
