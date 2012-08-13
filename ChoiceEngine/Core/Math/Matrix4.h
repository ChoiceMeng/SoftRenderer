#pragma once
#include <cmath>
#include <memory.h>
#include "Vector4.h"
#include "Vector3.h"
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

				// 见3D数学基础图形与游戏开发 P122 或 WIZ笔记 利用代数余子式计算N*N阶矩阵行列式
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

				// 见3D数学基础图形与游戏开发 P122 或 WIZ笔记 利用代数余子式计算N*N阶矩阵行列式
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

			inline static void RotateX(Matrix4& mat, float fDegree)
			{
				float sinA = sin(Degree_TO_Radian(fDegree));
				float cosA = cos(Degree_TO_Radian(fDegree));

				mat(0, 0) = 1, mat(0, 1) = 0, mat(0, 2) = 0, mat(0, 3) = 0,
				mat(1, 0) = 0, mat(1, 1) = cosA, mat(1, 2) = sinA, mat(1, 3) = 0,
				mat(2, 0) = 0, mat(2, 1) = -sinA, mat(2, 2) = cosA, mat(2, 3) = 0,
				mat(3, 0) = 0, mat(3, 1) = 0, mat(3, 2) = 0, mat(3, 3) = 1;
			}

			inline static void RotateY(Matrix4& mat, float fDegree)
			{
				float sinA = sin(Degree_TO_Radian(fDegree));
				float cosA = cos(Degree_TO_Radian(fDegree));

				mat(0, 0) = cosA, mat(0, 1) = 0, mat(0, 2) = -sinA, mat(0, 3) = 0,
					mat(1, 0) = 0, mat(1, 1) = 1, mat(1, 2) = 0, mat(1, 3) = 0,
					mat(2, 0) = sinA, mat(2, 1) = 0, mat(2, 2) = cosA, mat(2, 3) = 0,
					mat(3, 0) = 0, mat(3, 1) = 0, mat(3, 2) = 0, mat(3, 3) = 1;
			}

			inline static void RotateZ(Matrix4& mat, float fDegree)
			{
				float sinA = sin(Degree_TO_Radian(fDegree));
				float cosA = cos(Degree_TO_Radian(fDegree));

				mat(0, 0) = cosA, mat(0, 1) = sinA, mat(0, 2) = 0, mat(0, 3) = 0,
					mat(1, 0) = -sinA, mat(1, 1) = cosA, mat(1, 2) = 0, mat(1, 3) = 0,
					mat(2, 0) = 0, mat(2, 1) = 1, mat(2, 2) = 0, mat(2, 3) = 0,
					mat(3, 0) = 0, mat(3, 1) = 0, mat(3, 2) = 0, mat(3, 3) = 1;
			}

			inline static void RotateByVector(Matrix4& mat, Vector3 n, float fDegree)
			{
				float sinA = sin(Degree_TO_Radian(fDegree));
				float cosA = cos(Degree_TO_Radian(fDegree));

				float oneCosA = 1 - cosA;

				mat.m_fValue[0][0] = n.m_fX * n.m_fX * oneCosA + cosA,					mat.m_fValue[0][1] = n.m_fX * n.m_fY * oneCosA + n.m_fX * sinA,		mat.m_fValue[0][2] = n.m_fX * n.m_fZ * oneCosA - n.m_fY * sinA,		mat.m_fValue[0][3] = 0;
				mat.m_fValue[1][0] = n.m_fX * n.m_fY * oneCosA - n.m_fZ * sinA,		mat.m_fValue[1][1] = n.m_fY * n.m_fY * oneCosA + cosA,						mat.m_fValue[1][2] = n.m_fY * n.m_fZ * oneCosA + n.m_fX * sinA,		mat.m_fValue[1][3] = 0;
				mat.m_fValue[2][0] = n.m_fX * n.m_fZ * oneCosA + n.m_fY * sinA,		mat.m_fValue[2][1] = n.m_fY * n.m_fZ * oneCosA - n.m_fX * sinA,		mat.m_fValue[2][2] = n.m_fZ * n.m_fZ * oneCosA + cosA,					mat.m_fValue[2][3] = 0,
				mat.m_fValue[3][0] = 0,																		mat.m_fValue[3][1] = 0,																		mat.m_fValue[3][2] = 0,																		mat.m_fValue[3][3] = 1;
			}

			// 获取平移变换矩阵
			static void TranslateMatrix44(Matrix4 &mat, float x, float y, float z)
			{
				mat.m_fValue[0][0] = 1, mat.m_fValue[0][1] = 0, mat.m_fValue[0][2] = 0, mat.m_fValue[0][3] = 0;
				mat.m_fValue[1][0] = 0, mat.m_fValue[1][1] = 1, mat.m_fValue[1][2] = 0, mat.m_fValue[1][3] = 0;
				mat.m_fValue[2][0] = 0, mat.m_fValue[2][1] = 0, mat.m_fValue[2][2] = 1, mat.m_fValue[2][3] = 0,
					mat.m_fValue[3][0] = x, mat.m_fValue[3][1] = y, mat.m_fValue[3][2] = z, mat.m_fValue[3][3] = 1;
			}

			//--------------------------------------------------------------------------

			// 获取缩放变换矩阵
			static void ScaleMatrix44(Matrix4 &mat, float x, float y, float z)
			{
				//return FMatrix44(x, 0, 0, 0,
				//				0, y, 0, 0,
				//				0, 0, z, 0,
				//				0, 0, 0, 1);

				mat.m_fValue[0][0] = x, mat.m_fValue[0][1] = 0, mat.m_fValue[0][2] = 0, mat.m_fValue[0][3] = 0;
				mat.m_fValue[1][0] = 0, mat.m_fValue[1][1] = y, mat.m_fValue[1][2] = 0, mat.m_fValue[1][3] = 0;
				mat.m_fValue[2][0] = 0, mat.m_fValue[2][1] = 0, mat.m_fValue[2][2] = z, mat.m_fValue[2][3] = 0,
				mat.m_fValue[3][0] = 0, mat.m_fValue[3][1] = 0, mat.m_fValue[3][2] = 0, mat.m_fValue[3][3] = 1;
			}

			// 使用UVN相机系统构造观察矩阵
			static void ViewMatrix(Matrix4& mat, const Vector3& eye, const Vector3& at, const Vector3& up)
			{
				// note:等号右边必须为常数
				// Vector3 temp = at-eye;
				Vector3 z = (at-eye).Nomalize();
 				Vector3 x = up.CrossVector(z).Nomalize();
 				Vector3 y = z.CrossVector(x).Nomalize();
 
 				mat.m_fValue[0][0] = x.m_fX,				mat.m_fValue[0][1] = y.m_fX,				mat.m_fValue[0][2] = z.m_fX,				mat.m_fValue[0][3] = 0;
 				mat.m_fValue[1][0] = x.m_fY,				mat.m_fValue[1][1] = y.m_fY,				mat.m_fValue[1][2] = z.m_fY,				mat.m_fValue[1][3] = 0;
 				mat.m_fValue[2][0] = x.m_fZ,				mat.m_fValue[2][1] = y.m_fZ,				mat.m_fValue[2][2] = z.m_fZ,				mat.m_fValue[2][3] = 0,
 				// eye在x,y,z基上的投影 仔细想想为什么
 				mat.m_fValue[3][0] = -x.DotVector(eye),		mat.m_fValue[3][1] = -y.DotVector(eye),		mat.m_fValue[3][2] = -z.DotVector(eye),		mat.m_fValue[3][3] = 1;
			}

			/*
			我们有y’ = y * cot(fov/2) / z。这下完了，这是一个非线性变换，怎么用矩阵计算来完成呢？还好我们有w这个分量。注意到我们在做投影变换之前所进行的两次坐标变换——world变换和view变换，他们只是一系列旋转平移和缩放变换的叠加。仔细观察这些变换矩阵，你会发现它们其实不会影响向量的w分量。换句话说，只要不是故意，一个w分量等于1的向量，再来到投影变换之前他的w分量仍旧等于1。好的，接下来我们让w’= w*z, 新的w就记录下了view空间中的z值。同时在y分量上我们退而求其次，只要做到y’ = y * cot(fov/2)。那么，在做完线性变换之后，我们再用向量的y除以w，就得到了我们想要的最终的y值。
			x分量的变换可以如法炮制，只是fov要换一换。事实上，很多用以生成投影变换矩阵的函数都使用了aspect这个参数。这个参数给出了视平截体截面的纵横比(这个比值应与view port的纵横比相等，否则变换结果会失真)。如果我们按照惯例，定义aspect = size of X / size of Y。那么我们就可以继续使用同一个fov而给出x分量的变换规则：x’ = x * cot(fov/2) / aspect。
			现在只剩下z分量了。我们所渴望的变换应将z = Znear 变换到z = 0，将z = Zfar变换到z = 1。这个很简单，但是等等，x, y最后还要除以w，你z怎能例外。既然也要除，那么z = Zfar 就不能映射到z = 1了。唔，先映射到z = Zfar试试。于是，有z’ = Zfar*(z-Znear)/(Zfar – Znear)(线性插值原理:z’/(z-Znear) = Zfar(Zfar – Znear)。接下来，看看z’/z的性质。令f(z) = z’/z = Zfar*(z-Znear)/（z*(Zfar – Znear)）。
			则f’(z) = Zfar * Znear / ( z^2 * (Zfar –Znear )), 显而易见f’(z) > 0。所以除了z = 0是一个奇点，函数f(z)是一个单调增的函数。因此，当Znear≤z≤Zfar时，f(Znear)≤f(z)≤f(Zfar),
			即0≤f(z)≤1。
			至此，我们可以给出投影变换的表达式了。
			x’ = x*cot(fov/2)/aspect
			y’ = y*cot(fov/2)
			z’ = z*Zfar / ( Zfar – Znear ) – Zfar*Znear / ( Zfar – Znear )
			w’ = z
			这里的坐标已经归一化了，均落在-1,1的区间里
			以矩阵表示，则得到变换矩阵如下，
			cot(fov/2)/aspect 0 0 0
			0 cot(fov/2) 0 0
			0 0 Zfar/(Zfar-Znear) 1
			0 0 -Zfar*Znear/(Zfar-Znear) 0。
			做完线性变换之后，再进行所谓的“归一化”，即用w分量去除结果向量。
			*/
			// fov:俯仰视角 aspect:分辨率 zn:近裁面 zf:远裁面
			static void ProjectMatrix(Matrix4& mat, float fov, float aspect, float zn, float zf)
			{
				mat.m_fValue[0][0] = 1/tan(fov/2),	mat.m_fValue[0][1] = 0,								mat.m_fValue[0][2] = 0,						mat.m_fValue[0][3] = 0;
				mat.m_fValue[1][0] = 0,					mat.m_fValue[1][1] = 1/tan(fov/2)/aspect,	mat.m_fValue[1][2] =0,							mat.m_fValue[1][3] = 0;
				mat.m_fValue[2][0] = 0,					mat.m_fValue[2][1] =0,									mat.m_fValue[2][2] = zf/(zf-zn),			mat.m_fValue[2][3] = 1,	// 存放z值
				mat.m_fValue[3][0] = 0,					mat.m_fValue[3][1] = 0,								mat.m_fValue[3][2] = -zf*zn/(zf-zn),		mat.m_fValue[3][3] = 0;	
			}

			/*
			从投影坐标变换到屏幕坐标 ： 
			alpha = 0.5 * screen_width - 0.5
			beta = 0.5 * screen_height - 0.5
			screen_x = alpha + alpha * x
			screen_y = beta - beta * y
			这公式到底是怎么回事呢 ？ 咋这么一看，很难看出来，于是，我绝对对它进行提取公因式，可以得到新的公式 ：
			screen_x = alpha * ( 1 + x )
			screen_y = beta * ( 1 - y )
			我们知道，alpha 是半个屏幕的宽度、beta 是半个屏幕的高度，那么，我们对 x 的三个特殊值进行计算，看看结果正不正确 ：
			假设投影后，x 的值为 -1，那么 x 的屏幕坐标就是 ：
			screen_x = alpha * ( 1 - 1 ) = alpha * 0 = 0
			假设投影后，x 的值为 0，那么 x 的屏幕坐标就是 ：
			screen_x = alpha * ( 1 + 0 ) = alpha * 1 = 0.5 * screen_width - 0.5
			假设投影后，x 的值为 1，那么 x 的屏幕坐标就是：
			screen_x = alpha * ( 1 + 1 ) = alpha * 2 = screen_width - 0.5
			非常正确 ！！ x 从 [ -1, 1 ) 的区间正确地映射到了屏幕的 [ 0, screen_width ) 区间 ！！
			*/
			static void ScreenMatrix(Matrix4& mat, int width, int height)
			{
				int alpha = 0.5*width;
				int beta = 0.5*height;

				mat.m_fValue[0][0] = alpha,				mat.m_fValue[0][1] = 0,					mat.m_fValue[0][2] = 0,				mat.m_fValue[0][3] = 0;
				mat.m_fValue[1][0] = 0,					mat.m_fValue[1][1] = -beta,				mat.m_fValue[1][2] =0,				mat.m_fValue[1][3] = 0;
				mat.m_fValue[2][0] = 0,					mat.m_fValue[2][1] =0,						mat.m_fValue[2][2] =1,			mat.m_fValue[2][3] = 0,
				mat.m_fValue[3][0] = alpha,				mat.m_fValue[3][1] = beta,				mat.m_fValue[3][2] = 0,		mat.m_fValue[3][3] = 1;	
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


