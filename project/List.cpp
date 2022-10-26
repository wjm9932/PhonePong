#include <iostream>
#include "List.h"
#include <cassert>

List::List() :pHead(nullptr), pTail(nullptr)
	{
	}

	List::~List()
	{
		Node* deleteNode = nullptr;
		while (pHead != nullptr)
		{
			deleteNode = pHead;
			pHead = pHead->pNext;
			delete deleteNode;
		}
	}

	//count number of nodes in your list
	int List::Count(void)
	{
		int count = 0;
		for (Node* tempNode = pHead; tempNode != nullptr; tempNode = tempNode->pNext)
		{
			++count;
		}

		return count;
	}

	void List::PushBack(Map &value)
	{
		if (pHead == nullptr && pTail == nullptr)
		{
			pHead = new Node(value);
			pTail = pHead;
			return;
		}
		pTail->pNext = new Node(value);
		pTail = pTail->pNext;
	}

	void List::PushFront(Map &value)
	{
		if (pHead == nullptr && pTail == nullptr)
		{
			pHead = new Node(value);
			pTail = pHead;
			return;
		}

		pHead = new Node(value, pHead);
	}


	Map& List::GetHeadValue(void)
	{
		return pHead->value;
	}

	Map& List::GetTailValue(void)
	{
		return pTail->value;
	}

	void List::DeleteHead(void)
	{
		Node* deleteNode = pHead;
		pHead = pHead->pNext;
		delete deleteNode;
	}

		List::Node::Node(Map &value)
			: value(value), pNext(nullptr)
		{
		}
		List::Node::Node(Map &value, Node* pNode)
			: value(value), pNext(pNode)
		{
		}
