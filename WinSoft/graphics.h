#ifndef _GRAPHICS_H
#define _GRAPHICS_H

namespace WinSoft
{
	#define PI 3.14159265
	#define RADIANS_TO_DEGREES(x) x*180/PI
	#define DEGREES_TO_RADIANS(x) x*PI/180

	#define MAX_OBJECTS 255
	#define NOT_AN_OBJECT -1

	struct Point
	{
		float _x, _y;
	};

	struct Rect
	{
		Point _bottomLeft;
		Point _topRight;
	};

	enum Draw3DMode : int
	{
		NORMAL, WIREFRAME
	};

	struct Draw3DSettings
	{
		Draw3DMode _drawMode;
		Rect _windowResolution;
		bool _windowStretched;
	};
	
	typedef unsigned int Color32;
	struct FColor32
	{
		float _r, _g, _b, _a;
	};

	struct Vertex
	{
		Point _point;
		FColor32 _color;
	};

	enum Primitive : int
	{
		POINT, LINE, TRIANGLE, TRIANGLE_STRIP, QUAD
	};

	struct Object
	{
		Primitive _type;
		int _vertexCount;
		Vertex* _vertices;
	};

	struct ColorBorder
	{
		enum BorderType : int
		{
			INSET, OUTSET, NONE = -1
		};

		FColor32 _color;
		BorderType _type;
		int _width;
	};

	struct Surface
	{
		Rect _rect;
		void* _data;
	};

	void RefreshSurface(Surface surface, FColor32 color);

	void DrawLine(Vertex a, Vertex b, Surface surface);	
	void DrawRect(Rect rect, FColor32 color, Surface surface);
	void FillRect(Rect rect, const ColorBorder* border, FColor32 fillColor, Surface surface);
	void DrawCircle(Point center, float radius, FColor32 color, Surface surface);
	void FillCircle(Point center, float radius, FColor32 color, Surface surface);

	int CreateObject(Vertex* vertices, int vertexCount, Primitive type);
	void DrawObject(int id, Surface surface);
	void DestroyObjects();

	void Draw3D(Draw3DSettings& settings, Surface surface);

	void ToColorNormalized(const WinSoft::Color32& pcolor, WinSoft::FColor32& fcolor);
	void ToColor(const WinSoft::FColor32& color, WinSoft::Color32& pcolor);

	FColor32 LerpColor(FColor32 a, FColor32 b, float t);

	float Clamp01(float value);
	float Magnitude(Point a, Point b);
}

#endif
