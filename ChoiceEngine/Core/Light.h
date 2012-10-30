#pragma once
#include "Color.h"
#include "Vector4.h"
using namespace CE::Core;
using namespace CE::Math;

// 暂时使用方向光光源
class _declspec(dllexport)  CLight
{
public:
	CLight(void);
	~CLight(void);
public:
	Vector4 m_dWorldPos;
	bool	m_bEnable;

	float	m_fShininess; // 高光系数，使用phong模型计算镜面反射时使用
	
	CColor	m_dAmbient;
	CColor	m_dDiffuse;
	CColor	m_dSpecular;

	float	m_fKAmbient;
	float	m_fKDiffuse;
	float	m_fKSpecular;
};

