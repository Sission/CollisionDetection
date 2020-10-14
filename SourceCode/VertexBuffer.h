#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class VertexBuffer
{
public:
	VertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void*shader_byte_code, size_t size_byte_shader,
		RenderSystem * system);
	UINT getSizeVertexList();
	//void readFromVertexBuffer();
	ID3D11Buffer* getD3DBuffer();

	~VertexBuffer();
private:
	UINT m_size_vertex;
	UINT m_size_list;
private:
	ID3D11Buffer * m_buffer;
	ID3D11Buffer * m_buffer_copy;
	D3D11_BUFFER_DESC buff_desc_copy;

	ID3D11InputLayout* m_layout;
	RenderSystem * m_system = nullptr;
private:
	friend class DeviceContext;
};

