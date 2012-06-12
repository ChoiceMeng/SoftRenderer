#pragma once
#include <cmath>
#include <memory.h>

namespace CE
{
	namespace Math
	{
		class Matrix4
		{
		public:
			Matrix4(void);
			~Matrix4(void);

			Matrix4(const float value[4][4])
			{
				memcpy(m_fValue, value, sizeof(float) * 12);
			}

			Matrix4(const float v00, const float v01, const float v02, const float v03,
						 const float v10, const float v11, const float v12, const float v13,
						 const float v20, const float v21, const float v22, const float v23,
						 const float v30, const float v31, const float v32, const float v33)
			{
				m_fValue[0][0] = v00;
				m_fValue[0][1] = v01;
				m_fValue[0][2] = v02;
				m_fValue[0][3] = v03;
				m_fValue[1][0] = v10;
				m_fValue[1][1] = v11;
				m_fValue[1][2] = v12;
				m_fValue[1][3] = v13;
				m_fValue[2][0] = v20;
				m_fValue[2][1] = v21;
				m_fValue[2][2] = v22;
				m_fValue[2][3] = v23;
				m_fValue[3][0] = v30;
				m_fValue[3][1] = v31;
				m_fValue[3][2] = v32;
				m_fValue[3][3] = v33;
			}

			Matrix4 operator+(const Matrix4& rV)
			{
				return Matrix4(	m_fValue[0][0] + rV.m_fValue[0][0], m_fValue[0][1] + rV.m_fValue[0][1], m_fValue[0][2] + rV.m_fValue[0][2], m_fValue[0][3] + rV.m_fValue[0][3],
					m_fValue[1][0] + rV.m_fValue[1][0], m_fValue[1][1] + rV.m_fValue[1][1], m_fValue[2][2] + rV.m_fValue[2][2], m_fValue[1][3] + rV.m_fValue[1][3],
					m_fValue[2][0] + rV.m_fValue[2][0], m_fValue[1][1] + rV.m_fValue[1][1], m_fValue[2][2] + rV.m_fValue[2][2], m_fValue[2][3] + rV.m_fValue[2][3],
					m_fValue[2][0] + rV.m_fValue[2][0], m_fValue[1][1] + rV.m_fValue[1][1], m_fValue[2][2] + rV.m_fValue[2][2], m_fValue[3][3] + rV.m_fValue[3][3]);
			}

			Matrix4 operator-(const Matrix4& rV)
			{
				return Matrix4(	m_fValue[0][0] - rV.m_fValue[0][0], m_fValue[0][1] - rV.m_fValue[0][1], m_fValue[0][2] - rV.m_fValue[0][2], m_fValue[0][3] - rV.m_fValue[0][3],
					m_fValue[1][0] - rV.m_fValue[1][0], m_fValue[1][1] - rV.m_fValue[1][1], m_fValue[2][2] - rV.m_fValue[2][2], m_fValue[1][3] - rV.m_fValue[1][3],
					m_fValue[2][0] - rV.m_fValue[2][0], m_fValue[1][1] - rV.m_fValue[1][1], m_fValue[2][2] - rV.m_fValue[2][2], m_fValue[2][3] - rV.m_fValue[2][3],
					m_fValue[2][0] - rV.m_fValue[2][0], m_fValue[1][1] - rV.m_fValue[1][1], m_fValue[2][2] - rV.m_fValue[2][2], m_fValue[3][3] - rV.m_fValue[3][3]);
			}

		public:
			float m_fValue[4][4];
		};
	}
}


