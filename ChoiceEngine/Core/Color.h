#pragma once
#include "MathGlobal.h"

namespace CE
{
	namespace Core{
		class CColor
		{
		public:
			CColor(){}
			CColor(int Color)
			{
				r = (unsigned char)(Color & 0xff0000 >> 16);
				g = (unsigned char)(Color & 0x00ff00 >> 8);
				b = (unsigned char)(Color & 0x0000ff);
				a = 255;
			}

			CColor(unsigned char cr, unsigned char cg, unsigned char cb, unsigned char ca = 255) : r(cr), g(cg), b(cb), a(ca)
			{

			}

			CColor operator+(const CColor& Color)
			{
				unsigned char cr = Min(r + Color.r, 255);
				unsigned char cg = Min(g + Color.g, 255);
				unsigned char cb = Min(b + Color.b, 255);
				unsigned char ca = Min(a + Color.a, 255);

				return CColor(cr, cg, cb, ca);
			}

			CColor operator*(const CColor& Color) const
			{
				int cr = Min(r*Color.r, 255);
				int cg = Min(g*Color.g, 255);
				int cb = Min(b*Color.b, 255);
				int ca = Min(a*Color.a, 255);

				return CColor(cr, cg, cb, ca);
			}

			CColor operator*(float scale) const
			{
				int cr = Min(r*scale, 255);
				int cg = Min(g*scale, 255);
				int cb = Min(b*scale, 255);
				int ca = Min(a*scale, 255);

				return CColor(cr, cg, cb, ca);
			}

			CColor& operator*=(float scale)
			{
				r = Min(r*scale, 255);
				g = Min(g*scale, 255);
				b = Min(b*scale, 255);

				return *this;
			}
		public:
			unsigned char r, g, b, a;
		};
	}
}