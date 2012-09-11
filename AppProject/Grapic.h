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

class CGrapic
{
public:
	CGrapic(void);
	~CGrapic(void);
public:
	bool InitGDI(HINSTANCE hinstance);
	void ShutdownGraphics();

// 
 	void	SetFace(CFace* faces, int faceNum);
 	void	SetVertex(CVertex* vertexs, int vertexNum);
// 	void	DrawPrimitives();
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
 	void	TanslateToViewSpace();
 	void	TanslateToProjectiveSpace();
 	void	TanslateToScreenSpace(const CFace& face, Vector4& v0, Vector4& v1, Vector4& v2);
// 
// 	void	ProcessCull();
 	void	ProcessLight();
// 	void	ProcessRasterize();
	//////////////////////////////////////////////////////////////////////////

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