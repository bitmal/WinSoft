#ifndef _LIST_H
#define _LIST_H

namespace WinSoft
{
	struct Node
	{
		int nodeID;
		int listID;
		Node* next;
		void* data;
	};

	struct List
	{
		Node* _head;
		Node* _tail;
		int _id;
	};
	

	int CreateList();
	void DestroyList(int id);
	void Flush();

	void Insert(void* data, int listID)
	{

	}

	void Remove(int NodeID, int listID)
	{

	}

	void Find(int NodeID, int listID)
	{

	}
}


#endif
