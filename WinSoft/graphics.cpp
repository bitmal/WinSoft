#include "stdafx.h"
#include "graphics.h"

#include <cmath>

void WinSoft::RefreshSurface(WinSoft::Surface surface, WinSoft::Color32 color)
{
	int width = (int)(surface._rect._topRight._x);
	int height = (int)(surface._rect._topRight._y);

	PColor32 c = 0;
	PColor(color, c);

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

void WinSoft::DrawLine(WinSoft::Point a, WinSoft::Point b, WinSoft::Color32 color, WinSoft::Surface surface)
{
	WinSoft::PColor32 c{};
	PColor(color, c);

	/* Bresanham's Line Midpoint Algorithm */
	int dx = (int)b._x - a._x;
	int dy = (int)b._y - a._y;
	int dxAbs = abs(dx);
	int dyAbs = abs(dy);

	// Edge Case: zero magnitude
	if (!dx && !dy)
	{
		printf("DrawLine(line: %d): no magnitude\n", __LINE__);
		return;
	}		

	// Edge Case: 1 row
	if (!dyAbs)
	{		
		for (int x = a._x; dx>0 ? x<=b._x : x>=b._x; dx>0 ? ++x : --x)
		{
			int y = a._y;
			int index = (x + (y*surface._rect._topRight._x))*sizeof(c);

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
		for (int y = a._y; dy>0 ? y<=b._y : y>=b._y; dy>0 ? ++y : --y)
		{
			int x = a._x;
			int index = (x + (y*surface._rect._topRight._x))*sizeof(c);

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
			int x = a._x + (dx > 0 ? delta : -delta);
			int y = a._y + (dy > 0 ? delta : -delta);

			int index = (x + (y*surface._rect._topRight._x))*sizeof(c);

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
			for (int x = a._x; x <= b._x; ++x)
			{
				int y = lroundf((float)dy/dx*(x-a._x) + a._y);
				int index = (x + (y *surface._rect._topRight._x))*sizeof(c);

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
			for (int y = a._y; y <= b._y; ++y)
			{
				int x = lroundf((float)dx/dy*(y-a._y) + a._x);
				int index = (x + (y *surface._rect._topRight._x))*sizeof(c);

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
			for (int x = a._x; x >= b._x; --x)
			{
				int y = lroundf((float)dy/dx*(x-a._x) + a._y);
				int index = (x + (y *surface._rect._topRight._x))*sizeof(c);

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
			for (int y = a._y; y <= b._y; ++y)
			{
				int x = lroundf((float)dx/dy*(y-a._y) + a._x);
				int index = (x + (y *surface._rect._topRight._x))*sizeof(c);

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
			for (int x = a._x; x >= b._x; --x)
			{
				int y = lroundf((float)dy/dx*(x-a._x) + a._y);
				int index = (x + (y *surface._rect._topRight._x))*sizeof(c);

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
			for (int y = a._y; y >= b._y; --y)
			{
				int x = lroundf((float)dx/dy*(y-a._y) + a._x);
				int index = (x + (y *surface._rect._topRight._x))*sizeof(c);

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
			for (int x = a._x; x <= b._x; ++x)
			{
				int y = lroundf((float)dy/dx*(x-a._x) + a._y);
				int index = (x + (y *surface._rect._topRight._x))*sizeof(c);

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
			for (int y = a._y; y >= b._y; --y)
			{
				int x = lroundf((float)dx/dy*(y-a._y) + a._x);
				int index = (x + (y *surface._rect._topRight._x))*sizeof(c);

				BYTE* pixel = (BYTE*)surface._data;
				pixel[index+3] = c;
				pixel[index+2] = c >> 8;
				pixel[index+1] = c >> 16;
				pixel[index] = c >> 24;
			}
		}
	}
}

void WinSoft::PColor(const WinSoft::Color32& color, WinSoft::PColor32& pcolor)
{
	pcolor = (((BYTE)(255*color._b)) << 24) |
		     (((BYTE)(255*color._g)) << 16) |
		     (((BYTE)(255*color._r)) << 8)  |
		     ((BYTE)(255*color._a));
}
