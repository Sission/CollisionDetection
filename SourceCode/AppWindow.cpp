#include <d3d11.h>
#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"
#include "Prerequisites.h"
#include <cmath>
#include <algorithm>
#include "primitivetest.h"

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
	LPRECT prc = &this->getClientWindowRect();
	HDC dc = this->getClientWindowHdc();
	std::string temp1 = "\n1: object1, 2:object2, 3:camera, 4:lock";
	std::string temp2 = "\nW:in, S:out, A:left, T:right";
	std::string temp3 = "\nQ:up, Z:down";
	std::string temp4 = "\nI:rotX, K:-rotX, J:-rotY, L:rotY";
	std::string temp5 = "\nCollision count obj1:" + std::to_string(m_number_collision);
	std::string temp6 = "\nCollision count obj2:" + std::to_string(m_number_collision2);

	std::string temp0 = "Frame count:" + std::to_string(frame_count++);
	std::string temp = temp0 + "\nTime/frame:" + std::to_string(m_delta_time) + "s" + temp5 + temp6 + temp1 + temp2 + temp3 + temp4;
	LPCSTR content = temp.c_str();

	DrawText(dc,content,140, prc, DT_RIGHT);

	updateCamera();
	updateModel();
	updateBunnyBox();
	getRealtimeVertexList();

	std::vector<Vector2D> temp_result;
	if (constructOctree())
		temp_result = CollisionDetection::narrowPhaseDetection(Octree, Octree2, node_list, node_list2);
	//constructOctree();
	m_number_collision = 0;
	m_number_collision2 = 0;

	triangle_in_collision.clear();
	triangle_in_collision2.clear();
	triangle_in_collision.resize(data_size / 3);
	triangle_in_collision2.resize(data_size / 3);



	if (temp_result.size())
	{
		for (int n = 0; n < temp_result.size(); n++)
		{
			int tree1_cube_num = temp_result[n].m_x;
			int tree2_cube_num = temp_result[n].m_y;

			for (int i = 0; i < (*node_list)[tree1_cube_num].index_numbers.size(); i++)
			{
				int k1 = (*node_list)[tree1_cube_num].index_numbers[i];
				std::vector<std::vector<Vector3D>> temp_a;
				temp_a.push_back(testsample.maptoplane(vertices_list[k1]));
				temp_a.push_back(testsample.maptoplane(vertices_list[k1 + 1]));
				temp_a.push_back(testsample.maptoplane(vertices_list[k1 + 2]));
				for (int j = 0; j < (*node_list2)[tree2_cube_num].index_numbers.size(); j++)
				{
					int k2 = (*node_list2)[tree2_cube_num].index_numbers[j];

					//

					std::vector<std::vector<Vector3D>> temp_b;
					temp_b.push_back(testsample.maptoplane(vertices_list2[k2]));
					temp_b.push_back(testsample.maptoplane(vertices_list2[k2 + 1]));
					temp_b.push_back(testsample.maptoplane(vertices_list2[k2 + 2]));
					if (testsample.finalcheck(temp_a, temp_b))
					{
						if (!triangle_in_collision[k1 / 3])
						{
							triangle_in_collision[k1 / 3] = true;
							m_number_collision++;
						}
						if (!triangle_in_collision2[k2 / 3])
						{
							triangle_in_collision2[k2 / 3] = true;
							m_number_collision2++;
						}
							
					}
						
				}
			}
		}

	}


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
		m_last_translation = m_last_translation + Vector3D(m_rightward * 0.001f, m_upward * 0.001f, m_forward * 0.001f);
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

}

void AppWindow::updateCamera()
{
	
	if (m_pick_state == 3) { // if camera selected, update movement
		Matrix4x4 world_cam, temp;
		world_cam.setIdentity();

		Vector3D new_pos = m_world_cam.getTranslation();

	//rotation
		temp.setIdentity();
		temp.setRotationX(m_rot_x);
		world_cam *= temp;

		temp.setIdentity();
		temp.setRotationY(m_rot_y);
		world_cam *= temp;

		//translation
		new_pos = new_pos + world_cam.getZDirection() * (m_forward * 0.035f);
		new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 0.035f);
		new_pos = new_pos + world_cam.getYDirection() * (m_upward * 0.035f);

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
	m_mesh_last_T2.setMatrix(cc.m_world);

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
		//Vector3D current_vertex = (*list_vertices_temp)[i].m_position;
		//Vector4D current_vertex_4D = Vector4D((*list_vertices_temp)[i].m_position.m_x, (*list_vertices_temp)[i].m_position.m_y, (*list_vertices_temp)[i].m_position.m_z, 1.0f);
		//Vector3D temp_new_pos = m_mesh_last_T * (*list_vertices_temp)[i].m_position;
		vertices_list[i] = m_mesh_last_T * (*list_vertices_temp)[i].m_position;//temp_new_pos;
		vertices_list2[i] = m_mesh_last_T2 * (*list_vertices_temp2)[i].m_position;
	}

}


