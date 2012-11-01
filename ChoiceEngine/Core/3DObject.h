#pragma once
#include "../Model/3DS.H"
// #include "Geometry.h"

namespace CE
{
	namespace Core
	{
		class _declspec(dllexport) C3DObject
		{
		public:
			C3DObject(MAXOBJ::t3DModel* model);
			~C3DObject(void);
		public:
			int m_nVetexCount;
			int m_nFaceCount;
			class CVertex* m_pVertexList;
			struct CFace* m_pFace;
		};
	}
}
