#pragma once
#include "Pre.h"
#include "Color.h"

class CLight
{
public:
	CLight(void);
	~CLight(void);
private:
	Vector4 m_dWorldPos;
	bool	m_bEnable;

	float	m_fShininess; // �߹�ϵ�����ڼ���phongģ��ʱʹ��
	/*
	Phongģ��(���淴��)

	Phongģ����Ϊ���淴��Ĺ�ǿ�뷴����ߺ����ߵļн���أ�

	Ispec = Ks * Il * ( dot(V,R) )^Ns

	����Ks Ϊ���淴��ϵ����Ns�Ǹ߹�ָ����V��ʾ�Ӷ��㵽�ӵ�Ĺ۲췽��R������ⷽ�����ڷ����ķ���R����ͨ������ⷽ��L(�Ӷ���ָ���Դ)������ķ����������
	R + L = 2 * dot(N, L) * N  �� R = 2 * dot(N,L) * N - L

	�������յļ���ʽΪ��

	Ispec = Ks * Il * ( dot(V, (2 * dot(N,L) * N �C L ) )^Ns
	*/
	Color	m_dAmbient;
	Color	m_dDiffuse;
	Color	m_dSpecular;

	float	m_fKAmbient;
	float	m_fKDiffuse;
	float	m_fKSpecular;
};

