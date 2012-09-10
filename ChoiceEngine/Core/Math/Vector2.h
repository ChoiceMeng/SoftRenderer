/********************************************************************
	created:	2012/05/16
	filename: 	BaseMath
	author:		Choice
	
	purpose:	基础数学函数
*********************************************************************/
#pragma once

#include <cmath>

namespace CE
{
	namespace Math
	{
		class _declspec(dllexport)  Vector2
		{
		public:
			Vector2()
			{

			}

			Vector2(float x, float y)
			{
				x = x;
				y = y;
			}

			inline Vector2 operator *(const float nScale)
			{
				return Vector2(x * nScale, y * nScale);
			}

			inline Vector2 operator *(const Vector2& vec)
			{
				return Vector2(x * vec.x, y * vec.y);
			}

			inline Vector2 operator +(const Vector2& vec)
			{
				return Vector2(x + vec.x, y + vec.y);
			}

			inline Vector2 operator -(const Vector2& vec)
			{
				return Vector2(x - vec.x, y - vec.y);
			}

			inline Vector2 operator /(const float nScale)
			{
				float fScale = 1.0 / nScale;

				return Vector2(x * fScale, y * fScale);
			}

			inline Vector2 operator /(const Vector2& vec)
			{
				return Vector2(x / vec.x, y / vec.y);
			}

			inline Vector2 operator *=(int nScale)
			{
				x *= nScale;
				y *= nScale;

				return *this;
			}

			inline Vector2 operator *=(const Vector2& vec)
			{
				x *= vec.x;
				y *= vec.y;

				return *this;
			}

			inline Vector2 operator +=(const Vector2& vec)
			{
				x += vec.x;
				y += vec.y;

				return *this;
			}

			inline Vector2 operator -=(const Vector2& vec)
			{
				x -= vec.x;
				y -= vec.y;

				return *this;
			}

			inline Vector2 operator /=(const Vector2& vec)
			{
				x /= vec.x;
				y /= vec.y;

				return *this;
			}

			inline Vector2 operator /=(int nScale)
			{
				x /= nScale;
				y /= nScale;

				return *this;
			}

			inline float Length()
			{
				return sqrt( x * x + y * y );
			}

			inline float Distance(const Vector2& vec)
			{
				return (*this - vec).Length();
			}

			inline float DotVector(const Vector2& vec)
			{
				return x * vec.x + y * vec.y;
			}

			inline void Nomalize()
			{
				float fLength = Length();

				if(fLength > 1e-8)
				{
					float fFactor = 1.0 / fLength;
					x *= fFactor;
					y *= fFactor;
				}
			}
		public:
			float x, y;

			static const Vector2 ZERO;
			static const Vector2 UNIT_X;
			static const Vector2 UNIT_Y;
		};
	}
}