#include "stdafx.h"

WinSoft::BufferObject* _vboList = NULL;
WinSoft::BufferObject* _iboList = NULL;
int _vboCount = 0;
int _iboCount = 0;
// TODO: used for deletes of buffers to account for prerecorded buffer objects
// ie. See DeleteBuffer functions
int _bufferOffset = 0;

int WinSoft::CreateVBO(Vertex* vertices, int length)
{
	BufferObject* buffer = (BufferObject*)malloc(sizeof(BufferObject));
	buffer->_length = length;
	buffer->_data = malloc(sizeof(Vertex)*length);

	Vertex* data = (Vertex*)buffer->_data;
	for (int i = 0; i < length; ++i)
	{
		data[i] = vertices[i];
	}

	if (!_vboList)
	{
		_vboList = (BufferObject*)malloc(sizeof(BufferObject));
	}
	else
	{
		_vboList = (BufferObject*)realloc(_vboList, sizeof(BufferObject)*(_vboCount + 1));
	}

	_vboList[_vboCount++] = *buffer;

	return _vboCount - 1;
}

int WinSoft::CreateIBO(unsigned int* indices, int length)
{
	// TODO:
	BufferObject* buffer = (BufferObject*)malloc(sizeof(BufferObject));
	buffer->_length = length;
	buffer->_data = malloc(sizeof(unsigned int)*length);

	unsigned int* data = (unsigned int*)buffer->_data;
	for (int i = 0; i < length; ++i)
	{
		data[i] = indices[i];
	}

	if (!_iboList)
	{
		_iboList = (BufferObject*)malloc(sizeof(BufferObject));
	}
	else
	{
		_iboList = (BufferObject*)realloc(_iboList, sizeof(BufferObject)*(_iboCount + 1));
	}

	_iboList[_iboCount++] = *buffer;

	return _iboCount - 1;
}

WinSoft::BufferObject* WinSoft::MapVBO(int id)
{
	return _vboCount > 0 && id > NOT_AN_OBJECT && id < _vboCount ? &_vboList[id] : NULL;
}

WinSoft::BufferObject* WinSoft::MapIBO(int id)
{
	return _iboCount > 0 && id > NOT_AN_OBJECT && id < _iboCount ? &_iboList[id] : NULL;
}

void WinSoft::DeleteVBO(int id)
{
	// TODO:
}

void WinSoft::DeleteIBO(int id)
{
	// TODO:
}

void WinSoft::DestroyBuffers()
{
	if (_iboList)
	{
		for (int i = 0; i < _iboCount; ++i)
		{
			if (_iboList[i]._data)
			{
				free(_iboList[i]._data);
				_iboList[i]._data = NULL;
			}
		}

		free(_iboList);
		_iboList = NULL;
	}

	if (_vboList)
	{
		for (int i = 0; i < _vboCount; ++i)
		{
			if (_vboList[i]._data)
			{
				free(_vboList[i]._data);
				_vboList[i]._data = NULL;
			}
		}

		free(_vboList);
		_vboList = NULL;
	}
}
