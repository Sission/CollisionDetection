#pragma once
#include <locale>
#include <codecvt>
#include <vector>

#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexMesh.h"

class Mesh: public Resource
{
public:
	Mesh(const wchar_t* full_path);
	~Mesh();
	const VertexBufferPtr& getVertexBuffer();
	const IndexBufferPtr& getIndexBuffer();
	std::vector<VertexMesh> *getVertexMem();
	std::vector<unsigned int> *getIndexMem();
	void readFromVertexBuffer();
	
private:
	VertexBufferPtr m_vertex_buffer;
	IndexBufferPtr m_index_buffer;

	std::vector<VertexMesh> list_vertices_mem;
	std::vector<unsigned int> list_indices_mem;

private:
	friend class DeviceContext;
};

