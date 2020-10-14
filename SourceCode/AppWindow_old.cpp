/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

Copyright (c) 2019-2020, PardCode

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include <d3d11.h>

#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"
#include "Prerequisites.h"


struct vertex
{
	Vector3D position;
	Vector2D texcoord;
};


__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4D m_light_direction;
	Vector4D m_camera_position;
	float bc;
};


AppWindow::AppWindow()
{
}

void AppWindow::update()
{
	updateCamera();
	updateModel();
	updateBunnyBox();
	getRealtimeVertexList();
	for (int i = 0; i < cubenum; i++)
	{
		updateCubeBox(vertices_list[i * 90].m_x, vertices_list[i * 90].m_y, vertices_list[i * 90].m_z, i);
	}
	//getRealtimeVertexList();

	//DrawText(, "sample text" ,1, this->getClientWindowRect()->);
	//LPRECT prc = &this->getClientWindowRect();
	//HDC dc = this->getClientWindowHdc();

	//std::string temp1 = "\n1: object1, 2:object2, 3:camera, 4:lock";
	//std::string temp2 = "\nW:in, S:out, A:left, T:right";
	//std::string temp3 = "\nQ:up, Z:down";
	//std::string temp4 = "\nI:rotX, K:-rotX, J:-rotY, L:rotY";

	//std::string temp0 = "Frame count:" + std::to_string(frame_count++);
	//std::string temp = temp0 + "\nTime/frame:" + std::to_string(m_delta_time) + temp1 + temp2 + temp3 + temp4;
	//LPCSTR content = temp.c_str();

	//DrawText(dc,content,160, prc, DT_RIGHT);

}

void AppWindow::updateModel()
{
	constant cc;

	Matrix4x4 m_light_rot_matrix, temp;
	m_light_rot_matrix.setIdentity();
	//m_light_rot_matrix.setRotationY(m_light_rot_y);
	m_light_rot_y += 0.707f * m_delta_time;

	cc.m_world.setIdentity();
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_world.setScale(Vector3D(1.0f, 1.0f, 1.0f));

	if (m_pick_state == 1)
	{
		m_last_translation = m_last_translation + Vector3D(m_rightward * 0.05f, m_upward * 0.05f, m_forward * 0.05f);
		cc.bc = 1.0;
	}
	else
		cc.bc = 0.0;

	temp.setIdentity();
	temp.setRotationX(m_rot_mesh_x);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_mesh_y);
	cc.m_world *= temp;
	
	cc.m_world.setTranslation(m_last_translation);//Vector3D(5.0f, 5.0f, 5.0f)

	m_mesh_last_T.setMatrix(cc.m_world);

	cc.m_camera_position = m_world_cam.getTranslation();
	cc.m_light_direction = -m_light_rot_matrix.getZDirection();


	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);

	//constant cc;

	//Matrix4x4 m_light_rot_matrix;
	//m_light_rot_matrix.setIdentity();
	//m_light_rot_matrix.setRotationY(m_light_rot_y);
	//m_light_rot_y += 0.707f * m_delta_time;

	//cc.m_world.setIdentity();
	//
	//cc.m_view = m_view_cam;
	//cc.m_proj = m_proj_cam;
	//cc.m_camera_position = m_world_cam.getTranslation();
	//cc.m_light_direction = m_light_rot_matrix.getZDirection();
	//m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::updateCamera()
{
	
	if (m_pick_state == 3) { // if camera selected, update movement
		Matrix4x4 world_cam, temp;
		world_cam.setIdentity();

		//temp.setIdentity();
		//temp.setRotationX(m_rot_x);
		//world_cam *= temp;

		//temp.setIdentity();
		//temp.setRotationY(m_rot_y);
		//world_cam *= temp;


		Vector3D new_pos = m_world_cam.getTranslation();

	//rotation
		temp.setIdentity();
		temp.setRotationX(m_rot_x);
		world_cam *= temp;

		temp.setIdentity();
		temp.setRotationY(m_rot_y);
		world_cam *= temp;

		//translation
		new_pos = new_pos + world_cam.getZDirection() * (m_forward * 0.05f);
		new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 0.05f);
		new_pos = new_pos + world_cam.getYDirection() * (m_upward * 0.05f);

		world_cam.setTranslation(new_pos);

		m_world_cam = world_cam;

		world_cam.inverse();


		m_view_cam = world_cam;

		int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
		int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);


		m_proj_cam.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);
	}


	
}

