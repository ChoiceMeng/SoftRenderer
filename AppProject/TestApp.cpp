#include <Windows.h>
#include "WindowBase.h"

int WINAPI WinMain(
	HINSTANCE hInstance,      // handle to current instance
	HINSTANCE hPrevInstance,  // handle to previous instance
	LPSTR lpCmdLine,          // command line
	int nCmdShow              // show state
	)
{
	CWindowBase::m_pMe = new CWindowBase("SoftRender", hInstance);
	CWindowBase::m_pMe->ShowWindow();
	CWindowBase::m_pMe->StartLoop();
}