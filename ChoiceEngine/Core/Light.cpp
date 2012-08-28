#include "Light.h"


CLight::CLight(void):m_bEnable(true),m_fShininess(1.0f),m_dAmbient(255, 255, 255),
	m_dDiffuse(255, 255, 255),m_dSpecular(255, 255, 255),m_fKAmbient(1.0f),m_fKDiffuse(1.0f),m_fKSpecular(1.0f)

{
}


CLight::~CLight(void)
{
}
