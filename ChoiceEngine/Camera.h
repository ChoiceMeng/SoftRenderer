#pragma once
#include "Pre.h"

class CCamera
{
public:
	CCamera():m_WorldPos(0, 0, 0), m_LookAt(0, 0, 1), 
	~CCamera();
private:
	Vector3 m_WorldPos;
	Vector3 m_LookAt;
	Vector3 m_Up;

	float m_Fov;
	float m_Near;
	float m_Far;

	float m_Aspect;

};