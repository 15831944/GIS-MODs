#include "StdAfx.h"
#include "BCGPTipListBox.h"


///////////////////////////////////////////////////////////////////////////////
// CXTipListBox

BEGIN_MESSAGE_MAP(CBCGPTipListBox, CBCGPListBox)
	//{{AFX_MSG_MAP(CXTipListBox)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
CBCGPTipListBox::CBCGPTipListBox(void)
{
	m_hWndToolTip = NULL;

	m_point.x = 0;
	m_point.y=0;
	m_bShow = FALSE;
}



///////////////////////////////////////////////////////////////////////////////
// PreSubclassWindow
void CBCGPTipListBox::PreSubclassWindow() 
{
	TRACE(_T("in CXTipListBox::PreSubclassWindow\n"));

	CBCGPListBox::PreSubclassWindow();

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
	ASSERT(m_hWndToolTip);

	// initialize toolinfo struct
	memset(&m_ToolInfo, 0, sizeof(m_ToolInfo));
	m_ToolInfo.cbSize = sizeof(m_ToolInfo);
	m_ToolInfo.uFlags = TTF_TRACK | TTF_TRANSPARENT;
	m_ToolInfo.hwnd = m_hWnd;

	// add list box
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

	// remove border (listbox items only)
	LONG lStyle = ::GetWindowLong(m_hWndToolTip, GWL_STYLE);
	lStyle &= ~WS_BORDER;
	::SetWindowLong(m_hWndToolTip, GWL_STYLE, lStyle);
}

///////////////////////////////////////////////////////////////////////////////
// OnMouseMove
void CBCGPTipListBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(point.y==m_point.y&&m_bShow)
	{
		m_point = point;
		return CBCGPListBox::OnMouseMove(nFlags, point);
	}
	m_point = point;

	CRect rectClient;
	GetClientRect(&rectClient);

	if (rectClient.PtInRect(point))
	{
		CPoint pointScreen;
		::GetCursorPos(&pointScreen);
		BOOL bOutside = FALSE;
		int nItem = ItemFromPoint(point, bOutside);  // calculate listbox item number (if any)

		if (!bOutside && (nItem >= 0))
		{
			CString strText = _T("");
			GetText(nItem, strText);
			m_ToolInfo.lpszText = (LPTSTR)(LPCTSTR)strText;

			CRect rect;
			GetItemRect(nItem, &rect);
			ClientToScreen(&rect);

			HDC hDC = ::GetDC(m_hWnd);
			ASSERT(hDC);

			CFont *pFont = GetFont();
			HFONT hOldFont = (HFONT) ::SelectObject(hDC, (HFONT) *pFont);

			SIZE size;
			::GetTextExtentPoint32(hDC, strText, strText.GetLength(), &size);
			::SelectObject(hDC, hOldFont);
			::ReleaseDC(m_hWnd, hDC);

			//TRACE(_T("<%s>  cx=%d  width=%d\n"), strText, size.cx, rect.Width());
			if (size.cx > (rect.Width() - 3))
			{
				::SendMessage(m_hWndToolTip, TTM_UPDATETIPTEXT, 0, (LPARAM) &m_ToolInfo);
				::SendMessage(m_hWndToolTip, TTM_TRACKPOSITION, 0, 
					(LPARAM) MAKELONG(rect.left, rect.top));
				::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, TRUE, (LPARAM)(LPTOOLINFO) &m_ToolInfo);
				m_bShow = TRUE;
				SetTimer(1, 80, NULL);	// set timer for out-of-rect detection
			}
			else
			{
				::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, FALSE, (LPARAM)(LPTOOLINFO) &m_ToolInfo);
				m_bShow = FALSE;
			}
		}
	}
	else
	{
		::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, FALSE, (LPARAM)(LPTOOLINFO) &m_ToolInfo);
		m_bShow = FALSE;
	}

	CBCGPListBox::OnMouseMove(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// OnTimer
void CBCGPTipListBox::OnTimer(UINT nIDEvent) 
{
	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);

	CRect rectClient;
	GetClientRect(&rectClient);

	DWORD dwStyle = GetStyle();
	if ((!rectClient.PtInRect(point)) || ((dwStyle & WS_VISIBLE) == 0))
	{
		TRACE(_T("not in listbox =====\n"));
		KillTimer(nIDEvent);
		::SendMessage(m_hWndToolTip, TTM_TRACKACTIVATE, FALSE, (LPARAM)(LPTOOLINFO) &m_ToolInfo);
		m_bShow = FALSE;
	}
}