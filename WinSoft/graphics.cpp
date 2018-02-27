#include "stdafx.h"

#include <cmath>
#include <cstdlib>

void _UpdateIntersections(int objectID);

WinSoft::Object* _objects = NULL;
WinSoft::ScanLine* _scanLines = NULL;
int*  _drawQueue = NULL;
int _objectCount = 0;
int _scanLineCount = 0;

bool _objectIsDirty = false;

void _UpdateIntersections(int objectID)
{
	// object exists
	if (!WinSoft::ObjectExists(objectID))
		return;

	WinSoft::Object& object = _objects[objectID];
	WinSoft::BufferObject* vbo = WinSoft::MapVBO(object._vbo);
	WinSoft::BufferObject* ibo = WinSoft::MapVBO(object._ibo);

	// for each triangle of the object, calculate each intersection
	// and insert each into list left to right, top to bottom
	struct Prim
	{
		WinSoft::Vertex* a;
		WinSoft::Vertex* b;
		WinSoft::Vertex* c;
	};	

	int primCount = (ibo->_length - 2) * ibo->_length;

	for (int i = 0; i < primCount; ++i)
	{
		Prim prim{};
		prim.a = &((WinSoft::Vertex*)vbo->_data)[((unsigned int*)ibo->_data)[i * 2]];
		prim.b = &((WinSoft::Vertex*)vbo->_data)[((unsigned int*)ibo->_data)[i * 2] + 1];
		prim.c = &((WinSoft::Vertex*)vbo->_data)[((unsigned int*)ibo->_data)[i * 2] + 2];

		// figure out where to place the primitive in the list
		WinSoft::ScanLine* scanLine = NULL;

		for (int j = 0; j < _scanLineCount; ++j)
		{
			WinSoft::ScanLine& line = _scanLines[j];
			bool onLine = false;

			if (line._vertexCount > 0)
			{

			}
		}

		if (scanLine != NULL)
		{

		}
		else
		{
			if (_scanLines == NULL)
			{

			}
			else
			{

			}
		}
	}
}

void WinSoft::RefreshSurface(WinSoft::Surface surface, WinSoft::FColor32 color)
{
	int width = (int)(surface._rect._topRight._x);
	int height = (int)(surface._rect._topRight._y);

	Color32 c = 0;
	ToColor(color, c);

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int index = (x + (y * width))*sizeof(c);

			BYTE* pixel = ((BYTE*)surface._data);
			pixel[index+3] = c;
			pixel[index+2] = c >> 8;
			pixel[index+1] = c >> 16;
			pixel[index] = c >> 24;
		}
	}
}

