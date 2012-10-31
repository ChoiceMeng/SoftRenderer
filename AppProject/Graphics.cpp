#include "Graphics.h"

const static std::string MOD_NAME_1 = "Sphere02.3DS";
const static std::string MOD_NAME_2 = "Teapot02.3DS";

const static std::string TEX_NAME_1 = "fieldstone_c.bmp";
const static std::string TEX_NAME_2 = "qipan2.bmp";

const static std::string BUMP_TEX_NAME = "fieldstone_n.bmp";

HINSTANCE GHInstance;
CGraphics::CGraphics(void)
{
	mFillType = FILL_SOLID;
	mCullType = CULL_BACK;
	mShadeType = SHADE_GAUROUD;

	mFaces			= NULL;
	mFaceNum		= 0;
	mVertexs		= NULL;
	mVertexNum		= 0;

	mMatrix[TS_LOCAL]	= IDENTITY;
	mMatrix[TS_WORLD]	= IDENTITY;
	mMatrix[TS_VIEW]	= IDENTITY;
	mMatrix[TS_PROJECT] = IDENTITY;
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

	//////////////////////////////////////////////////////////////////////////model data
	MAXOBJ::t3DModel* model = new MAXOBJ::t3DModel;
	MAXOBJ::CLoad3DS modelLoader;
	modelLoader.Import3DS(model, GetFilePath(MOD_NAME_1).c_str());
	mObject_Sphere = new C3DObject(model);

	MAXOBJ::t3DModel* model1 = new MAXOBJ::t3DModel;
	modelLoader.Import3DS(model1, GetFilePath(MOD_NAME_2).c_str());
	mObject_Teapot = new C3DObject(model1);

	mLights  = new CLight();
	mLights->m_fShininess = 10;
	mLights->m_fKSpecular = 5.5;
	mLights->m_fKDiffuse = 0.75;
	mLights->m_dWorldPos = Vector4(40, 50, 40);

	m_pCamera = new CCamera();
	m_pCamera->m_Near		= 1.0f;
	m_pCamera->m_Far		= 200.0f;
	m_pCamera->m_Fov	= 75.0f;
	m_pCamera->m_Aspect		= SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	m_pCamera->m_WorldPos = Vector3(30.0f, 30.0f, 0.001f);
	m_pCamera->m_LookAt		= Vector3(0.0f, 6.0f, 0.0f);
	m_pCamera->m_Up			= Vector3(0.0f, 1.0f, 0.0f);

	mTextures		= new CTexture(GetFilePath(TEX_NAME_1));
	//mTexture2		= new FTexture(GetFilePath(TEX_NAME_2));
	mNormalTexture	= new CTexture(GetFilePath(BUMP_TEX_NAME));

	mShadeType	= SHADE_GAUROUD;

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

	// test
	Matrix4 testMat(1,2,3,4,11,12,13,14,21,22,23,24,31,32,33,34);
	Matrix4 textOut;
	float d = testMat.DetM();

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
					Vector4 v0Normal = mVertexs[mFaces[i].mVertIndex[0]].mNormal;
					Vector4 v1Normal = mVertexs[mFaces[i].mVertIndex[1]].mNormal;
					Vector4 v2Normal = mVertexs[mFaces[i].mVertIndex[2]].mNormal;

					// 待修改:mVertex在TranslateToViewSpace中已经变成了观察坐标
					Vector4 lightDir0 = (mLightPositionView - mVertexs[mFaces[i].mVertIndex[0]].mVertex).Nomalize();
					Vector4 lightDir1 = (mLightPositionView - mVertexs[mFaces[i].mVertIndex[1]].mVertex).Nomalize();
					Vector4 lightDir2 = (mLightPositionView - mVertexs[mFaces[i].mVertIndex[2]].mVertex).Nomalize();

					float dot = 1.0f;
					dot = v0Normal.DotVector(lightDir0);
					if(dot < 0)
						dot = DARK_COLOR_FACTOR;
					face.mColor[0] *= dot;

					dot = v1Normal.DotVector(lightDir1);
					if(dot < 0)
						dot = DARK_COLOR_FACTOR;
					face.mColor[1] *= dot;

					dot = v2Normal.DotVector(lightDir2);
					if(dot < 0)
						dot = DARK_COLOR_FACTOR;
					face.mColor[2] *= dot;
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
	// mFaceNum += faceNum;
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
	Vector4 vNew1;
	vNew1.x = (v2.x-v0.x)*(v1.y-v0.y)/(v2.y-v0.y)+v0.x;
	vNew1.y = v1.y;
	// z也满足斜率公式
	vNew1.z = (v2.z-v0.z)*(v1.y-v0.y)/(v2.y-v0.y)+v0.z;

	// 提取斜率公式中公有部分
	float factor = (v1.y-v0.y)/(v2.y-v0.y);

	// 新顶点的normal和uv也需要重新计算
	Vector4 new1Normal = (n2 - n0)	* factor + n0;
	// view坐标系坐标
	Vector4 newvV		= (v2V - v0V) * factor + v0V;

	float u = ( uv2.x - uv0.x ) * factor + uv0.x;	
	float v = ( uv2.y - uv0.y ) * factor + uv0.y;

	float r = (c2.r - c0.r) * factor + c0.r;
	float g = (c2.g - c0.g) * factor + c0.g;
	float b = (c2.b - c0.b) * factor + c0.b;

	CColor color;
	color.r = (unsigned char)r;
	color.g = (unsigned char)g;
	color.b = (unsigned char)b;

	if(vNew1.x > v1.x)
	{
		// 左三角形
		//				/| v0
		//			   / |
		//			  /	 | 
		//		v1	 ----- vNew1

		//      v1------ vNew1
		//			\	 |  
		//			 \   | 
		//			  \	 |
		//			   \ |
		//				\|	
		//			     v2
		if(mShadeType == SHADE_FLAT)
		{
			RasterizeFlatFaceUp(v0, v1, vNew1, uv0, uv1, Vector4(u, v, 0.0f, 0.0f), c0, c1, color);
			RasterizeFlatFaceDown(v0, vNew1, v2,  uv0, Vector4(u, v, 0.0f, 0.0f), uv2, c1, color, c2);
		}
		else if(mShadeType == SHADE_GAUROUD || mShadeType == SHADE_PHONG_Vertex)
		{
			RasterizeGouraudFaceUp(v0, v1, vNew1, uv0, uv1, Vector4(u, v, 0.0f), c0, c1, color);
			RasterizeGouraudFaceDown(v0, vNew1, v2,  uv0, Vector4(u, v, 0.0f), uv2, c1, color, c2);
		}else
		{
			RasterizePhongFaceUp(v0, v1, vNew1, v0V, v1V, newvV, n0, n1, new1Normal, uv0, uv1, Vector4(u, v, 0.0f, 0.0f), c0, c1, color);
			RasterizePhongFaceUp(v0, vNew1, v2,  v0V, newvV, v2V,  n0, new1Normal, n2,  uv0, Vector4(u, v, 0.0f, 0.0f), uv2, c0, color, c2 );
		}
	}
	else
	{
		// 右三角形
		//			 v0
		//			 |\	 
		//			 | \  
		//			 |  \	  
		//	vNew1---v1

		// vNew1---v1
		//			 |   /
		//			 |  / 
		//			 | /
		//			 |/
		//			 v2
		if(mShadeType == SHADE_FLAT)
		{
			RasterizeFlatFaceUp(v0, vNew1, v1,  uv0, Vector4(u, v, 0.0f, 0.0f), uv1,  c0, color, c1 );
			RasterizeFlatFaceDown(vNew1, v1,  v2,  Vector4(u, v, 0.0f, 0.0f), uv1,  uv2, color, c1, c2);
		}else if(mShadeType == SHADE_GAUROUD || mShadeType == SHADE_PHONG_Vertex)
		{
			RasterizeGouraudFaceUp(v0, vNew1, v1, uv0, Vector4(u, v, 0.0f), uv1, c0, color, c1 );
			RasterizeGouraudFaceDown(vNew1, v1,  v2, Vector4(u, v, 0.0f), uv1, uv2, color, c1, c2);
		}else
		{
			RasterizePhongFaceUp(v0, vNew1, v1, v0V, newvV, v1V, n0, new1Normal, n1, uv0, Vector4(u, v, 0.0f, 0.0f), uv1, c0, color, c1 );
			RasterizePhongFaceDown(vNew1, v1, v2, newvV, v1V, v2V,  new1Normal, n1, n2, Vector4(u, v, 0.0f, 0.0f),  uv1, uv2, color, c0, c2 );
		}
	}
}

