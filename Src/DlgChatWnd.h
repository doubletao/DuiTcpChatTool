#pragma once

class CDlgChatWnd : public CWindowWnd, public INotifyUI
{
public:
	CDlgChatWnd(void);
	virtual ~CDlgChatWnd(void);
	virtual LPCTSTR GetWindowClassName() const;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Notify(TNotifyUI& msg);
protected:
	void OnInitialDlg();//加载xml等初始化动作
	void InitialCtrl();//映射控件到成员变量
protected:
	CPaintManagerUI m_PaintManager;

	CButtonUI * m_pBtnSend;
	CListUI * m_pListUser;
	CEditUI * m_pEditChatLog;
	CEditUI * m_pEditLoginLog;
	CEditUI * m_pEditInputBox;
};
