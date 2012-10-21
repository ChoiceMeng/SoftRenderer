#include "Graphics.h"

HINSTANCE GHInstance;
CGraphics::CGraphics(void)
{
}


CGraphics::~CGraphics(void)
{
}

bool CGraphics::InitGDI( HINSTANCE hinstance )
{
	GHInstance = hinstance;

	// CreateCompatibleDC创建上下文环境(内存显示设备) CreateCompatibleBitmap创建位图
	mBufferedHDC = ::CreateCompatibleDC(NULL);

	////////////////////
	BITMAPINFO info					= {0};    
	info.bmiHeader.biSize			= sizeof(info.bmiHeader);    
	info.bmiHeader.biWidth			= SCREEN_WIDTH;
	info.bmiHeader.biHeight			= -SCREEN_HEIGHT;    
	info.bmiHeader.biPlanes			= 1;    
	info.bmiHeader.biBitCount		= 32;    
	info.bmiHeader.biCompression	= BI_RGB;    
	info.bmiHeader.biSizeImage		= SCREEN_WIDTH * SCREEN_HEIGHT * 32 / 8;  

	// 创建一块内存纹理并获取其数据指针
	void* pBits = NULL;
	mBufferedHandle = ::CreateDIBSection(mBufferedHDC, &info, DIB_RGB_COLORS, &pBits, NULL, 0);
	::SelectObject(mBufferedHDC, mBufferedHandle);

	//here: "dib.dsBm.bmBits" will points to the pixels of hdib.
	::GetObject(mBufferedHandle , sizeof(DIBSECTION), &mDIBSection);
	mDatas = (BYTE*)mDIBSection.dsBm.bmBits;
	mPitch = mDIBSection.dsBm.bmWidthBytes;

	// 设置刷新区域大小
	::SetRect(&mBufferSize, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//GPen = (HPEN)::GetStockObject(WHITE_PEN); 
	mPen = ::CreatePen(PS_SOLID, 1, RGB(255, 255, 255)); 
	::SelectObject(mBufferedHDC, mPen);

	mBgBrush= ::CreateSolidBrush(RGB(0, 0, 255));
	::SelectObject (mBufferedHDC, mBgBrush) ;

	// 设置字体
	HFONT hfnt = (HFONT)::GetStockObject(OEM_FIXED_FONT); 
	::SelectObject(mBufferedHDC, hfnt);
	// 设置文字背景为透明色
	::SetBkMode(mBufferedHDC, TRANSPARENT); 

	mZBuffer = new float[SCREEN_WIDTH * SCREEN_HEIGHT];
	memset(mZBuffer, 0, sizeof(float) * SCREEN_WIDTH * SCREEN_HEIGHT);

	return true;
}

// 关闭绘图系统
void CGraphics::ShutdownGraphicss()
{
	::DeleteObject(mPen);
	::DeleteObject(mBgBrush);
	::DeleteObject(mBufferedHandle);
	::DeleteDC(mBufferedHDC);

	//SafeDeleteArray(mFaces);
	//SafeDeleteArray(mVertexs);
}

void CGraphics::BuildMatrix()
{
	assert(m_pCamera != NULL);
	// 摄像机坐标系变换矩阵
	Matrix4::ViewMatrix(mMatrix[TS_VIEW], m_pCamera->m_WorldPos, m_pCamera->m_LookAt, m_pCamera->m_Up);
	// 透视坐标系变换矩阵
	Matrix4::ProjectMatrix(mMatrix[TS_PROJECT],	Degree_TO_Radian(m_pCamera->m_Fov), 
		(float)GetScreenWidth() / (float)GetScreenHeight(), 
		m_pCamera->m_Near, m_pCamera->m_Far);
	// 屏幕坐标系变换矩阵
	Matrix4::ScreenMatrix(mMatrix[TS_SCRREN], GetScreenWidth(), GetScreenHeight());
}

void CGraphics::TranslateToViewSpace()
{
	Matrix4 mat = mMatrix[TS_LOCAL] * mMatrix[TS_WORLD] * mMatrix[TS_VIEW];
	for (int i = 0; i < mVertexNum; i++)
	{
		CVertex& ver = mVertexs[i];
		// 顶点变换到观察坐标系
		ver.mVertex = Vec4MulMat4W(ver.mVertex, mat);
		// 顶点法线
		ver.mNormal = Vec4MulMat4W(ver.mNormal, mat);

		ver.mVertexView = ver.mVertex;
	}

	Matrix4 invViewMat;
	InverseMatrix4(invViewMat, mMatrix[TS_VIEW]);
	Matrix4 invWorldMat;
	InverseMatrix4(invWorldMat, mMatrix[TS_WORLD]);
	Matrix4 invLocalMat;
	InverseMatrix4(invLocalMat, mMatrix[TS_LOCAL]);
	m_matWorld2Local = invViewMat * invWorldMat * invLocalMat;
}

void CGraphics::TranslateToProjectiveSpace()
{
	for (int i = 0; i < mVertexNum; i++)
	{
		CVertex& ver = mVertexs[i];
		ver.mVertex = Vec4MulMat4W(ver.mVertex, mMatrix[TS_PROJECT]);
		// 执行透视除法, 将齐次坐标变换到笛卡尔积坐标系
		ver.mVertex /= ver.mVertex.w;
	}
}

void CGraphics::TranslateToScreenSpace( const CFace& face, Vector4& v0, Vector4& v1, Vector4& v2 )
{
	v0 = Vec4MulMat4(mVertexs[face.mVertIndex[0]].mVertex, mMatrix[TS_SCRREN]);
	v1 = Vec4MulMat4(mVertexs[face.mVertIndex[1]].mVertex, mMatrix[TS_SCRREN]);
	v2 = Vec4MulMat4(mVertexs[face.mVertIndex[2]].mVertex, mMatrix[TS_SCRREN]);
}

void CGraphics::SetVertex( CVertex* vertexs, int vertexNum )
{
	if (mVertexNum != vertexNum)
	{
		delete[] mVertexs;
		mVertexs = new CVertex[vertexNum];
		mVertexNum = vertexNum;
	}	
	memcpy(mVertexs, vertexs, sizeof(CVertex) * vertexNum);
}

void CGraphics::SetCamera( CCamera* camera )
{
	assert(camera);
	m_pCamera = camera;
}

void CGraphics::SetTransform( ETS_Transform space, const Matrix4& mat )
{
	assert(space < TS_NUM && space >= TS_LOCAL);
	mMatrix[space] = mat;
}

void CGraphics::SetTexture( /*int index, */CTexture* texture )
{
	assert(texture);
	mTextures = texture;
}

void CGraphics::SetLight( /*int index, */CLight* light )
{
	assert(light);
	mLights = light;
}

const static float DARK_COLOR_FACTOR = 0.05f;
void CGraphics::ProcessLight()
{
	CLight* pLight = mLights;
	if(pLight != NULL && pLight->m_bEnable)
	{
		// 目前就一盏灯
		mLightPositionView = Vec4MulMat4(pLight->m_dWorldPos, mMatrix[TS_VIEW]);

		if(mShadeType == SHADE_FLAT) // 平面着色
		{
			for(int i = 0; i < mFaceNum; ++i)
			{
				CFace& pFace = mFaces[i];
				if(!pFace.isCulled)
				{
					Vector4 edge0 = mVertexs[pFace.mVertIndex[0]].mVertex - mVertexs[pFace.mVertIndex[1]].mVertex;
					Vector4 edge1 = mVertexs[pFace.mVertIndex[0]].mVertex - mVertexs[pFace.mVertIndex[2]].mVertex;

					Vector4 faceNormal = edge0.CrossVector(edge1);
					Vector4 lightVect = (mLightPositionView - mVertexs[pFace.mVertIndex[0]].mVertex).Nomalize();

					// 颜色系数
					float dot = faceNormal.DotVector(lightVect);
					if(dot < 0)
						dot = DARK_COLOR_FACTOR;

					pFace.mColor[0] *= dot;
					pFace.mColor[1] *= dot;
					pFace.mColor[2] *= dot;
				}
			}
		}
		else if(SHADE_GAUROUD == mShadeType)
		{
			// 对面的三个顶点分别计算光线夹角
			for(int i = 0; i < mFaceNum; ++i)
			{
				CFace& face = mFaces[i];
				if(!face.isCulled)
				{
					Vector4 v0Normal = mVertexs[mFaces[i].mVertIndex[0]].mVertex.Nomalize();
					Vector4 v1Normal = mVertexs[mFaces[i].mVertIndex[1]].mVertex.Nomalize();
					Vector4 v2Normal = mVertexs[mFaces[i].mVertIndex[2]].mVertex.Nomalize();

					Vector4 lightDir0 = (mLightPositionView - mVertexs[mFaces[i].mVertIndex[0]].mVertex).Nomalize();
					Vector4 lightDir1 = (mLightPositionView - mVertexs[mFaces[i].mVertIndex[1]].mVertex).Nomalize();
					Vector4 lightDir2 = (mLightPositionView - mVertexs[mFaces[i].mVertIndex[2]].mVertex).Nomalize();

					float dot = 1.0f;
					dot = v0Normal.DotVector(lightDir0);
					if(dot < 0)
						dot = DARK_COLOR_FACTOR;
					mFaces->mColor[0] *= dot;

					dot = v1Normal.DotVector(lightDir1);
					if(dot < 0)
						dot = DARK_COLOR_FACTOR;
					mFaces->mColor[1] *= dot;

					dot = v2Normal.DotVector(lightDir2);
					if(dot < 0)
						dot = DARK_COLOR_FACTOR;
					mFaces->mColor[2] *= dot;
				}
			}
		}
		else if(SHADE_PHONG_Vertex == mShadeType)
		{
			for(int i = 0; i < mFaceNum; ++i)
			{
				CFace& face = mFaces[i];
				if(!face.isCulled)
				{
					Vector4 pos0 = mVertexs[face.mVertIndex[0]].mVertex;
					Vector4 normal0 = mVertexs[face.mVertIndex[0]].mNormal;
					face.mColor[0] = PhongCal(pLight, mLightPositionView, pos0, normal0);

					Vector4 pos1 = mVertexs[face.mVertIndex[1]].mVertex;
					Vector4 normal1 = mVertexs[face.mVertIndex[1]].mNormal;
					face.mColor[1] = PhongCal(pLight, mLightPositionView, pos1, normal1);

					Vector4 pos2 = mVertexs[face.mVertIndex[2]].mVertex;
					Vector4 normal2 = mVertexs[face.mVertIndex[2]].mNormal;
					face.mColor[2] = PhongCal(pLight, mLightPositionView, pos2, normal2);
				}
			}
		}
	}
}

void CGraphics::SetFace( CFace* faces, int faceNum )
{
	if (mFaceNum != faceNum)
	{
		delete[] mFaces;
		mFaces = new CFace[faceNum];
		mFaceNum = faceNum;
	}
	memcpy(mFaces, faces, sizeof(CFace) * faceNum);
	mFaceNum += faceNum;
}

CE::Core::CColor CGraphics::PhongCal( CLight* pLight, const Vector4& lightPos, const Vector4& vertexPosView, const Vector4& normal )
{
	if(!pLight)
		return CColor(255, 255, 255);

	// 必须标准化
	Vector4 n = normal.Nomalize();
	// 顶点到光源的向量
	Vector4 l = (lightPos - vertexPosView).Nomalize();
	// 顶点到观察点向量(相机位于原点)
	Vector4 v = (Vector4(0,0,0,0) - vertexPosView).Nomalize();
	// 反射光向量，参看笔记
	Vector4 r = (n * ( 2 * Max(l.DotVector(n), 0)) - l).Nomalize();

	// phone模型计算公式
	CColor diffuse = pLight->m_dDiffuse * (pLight->m_fKDiffuse * Max(l.DotVector(n), DARK_COLOR_FACTOR));
	CColor respec = pLight->m_dSpecular * (pLight->m_fKSpecular * pow(Max(v.DotVector(r), 0), pLight->m_fShininess));

	return diffuse + respec;
}

void SwapVectex(	Vector4& v0, Vector4& v0V, Vector4& n0, Vector4& uv0, CColor& c0, 
	Vector4& v1, Vector4& v1V, Vector4& n1, Vector4& uv1, CColor& c1)
{
	std::swap(v0, v1);
	std::swap(uv0, uv1);
	std::swap(c0, c1);
	std::swap(v0V, v1V);
	std::swap(n0, n1);
}

bool IsInScreen(const Vector4& v0, const Vector4& v1, const Vector4& v2, int screenWidth, int screenHeight)
{
	if(v0.y > screenHeight || v2.y < 0)
		return false;

	int left = Min(v0.x, Min(v1.x, v2.x));
	int right = Max(v0.x, Max(v1.x, v2.x));

	if(left > screenWidth || right < 0)
		return false;
}

void CGraphics::ProcessRasterize()
{
	// 光栅化
	for (int i = 0; i < mFaceNum; i++)
	{
		CFace& face = mFaces[i];
		if (!face.isCulled)
		{
			// face的顶点已经在TranslateToProjectiveSpace中变换到投影空间了
			// 将位于透视坐标系下经过透视除法的顶点变换到屏幕坐标系
			// 这里不对原始数据进行修改, 而是用他们的拷贝
			Vector4 v0, v1, v2;
			TranslateToScreenSpace(face, v0, v1, v2);

			// normal已经在TanslateToViewSpace中变换到观察空间了
			// 摄像机坐标系下面的顶点, for phong
			Vector4 v0V = mVertexs[face.mVertIndex[0]].mVertexView;
			Vector4 v1V = mVertexs[face.mVertIndex[1]].mVertexView;
			Vector4 v2V = mVertexs[face.mVertIndex[2]].mVertexView;

			// 摄像机坐标系下面的法线, for phong
			Vector4 n0 = mVertexs[face.mVertIndex[0]].mNormal;
			Vector4 n1 = mVertexs[face.mVertIndex[1]].mNormal;
			Vector4 n2 = mVertexs[face.mVertIndex[2]].mNormal;

			Vector4 uv0 = mVertexs[face.mVertIndex[0]].mUv;
			Vector4 uv1 = mVertexs[face.mVertIndex[1]].mUv;
			Vector4 uv2 = mVertexs[face.mVertIndex[2]].mUv;

			CColor c0 = face.mColor[0];
			CColor c1 = face.mColor[1];
			CColor c2 = face.mColor[2];

			RasterizeFace(i, v0, v1, v2, v0V, v1V, v2V, n0, n1, n2, uv0, uv1, uv2, c0, c1, c2);
		}
	}
}

// 光栅化三角面
void CGraphics::RasterizeFace(int faceIndex,
	Vector4& v0, Vector4& v1, Vector4& v2, 
	Vector4& v0V, Vector4& v1V, Vector4& v2V,
	Vector4& n0,  Vector4& n1, Vector4& n2,
	Vector4& uv0, Vector4& uv1, Vector4& uv2,
	CColor& c0, CColor& c1, CColor& c2)
{
	// 保证Y坐标上V0<V1<V2的顺序，再区分左三角形还是右三角形
	if(v0.y > v1.y) SwapVectex(v0, v0V, n0, uv0, c0, v1, v1V, n1, uv1, c1);
	if(v0.y > v2.y) SwapVectex(v0, v0V, n0, uv0, c0, v2, v2V, n2, uv2, c2);
	if(v1.y > v2.y) SwapVectex(v1, v1V, n1, uv1, c1, v2, v2V, n2, uv2, c2);

	if(!IsInScreen(v0, v1, v2, SCREEN_WIDTH, SCREEN_HEIGHT))
		return;

	// 由斜率公式得出，在一条直线上的三点:v0,v1,v2 有:(v1.y-v0.y)/(v1.x-v0.x) = (v2.y-v0.y)/（v2.x-v0.x)
	Vector3 vNew1;
	vNew1.x = (v1.y-v0.y)/(v2.y-v0.y)*(v2.x-v0.x)+v0.x;
	vNew1.y = v1.y;
	// z也满足斜率公式，但是在投影空间中z只按照1/z线性变化
	vNew1.z = 1 / ( (1/v1.z-1/v0.z)/(1/v2.z-1/v0.z)*(1/v2.z-1/v0.z) + 1/v0.z );

	// 提取斜率公式中公有部分
	float factor = (v1.y-v0.y)/(v2.y-v0.y);
	float vNew1Z = (1/v1.z-1/v0.z)/(1/v2.z-1/v0.z)*(1/v2.z-1/v0.z) + 1/v0.z;

	// 新顶点的normal和uv也需要重新计算
	Vector4 new1Normal = (n2  / v2.z - n0  / v0.z)	* factor + n0  / v0.z;
	// view坐标系坐标
	Vector4 newvV		= (v2V / v2.z - v0V / v0.z) * factor + v0V / v0.z;

	float u = (uv2.x  / v2.z - uv0.x / v0.z ) * factor + uv0.x / v0.z;	
	float v = (uv2.y  / v2.z - uv0.y / v0.z ) * factor + uv0.y / v0.z;

	// 透视除法
	new1Normal	/= vNew1Z;
	newvV		/= vNew1Z;
	u			/= vNew1Z;
	v			/= vNew1Z;

	float r = (c2.r - c0.r) * factor + c0.r;
	float g = (c2.g - c0.g) * factor + c0.g;
	float b = (c2.b - c0.b) * factor + c0.b;

	CColor color;
	color.r = (unsigned char)r;
	color.g = (unsigned char)g;
	color.b = (unsigned char)b;
}

void CGraphics::DrawPrimitives()
{
	BuildMatrix();

	TranslateToViewSpace();

	ProcessLight();

	TranslateToProjectiveSpace();

	ProcessRasterize();
}

void	CGraphics::ClearBuffer(const CColor& c)
{
	::FillRect(mBufferedHDC, &mBufferSize, mBgBrush);
	// 重置深度缓存
	// 注 : 这里memset只能对Int类型的数组进行初始化, 所以这里直接使用了Int类型
	// 而没有使用float类型, 应该使用float
	::memset(mZBuffer, 0, sizeof(float) * SCREEN_WIDTH * SCREEN_HEIGHT);
}

void CGraphics::FlipBuffer(HDC hdc)
{
	// 将已经绘制好的缓冲区递交给Graphics在屏幕上绘制, 并将当前缓冲区设置为另一个缓冲区
	::BitBlt(hdc,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,mBufferedHDC,0,0,SRCCOPY);
}