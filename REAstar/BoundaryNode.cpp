#include "stdafx.h"
#include "BoundaryNode.h"

BoundaryNode::BoundaryNode(NeighbourIndex t_dir, RectNode* t_parent) :
	m_previous(nullptr)
{
	m_dir = t_dir;
	m_parentRect = t_parent;
	m_minfval = std::numeric_limits<int>::max();
}

BoundaryNode::BoundaryNode(std::vector<int>& t_boundary, NeighbourIndex t_dir, RectNode* t_parent)/* : 
	m_previo*/
{
	m_boundary = t_boundary;
	m_dir = t_dir;
	m_parentRect = t_parent;
}