//				/\ v0
//			   /  \
//			  /	   \
//		v1	 --------	v2
void CGraphics::RasterizeFlatFaceUp(const Vector4& v0, const Vector4& v1, const Vector4& v2, 
	const Vector4& uv0, const Vector4& uv1, const Vector4& uv2,
	const CColor& c0, const CColor& c1, const CColor& c2)
{
	CTexture* tex = mTextures;

	// 线性插值
	float xLDetal = (v1.x - v0.x) / (v1.y - v0.y); // Y加1x的增量
	float xRDetal = (v2.x - v0.x) / (v2.y - v0.y);

	float zLDetal = (1/v1.z - 1/v0.z) / (v1.y - v0.y); // z是按1/z线性变化的
	float zRDetal = (1/v2.z - 1/v0.z) / (v2.y - v0.y);

	float uLDetal = (uv1.x / v1.z - uv0.x / v0.z) / (v1.y - v0.y);
	float uRDetal =  (uv2.x / v2.z - uv0.x / v0.z) / (v2.y - v0.y);

	float vLDetal = (uv1.y / v1.z - uv0.y / v0.z) / (v1.y - v0.y);
	float vRDetal =  (uv2.y / v2.z - uv0.y / v0.z) / (v2.y - v0.y);

	for(float y = v0.y; y < v2.y; ++y)
	{
		// 避免浮点误差
		y = (int)(y+0.5f); 

		int xL, xR;
		xL = (y - v0.y) * xLDetal + v0.x;
		xR = (y - v0.y) * xRDetal + v0.x;

		float zL, zR;
		zL = (y - v0.y) * zLDetal + 1 / v0.z;
		zR = (y - v0.y) * zRDetal + 1 / v0.z;

		int divWidth = EqualFloat(xR - xL, 0.0) ? 1.0f : 1.0f/(xR - xL); // 避免分母为零
		float zStep = (zR - zL) / divWidth;

		// uv
		float uL = (y - v0.y) * uLDetal + uv0.x / v0.z;
		float vL = (y - v0.y) * vLDetal + uv0.y / v0.z;

		float uR = (y - v0.y) * uRDetal + uv0.x / v0.z;
		float vR = (y - v0.y) * vRDetal + uv0.y / v0.z;

		float uStep = (uR - uL) / divWidth;
		float vStep = (vR - vR) / divWidth;

		float u = uL, v = vL;
		CColor texC(255, 255, 255);

		for(float x = xL, z = zL; x < xR; x++, z += zStep, u += uStep, v += vStep)
		{
			x = (int)(x + 0.5f);

			// 先检查像素是否在屏幕内, 然后检查Zbuffer
			if (x <0 || y < 0 || x > SCREEN_WIDTH || y > SCREEN_HEIGHT)
				continue;
			if (!CheckZ(x, y, z))
				continue;

			if(tex)
			{
				texC = tex->GetPixelUV(u / z, v / z);
			}

			if(mFillType == FILL_WIREFRAME)
			{
				if(x > 0 && y > 0 && (x < xL + 1) || (x > xR - 1))
					SetPixel(x, y, c0);	// 固定着色，都使用顶点颜色
			}else{
				SetPixel(x, y, c0*texC);
			}
		}
	}
}

