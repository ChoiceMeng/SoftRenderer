#pragma once
#include "Math/Vector4.h"
#include "Core/Color.h"
using namespace CE::Math;
using namespace CE::Core;

namespace CE
{
	namespace Core
	{
		class CVertex
		{
		public:
			Vector4 mVertex;
			Vector4 mVertexView;
			Vector4 mNormal;
			Vector4 mUv;

			CVertex(){}
		};

		class CFace
		{
		public:
			bool	isCulled;				// is culled
			int		mVertIndex[3];			// vertex index
			CColor	mColor[3];

			CFace():isCulled(false)
			{
				mColor[0].r = mColor[0].g = mColor[0].b = 255;
				mColor[1].r = mColor[1].g = mColor[1].b = 255;
				mColor[2].r = mColor[2].g = mColor[2].b = 255;
			}
		};
	}
}