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

	// CreateCompatibleDC���������Ļ���(�ڴ���ʾ�豸) CreateCompatibleBitmap����λͼ
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

	// ����һ���ڴ�������ȡ������ָ��
	void* pBits = NULL;
	mBufferedHandle = ::CreateDIBSection(mBufferedHDC, &info, DIB_RGB_COLORS, &pBits, NULL, 0);
	::SelectObject(mBufferedHDC, mBufferedHandle);

	//here: "dib.dsBm.bmBits" will points to the pixels of hdib.
	::GetObject(mBufferedHandle , sizeof(DIBSECTION), &mDIBSection);
	mDatas = (BYTE*)mDIBSection.dsBm.bmBits;
	mPitch = mDIBSection.dsBm.bmWidthBytes;

	// ����ˢ�������С
	::SetRect(&mBufferSize, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//GPen = (HPEN)::GetStockObject(WHITE_PEN); 
	mPen = ::CreatePen(PS_SOLID, 1, RGB(255, 255, 255)); 
	::SelectObject(mBufferedHDC, mPen);

	mBgBrush= ::CreateSolidBrush(RGB(0, 0, 255));
	::SelectObject (mBufferedHDC, mBgBrush) ;

	// ��������
	HFONT hfnt = (HFONT)::GetStockObject(OEM_FIXED_FONT); 
	::SelectObject(mBufferedHDC, hfnt);
	// �������ֱ���Ϊ͸��ɫ
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

// �رջ�ͼϵͳ
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
	// ���������ϵ�任����
	Matrix4::ViewMatrix(mMatrix[TS_VIEW], m_pCamera->m_WorldPos, m_pCamera->m_LookAt, m_pCamera->m_Up);
	// ͸������ϵ�任����
	Matrix4::ProjectMatrix(mMatrix[TS_PROJECT],	Degree_TO_Radian(m_pCamera->m_Fov), 
		(float)GetScreenWidth() / (float)GetScreenHeight(), 
		m_pCamera->m_Near, m_pCamera->m_Far);
	// ��Ļ����ϵ�任����
	Matrix4::ScreenMatrix(mMatrix[TS_SCRREN], GetScreenWidth(), GetScreenHeight());
}

