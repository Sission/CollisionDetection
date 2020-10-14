#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class ConstantBuffer
{
public:
	ConstantBuffer(void* buffer, UINT size_buffer,RenderSystem * system);
	void update(DeviceContextPtr context, void* buffer);
	ID3D11Buffer* getD3DBuffer();
	~ConstantBuffer();
private:
	ID3D11Buffer * m_buffer;
	RenderSystem * m_system = nullptr;
private:
	friend class DeviceContext;
};