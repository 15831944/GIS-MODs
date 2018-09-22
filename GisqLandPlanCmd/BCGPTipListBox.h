/*********************************************
�ļ���:  CBCGPTipListBox
������:  liyuchun
��д����:2016-08-30
��Ҫ����:��������ʾ
�޸ļ�¼(who-when-why):
********************************************/

#ifndef BCGP_LISTCOMBOX_H
#define BCGP_LISTCOMBOX_H

class  CBCGPTipListBox : public CBCGPListBox
{
// Construction
public:
	CBCGPTipListBox();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXTipListBox)
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HWND			m_hWndToolTip;
	TOOLINFO		m_ToolInfo;
	CPoint            m_point;
	BOOL    m_bShow;
	// Generated message map functions
protected:
	//{{AFX_MSG(CXTipListBox)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif 


