#include "stdafx.h"
#include "SearchNode.h"

SearchNode::SearchNode(int t_start, int t_end, NeighbourIndex t_dir, int t_minfval)
{
	m_interval.push_back(t_start);
	m_interval.push_back(t_end);
	m_dir = t_dir;
	m_minfval = t_minfval;
}

SearchNode::SearchNode(int t_start, NeighbourIndex t_dir)
{
	m_interval.push_back(t_start);
	m_dir = t_dir;
	m_minfval = std::numeric_limits<int>::max();
}

float SearchNode::getFval() const
{
	return m_minfval;
}
