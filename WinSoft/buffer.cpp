#include "stdafx.h"

WinSoft::BufferObject* _vboList = NULL;
WinSoft::BufferObject* _iboList = NULL;
int _vboCount = 0;
int _iboCount = 0;
int _vboCapacity = 0;
int _iboCapacity = 0;

int WinSoft::CreateVBO(Vertex* vertices, int length)
{	
	int id = NOT_AN_OBJECT;	

	if (!_vboList)
	{
		_vboList = (BufferObject*)malloc(sizeof(BufferObject));
		_vboList->_status = ACTIVE;
		_vboList->_length = length;
		_vboList->_data = malloc(sizeof(Vertex)*length);

		for (int i = 0; i < length; ++i)
		{
			((Vertex*)_vboList->_data)[i] = vertices[i];
		}

		id = _vboCapacity;

		++_vboCapacity;
		++_vboCount;
	}
	else
	{
		if (_vboCount >= _vboCapacity)
		{
			_vboList = (BufferObject*)realloc(_vboList, sizeof(BufferObject)*(_vboCapacity + 1));
			_vboList[_vboCapacity]._status = ACTIVE;
			_vboList[_vboCapacity]._length = length;
			_vboList[_vboCapacity]._data = malloc(sizeof(Vertex)*length);

			for (int i = 0; i < length; ++i)
			{
				((Vertex*)_vboList[_vboCapacity]._data)[i] = vertices[i];
			}

			id = _vboCapacity;

			++_vboCount;
			++_vboCapacity;
		}
		else
		{
			bool success = false;
			
			// look for free slot
			for (int i = 0; i < _vboCapacity; ++i)
			{
				if (_vboList[i]._status == INVALIDATED)
				{
					_vboList[i]._status = ACTIVE;
					_vboList[i]._length = length;
					_vboList[i]._data = (Vertex*)malloc(sizeof(Vertex)*(length));
					id = i;

					for (int j = 0; j < length; ++j)
					{
						((Vertex*)_vboList[id]._data)[j] = vertices[j];
					}

					++_vboCount;

					success = true;
					break;
				}
			}

			if (!success)
			{
				fprintf(stderr, "Failure to find invalidated block to allocate vertex buffer: %s:: %s:: %d\n", __FILE__, __func__, __LINE__);
			}
		}
	}

	return id;
}

int WinSoft::CreateIBO(unsigned int* indices, int length)
{
	int id = NOT_AN_OBJECT;

	if (!_iboList)
	{
		_iboList = (BufferObject*)malloc(sizeof(BufferObject));
		_iboList->_status = ACTIVE;
		_iboList->_length = length;
		_iboList->_data = malloc(sizeof(unsigned int)*length);

		for (int i = 0; i < length; ++i)
		{
			((unsigned int*)_iboList->_data)[i] = indices[i];
		}

		id = _iboCapacity;

		++_iboCapacity;
		++_iboCount;
	}
	else
	{
		if (_iboCount >= _iboCapacity)
		{
			_iboList = (BufferObject*)realloc(_iboList, sizeof(BufferObject)*(_iboCapacity + 1));
			_iboList[_iboCapacity]._status = ACTIVE;
			_iboList[_iboCapacity]._length = length;
			_iboList[_iboCapacity]._data = malloc(sizeof(unsigned int)*length);

			for (int i = 0; i < length; ++i)
			{
				((unsigned int*)_iboList[_iboCapacity]._data)[i] = indices[i];
			}

			id = _iboCapacity;

			++_iboCount;
			++_iboCapacity;
		}
		else
		{
			bool success = false;

			// look for free slot
			for (int i = 0; i < _iboCapacity; ++i)
			{
				if (_iboList[i]._status == INVALIDATED)
				{
					_iboList[i]._status = ACTIVE;
					_iboList[i]._length = length;
					_iboList[i]._data = (unsigned int*)malloc(sizeof(unsigned int)*(length));
					id = i;

					for (int j = 0; j < length; ++j)
					{
						((unsigned int*)_iboList[id]._data)[j] = indices[j];
					}

					++_iboCount;

					success = true;
					break;
				}
			}

			if (!success)
			{
				fprintf(stderr, "Failure to find invalidated block to allocate index buffer: %s:: %s:: %d\n", __FILE__, __func__, __LINE__);
			}
		}
	}

	return id;
}

WinSoft::BufferObject* WinSoft::MapVBO(int id)
{
	return _vboCount > 0 && id > NOT_AN_OBJECT && id < _vboCapacity && _vboList[id]._status == ACTIVE ? &_vboList[id] : NULL;
}

WinSoft::BufferObject* WinSoft::MapIBO(int id)
{
	return _iboCount > 0 && id > NOT_AN_OBJECT && id < _iboCapacity && _iboList[id]._status == ACTIVE ? &_iboList[id] : NULL;
}

void WinSoft::InvalidateVBO(int id)
{
	if (_vboCapacity > 0 && id < _vboCapacity && _vboList[id]._status == ACTIVE)
	{
		_vboList[id]._status = INVALIDATED;
		_vboList[id]._length = 0;

		if (_vboList[id]._data)
		{
			free(_vboList[id]._data);
		}

		--_vboCount;
	}
	else
	{
		fprintf(stderr, "Cannot invalidate vertex buffer. May not exist, or may have been previously invalidated: %s:: %s:: %d\n", __FILE__, __func__, __LINE__);
	}
}

void WinSoft::InvalidateIBO(int id)
{
	if (_iboCapacity > 0 && id < _iboCapacity && _iboList[id]._status == ACTIVE)
	{
		_iboList[id]._status = INVALIDATED;
		_iboList[id]._length = 0;

		if (_iboList[id]._data)
		{
			free(_iboList[id]._data);
		}

		--_iboCount;
	}
	else
	{
		fprintf(stderr, "Cannot invalidate index buffer. May not exist, or may have been previously invalidated: %s:: %s:: %d\n", __FILE__, __func__, __LINE__);
	}
}

WinSoft::BufferStatus WinSoft::VBOBufferStatus(int id)
{
	return _vboCapacity > 0 && id > NOT_AN_OBJECT && id < _vboCapacity ? _vboList[id]._status : UNKNOWN;
}

WinSoft::BufferStatus WinSoft::IBOBufferStatus(int id)
{
	return _iboCapacity > 0 && id > NOT_AN_OBJECT && id < _iboCapacity ? _iboList[id]._status : UNKNOWN;
}

void WinSoft::FlushBufferMemory()
{
	if (_iboList)
	{
		for (int i = 0; i < _iboCapacity; ++i)
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
		for (int i = 0; i < _vboCapacity; ++i)
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

	_vboCapacity = _vboCount = _iboCapacity = _iboCount = 0;
}
