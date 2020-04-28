#pragma once
#include <vector>
#include "SearchNode.h"
#include "GridTile.h"

class BoundaryNode
{
public:
	BoundaryNode();
	BoundaryNode(NeighbourIndex t_dir);
	BoundaryNode(std::vector<int>& t_boundary, NeighbourIndex t_dir);

	//private:
	std::vector<int> m_boundary;
	std::vector<int> m_eni; //Extened Neighbour Interval EG. [2,11] to [10,11]
	std::vector<SearchNode> m_fsi; //Free subinterval of ENI EG. [8,11] to [10,11]

	NeighbourIndex m_dir;
	float m_minfval;
	GridTile* m_previous = nullptr;
};