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

	float	m_fShininess; // 高光系数，在计算phong模型时使用
	/*
	Phong模型(镜面反射)

	Phong模型认为镜面反射的光强与反射光线和视线的夹角相关：

	Ispec = Ks * Il * ( dot(V,R) )^Ns

	其中Ks 为镜面反射系数，Ns是高光指数，V表示从顶点到视点的观察方向，R代表反射光方向。由于反射光的方向R可以通过入射光方向L(从顶点指向光源)和物体的法向量求出，
	R + L = 2 * dot(N, L) * N  即 R = 2 * dot(N,L) * N - L

	所以最终的计算式为：

	Ispec = Ks * Il * ( dot(V, (2 * dot(N,L) * N – L ) )^Ns
	*/
	Color	m_dAmbient;
	Color	m_dDiffuse;
	Color	m_dSpecular;

	float	m_fKAmbient;
	float	m_fKDiffuse;
	float	m_fKSpecular;
};

