#pragma once
#include "AABB.h"
#include <memory>
#include <vector>
//#include <map>
//#include <forward_list>

#define AABB_NULL_NODE 0xffffffff

struct AABBNode
{
	AABB node_info; 
	//std::shared_ptr<AABB> object;
	// tree links
	unsigned int parent_index;
	unsigned int left_child_index; //leftChildNodeIndex should be smaller than rightChildNodeIndex, (you can understand it as lower bound and upper bound)
	unsigned int right_child_index;

	// node linked list link
	unsigned int next_index;

	// included vertices
	std::vector<unsigned> index_numbers; // ***

	bool is_pruned;
	bool isLeaf() const { return left_child_index == AABB_NULL_NODE&&right_child_index == AABB_NULL_NODE; }

	AABBNode(): parent_index(AABB_NULL_NODE), left_child_index(AABB_NULL_NODE), right_child_index(AABB_NULL_NODE), next_index(AABB_NULL_NODE)
	{
		is_pruned = false;
	}
};

class AABBTree //only using top down construction
{
private:
	//std::map<std::shared_ptr<IAABB>, unsigned> _objectNodeIndexMap;
	std::vector<AABBNode> node_list;
	unsigned int root_index;
	unsigned int next_free_index;
	unsigned int node_capacity;
	unsigned int branch_size;

public:
	AABBTree();
	AABBTree(unsigned int initial_size, unsigned int bs);
	unsigned int addNode(unsigned int parent_index, bool is_left, bool is_right, bool is_leaf, Vector3D min_coord, Vector3D max_coord);
	unsigned int getBranchSize();
	void pruneNode(unsigned int node_index);
	void Initialization(unsigned int initial_size, unsigned int bs);
	std::vector<AABBNode>* getAABBTree();
	~AABBTree();

};