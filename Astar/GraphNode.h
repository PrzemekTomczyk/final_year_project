#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <list>

// Forward references
template <typename NodeType, typename ArcType> class GraphArc;

// -------------------------------------------------------
// Name:        GraphNode
// Description: This is the node class. The node class 
//              contains data, and has a linked list of 
//              arcs.
// -------------------------------------------------------
template<class NodeType, class ArcType>
class GraphNode {
public:
	// typedef the classes to make our lives easier.
	typedef GraphArc<NodeType, ArcType> Arc;
	typedef GraphNode<NodeType, ArcType> Node;

	// Constructor function
	GraphNode(Node * previous = 0);

    // Accessor functions
    list<Arc> const & arcList() const {
        return m_arcList;              
    }

    bool getMarked() const {
        return m_marked;
    }

    NodeType const & data() const {
        return m_data;
    }

	Node * previous() const {
		return m_previous;
	}

    // Manipulator functions
    void setData(NodeType data) {
        m_data = data;
    }

	void setPos(int t_x, int t_y) {
		m_xpos = t_x;
		m_ypos = t_y;
	}

	float getEstToGoal() {
		return m_estDist;
	}
    
    void setMarked(bool mark) {
        m_marked = mark;
    }

	void setVisited(bool t_vist) {
		m_visted = t_vist;
	}

	bool getVisited() {
		return m_visted;
	}
           
	void setPrevious(Node *previous) {
		m_previous = previous;
	}

    Arc* getArc( Node* pNode );    
    void addArc( Node* pNode, ArcType pWeight );
    void removeArc( Node* pNode );

	float getEstDist();
	float getCurrDist();
	float getTotalDist();

	void setEstDist(float t_val);
	void setCurrDist(float t_val);
	void setTotalDist(float t_val);


	void printPreviousPath();

	int getX() { return m_xpos; };
	int getY() { return m_ypos; };

private:
	
	// -------------------------------------------------------
	// Description: data inside the node
	// -------------------------------------------------------
	NodeType m_data;

	int m_xpos;
	int m_ypos;

	// current distance from start to this node
	float m_currDist;

	// estimated distance from this node to goal
	float m_estDist;

	// current distance + estimated distance
	float m_totalDist;

	// -------------------------------------------------------
	// Description: list of arcs that the node has.
	// -------------------------------------------------------
	list<Arc> m_arcList;

	// -------------------------------------------------------
	// Description: This remembers if the node is marked.
	// -------------------------------------------------------
	bool m_marked;
	bool m_visted;

	// -------------------------------------------------------
	// Description: pointer to previous node
	// -------------------------------------------------------
	Node *m_previous;
};


// Constructor function
template<class NodeType, class ArcType>
inline GraphNode<NodeType, ArcType>::GraphNode(Node * previous) : m_previous(previous) 
{
	m_marked = false;
}

// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    This finds the arc in the current node that
//                  points to the node in the parameter.
//  Arguments:      The node that the arc connects to.
//  Return Value:   A pointer to the arc, or 0 if an arc doesn't
//                  exist from this to the specified input node.
// ----------------------------------------------------------------
template<typename NodeType, typename ArcType>
GraphArc<NodeType, ArcType>* GraphNode<NodeType, ArcType>::getArc( Node* node ) {

     auto iter = m_arcList.begin();
     auto endIter = m_arcList.end();
     Arc* arc = 0;
     
     // find the arc that matches the node
     for( ; iter != endIter && nullptr == arc; ++iter ) {         
          if ( (*iter).node() == node) {
               arc = &( (*iter) );
          }
     }

     // returns nullptr if not found
     return arc;
}

// ----------------------------------------------------------------
//  Name:           addArc
//  Description:    This adds an arc from the current node pointing
//                  to the first parameter, with the second parameter 
//                  as the weight.
//  Arguments:      First argument is the node to connect the arc to.
//                  Second argument is the weight of the arc.
//  Return Value:   None.
// ----------------------------------------------------------------
template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::addArc( Node* node, ArcType weight ) {
   // Create a new arc.
   Arc a;
   a.setNode(node);
   a.setWeight(weight);   
   // Add it to the arc list.
   m_arcList.push_back( a );
}


// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This finds an arc from this node to input node 
//                  and removes it.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::removeArc( Node* node ) {
     auto iter = m_arcList.begin();
     auto endIter = m_arcList.end();

     int size = m_arcList.size();
     // find the arc that matches the node
     for( ; iter != endIter && m_arcList.size() == size;  
                                                    ++iter ) {
          if ( (*iter).node() == node) {
             m_arcList.remove( (*iter) );
          }                           
     }
}

template<class NodeType, class ArcType>
inline float GraphNode<NodeType, ArcType>::getEstDist()
{
	return m_estDist;
}

template<class NodeType, class ArcType>
inline float GraphNode<NodeType, ArcType>::getCurrDist()
{
	return m_currDist;
}

template<class NodeType, class ArcType>
inline void GraphNode<NodeType, ArcType>::setEstDist(float t_val)
{
	m_estDist = t_val;
}

template<class NodeType, class ArcType>
inline void GraphNode<NodeType, ArcType>::setCurrDist(float t_val)
{
	m_currDist = t_val;
}

template<class NodeType, class ArcType>
inline void GraphNode<NodeType, ArcType>::setTotalDist(float t_val)
{
	m_totalDist = t_val;
}

template<class NodeType, class ArcType>
inline float GraphNode<NodeType, ArcType>::getTotalDist()
{
	return m_totalDist;
}

template<class NodeType, class ArcType>
inline void GraphNode<NodeType, ArcType>::printPreviousPath()
{
	std::cout << "Index: " << m_data << std::endl;

	if (m_previous != nullptr)
	{
		m_previous->printPreviousPath();
	}
}

#include "GraphArc.h"

#endif
