#pragma once
#include "Pre.h"

namespace CE
{
	class CCamera
	{
	public:
		CCamera():m_WorldPos(0, 0, 0), m_LookAt(0, 0, 1){}
		~CCamera(){}
	public:
		void MoveForward()
		{
			Vector3 step = m_LookAt - m_WorldPos;
			step = step.Normalize();
			m_WorldPos += step; // ×óÊÖ×ø±êÏµ
		}

		void MoveBackward()
		{
			Vector3 step = m_LookAt - m_WorldPos;
			m_WorldPos -= step.Normalize();
		}
		void MoveLeft()
		{
			Vector3 step = m_LookAt.CrossVector(m_Up).Normalize();
			m_WorldPos -= step;
		}
		void MoveRight()
		{
			Vector3 step = m_LookAt.CrossVector(m_Up).Normalize();
			m_WorldPos += step;
		}
		void Pitch();
		void Yaw();
		void Roll();
	public:
		Vector3 m_WorldPos;
		Vector3 m_LookAt;
		Vector3 m_Up;

		float m_Fov;
		float m_Near;
		float m_Far;

		float m_Aspect;
	};
}