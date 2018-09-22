/*********************************************
�ļ���:  CBCGPTipComboBox
������:  liyuchun
��д����:2016-08-30
��Ҫ����:��������ʾ
�޸ļ�¼(who-when-why):
********************************************/

#ifndef BCGPTIPCOMBOBOX_H	
#define BCGPTIPCOMBOBOX_H

#include "BCGPTipListBox.h"

class CBCGPTipComboBox : public CBCGPComboBox
{
	// Construction
public:
	CBCGPTipComboBox();
	
	virtual void PreSubclassWindow();


protected:
	DECLARE_MESSAGE_MAP()

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);

protected:
	CBCGPTipListBox	m_listbox;
	HWND			m_hWndToolTip;
	TOOLINFO		m_ToolInfo;

	CPoint            m_point;
	BOOL    m_bShow;
};

#endif 