AppWindow::~AppWindow()
{
}

void AppWindow::getinitvertex()
{
	list_vertices_temp = m_mesh->getVertexMem();
	list_indices_temp = m_mesh->getIndexMem();
	list_vertices_temp2 = m_bunny_mesh->getVertexMem();
	list_indices_temp2 = m_bunny_mesh->getIndexMem();

	data_size = (*list_vertices_temp).size();
	vertices_list.reserve(data_size);
	indices_list.reserve(data_size);
	vertices_list2.reserve(data_size);
	indices_list2.reserve(data_size);

	//cube_list1.reserve(data_size);
	//cube_list2.reserve(data_size);
	for (size_t i = 0; i < data_size; i++)
	{
		vertices_list.push_back((*list_vertices_temp)[i].m_position);
		vertices_list2.push_back((*list_vertices_temp2)[i].m_position);
	}

	for (size_t i = 0; i < data_size; i++)
	{
		indices_list.push_back((*list_indices_temp)[i]);
		indices_list2.push_back((*list_indices_temp2)[i]);
	}

	triangle_in_collision.resize(data_size / 3);
	triangle_in_collision2.resize(data_size / 3);

}
void AppWindow::onCreate()
{
	Window::onCreate();

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
	edge = getcubeegde(vertices_list, vertices_list2);
	constructOctree2();

}


double AppWindow::getcubeegde(std::vector<Vector3D> vertices_list, std::vector<Vector3D> vertices_list2)
{

	minx = vertices_list[0].m_x;
	miny = vertices_list[0].m_y;
	minz = vertices_list[0].m_z;
	minx2 = vertices_list2[0].m_x;
	miny2 = vertices_list2[0].m_y;
	minz2 = vertices_list2[0].m_z;

	maxx = vertices_list[0].m_x;
	maxy = vertices_list[0].m_y;
	maxz = vertices_list[0].m_z;
	maxx2 = vertices_list2[0].m_x;
	maxy2 = vertices_list2[0].m_y;
	maxz2 = vertices_list2[0].m_z;


	for (int i = 1; i < data_size; i += 3)
	{
		maxx = (vertices_list[i].m_x > maxx ? vertices_list[i].m_x : maxx);
		maxy = (vertices_list[i].m_y > maxy ? vertices_list[i].m_y : maxy);
		maxz = (vertices_list[i].m_z > maxz ? vertices_list[i].m_z : maxz);
		maxx2 = (vertices_list2[i].m_x > maxx2 ? vertices_list2[i].m_x : maxx2);
		maxy2 = (vertices_list2[i].m_y > maxy2 ? vertices_list2[i].m_y : maxy2);
		maxz2 = (vertices_list2[i].m_z > maxz2 ? vertices_list2[i].m_z : maxz2);

		minx = (vertices_list[i].m_x > minx ? minx : vertices_list[i].m_x);
		miny = (vertices_list[i].m_y > miny ? miny : vertices_list[i].m_y);
		minz = (vertices_list[i].m_z > minz ? minz : vertices_list[i].m_z);
		minx2 = (vertices_list2[i].m_x > minx2 ? minx2 : vertices_list2[i].m_x);
		miny2 = (vertices_list2[i].m_y > miny2 ? miny2 : vertices_list2[i].m_y);
		minz2 = (vertices_list2[i].m_z > minz2 ? minz2 : vertices_list2[i].m_z);
	}
	edge = ((maxx - minx) > (maxy - miny)) ? (maxx - minx) : (maxy - miny);
	edge = (edge > (maxz - minz)) ? edge : (maxz - minz);

	return edge;
}

