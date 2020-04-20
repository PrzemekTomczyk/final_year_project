#include "stdafx.h"
#include "RectNode.h"

RectNode::RectNode() :
	m_top(NeighbourIndex::TOP, this),
	m_bot(NeighbourIndex::BOTTOM, this),
	m_right(NeighbourIndex::RIGHT, this),
	m_left(NeighbourIndex::LEFT, this),
	//m_previous(nullptr),
	m_parentTile(nullptr)
{
	//m_minfval = std::numeric_limits<int>::max();
}