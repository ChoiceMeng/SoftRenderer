#pragma once
#include "Color.h"
#include "Vector4.h"
using namespace CE::Core;
using namespace CE::Math;

// ��ʱʹ�÷�����Դ
class _declspec(dllexport)  CLight
{
public:
	CLight(void);
	~CLight(void);
public:
	Vector4 m_dWorldPos;
	bool	m_bEnable;

	float	m_fShininess; // �߹�ϵ����ʹ��phongģ�ͼ��㾵�淴��ʱʹ��
	
	CColor	m_dAmbient;
	CColor	m_dDiffuse;
	CColor	m_dSpecular;

	float	m_fKAmbient;
	float	m_fKDiffuse;
	float	m_fKSpecular;
};

