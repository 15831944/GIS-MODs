#pragma once

#ifndef TIPCOMBOBOXTREECTRL_H
#define TIPCOMBOBOXTREECTRL_H

#include "..\include\ComboBoxTreeCtrl.h"
#include "BCGPTipComboBox.h"

class CTipComboboxTreeCtrl : public CComboBoxTreeCtrl
{
//		DECLARE_DYNAMIC(CTipComboboxTreeCtrl)
public:
	CTipComboboxTreeCtrl()
	{
		m_hWndToolTip = NULL;

		// create tooltip
		m_hWndToolTip = ::CreateWindowEx(WS_EX_TOPMOST,
			TOOLTIPS_CLASS,
			NULL,
			TTS_NOPREFIX | TTS_ALWAYSTIP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			m_hWnd,
			NULL,
			NULL,
			NULL);

		m_point.x = 0;
		m_point.y=0;
		m_bShow = FALSE;

	}


	virtual void PreSubclassWindow();


protected:
	DECLARE_MESSAGE_MAP()

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);

protected:
	//CBCGPTipListBox	m_listbox;
	HWND			m_hWndToolTip;
	TOOLINFO		m_ToolInfo;

	CPoint            m_point;
	BOOL    m_bShow;
};

#endif 