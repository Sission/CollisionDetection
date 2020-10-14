#pragma once
#include <d3d11.h>
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"
#include "Matrix4x4.h"
#include "Vector3D.h"
#include "Prerequisites.h"
#include "VertexMesh.h"
#include "AABBTree.h"
#include "AABB.h"
#include "CollisionDetection.h"
#include "primitivetest.h"

class AppWindow: public Window,public InputListener
{
public:
	AppWindow();
	~AppWindow();

	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;	
	virtual void onFocus() override;
	virtual void onKillFocus() override;

	// Inherited via InputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_pos) override;

	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;
public:
	void update();
	void updateModel();
	void updateCamera();
	void updateBunnyBox();
	void updateCubeBox(float x, float y, float z, int i);
	void getinitvertex();
	void drawMesh(const MeshPtr& mesh, const VertexShaderPtr& vs, const PixelShaderPtr& ps, const ConstantBufferPtr& cb, const TexturePtr& tex);
	void readFromVertexBuffer(const MeshPtr& mesh);
	void readFromConstantBuffer(const ConstantBufferPtr& buffer);
	void getRealtimeVertexList();
	void bvlevel(int i, double tempedge);
	void bvlevel2(int i, double tempedge);
	void assignverticeindex(int pointer, int power, std::vector<AABBNode>* node_list, std::vector<Vector3D> vertices_list);
	void pruning(int pointer, int power, std::vector<AABBNode>* node_list);
	bool constructOctree();
	bool constructOctree2();
	double getcubeegde(std::vector<Vector3D> vertices_list, std::vector<Vector3D> vertices_list2);

private:
	SwapChainPtr m_swap_chain;
	VertexShaderPtr m_vs;
	VertexShaderPtr m_bunny_vs;
	VertexShaderPtr m_cubevs;
	PixelShaderPtr m_ps;
	PixelShaderPtr m_bunny_ps;
	ConstantBufferPtr m_cb;
	ConstantBufferPtr m_bunny_cb;
	ConstantBufferPtr m_cubecb;
	TexturePtr m_tex;
	TexturePtr m_bunny_tex;
	TexturePtr m_cube_tex;
	MeshPtr m_mesh;
	MeshPtr m_bunny_mesh;
	MeshPtr m_cube;

	//ID3D11Buffer* m_buffer_copy;
	//D3D11_BUFFER_DESC buff_desc_copy;

private:
	long m_old_delta;
	long m_new_delta;
	long frame_count = 0;

	float m_delta_time;

	float m_delta_pos;
	float m_delta_scale;
	float m_delta_rot;

	float m_rot_x=0.0f;
	float m_rot_y = 0.0f;

	float m_rot_mesh_x = 0.0f;
	float m_rot_mesh_y = 0.0f;

	float m_rot_mesh2_x = 0.0f;
	float m_rot_mesh2_y = 0.0f;

	float m_light_rot_y = 0.0f;

	float m_scale_cube = 1;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;
	float m_upward = 0.0f; //XL

	int m_pick_state = 3; //XL
	bool one_pick_lock = false; //XL
	bool m_left_mouse_down_flag = false; //XL


	Matrix4x4 m_world_cam;
	Matrix4x4 m_view_cam;
	Matrix4x4 m_proj_cam;

	Matrix4x4 m_mesh_last_T;
	Matrix4x4 m_mesh_last_T2;

	Vector3D m_last_translation = Vector3D(0.1f, 0.1f, 0.1f);
	Vector3D m_last_translation2 = Vector3D(0.0f, 0.0f, 0.0f);

	std::vector<VertexMesh>* list_vertices_temp;
	std::vector<unsigned int>* list_indices_temp;
	std::vector<VertexMesh>* list_vertices_temp2;
	std::vector<unsigned int>* list_indices_temp2;

	std::vector<Vector2D> last_collision_pairs;

	int data_size;
	int cubenum = 4096;
	int interval = floor(91014 / cubenum);
	int m_number_collision = 0;
	int m_number_collision2 = 0;

	double minx, minx2;
	double miny, miny2;
	double minz, minz2;
	double maxx, maxx2;
	double maxy, maxy2;
	double maxz, maxz2;
	double edge;
	double d = 0.0018;
	double updateminx, updateminx2;
	double updateminy, updateminy2;
	double updateminz, updateminz2;
	double updatemaxx, updatemaxx2;
	double updatemaxy, updatemaxy2;
	double updatemaxz, updatemaxz2;
	
	//std::vector<Vector3D> cube_list1;
	//std::vector<Vector3D> cube_list2;
	std::vector<Vector3D> vertices_list;
	std::vector<unsigned int> indices_list;
	std::vector<Vector3D> vertices_list2;
	std::vector<unsigned int> indices_list2;

	std::vector<bool> triangle_in_collision;
	std::vector<bool> triangle_in_collision2;

	std::vector<double> xcoordinates;
	std::vector<double> ycoordinates;
	std::vector<double> zcoordinates;
	std::vector<ConstantBufferPtr> cbprtlist;
	std::vector<PixelShaderPtr> psprtlist;
	
	std::vector<AABBNode>* node_list;
	std::vector<AABBNode>* node_list2;

	std::vector<std::vector<Vector3D>> mapa_list;
	std::vector<std::vector<Vector3D>> mapb_list;
	
	AABBTree Octree;
	AABBTree Octree2;

	primitivetest testsample;


};

