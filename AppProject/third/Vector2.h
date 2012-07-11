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
				m_fX = x;
				m_fY = y;
			}

			inline Vector2 operator *(const float nScale)
			{
				return Vector2(m_fX * nScale, m_fY * nScale);
			}

			inline Vector2 operator *(const Vector2& vec)
			{
				return Vector2(m_fX * vec.m_fX, m_fY * vec.m_fY);
			}

			inline Vector2 operator +(const Vector2& vec)
			{
				return Vector2(m_fX + vec.m_fX, m_fY + vec.m_fY);
			}

			inline Vector2 operator -(const Vector2& vec)
			{
				return Vector2(m_fX - vec.m_fX, m_fY - vec.m_fY);
			}

			inline Vector2 operator /(const float nScale)
			{
				float fScale = 1.0 / nScale;

				return Vector2(m_fX * fScale, m_fY * fScale);
			}

			inline Vector2 operator /(const Vector2& vec)
			{
				return Vector2(m_fX / vec.m_fX, m_fY / vec.m_fY);
			}

			inline Vector2 operator *=(int nScale)
			{
				m_fX *= nScale;
				m_fY *= nScale;

				return *this;
			}

			inline Vector2 operator *=(const Vector2& vec)
			{
				m_fX *= vec.m_fX;
				m_fY *= vec.m_fY;

				return *this;
			}

			inline Vector2 operator +=(const Vector2& vec)
			{
				m_fX += vec.m_fX;
				m_fY += vec.m_fY;

				return *this;
			}

			inline Vector2 operator -=(const Vector2& vec)
			{
				m_fX -= vec.m_fX;
				m_fY -= vec.m_fY;

				return *this;
			}

			inline Vector2 operator /=(const Vector2& vec)
			{
				m_fX /= vec.m_fX;
				m_fY /= vec.m_fY;

				return *this;
			}

			inline Vector2 operator /=(int nScale)
			{
				m_fX /= nScale;
				m_fY /= nScale;

				return *this;
			}

			inline float Length()
			{
				return sqrt( m_fX * m_fX + m_fY * m_fY );
			}

			inline float Distance(const Vector2& vec)
			{
				return (*this - vec).Length();
			}

			inline float DotVector(const Vector2& vec)
			{
				return m_fX * vec.m_fX + m_fY * vec.m_fY;
			}

			inline void Nomalize()
			{
				float fLength = Length();

				if(fLength > 1e-8)
				{
					float fFactor = 1.0 / fLength;
					m_fX *= fFactor;
					m_fY *= fFactor;
				}
			}
		public:
			float m_fX, m_fY;

			static const Vector2 ZERO;
			static const Vector2 UNIT_X;
			static const Vector2 UNIT_Y;
		};
	}
}