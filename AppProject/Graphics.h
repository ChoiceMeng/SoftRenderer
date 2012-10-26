#pragma once
#include "common.h"
#include <Windows.h>
#include "Texture.h"
#include "Camera.h"
#include "Pre.h"

enum ETS_Transform
{
	TS_LOCAL	= 0,
	TS_WORLD	= 1,
	TS_VIEW		= 2,
	TS_PROJECT	= 3,
	TS_SCRREN	= 4,
	TS_NUM
};

enum ESHADE_Type
{
	SHADE_FLAT				= 0, // 平面着色
	SHADE_GAUROUD			= 1,
	SHADE_PHONG_Vertex		= 2,
	SHADE_PHONG_PIXEL		= 3,
};

enum EFill_Type
{
	FILL_WIREFRAME	= 1,
	FILL_SOLID		= 2,
};

class CGraphics
{
public:
	CGraphics(void);
	~CGraphics(void);
public:
	bool InitGDI(HINSTANCE hinstance);
	void ShutdownGraphicss();

// 
 	void	SetFace(CFace* faces, int faceNum);
 	void	SetVertex(CVertex* vertexs, int vertexNum);
 	void	DrawPrimitives();
// 
 	void	SetCamera(CCamera* camera);
	void	SetTransform(ETS_Transform space, const Matrix4& mat);
	void	SetTexture(/*int index, */CTexture* texture);
	void	SetBumpTexture(CTexture* bumpTex);
	void	SetLight(/*int index, */CLight* light);
// 	void	SetCullMode(ECULL_Type cullType) { mCullType = cullType; }
// 	void	SetShadeMode(ESHADE_Type shadeType) { mShadeType = shadeType; }
// 	void	SetFillMode(EFill_Type fillType) { mFillType = fillType; }
// 
// 	////////////////////////////////////////////////////////////////////////// process
 	void	BuildMatrix();
 	void	TranslateToViewSpace();
 	void	TranslateToProjectiveSpace();
 	void	TranslateToScreenSpace(const CFace& face, Vector4& v0, Vector4& v1, Vector4& v2);
// 
// 	void	ProcessCull();
 	void	ProcessLight();
	CColor PhongCal(CLight* pLight, const Vector4& lightPos, const Vector4& posView, const Vector4& normal);

	// 光栅化
 	void	ProcessRasterize();
	// 光栅化函数
	void RasterizeFace(int faceIndex,
		Vector4& v0, Vector4& v1, Vector4& v2, 
		Vector4& v0V, Vector4& v1V, Vector4& v2V,
		Vector4& n0,  Vector4& n1, Vector4& n2,
		Vector4& uv0, Vector4& uv1, Vector4& uv2,
		CColor& c0, CColor& c1, CColor& c2);

	void RasterizeFace(const Vector4& v0, const Vector4& v1, const Vector4& v2, 
		const Vector4& uv0, const Vector4& uv1, const Vector4& uv2,
		const CColor& c0, const CColor& c1, const CColor& c2);
	void CGraphics::RasterizeFlatFaceUp(const Vector4& v0, const Vector4& v1, const Vector4& v2, 
		const Vector4& uv0, const Vector4& uv1, const Vector4& uv2,
		const CColor& c0, const CColor& c1, const CColor& c2);

	void RasterizeFlatFaceDown(const Vector4& v0, const Vector4& v1, const Vector4& v2, 
		const Vector4& uv0, const Vector4& uv1, const Vector4& uv2,
		const CColor& c0, const CColor& c1, const CColor& c2);

	void SetPixel(int x, int y, const CColor& color);
	//////////////////////////////////////////////////////////////////////////

	// void SwapVectex(	Vector4& v0, Vector4& v0V, Vector4& n0, Vector4& uv0, CColor& c0, 
	//	Vector4& v1, Vector4& v1V, Vector4& n1, Vector4& uv1, CColor& c1);

	// 清空当前缓冲区, 并将其颜色设置为黑色
	void	ClearBuffer(const CColor &c = CColor(0, 0, 255));
	// 将已经绘制好的缓冲区递交给Graphics在屏幕上绘制, 并将当前缓冲区设置为另一个缓冲区
	void	FlipBuffer(HDC hdc);

	int		GetScreenWidth() const { return SCREEN_WIDTH; }
	int		GetScreenHeight() const { return SCREEN_HEIGHT; }

	bool		CheckZ(int x, int y, float z);
private:
	HBITMAP							mBufferedHandle;
	HDC								mBufferedHDC;

	DIBSECTION					mDIBSection;

	BYTE*							mDatas;
	int								mPitch;

	RECT							mBufferSize;

	CCamera*					m_pCamera;

	HBRUSH							mBgBrush;
	HPEN							mPen;

	float*							mZBuffer; // z buff

	Matrix4						mMatrix[TS_NUM];

	// OBJECT DATA
	CFace*							mFaces;
	int								mFaceNum;
	CVertex*						mVertexs;
	int								mVertexNum;

	Matrix4							m_matWorld2Local;

	CLight*							mLights/*[MAX_LIGHT_NUM]*/;
	Vector4							mLightPositionView;	// 光照位置, 摄像机坐标系下
	CTexture*						mTextures/*[MAX_TEXTURE_NUM]*/; // 暂时一张纹理

	ESHADE_Type						mShadeType;
	EFill_Type								mFillType;
};

extern HINSTANCE GHInstance;