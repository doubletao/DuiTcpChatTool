// DuiTcpChatTool.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "DuiTcpChatTool.h"

#define MAX_LOADSTRING 100

class CDuiFrameWnd : public CWindowWnd, public INotifyUI
{
public:
	virtual LPCTSTR GetWindowClassName() const { return _T("DUIMainFrame"); }
	virtual void Notify(TNotifyUI& msg) 
	{
		if (msg.sType == _T("click"))
		{
			if (msg.pSender->GetName() == _T("btntest"))
			{
				int i = 0;
				i++;
			}
		}
	}

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;

		if( uMsg == WM_CREATE ) 
		{
			CControlUI *pWnd = new CButtonUI;
			pWnd->SetText(_T("Hello World"));   // 设置文字
			pWnd->SetName(_T("btntest"));
			pWnd->SetBkColor(0xfff0f0f0);       // 设置背景色

			m_PaintManager.Init(m_hWnd);
			m_PaintManager.AttachDialog(pWnd);
			m_PaintManager.AddNotifier(this);
			return lRes;
		}
		else if (uMsg == WM_LBUTTONDOWN)
		{
			int i = 0;
			i++;
		}

		if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) 
		{
			return lRes;
		}

		return __super::HandleMessage(uMsg, wParam, lParam);
	}

protected:
	CPaintManagerUI m_PaintManager;
};

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	CDuiFrameWnd wnd;
	wnd.Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	wnd.ShowModal();
}