bool AppWindow::constructOctree()
{
	unsigned int branch_size = 8;
	unsigned int node_size = pow(branch_size, 0) + pow(branch_size, 1) + pow(branch_size, 2) + pow(branch_size, 3) + pow(branch_size, 4) + pow(branch_size, 5) + pow(branch_size, 6);
	unsigned int level = 7; 
	double tempedge = getcubeegde(vertices_list, vertices_list2);//edge;

	Octree.Initialization(pow(branch_size, 0), branch_size);
	
	if (frame_count < 2)
		node_list = Octree.getAABBTree();
	// Level 0 - root
	
	//Vector3D new_min_coord = Vector3D(minx, miny, minz);
	//Octree.addNode(AABB_NULL_NODE, false, false, false, new_min_coord, new_min_coord + Vector3D(tempedge, tempedge, tempedge));	
	Octree.addNode(AABB_NULL_NODE, false, false, false, Vector3D(minx, miny, minz), Vector3D(minx + tempedge, miny + tempedge, minz + tempedge));
	// Here we run broad phase detection.
	// if broadphase detected no collision at the root level, no need to construct the tree, proceed to next frame.
	if (!CollisionDetection::broadPhaseDetectionRoot(node_list, node_list2)) return false;
	//return false;
	// The root node has all vertices' indices
	// Level 1
	bvlevel(0, tempedge);
	// 1 + 8 cubes already;
	unsigned int pointer = 1;
	for (int j = pointer; j < pointer + branch_size; j++)
		// loop from [1] to [9]
	{
		(*node_list)[j].index_numbers.clear();
		for (int i = 0; i < data_size; i = i + 3)
		{
			if ((*node_list)[j].node_info >= vertices_list[i])
			{
				(*node_list)[j].index_numbers.push_back(i);
			}
		}
	}
	// Finish the vertices assigning to level 1 (8 cubes);

	// pruning the level 1 (8 cubes) - mark the level 1 cubes not containing any vertices;
	for (int j = pointer; j < pointer + branch_size; j++)
	{
		if ((*node_list)[j].index_numbers.size() == 0)
		{
		(*node_list)[j].is_pruned = true;
		}
	}

	// Level 2
	// expand the bounding volume by the half of the cube's edge;
	// 64 cubes totally
	tempedge = tempedge / 2;
	for (int i = pointer; i < pointer + branch_size; i++)
	{
		bvlevel(i, tempedge);
	}
	// create level 2 (64 cubes) BVs, 1 + 8 + 64 in total; assign 64 cubes to parent [1] to [8]

	pointer = pointer + pow(branch_size, 1);
	assignverticeindex(pointer, 2, node_list, vertices_list);
	// Finish the vertices assigning to the level 2 (64 cubes);
	pruning(pointer, 2, node_list);
	// pruning level 2 - mark the level 2 BV not containing any vertices;


	// Level 3
	// 512 cubes totally
	tempedge = tempedge / 2;
	for (int i = pointer; i < pointer + pow(branch_size,2); i++)
	{
		bvlevel(i, tempedge);
	}
	// create level 2 (64 cubes) BVs, 1 + 8 + 64 + 512 in total; assign 512 cubes to parent [9] to [72]
	pointer = pointer + pow(branch_size, 2);
	// pointer is 73
	assignverticeindex(pointer, 3, node_list, vertices_list);
	// Finish the vertices assigning to the level 3 (512 cubes);
	pruning(pointer, 3, node_list);
	// pruning level 3 - mark the level 3 BV not containing any vertices;

	// Level 4
	// 4096 cubes totally
	tempedge = tempedge / 2;
	for (int i = pointer; i < pointer + pow(branch_size, 3); i++)
	{
		bvlevel(i, tempedge);
	}
	// create level 2 (64 cubes) BVs, 1 + 8 + 64 + 512 in total; assign 512 cubes to parent [9] to [72]
	pointer = pointer + pow(branch_size, 3);
	// pointer is 73
	assignverticeindex(pointer, 4, node_list, vertices_list);
	// Finish the vertices assigning to the level 4 (4096 cubes);
	pruning(pointer, 4, node_list);
	// pruning level 4 - mark the level 3 BV not containing any vertices;

	// Level 5
	// 4096*8 cubes totally
	tempedge = tempedge / 2;
	for (int i = pointer; i < pointer + pow(branch_size, 4); i++)
	{
		bvlevel(i, tempedge);
	}
	// create level 2 (64 cubes) BVs, 1 + 8 + 64 + 512 in total; assign 512 cubes to parent [9] to [72]
	pointer = pointer + pow(branch_size, 4);
	// pointer is 73
	assignverticeindex(pointer, 5, node_list, vertices_list);
	// Finish the vertices assigning to the level 4 (4096 cubes);
	pruning(pointer, 5, node_list);
	// pruning level 4 - mark the level 3 BV not containing any vertices;

	// Level 6
	// 4096*8 *8 cubes totally
	tempedge = tempedge / 2;
	for (int i = pointer; i < pointer + pow(branch_size, 5); i++)
	{
		bvlevel(i, tempedge);
	}
	// create level 2 (64 cubes) BVs, 1 + 8 + 64 + 512 in total; assign 512 cubes to parent [9] to [72]
	pointer = pointer + pow(branch_size, 5);
	// pointer is 73
	assignverticeindex(pointer, 6, node_list, vertices_list);
	// Finish the vertices assigning to the level 4 (4096 cubes);
	pruning(pointer, 6, node_list);
	// pruning level 4 - mark the level 3 BV not containing any vertices;

  	return true;
 }

