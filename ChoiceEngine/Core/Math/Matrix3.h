#pragma once
#include <cmath>
#include <memory.h>
#include <assert.h>
#include "Vector3.h"

namespace CE
{
	namespace Math
	{
		class _declspec(dllexport)  Matrix3
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

				return mat;
			}

			/*	MARK:
				不能用行向量右乘矩阵或者列向量左乘矩阵。另外使用列向量右乘矩阵时使用的必须是列矩阵。关于行矩阵列矩阵的区别：

				行矩阵：
				三个轴向量为前三行，最后一行为位移变换
				连乘时从左到右接合，左边的变换先应用
				变换向量时为vector * matrix
				列矩阵：
				三个轴向量为前三列，最后一列为位移变换
				连乘时从右到左接合，右边的变换先应用，注意这和*运算符的接合顺序相反
				变换向量时为matrix * vector

				DirectX采用了行矩阵，OpenGL采用了列矩阵

				本引擎采用列矩阵和列向量，如果使用行矩阵那么下面的运算应该写进Vector3中
			*/
			Vector3 Matrix3::operator*(const Vector3& rV)
			{
				Vector3 result;
				for(int i = 0; i < 3; ++i)
				{
					result[i] = m_fValue[i][0] * rV.x + m_fValue[i][1] * rV.y + m_fValue[i][2] * rV.z;
				}

				return result;
			}

			float& operator()(int nRow, int nCol)
			{
				assert(nRow < 3 && nCol < 3);

				return m_fValue[nRow][nCol];
			}

			// 行列式
			double Matrix3::DetM()
			{
				return m_fValue[0][0]*(m_fValue[1][1] * m_fValue[2][2] - m_fValue[1][2]*m_fValue[2][1]) +
					   m_fValue[0][1]*(m_fValue[1][2] * m_fValue[2][0] - m_fValue[1][0]*m_fValue[2][2]) +
					   m_fValue[0][2]*(m_fValue[1][0] * m_fValue[2][1] - m_fValue[1][0]*m_fValue[2][0]);
			}

			// 逆矩阵
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

			inline Matrix3 Matrix3::Scale(const float fScale)
			{
				Matrix3 mat = *this;
				mat(0, 0) = fScale;
				mat(1, 1) = fScale;
				mat(2, 2) = fScale;

				return mat;
			}
		public:
			float m_fValue[3][3];
		};
	}
}


