#pragma once
#include <cmath>

// ��ѧ����
#define PI 3.141592654f
#define PI2 (PI * 2.0f)
#define PI_DIV_2 (PI * 0.5f)
#define PI_DIV_4 (PI * 0.25f)
#define PI_INV 0.318309886f

// ���㾫��
#define EPSILON_E6 ((float)(1E-6))

//�Ƕȱ仡��ת��, ���ȱ�Ƕ�ת��
//#define Degree_TO_Radian(deg) ((float)deg * PI / 180.0f)
//#define Radian_To_Degree(rad) ((float)rad * 180.0f / PI)
#define Degree_TO_Radian(deg) (deg * 0.017453f)
#define Radian_To_Degree(rad) (rad * 57.295780f)

// ��������������Ƿ����
inline bool EqualFloat(float l, float r)
{
	return abs(l - r) <= EPSILON_E6;
}

template <class T1, class T2> 
const T1 Min ( const T1& a, const T2& b ) {
	return (a<b)?a:b;
}

template <class T1, class T2> 
const T1 Max ( const T1& a, const T2& b ) {
	return (b<a)?a:b;
}

template<class T>
const T Abs( const T& a )
{
	return a > 0 ? a : -a;
}

// ��val�ķ�Χ������min-max֮��
template<typename T1, typename T2, typename T3>
const T1 Clamp(const T1& val, const T2& minV, const T3& maxV)
{
	if (val < minV)
		return minV;
	else if (val > maxV)
		return maxV;
	else
		return val;
}