//		v0 --------	v1
//			\    /
//			 \  /
//			  \/ v2
void CGraphics::RasterizeFlatFaceDown(const Vector4& v0, const Vector4& v1, const Vector4& v2, 
	const Vector4& uv0, const Vector4& uv1, const Vector4& uv2,
	const CColor& c0, const CColor& c1, const CColor& c2)
{
	CTexture* tex = mTextures;

	// 线性插值
	float xLDetal = (v2.x - v0.x) / (v2.y - v0.y); // Y加1x的增量
	float xRDetal = (v2.x - v1.x) / (v2.y - v1.y);

	float zLDetal = (1/v2.z - 1/v0.z) / (v2.y - v0.y); // z是按1/z线性变化的
	float zRDetal = (1/v2.z - 1/v1.z) / (v2.y - v1.y);

	float uLDetal = (uv2.x / v2.z - uv0.x / v0.z) / (v2.y - v0.y);
	float uRDetal =  (uv2.x / v2.z - uv1.x / v1.z) / (v2.y - v1.y);

	float vLDetal = (uv2.y / v2.z - uv1.y / v0.z) / (v2.y - v1.y);
	float vRDetal =  (uv2.y / v2.z - uv1.y / v0.z) / (v2.y - v1.y);

	for(float y = v0.y; y < v2.y; ++y)
	{
		// 避免浮点误差
		y = (int)(y+0.5f); 

		int xL, xR;
		xL = (y - v0.y) * xLDetal + v0.x;
		xR = (y - v1.y) * xRDetal + v1.x;

		float zL, zR;
		zL = (y - v0.y) * zLDetal + 1 / v0.z;
		zR = (y - v1.y) * zRDetal + 1 / v1.z;

		int divWidth = EqualFloat(xR - xL, 0.0) ? 1.0f : 1.0f/(xR - xL); // 避免分母为零
		float zStep = (zR - zL) / divWidth;

		// uv
		float uL = (y - v0.y) * uLDetal + uv0.x / v0.z;
		float vL = (y - v0.y) * vLDetal + uv0.y / v0.z;

		float uR = (y - v1.y) * uRDetal + uv1.x / v1.z;
		float vR = (y - v1.y) * vRDetal + uv1.y / v1.z;

		float uStep = (uR - uL) / divWidth;
		float vStep = (vR - vR) / divWidth;

		float u = uL, v = vL;
		CColor texC(255, 255, 255);

		for(float x = xL, z = zL; x < xR; x++, z += zStep, u += uStep, v += vStep)
		{
			x = (int)(x + 0.5f);

			// 先检查像素是否在屏幕内, 然后检查Zbuffer
			if (x <0 || y < 0 || x > SCREEN_WIDTH || y > SCREEN_HEIGHT)
				continue;
			if (!CheckZ(x, y, z))
				continue;

			if(tex)
			{
				texC = tex->GetPixelUV(u / z, v / z);
			}

			if(mFillType == FILL_WIREFRAME)
			{
				if(x > 0 && y > 0 && (x < xL + 1) || (x > xR - 1))
					SetPixel(x, y, c0);	// 固定着色，都使用顶点颜色
			}else{
				SetPixel(x, y, c0*texC);
			}
		}
	}
}