void WinSoft::DrawLine(Vertex a, Vertex b, WinSoft::Surface surface)
{
	// TODO: Bresenham's for more efficiency
	// TODO: Wu's line antialiasing
	
	/* Line Midpoint Algorithm */
	int dx = (int)b._point._x - (int)a._point._x;
	int dy = (int)b._point._y - (int)a._point._y;
	int dxAbs = abs(dx);
	int dyAbs = abs(dy);
	float magnitude = Magnitude(a._point, b._point);

	// Edge Case: zero magnitude
	if (!magnitude)
	{
		printf("DrawLine(line: %d): no magnitude\n", __LINE__);
		return;
	}		

	// Edge Case: 1 row
	if (!dyAbs)
	{		
		for (int x = (int)a._point._x; dx>0 ? x<=(int)b._point._x : x>=(int)b._point._x; dx>0 ? ++x : --x)
		{			
			int y = (int)a._point._y;
			
			float distance = Magnitude(a._point, Point{(float)x, (float)y});			
			FColor32 lerp = LerpColor(a._color, b._color, distance/magnitude);
			Color32 c;
			ToColor(lerp, c);

			int index = (int)(x + (y*surface._rect._topRight._x))*sizeof(c);

			BYTE* pixel = (BYTE*)surface._data;
			pixel[index+3] = c;
			pixel[index+2] = c >> 8;
			pixel[index+1] = c >> 16;
			pixel[index] = c >> 24;
		}

		return;
	}

	// Edge Case: 1 column
	if (!dxAbs)
	{
		for (int y = (int)a._point._y; dy>0 ? y<=(int)b._point._y : y>=(int)b._point._y; dy>0 ? ++y : --y)
		{
			int x = (int)a._point._x;

			float distance = Magnitude(a._point, Point{(float)x, (float)y});			
			FColor32 lerp = LerpColor(a._color, b._color, distance/magnitude);
			Color32 c;
			ToColor(lerp, c);

			int index = (int)(x + (y*surface._rect._topRight._x))*sizeof(c);

			BYTE* pixel = (BYTE*)surface._data;
			pixel[index+3] = c;
			pixel[index+2] = c >> 8;
			pixel[index+1] = c >> 16;
			pixel[index] = c >> 24;
		}

		return;
	}

	// Edge Case: columns == rows
	if (dxAbs == dyAbs)
	{
		for (int delta = 0; delta <= dxAbs; ++delta)
		{
			int x = (int)a._point._x + (dx > 0 ? delta : -delta);
			int y = (int)a._point._y + (dy > 0 ? delta : -delta);

			float distance = Magnitude(a._point, Point{(float)x, (float)y});			
			FColor32 lerp = LerpColor(a._color, b._color, distance/magnitude);
			Color32 c;
			ToColor(lerp, c);

			int index = (int)(x + (y*surface._rect._topRight._x))*sizeof(c);

			BYTE* pixel = (BYTE*)surface._data;
			pixel[index+3] = c;
			pixel[index+2] = c >> 8;
			pixel[index+1] = c >> 16;
			pixel[index] = c >> 24;
		}

		return;
	}

	// Edge Case: columns != rows
	// QUADRANT: UP-RIGHT
	if (dx > 0 && dy > 0)
	{
		// COLUMN-MAJOR
		if (dx > dy)
		{			
			for (int x = (int)a._point._x; x <= (int)b._point._x; ++x)
			{
				int y = lroundf((float)dy/dx*(x-a._point._x) + a._point._y);

				float distance = Magnitude(a._point, Point{(float)x, (float)y});	
				FColor32 lerp = LerpColor(a._color, b._color, distance/magnitude);
				Color32 c;
				ToColor(lerp, c);

				int index = (int)(x + (y *surface._rect._topRight._x))*sizeof(c);

				BYTE* pixel = (BYTE*)surface._data;
				pixel[index+3] = c;
				pixel[index+2] = c >> 8;
				pixel[index+1] = c >> 16;
				pixel[index] = c >> 24;
			}
		}
		// ROW-MAJOR
		else if (dy > dx)
		{
			for (int y = (int)a._point._y; y <= (int)b._point._y; ++y)
			{
				int x = lroundf((float)dx/dy*(y-a._point._y) + a._point._x);

				float distance = Magnitude(a._point, Point{(float)x, (float)y});
				FColor32 lerp = LerpColor(a._color, b._color, distance/magnitude);
				Color32 c;
				ToColor(lerp, c);

				int index = (int)(x + (y *surface._rect._topRight._x))*sizeof(c);

				BYTE* pixel = (BYTE*)surface._data;
				pixel[index+3] = c;
				pixel[index+2] = c >> 8;
				pixel[index+1] = c >> 16;
				pixel[index] = c >> 24;
			}
		}
	}
	// QUADRANT: UP-LEFT
	else if (dx < 0 && dy > 0)
	{
		// COLUMN-MAJOR
		if (dxAbs > dy)
		{
			for (int x = (int)a._point._x; x >= (int)b._point._x; --x)
			{
				int y = lroundf((float)dy/dx*(x-a._point._x) + a._point._y);

				float distance = Magnitude(a._point, Point{(float)x, (float)y});			
				FColor32 lerp = LerpColor(a._color, b._color, distance/magnitude);
				Color32 c;
				ToColor(lerp, c);

				int index = (int)(x + (y *surface._rect._topRight._x))*sizeof(c);

				BYTE* pixel = (BYTE*)surface._data;
				pixel[index+3] = c;
				pixel[index+2] = c >> 8;
				pixel[index+1] = c >> 16;
				pixel[index] = c >> 24;
			}
		}
		// ROW-MAJOR
		else if (dy > dxAbs)
		{			
			for (int y = (int)a._point._y; y <= (int)b._point._y; ++y)
			{
				int x = lroundf((float)dx/dy*(y-a._point._y) + a._point._x);

				float distance = Magnitude(a._point, Point{(float)x, (float)y});			
				FColor32 lerp = LerpColor(a._color, b._color, distance/magnitude);
				Color32 c;
				ToColor(lerp, c);

				int index = (int)(x + (y *surface._rect._topRight._x))*sizeof(c);

				BYTE* pixel = (BYTE*)surface._data;
				pixel[index+3] = c;
				pixel[index+2] = c >> 8;
				pixel[index+1] = c >> 16;
				pixel[index] = c >> 24;
			}
		}
	}	
	// QUADRANT: DOWN-LEFT
	else if (dx < 0 && dy < 0)
	{
		// COLUMN-MAJOR
		if (dx < dy)
		{
			for (int x = (int)a._point._x; x >= (int)b._point._x; --x)
			{
				int y = lroundf((float)dy/dx*(x-a._point._x) + a._point._y);

				float distance = Magnitude(a._point, Point{(float)x, (float)y});			
				FColor32 lerp = LerpColor(a._color, b._color, distance/magnitude);
				Color32 c;
				ToColor(lerp, c);

				int index = (int)(x + (y *surface._rect._topRight._x))*sizeof(c);

				BYTE* pixel = (BYTE*)surface._data;
				pixel[index+3] = c;
				pixel[index+2] = c >> 8;
				pixel[index+1] = c >> 16;
				pixel[index] = c >> 24;
			}
		}
		// ROW-MAJOR
		else if (dy < dx)
		{
			for (int y = (int)a._point._y; y >= (int)b._point._y; --y)
			{
				int x = lroundf((float)dx/dy*(y-a._point._y) + a._point._x);

				float distance = Magnitude(a._point, Point{(float)x, (float)y});			
				FColor32 lerp = LerpColor(a._color, b._color, distance/magnitude);
				Color32 c;
				ToColor(lerp, c);

				int index = (int)(x + (y *surface._rect._topRight._x))*sizeof(c);

				BYTE* pixel = (BYTE*)surface._data;
				pixel[index+3] = c;
				pixel[index+2] = c >> 8;
				pixel[index+1] = c >> 16;
				pixel[index] = c >> 24;
			}
		}
	}
	// QUADRANT: DOWN-RIGHT
	else if (dx > 0 && dy < 0)
	{
		// COLUMN-MAJOR
		if (dx > dyAbs)
		{
			for (int x = (int)a._point._x; x <= (int)b._point._x; ++x)
			{
				int y = lroundf((float)dy/dx*(x-a._point._x) + a._point._y);

				float distance = Magnitude(a._point, Point{(float)x, (float)y});			
				FColor32 lerp = LerpColor(a._color, b._color, distance/magnitude);
				Color32 c;
				ToColor(lerp, c);

				int index = (int)(x + (y *surface._rect._topRight._x))*sizeof(c);

				BYTE* pixel = (BYTE*)surface._data;
				pixel[index+3] = c;
				pixel[index+2] = c >> 8;
				pixel[index+1] = c >> 16;
				pixel[index] = c >> 24;
			}
		}
		// ROW-MAJOR
		else if (dyAbs > dx)
		{
			for (int y = (int)a._point._y; y >= (int)b._point._y; --y)
			{
				int x = lroundf((float)dx/dy*(y-a._point._y) + a._point._x);

				float distance = Magnitude(a._point, Point{(float)x, (float)y});			
				FColor32 lerp = LerpColor(a._color, b._color, distance/magnitude);
				Color32 c;
				ToColor(lerp, c);

				int index = (int)(x + (y *surface._rect._topRight._x))*sizeof(c);

				BYTE* pixel = (BYTE*)surface._data;
				pixel[index+3] = c;
				pixel[index+2] = c >> 8;
				pixel[index+1] = c >> 16;
				pixel[index] = c >> 24;
			}
		}
	}
}

