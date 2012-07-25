#include "Matrix3.h"

namespace CE
{
	namespace Math
	{
		Matrix3::Matrix3(void)
		{
			memcpy(m_fValue, 0, sizeof(float) * 9);
		}

		Matrix3::~Matrix3(void)
		{
		}
	}
}
