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

	//////////////////////////////////////////////////////////////////////////

	// 清空当前缓冲区, 并将其颜色设置为黑色
	void	ClearBuffer(const CColor &c = CColor(0, 0, 255));
	// 将已经绘制好的缓冲区递交给Graphics在屏幕上绘制, 并将当前缓冲区设置为另一个缓冲区
	void	FlipBuffer(HDC hdc);

	int		GetScreenWidth() const { return SCREEN_WIDTH; }
	int		GetScreenHeight() const { return SCREEN_HEIGHT; }
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
	CTexture*						mTextures/*[MAX_TEXTURE_NUM]*/;

	ESHADE_Type						mShadeType;
};

extern HINSTANCE GHInstance;