void AppWindow::updateBunnyBox()
{
	//constant cc;

	//cc.m_world.setIdentity();
	//cc.m_world.setScale(Vector3D(10.0f, 10.0f, 10.0f));
	//cc.m_world.setTranslation(Vector3D(1.0f, 0.0f, 1.0f));
	//cc.m_view = m_view_cam;
	//cc.m_proj = m_proj_cam;
	////cc.m_camera_position = m_world_cam.getTranslation();
	////cc.m_light_direction = m_light_rot_matrix.getZDirection();


	//m_bunny_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);

	constant cc;

	Matrix4x4 m_light_rot_matrix, temp;
	m_light_rot_matrix.setIdentity();
	//m_light_rot_matrix.setRotationY(m_light_rot_y);
	m_light_rot_y += 0.707f * m_delta_time;

	cc.m_world.setIdentity();
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_world.setScale(Vector3D(1.0f, 1.0f, 1.0f));
//	cc.m_world.setTranslation(Vector3D(1.0f, 0.0f, 1.0f));

	if (m_pick_state == 2)
	{
		m_last_translation2 = m_last_translation2 + Vector3D(m_rightward * 0.05f, m_upward * 0.05f, m_forward * 0.05f);
		cc.bc = 1.0;
	}
	else
		cc.bc = 0.0;

	temp.setIdentity();
	temp.setRotationX(m_rot_mesh2_x);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_mesh2_y);
	cc.m_world *= temp;

	cc.m_world.setTranslation(m_last_translation2);

	cc.m_camera_position = m_world_cam.getTranslation();
	cc.m_light_direction = -m_light_rot_matrix.getZDirection();


	m_bunny_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::updateCubeBox(float x, float y, float z, int i)
{

	constant cc;

	Matrix4x4 m_light_rot_matrix, temp;
	m_light_rot_matrix.setIdentity();
	//m_light_rot_matrix.setRotationY(m_light_rot_y);
	m_light_rot_y += 0.707f * m_delta_time;

	cc.m_world.setIdentity();
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;

	//temp.setIdentity();
	//temp.setRotationX(m_rot_mesh2_x);
	//cc.m_world *= temp;

	//temp.setIdentity();
	//temp.setRotationY(m_rot_mesh2_y);
	//cc.m_world *= temp;

	//cc.m_world.setTranslation(m_last_translation2);
	// cc.m_world.setScale(Vector3D(10.0f, 10.0f, 10.0f));
	cc.m_world.setTranslation(Vector3D(x, y, z));
	cc.m_camera_position = m_world_cam.getTranslation();
	cc.m_light_direction = m_light_rot_matrix.getZDirection();
	cbprtlist[i]->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::drawMesh(const MeshPtr& mesh, const VertexShaderPtr& vs, const PixelShaderPtr& ps, const ConstantBufferPtr& cb, const TexturePtr& tex)
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(vs, cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(ps, cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(ps);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(ps, tex);

	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());


	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
	//GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawTriangleStrip(mesh->getVertexBuffer()->getSizeVertexList(), 0);
	//printf("draw finished frame %d", frame_count);
	//const std::vector<VertexMesh> * list_vertices_temp = m_mesh->getVertexMem();
	//printf("draw finished frame %f", (*list_vertices_temp)[0].m_position.m_x);
	//readFromVertexBuffer(mesh);
	//readFromConstantBuffer(cb);
}

void AppWindow::readFromVertexBuffer(const MeshPtr& mesh)
{
	ID3D11Buffer* m_buffer_copy;
	D3D11_BUFFER_DESC buff_desc_copy;

	m_buffer_copy = mesh->getVertexBuffer()->getD3DBuffer();
	m_buffer_copy->GetDesc(&buff_desc_copy);
	buff_desc_copy.Usage = D3D11_USAGE_STAGING;
	buff_desc_copy.BindFlags = 0;
	buff_desc_copy.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	ID3D11Buffer* temp_buffer;
	GraphicsEngine::get()->getRenderSystem()->getD3DDevice()->CreateBuffer(&buff_desc_copy, nullptr, &temp_buffer);
	//if (FAILED(GraphicsEngine::get()->getRenderSystem()->getD3DDevice()->CreateBuffer(m_buffer_copy, nullptr, &temp_buffer)))
	//{
	//	throw std::exception("VertexBuffer not created successfully");
	//}

	//DeviceContextPtr temp_context;
	//temp_context = GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext();

	ID3D11DeviceContext *context_ptr;

	context_ptr = GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->getD3DDeviceContext();
	context_ptr->CopyResource(temp_buffer, m_buffer_copy);

	D3D11_MAPPED_SUBRESOURCE mapped;
	HRESULT flag = context_ptr->Map(temp_buffer, 0, D3D11_MAP_READ, 0, &mapped);


	printf("%f\n", ((float*)mapped.pData)[0]);
}

void AppWindow::readFromConstantBuffer(const ConstantBufferPtr& buffer)
{
	ID3D11Buffer* m_buffer_copy;
	D3D11_BUFFER_DESC buff_desc_copy;
	
	m_buffer_copy = buffer->getD3DBuffer();
	m_buffer_copy->GetDesc(&buff_desc_copy);
	buff_desc_copy.Usage = D3D11_USAGE_STAGING;
	buff_desc_copy.BindFlags = 0;
	buff_desc_copy.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	ID3D11Buffer* temp_buffer;
	GraphicsEngine::get()->getRenderSystem()->getD3DDevice()->CreateBuffer(&buff_desc_copy, nullptr, &temp_buffer);
	//if (FAILED(GraphicsEngine::get()->getRenderSystem()->getD3DDevice()->CreateBuffer(m_buffer_copy, nullptr, &temp_buffer)))
	//{
	//	throw std::exception("VertexBuffer not created successfully");
	//}

	//DeviceContextPtr temp_context;
	//temp_context = GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext();

	ID3D11DeviceContext* context_ptr;

	context_ptr = GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->getD3DDeviceContext();
	context_ptr->CopyResource(temp_buffer, m_buffer_copy);

	D3D11_MAPPED_SUBRESOURCE mapped;
	HRESULT flag = context_ptr->Map(temp_buffer, 0, D3D11_MAP_READ, 0, &mapped);


	printf("done mapping\n");

}

void AppWindow::getRealtimeVertexList() //const MeshPtr& mesh
{


	for (size_t i = 0; i < data_size; i++)
	{	
		//update each point with vertex transformation into global frame
		Vector3D current_vertex = (*list_vertices_temp)[i].m_position;
		Vector4D current_vertex_4D = Vector4D(current_vertex.m_x, current_vertex.m_y, current_vertex.m_z, 1.0f);
		Vector3D temp_new_pos = m_mesh_last_T * current_vertex;

		vertices_list[i] = temp_new_pos;
	}

	for (size_t i = 0; i < data_size; i++)
	{
		indices_list[i] = (*list_indices_temp)[i];
	}

	for (size_t i = 0; i < data_size; i++)
	{
		cube_list1[i] = Vector3D((*list_vertices_temp)[i].m_position.m_x + 0.0003, (*list_vertices_temp)[i].m_position.m_y + 0.00035, (*list_vertices_temp)[i].m_position.m_z + 0.0005);
		cube_list2[i] = Vector3D((*list_vertices_temp)[i].m_position.m_x - 0.0003, (*list_vertices_temp)[i].m_position.m_y - 0.00035, (*list_vertices_temp)[i].m_position.m_z - 0.0005);
	}
}


AppWindow::~AppWindow()
{
}

void AppWindow::getinitvertex()
{
	list_vertices_temp = m_mesh->getVertexMem();
	list_indices_temp = m_mesh->getIndexMem();
	data_size = (*list_vertices_temp).size();
	vertices_list.reserve(data_size);
	indices_list.reserve(data_size);
	cube_list1.reserve(data_size);
	cube_list2.reserve(data_size);
	for (size_t i = 0; i < data_size; i++)
	{
		vertices_list.push_back((*list_vertices_temp)[i].m_position);
	}

	for (size_t i = 0; i < data_size; i++)
	{
		indices_list.push_back((*list_indices_temp)[i]);
	}

	for (size_t i = 0; i < data_size; i++)
	{
		cube_list1.push_back(Vector3D((*list_vertices_temp)[i].m_position.m_x + 0.0003, (*list_vertices_temp)[i].m_position.m_y + 0.00035, (*list_vertices_temp)[i].m_position.m_z + 0.0005));
		cube_list2.push_back(Vector3D((*list_vertices_temp)[i].m_position.m_x - 0.0003, (*list_vertices_temp)[i].m_position.m_y - 0.00035, (*list_vertices_temp)[i].m_position.m_z - 0.0005));
	}
}
void AppWindow::onCreate()
{
	Window::onCreate();
	cbprtlist.reserve(1000);
	psprtlist.reserve(1000);

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(true);
	
	
	m_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\Assets\\Textures\\brick.png");
	m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\Assets\\Meshes\\stanford-bunny.obj");
	m_bunny_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\Assets\\Meshes\\stanford-bunny.obj");
	m_cube_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"..\\Assets\\Textures\\red.png");
	m_cube = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"..\\Assets\\Meshes\\cube.obj");
	// initialization of vertices data

	//

	RECT rc = this->getClientWindowRect();
	m_swap_chain=GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	m_world_cam.setTranslation(Vector3D(0, 0, -1));

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs=GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	for (int i = 0; i < cubenum; i++)
	{
		PixelShaderPtr temp = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
		psprtlist.push_back(temp);
	}
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();


	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"BunnyPShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_bunny_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	

	constant cc;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
	for (int i = 0; i < cubenum; i++)
	{
		ConstantBufferPtr temp = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
		cbprtlist.push_back(temp);
	}
	m_bunny_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
	getinitvertex();

	constructSampleTree();
}


