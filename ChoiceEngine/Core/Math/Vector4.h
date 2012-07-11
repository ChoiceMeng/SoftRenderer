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
				m_fX = x;
				m_fY = y;
				m_fZ = z;
				m_fW = w;
			}

			inline Vector4 operator*(const float nScale)
			{
				return Vector4(m_fX * nScale, m_fY * nScale, m_fZ * nScale, m_fW * nScale);
			}

			inline Vector4 operator*(const Vector4& vec)
			{
				return Vector4(m_fX * vec.m_fX, m_fY * vec.m_fY, m_fZ * vec.m_fZ, m_fW * vec.m_fW);
			}

			inline Vector4 operator+(const Vector4& vec)
			{
				return Vector4(m_fX + vec.m_fX, m_fY + vec.m_fY, m_fZ + vec.m_fZ, m_fW + vec.m_fW);
			}

			inline Vector4 operator-(const Vector4& vec)
			{
				return Vector4(m_fX - vec.m_fX, m_fY - vec.m_fY, m_fZ - vec.m_fZ, m_fW - vec.m_fW);
			}

			inline Vector4 operator/(const float nScale)
			{
				float fScale = 1.0 / nScale;

				return Vector4(m_fX * fScale, m_fY * fScale, m_fZ * fScale, m_fW * m_fW);
			}

			inline Vector4 operator/(const Vector4& vec)
			{
				return Vector4(m_fX / vec.m_fX, m_fY / vec.m_fY, m_fZ / vec.m_fZ, m_fW / vec.m_fW);
			}

			inline Vector4 operator*=(int nScale)
			{
				m_fX *= nScale;
				m_fY *= nScale;
				m_fZ *= nScale;
				m_fW *= nScale;

				return *this;
			}

			inline Vector4 operator*=(const Vector4& vec)
			{
				m_fX *= vec.m_fX;
				m_fY *= vec.m_fY;
				m_fZ *= vec.m_fZ;
				m_fW *= vec.m_fW;

				return *this;
			}

			inline Vector4 operator+=(const Vector4& vec)
			{
				m_fX += vec.m_fX;
				m_fY += vec.m_fY;
				m_fZ += vec.m_fZ;
				m_fW += vec.m_fW;

				return *this;
			}

			inline Vector4 operator-=(const Vector4& vec)
			{
				m_fX -= vec.m_fX;
				m_fY -= vec.m_fY;
				m_fZ -= vec.m_fZ;
				m_fW -= vec.m_fW;

				return *this;
			}

			inline Vector4 operator/=(const Vector4& vec)
			{
				m_fX /= vec.m_fX;
				m_fY /= vec.m_fY;
				m_fZ /= vec.m_fZ;
				m_fW /= vec.m_fW;

				return *this;
			}

			inline Vector4 operator/=(int nScale)
			{
				m_fX /= nScale;
				m_fY /= nScale;
				m_fZ /= nScale;
				m_fW /= nScale;

				return *this;
			}

			inline float operator[](const int nIdx) const
			{
				if(nIdx >= 4)
					return 0;

				return *(&m_fX + nIdx);
			}

			inline float& operator[](const int nIdx)
			{
				if(nIdx >= 4)
					return m_fX;

				return *(&m_fX + nIdx);
			}
		public:
			float m_fX, m_fY, m_fZ, m_fW;

			static const Vector4 ZERO;
			static const Vector4 UNIT_X;
			static const Vector4 UNIT_Y;
			static const Vector4 UNIT_Z;
			static const Vector4 UNIT_W;
		};
	}
}