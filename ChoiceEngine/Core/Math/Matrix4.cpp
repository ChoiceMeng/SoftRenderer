#include "Matrix4.h"

namespace CE
{
	namespace Math
	{
		Matrix4::Matrix4(void)
		{
			memset(m_fValue, 0, sizeof(float)* 16);
		}

		Matrix4::~Matrix4(void)
		{
		}
	}
}
