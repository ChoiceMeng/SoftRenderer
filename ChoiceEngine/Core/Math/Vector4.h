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
		class _declspec(dllexport)  Vector4
		{
		public:
			Vector4()
			{

			}

			Vector4(float x, float y, float z, float w)
			{
				x = x;
				y = y;
				z = z;
				w = w;
			}

			inline Vector4 operator*(const float nScale)
			{
				return Vector4(x * nScale, y * nScale, z * nScale, w * nScale);
			}

			inline Vector4 operator*(const Vector4& vec)
			{
				return Vector4(x * vec.x, y * vec.y, z * vec.z, w * vec.w);
			}

			inline Vector4 operator+(const Vector4& vec)
			{
				return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
			}

			inline Vector4 operator-(const Vector4& vec)
			{
				return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
			}

			inline Vector4 operator/(const float nScale)
			{
				float fScale = 1.0 / nScale;

				return Vector4(x * fScale, y * fScale, z * fScale, w * w);
			}

			inline Vector4 operator/(const Vector4& vec)
			{
				return Vector4(x / vec.x, y / vec.y, z / vec.z, w / vec.w);
			}

			inline Vector4 operator*=(int nScale)
			{
				x *= nScale;
				y *= nScale;
				z *= nScale;
				w *= nScale;

				return *this;
			}

			inline Vector4 operator*=(const Vector4& vec)
			{
				x *= vec.x;
				y *= vec.y;
				z *= vec.z;
				w *= vec.w;

				return *this;
			}

			inline Vector4 operator+=(const Vector4& vec)
			{
				x += vec.x;
				y += vec.y;
				z += vec.z;
				w += vec.w;

				return *this;
			}

			inline Vector4 operator-=(const Vector4& vec)
			{
				x -= vec.x;
				y -= vec.y;
				z -= vec.z;
				w -= vec.w;

				return *this;
			}

			inline Vector4 operator/=(const Vector4& vec)
			{
				x /= vec.x;
				y /= vec.y;
				z /= vec.z;
				w /= vec.w;

				return *this;
			}

			inline Vector4 operator/=(int nScale)
			{
				x /= nScale;
				y /= nScale;
				z /= nScale;
				w /= nScale;

				return *this;
			}

			inline float operator[](const int nIdx) const
			{
				if(nIdx >= 4)
					return 0;

				return *(&x + nIdx);
			}

			inline float& operator[](const int nIdx)
			{
				if(nIdx >= 4)
					return x;

				return *(&x + nIdx);
			}
		public:
			float x, y, z, w;

			static const Vector4 ZERO;
			static const Vector4 UNIT_X;
			static const Vector4 UNIT_Y;
			static const Vector4 UNIT_Z;
			static const Vector4 UNIT_W;
		};
	}
}