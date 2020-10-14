#pragma once
#include "AABB.h"
#include "AABBTree.h"
#include "Vector2D.h"
#include <vector>
#include <stack> 

class CollisionDetection
{
public:
	CollisionDetection();
	static bool broadPhaseDetectionRoot(std::vector<AABBNode>* node_list, std::vector<AABBNode>* node_list2); //called when bpd_lvl == 0
	bool broadPhaseDetection(AABBTree octree1, AABBTree octree2, unsigned terminating_level); // terminating_level >= 1
	static std::vector<Vector2D> narrowPhaseDetection(AABBTree octree1, AABBTree octree2, std::vector<AABBNode>* node_list, std::vector<AABBNode>* node_list2);

	bool hybridUpdateDetection(); //hybrid method mentioned in the paper if time permits
	bool isLeaf(AABBNode node);
	static bool oneToManyIntersect(std::vector<AABBNode>* node_list, std::vector<AABBNode>* node_list2, int index1, std::vector<int> indices2);
	
	static bool isInCollisionSet(Vector2D collision_pair, std::vector<Vector2D> collision_set);

	unsigned int getBroadPhaseDetectionLevel();


	~CollisionDetection();

private:
	unsigned int bpd_lvl = 0; //if set 0, the broad phase detection only run intersection test between the two root BVs
	//std::vector<AABBNode>* node_list;
	//std::vector<AABBNode>* node_list2;


};

