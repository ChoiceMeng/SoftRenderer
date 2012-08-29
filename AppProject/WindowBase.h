#pragma once
#include "common.h"
#include <Windows.h>

class CWindowBase
{
public:
	CWindowBase(const std::string szWindowName, HINSTANCE hInstance);
	~CWindowBase(void);
public:
	static CWindowBase* m_pMe;

	// ȫ��Win�¼��ص�����
	static LRESULT WinProc(HWND hWnd, UINT Msg,WPARAM wParam,LPARAM lParam);

	// ע�ᴰ��
	bool registerWindow();
	// ��������
	bool createWindow();
protected:
	HWND					mHwnd;	// ::CreateWindow����
	std::string				mWindowTitle;
	HINSTANCE				mHinstance;
};

