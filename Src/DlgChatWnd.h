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
	void OnInitialDlg();//����xml�ȳ�ʼ������
	void InitialCtrl();//ӳ��ؼ�����Ա����
protected:
	CPaintManagerUI m_PaintManager;

	CButtonUI * m_pBtnSend;
	CListUI * m_pListUser;
	CEditUI * m_pEditChatLog;
	CEditUI * m_pEditLoginLog;
	CEditUI * m_pEditInputBox;
};