//				/\ v0
//			   /  \
//			  /	   \
//		v1	 --------	v2
void CGraphics::RasterizeGouraudFaceUp(const Vector4& v0, const Vector4& v1, const Vector4& v2, 
	const Vector4& uv0, const Vector4& uv1, const Vector4& uv2,
	const CColor& c0, const CColor& c1, const CColor& c2)
{
	CTexture* tex = mTextures;

	// 线性插值
	float xLDetal = (v1.x - v0.x) / (v1.y - v0.y); // Y加1x的增量
	float xRDetal = (v2.x - v0.x) / (v2.y - v0.y);

	float zLDetal = (1/v1.z - 1/v0.z) / (v1.y - v0.y); // z是按1/z线性变化的
	float zRDetal = (1/v2.z - 1/v0.z) / (v2.y - v0.y);

	float uLDetal = (uv1.x / v1.z - uv0.x / v0.z) / (v1.y - v0.y);
	float uRDetal =  (uv2.x / v2.z - uv0.x / v0.z) / (v2.y - v0.y);

	float vLDetal = (uv1.y / v1.z - uv0.y / v0.z) / (v1.y - v0.y);
	float vRDetal =  (uv2.y / v2.z - uv0.y / v0.z) / (v2.y - v0.y);

	// Y方向对颜色的插值
	float rLDetal = (c1.r - c0.r) / (v1.y - v0.y);
	float rRDetal = (c2.r - c0.r) / (v2.y - v0.y);

	float gLDetal = (c1.g - c0.g) / (v1.y - v0.y);
	float gRDetal = (c2.g - c0.g) / (v2.y - v0.y);

	float bLDetal = (c1.b - c0.b) / (v1.y - v0.y);
	float bRDetal = (c2.b - c0.b) / (v2.y - v0.y);

	for(float y = v0.y; y < v2.y; ++y)
	{
		// 避免浮点误差
		y = (int)(y+0.5f); 

		int xL, xR;
		xL = (y - v0.y) * xLDetal + v0.x;
		xR = (y - v0.y) * xRDetal + v0.x;

		float zL, zR;
		zL = (y - v0.y) * zLDetal + 1 / v0.z;
		zR = (y - v0.y) * zRDetal + 1 / v0.z;

		int divWidth = EqualFloat(xR - xL, 0.0) ? 1.0f : 1.0f/(xR - xL); // 避免分母为零
		float zStep = (zR - zL) / divWidth;

		// x方向color插值
		unsigned char crL = (y - v0.y) * rLDetal + c0.r;
		unsigned char crR = (y - v0.y) * rRDetal + c0.r;

		unsigned char cgL = (y - v0.y) * gLDetal + c0.g;
		unsigned char cgR = (y - v0.y) * gRDetal + c0.g;

		unsigned char cbL = (y - v0.y) * bLDetal + c0.b;
		unsigned char cbR = (y - v0.y) * bRDetal + c0.b;

		float colorRStep = (crR - crL) * divWidth;
		float colorGStep = (cgR - cgL) * divWidth;
		float colorBStep = (cbR - cbL) * divWidth;

		// uv
		float uL = (y - v0.y) * uLDetal + uv0.x / v0.z;
		float vL = (y - v0.y) * vLDetal + uv0.y / v0.z;

		float uR = (y - v0.y) * uRDetal + uv0.x / v0.z;
		float vR = (y - v0.y) * vRDetal + uv0.y / v0.z;

		float uStep = (uR - uL) / divWidth;
		float vStep = (vR - vL) / divWidth;

		float u = uL, v = vL;
		CColor texC(255, 255, 255);

		float r = crL, g = cgL, b = cbL;
		for(float x = xL, z = zL; x < xR; x++, z += zStep, u += uStep, v += vStep)
		{
			x = (int)(x + 0.5f);

			r = crL + (x - xL)*colorRStep;
			g = cgL + (x - xL)*colorGStep;
			b = cbL + (x - xL)*colorBStep;

			// 避免溢出
			r = Clamp(r, 0.0f, 255.0f);
			g = Clamp(g, 0.0f, 255.0f);
			b = Clamp(b, 0.0f, 255.0f);

			// 先检查像素是否在屏幕内, 然后检查Zbuffer
			if (x <0 || y < 0 || x > SCREEN_WIDTH || y > SCREEN_HEIGHT)
				continue;
			if (!CheckZ(x, y, z))
				continue;

			if(tex)
			{
				texC = tex->GetPixelUV(u / z, v / z);
			}

			if(mFillType == FILL_WIREFRAME)
			{
				if(x > 0 && y > 0 && (x < xL + 1) || (x > xR - 1))
					SetPixel(x, y, CColor(r, g, b));
			}else{
				SetPixel(x, y, CColor(r, g, b)*texC); // 使用插值颜色填充
			}
		}
	}
}