void WinSoft::DrawRect(Rect rect, FColor32 color, Surface surface)
{
	float lX = rect._bottomLeft._x;
	float rX = rect._topRight._x;
	float bY = rect._bottomLeft._y; 
	float tY = rect._topRight._y;

	Vertex bottomLeft { Point{lX, bY}, color };
	Vertex topLeft { Point{lX, tY}, color };
	Vertex topRight { Point{rX, tY}, color };
	Vertex bottomRight { Point{rX, bY}, color };

	DrawLine(bottomLeft, topLeft, surface);
	DrawLine(topLeft, topRight, surface);
	DrawLine(topRight, bottomRight, surface);
	DrawLine(bottomRight, bottomLeft, surface);
}

void WinSoft::FillRect(Rect rect, const ColorBorder* border, FColor32 fillColor, Surface surface)
{
	Color32 bC = 0;
	Color32 fC = 0;
	Color32 color = 0;

	ToColor(border->_color, bC);
	ToColor(fillColor, fC);

	int lX = (int)rect._bottomLeft._x - (border->_type == ColorBorder::OUTSET ? border->_width: 0);
	int rX = (int)rect._topRight._x + (border->_type == ColorBorder::OUTSET ? border->_width : 0);
	int bY = (int)rect._bottomLeft._y - (border->_type == ColorBorder::OUTSET ? border->_width : 0);
	int tY = (int)rect._topRight._y + (border->_type == ColorBorder::OUTSET ? border->_width : 0);

	for (int y = bY; y <= tY; ++y)
	{
		for (int x = lX; x <= rX; ++x)
		{
			int index = (int)(x + (y * (surface._rect._topRight._x)))*sizeof(color);
			color = fC;

			if (border->_type != border->NONE)
			{
				if ((x <= (lX + border->_width) && x >= lX) ||
					(x >= (rX - border->_width) && x <= rX) ||
					(y <= (bY + border->_width) && y >= bY) ||
					(y >= (tY - border->_width) && y <= tY))
				{
					color = bC;
				}
			}

			BYTE* pixel = ((BYTE*)surface._data);
			pixel[index+3] = color;
			pixel[index+2] = color >> 8;
			pixel[index+1] = color >> 16;
			pixel[index] = color >> 24;
		}
	}
}

