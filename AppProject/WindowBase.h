#pragma once
#include "common.h"
#include "Pre.h"

class CGraphics;
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

	void ShowWindow();
	// window事件处理
	void StartLoop();

	virtual void OnKeyDown(int msg);

	void DrawInfos();
protected:
	HWND					mHwnd;	// ::CreateWindow返回
	std::string				mWindowTitle;
	HINSTANCE				mHinstance;

	CGraphics*				m_pGraphics;
public:
	C3DObject* mObject_Drawing;
};

