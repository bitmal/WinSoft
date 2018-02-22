#ifndef _BUFFER_H
#define _BUFFER_H

namespace WinSoft
{
	enum BufferStatus : int
	{
		UNKNOWN, ACTIVE, INVALIDATED
	};

	struct BufferObject
	{
		BufferStatus _status;
		int _length;
		void* _data;
	};

	int CreateVBO(Vertex* vertices, int length);
	int CreateIBO(unsigned int* indices, int length);
	BufferObject* MapVBO(int id);
	BufferObject* MapIBO(int id);
	void InvalidateVBO(int id);
	void InvalidateIBO(int id);
	BufferStatus VBOBufferStatus(int id);
	BufferStatus IBOBufferStatus(int id);

	void FlushBufferMemory();
}

#endif
