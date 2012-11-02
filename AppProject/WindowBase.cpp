#include "WindowBase.h"
#include "Windowsx.h"
#include <ctime>
#include "Graphics.h"
#include "common.h"

CWindowBase* CWindowBase::m_pMe;
CWindowBase::CWindowBase(const std::string szWindowName, HINSTANCE hInstance):
mWindowTitle(szWindowName), mHinstance(hInstance)
{
	if (!(registerWindow() && createWindow()))
	{
		throw "Init Window Failed!";
	}

	m_pGraphics = new CGraphics;
	m_pGraphics->InitGDI(mHinstance);

	mObject_Drawing = m_pGraphics->mObject_Teapot;

	OnUpdate();

	UpdateWindow();
}


CWindowBase::~CWindowBase(void)
{
	m_pGraphics->ShutdownGraphicss();



	DestroyWindow(mHwnd);
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
	if (!CWindowBase::m_pMe)
		return DefWindowProc(hWnd, Msg, wParam, lParam);

	switch(Msg)
	{
		// 绘制
	case WM_MOVING:
	case WM_PAINT:
		{
			PAINTSTRUCT  ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			CWindowBase::m_pMe->OnPaint(hdc);
			EndPaint(hWnd, &ps);
			break;
		}
	case WM_TIMER:
		{
			CWindowBase::m_pMe->UpdateWindow();
			break;
		}
		// 键盘按下
	/*case WM_KEYDOWN:
		{
			CWindowBase::m_pMe->OnKeyDown(wParam);
			break;
		}
		// 键盘释放
	case WM_KEYUP:
		{
			CWindowBase::m_pMe->OnKeyRelease(wParam);
			break;
		}
	case  WM_LBUTTONDOWN:
		{
			CWindowBase::m_pMe->OnMousePress(false);
			break;
		}
	case WM_LBUTTONUP:
		{
			CWindowBase::m_pMe->OnMouseRelease(false);
			break;
		}
	case  WM_RBUTTONDOWN:
		{
			CWindowBase::m_pMe->OnMousePress(true);
		}
	case WM_RBUTTONUP:
		{
			CWindowBase::m_pMe->OnMouseRelease(true);
			break;
		}
	case WM_MOUSEWHEEL:
		{
			short delta = GET_WHEEL_DELTA_WPARAM(wParam); // wheel rotation
			CWindowBase::m_pMe->OnMouseWheel((int)(delta * 0.5f / PI));
		}

		// 鼠标移动
	case WM_MOUSEMOVE:
		{
			int x = GET_X_LPARAM(lParam); 
			int y = GET_Y_LPARAM(lParam); 
			CWindowBase::m_pMe->OnMouseMove(x, y);
			break;
		}*/
		// 关闭窗口
	case WM_CLOSE:
		{
			DestroyWindow(hWnd);
			break;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	return 0;
}

void CWindowBase::OnKeyDown(int msg)
{
	//::MessageBox(0, "Window onKeyDown", 0, 0);
	/*switch(msg)
	{
	case 'H':
	case 'h':
		mShowHelp = !mShowHelp;
		break;
	case '1':
		mShadeMode = SHADE_FLAT; break;
	case '2':
		mShadeMode = SHADE_GAUROUD; break;
	case '3':
		mShadeMode = SHADE_PHONG_Vertex; break;
	case '4':
		mShadeMode = SHADE_PHONG_PIXEL; break;
	case 'Q':
	case 'q':
		mUseLight = !mUseLight;
		break;
	case 'E':
	case 'e':
		{
			static int i = 0;
			i = (i + 1) % 3;
			if (i == 0)
				mUsedTexture = mTexture1;
			else if (i == 1)
				mUsedTexture = mTexture2;
			else
				mUsedTexture = NULL;
		}
		break;
	case 'X':
	case 'x':
		{
			mCamera->mWorldPosition = FVector4(30.0f, 30.0f, 0.001f);
			mCamera->mLookAt		= FVector4(0.0f, 6.0f, 0.0f);
			mCamera->mUp			= FVector4(0.0f, 1.0f, 0.0f);
			break;
		}
	case 'C':
	case 'c':
		{
			mObject_Drawing = (mObject_Drawing == mObject_Sphere ? mObject_Teapot : mObject_Sphere);
		}
		break;
	case 'R':
	case 'r':
		mSelfRotate = !mSelfRotate; break;
	case 'W':
	case 'w':
		mCamera->MoveForward();break;
	case 'S':
	case 's':
		mCamera->MoveBackward();break;
	case 'A':
	case 'a':
		mCamera->MoveLeft();break;
	case 'D':
	case 'd':
		mCamera->MoveRight();break;
	case 'F':
	case 'f':
		mUseBumpMap = !mUseBumpMap; break;
	}*/
}

// 窗口绘制函数
void CWindowBase::OnPaint(HDC hdc)
{

	m_pGraphics->ClearBuffer();
	
	//----Begin Object 1----
	Matrix4 mat = IDENTITY;
	static long t = 90;

	//if (mSelfRotate)
	//	t += 2;

	// Matrix4::RotateY(mat, t);
	m_pGraphics->SetTransform(TS_LOCAL, mat);
	// TranslateMatrix44(mat, 0, 0, 0);
	m_pGraphics->SetTransform(TS_WORLD, mat);

	// 提交并绘制
	m_pGraphics->SetVertex(mObject_Drawing->m_pVertexList, mObject_Drawing->m_nVetexCount);
	m_pGraphics->SetFace(mObject_Drawing->m_pFace, mObject_Drawing->m_nFaceCount);
	m_pGraphics->DrawPrimitives();
	//----End Object 1-----

	DrawInfos();
	
	m_pGraphics->PrintFloatBuff();

	m_pGraphics->mVisibleFaceNum = 0;
	m_pGraphics->FlipBuffer(hdc);
}

void CWindowBase::DrawInfos()
{
// 	int i = mGraphics->GetVisibleFaceNum();
 	std::string visibleFace = IntToString((float)m_pGraphics->mVisibleFaceNum);
 //	std::string totalFace	= IntToString((float)m_pGraphics->GetTotalFaceNum());
 	std::string faces = "  Faces [Visble/Total] - " + visibleFace;// + " / " + totalFace;
	m_pGraphics->DrawString(faces, 10, 35, CColor(0, 255, 0));
// 
// 	std::string visibleFace = IntToString((float)mGraphics->GetVisibleFaceNum());
	std::string szCameraPos = "CameraPos:" +  IntToString((int)(m_pGraphics->GetCamera()->m_WorldPos.x)) + "," +  IntToString((int)(m_pGraphics->GetCamera()->m_WorldPos.y)) + "," +  IntToString((int)(m_pGraphics->GetCamera()->m_WorldPos.z));
 	m_pGraphics->DrawString(szCameraPos, 10, 15, CColor(0, 255, 0));
// 
// 	if (mShowHelp)
// 	{
// 		std::string help;
// 		help = "  1. Flat 2. Gaurund 3. Phong Vext 4. Phong Pixel [Shading]";
// 		mGraphics->DrawString(help, 10, 40, FColor(0, 255, 0));
// 		help = "  F. Bump Map On/Off(Phong Pixl Textrued)";
// 		mGraphics->DrawString(help, 10, 55, FColor(0, 255, 0));
// 		help = "  E. Texture 1/Texture 2/Off";
// 		mGraphics->DrawString(help, 10, 70, FColor(0, 255, 0));
// 		help = "  R. Self Rotation";
// 		mGraphics->DrawString(help, 10, 85, FColor(0, 255, 0));
// 		help = "  Q. Light On/Off";
// 		mGraphics->DrawString(help, 10, 100, FColor(0, 255, 0));
// 		help = "  X. Reset Camera";
// 		mGraphics->DrawString(help, 10, 115, FColor(0, 255, 0));
// 		help = "  C. Change Model";
// 		mGraphics->DrawString(help, 10, 130, FColor(0, 255, 0));
// 
// 		help = "  W. S. A. D. Move Camera";
// 		mGraphics->DrawString(help, 10, 430, FColor(0, 255, 0));
// 	}
// 	else
// 	{
// 		std::string help;
// 		help = "  [Help ] H.";
// 		mGraphics->DrawString(help, 10, 40, FColor(0, 255, 0));
// 	}
}

void CWindowBase::OnUpdate()
{
	// Graphics update

}

void CWindowBase::UpdateWindow()
{
	// 刷新窗口
	::InvalidateRect (mHwnd, NULL, FALSE) ;
	::UpdateWindow(mHwnd);
}

void CWindowBase::ShowWindow()
{
	::ShowWindow(mHwnd, SW_SHOW);
	::UpdateWindow(mHwnd);
}

void CWindowBase::StartLoop()
{
	//进入消息循环
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );
	while( msg.message!=WM_QUIT )
	{
		if( ::PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			::TranslateMessage( &msg );
			::DispatchMessage( &msg );
		}
		else
		{
			OnUpdate();  //渲染图形
			::Sleep(1);
		}
	}
}