void WinSoft::DrawCircle(Point center, float radius, FColor32 color, Surface surface)
{
	/*float maxY = center._y + DEGREES_TO_RADIANS(45)*radius;
	float x = center._x;
	float y = center._y;

	Color32 col = 0;
	ToColor(color, col);

	for(int deg = 0; deg <= 45; ++deg)
	{
		y = DEGREES_TO_RADIANS(deg)*radius;
		x = sqrt((radius*radius)-(y*y));
		x = roundf(x+center._x);
		y = roundf(y+center._y);

		int index = (x + (y * ((int)surface._rect._topRight._x)))*sizeof(col);
		printf("%f, %f\n", x, y);

		BYTE* pixel = ((BYTE*)surface._data);
		pixel[index+3] = col;
		pixel[index+2] = col >> 8;
		pixel[index+1] = col >> 16;
		pixel[index] = col >> 24;
	}*/
	
	Color32 col = 0;
	ToColor(color, col);

	// Circle-Midpoint Algorithm
	int x = (int)radius-1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - ((int)radius << 1);

	while (x >= y)
	{
		int index = (int)(center._x+x + ((center._y+y) * (surface._rect._topRight._x)))*sizeof(col);
		BYTE* pixel = ((BYTE*)surface._data);
		pixel[index+3] = col;
		pixel[index+2] = col >> 8;
		pixel[index+1] = col >> 16;
		pixel[index] = col >> 24;

		index = (int)(center._x+y + ((center._y+x) * (surface._rect._topRight._x)))*sizeof(col);
		pixel = ((BYTE*)surface._data);
		pixel[index+3] = col;
		pixel[index+2] = col >> 8;
		pixel[index+1] = col >> 16;
		pixel[index] = col >> 24;

		index = (int)(center._x-y + ((center._y+x) * (surface._rect._topRight._x)))*sizeof(col);
		pixel = ((BYTE*)surface._data);
		pixel[index+3] = col;
		pixel[index+2] = col >> 8;
		pixel[index+1] = col >> 16;
		pixel[index] = col >> 24;

		index = (int)(center._x-x + ((center._y+y) * (surface._rect._topRight._x)))*sizeof(col);
		pixel = ((BYTE*)surface._data);
		pixel[index+3] = col;
		pixel[index+2] = col >> 8;
		pixel[index+1] = col >> 16;
		pixel[index] = col >> 24;

		index = (int)(center._x-x + ((center._y-y) * (surface._rect._topRight._x)))*sizeof(col);
		pixel = ((BYTE*)surface._data);
		pixel[index+3] = col;
		pixel[index+2] = col >> 8;
		pixel[index+1] = col >> 16;
		pixel[index] = col >> 24;

		index = (int)(center._x-y + ((center._y-x) * (surface._rect._topRight._x)))*sizeof(col);
		pixel = ((BYTE*)surface._data);
		pixel[index+3] = col;
		pixel[index+2] = col >> 8;
		pixel[index+1] = col >> 16;
		pixel[index] = col >> 24;

		index = (int)(center._x+y + ((center._y-x) * (surface._rect._topRight._x)))*sizeof(col);
		pixel = ((BYTE*)surface._data);
		pixel[index+3] = col;
		pixel[index+2] = col >> 8;
		pixel[index+1] = col >> 16;
		pixel[index] = col >> 24;

		index = (int)(center._x+x + ((center._y-y) * (surface._rect._topRight._x)))*sizeof(col);
		pixel = ((BYTE*)surface._data);
		pixel[index+3] = col;
		pixel[index+2] = col >> 8;
		pixel[index+1] = col >> 16;
		pixel[index] = col >> 24;

		if (err <= 0)
		{
			y++;
			err += dy;
			dy += 2;
		}

		if (err > 0)
		{
			x--;
			dx += 2;
			err += dx - ((int)radius << 1);
		}
	}
}

