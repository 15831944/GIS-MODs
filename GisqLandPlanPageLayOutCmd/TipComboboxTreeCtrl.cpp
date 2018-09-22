#include "StdAfx.h"
#include "TipComboboxTreeCtrl.h"

BEGIN_MESSAGE_MAP(CTipComboboxTreeCtrl, CComboBoxTreeCtrl)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// PreSubclassWindow
void CTipComboboxTreeCtrl::PreSubclassWindow() 
{
	TRACE(_T("in CXTipComboBox::PreSubclassWindow\n"));

	CComboBoxTreeCtrl::PreSubclassWindow();


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
HBRUSH CTipComboboxTreeCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		if (m_wndTreeList.GetSafeHwnd() == NULL)
		{
			TRACE(_T("subclassing listbox\n"));
			m_wndTreeList.SubclassWindow(pWnd->GetSafeHwnd());
		}
	}
	HBRUSH hbr = CComboBoxTreeCtrl::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

///////////////////////////////////////////////////////////////////////////////
// OnDestroy
void CTipComboboxTreeCtrl::OnDestroy() 
{
	if (m_wndTreeList.GetSafeHwnd() != NULL)
		m_wndTreeList.UnsubclassWindow();

	CComboBoxTreeCtrl::OnDestroy();
}

///////////////////////////////////////////////////////////////////////////////
// OnMouseMove
void CTipComboboxTreeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
// 	if (m_wndTreeList.IsWindowVisible())
// 	{
// 		CComboBoxTreeCtrl::OnMouseMove(nFlags, point);
// 		::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, FALSE, (LPARAM)(LPTOOLINFO) &m_ToolInfo);
// 		m_bShow = FALSE;
// 		return;
// 	}
	if(point.y==m_point.y&&m_bShow)
	{
		m_point = point;
		return CComboBoxTreeCtrl::OnMouseMove(nFlags, point);
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
				(LPARAM) MAKELONG(rectClient.right+20, rectClient.top));
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

	CComboBoxTreeCtrl::OnMouseMove(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// OnTimer
void CTipComboboxTreeCtrl::OnTimer(UINT nIDEvent) 
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

