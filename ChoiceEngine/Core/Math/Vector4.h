/********************************************************************
	created:	2012/05/16
	filename: 	BaseMath
	author:		Choice
	
	purpose:	������ѧ����
*********************************************************************/
#pragma once

#include <cmath>

namespace CE
{
	namespace Math
	{
		class Vector4
		{
		public:
			Vector4(float x, float y, float z)
			{
				m_fX = x;
				m_fY = y;
				m_fZ = z;
			}

			inline Vector4 operator*(const float nScale)
			{
				return Vector4(m_fX * nScale, m_fY * nScale, m_fZ * nScale);
			}

			inline Vector4 operator*(const Vector4& vec)
			{
				return Vector4(m_fX * vec.m_fX, m_fY * vec.m_fY, m_fZ * vec.m_fZ);
			}

			inline Vector4 operator+(const Vector4& vec)
			{
				return Vector4(m_fX + vec.m_fX, m_fY + vec.m_fY, m_fZ + vec.m_fZ);
			}

			inline Vector4 operator-(const Vector4& vec)
			{
				return Vector4(m_fX - vec.m_fX, m_fY - vec.m_fY, m_fZ - vec.m_fZ);
			}

			inline Vector4 operator/(const float nScale)
			{
				float fScale = 1.0 / nScale;

				return Vector4(m_fX * fScale, m_fY * fScale, m_fZ * fScale);
			}

			inline Vector4 operator/(const Vector4& vec)
			{
				return Vector4(m_fX / vec.m_fX, m_fY / vec.m_fY, m_fZ / vec.m_fZ);
			}

			inline Vector4 operator*=(int nScale)
			{
				m_fX *= nScale;
				m_fY *= nScale;
				m_fZ *= nScale;

				return *this;
			}

			inline Vector4 operator*=(const Vector4& vec)
			{
				m_fX *= vec.m_fX;
				m_fY *= vec.m_fY;
				m_fZ *= vec.m_fZ;

				return *this;
			}

			inline Vector4 operator+=(const Vector4& vec)
			{
				m_fX += vec.m_fX;
				m_fY += vec.m_fY;
				m_fZ += vec.m_fZ;

				return *this;
			}

			inline Vector4 operator-=(const Vector4& vec)
			{
				m_fX -= vec.m_fX;
				m_fY -= vec.m_fY;
				m_fZ -= vec.m_fZ;

				return *this;
			}

			inline Vector4 operator/=(const Vector4& vec)
			{
				m_fX /= vec.m_fX;
				m_fY /= vec.m_fY;
				m_fZ /= vec.m_fZ;

				return *this;
			}

			inline Vector4 operator/=(int nScale)
			{
				m_fX /= nScale;
				m_fY /= nScale;
				m_fZ /= nScale;

				return *this;
			}

			// inline float operator

			inline float Length()
			{
				return sqrt( m_fX * m_fX + m_fY * m_fY + m_fZ * m_fZ );
			}

			inline float Distance(const Vector4& vec)
			{
				return (*this - vec).Length();
			}

			inline float DotVector(const Vector4& vec)
			{
				return m_fX * vec.m_fX + m_fY * vec.m_fY + m_fZ * vec.m_fZ;
			}

			inline Vector4 CrossVector(const Vector4& vec)
			{
				return Vector4(m_fY * vec.m_fZ - m_fZ * vec.m_fY, m_fZ * vec.m_fX - m_fX * vec.m_fZ, m_fX * vec.m_fY - m_fY * vec.m_fX);
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
			float m_fX, m_fY, m_fZ, m_fW;

			static const Vector4 ZERO;
			static const Vector4 UNIT_X;
			static const Vector4 UNIT_Y;
			static const Vector4 UNIT_Z;
		};
	}
}