//		v0 --------	v1
//			\    /
//			 \  /
//			  \/ v2
void CGraphics::RasterizeGouraudFaceDown(const Vector4& v0, const Vector4& v1, const Vector4& v2, 
	const Vector4& uv0, const Vector4& uv1, const Vector4& uv2,
	const CColor& c0, const CColor& c1, const CColor& c2)
{
	CTexture* tex = mTextures;

	// 线性插值
	float xLDetal = (v2.x - v0.x) / (v2.y - v0.y); // Y加1x的增量
	float xRDetal = (v2.x - v1.x) / (v2.y - v1.y);

	float zLDetal = (1/v2.z - 1/v0.z) / (v2.y - v0.y); // z是按1/z线性变化的
	float zRDetal = (1/v2.z - 1/v1.z) / (v2.y - v1.y);

	float uLDetal = (uv2.x / v2.z - uv0.x / v0.z) / (v2.y - v0.y);
	float uRDetal =  (uv2.x / v2.z - uv0.x / v0.z) / (v2.y - v0.y);

	float vLDetal = (uv2.y / v2.z - uv1.y / v0.z) / (v2.y - v1.y);
	float vRDetal =  (uv2.y / v2.z - uv1.y / v0.z) / (v2.y - v1.y);

	// Y方向对颜色的插值
	float rLDetal = (c2.r - c0.r) / (v2.y - v0.y);
	float rRDetal = (c2.r - c1.r) / (v2.y - v1.y);

	float gLDetal = (c2.g - c0.g) / (v2.y - v0.y);
	float gRDetal = (c2.g - c1.g) / (v2.y - v1.y);

	float bLDetal = (c2.b - c0.b) / (v2.y - v0.y);
	float bRDetal = (c2.b - c1.b) / (v2.y - v1.y);

	for(float y = v0.y; y < v2.y; ++y)
	{
		// 避免浮点误差
		y = (int)(y+0.5f); 

		int xL, xR;
		xL = (y - v0.y) * xLDetal + v0.x;
		xR = (y - v1.y) * xRDetal + v1.x;

		float zL, zR;
		zL = (y - v0.y) * zLDetal + 1 / v0.z;
		zR = (y - v1.y) * zRDetal + 1 / v1.z;

		int divWidth = EqualFloat(xR - xL, 0.0) ? 1.0f : 1.0f/(xR - xL); // 避免分母为零
		float zStep = (zR - zL) / divWidth;

		// x方向color插值
		unsigned char crL = (y - v0.y) * rLDetal + c0.r;
		unsigned char crR = (y - v1.y) * rRDetal + c1.r;

		unsigned char cgL = (y - v0.y) * gLDetal + c0.g;
		unsigned char cgR = (y - v1.y) * gRDetal + c1.g;

		unsigned char cbL = (y - v0.y) * bLDetal + c0.b;
		unsigned char cbR = (y - v1.y) * bRDetal + c1.b;

		float colorRStep = (crR - crL) * divWidth;
		float colorGStep = (cgR - cgL) * divWidth;
		float colorBStep = (cbR - cbL) * divWidth;

		// uv
		float uL = (y - v0.y) * uLDetal + uv0.x / v0.z;
		float vL = (y - v0.y) * vLDetal + uv0.y / v0.z;

		float uR = (y - v1.y) * uRDetal + uv1.x / v1.z;
		float vR = (y - v1.y) * vRDetal + uv1.y / v1.z;

		float uStep = (uR - uL) / divWidth;
		float vStep = (vR - vL) / divWidth;

		float u = uL, v = vL;
		CColor texC(255, 255, 255);

		float r = crL, g = cgL, b = cbL;
		for(float x = xL, z = zL; x < xR; x++, z += zStep, u += uStep, v += vStep)
		{
			x = (int)(x + 0.5f);

			r = crL + (x - xL)*colorRStep;
			g = cgL + (x - xL)*colorGStep;
			b = cbL + (x - xL)*colorBStep;

			// 避免溢出
			r = Clamp(r, 0.0f, 255.0f);
			g = Clamp(g, 0.0f, 255.0f);
			b = Clamp(b, 0.0f, 255.0f);

			// 先检查像素是否在屏幕内, 然后检查Zbuffer
			if (x <0 || y < 0 || x > SCREEN_WIDTH || y > SCREEN_HEIGHT)
				continue;
			if (!CheckZ(x, y, z))
				continue;

			if(tex)
			{
				texC = tex->GetPixelUV(u / z, v / z);
			}

			if(mFillType == FILL_WIREFRAME)
			{
				if(x > 0 && y > 0 && (x < xL + 1) || (x > xR - 1))
					SetPixel(x, y, CColor(r, g, b));
			}else{
				SetPixel(x, y, CColor(r, g, b)*texC);	// 使用插值颜色填充
			}
		}
	}
}

