#include "StdAfx.h"
#include "DlgChatWnd.h"
#include "GlobalFunction.h"

CDlgChatWnd::CDlgChatWnd(void)
{
}


CDlgChatWnd::~CDlgChatWnd(void)
{
}

LPCTSTR CDlgChatWnd::GetWindowClassName() const
{
	return _T("DUIMainFrame");
}

LRESULT CDlgChatWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;

	if( uMsg == WM_CREATE ) 
	{
		OnInitialDlg();
		InitialCtrl();
		return lRes;
	}

	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) 
	{
		return lRes;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CDlgChatWnd::OnInitialDlg()
{
	//初始化manager
	m_PaintManager.Init(m_hWnd);
	//解析xml
	CDialogBuilder builder;
	CString strXmlPath = CGlobalFunction::GetCurPath();
	strXmlPath.Append(_T("\\CDlgChatWnd.xml"));
	CControlUI* pRoot = builder.Create(strXmlPath.GetBuffer(), (UINT)0, NULL, &m_PaintManager);
	ASSERT(pRoot && "Failed to parse XML");
	//attach对话框内容到manager
	m_PaintManager.AttachDialog(pRoot);
	//消息由自己解析
	m_PaintManager.AddNotifier(this);
}

void CDlgChatWnd::InitialCtrl()
{
	m_pBtnSend = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("ButtonSend")));
	m_pListUser = static_cast<CListUI*>(m_PaintManager.FindControl(_T("ListUser")));
	m_pEditChatLog = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("EditChatLog")));
	m_pEditLoginLog = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("EditLoginLog")));
	m_pEditInputBox = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("EditInputBox")));
}

void CDlgChatWnd::Notify(TNotifyUI& msg)
{
	if(msg.sType == _T("click"))
	{
		if(msg.pSender == m_pBtnSend)
		{
			CString strMsg = m_pEditInputBox->GetText();
			CString strChatLog = m_pEditChatLog->GetText();
			strChatLog.AppendFormat(_T("\n%s"), strMsg);
			m_pEditChatLog->SetText(strChatLog);

			CListTextElementUI* pListElement = new CListTextElementUI;
			m_pListUser->Add(pListElement);
			pListElement->SetText(0, strMsg);
		}
	}
}