bool AppWindow::constructOctree2()
 {
	 unsigned int branch_size = 8;
	 unsigned int node_size = pow(branch_size, 0) + pow(branch_size, 1) + pow(branch_size, 2) + pow(branch_size, 3) + pow(branch_size, 4) + pow(branch_size, 5) + pow(branch_size, 6);
	 unsigned int level = 7; // 5
	 double tempedge = edge;

	 Octree2.Initialization(pow(branch_size, 0), branch_size);
	 node_list2 = Octree2.getAABBTree();
	 // Level 0
	 Octree2.addNode(AABB_NULL_NODE, false, false, false, Vector3D(minx2, miny2, minz2), Vector3D(minx2 + tempedge, miny2 + tempedge, minz2 + tempedge));
	 bvlevel2(0, tempedge);
	 // 1 + 8 cubes already;
	 unsigned int pointer = 1;
	 for (int j = pointer; j < pointer + branch_size; j++)
		 // loop from [1] to [9]
	 {
		 (*node_list2)[j].index_numbers.clear();
		 for (int i = 0; i < data_size; i = i + 3)
		 {
			 if ((*node_list2)[j].node_info >= vertices_list2[i])
			 {
				 (*node_list2)[j].index_numbers.push_back(i);
			 }
		 }
	 }
	 // Finish the vertices assigning to level 1 (8 cubes);

	 // pruning the level 1 (8 cubes) - mark the level 1 cubes not containing any vertices;
	 for (int j = pointer; j < pointer + branch_size; j++)
	 {
		 if ((*node_list2)[j].index_numbers.size() == 0)
		 {
			 (*node_list2)[j].is_pruned = true;
		 }
	 }

	 // Level 2
	 // expand the bounding volume by the half of the cube's edge;
	 // 64 cubes totally
	 tempedge = tempedge / 2;
	 for (int i = pointer; i < pointer + branch_size; i++)
	 {
		 bvlevel2(i, tempedge);
	 }
	 // create level 2 (64 cubes) BVs, 1 + 8 + 64 in total; assign 64 cubes to parent [1] to [8]
	 pointer = pointer + pow(branch_size, 1);
	 assignverticeindex(pointer, 2, node_list2, vertices_list2);
	 // Finish the vertices assigning to the level 2 (64 cubes);
	 pruning(pointer, 2, node_list2);
	 // pruning level 2 - mark the level 2 BV not containing any vertices;

	 // Level 3
	 // 512 cubes totally
	 tempedge = tempedge / 2;
	 for (int i = pointer; i < pointer + pow(branch_size, 2); i++)
	 {
		 bvlevel2(i, tempedge);
	 }
	 // create level 2 (64 cubes) BVs, 1 + 8 + 64 + 512 in total; assign 512 cubes to parent [9] to [72]
	 pointer = pointer + pow(branch_size, 2);
	 // pointer is 73
	 assignverticeindex(pointer, 3, node_list2, vertices_list2);
	 // Finish the vertices assigning to the level 3 (512 cubes);
	 pruning(pointer, 3, node_list2);
	 // pruning level 3 - mark the level 3 BV not containing any vertices;

	  // Level 4
	 // 4096 cubes totally
	 tempedge = tempedge / 2;
	 for (int i = pointer; i < pointer + pow(branch_size, 3); i++)
	 {
		 bvlevel2(i, tempedge);
	 }
	 // create level 2 (64 cubes) BVs, 1 + 8 + 64 + 512 in total; assign 512 cubes to parent [9] to [72]
	 pointer = pointer + pow(branch_size, 3);
	 // pointer is 73
	 assignverticeindex(pointer, 4, node_list2, vertices_list2);
	 // Finish the vertices assigning to the level 4 (4096 cubes);
	 pruning(pointer, 4, node_list2);
	 // pruning level 4 - mark the level 4 BV not containing any vertices;

	  // Level 5
	 // 4096*8 cubes totally
	 tempedge = tempedge / 2;
	 for (int i = pointer; i < pointer + pow(branch_size, 4); i++)
	 {
		 bvlevel2(i, tempedge);
	 }
	 // create level 2 (64 cubes) BVs, 1 + 8 + 64 + 512 in total; assign 512 cubes to parent [9] to [72]
	 pointer = pointer + pow(branch_size, 4);
	 // pointer is 73
	 assignverticeindex(pointer, 5, node_list2, vertices_list2);
	 // Finish the vertices assigning to the level 4 (4096 cubes);
	 pruning(pointer, 5, node_list2);
	 // pruning level 4 - mark the level 4 BV not containing any vertices;

	  // Level 6
	 // 4096*8*8 cubes totally
	 tempedge = tempedge / 2;
	 for (int i = pointer; i < pointer + pow(branch_size, 5); i++)
	 {
		 bvlevel2(i, tempedge);
	 }
	 // create level 2 (64 cubes) BVs, 1 + 8 + 64 + 512 in total; assign 512 cubes to parent [9] to [72]
	 pointer = pointer + pow(branch_size, 5);
	 // pointer is 73
	 assignverticeindex(pointer, 6, node_list2, vertices_list2);
	 // Finish the vertices assigning to the level 4 (4096 cubes);
	 pruning(pointer, 6, node_list2);
	 // pruning level 4 - mark the level 4 BV not containing any vertices;
	 return true;
 }

