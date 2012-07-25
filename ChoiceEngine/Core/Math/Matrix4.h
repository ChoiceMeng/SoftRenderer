#pragma once
#include <cmath>
#include <memory.h>
#include "Vector4.h"
#include <assert.h>
#include "Pre.h"

namespace CE
{
	namespace Math
	{
		class _declspec(dllexport)  Matrix4
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

			Matrix4 operator*(const Matrix4& rV)
			{
				Matrix4 mat;
				for(int row = 0; row < 4; ++row)
				{
					for(int col = 0; col < 4; ++col)
					{
						mat.m_fValue[row][col] = m_fValue[row][0] * rV.m_fValue[0][col] + m_fValue[row][1] * rV.m_fValue[1][col] + m_fValue[row][2] * rV.m_fValue[2][col] + m_fValue[row][3] * rV.m_fValue[3][col];
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

				本引擎采用行优先存储矩阵和行向量，如果使用行矩阵那么下面的运算应该写进Vector4中
			*/
			Vector4 operator*(const Vector4 rV)
			{
				Vector4 vResult;
				for(int i = 0; i < 4; ++i)
				{
					vResult[i] = m_fValue[i][0] * rV.m_fX + m_fValue[i][1] * rV.m_fY + m_fValue[i][2] * rV.m_fZ + m_fValue[i][3] * rV.m_fW;
				}

				return vResult;
			}

			float& operator()(int nRow, int nCol)
			{
				assert(nRow < 4 && nCol < 4);

				return m_fValue[nRow][nCol];
			}

			// 行列式
			double Matrix4::DetM()
			{
				Matrix4 mat = *this;
				double d = (mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0)) * (mat(2, 2) * mat(3, 3) - mat(2, 3) * mat(3, 2)) -
					(mat(0, 0) * mat(1, 2) - mat(0, 2) * mat(1, 0)) * (mat(2, 1) * mat(3, 3) - mat(2, 3) * mat(3, 1)) +
					(mat(0, 0) * mat(1, 3) - mat(0, 3) * mat(1, 0)) * (mat(2, 1) * mat(3, 2) - mat(2, 2) * mat(3, 1)) +
					(mat(0, 1) * mat(1, 2) - mat(0, 2) * mat(1, 1)) * (mat(2, 0) * mat(3, 3) - mat(2, 3) * mat(3, 0)) -
					(mat(0, 1) * mat(1, 3) - mat(0, 3) * mat(1, 1)) * (mat(2, 0) * mat(3, 2) - mat(2, 2) * mat(3, 0)) +
					(mat(0, 2) * mat(1, 3) - mat(0, 3) * mat(1, 2)) * (mat(2, 0) * mat(3, 1) - mat(2, 1) * mat(3, 0));

				return d;
			}

			// 逆矩阵
			Matrix4 Matrix4::Inverse()
			{
				double detM = DetM();

				double d = 1 / detM;

				Matrix4 out;

				Matrix4 m = *this;
				out(0, 0) = d * (m(1, 1) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) +
					m(1, 2) * (m(2, 3) * m(3, 1) - m(2, 1) * m(3, 3)) +
					m(1, 3) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)));
				out(0, 1) = d * (m(2, 1) * (m(0, 2) * m(3, 3) - m(0, 3) * m(3, 2)) +
					m(2, 2) * (m(0, 3) * m(3, 1) - m(0, 1) * m(3, 3)) +
					m(2, 3) * (m(0, 1) * m(3, 2) - m(0, 2) * m(3, 1)));
				out(0, 2) = d * (m(3, 1) * (m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) +
					m(3, 2) * (m(0, 3) * m(1, 1) - m(0, 1) * m(1, 3)) +
					m(3, 3) * (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)));
				out(0, 3) = d * (m(0, 1) * (m(1, 3) * m(2, 2) - m(1, 2) * m(2, 3)) +
					m(0, 2) * (m(1, 1) * m(2, 3) - m(1, 3) * m(2, 1)) +
					m(0, 3) * (m(1, 2) * m(2, 1) - m(1, 1) * m(2, 2)));
				out(1, 0) = d * (m(1, 2) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) +
					m(1, 3) * (m(2, 2) * m(3, 0) - m(2, 0) * m(3, 2)) +
					m(1, 0) * (m(2, 3) * m(3, 2) - m(2, 2) * m(3, 3)));
				out(1, 1) = d * (m(2, 2) * (m(0, 0) * m(3, 3) - m(0, 3) * m(3, 0)) +
					m(2, 3) * (m(0, 2) * m(3, 0) - m(0, 0) * m(3, 2)) +
					m(2, 0) * (m(0, 3) * m(3, 2) - m(0, 2) * m(3, 3)));
				out(1, 2) = d * (m(3, 2) * (m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) +
					m(3, 3) * (m(0, 2) * m(1, 0) - m(0, 0) * m(1, 2)) +
					m(3, 0) * (m(0, 3) * m(1, 2) - m(0, 2) * m(1, 3)));
				out(1, 3) = d * (m(0, 2) * (m(1, 3) * m(2, 0) - m(1, 0) * m(2, 3)) +
					m(0, 3) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
					m(0, 0) * (m(1, 2) * m(2, 3) - m(1, 3) * m(2, 2)));
				out(2, 0) = d * (m(1, 3) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0)) +
					m(1, 0) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
					m(1, 1) * (m(2, 3) * m(3, 0) - m(2, 0) * m(3, 3)));
				out(2, 1) = d * (m(2, 3) * (m(0, 0) * m(3, 1) - m(0, 1) * m(3, 0)) +
					m(2, 0) * (m(0, 1) * m(3, 3) - m(0, 3) * m(3, 1)) +
					m(2, 1) * (m(0, 3) * m(3, 0) - m(0, 0) * m(3, 3)));
				out(2, 2) = d * (m(3, 3) * (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) +
					m(3, 0) * (m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) +
					m(3, 1) * (m(0, 3) * m(1, 0) - m(0, 0) * m(1, 3)));
				out(2, 3) = d * (m(0, 3) * (m(1, 1) * m(2, 0) - m(1, 0) * m(2, 1)) +
					m(0, 0) * (m(1, 3) * m(2, 1) - m(1, 1) * m(2, 3)) +
					m(0, 1) * (m(1, 0) * m(2, 3) - m(1, 3) * m(2, 0)));
				out(3, 0) = d * (m(1, 0) * (m(2, 2) * m(3, 1) - m(2, 1) * m(3, 2)) +
					m(1, 1) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
					m(1, 2) * (m(2, 1) * m(3, 0) - m(2, 0) * m(3, 1)));
				out(3, 1) = d * (m(2, 0) * (m(0, 2) * m(3, 1) - m(0, 1) * m(3, 2)) +
					m(2, 1) * (m(0, 0) * m(3, 2) - m(0, 2) * m(3, 0)) +
					m(2, 2) * (m(0, 1) * m(3, 0) - m(0, 0) * m(3, 1)));
				out(3, 2) = d * (m(3, 0) * (m(0, 2) * m(1, 1) - m(0, 1) * m(1, 2)) +
					m(3, 1) * (m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) +
					m(3, 2) * (m(0, 1) * m(1, 0) - m(0, 0) * m(1, 1)));
				out(3, 3) = d * (m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) +
					m(0, 1) * (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) +
					m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0)));

				return out;
			}

			static Matrix4 RotateMatrix(float fDegree)
			{
				Matrix4 mat = *this;
				float sinA = sin(Degree_TO_Radian(fDegree));
				float cosA = cos(Degree_TO_Radian(fDegree));

				mat.m[0][0] = cosA,		mat.m[0][1] = sinA, mat.m[0][2] = 0, mat.m[0][3] = 0;
				mat.m[1][0] = -sinA,	mat.m[1][1] = cosA, mat.m[1][2] = 0, mat.m[1][3] = 0;
				mat.m[2][0] = 0,		mat.m[2][1] = 0,	mat.m[2][2] = 1, mat.m[2][3] = 0,
				mat.m[3][0] = 0,		mat.m[3][1] = 0,	mat.m[3][2] = 0, mat.m[3][3] = 1;
			}
		public:
			/* 采用行优先存储格式
				x1, y1, z1, w1,
				x2, y2, z2, w2,
				x3, y3, z3, w3,
				dx,dy,dz,    1
			*/
			float m_fValue[4][4];
			/*
				ps 列优先存储
				x1, x2, x3, dx,
				y1, y2, y3, dy,
				z1, z2, z3, dz,
				w1,w2,w3, 1
			*/
		};
	}
}


