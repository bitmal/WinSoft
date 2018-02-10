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

	struct ColorBorder
	{
		enum BorderType : int
		{
			INSET, OUTSET, NONE = -1
		};

		Color32 _color;
		BorderType _type;
		unsigned int _width;
	};

	struct Surface
	{
		Rect _rect;
		void* _data;
	};

	void RefreshSurface(Surface surface, Color32 color);

	void DrawLine(Point a, Point b, Color32 color, Surface surface);	
	void DrawRect(Rect rect, ColorBorder border, Surface surface);
	void FillRect(Rect rect, const ColorBorder* border, Color32 fillColor, Surface surface);

	void FColor(const WinSoft::PColor32& pcolor, WinSoft::Color32& fcolor);
	void PColor(const WinSoft::Color32& color, WinSoft::PColor32& pcolor);
}

#endif