void WinSoft::FillCircle(Point center, float radius, FColor32 color, Surface surface)
{
	// TODO:
}

int WinSoft::CreateObject(int vbo, int ibo, Primitive type)
{	
	// TODO: Should be preallocated block that expands as necessary(dynamic array),
	// rather than reallocating block after every created object
	Object* object = (Object*)(_objects ? realloc(_objects, sizeof(Object)*(_objectCount+1)) : malloc(sizeof(Object)));
	
	if (!object)
	{
		fprintf(stderr, "%s, %s(), line %d: Failure to (re)allocate block for object(s).\n", __func__, __FILE__, __LINE__);
		return NOT_AN_OBJECT;
	}
	else
	{
		_objects = object;
		object[_objectCount]._vbo = vbo;
		object[_objectCount]._ibo = ibo;
		object[_objectCount]._type = type;
		object[_objectCount]._dirty = true;

		++_objectCount;
	}

	return _objectCount-1;
}

void WinSoft::DrawObject(int id)
{		
	if (_objectCount > 0 && id < _objectCount)
	{	
		//TODO: calculate intersections if object is dirty
		Object* object = _objects + id;		

		for (int i = 0; i < MAX_OBJECTS; ++i)
		{
			if (_drawQueue[i] == NOT_AN_OBJECT)
			{
				_drawQueue[i] = id;
				_drawQueue[i+1] = NOT_AN_OBJECT;
				break;
			}
		}
	}	
}

bool WinSoft::ObjectExists(int id)
{
	return id > NOT_AN_OBJECT && id < _objectCount;
}

void WinSoft::SetObjectDirty(int id)
{
}

void WinSoft::DestroyObjects()
{
	if (_drawQueue)
	{
		free(_drawQueue);
		_drawQueue = NULL;
	}

	if (_objects)
	{
		free(_objects);
		_objects = NULL;		
	}

	_objectCount = 0;	
}