void AppWindow::pruning(int pointer, int power, std::vector<AABBNode>* node_list)
 {
	 for (int j = pointer; j < pointer + pow(8, power); j++)
	 {
		 if ((*node_list)[j].index_numbers.size() == 0)
		 {
			 (*node_list)[j].is_pruned = true;
		 }
	 }
 }

void AppWindow::assignverticeindex(int pointer, int power, std::vector<AABBNode>* node_list, std::vector<Vector3D> vertices_list)
{
	for (int j = pointer; j < pointer + pow(8, power); j++)
		// loop from [73] to [584]
	{
		int parent_index_j = (*node_list)[j].parent_index;
		(*node_list)[j].index_numbers.clear();

		for (int i = 0; i < (*node_list)[parent_index_j].index_numbers.size(); i = i + 1) //int i = 0; i < data_size; i = i + 3
		{
			if ((*node_list)[(*node_list)[j].parent_index].is_pruned == true)
				// pruning
			{
				(*node_list)[j].is_pruned = true;
				break;
			}
			else if ((*node_list)[j].node_info >= vertices_list[(*node_list)[parent_index_j].index_numbers[i]]) // vertices[i]
			{
				(*node_list)[j].index_numbers.push_back((*node_list)[parent_index_j].index_numbers[i]);
			}
		}
	}
}