void AppWindow::constructSampleTree()
{
	unsigned int node_size = 1 + 2 + 4;
	unsigned int branch_size = 2;
	unsigned int level = 3;

	AABBTree sample_tree(node_size, branch_size);

	// level 0 - root_node
	sample_tree.addNode(AABB_NULL_NODE, false, false, false);
	// level 1
	sample_tree.addNode(0, true, false, false);
	sample_tree.addNode(0, false, true, false);

	sample_tree.addNode(1, true, false, true);
	sample_tree.addNode(1, false, true, true);

	sample_tree.addNode(2, true, false, true);
	sample_tree.addNode(2, false, true, true);

	printf("done construction.");
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();

	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0.86, 0.86f,0.86f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	//COMPUTE TRANSFORM MATRICES
	update();
	//rendering No.1 object
	drawMesh(m_mesh, m_vs, m_ps, m_cb, m_tex);
	//rendering No.2 object
	drawMesh(m_bunny_mesh, m_vs, m_bunny_ps, m_bunny_cb, m_tex);

	for (int i = 0; i < cubenum; i++)
	{
		drawMesh(m_cube, m_vs, psprtlist[i], cbprtlist[i], m_cube_tex);
	}


	m_swap_chain->present(true);


	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();

	m_delta_time = (m_old_delta)?((m_new_delta - m_old_delta) / 1000.0f):0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onKeyDown(int key)
{
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	if (key == '1') m_pick_state = 1;
	else if (key == '2') m_pick_state = 2;
	else if (key == '3') m_pick_state = 3;
	else if (key == '4') m_pick_state = 4;


	if (key == 'W')
	{
		//m_rot_x += 3.14f*m_delta_time;
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		//m_rot_x -= 3.14f*m_delta_time;
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		//m_rot_y += 3.14f*m_delta_time;
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
		//m_rot_y -= 3.14f*m_delta_time;
		m_rightward = 1.0f;
	}

	else if (key == 'Q')
	{
		//m_rot_y += 3.14f*m_delta_time;
		m_upward = 1.0f;
	}
	else if (key == 'Z')
	{
		//m_rot_y -= 3.14f*m_delta_time;
		m_upward = -1.0f;
	}
	else if (key == 'I')
	{
		if (m_pick_state == 1) m_rot_mesh_x += m_delta_time * 0.5f;
		else if (m_pick_state == 2) m_rot_mesh2_x += m_delta_time * 0.5f;
	}
	else if (key == 'K')
	{
		if (m_pick_state == 1) m_rot_mesh_x -= m_delta_time * 0.5f;
		else if (m_pick_state == 2) m_rot_mesh2_x -= m_delta_time * 0.5f;
	}
	else if (key == 'J')
	{
		if (m_pick_state == 1) m_rot_mesh_y -= m_delta_time * 0.5f;
		else if (m_pick_state == 2) m_rot_mesh2_y -= m_delta_time * 0.5f;
	}
	else if (key == 'L')
	{
		if (m_pick_state == 1) m_rot_mesh_y += m_delta_time * 0.5f;
		else if (m_pick_state == 2) m_rot_mesh2_y += m_delta_time * 0.5f;
	}
	else if (key == 'P')
	{
		if (!one_pick_lock) {
			m_pick_state = (m_pick_state + 1) % 4;
			if (m_pick_state == 0) m_pick_state = 4;
			one_pick_lock = true;
			printf("%d\n", m_pick_state);
		}

		
	}
}

void AppWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;
	m_upward = 0.0f;
	one_pick_lock = false;
}

void AppWindow::onMouseMove(const Point & mouse_pos)
{
	
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	if (m_left_mouse_down_flag)
	{
		m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * m_delta_time * 0.1f;
		m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * m_delta_time * 0.1f;
		InputSystem::get()->setCursorPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));
	}

	//InputSystem::get()->setCursorPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));
}

void AppWindow::onLeftMouseDown(const Point & mouse_pos)
{
	m_scale_cube = 1.0f;
	m_left_mouse_down_flag = true;
}

void AppWindow::onLeftMouseUp(const Point & mouse_pos)
{
	m_scale_cube = 1.0f;
	m_left_mouse_down_flag = false;
}

void AppWindow::onRightMouseDown(const Point & mouse_pos)
{
	m_scale_cube = 1.0f;
}

void AppWindow::onRightMouseUp(const Point & mouse_pos)
{
	m_scale_cube = 1.0f;
}
