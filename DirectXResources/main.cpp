#include "Macro.h"
#include "WPSShader.h"
#include "WVSShader.h"
#include "WBuffer.h"
#include "WTexture.h"
#include "WEffect.h"
#include "WFont.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>

const int drawNum = 30;
const float PI = 3.1415926535;
int s = 0;
const int width = 800, height = 800;
XMMATRIX World;
XMMATRIX View;
XMMATRIX Projection;
XMFLOAT4 meshColor(1,1,1,0);
struct Vertex{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
};
struct CBNeverChanges{
	XMMATRIX mView;
};

struct CBChangeOnResize{
	XMMATRIX mProjection;
};

struct CBChangesEveryFrame{
	XMMATRIX mWorld;
	XMFLOAT4 vMeshColor;
};

WVSShader *gVertexShader;
WPSShader *gPixelShader;
WConstantBuffer<CBNeverChanges> *gCBNeverChanges;
WConstantBuffer<CBChangeOnResize> *gCBChangeOnResize;
WConstantBuffer<CBChangesEveryFrame> *gCBChangesEveryFrame;
WTexture *gTexture;
WTexture *gTexture2;
WBuffer<Vertex> vertexBuffer,vertexBuffer2;
WBuffer<WORD> indexBuffer,indexBuffer2;

HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* pd3dDevices, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext){
	using namespace DX;

	ID3D11DeviceContext *pd3dImmediateContext = DXUTGetD3D11DeviceContext();

	D3D11_VIEWPORT vp = {
		(float)width,(float)height,0,1,0,0
	};
	pd3dImmediateContext->RSSetViewports(1, &vp);
	gVertexShader = new WVSShader();
	gPixelShader = new WPSShader();
	gCBNeverChanges = new WConstantBuffer<CBNeverChanges>();
	gCBChangeOnResize = new WConstantBuffer<CBChangeOnResize>();
	gCBChangesEveryFrame = new WConstantBuffer<CBChangesEveryFrame>();
	gTexture = new WTexture();
	gTexture2 = new WTexture();


	gVertexShader->Compile(L"light.fx", "VS");
	gPixelShader->Compile(L"light.fx", "PS");	
	D3D11_INPUT_ELEMENT_DESC inputElement[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	Vertex vertexData[] = {
		{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},

		{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},

		{XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},

		{XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},

		{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f)},

		{XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)},
	};
	WORD indices[] =
	{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	};
	vertexBuffer2.CreateBuffer(vertexData, 24, D3D11_BIND_VERTEX_BUFFER);
	vertexBuffer.CreateBuffer(vertexData, 24, D3D11_BIND_VERTEX_BUFFER);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer * tempb = vertexBuffer.getBuffer();
	pd3dImmediateContext->IASetVertexBuffers(0, 1, &tempb, &stride, &offset);
	indexBuffer.CreateBuffer(indices, 36, D3D11_BIND_INDEX_BUFFER);
	indexBuffer2.CreateBuffer(indices, 36, D3D11_BIND_INDEX_BUFFER);
	pd3dImmediateContext->IASetIndexBuffer(indexBuffer.getBuffer(), DXGI_FORMAT_R16_UINT, 0);
	gVertexShader->createInputLayout(inputElement, 2);
	pd3dImmediateContext->IASetInputLayout(gVertexShader->getInputLayout());
	pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gCBNeverChanges->CreateBuffer();
	gCBChangeOnResize->CreateBuffer();
	gCBChangesEveryFrame->CreateBuffer();

	gTexture->init(L"pic.jpg");
	gTexture2->init(L"807585.dds");

	World = XMMatrixIdentity();
	XMVECTOR Eye = XMVectorSet(0.0f, -6.0f, 6.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
	View = XMMatrixLookAtLH(Eye, At, Up);

	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = XMMatrixTranspose(View);
	pd3dImmediateContext->UpdateSubresource(gCBNeverChanges->getBuffer(), 0, NULL, &cbNeverChanges, 0, 0);

	Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f);

	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = XMMatrixTranspose(Projection);
	pd3dImmediateContext->UpdateSubresource(gCBChangeOnResize->getBuffer(), 0, NULL, &cbChangesOnResize, 0, 0);

	return S_OK;
}

