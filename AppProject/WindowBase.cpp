#include "WindowBase.h"
#include "Windowsx.h"
#include <ctime>


CWindowBase::CWindowBase(const std::string szWindowName, HINSTANCE hInstance):
mWindowTitle(szWindowName), mHinstance(hInstance)
{
}


CWindowBase::~CWindowBase(void)
{
}

bool CWindowBase::createWindow()
{
	mHwnd = ::CreateWindow(	"SoftRender", mWindowTitle.c_str(), 
		// 窗口不可更改大小
		WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX)& (~WS_THICKFRAME),
		CW_USEDEFAULT, CW_USEDEFAULT, 
		SCREEN_WIDTH, SCREEN_HEIGHT, 
		0, 0, mHinstance, 0);

	if (!mHwnd)
	{
		::MessageBox(0, "Create Window Failed", 0, 0);
		return false;
	}

	::SetTimer(mHwnd, WM_TIMER, 30,NULL);
	// 手动播种
	srand((unsigned int)time(NULL));

	RECT rect;
	GetClientRect(mHwnd,&rect); 
	int scrWidth=GetSystemMetrics(SM_CXSCREEN);
	int scrHeight=GetSystemMetrics(SM_CYSCREEN);
	rect.left=(scrWidth-rect.right)/2;  
	rect.top=(scrHeight-rect.bottom)/2;  
	//设置窗体位置  
	SetWindowPos(mHwnd,HWND_TOP,rect.left,rect.top, SCREEN_WIDTH,SCREEN_HEIGHT,SWP_SHOWWINDOW);  

	return true;
}

bool CWindowBase::registerWindow()
{
	WNDCLASS wind;
	{
		wind.style			= CS_HREDRAW | CS_VREDRAW;
		wind.lpfnWndProc	= (WNDPROC)CWindowBase::WinProc;
		wind.cbClsExtra		= 0;
		wind.cbWndExtra		= 0;
		wind.hInstance		= mHinstance;
		wind.hIcon			= LoadIcon(0, IDI_APPLICATION);
		wind.hCursor		= LoadCursor(0, IDC_ARROW);
		wind.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
		wind.lpszMenuName	= 0;
		wind.lpszClassName	= "SoftRender";
	}

	if (!RegisterClass(&wind))
	{
		::MessageBox(0, "Register Class Failed", 0, 0);
		return false;
	}

	return true;
}

LRESULT CWindowBase::WinProc( HWND hWnd, UINT Msg,WPARAM wParam,LPARAM lParam )
{

	return 0;
}

