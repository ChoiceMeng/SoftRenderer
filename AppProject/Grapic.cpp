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