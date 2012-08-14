#include "Pre.h"

class Color
{
public:
	Color(int color)
	{
		r = (unsigned char)(color & 0xff0000 >> 16);
		g = (unsigned char)(color & 0x00ff00 >> 8);
		b = (unsigned char)(color & 0x0000ff);
		a = 255;
	}

	Color(unsigned char cr, unsigned char cg, unsigned char cb, unsigned char ca = 255) : r(cr), g(cg), b(cb), a(ca)
	{

	}

	Color operator+(const Color& color) const
	{
		int cr = Min(r + color.r, 255);
		int cg = Min(g + color.g, 255);
		int cb = Min(b + color.b, 255);
		int ca = Min(a + color.a, 255);

		return Color(cr, cg, cb, ca);
	}

	Color operator*(const Color& color) const
	{
		int cr = Min(r*color.r, 255);
		int cg = Min(g*color.g, 255);
		int cb = Min(b*color.b, 255);
		int ca = Min(a*color.a, 255);

		return Color(cr, cg, cb, ca);
	}

	Color operator*(float scale)
	{
		int cr = Min(r*scale, 255);
		int cg = Min(g*scale, 255);
		int cb = Min(b*scale, 255);
		int ca = Min(a*scale, 255);

		return Color(cr, cg, cb, ca);
	}

	Color& operator*=(float scale)
	{
		r = Min(r*scale, 255);
		g = Min(g*scale, 255);
		b = Min(b*scale, 255);

		return *this;
	}
private:
	unsigned char r, g, b, a;
};