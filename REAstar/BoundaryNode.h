#pragma once
#include <vector>
#include <string>
#include "Utils.h"
#include "SearchNode.h"
#include "GridTile.h"

//class RectNode;
class BoundaryNode
{
public:
	BoundaryNode();
	BoundaryNode(NeighbourIndex t_dir/*, RectNode* t_parent*/);
	//BoundaryNode(std::vector<int>& t_boundary, NeighbourIndex t_dir/*, RectNode* t_parent*/);
	BoundaryNode(std::vector<int>& t_boundary, NeighbourIndex t_dir);

	//private:
	std::vector<int> m_boundary;
	std::vector<int> m_eni; //Extened Neighbour Interval EG. [2,11] to [10,11]
	std::vector<SearchNode> m_fsi; //Free subinterval of ENI EG. [8,11] to [10,11]

	NeighbourIndex m_dir;
	float m_minfval;
	//RectNode* m_parentRect;
	GridTile* m_previous = nullptr;
};