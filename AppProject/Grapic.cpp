#include "Grapic.h"

HINSTANCE GHInstance;
CGrapic::CGrapic(void)
{
}


CGrapic::~CGrapic(void)
{
}

bool CGrapic::InitGDI( HINSTANCE hinstance )
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
void CGrapic::ShutdownGraphics()
{
	::DeleteObject(mPen);
	::DeleteObject(mBgBrush);
	::DeleteObject(mBufferedHandle);
	::DeleteDC(mBufferedHDC);

	//SafeDeleteArray(mFaces);
	//SafeDeleteArray(mVertexs);
}

void CGrapic::BuildMatrix()
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

void CGrapic::TanslateToViewSpace()
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

void CGrapic::TanslateToProjectiveSpace()
{
	for (int i = 0; i < mVertexNum; i++)
	{
		CVertex& ver = mVertexs[i];
		ver.mVertex = Vec4MulMat4W(ver.mVertex, mMatrix[TS_PROJECT]);
		// 执行透视除法, 将齐次坐标变换到笛卡尔积坐标系
		ver.mVertex /= ver.mVertex.w;
	}
}

void CGrapic::TanslateToScreenSpace( const CFace& face, Vector4& v0, Vector4& v1, Vector4& v2 )
{
	v0 = Vec4MulMat4(mVertexs[face.mVertIndex[0]].mVertex, mMatrix[TS_SCRREN]);
	v1 = Vec4MulMat4(mVertexs[face.mVertIndex[1]].mVertex, mMatrix[TS_SCRREN]);
	v2 = Vec4MulMat4(mVertexs[face.mVertIndex[2]].mVertex, mMatrix[TS_SCRREN]);
}

void CGrapic::SetVertex( CVertex* vertexs, int vertexNum )
{
	if (mVertexNum != vertexNum)
	{
		delete[] mVertexs;
		mVertexs = new CVertex[vertexNum];
		mVertexNum = vertexNum;
	}	
	memcpy(mVertexs, vertexs, sizeof(CVertex) * vertexNum);
}
