#ifndef _GRAPHICS_H
#define _GRAPHICS_H

namespace WinSoft
{
	#define PI 3.14159265
	#define RADIANS_TO_DEGREES(x) x*180/PI
	#define DEGREES_TO_RADIANS(x) x*PI/180

	struct Point
	{
		float _x, _y;
	};

	struct Rect
	{
		Point _bottomLeft;
		Point _topRight;
	};
	
	typedef unsigned int Color32;
	struct FColor32
	{
		float _r, _g, _b, _a;
	};

	struct ColorBorder
	{
		enum BorderType : int
		{
			INSET, OUTSET, NONE = -1
		};

		FColor32 _color;
		BorderType _type;
		unsigned int _width;
	};

	struct Surface
	{
		Rect _rect;
		void* _data;
	};

	void RefreshSurface(Surface surface, FColor32 color);

	void DrawLine(Point a, Point b, FColor32 color, Surface surface);	
	void DrawRect(Rect rect, FColor32 color, Surface surface);
	void FillRect(Rect rect, const ColorBorder* border, FColor32 fillColor, Surface surface);
	void DrawCircle(Point center, float radius, FColor32 color, Surface surface);
	void FillCircle(Point center, float radius, FColor32 color, Surface surface);

	void ToColorNormalized(const WinSoft::Color32& pcolor, WinSoft::FColor32& fcolor);
	void ToColor(const WinSoft::FColor32& color, WinSoft::Color32& pcolor);
}

#endif
