#pragma once
#include "Macro.h"
template <class BufferData>
class WBuffer{
public:
	WBuffer(){
		m_buffer = nullptr;
	}
	virtual ~WBuffer(){
		m_buffer->Release();
		m_buffer = nullptr;
	}
	void CreateBuffer(BufferData data[], size_t size, UINT BindFlags){
		ID3D11Device* pd3dDevice = DXUTGetD3D11Device();
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = BindFlags;
		desc.ByteWidth = sizeof(BufferData) * size;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA dataPointer;
		ZeroMemory(&dataPointer, sizeof(dataPointer));
		dataPointer.pSysMem = data;
		DX::ThrowIfFailed(pd3dDevice->CreateBuffer(&desc, &dataPointer, &m_buffer));
	}
	ID3D11Buffer* const getBuffer(){
		return m_buffer;
	}
protected:
	ID3D11Buffer* m_buffer;
};

template <class BufferStruct>
class WConstantBuffer :public WBuffer<BufferStruct>{
public:
	WConstantBuffer() :WBuffer<BufferStruct>(){}
	~WConstantBuffer(){}
	void CreateBuffer(){
		ID3D11Device* pd3dDevice = DXUTGetD3D11Device();
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(BufferStruct);
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;

		DX::ThrowIfFailed(pd3dDevice->CreateBuffer(&desc, nullptr, &m_buffer));
	}
};