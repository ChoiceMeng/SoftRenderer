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
				�������������ҳ˾��������������˾�������ʹ���������ҳ˾���ʱʹ�õı������о��󡣹����о����о��������

				�о���
				����������Ϊǰ���У����һ��Ϊλ�Ʊ任
				����ʱ�����ҽӺϣ���ߵı任��Ӧ��
				�任����ʱΪvector * matrix
				�о���
				����������Ϊǰ���У����һ��Ϊλ�Ʊ任
				����ʱ���ҵ���Ӻϣ��ұߵı任��Ӧ�ã�ע�����*������ĽӺ�˳���෴
				�任����ʱΪmatrix * vector

				DirectX�������о���OpenGL�������о���
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

			// ����ʽ
			double Matrix4::DetM()
			{
				Matrix4 mat = *this;

				// ��3D��ѧ����ͼ������Ϸ���� P122 �� WIZ�ʼ� ���ô�������ʽ����N*N�׾�������ʽ
				double d = (mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0)) * (mat(2, 2) * mat(3, 3) - mat(2, 3) * mat(3, 2)) -
					(mat(0, 0) * mat(1, 2) - mat(0, 2) * mat(1, 0)) * (mat(2, 1) * mat(3, 3) - mat(2, 3) * mat(3, 1)) +
					(mat(0, 0) * mat(1, 3) - mat(0, 3) * mat(1, 0)) * (mat(2, 1) * mat(3, 2) - mat(2, 2) * mat(3, 1)) +
					(mat(0, 1) * mat(1, 2) - mat(0, 2) * mat(1, 1)) * (mat(2, 0) * mat(3, 3) - mat(2, 3) * mat(3, 0)) -
					(mat(0, 1) * mat(1, 3) - mat(0, 3) * mat(1, 1)) * (mat(2, 0) * mat(3, 2) - mat(2, 2) * mat(3, 0)) +
					(mat(0, 2) * mat(1, 3) - mat(0, 3) * mat(1, 2)) * (mat(2, 0) * mat(3, 1) - mat(2, 1) * mat(3, 0));

				return d;
			}

			// �����
			Matrix4 Matrix4::Inverse()
			{
				double detM = DetM();

				double d = 1 / detM;

				Matrix4 out;

				// ��3D��ѧ����ͼ������Ϸ���� P122 �� WIZ�ʼ� ���ô�������ʽ����N*N�׾�������ʽ
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

			// ��ȡƽ�Ʊ任����
			static void TranslateMatrix44(Matrix4 &mat, float x, float y, float z)
			{
				mat.m_fValue[0][0] = 1, mat.m_fValue[0][1] = 0, mat.m_fValue[0][2] = 0, mat.m_fValue[0][3] = 0;
				mat.m_fValue[1][0] = 0, mat.m_fValue[1][1] = 1, mat.m_fValue[1][2] = 0, mat.m_fValue[1][3] = 0;
				mat.m_fValue[2][0] = 0, mat.m_fValue[2][1] = 0, mat.m_fValue[2][2] = 1, mat.m_fValue[2][3] = 0,
					mat.m_fValue[3][0] = x, mat.m_fValue[3][1] = y, mat.m_fValue[3][2] = z, mat.m_fValue[3][3] = 1;
			}

			//--------------------------------------------------------------------------

			// ��ȡ���ű任����
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

			// ʹ��UVN���ϵͳ����۲����
			static void ViewMatrix(Matrix4& mat, const Vector3& eye, const Vector3& at, const Vector3& up)
			{
				// note:�Ⱥ��ұ߱���Ϊ����
				// Vector3 temp = at-eye;
				Vector3 z = (at-eye).Nomalize();
 				Vector3 x = up.CrossVector(z).Nomalize();
 				Vector3 y = z.CrossVector(x).Nomalize();
 
 				mat.m_fValue[0][0] = x.m_fX,				mat.m_fValue[0][1] = y.m_fX,				mat.m_fValue[0][2] = z.m_fX,				mat.m_fValue[0][3] = 0;
 				mat.m_fValue[1][0] = x.m_fY,				mat.m_fValue[1][1] = y.m_fY,				mat.m_fValue[1][2] = z.m_fY,				mat.m_fValue[1][3] = 0;
 				mat.m_fValue[2][0] = x.m_fZ,				mat.m_fValue[2][1] = y.m_fZ,				mat.m_fValue[2][2] = z.m_fZ,				mat.m_fValue[2][3] = 0,
 				// eye��x,y,z���ϵ�ͶӰ ��ϸ����Ϊʲô
 				mat.m_fValue[3][0] = -x.DotVector(eye),		mat.m_fValue[3][1] = -y.DotVector(eye),		mat.m_fValue[3][2] = -z.DotVector(eye),		mat.m_fValue[3][3] = 1;
			}

			/*
			������y�� = y * cot(fov/2) / z���������ˣ�����һ�������Ա任����ô�þ������������أ�����������w���������ע�⵽��������ͶӰ�任֮ǰ�����е���������任����world�任��view�任������ֻ��һϵ����תƽ�ƺ����ű任�ĵ��ӡ���ϸ�۲���Щ�任������ᷢ��������ʵ����Ӱ��������w���������仰˵��ֻҪ���ǹ��⣬һ��w��������1��������������ͶӰ�任֮ǰ����w�����Ծɵ���1���õģ�������������w��= w*z, �µ�w�ͼ�¼����view�ռ��е�zֵ��ͬʱ��y�����������˶�����Σ�ֻҪ����y�� = y * cot(fov/2)����ô�����������Ա任֮����������������y����w���͵õ���������Ҫ�����յ�yֵ��
			x�����ı任�����編���ƣ�ֻ��fovҪ��һ������ʵ�ϣ��ܶ���������ͶӰ�任����ĺ�����ʹ����aspect������������������������ƽ���������ݺ��(�����ֵӦ��view port���ݺ����ȣ�����任�����ʧ��)��������ǰ��չ���������aspect = size of X / size of Y����ô���ǾͿ��Լ���ʹ��ͬһ��fov������x�����ı任����x�� = x * cot(fov/2) / aspect��
			����ֻʣ��z�����ˡ������������ı任Ӧ��z = Znear �任��z = 0����z = Zfar�任��z = 1������ܼ򵥣����ǵȵȣ�x, y���Ҫ����w����z�������⡣��ȻҲҪ������ôz = Zfar �Ͳ���ӳ�䵽z = 1�ˡ�����ӳ�䵽z = Zfar���ԡ����ǣ���z�� = Zfar*(z-Znear)/(Zfar �C Znear)(���Բ�ֵԭ��:z��/(z-Znear) = Zfar(Zfar �C Znear)��������������z��/z�����ʡ���f(z) = z��/z = Zfar*(z-Znear)/��z*(Zfar �C Znear)����
			��f��(z) = Zfar * Znear / ( z^2 * (Zfar �CZnear )), �Զ��׼�f��(z) > 0�����Գ���z = 0��һ����㣬����f(z)��һ���������ĺ�������ˣ���Znear��z��Zfarʱ��f(Znear)��f(z)��f(Zfar),
			��0��f(z)��1��
			���ˣ����ǿ��Ը���ͶӰ�任�ı��ʽ�ˡ�
			x�� = x*cot(fov/2)/aspect
			y�� = y*cot(fov/2)
			z�� = z*Zfar / ( Zfar �C Znear ) �C Zfar*Znear / ( Zfar �C Znear )
			w�� = z
			����������Ѿ���һ���ˣ�������-1,1��������
			�Ծ����ʾ����õ��任�������£�
			cot(fov/2)/aspect 0 0 0
			0 cot(fov/2) 0 0
			0 0 Zfar/(Zfar-Znear) 1
			0 0 -Zfar*Znear/(Zfar-Znear) 0��
			�������Ա任֮���ٽ�����ν�ġ���һ����������w����ȥ�����������
			*/
			// fov:�����ӽ� aspect:�ֱ��� zn:������ zf:Զ����
			static void ProjectMatrix(Matrix4& mat, float fov, float aspect, float zn, float zf)
			{
				mat.m_fValue[0][0] = 1/tan(fov/2),	mat.m_fValue[0][1] = 0,								mat.m_fValue[0][2] = 0,						mat.m_fValue[0][3] = 0;
				mat.m_fValue[1][0] = 0,					mat.m_fValue[1][1] = 1/tan(fov/2)/aspect,	mat.m_fValue[1][2] =0,							mat.m_fValue[1][3] = 0;
				mat.m_fValue[2][0] = 0,					mat.m_fValue[2][1] =0,									mat.m_fValue[2][2] = zf/(zf-zn),			mat.m_fValue[2][3] = 1,	// ���zֵ
				mat.m_fValue[3][0] = 0,					mat.m_fValue[3][1] = 0,								mat.m_fValue[3][2] = -zf*zn/(zf-zn),		mat.m_fValue[3][3] = 0;	
			}

			/*
			��ͶӰ����任����Ļ���� �� 
			alpha = 0.5 * screen_width - 0.5
			beta = 0.5 * screen_height - 0.5
			screen_x = alpha + alpha * x
			screen_y = beta - beta * y
			�⹫ʽ��������ô������ �� զ��ôһ�������ѿ����������ǣ��Ҿ��Զ���������ȡ����ʽ�����Եõ��µĹ�ʽ ��
			screen_x = alpha * ( 1 + x )
			screen_y = beta * ( 1 - y )
			����֪����alpha �ǰ����Ļ�Ŀ�ȡ�beta �ǰ����Ļ�ĸ߶ȣ���ô�����Ƕ� x ����������ֵ���м��㣬�������������ȷ ��
			����ͶӰ��x ��ֵΪ -1����ô x ����Ļ������� ��
			screen_x = alpha * ( 1 - 1 ) = alpha * 0 = 0
			����ͶӰ��x ��ֵΪ 0����ô x ����Ļ������� ��
			screen_x = alpha * ( 1 + 0 ) = alpha * 1 = 0.5 * screen_width - 0.5
			����ͶӰ��x ��ֵΪ 1����ô x ����Ļ������ǣ�
			screen_x = alpha * ( 1 + 1 ) = alpha * 2 = screen_width - 0.5
			�ǳ���ȷ ���� x �� [ -1, 1 ) ��������ȷ��ӳ�䵽����Ļ�� [ 0, screen_width ) ���� ����
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
			/* ���������ȴ洢��ʽ
				x1, y1, z1, w1,
				x2, y2, z2, w2,
				x3, y3, z3, w3,
				dx,dy,dz,    1
			*/
			float m_fValue[4][4];
			/*
				ps �����ȴ洢
				x1, x2, x3, dx,
				y1, y2, y3, dy,
				z1, z2, z3, dz,
				w1,w2,w3, 1
			*/
		};
	}
}


