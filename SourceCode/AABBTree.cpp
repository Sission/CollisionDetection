#include "AABBTree.h"
#include <cassert>

AABBTree::AABBTree(): root_index(0), next_free_index(AABB_NULL_NODE)
{
	node_capacity = 0;
	branch_size = 2;
}

AABBTree::AABBTree(unsigned int initial_size, unsigned int bs): root_index(0), next_free_index(AABB_NULL_NODE)
{
	node_capacity = initial_size;
	branch_size = bs;
	node_list.resize(initial_size); //initialize the node_list vector with empty nodes
	for (int i = 0; i < initial_size; i++)
	{
		AABBNode& node = node_list[i]; // notice & here, means the change of content in node will exactly change node_list[i]
		node.next_index = i + 1;
	}
	node_list[initial_size - 1].next_index = AABB_NULL_NODE;
	if (initial_size > 0) next_free_index = 0;
}

void AABBTree::Initialization(unsigned int initial_size, unsigned int bs)
{
	node_capacity = initial_size;
	branch_size = bs;
	node_list.clear();
	node_list.resize(initial_size); //initialize the node_list vector with empty nodes
	for (int i = 0; i < initial_size; i++)
	{
		AABBNode& node = node_list[i]; // notice & here, means the change of content in node will exactly change node_list[i]
		node.next_index = i + 1;
	}
	node_list[initial_size - 1].next_index = AABB_NULL_NODE;
	if (initial_size > 0) next_free_index = 0;
}


unsigned int AABBTree::addNode(unsigned int parent_index, bool is_left, bool is_right, bool is_leaf, Vector3D min_coord, Vector3D max_coord)
{
	// if we have no free tree nodes then grow the vector size by 1
	if (next_free_index == AABB_NULL_NODE)
	{
		//assert(allocated_node_count == node_capacity);

		node_capacity += 1;
		node_list.resize(node_capacity);

		AABBNode& node = node_list[node_capacity-1];
		node.next_index = AABB_NULL_NODE;
		next_free_index = node_capacity - 1;
	}

	unsigned int node_index = next_free_index;
	AABBNode& added_node = node_list[node_index];
	added_node.node_info.setAABB(min_coord, max_coord);
	added_node.parent_index = parent_index;
	added_node.next_index = node_index + 1;
	if (added_node.next_index == node_capacity)
		added_node.next_index = AABB_NULL_NODE;

	if (is_left)
		node_list[parent_index].left_child_index = node_index;
	if (is_right)
		node_list[parent_index].right_child_index = node_index;
	if (is_leaf){
		added_node.left_child_index = AABB_NULL_NODE;
		added_node.right_child_index = AABB_NULL_NODE;
	}

	next_free_index = added_node.next_index;

	return node_index;
}

unsigned int AABBTree::getBranchSize()
{
	return branch_size;
}

void AABBTree::pruneNode(unsigned int node_index)
{
	node_list[node_index].is_pruned = true;
}



std::vector<AABBNode>* AABBTree::getAABBTree()
{
	return &node_list;
}


AABBTree::~AABBTree()
{
}
