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
		class _declspec(dllexport)  Vector3
		{
		public:
			Vector3() : x(0.0f), y(0.0f), z(0.0f)
			{

			}

			Vector3(float x1, float y1, float z1)
			{
				x = x1;
				y = y1;
				z = z1;
			}

			inline Vector3 operator*(const float nScale) const
			{
				return Vector3(x * nScale, y * nScale, z * nScale);
			}

			inline Vector3 operator*(const Vector3& vec) const
			{
				return Vector3(x * vec.x, y * vec.y, z * vec.z);
			}

			inline Vector3 operator+(const Vector3& vec) const
			{
				return Vector3(x + vec.x, y + vec.y, z + vec.z);
			}

			inline Vector3 operator-(const Vector3& vec) const
			{
				return Vector3(x - vec.x, y - vec.y, z - vec.z);
			}

			inline Vector3 operator/(const float nScale) const
			{
				float fScale = 1.0 / nScale;

				return Vector3(x * fScale, y * fScale, z * fScale);
			}

			inline Vector3 operator/(const Vector3& vec) const
			{
				return Vector3(x / vec.x, y / vec.y, z / vec.z);
			}

			inline Vector3 operator*=(int nScale)
			{
				x *= nScale;
				y *= nScale;
				z *= nScale;

				return *this;
			}

			inline Vector3 operator*=(const Vector3& vec)
			{
				x *= vec.x;
				y *= vec.y;
				z *= vec.z;

				return *this;
			}

			inline Vector3 operator+=(const Vector3& vec)
			{
				x += vec.x;
				y += vec.y;
				z += vec.z;

				return *this;
			}

			inline Vector3 operator-=(const Vector3& vec)
			{
				x -= vec.x;
				y -= vec.y;
				z -= vec.z;

				return *this;
			}

			inline Vector3 operator/=(const Vector3& vec)
			{
				x /= vec.x;
				y /= vec.y;
				z /= vec.z;

				return *this;
			}

			inline Vector3 operator/=(int nScale)
			{
				x /= nScale;
				y /= nScale;
				z /= nScale;

				return *this;
			}

			inline float operator[](const int nIdx) const
			{
				if(nIdx >= 3)
					return 0;

				return *(&x + nIdx);
			}

			inline float& operator[](const int nIdx)
			{
				if(nIdx >= 3)
					return x;

				return *(&x + nIdx);
			}

			inline bool operator==(const Vector3& rV) const
			{
				return (x == rV.x && y == rV.y && z == rV.z);
			}

			inline bool operator!=(const Vector3& rV) const
			{
				return (x != rV.x || y != rV.y || z != rV.z);
			}

			// 向量的模
			inline float Length()
			{
				return sqrt( x * x + y * y + z * z );
			}

			inline float Distance(const Vector3& vec)
			{
				return (*this - vec).Length();
			}

			inline float DotVector(const Vector3& vec)
			{
				return x * vec.x + y * vec.y + z * vec.z;
			}

			inline Vector3 CrossVector(Vector3& vec) const
			{
				return Vector3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
			}

			inline Vector3 Normalize()
			{
				float fLength = Length();

				if(fLength > 1e-8)
				{
					// 向量除以模
					float fFactor = 1.0 / fLength;
					x *= fFactor;
					y *= fFactor;
					z *= fFactor;
				}

				return *this;
			}
		public:
			float x, y, z;

			static const Vector3 ZERO;
			static const Vector3 UNIT_X;
			static const Vector3 UNIT_Y;
			static const Vector3 UNIT_Z;
		};
	}
}