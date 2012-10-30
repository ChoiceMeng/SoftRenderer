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

	void ShowWindow();
	// window�¼�����
	void StartLoop();

	virtual void OnKeyDown(int msg);

	void DrawInfos();
protected:
	HWND					mHwnd;	// ::CreateWindow����
	std::string				mWindowTitle;
	HINSTANCE				mHinstance;

	CGraphics*				m_pGraphics;
public:
	C3DObject* mObject_Drawing;
};