void CGraphics::TranslateToViewSpace()
{
	Matrix4 mat = mMatrix[TS_LOCAL] * mMatrix[TS_WORLD] * mMatrix[TS_VIEW];
	for (int i = 0; i < mVertexNum; i++)
	{
		CVertex& ver = mVertexs[i];
		// ����任���۲�����ϵ
		ver.mVertex = Vec4MulMat4W(ver.mVertex, mat);
		// ���㷨��
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
		// ִ��͸�ӳ���, ���������任���ѿ���������ϵ
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
		// Ŀǰ��һյ��
		mLightPositionView = Vec4MulMat4(pLight->m_dWorldPos, mMatrix[TS_VIEW]);

		if(mShadeType == SHADE_FLAT) // ƽ����ɫ
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

					// ��ɫϵ��
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
			// �������������ֱ������߼н�
			for(int i = 0; i < mFaceNum; ++i)
			{
				CFace& face = mFaces[i];
				if(!face.isCulled)
				{
					Vector4 v0Normal = mVertexs[mFaces[i].mVertIndex[0]].mNormal;
					Vector4 v1Normal = mVertexs[mFaces[i].mVertIndex[1]].mNormal;
					Vector4 v2Normal = mVertexs[mFaces[i].mVertIndex[2]].mNormal;

					// ���޸�:mVertex��TranslateToViewSpace���Ѿ�����˹۲�����
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

	// �����׼��
	Vector4 n = normal.Nomalize();
	// ���㵽��Դ������
	Vector4 l = (lightPos - vertexPosView).Nomalize();
	// ���㵽�۲������(���λ��ԭ��)
	Vector4 v = (Vector4(0,0,0,0) - vertexPosView).Nomalize();
	// ������������ο��ʼ�
	Vector4 r = (n * ( 2 * Max(l.DotVector(n), 0)) - l).Nomalize();

	// phoneģ�ͼ��㹫ʽ
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
	// ��դ��
	for (int i = 0; i < mFaceNum; i++)
	{
		CFace& face = mFaces[i];
		if (!face.isCulled)
		{
			// face�Ķ����Ѿ���TranslateToProjectiveSpace�б任��ͶӰ�ռ���
			// ��λ��͸������ϵ�¾���͸�ӳ����Ķ���任����Ļ����ϵ
			// ���ﲻ��ԭʼ���ݽ����޸�, ���������ǵĿ���
			Vector4 v0, v1, v2;
			TranslateToScreenSpace(face, v0, v1, v2);

			// normal�Ѿ���TanslateToViewSpace�б任���۲�ռ���
			// ���������ϵ����Ķ���, for phong
			Vector4 v0V = mVertexs[face.mVertIndex[0]].mVertexView;
			Vector4 v1V = mVertexs[face.mVertIndex[1]].mVertexView;
			Vector4 v2V = mVertexs[face.mVertIndex[2]].mVertexView;

			// ���������ϵ����ķ���, for phong
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

// ��դ��������
void CGraphics::RasterizeFace(int faceIndex,
	Vector4& v0, Vector4& v1, Vector4& v2, 
	Vector4& v0V, Vector4& v1V, Vector4& v2V,
	Vector4& n0,  Vector4& n1, Vector4& n2,
	Vector4& uv0, Vector4& uv1, Vector4& uv2,
	CColor& c0, CColor& c1, CColor& c2)
{
	// ��֤Y������V0<V1<V2��˳���������������λ�����������
	if(v0.y > v1.y) SwapVectex(v0, v0V, n0, uv0, c0, v1, v1V, n1, uv1, c1);
	if(v0.y > v2.y) SwapVectex(v0, v0V, n0, uv0, c0, v2, v2V, n2, uv2, c2);
	if(v1.y > v2.y) SwapVectex(v1, v1V, n1, uv1, c1, v2, v2V, n2, uv2, c2);

	if(!IsInScreen(v0, v1, v2, SCREEN_WIDTH, SCREEN_HEIGHT))
		return;

	// ��б�ʹ�ʽ�ó�����һ��ֱ���ϵ�����:v0,v1,v2 ��:(v1.y-v0.y)/(v1.x-v0.x) = (v2.y-v0.y)/��v2.x-v0.x)
	Vector4 vNew1;
	vNew1.x = (v2.x-v0.x)*(v1.y-v0.y)/(v2.y-v0.y)+v0.x;
	vNew1.y = v1.y;
	// zҲ����б�ʹ�ʽ
	vNew1.z = (v2.z-v0.z)*(v1.y-v0.y)/(v2.y-v0.y)+v0.z;

	// ��ȡб�ʹ�ʽ�й��в���
	float factor = (v1.y-v0.y)/(v2.y-v0.y);

	// �¶����normal��uvҲ��Ҫ���¼���
	Vector4 new1Normal = (n2 - n0)	* factor + n0;
	// view����ϵ����
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
		// ��������
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
		// ��������
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

	// ���Բ�ֵ
	float xLDetal = (v1.x - v0.x) / (v1.y - v0.y); // Y��1x������
	float xRDetal = (v2.x - v0.x) / (v2.y - v0.y);

	float zLDetal = (1/v1.z - 1/v0.z) / (v1.y - v0.y); // z�ǰ�1/z���Ա仯��
	float zRDetal = (1/v2.z - 1/v0.z) / (v2.y - v0.y);

	float uLDetal = (uv1.x / v1.z - uv0.x / v0.z) / (v1.y - v0.y);
	float uRDetal =  (uv2.x / v2.z - uv0.x / v0.z) / (v2.y - v0.y);

	float vLDetal = (uv1.y / v1.z - uv0.y / v0.z) / (v1.y - v0.y);
	float vRDetal =  (uv2.y / v2.z - uv0.y / v0.z) / (v2.y - v0.y);

	for(float y = v0.y; y < v2.y; ++y)
	{
		// ���⸡�����
		y = (int)(y+0.5f); 

		int xL, xR;
		xL = (y - v0.y) * xLDetal + v0.x;
		xR = (y - v0.y) * xRDetal + v0.x;

		float zL, zR;
		zL = (y - v0.y) * zLDetal + 1 / v0.z;
		zR = (y - v0.y) * zRDetal + 1 / v0.z;

		int divWidth = EqualFloat(xR - xL, 0.0) ? 1.0f : 1.0f/(xR - xL); // �����ĸΪ��
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

			// �ȼ�������Ƿ�����Ļ��, Ȼ����Zbuffer
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
					SetPixel(x, y, c0);	// �̶���ɫ����ʹ�ö�����ɫ
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

	// ���Բ�ֵ
	float xLDetal = (v2.x - v0.x) / (v2.y - v0.y); // Y��1x������
	float xRDetal = (v2.x - v1.x) / (v2.y - v1.y);

	float zLDetal = (1/v2.z - 1/v0.z) / (v2.y - v0.y); // z�ǰ�1/z���Ա仯��
	float zRDetal = (1/v2.z - 1/v1.z) / (v2.y - v1.y);

	float uLDetal = (uv2.x / v2.z - uv0.x / v0.z) / (v2.y - v0.y);
	float uRDetal =  (uv2.x / v2.z - uv1.x / v1.z) / (v2.y - v1.y);

	float vLDetal = (uv2.y / v2.z - uv1.y / v0.z) / (v2.y - v1.y);
	float vRDetal =  (uv2.y / v2.z - uv1.y / v0.z) / (v2.y - v1.y);

	for(float y = v0.y; y < v2.y; ++y)
	{
		// ���⸡�����
		y = (int)(y+0.5f); 

		int xL, xR;
		xL = (y - v0.y) * xLDetal + v0.x;
		xR = (y - v1.y) * xRDetal + v1.x;

		float zL, zR;
		zL = (y - v0.y) * zLDetal + 1 / v0.z;
		zR = (y - v1.y) * zRDetal + 1 / v1.z;

		int divWidth = EqualFloat(xR - xL, 0.0) ? 1.0f : 1.0f/(xR - xL); // �����ĸΪ��
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

			// �ȼ�������Ƿ�����Ļ��, Ȼ����Zbuffer
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
					SetPixel(x, y, c0);	// �̶���ɫ����ʹ�ö�����ɫ
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

	// ���Բ�ֵ
	float xLDetal = (v1.x - v0.x) / (v1.y - v0.y); // Y��1x������
	float xRDetal = (v2.x - v0.x) / (v2.y - v0.y);

	float zLDetal = (1/v1.z - 1/v0.z) / (v1.y - v0.y); // z�ǰ�1/z���Ա仯��
	float zRDetal = (1/v2.z - 1/v0.z) / (v2.y - v0.y);

	float uLDetal = (uv1.x / v1.z - uv0.x / v0.z) / (v1.y - v0.y);
	float uRDetal =  (uv2.x / v2.z - uv0.x / v0.z) / (v2.y - v0.y);

	float vLDetal = (uv1.y / v1.z - uv0.y / v0.z) / (v1.y - v0.y);
	float vRDetal =  (uv2.y / v2.z - uv0.y / v0.z) / (v2.y - v0.y);

	// Y�������ɫ�Ĳ�ֵ
	float rLDetal = (c1.r - c0.r) / (v1.y - v0.y);
	float rRDetal = (c2.r - c0.r) / (v2.y - v0.y);

	float gLDetal = (c1.g - c0.g) / (v1.y - v0.y);
	float gRDetal = (c2.g - c0.g) / (v2.y - v0.y);

	float bLDetal = (c1.b - c0.b) / (v1.y - v0.y);
	float bRDetal = (c2.b - c0.b) / (v2.y - v0.y);

	for(float y = v0.y; y < v2.y; ++y)
	{
		// ���⸡�����
		y = (int)(y+0.5f); 

		int xL, xR;
		xL = (y - v0.y) * xLDetal + v0.x;
		xR = (y - v0.y) * xRDetal + v0.x;

		float zL, zR;
		zL = (y - v0.y) * zLDetal + 1 / v0.z;
		zR = (y - v0.y) * zRDetal + 1 / v0.z;

		int divWidth = EqualFloat(xR - xL, 0.0) ? 1.0f : 1.0f/(xR - xL); // �����ĸΪ��
		float zStep = (zR - zL) / divWidth;

		// x����color��ֵ
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

			// �������
			r = Clamp(r, 0.0f, 255.0f);
			g = Clamp(g, 0.0f, 255.0f);
			b = Clamp(b, 0.0f, 255.0f);

			// �ȼ�������Ƿ�����Ļ��, Ȼ����Zbuffer
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
				SetPixel(x, y, CColor(r, g, b)*texC); // ʹ�ò�ֵ��ɫ���
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

	// ���Բ�ֵ
	float xLDetal = (v2.x - v0.x) / (v2.y - v0.y); // Y��1x������
	float xRDetal = (v2.x - v1.x) / (v2.y - v1.y);

	float zLDetal = (1/v2.z - 1/v0.z) / (v2.y - v0.y); // z�ǰ�1/z���Ա仯��
	float zRDetal = (1/v2.z - 1/v1.z) / (v2.y - v1.y);

	float uLDetal = (uv2.x / v2.z - uv0.x / v0.z) / (v2.y - v0.y);
	float uRDetal =  (uv2.x / v2.z - uv0.x / v0.z) / (v2.y - v0.y);

	float vLDetal = (uv2.y / v2.z - uv1.y / v0.z) / (v2.y - v1.y);
	float vRDetal =  (uv2.y / v2.z - uv1.y / v0.z) / (v2.y - v1.y);

	// Y�������ɫ�Ĳ�ֵ
	float rLDetal = (c2.r - c0.r) / (v2.y - v0.y);
	float rRDetal = (c2.r - c1.r) / (v2.y - v1.y);

	float gLDetal = (c2.g - c0.g) / (v2.y - v0.y);
	float gRDetal = (c2.g - c1.g) / (v2.y - v1.y);

	float bLDetal = (c2.b - c0.b) / (v2.y - v0.y);
	float bRDetal = (c2.b - c1.b) / (v2.y - v1.y);

	for(float y = v0.y; y < v2.y; ++y)
	{
		// ���⸡�����
		y = (int)(y+0.5f); 

		int xL, xR;
		xL = (y - v0.y) * xLDetal + v0.x;
		xR = (y - v1.y) * xRDetal + v1.x;

		float zL, zR;
		zL = (y - v0.y) * zLDetal + 1 / v0.z;
		zR = (y - v1.y) * zRDetal + 1 / v1.z;

		int divWidth = EqualFloat(xR - xL, 0.0) ? 1.0f : 1.0f/(xR - xL); // �����ĸΪ��
		float zStep = (zR - zL) / divWidth;

		// x����color��ֵ
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

			// �������
			r = Clamp(r, 0.0f, 255.0f);
			g = Clamp(g, 0.0f, 255.0f);
			b = Clamp(b, 0.0f, 255.0f);

			// �ȼ�������Ƿ�����Ļ��, Ȼ����Zbuffer
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
				SetPixel(x, y, CColor(r, g, b)*texC);	// ʹ�ò�ֵ��ɫ���
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

	// ���Բ�ֵ
	float xLDetal = (v1.x - v0.x) / (v1.y - v0.y); // Y��1x������
	float xRDetal = (v2.x - v0.x) / (v2.y - v0.y);

	float zLDetal = (1/v1.z - 1/v0.z) / (v1.y - v0.y); // z�ǰ�1/z���Ա仯��
	float zRDetal = (1/v2.z - 1/v0.z) / (v2.y - v0.y);

	float uLDetal = (uv1.x / v1.z - uv0.x / v0.z) / (v1.y - v0.y);
	float uRDetal =  (uv2.x / v2.z - uv0.x / v0.z) / (v2.y - v0.y);

	float vLDetal = (uv1.y / v1.z - uv0.y / v0.z) / (v1.y - v0.y);
	float vRDetal =  (uv2.y / v2.z - uv0.y / v0.z) / (v2.y - v0.y);

	// ���߲�ֵ
	Vector4 nLDetal = (n1 - n0) / (v1.y - v0.y);
	Vector4 nRDetal = (n2 - n0) / (v1.y - v0.y);

	// �۲�����ϵ�����ֵ
	Vector4 vVLDetal = (vV1 - vV0) / (v1.y - v0.y);
	Vector4 vVRDetal = (vV2 - vV0) / (v1.y - v0.y);

	// �������߿ռ����
	Matrix4 TBN;
	if (mNormalTexture != NULL)
		FindTBN(vV0, vV1, vV2, uv0, uv1, uv2, TBN);

	for(float y = v0.y; y < v2.y; ++y)
	{
		// ���⸡�����
		y = (int)(y+0.5f); 

		int xL, xR;
		xL = (y - v0.y) * xLDetal + v0.x;
		xR = (y - v0.y) * xRDetal + v0.x;

		float zL, zR;
		zL = (y - v0.y) * zLDetal + 1 / v0.z;
		zR = (y - v0.y) * zRDetal + 1 / v0.z;

		int divWidth = EqualFloat(xR - xL, 0.0) ? 1.0f : 1.0f/(xR - xL); // �����ĸΪ��
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

		// phong��ز�������
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

			// �ȼ�������Ƿ�����Ļ��, Ȼ����Zbuffer
			if (x <0 || y < 0 || x > SCREEN_WIDTH || y > SCREEN_HEIGHT)
				continue;
			if (!CheckZ(x, y, z))
				continue;

			if(tex)
			{
				texC = tex->GetPixelUV(u / z, v / z);
			}

			// phong�����ؼ������
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

		// ���Բ�ֵ
		float xLDetal = (v0.x - v2.x) / (v2.y - v0.y); // Y��1x������
		float xRDetal = (v1.x - v2.x) / (v2.y - v1.y);

		float zLDetal = (1/v0.z - 1/v2.z) / (v2.y - v0.y); // z�ǰ�1/z���Ա仯��
		float zRDetal = (1/v1.z - 1/v2.z) / (v2.y - v1.y);

		float uLDetal = (uv0.x / v0.z - uv2.x / v2.z) / (v2.y - v0.y);
		float uRDetal =  (uv1.x / v1.z - uv2.x / v2.z) / (v2.y - v1.y);

		float vLDetal = (uv0.y / v0.z - uv2.y / v2.z) / (v2.y - v0.y);
		float vRDetal =  (uv1.y / v1.z - uv2.y / v2.z) / (v2.y - v1.y);

		// ���߲�ֵ
		Vector4 nLDetal = (n0 - n2) / (v2.y - v0.y);
		Vector4 nRDetal = (n1 - n2) / (v2.y - v1.y);

		// �۲�����ϵ�����ֵ
		Vector4 vVLDetal = (vV0 - vV2) / (v2.y - v0.y);
		Vector4 vVRDetal = (vV1 - vV2) / (v2.y - v1.y);

		// �������߿ռ����
		Matrix4 TBN;
		if (mNormalTexture != NULL)
			FindTBN(vV0, vV1, vV2, uv0, uv1, uv2, TBN);

		for(float y = v0.y; y < v2.y; ++y)
		{
			// ���⸡�����
			y = (int)(y+0.5f); 

			int xL, xR;
			xL = (y - v0.y) * xLDetal + v0.x;
			xR = (y - v1.y) * xRDetal + v1.x;

			float zL, zR;
			zL = (y - v0.y) * zLDetal + 1 / v0.z;
			zR = (y - v1.y) * zRDetal + 1 / v1.z;

			int divWidth = EqualFloat(xR - xL, 0.0) ? 1.0f : 1.0f/(xR - xL); // �����ĸΪ��
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

			// phong��ز�������
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

				// �ȼ�������Ƿ�����Ļ��, Ȼ����Zbuffer
				if (x <0 || y < 0 || x > SCREEN_WIDTH || y > SCREEN_HEIGHT)
					continue;
				if (!CheckZ(x, y, z))
					continue;

				if(tex)
				{
					texC = tex->GetPixelUV(u / z, v / z);
				}

				// phong�����ؼ������
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

// ͨ��������ͼ�������߿ռ����
void CGraphics::FindTBN(Vector4 Vertices0, Vector4 Vertices1, Vector4 Vertices2, Vector4 TexCoords0, Vector4 TexCoords1, Vector4 TexCoords2, Matrix4& TBN) 
{
    /* Calculate the vectors from the current vertex
       to the two other vertices in the triangle */
	// �������������������ΪP1(u1,v1)��P2(u2,v2)��P3(u3,v3)���������߿ռ��������ΪT��B,N(TΪ���߷���Ҳ����u����,BΪ�����߷���,Ҳ����v����),��ʵT��B�������������ڵ�ƽ���ϡ�
	/* ��ʹ�����߿ռ�ķ���ӳ�䣨normalmap��ʱ��һ����˵���������г��˷��������⻹��Ҫ����tangent������binormal������Ϣ���Ա���VS�м������߿ռ�任����
		����tangent����ƽ������������u��������
		binormal����ƽ������������v��������
		normal��tangent��binormal���߻��ഹֱ�����������߿ռ�����ϵ���������߿ռ䵽����ռ�ı任������� float3x3( tangent, binormal, normal )
		====================
		�ܽ�
		����ӳ��  ���ǰѷ�����Ϣ������ͼ�� �����ʱ�� ������ת��tangentspace������ */
	Vector4 v2v1 = Vertices0 - Vertices2;
    Vector4 v3v1 = Vertices1 - Vertices2;

    //Calculate the ��direction�� of the triangle based on texture coordinates.

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
               Look at Sren��s article for more information. */
			/*
			��Ϊ�Կ�һ��������������Ϊ��С��Ⱦ��λ�ģ����Ǽ��������ε�����Ϊp1, p2, p3���������������Ӧ��һ���ռ�Ϊ(T,B,N), ����T��B���ɵ�ƽ���������������ƽ��, ��T��B�ֱ��ӦU��V���򣬴˿ռ�
			�����������κ�������tangent space�任��world space��ʵ������������Ҫ����Ƿ������ģ�����������˵����������漴�ɣ� 

			��ôP2-P1������p�ض���T��B�����ɵ�ƽ���ϣ��ɴ��Ƴ��� 

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

			// ���ҪT��B��N����������������world space�任��tangent space, ����ֻҪ�������漴�ɡ� 
			TBN = Matrix4(T, B, N).Transpose(); 
	}
}

CColor CGraphics::CalNormallMapNormal(CTexture* bumpTex, CLight* light, const Vector4& lightPosV, const Matrix4& TBN, const Vector4& vPos, float u, float v)
{
	if (light == NULL)
		return CColor(255, 255, 255);

	CColor cT = bumpTex->GetPixelUV(u, v);
	float factor = 2.0f / 255.0f;
	// ���߼��㰼͹
	float xN = cT.b * factor - 1.0f;
	float yN = cT.r * factor - 1.0f;
	float zN = cT.g * factor - 1.0f;
	Vector4 nT(xN, yN, zN, 0.0f);
	nT = nT.Nomalize();

	Vector4 litDir = lightPosV - vPos;
	// �۲�����ϵ��Ĺ�������Ҫת������������ϵ
	litDir = Vec4MulMat4(litDir, TBN);
	litDir = litDir.Nomalize();

	float f = nT.DotVector(litDir);
	f = Clamp(f, 0.0f, 1.0f);
	return CColor(255, 255, 255) * (1 - f);
}

bool CGraphics::CheckZ(int x, int y, float z)
{
	// ����ZӦ��ʹ��float�������洢, ���ʹ��int����, ��ô�ᵼ�¾��ȶ�ʧ
	// �������������
	int index = y * SCREEN_WIDTH + x;
	//float divZ = 1.0f / z;
	// �����ǻ���1/z���ıȽ�
	if (mZBuffer[index] > z) // ˵����ĸ�ϴ�����������ϵ�н�ǰ
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
	// ������Ȼ���
	// ע : ����memsetֻ�ܶ�Int���͵�������г�ʼ��, ��������ֱ��ʹ����Int����
	// ��û��ʹ��float����, Ӧ��ʹ��float
	::memset(mZBuffer, 0, sizeof(float) * SCREEN_WIDTH * SCREEN_HEIGHT);
}

void CGraphics::FlipBuffer(HDC hdc)
{
	// ���Ѿ����ƺõĻ������ݽ���Graphics����Ļ�ϻ���, ������ǰ����������Ϊ��һ��������
	::BitBlt(hdc,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,mBufferedHDC,0,0,SRCCOPY);
}

void CGraphics::SetPixel( int x, int y, const CColor& c )
{
	// ���ﱾ��Ӧ�ü���zֵ,����Ϊ�˱����Image���صĶ�ȡ, �ҽ�z�������˳���
	// ����, �ڵ���setPixel֮ǰӦ���ȼ��checkZ, ����true�ڵ���setPixel
	// ----------------------------------------------------------------
	// ����ZӦ��ʹ��float�������洢, ���ʹ��int����, ��ô�ᵼ�¾��ȶ�ʧ
	// �������������
	//int index = y * SCREEN_WIDTH + x;
	//float divZ = 1.0f / z;
	//// �����ǻ���1/z���ıȽ�
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
	// �򵥱����޳�
	if(mCullType == CULL_NONE || mFillType == FILL_WIREFRAME)
		return;

	int factor = ( mCullType == CULL_BACK ? 1 : -1 );

	// ʹ�ù۲���������޳��ж�
	for(int i = 0; i < mFaceNum; ++i)
	{
		CFace& face = mFaces[i];
		Vector4 v0 = mVertexs[face.mVertIndex[0]].mVertexView - mVertexs[face.mVertIndex[1]].mVertexView;
		Vector4 v1 = mVertexs[face.mVertIndex[0]].mVertexView - mVertexs[face.mVertIndex[2]].mVertexView;

		Vector4 faceNormal = v0.CrossVector(v1) * factor;
		float result = faceNormal.DotVector(-mVertexs[face.mVertIndex[1]].mVertexView/*����������������*/);

		if(result < 0) // �нǴ���90��
			face.isCulled = true;
	}
}