//				/\ v0
//			   /  \
//			  /	   \
//		v1	 --------	v2
void CGraphics::RasterizePhongFaceUp(const Vector4& v0, const Vector4& v1, const Vector4& v2,
	const Vector4& vV0, const Vector4& vV1, const Vector4& vV2,
	const Vector4& n0, const Vector4& n1, const Vector4& n2,
	const Vector4& uv0, const Vector4& uv1, const Vector4& uv2,
	const CColor& c0, const CColor& c1, const CColor& c2)
{
	CTexture* tex = mTextures;

	// 线性插值
	float xLDetal = (v1.x - v0.x) / (v1.y - v0.y); // Y加1x的增量
	float xRDetal = (v2.x - v0.x) / (v2.y - v0.y);

	float zLDetal = (1/v1.z - 1/v0.z) / (v1.y - v0.y); // z是按1/z线性变化的
	float zRDetal = (1/v2.z - 1/v0.z) / (v2.y - v0.y);

	float uLDetal = (uv1.x / v1.z - uv0.x / v0.z) / (v1.y - v0.y);
	float uRDetal =  (uv2.x / v2.z - uv0.x / v0.z) / (v2.y - v0.y);

	float vLDetal = (uv1.y / v1.z - uv0.y / v0.z) / (v1.y - v0.y);
	float vRDetal =  (uv2.y / v2.z - uv0.y / v0.z) / (v2.y - v0.y);

	// 法线插值
	Vector4 nLDetal = (n1 - n0) / (v1.y - v0.y);
	Vector4 nRDetal = (n2 - n0) / (v1.y - v0.y);

	// 观察坐标系坐标插值
	Vector4 vVLDetal = (vV1 - vV0) / (v1.y - v0.y);
	Vector4 vVRDetal = (vV2 - vV0) / (v1.y - v0.y);

	// 计算切线空间矩阵
	Matrix4 TBN;
	if (mNormalTexture != NULL)
		FindTBN(vV0, vV1, vV2, uv0, uv1, uv2, TBN);

	for(float y = v0.y; y < v2.y; ++y)
	{
		// 避免浮点误差
		y = (int)(y+0.5f); 

		int xL, xR;
		xL = (y - v0.y) * xLDetal + v0.x;
		xR = (y - v0.y) * xRDetal + v0.x;

		float zL, zR;
		zL = (y - v0.y) * zLDetal + 1 / v0.z;
		zR = (y - v0.y) * zRDetal + 1 / v0.z;

		int divWidth = EqualFloat(xR - xL, 0.0) ? 1.0f : 1.0f/(xR - xL); // 避免分母为零
		float zStep = (zR - zL) / divWidth;

		// uv
		float uL = (y - v0.y) * uLDetal + uv0.x / v0.z;
		float vL = (y - v0.y) * vLDetal + uv0.y / v0.z;

		float uR = (y - v0.y) * uRDetal + uv0.x / v0.z;
		float vR = (y - v0.y) * vRDetal + uv0.y / v0.z;

		float uStep = (uR - uL) / divWidth;
		float vStep = (vR - vL) / divWidth;

		float u = uL, v = vL;
		CColor texC(255, 255, 255);

		// phong相关参数计算
		Vector4 normalL = nLDetal * (y - v0.y) + n0;
		Vector4 normalR = nRDetal * (y - v0.y) + n0;
		Vector4 normalStep = (normalR - normalL) * divWidth;
		Vector4 n = normalL;

		Vector4 vViewL = vVLDetal * (y - v0.y) + vV0;
		Vector4 vViewR = vVRDetal * (y - v0.y) + vV0;
		Vector4 vViewStep = (vViewR - vViewL) * divWidth;
		Vector4 vV = vViewL;

		CColor c(255, 255, 255);
		for(float x = xL, z = zL; x < xR; x++, z += zStep, u += uStep, v += vStep, n += normalStep, vV += vViewStep)
		{
			x = (int)(x + 0.5f);

			// 先检查像素是否在屏幕内, 然后检查Zbuffer
			if (x <0 || y < 0 || x > SCREEN_WIDTH || y > SCREEN_HEIGHT)
				continue;
			if (!CheckZ(x, y, z))
				continue;

			if(tex)
			{
				texC = tex->GetPixelUV(u / z, v / z);
			}

			// phong逐像素计算光照
			if(mNormalTexture)
			{
				CColor bumpColor = CalNormallMapNormal(mNormalTexture, mLights, mLightPositionView, TBN, vV, u, v);
				c = PhongCal(mLights, mLightPositionView, vV / z, n / z) * bumpColor;
			}else
			{
				c = PhongCal(mLights, mLightPositionView, vV / z, n / z);
			}

			if(mFillType == FILL_WIREFRAME)
			{
				if(x > 0 && y > 0 && (x < xL + 1) || (x > xR - 1))
					SetPixel(x, y, c);
			}else{
				SetPixel(x, y, c*texC);
			}
		}
	}
}

//		v0 --------	v1
//			\    /
//			 \  /
//			  \/ v2
void CGraphics::RasterizePhongFaceDown(const Vector4& v0, const Vector4& v1, const Vector4& v2,
	const Vector4& vV0, const Vector4& vV1, const Vector4& vV2,
	const Vector4& n0, const Vector4& n1, const Vector4& n2,
	const Vector4& uv0, const Vector4& uv1, const Vector4& uv2,
	const CColor& c0, const CColor& c1, const CColor& c2)
{
	{
		CTexture* tex = mTextures;

		// 线性插值
		float xLDetal = (v0.x - v2.x) / (v2.y - v0.y); // Y加1x的增量
		float xRDetal = (v1.x - v2.x) / (v2.y - v1.y);

		float zLDetal = (1/v0.z - 1/v2.z) / (v2.y - v0.y); // z是按1/z线性变化的
		float zRDetal = (1/v1.z - 1/v2.z) / (v2.y - v1.y);

		float uLDetal = (uv0.x / v0.z - uv2.x / v2.z) / (v2.y - v0.y);
		float uRDetal =  (uv1.x / v1.z - uv2.x / v2.z) / (v2.y - v1.y);

		float vLDetal = (uv0.y / v0.z - uv2.y / v2.z) / (v2.y - v0.y);
		float vRDetal =  (uv1.y / v1.z - uv2.y / v2.z) / (v2.y - v1.y);

		// 法线插值
		Vector4 nLDetal = (n0 - n2) / (v2.y - v0.y);
		Vector4 nRDetal = (n1 - n2) / (v2.y - v1.y);

		// 观察坐标系坐标插值
		Vector4 vVLDetal = (vV0 - vV2) / (v2.y - v0.y);
		Vector4 vVRDetal = (vV1 - vV2) / (v2.y - v1.y);

		// 计算切线空间矩阵
		Matrix4 TBN;
		if (mNormalTexture != NULL)
			FindTBN(vV0, vV1, vV2, uv0, uv1, uv2, TBN);

		for(float y = v0.y; y < v2.y; ++y)
		{
			// 避免浮点误差
			y = (int)(y+0.5f); 

			int xL, xR;
			xL = (y - v0.y) * xLDetal + v0.x;
			xR = (y - v1.y) * xRDetal + v1.x;

			float zL, zR;
			zL = (y - v0.y) * zLDetal + 1 / v0.z;
			zR = (y - v1.y) * zRDetal + 1 / v1.z;

			int divWidth = EqualFloat(xR - xL, 0.0) ? 1.0f : 1.0f/(xR - xL); // 避免分母为零
			float zStep = (zR - zL) / divWidth;

			// uv
			float uL = (y - v0.y) * uLDetal + uv0.x / v0.z;
			float vL = (y - v1.y) * vLDetal + uv1.y / v1.z;

			float uR = (y - v0.y) * uRDetal + uv0.x / v0.z;
			float vR = (y - v1.y) * vRDetal + uv1.y / v1.z;

			float uStep = (uR - uL) / divWidth;
			float vStep = (vR - vR) / divWidth;

			float u = uL, v = vL;
			CColor texC(255, 255, 255);

			// phong相关参数计算
			Vector4 normalL = nLDetal * (y - v0.y) + n0;
			Vector4 normalR = nRDetal * (y - v1.y) + n1;
			Vector4 normalStep = (normalR - normalL) * divWidth;
			Vector4 n = normalL;

			Vector4 vViewL = vVLDetal * (y - v0.y) + vV0;
			Vector4 vViewR = vVRDetal * (y - v1.y) + vV1;
			Vector4 vViewStep = (vViewR - vViewL) * divWidth;
			Vector4 vV = vViewL;

			CColor c(255, 255, 255);
			for(float x = xL, z = zL; x < xR; x++, z += zStep, u += uStep, v += vStep, n += normalStep, vV += vViewStep)
			{
				x = (int)(x + 0.5f);

				// 先检查像素是否在屏幕内, 然后检查Zbuffer
				if (x <0 || y < 0 || x > SCREEN_WIDTH || y > SCREEN_HEIGHT)
					continue;
				if (!CheckZ(x, y, z))
					continue;

				if(tex)
				{
					texC = tex->GetPixelUV(u / z, v / z);
				}

				// phong逐像素计算光照
				if(mNormalTexture)
				{
					CColor bumpColor = CalNormallMapNormal(mNormalTexture, mLights, mLightPositionView, TBN, vV, u, v);
					c = PhongCal(mLights, mLightPositionView, vV / z, n / z) * bumpColor;
				}else
				{
					c = PhongCal(mLights, mLightPositionView, vV / z, n / z);
				}

				if(mFillType == FILL_WIREFRAME)
				{
					if(x > 0 && y > 0 && (x < xL + 1) || (x > xR - 1))
						SetPixel(x, y, c);
				}else{
					SetPixel(x, y, c*texC);
				}
			}
		}
	}
}

