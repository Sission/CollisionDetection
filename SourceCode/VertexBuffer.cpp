#include "VertexBuffer.h"
#include "RenderSystem.h"
#include <exception>

VertexBuffer::VertexBuffer(void* list_vertices,UINT size_vertex,UINT size_list,
	void*shader_byte_code,size_t size_byte_shader,
	RenderSystem * system) : m_system(system) ,m_layout(0),m_buffer(0)
{
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_vertex * size_list;
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_vertices;

	m_size_vertex = size_vertex;
	m_size_list = size_list;

	if (FAILED(m_system->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
	{
		throw std::exception("VertexBuffer not created successfully");
	}
	
	D3D11_INPUT_ELEMENT_DESC layout[]=
	{
		//SEMANTIC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALIGNED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
		{"POSITION", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D11_INPUT_PER_VERTEX_DATA ,0},
		{"TEXCOORD", 0,  DXGI_FORMAT_R32G32_FLOAT, 0, 12,D3D11_INPUT_PER_VERTEX_DATA ,0 },
	    {"NORMAL", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 20,D3D11_INPUT_PER_VERTEX_DATA ,0 },
	};
	
	UINT size_layout = ARRAYSIZE(layout);

	if (FAILED(m_system->m_d3d_device->CreateInputLayout(layout, size_layout, shader_byte_code, size_byte_shader, &m_layout)))
	{
		throw std::exception("InputLayout not created successfully");
	}

}


UINT VertexBuffer::getSizeVertexList()
{
	return this->m_size_list;
}

ID3D11Buffer* VertexBuffer::getD3DBuffer()
{
	return m_buffer;
}

/*
void VertexBuffer::readFromVertexBuffer()
{
	m_buffer_copy = m_buffer;
	m_buffer_copy->GetDesc(&buff_desc_copy);
	buff_desc_copy.Usage = D3D11_USAGE_STAGING;
	buff_desc_copy.BindFlags = 0;
	buff_desc_copy.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	
	ID3D11Buffer * temp_buffer;
	if (FAILED(m_system->m_d3d_device->CreateBuffer(&buff_desc_copy, nullptr, &temp_buffer)))
	{
		throw std::exception("VertexBuffer not created successfully");
	}

	DeviceContextPtr temp_context;
	temp_context = m_system->getImmediateDeviceContext();
	
	ID3D11DeviceContext* context_ptr;
	context_ptr = temp_context->getD3DDeviceContext();
}
*/

VertexBuffer::~VertexBuffer()
{
	m_layout->Release();
	m_buffer->Release();
}
