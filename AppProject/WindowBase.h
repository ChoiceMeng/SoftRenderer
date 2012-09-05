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
	// ע�ᴰ��
	bool registerWindow();
	// ��������
	bool createWindow();
	// ȫ��Win�¼��ص�����
	static LRESULT WinProc(HWND hWnd, UINT Msg,WPARAM wParam,LPARAM lParam);
	// ���ڻ��ƺ���
	virtual void OnPaint(HDC hdc);
	virtual void OnUpdate();

	void UpdateWindow();

	// window�¼�����
	void StartLoop();

	virtual void OnKeyDown(int msg);
protected:
	HWND					mHwnd;	// ::CreateWindow����
	std::string				mWindowTitle;
	HINSTANCE				mHinstance;
};

