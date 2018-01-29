#pragma once

class CDlgChatWnd : public WindowImplBase
{
public:
	CDlgChatWnd(void);
	virtual ~CDlgChatWnd(void);
	virtual LPCTSTR GetWindowClassName() const;
	virtual void InitWindow();
	virtual void Notify(TNotifyUI& msg);

	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
protected:
	void InitialCtrl();//映射控件到成员变量
protected:

	CButtonUI * m_pBtnSend;
	CListUI * m_pListUser;
	CEditUI * m_pEditChatLog;
	CEditUI * m_pEditLoginLog;
	CEditUI * m_pEditInputBox;
};
