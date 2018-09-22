#include "StdAfx.h"
#include "BCGPTipComboBox.h"


///////////////////////////////////////////////////////////////////////////////
// CXTipComboBox

BEGIN_MESSAGE_MAP(CBCGPTipComboBox, CComboBox)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
CBCGPTipComboBox::CBCGPTipComboBox(void)
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

///////////////////////////////////////////////////////////////////////////////
// PreSubclassWindow
void CBCGPTipComboBox::PreSubclassWindow() 
{
	TRACE(_T("in CXTipComboBox::PreSubclassWindow\n"));

	CComboBox::PreSubclassWindow();

	
	ASSERT(m_hWndToolTip);
	// initialize toolinfo struct
	memset(&m_ToolInfo, 0, sizeof(m_ToolInfo));
	m_ToolInfo.cbSize = sizeof(m_ToolInfo);
	m_ToolInfo.uFlags = TTF_TRACK | TTF_TRANSPARENT;
	m_ToolInfo.hwnd = m_hWnd;

	// add combo box
	::SendMessage(m_hWndToolTip, TTM_SETMAXTIPWIDTH, 0, SHRT_MAX);
	::SendMessage(m_hWndToolTip, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &m_ToolInfo);
	::SendMessage(m_hWndToolTip, TTM_SETTIPBKCOLOR, ::GetSysColor(COLOR_HIGHLIGHT), 0);
	::SendMessage(m_hWndToolTip, TTM_SETTIPTEXTCOLOR, ::GetSysColor(COLOR_HIGHLIGHTTEXT), 0);

	// reduce top & bottom margins
	CRect rectMargins(0,-1,0,-1);
	::SendMessage(m_hWndToolTip, TTM_SETMARGIN, 0, (LPARAM)&rectMargins);
	// set font
	CFont *pFont = GetFont();
	::SendMessage(m_hWndToolTip, WM_SETFONT, (WPARAM)(HFONT)*pFont, FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// OnCtlColor
HBRUSH CBCGPTipComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		if (m_listbox.GetSafeHwnd() == NULL)
		{
			TRACE(_T("subclassing listbox\n"));
			m_listbox.SubclassWindow(pWnd->GetSafeHwnd());
		}
	}
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

///////////////////////////////////////////////////////////////////////////////
// OnDestroy
void CBCGPTipComboBox::OnDestroy() 
{
	if (m_listbox.GetSafeHwnd() != NULL)
		m_listbox.UnsubclassWindow();

	CComboBox::OnDestroy();
}

///////////////////////////////////////////////////////////////////////////////
// OnMouseMove
void CBCGPTipComboBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(point.y==m_point.y&&m_bShow)
	{
		m_point = point;
		return CComboBox::OnMouseMove(nFlags, point);
	}
	m_point = point;

	CRect rectClient;
	GetClientRect(&rectClient);
	int nComboButtonWidth = ::GetSystemMetrics(SM_CXHTHUMB) + 2;
	rectClient.right = rectClient.right - nComboButtonWidth;

	if (rectClient.PtInRect(point))
	{
		TRACE(_T("in ccombo\n"));
		ClientToScreen(&rectClient);

		CString strText = _T("");
		GetWindowText(strText);
		m_ToolInfo.lpszText = (LPTSTR)(LPCTSTR)strText;

		HDC hDC = ::GetDC(m_hWnd);
		ASSERT(hDC);

		CFont *pFont = GetFont();
		HFONT hOldFont = (HFONT) ::SelectObject(hDC, (HFONT) *pFont);

		SIZE size;
		::GetTextExtentPoint32(hDC, strText, strText.GetLength(), &size);
		::SelectObject(hDC, hOldFont);
		::ReleaseDC(m_hWnd, hDC);

		if (size.cx > (rectClient.Width() - 6))
		{
			rectClient.left += 1;
			rectClient.top += 3;

			COLORREF rgbText = ::GetSysColor(COLOR_WINDOWTEXT);
			COLORREF rgbBackground = ::GetSysColor(COLOR_WINDOW);

			CWnd *pWnd = GetFocus();
			if (pWnd)
			{
				if (pWnd->m_hWnd == m_hWnd)
				{
					rgbText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
					rgbBackground = ::GetSysColor(COLOR_HIGHLIGHT);
				}
			}

			::SendMessage(m_hWndToolTip, TTM_SETTIPBKCOLOR, rgbBackground, 0);
			::SendMessage(m_hWndToolTip, TTM_SETTIPTEXTCOLOR, rgbText, 0);
			::SendMessage(m_hWndToolTip, TTM_UPDATETIPTEXT, 0, (LPARAM) &m_ToolInfo);
			::SendMessage(m_hWndToolTip, TTM_TRACKPOSITION, 0, 
				(LPARAM) MAKELONG(rectClient.left, rectClient.top));
			::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, TRUE, (LPARAM)(LPTOOLINFO) &m_ToolInfo);
			m_bShow = TRUE;
			TRACE(_T("setting timer\n"));
			SetTimer(1, 80, NULL);	// set timer for out-of-rect detection
		}
		else	// text fits inside client rect
		{
			::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, FALSE, (LPARAM)(LPTOOLINFO) &m_ToolInfo);
			m_bShow = FALSE;
		}
	}
	else	// not inside client rect
	{
		::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, FALSE, (LPARAM)(LPTOOLINFO) &m_ToolInfo);
		m_bShow = FALSE;
	}

	CComboBox::OnMouseMove(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// OnTimer
void CBCGPTipComboBox::OnTimer(UINT nIDEvent) 
{
	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);

	CRect rectClient;
	GetClientRect(&rectClient);
	int nComboButtonWidth = ::GetSystemMetrics(SM_CXHTHUMB) + 2;

	rectClient.right = rectClient.right - nComboButtonWidth;

	if (!rectClient.PtInRect(point))
	{
		TRACE(_T("killing timer\n"));
		KillTimer(nIDEvent);
		::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, FALSE, (LPARAM)(LPTOOLINFO) &m_ToolInfo);
		m_bShow = FALSE;
	}
}

