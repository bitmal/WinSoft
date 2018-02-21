#ifndef _BUFFER_H
#define _BUFFER_H

namespace WinSoft
{
	struct BufferObject
	{
		int _length;
		void* _data;
	};

	int CreateVBO(Vertex* vertices, int length);
	int CreateIBO(unsigned int* indices, int length);
	BufferObject* MapVBO(int id);
	BufferObject* MapIBO(int id);
	void DeleteVBO(int id);
	void DeleteIBO(int id);

	void DestroyBuffers();
}

#endif
