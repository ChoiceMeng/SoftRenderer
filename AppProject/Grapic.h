#pragma once
#include "common.h"
#include <Windows.h>
#include "Camera.h"
#include "Pre.h"

class CGrapic
{
public:
	CGrapic(void);
	~CGrapic(void);
public:
	bool InitGDI(HINSTANCE hinstance);
	void ShutdownGraphics();

// 	void	SetTransform(ETS_Transform space, const FMatrix44& mat);
// 	void	SetTexture(/*int index, */FTexture* texture);
// 	void	SetBumpTexture(FTexture* bumpTex);
// 	void	SetLight(/*int index, */FLight* light);
// 
// 	void	SetFace(FFace* faces, int faceNum);
// 	void	SetVertex(FVertex* vertexs, int vertexNum);
// 	void	DrawPrimitives();
// 
// 	void	SetCamera(FCamera* camera);
// 	void	SetCullMode(ECULL_Type cullType) { mCullType = cullType; }
// 	void	SetShadeMode(ESHADE_Type shadeType) { mShadeType = shadeType; }
// 	void	SetFillMode(EFill_Type fillType) { mFillType = fillType; }
// 
// 	////////////////////////////////////////////////////////////////////////// process
// 	void	BuildMatrix();
// 	void	TanslateToViewSpace();
// 	void	TanslateToProjectiveSpace();
// 	void	TanslateToScreenSpace(const FFace& face, FVector4& v0, FVector4& v1, FVector4& v2);
// 
// 	void	ProcessCull();
// 	void	ProcessLight();
// 	void	ProcessRasterize();
	//////////////////////////////////////////////////////////////////////////
private:
	HBITMAP							mBufferedHandle;
	HDC								mBufferedHDC;

	DIBSECTION					mDIBSection;

	BYTE*							mDatas;
	int								mPitch;

	RECT							mBufferSize;

	CCamera					m_pCamera;

	HBRUSH							mBgBrush;
	HPEN							mPen;

	float*							mZBuffer; // z buff
};

extern HINSTANCE GHInstance;