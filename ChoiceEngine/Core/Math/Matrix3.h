#pragma once
#include <cmath>
#include <memory.h>
#include "Vector3.h"

namespace CE
{
	namespace Math
	{
		class Matrix3
		{
		public:
			Matrix3(void);
			~Matrix3(void);

			Matrix3(const float value[3][3])
			{
				memcpy(m_fValue, value, sizeof(float) * 9);
			}

			Matrix3(const float v00, const float v01, const float v02,
						 const float v10, const float v11, const float v12,
						 const float v20, const float v21, const float v22)
			{
				m_fValue[0][0] = v00;
				m_fValue[0][1] = v01;
				m_fValue[0][2] = v02;
				m_fValue[1][0] = v10;
				m_fValue[1][1] = v11;
				m_fValue[1][2] = v12;
				m_fValue[2][0] = v20;
				m_fValue[2][1] = v21;
				m_fValue[2][2] = v22;
			}

			Matrix3 operator+(const Matrix3& rV)
			{
				return Matrix3(m_fValue[0][0] + rV.m_fValue[0][0], m_fValue[0][1] + rV.m_fValue[0][1], m_fValue[0][2] + rV.m_fValue[0][2],
									   m_fValue[1][0] + rV.m_fValue[1][0], m_fValue[1][1] + rV.m_fValue[1][1], m_fValue[2][2] + rV.m_fValue[2][2],
									   m_fValue[2][0] + rV.m_fValue[2][0], m_fValue[1][1] + rV.m_fValue[1][1], m_fValue[2][2] + rV.m_fValue[2][2]);
			}

			Matrix3 operator-(const Matrix3& rV)
			{
				return Matrix3(m_fValue[0][0] - rV.m_fValue[0][0], m_fValue[0][1] - rV.m_fValue[0][1], m_fValue[0][2] - rV.m_fValue[0][2],
					m_fValue[1][0] - rV.m_fValue[1][0], m_fValue[1][1] - rV.m_fValue[1][1], m_fValue[2][2] - rV.m_fValue[2][2],
					m_fValue[2][0] - rV.m_fValue[2][0], m_fValue[1][1] - rV.m_fValue[1][1], m_fValue[2][2] - rV.m_fValue[2][2]);
			}

			Matrix3 operator*(const Matrix3& rV)
			{
				Matrix3 mat;
				for(int row = 0; row < 3; ++row)
				{
					for(int col = 0; col < 3; ++col)
					{
						mat.m_fValue[row][col] = m_fValue[row][0] * rV.m_fValue[0][col] + m_fValue[row][1] * rV.m_fValue[1][col] + m_fValue[row][2] * rV.m_fValue[2][col];
					}
				}
			}

			Vector3 Matrix3::operator*(const Vector3& rV)
			{
				Vector3 result;
				for(int i = 0; i < 3; ++i)
				{
					result[i] = m_fValue[i][0] * rV.m_fX + m_fValue[i][1] * rV.m_fY + m_fValue[i][2] * rV.m_fZ;
				}
			}

			double Matrix3::DetM()
			{
				return m_fValue[0][0]*(m_fValue[1][1] * m_fValue[2][2] - m_fValue[1][2]*m_fValue[2][1]) +
					   m_fValue[0][1]*(m_fValue[1][2] * m_fValue[2][0] - m_fValue[1][0]*m_fValue[2][2]) +
					   m_fValue[0][2]*(m_fValue[1][0] * m_fValue[2][1] - m_fValue[1][0]*m_fValue[2][0]);
			}

			Matrix3 Matrix3::Inverse()
			{
				double detM = DetM();

				double detM_inv = 1 / detM;

				Matrix3 resultMat;
				
				resultMat.m_fValue[0][0] = detM_inv * (m_fValue[1][1] * m_fValue[2][2] - m_fValue[1][2] * m_fValue[2][1]);
				resultMat.m_fValue[0][1] = -detM_inv * (m_fValue[0][1] * m_fValue[2][2] - m_fValue[0][2] * m_fValue[2][1]);
				resultMat.m_fValue[0][2] = detM_inv * (m_fValue[0][1] * m_fValue[1][2] - m_fValue[0][2] * m_fValue[1][1]);

				resultMat.m_fValue[1][0] = -detM_inv * (m_fValue[1][0] * m_fValue[2][2] - m_fValue[1][2] * m_fValue[2][0]);
				resultMat.m_fValue[1][1] = detM_inv * (m_fValue[0][0] * m_fValue[2][2] - m_fValue[0][2] * m_fValue[2][0]);
				resultMat.m_fValue[1][2] = -detM_inv * (m_fValue[0][1] * m_fValue[1][2] - m_fValue[0][2] * m_fValue[1][0]);

				resultMat.m_fValue[2][0] = detM_inv * (m_fValue[1][0] * m_fValue[2][1] - m_fValue[1][1] * m_fValue[2][0]);
				resultMat.m_fValue[2][1] = -detM_inv * (m_fValue[0][0] * m_fValue[2][1] - m_fValue[0][1] * m_fValue[2][0]);
				resultMat.m_fValue[2][2] = detM_inv * (m_fValue[0][0] * m_fValue[1][1] - m_fValue[0][1] * m_fValue[1][0]);

				return resultMat;
			}
		public:
			float m_fValue[3][3];
		};
	}
}


