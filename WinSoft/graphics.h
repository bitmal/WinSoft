#ifndef _GRAPHICS_H
#define _GRAPHICS_H

namespace WinSoft
{
	struct Point
	{
		float _x, _y;
	};

	struct Rect
	{
		Point _bottomLeft;
		Point _topRight;
	};
	
	typedef unsigned int PColor32;
	struct Color32
	{
		float _r, _g, _b, _a;
	};

	struct Surface
	{
		Rect _rect;
		void* _data;
	};

	void RefreshSurface(Surface surface, Color32 color);
	void DrawLine(Point a, Point b, Color32 color, Surface surface);

	void PColor(const WinSoft::Color32& color, WinSoft::PColor32& pcolor);
}

#endif
