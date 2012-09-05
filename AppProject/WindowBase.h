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
	// 注册窗口
	bool registerWindow();
	// 创建窗口
	bool createWindow();
	// 全局Win事件回调函数
	static LRESULT WinProc(HWND hWnd, UINT Msg,WPARAM wParam,LPARAM lParam);
	// 窗口绘制函数
	virtual void OnPaint(HDC hdc);
	virtual void OnUpdate();

	void UpdateWindow();

	// window事件处理
	void StartLoop();

	virtual void OnKeyDown(int msg);
protected:
	HWND					mHwnd;	// ::CreateWindow返回
	std::string				mWindowTitle;
	HINSTANCE				mHinstance;
};