void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext){
	float ClearColor[4] = {0.0f, 0.125f, 0.3f, 1.0f};
	ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
	pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
	ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
	pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);

	static float t = 0.0f;
	
	static DWORD dwTimeStart = 0;
	DWORD dwTimeCur = GetTickCount();
	if (dwTimeStart == 0)
		dwTimeStart = dwTimeCur;
	t = (dwTimeCur - dwTimeStart) / 1000.0f;
	
	pd3dImmediateContext->IASetInputLayout(gVertexShader->getInputLayout());
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer * tempb = vertexBuffer.getBuffer();
	pd3dImmediateContext->IASetVertexBuffers(0, 1, &tempb, &stride, &offset);

	pd3dImmediateContext->IASetIndexBuffer(indexBuffer.getBuffer(), DXGI_FORMAT_R16_UINT, 0);

	World = XMMatrixRotationY(t)*XMMatrixRotationZ(t);

	// Setup our lighting parameters
	CBChangesEveryFrame cb;
	cb.mWorld = XMMatrixTranspose(World);
	cb.vMeshColor = meshColor;
	pd3dImmediateContext->UpdateSubresource(gCBChangesEveryFrame->getBuffer(), 0, NULL, &cb, 0, 0);

	pd3dImmediateContext->VSSetShader(gVertexShader->getShader(), nullptr, 0);
	ID3D11Buffer* const tempcb[] = {gCBNeverChanges->getBuffer(),gCBChangeOnResize->getBuffer(),gCBChangesEveryFrame->getBuffer()};
	pd3dImmediateContext->VSSetConstantBuffers(0, 3, tempcb);
	pd3dImmediateContext->PSSetShader(gPixelShader->getShader(), nullptr, 0);
	pd3dImmediateContext->PSSetConstantBuffers(2, 1, tempcb + 2);
	ID3D11ShaderResourceView* tempsrv = gTexture->getShaderResourceView();
	ID3D11SamplerState* tempsl=gTexture->getSamplerState();
	pd3dImmediateContext->PSSetShaderResources(0, 1, &tempsrv);
	pd3dImmediateContext->PSSetSamplers(0, 1, &tempsl);
	pd3dImmediateContext->DrawIndexed(36, 0, 0);

	tempb = vertexBuffer2.getBuffer();
	pd3dImmediateContext->IASetVertexBuffers(0, 1, &tempb, &stride, &offset);
	pd3dImmediateContext->IASetIndexBuffer(indexBuffer2.getBuffer(), DXGI_FORMAT_R16_UINT, 0);
	World = XMMatrixRotationY(t)*XMMatrixRotationZ(-t)*XMMatrixTranslation(5*sin(t),0,0)*XMMatrixTranslation(0,3,0);
	cb.mWorld = XMMatrixTranspose(World);
	pd3dImmediateContext->UpdateSubresource(gCBChangesEveryFrame->getBuffer(), 0, NULL, &cb, 0, 0);
	ID3D11ShaderResourceView* tempsrv2 = gTexture2->getShaderResourceView();
	ID3D11SamplerState* tempsl2 = gTexture2->getSamplerState();
	pd3dImmediateContext->PSSetShaderResources(0, 1, &tempsrv2);
	pd3dImmediateContext->PSSetSamplers(0, 1, &tempsl2);
	pd3dImmediateContext->DrawIndexed(36, 0, 0);
	
}


void CALLBACK onRelease(void* pUserContext){
	delete gVertexShader;
	delete gPixelShader;
	delete gCBChangeOnResize;
	delete gCBChangesEveryFrame;
	delete gCBNeverChanges;
	delete gTexture;
	delete gTexture2;
;
}

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow ){
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
	DXUTSetCallbackD3D11DeviceCreated(OnD3D11CreateDevice);
	DXUTSetCallbackD3D11FrameRender(OnD3D11FrameRender);
	DXUTSetCallbackD3D11DeviceDestroyed(onRelease);

	DXUTInit(true, true, NULL);
	DXUTSetCursorSettings(true, true);
	DXUTCreateWindow(L"Applications on DirectX11");

	FT_Library pFTLib = NULL;
	FT_Face pFTFace = NULL;
	FT_Error err = FT_Init_FreeType(&pFTLib);
	if (err){
		pFTLib = 0;
		std::cerr << "There is some error when Init Library" << std::endl;
		return -1;
	}

	DXUTCreateDevice(D3D_FEATURE_LEVEL_11_0, true, width, height);
	DXUTMainLoop(); 

    return DXUTGetExitCode();
}

