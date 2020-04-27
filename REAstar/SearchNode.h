#pragma once
#include <vector>
#include <string>
#include "Utils.h"
#include "GridTile.h"

class SearchNode
{
public:
	SearchNode(int t_start, int t_end, NeighbourIndex t_dir, int t_minfval);
	SearchNode(int t_start,NeighbourIndex t_dir);
	std::vector<int> m_interval;
	NeighbourIndex m_dir;
	float m_minfval;
	GridTile* m_minValTile = nullptr;
	GridTile* m_previous = nullptr;

	float getFval() const;
};