// 通过法线贴图计算切线空间矩阵
void CGraphics::FindTBN(Vector4 Vertices0, Vector4 Vertices1, Vector4 Vertices2, Vector4 TexCoords0, Vector4 TexCoords1, Vector4 TexCoords2, Matrix4& TBN) 
{
    /* Calculate the vectors from the current vertex
       to the two other vertices in the triangle */
	// 假设三角形三个坐标点为P1(u1,v1)、P2(u2,v2)、P3(u3,v3)，假设切线空间的三个基为T，B,N(T为切线方向，也就是u方向,B为负法线方向,也就是v方向),其实T和B均在三角形所在的平面上。
	/* 在使用切线空间的法线映射（normalmap）时，一般来说顶点数据中除了法线向量外还需要加入tangent向量和binormal向量信息，以便在VS中计算切线空间变换矩阵。
		其中tangent是切平面上纹理坐标u的正方向，
		binormal是切平面上纹理坐标v的正方向。
		normal，tangent和binormal三者互相垂直，构成了切线空间坐标系，于是切线空间到世界空间的变换矩阵就是 float3x3( tangent, binormal, normal )
		====================
		总结
		法线映射  就是把法线信息存在贴图里 计算的时候 把数据转到tangentspace来计算 */
	Vector4 v2v1 = Vertices0 - Vertices2;
    Vector4 v3v1 = Vertices1 - Vertices2;

    //Calculate the “direction” of the triangle based on texture coordinates.

    // Calculate c2c1_T and c2c1_B
    float c2c1_T = TexCoords0.x - TexCoords2.x;
    float c2c1_B = TexCoords0.y - TexCoords2.y;

    // Calculate c3c1_T and c3c1_B
    float c3c1_T = TexCoords1.x - TexCoords2.x;
    float c3c1_B = TexCoords1.y - TexCoords2.y;

    //Look at the references for more explanation for this one.
    float fDenominator = c2c1_T * c3c1_B - c3c1_T * c2c1_B;  
    /*ROUNDOFF here is a macro that sets a value to 0.0f if the value is a very small
      value, such as > -0.001f and < 0.001. */

    /* EDIT by c programmer: you should NEVER perform an equality test against a floating point value, even if
       your macro has set fDenominator to 0.0f.  The comparison can still fail.  The code needs fixed.
       Instead you should check if fDenominator is within an epsilon value of 0.0f. */

    if (EqualFloat(fDenominator, 0.0f))
    {
           /* We won't risk a divide by zero, so set the tangent matrix to the identity matrix */
           TBN = Matrix4(	Vector4(1.0f, 0.0f, 0.0f),
							Vector4(0.0f, 1.0f, 0.0f),
							Vector4(0.0f, 0.0f, 1.0f));
    }
    else
    {            
            // Calculate the reciprocal value once and for all (to achieve speed)
            float fScale = 1.0f / fDenominator;

            /* Time to calculate the tangent, binormal, and normal.
               Look at Sren’s article for more information. */
			/*
			因为显卡一般是以三角形作为最小渲染单位的，我们假设三角形的三点为p1, p2, p3，设该三角形所对应的一个空间为(T,B,N), 其中T和B构成的平面就是三角形所在平面, 而T和B分别对应U和V方向，此空间
			的作用是让任何向量从tangent space变换到world space（实际上我们最终要求的是反过来的，先这样好作说明，最后求逆即可） 

			那么P2-P1的向量p必定在T及B所构成的平面上，由此推出： 

			P2-P1 = (U2-U1)*T+(V2-V1)*B
			P3-P1 = (U3-U1)*T+(V3-V1)*B 
			*/
            Vector4 T, B, N;
            T = Vector4((c3c1_B * v2v1.x - c2c1_B * v3v1.x) * fScale,
                         (c3c1_B * v2v1.y - c2c1_B * v3v1.y) * fScale,
                         (c3c1_B * v2v1.z - c2c1_B * v3v1.z) * fScale);

            B = Vector4((-c3c1_T * v2v1.x + c2c1_T * v3v1.x) * fScale,
                         (-c3c1_T * v2v1.y + c2c1_T * v3v1.y) * fScale,
                         (-c3c1_T * v2v1.z + c2c1_T * v3v1.z) * fScale);

            N = T.CrossVector(B); //Cross product!

			// 最后要T和B和N反过来作用向量从world space变换到tangent space, 我们只要对其求逆即可。 
			TBN = Matrix4(T, B, N).Transpose(); 
	}
}

