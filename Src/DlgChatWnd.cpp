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

void CDlgChatWnd::InitWindow()
{
	InitialCtrl();
}

CDuiString CDlgChatWnd::GetSkinFolder()
{
	CString strXmlPath = CGlobalFunction::GetCurPath();
	return strXmlPath.GetBuffer();
}

CDuiString CDlgChatWnd::GetSkinFile()
{
	return _T("\\CDlgChatWnd.xml");
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
	bool bHandled = false;
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
			bHandled = true;
		}
	}
	if (!bHandled)
	{
		WindowImplBase::Notify(msg);
	}
}
