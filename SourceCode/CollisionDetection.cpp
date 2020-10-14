#include "CollisionDetection.h"
//#include <stack> 

CollisionDetection::CollisionDetection()
{
}

bool CollisionDetection::broadPhaseDetectionRoot(std::vector<AABBNode>* node_list, std::vector<AABBNode>* node_list2) //run broadPhaseDetection on the biggest two boxes built from the top down level
{
	return (*node_list)[0].node_info & (*node_list2)[0].node_info;
}

bool CollisionDetection::broadPhaseDetection(AABBTree octree1, AABBTree octree2, unsigned terminating_level) 
{
	return (*octree1.getAABBTree())[0].node_info & (*octree2.getAABBTree())[0].node_info;
}

std::vector<Vector2D> CollisionDetection::narrowPhaseDetection(AABBTree octree1, AABBTree octree2, std::vector<AABBNode>* node_list, std::vector<AABBNode>* node_list2)
{
	//run dfs to traverse two trees 
	
	std::stack<int> traverse_stack;
	std::stack<int> traverse_stack2;

	std::vector<int> collision_leaves2;

	std::vector<Vector2D> collision_pairs;

	/*node_list = octree1.getAABBTree();
	node_list2 = octree2.getAABBTree();*/

	int one = 1;

	for(int i = 1; i < octree1.getBranchSize(); i++)
	{
		int temp_index = i;
		
		collision_leaves2.clear();
		traverse_stack2.push(0);
		int last_temp_pop2 = 0;
		while (!traverse_stack2.empty())
		{
			int temp_index2 = traverse_stack2.top();
			//pop conditions (1) child traversed (2) pruned (3) no collision
			if ((*node_list2)[temp_index2].right_child_index == last_temp_pop2 ||
				(*node_list2)[temp_index2].is_pruned || 
				!((*node_list2)[temp_index2].node_info&(*node_list)[temp_index].node_info)) 
			{
				last_temp_pop2 = temp_index2;
				traverse_stack2.pop();
			}
			else // child not traversed && not pruned && collision detected
			{
				if ((*node_list2)[temp_index2].isLeaf())
				{
					collision_leaves2.push_back(temp_index2);
					last_temp_pop2 = temp_index2;
					traverse_stack2.pop();
				}
				else if (last_temp_pop2 <= temp_index2) //if last pop is the node before current node, either at the same lvl or the parent, go deeper to left child
				{
					traverse_stack2.push((*node_list2)[temp_index2].left_child_index);
				}
				else 
				{
					traverse_stack2.push(last_temp_pop2 + 1);
				}
			}
		}

		traverse_stack.push(temp_index);
		int last_temp_pop = temp_index;
		//for the current subtree of octree1, traverse to find the leaf pairs 
		while (!traverse_stack.empty()) //* AABB operator '>=' is included in '&' since the cube in higher level of octree1 must be larger than the leaf cubes in octree2
		{ 
			temp_index = traverse_stack.top();
			if ((*node_list)[temp_index].right_child_index == last_temp_pop ||
				(*node_list)[temp_index].is_pruned ||
				!oneToManyIntersect(node_list, node_list2, temp_index, collision_leaves2))
			{
				last_temp_pop = temp_index;
				traverse_stack.pop();
			}
			else
			{
				if ((*node_list)[temp_index].isLeaf())
				{
					for (int j = 0; j < collision_leaves2.size(); j++)
					{
						if ((*node_list)[temp_index].node_info & (*node_list2)[collision_leaves2[j]].node_info)
							collision_pairs.push_back(Vector2D(temp_index, collision_leaves2[j]));
					
					}
					
					last_temp_pop = temp_index;
					traverse_stack.pop();
				}
				else if (last_temp_pop <= temp_index) //depth first search
				{
					traverse_stack.push((*node_list)[temp_index].left_child_index);
				}
				else
				{
					traverse_stack.push(last_temp_pop + 1);
				}
			}
		}
	}

	return collision_pairs;
	// [A_600, B_601]
	//should return pairs of collision leaf BVs

	// -> return for primitive test

}

bool CollisionDetection::hybridUpdateDetection()
{
	return false;
}

bool CollisionDetection::isLeaf(AABBNode node)
{
	return false;
}

bool CollisionDetection::oneToManyIntersect(std::vector<AABBNode>* node_list, std::vector<AABBNode>* node_list2, int index1, std::vector<int> indices2)
{
	for(int i = 0; i < indices2.size(); i++)
	{
		if((*node_list)[index1].node_info & (*node_list2)[indices2[i]].node_info) return true;
	}
	return false;
}

bool CollisionDetection::isInCollisionSet(Vector2D collision_pair, std::vector<Vector2D> collision_set)
{
	for (int i = 0; i < collision_set.size(); i++)
	{
		if (collision_pair == collision_set[i]) return true;
	}
	return false;
}

unsigned int CollisionDetection::getBroadPhaseDetectionLevel()
{
	return bpd_lvl;
}

CollisionDetection::~CollisionDetection()
{
}
