// DuiTcpChatTool.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "DuiTcpChatTool.h"
#include "DlgChatWnd.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	CDlgChatWnd chatWnd;
	chatWnd.Create(NULL, _T("chatWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	chatWnd.ShowModal();
	return 0;
}
