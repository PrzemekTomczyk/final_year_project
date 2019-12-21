#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <queue>
#include <vector>
#include <functional>

template <class NodeType, class ArcType> class GraphArc;
template <class NodeType, class ArcType> class GraphNode;

template <class NodeType, class ArcType>
class NodeComparer
{
public:
	typedef GraphNode<NodeType, ArcType> Node;
	bool operator()(Node* n1, Node* n2)
	{
		return n1->getTotalDist() > n2->getTotalDist();
	}
};

// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    This is the graph class, it contains all the
//                  nodes.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
class Graph
{
public:
	// typedef the classes to make our lives easier.
	typedef GraphArc<NodeType, ArcType> Arc;
	typedef GraphNode<NodeType, ArcType> Node;


	// Constructor and destructor functions
	Graph(int size);
	~Graph();

	// Accessors
	Node* nodeIndex(int index) const {
		return m_nodes.at(index);
	}

	// Public member functions.
	bool addNode(NodeType data, int t_x, int t_y, int index);
	void removeNode(int index);
	bool addArc(int from, int to, ArcType weight);
	void removeArc(int from, int to);
	Arc* getArc(int from, int to);
	void clearAll();
	void aStar(Node* start, Node* dest, std::function<void(Node*)> f_visit, std::vector<Node*>& path);
private:
	// ----------------------------------------------------------------
	//  Description:    A container of all the nodes in the graph.
	// ----------------------------------------------------------------

	std::vector<Node*> m_nodes;
};

// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    Constructor, this constructs an empty graph
//  Arguments:      The maximum number of nodes.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::Graph(int maxNodes) : m_nodes(maxNodes, nullptr)
{
}

// ----------------------------------------------------------------
//  Name:           ~Graph
//  Description:    destructor, This deletes every node
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::~Graph() {
	for (int index = 0; index < m_nodes.size(); index++) {
		if (m_nodes[index] != nullptr) {
			delete m_nodes.at(index);
		}
	}
}

// ----------------------------------------------------------------
//  Name:           addNode
//  Description:    This adds a node at a given index in the graph.
//  Arguments:      The first parameter is the data to store in the node.
//					The second and third parameter are the x and y pos of the node
//                  The fourth parameter is the index to store the node.
//  Return Value:   true if successful
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addNode(NodeType data, int t_x, int t_y, int index) {
	bool nodeNotPresent = false;
	// find out if a node does not exist at that index.
	if (nullptr == m_nodes.at(index)) {
		nodeNotPresent = true;
		// create a new node, put the data in it, and unmark it.
		m_nodes.at(index) = new Node;
		m_nodes.at(index)->setData(data);
		m_nodes.at(index)->setPos(t_x, t_y);
		m_nodes.at(index)->setMarked(false);
	}

	return nodeNotPresent;
}

// ----------------------------------------------------------------
//  Name:           removeNode
//  Description:    This removes a node from the graph
//  Arguments:      The index of the node to return.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeNode(int index) {
	// Only proceed if node does exist.
	if (nullptr != m_nodes.at(index)) {
		// now find every arc that points to the node that
		// is being removed and remove it.        
		Arc* arc;

		// loop through every node
		for (int node = 0; node < m_nodes.size(); node++) {
			// if the node is valid...
			if (nullptr != m_nodes.at(node)) {
				// see if the node has an arc pointing to the current node.
				arc = m_nodes.at(node)->getArc(m_nodes.at(index));
			}
			// if it has an arc pointing to the current node, then
			// remove the arc.
			if (arc != 0) {
				removeArc(node, index);
			}
		}


		// now that every arc pointing to the current node has been removed,
		// the node can be deleted.
		delete m_nodes.at(index);
		m_nodes.at(index) = nullptr;
	}
}

// ----------------------------------------------------------------
//  Name:           addArc
//  Description:    Adds an arc from the first index to the 
//                  second index with the specified weight.
//  Arguments:      The first argument is the originating node index
//                  The second argument is the ending node index
//                  The third argument is the weight of the arc
//  Return Value:   true on success.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addArc(int from, int to, ArcType weight) {
	bool proceed = true;
	// make sure both nodes exist.
	if (nullptr == m_nodes.at(from) || nullptr == m_nodes.at(to)) {
		proceed = false;
	}

	// if an arc already exists we should not proceed
	if (m_nodes.at(from)->getArc(m_nodes.at(to)) != nullptr) {
		proceed = false;
	}

	if (proceed == true) {
		// add the arc to the "from" node.
		m_nodes.at(from)->addArc(m_nodes.at(to), weight);
	}

	return proceed;
}

// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This removes the arc from the first index to the second index
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeArc(int from, int to) {
	// Make sure that the node exists before trying to remove
	// an arc from it.
	bool nodeExists = true;
	if (nullptr == m_nodes.at(from) || nullptr == m_nodes.at(to)) {
		nodeExists = false;
	}

	if (nodeExists == true) {
		// remove the arc.
		m_nodes.at(from)->removeArc(m_nodes.at(to));
	}
}


// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    Gets a pointer to an arc from the first index
//                  to the second index.
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   pointer to the arc, or 0 if it doesn't exist.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
GraphArc<NodeType, ArcType>* Graph<NodeType, ArcType>::getArc(int from, int to) {
	Arc* arc = 0;
	// make sure the to and from nodes exist
	if (nullptr != m_nodes.at(from) && nullptr != m_nodes.at(to)) {
		arc = m_nodes.at(from)->getArc(m_nodes.at(to));
	}

	return arc;
}


// ----------------------------------------------------------------
//  Name:           clearMarks
//  Description:    This clears every mark on every node.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::clearAll() {
	for (int index = 0; index < m_nodes.size(); index++) {
		if (nullptr != m_nodes.at(index)) {
			m_nodes.at(index)->setMarked(false);
			m_nodes.at(index)->setPrevious(nullptr);
			m_nodes.at(index)->setVisited(false);
		}
	}
}

template<class NodeType, class ArcType>
inline void Graph<NodeType, ArcType>::aStar(Node* start, Node* dest, std::function<void(Node*)> f_visit, std::vector<Node*>& path)
{
	if (start != nullptr && dest != nullptr)
	{
		clearAll();

		// make priority queue
		std::priority_queue<Node*, std::vector<Node*>, NodeComparer<NodeType, ArcType>> pq;

		//for each node v in graph G
		for (int i = 0; i < m_nodes.size(); i++) {
			if (nullptr != m_nodes.at(i)) {
				// calc h[v] # compute estimated distance to goal for each node
				float estDist = std::pow(dest->getX() - m_nodes.at(i)->getX(), 2);
				estDist += std::pow(dest->getY() - m_nodes.at(i)->getY(), 2);
				estDist = std::sqrt(estDist);
				m_nodes.at(i)->setEstDist(estDist);

				// init g[v] to infinity # dont YET know the distance to these nodes
				m_nodes.at(i)->setCurrDist(std::numeric_limits<int>::max());
				m_nodes.at(i)->setTotalDist(std::numeric_limits<int>::max());
			}
		}

		// init g[s] to 0
		start->setCurrDist(0);
		start->setTotalDist(0);
		// mark s
		start->setMarked(true);

		// Add s to pq
		Node* current;
		pq.push(start);

		// while the !pq.empty() && pq.top() != goal node
		while (!pq.empty() && pq.top() != dest)
		{
			current = pq.top();
			current->setVisited(true);
			f_visit(current);

			//for each child node c of pq.top()
			auto iter = current->arcList().begin();
			auto endIter = current->arcList().end();

			//for each child node c of pq.top()
			for (; iter != endIter; iter++)
			{
				Node* child = (*iter).node();

				//if child != previous(pq.top())
				if (child != current->previous()) {
					//g(child) # g(c) is weight from current to child + distance of previous node
					int childCurrDist = current->getCurrDist() + current->getArc(child)->weight();//f(c)
					//let total child dist = h(child) + 
					int totalChildDist = child->getEstDist() + childCurrDist;

					if (totalChildDist < child->getTotalDist()) {
						// set current distance
						child->setCurrDist(childCurrDist);

						//let f[c] = total distance
						child->setTotalDist(totalChildDist);

						// set previous pointer of child to pq.top()
						child->setPrevious(current);
					}//end if

					if (!child->getMarked()) {
						//mark child
						child->setMarked(true);

						//add child to pq
						pq.push(child);
					}//end if
				}//end if
			}//end for

			//pop from pq
			pq.pop();

		}//end while

		if (dest->previous() != nullptr) {
			Node* ptr = dest;
			path.push_back(ptr);

			//add all nodes with previous to the path
			while (nullptr != ptr->previous())
			{
				ptr = ptr->previous();
				path.push_back(ptr);
			}
		}
	}//end if
}

#include "GraphNode.h"
#include "GraphArc.h"

#endif