#pragma once
#include "MapGenerator.h"


class List
{
public:
	List();
	List(const List& list);
	~List();

	//count number of nodes in your list
	int Count(void);

	// This creates a node and adds the node to the back of the list
	void PushBack(Map &value);

	// This creates a node and adds the node to the front of the list
	void PushFront(Map &value);

	Map& GetHeadValue(void);

	Map& GetTailValue(void);

	void DeleteHead(void);

private:
	struct Node
	{
		Node(Map &value);
		Node(Map &value, Node* pNode);

		Map value;
		Node* pNext;
	};

	Node* pHead;
	Node* pTail;
};