void AppWindow::bvlevel(int i, double tempedge)
{
	updateminx = (*node_list)[i].node_info.getMinCoordination().m_x;
	updateminy = (*node_list)[i].node_info.getMinCoordination().m_y;
	updateminz = (*node_list)[i].node_info.getMinCoordination().m_z;
	// Low level 4 cubes
	Octree.addNode(i, true, false, false, Vector3D(updateminx , updateminy , updateminz ), Vector3D(updateminx + tempedge / 2 , updateminy + tempedge / 2 , updateminz + tempedge / 2 ));
	Octree.addNode(i, false, false, false, Vector3D(updateminx  + tempedge / 2, updateminy , updateminz ), Vector3D(updateminx + tempedge , updateminy + tempedge / 2 , updateminz + tempedge / 2 ));
	Octree.addNode(i, false, false, false, Vector3D(updateminx , updateminy  + tempedge / 2, updateminz ), Vector3D(updateminx + tempedge / 2 , updateminy + tempedge , updateminz + tempedge / 2 ));
	Octree.addNode(i, false, false, false, Vector3D(updateminx  + tempedge / 2, updateminy  + tempedge / 2, updateminz ), Vector3D(updateminx + tempedge , updateminy + tempedge , updateminz + tempedge / 2 ));
	// Top level 4 cubes
	Octree.addNode(i, false, false, false, Vector3D(updateminx , updateminy , updateminz  + tempedge / 2), Vector3D(updateminx + tempedge / 2 , updateminy + tempedge / 2 , updateminz + tempedge ));
	Octree.addNode(i, false, false, false, Vector3D(updateminx  + tempedge / 2, updateminy , updateminz  + tempedge / 2), Vector3D(updateminx + tempedge , updateminy + tempedge / 2 , updateminz + tempedge ));
	Octree.addNode(i, false, false, false, Vector3D(updateminx , updateminy  + tempedge / 2, updateminz  + tempedge / 2), Vector3D(updateminx  + tempedge / 2, updateminy  + tempedge, updateminz  + tempedge));
	Octree.addNode(i, false, true, false, Vector3D(updateminx  + tempedge / 2, updateminy  + tempedge / 2, updateminz  + tempedge / 2), Vector3D(updateminx + tempedge , updateminy + tempedge , updateminz + tempedge ));
}


void AppWindow::bvlevel2(int i, double tempedge)
{
	updateminx2 = (*node_list2)[i].node_info.getMinCoordination().m_x;
	updateminy2 = (*node_list2)[i].node_info.getMinCoordination().m_y;
	updateminz2 = (*node_list2)[i].node_info.getMinCoordination().m_z;
	// Low level 4 cubes
	Octree2.addNode(i, true, false, false, Vector3D(updateminx2 , updateminy2 , updateminz2 ), Vector3D(updateminx2 + tempedge / 2 , updateminy2 + tempedge / 2 , updateminz2 + tempedge / 2 ));
	Octree2.addNode(i, false, false, false, Vector3D(updateminx2  + tempedge / 2, updateminy2 , updateminz2 ), Vector3D(updateminx2 + tempedge , updateminy2 + tempedge / 2 , updateminz2 + tempedge / 2 ));
	Octree2.addNode(i, false, false, false, Vector3D(updateminx2 , updateminy2  + tempedge / 2, updateminz2 ), Vector3D(updateminx2 + tempedge / 2 , updateminy2 + tempedge , updateminz2 + tempedge / 2 ));
	Octree2.addNode(i, false, false, false, Vector3D(updateminx2  + tempedge / 2, updateminy2  + tempedge / 2, updateminz2 ), Vector3D(updateminx2 + tempedge , updateminy2 + tempedge , updateminz2 + tempedge / 2 ));
	// Top level 4 cubes
	Octree2.addNode(i, false, false, false, Vector3D(updateminx2 , updateminy2 , updateminz2  + tempedge / 2), Vector3D(updateminx2 + tempedge / 2 , updateminy2 + tempedge / 2 , updateminz2 + tempedge ));
	Octree2.addNode(i, false, false, false, Vector3D(updateminx2  + tempedge / 2, updateminy2 , updateminz2  + tempedge / 2), Vector3D(updateminx2 + tempedge , updateminy2 + tempedge / 2 , updateminz2 + tempedge ));
	Octree2.addNode(i, false, false, false, Vector3D(updateminx2 , updateminy2  + tempedge / 2, updateminz2  + tempedge / 2), Vector3D(updateminx2  + tempedge / 2, updateminy2  + tempedge, updateminz2  + tempedge));
	Octree2.addNode(i, false, true, false, Vector3D(updateminx2  + tempedge / 2, updateminy2  + tempedge / 2, updateminz2  + tempedge / 2), Vector3D(updateminx2 + tempedge , updateminy2 + tempedge , updateminz2 + tempedge ));
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

	/*drawing AABB cubes, comment out to accelerate performance*/
	//for (int i = 0; i < cubenum; i++)
	//{
	//	drawMesh(m_cube, m_vs, psprtlist[i], cbprtlist[i], m_cube_tex);
	//}


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
		m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * m_delta_time * 0.05f;
		m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * m_delta_time * 0.05f;
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