void WinSoft::Draw3D(Draw3DSettings& settings, Surface surface)
{
	if (settings._drawMode == NORMAL)
	{
		
	}
	else if (settings._drawMode == WIREFRAME)
	{
		for (int i = 0; i < MAX_OBJECTS; ++i)
		{
			int id = _drawQueue[i];
			
			if (id == NOT_AN_OBJECT)
				break;
			
			_drawQueue[i] = NOT_AN_OBJECT;

			if (_objectCount > 0 && id < _objectCount)
			{
				//TODO: Draw all primitive types
				//TODO: Including points, line lists, triangle lists, triangle fans, etc.
				Object* object = _objects + id;
				BufferObject* vbo = WinSoft::MapVBO(object->_vbo);
				BufferObject* ibo = WinSoft::MapIBO(object->_ibo);

				if (!vbo || !ibo)
				{
					fprintf(stderr, "Associated vertex or index buffer is not available to render object %d: %s:: %s:: %d\n", id, __FILE__, __func__, __LINE__);
					continue;
				}

				switch (object->_type)
				{
				case Primitive::LINE:
				{
					for (int i = 0; i < ibo->_length - 1; ++i)
					{
						unsigned int* index = ((unsigned int*) ibo->_data)+i;
						DrawLine(((Vertex*)vbo->_data)[*index], ((Vertex*)vbo->_data)[*(index+1)], surface);
					}
				}
				break;
				case Primitive::TRIANGLE:
				{
					for (int i = 0; i < ibo->_length - 2; i += 2)
					{
						unsigned int* index = ((unsigned int*) ibo->_data)+i;

						DrawLine(((Vertex*)vbo->_data)[*index], ((Vertex*)vbo->_data)[*(index+1)], surface);
						DrawLine(((Vertex*)vbo->_data)[*(index+1)], ((Vertex*)vbo->_data)[*(index+2)], surface);
						DrawLine(((Vertex*)vbo->_data)[*(index+2)], ((Vertex*)vbo->_data)[*index], surface);
					}
				}
				break;
				case Primitive::TRIANGLE_STRIP:
				{
					for (int i = 0; i < ibo->_length - 2; ++i)
					{
						unsigned int* index = ((unsigned int*) ibo->_data)+i;

						DrawLine(((Vertex*)vbo->_data)[*index], ((Vertex*)vbo->_data)[*(index+1)], surface);
						DrawLine(((Vertex*)vbo->_data)[*(index+1)], ((Vertex*)vbo->_data)[*(index+2)], surface);
						DrawLine(((Vertex*)vbo->_data)[*(index+2)], ((Vertex*)vbo->_data)[*index], surface);
					}
				}
				break;
				default:
					break;
				}
			}
		}		
	}
}

void WinSoft::Startup()
{
	if (!_drawQueue)
	{
		_drawQueue = (int*)malloc(sizeof(int)*(MAX_OBJECTS + 1));
		_drawQueue[0] = _drawQueue[MAX_OBJECTS] = NOT_AN_OBJECT;
	}
}

void WinSoft::Shutdown()
{
	FlushBufferMemory();
	DestroyObjects();
}

void WinSoft::ToColorNormalized(const WinSoft::Color32& pcolor, WinSoft::FColor32& fcolor)
{
	fcolor._r = (float)(pcolor & (0xFF<<8)) / (0xFF<<8);
	fcolor._g = (float)(pcolor & (0xFF<<16)) / (0xFF<<16);
	fcolor._b = (float)(pcolor & (0xFF<<24)) / (0xFF<<24);
	fcolor._a = (float)(pcolor & (0xFF)) / (0xFF);
}

void WinSoft::ToColor(const WinSoft::FColor32& color, WinSoft::Color32& pcolor)
{
	pcolor = (((BYTE)(255*color._b)) << 24) |
		     (((BYTE)(255*color._g)) << 16) |
		     (((BYTE)(255*color._r)) << 8)  |
		     ((BYTE)(255*color._a));
}

float WinSoft::Clamp01(float value)
{
	return value < 0.0f ? 0.0f : (value > 1.0f ? 1.0f : value);
}

WinSoft::FColor32 WinSoft::LerpColor(FColor32 a, FColor32 b, float t)
{
	t = Clamp01(t);
	return FColor32{a._r+(b._r - a._r)*t, a._g+(b._g - a._g)*t, a._b+(b._b - a._b)*t, a._a+(b._a - a._a)*t};
}

float WinSoft::Magnitude(Point a, Point b)
{
	return sqrt((b._x-a._x)*(b._x-a._x) + (b._y-a._y)*(b._y-a._y));
}
