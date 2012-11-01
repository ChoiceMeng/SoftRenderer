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
			Vector4() : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
			{

			}

			Vector4(float x1, float y1, float z1, float w1 = 1.0f)
			{
				x = x1;
				y = y1;
				z = z1;
				w = w1;
			}

			inline Vector4 operator*(const float nScale)
			{
				return Vector4(x * nScale, y * nScale, z * nScale, 1);
			}

			inline Vector4 operator*(const Vector4& vec)
			{
				return Vector4(x * vec.x, y * vec.y, z * vec.z, 1);
			}

			inline Vector4 operator+(const Vector4& vec)
			{
				return Vector4(x + vec.x, y + vec.y, z + vec.z, 1);
			}

			inline Vector4 operator-(const Vector4& vec) const
			{
				return Vector4(x - vec.x, y - vec.y, z - vec.z, 1);
			}

			inline Vector4 operator-() const
			{
				return Vector4(-x, -y, -z, 1);
			}

			inline Vector4 operator/(const float nScale)
			{
				float fScale = 1.0 / nScale;

				return Vector4(x * fScale, y * fScale, z * fScale, 1);
			}

			inline Vector4 operator/(const Vector4& vec)
			{
				return Vector4(x / vec.x, y / vec.y, z / vec.z, 1);
			}

			inline Vector4 operator*=(int nScale)
			{
				x *= nScale;
				y *= nScale;
				z *= nScale;

				return *this;
			}

			inline Vector4 operator*=(const Vector4& vec)
			{
				x *= vec.x;
				y *= vec.y;
				z *= vec.z;

				return *this;
			}

			inline Vector4 operator+=(const Vector4& vec)
			{
				x += vec.x;
				y += vec.y;
				z += vec.z;

				return *this;
			}

			inline Vector4 operator-=(const Vector4& vec)
			{
				x -= vec.x;
				y -= vec.y;
				z -= vec.z;

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

			inline Vector4 operator/=(float nScale)
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

			// 向量的模
			inline float Length() const
			{
				return sqrt( x * x + y * y + z * z );
			}

			inline float Distance(const Vector4& vec)
			{
				return (*this - vec).Length();
			}

			inline float DotVector(const Vector4& vec)
			{
				return x * vec.x + y * vec.y + z * vec.z;
			}

			inline Vector4 CrossVector(Vector4& vec) const
			{
				return Vector4(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x, 1);
			}

			inline Vector4 Nomalize() const
			{
				float fLength = Length();

				if(fLength > 1e-8)
				{
					// 向量除以模
					float fFactor = 1.0 / fLength;
					return Vector4(x * fFactor, y * fFactor, z * fFactor, 1);
				}

				return *this;
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