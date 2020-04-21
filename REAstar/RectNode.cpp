#include "stdafx.h"
#include "RectNode.h"

RectNode::RectNode() :
	m_top(NeighbourIndex::TOP),
	m_bot(NeighbourIndex::BOTTOM),
	m_right(NeighbourIndex::RIGHT),
	m_left(NeighbourIndex::LEFT),
	//m_previous(nullptr),
	m_parentTile(nullptr)
{
	//m_minfval = std::numeric_limits<int>::max();
}