CColor CGraphics::CalNormallMapNormal(CTexture* bumpTex, CLight* light, const Vector4& lightPosV, const Matrix4& TBN, const Vector4& vPos, float u, float v)
{
	if (light == NULL)
		return CColor(255, 255, 255);

	CColor cT = bumpTex->GetPixelUV(u, v);
	float factor = 2.0f / 255.0f;
	// 法线计算凹凸
	float xN = cT.b * factor - 1.0f;
	float yN = cT.r * factor - 1.0f;
	float zN = cT.g * factor - 1.0f;
	Vector4 nT(xN, yN, zN, 0.0f);
	nT = nT.Nomalize();

	Vector4 litDir = lightPosV - vPos;
	// 观察坐标系里的光线坐标要转换到切线坐标系
	litDir = Vec4MulMat4(litDir, TBN);
	litDir = litDir.Nomalize();

	float f = nT.DotVector(litDir);
	f = Clamp(f, 0.0f, 1.0f);
	return CColor(255, 255, 255) * (1 - f);
}

bool CGraphics::CheckZ(int x, int y, float z)
{
	// 这里Z应该使用float类型来存储, 如果使用int类型, 那么会导致精度丢失
	// 产生错误的现象
	int index = y * SCREEN_WIDTH + x;
	//float divZ = 1.0f / z;
	// 这里是基于1/z做的比较
	if (mZBuffer[index] > z) // 说明分母较大，则右手坐标系中较前
		return false;

	mZBuffer[index] = z;
	return true;
}

void CGraphics::DrawPrimitives()
{
	BuildMatrix();

	TranslateToViewSpace();

	ProcessCull();

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

void CGraphics::SetPixel( int x, int y, const CColor& c )
{
	// 这里本来应该计算z值,但是为了避免对Image像素的读取, 我将z检查分离了出来
	// 所以, 在调用setPixel之前应该先检测checkZ, 返回true在调用setPixel
	// ----------------------------------------------------------------
	// 这里Z应该使用float类型来存储, 如果使用int类型, 那么会导致精度丢失
	// 产生错误的现象
	//int index = y * SCREEN_WIDTH + x;
	//float divZ = 1.0f / z;
	//// 这里是基于1/z做的比较
	//if (GZBuffer[index] > divZ)
	//	return;

	//GZBuffer[index] = divZ;
	//SetPixel(GBufferedHDC, x, y, RGB(c.r, c.g, c.b));

	//each scan line in a bitmap has a width, a.k.a : pitch
	//int pitch = GDIBSection.dsBm.bmWidthBytes;
	BYTE* pSrcPix = mDatas + (mPitch * y);
	pSrcPix += x * 4;
	// blue green red alpha
	pSrcPix[0] = c.b;
	pSrcPix[1] = c.g;
	pSrcPix[2] = c.r;
	pSrcPix[3] = c.a;
}

void CGraphics::DrawString(const std::string &str, int x, int y, const CColor &c)
{
	::SetTextColor(mBufferedHDC, RGB(c.r, c.g, c.b));
	::TextOut(mBufferedHDC, x, y, str.c_str(), str.length()); 
}

void CGraphics::ProcessCull()
{
	// 简单背面剔除
	if(mCullType == CULL_NONE || mFillType == FILL_WIREFRAME)
		return;

	int factor = ( mCullType == CULL_BACK ? 1 : -1 );

	// 使用观察坐标进行剔除判断
	for(int i = 0; i < mFaceNum; ++i)
	{
		CFace& face = mFaces[i];
		Vector4 v0 = mVertexs[face.mVertIndex[0]].mVertexView - mVertexs[face.mVertIndex[1]].mVertexView;
		Vector4 v1 = mVertexs[face.mVertIndex[0]].mVertexView - mVertexs[face.mVertIndex[2]].mVertexView;

		Vector4 faceNormal = v0.CrossVector(v1) * factor;
		float result = faceNormal.DotVector(-mVertexs[face.mVertIndex[1]].mVertexView/*摄像机到顶点的向量*/);

		if(result < 0) // 夹角大于90度
			face.isCulled = true;
	}
}