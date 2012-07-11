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
			Vector3()
			{

			}

			Vector3(float x, float y, float z)
			{
				m_fX = x;
				m_fY = y;
				m_fZ = z;
			}

			inline Vector3 operator*(const float nScale)
			{
				return Vector3(m_fX * nScale, m_fY * nScale, m_fZ * nScale);
			}

			inline Vector3 operator*(const Vector3& vec)
			{
				return Vector3(m_fX * vec.m_fX, m_fY * vec.m_fY, m_fZ * vec.m_fZ);
			}

			inline Vector3 operator+(const Vector3& vec)
			{
				return Vector3(m_fX + vec.m_fX, m_fY + vec.m_fY, m_fZ + vec.m_fZ);
			}

			inline Vector3 operator-(const Vector3& vec)
			{
				return Vector3(m_fX - vec.m_fX, m_fY - vec.m_fY, m_fZ - vec.m_fZ);
			}

			inline Vector3 operator/(const float nScale)
			{
				float fScale = 1.0 / nScale;

				return Vector3(m_fX * fScale, m_fY * fScale, m_fZ * fScale);
			}

			inline Vector3 operator/(const Vector3& vec)
			{
				return Vector3(m_fX / vec.m_fX, m_fY / vec.m_fY, m_fZ / vec.m_fZ);
			}

			inline Vector3 operator*=(int nScale)
			{
				m_fX *= nScale;
				m_fY *= nScale;
				m_fZ *= nScale;

				return *this;
			}

			inline Vector3 operator*=(const Vector3& vec)
			{
				m_fX *= vec.m_fX;
				m_fY *= vec.m_fY;
				m_fZ *= vec.m_fZ;

				return *this;
			}

			inline Vector3 operator+=(const Vector3& vec)
			{
				m_fX += vec.m_fX;
				m_fY += vec.m_fY;
				m_fZ += vec.m_fZ;

				return *this;
			}

			inline Vector3 operator-=(const Vector3& vec)
			{
				m_fX -= vec.m_fX;
				m_fY -= vec.m_fY;
				m_fZ -= vec.m_fZ;

				return *this;
			}

			inline Vector3 operator/=(const Vector3& vec)
			{
				m_fX /= vec.m_fX;
				m_fY /= vec.m_fY;
				m_fZ /= vec.m_fZ;

				return *this;
			}

			inline Vector3 operator/=(int nScale)
			{
				m_fX /= nScale;
				m_fY /= nScale;
				m_fZ /= nScale;

				return *this;
			}

			inline float operator[](const int nIdx) const
			{
				if(nIdx >= 3)
					return 0;

				return *(&m_fX + nIdx);
			}

			inline float& operator[](const int nIdx)
			{
				if(nIdx >= 3)
					return m_fX;

				return *(&m_fX + nIdx);
			}

			inline bool operator==(const Vector3& rV) const
			{
				return (m_fX == rV.m_fX && m_fY == rV.m_fY && m_fZ == rV.m_fZ);
			}

			inline bool operator!=(const Vector3& rV) const
			{
				return (m_fX != rV.m_fX || m_fY != rV.m_fY || m_fZ != rV.m_fZ);
			}

			inline float Length()
			{
				return sqrt( m_fX * m_fX + m_fY * m_fY + m_fZ * m_fZ );
			}

			inline float Distance(const Vector3& vec)
			{
				return (*this - vec).Length();
			}

			inline float DotVector(const Vector3& vec)
			{
				return m_fX * vec.m_fX + m_fY * vec.m_fY + m_fZ * vec.m_fZ;
			}

			inline Vector3 CrossVector(const Vector3& vec)
			{
				return Vector3(m_fY * vec.m_fZ - m_fZ * vec.m_fY, m_fZ * vec.m_fX - m_fX * vec.m_fZ, m_fX * vec.m_fY - m_fY * vec.m_fX);
			}

			inline void Nomalize()
			{
				float fLength = Length();

				if(fLength > 1e-8)
				{
					float fFactor = 1.0 / fLength;
					m_fX *= fFactor;
					m_fY *= fFactor;
					m_fZ *= fFactor;
				}
			}
		public:
			float m_fX, m_fY, m_fZ;

			static const Vector3 ZERO;
			static const Vector3 UNIT_X;
			static const Vector3 UNIT_Y;
			static const Vector3 UNIT_Z;
		};
	}
}