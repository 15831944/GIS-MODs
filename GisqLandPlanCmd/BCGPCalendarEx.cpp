#include "stdafx.h"
#include <locale.h>


#include <BCGCBPro.h>
#include <BCGPRibbonCustomizePage.h>
#include "BCGPCalendarEx.h"

#define DAYS_IN_WEEK        7
#define ID_TODAY_BUTTON		101

CBCGPCalendarEx::CBCGPCalendarEx(void)
{
	//m_sizeCalendar = CSize(200,200);
// 	m_nVertMargin = 20;
// 	m_nHorzMargin = 20;
}

CBCGPCalendarEx::~CBCGPCalendarEx(void)
{
}

BEGIN_MESSAGE_MAP(CBCGPCalendarEx,CBCGPCalendar)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CBCGPCalendarEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CBCGPCalendar::OnCreate(lpCreateStruct);
	m_btnToday.SetWindowText(_T("今天"));
	
	return 0;
}

void CBCGPCalendarEx::OnSize(UINT nType, int cx, int cy)
{
	CBCGPCalendar::OnSize(nType,cx,cy);
}

void CBCGPCalendarEx::OnDrawMonth (CDC* pDC, CRect rect, int nMonthIndex)
{
	ASSERT_VALID (pDC);

	const BOOL bCtrl = ::GetAsyncKeyState (VK_CONTROL) & 0x8000;

	UINT dtFlags = DT_VCENTER | DT_SINGLELINE;

	if (m_bSingleMonth)
	{
		dtFlags |= DT_CENTER;
	}
	else
	{
		dtFlags |= DT_RIGHT;
	}

	CBrush brSelected (m_Colors.clrSelected);

	CRect rectClip;
	pDC->GetClipBox (rectClip);

	int i = 0;

	const COleDateTime date = GetMonthDate (nMonthIndex);

	if (date < m_MinDate || date > m_MaxDate)
	{
		return;
	}

	const int nDaysInMonth = GetMaxMonthDay (date);
	const COleDateTime today = COleDateTime::GetCurrentTime ();

	const BOOL bDrawNextMonth = (nMonthIndex == m_nMonths - 1);

	//---------------------------------------
	// Draw month + year + prev/next buttons:
	//---------------------------------------
	CRect rectCaption = rect;
	rectCaption.bottom = rectCaption.top + m_sizeBox.cy + 2;

	if (!m_bSingleMonth)
	{
		rectCaption.DeflateRect (1, 0);
		rectCaption.top++;
	}
	else
	{
		CRect rectClient;
		GetClientRect (rectClient);

		rectCaption.left = rectClient.left + m_nHorzMargin;
		rectCaption.right = rectClient.right - m_nHorzMargin;
		rectCaption.top += m_nVertMargin;
	}

	if (m_bGradientFillCaption)
	{
		COLORREF clrLight;
		COLORREF clrDark;

		if (GetRValue (m_Colors.clrCaption) > 192 &&
			GetGValue (m_Colors.clrCaption) > 192 &&
			GetBValue (m_Colors.clrCaption) > 192)
		{
			clrLight = m_Colors.clrCaption;
			clrDark = CBCGPDrawManager::PixelAlpha (clrLight, 80);
		}
		else
		{
			clrDark = m_Colors.clrCaption;
			clrLight = CBCGPDrawManager::PixelAlpha (clrDark, 120);
		}

		CBCGPDrawManager dm (*pDC);
		dm.FillGradient (rectCaption, clrDark, clrLight, TRUE);
	}
	else
	{
		CBrush br (m_Colors.clrCaption);
		pDC->FillRect (rectCaption, &br);
	}

	rectCaption.right -= 2;

	COLORREF cltTextOld = pDC->SetTextColor (m_Colors.clrCaptionText);

	{
		//CBCGPDefaultLocale dl(m_bDontChangeLocale);

		CString strYear;
		strYear.Format(_T("%04d"), date.GetYear());

		CString strYearMonth = date.Format(_T("%B")) + _T(" ") + strYear;

		pDC->DrawText (strYearMonth, rectCaption, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}

	rect.DeflateRect (m_nDaysHorzMarginLeft, 0, m_nDaysHorzMarginRight, 0);
	rect.top = rectCaption.bottom;

	pDC->SetTextColor (m_Colors.clrInactiveText == (COLORREF)-1 ? globalData.clrGrayedText : m_Colors.clrInactiveText);

	//----------------
	// Draw week days:
	//----------------
	m_arWeekDays.RemoveAll();
	m_arWeekDays.SetSize (DAYS_IN_WEEK);
	for(int nWeek=0;nWeek<DAYS_IN_WEEK;nWeek++)
	{
		CString strWeekDay;
		if(nWeek==0)
			strWeekDay=_T("日");
		else if(nWeek==1)
			strWeekDay=_T("一");
		else if(nWeek==2)
			strWeekDay=_T("二");
		else if(nWeek==3)
			strWeekDay=_T("三");
		else if(nWeek==4)
			strWeekDay=_T("四");
		else if(nWeek==5)
			strWeekDay=_T("五");
		else if(nWeek==6)
			strWeekDay=_T("六");
		m_arWeekDays.SetAt (nWeek, strWeekDay);
	}
 	CRect rectWeeks = rect;
 	rectWeeks.bottom = rectWeeks.top + m_sizeBox.cy + 1;
 
 	for (i = 0; i < DAYS_IN_WEEK; i++)
 	{
 		CRect rectWeekDay = rectWeeks; 
 		rectWeekDay.left = rectWeeks.left + m_sizeBox.cx * i - 1;
 		rectWeekDay.right = rectWeekDay.left + m_sizeBox.cx;
 
 		pDC->DrawText (m_arWeekDays [i], rectWeekDay, dtFlags);
 	}

	CPen pen (PS_SOLID, 1, m_Colors.clrLine == (COLORREF)-1 ? globalData.clrBtnShadow : m_Colors.clrLine);
	CPen* pOldPen = pDC->SelectObject (&pen);

	int yLine = rectWeeks.bottom - 1;
	const int xLineLeft = rect.left;
	const int xLineRight = rect.right + 2;

	pDC->MoveTo (xLineLeft, yLine);
	pDC->LineTo (xLineRight, yLine);

	rect.top = rectWeeks.bottom + 2;

	BOOL bPrevMarked = FALSE;
	CFont* pOldFont = NULL;

	CRect rectInter;

	CRect rectWeekNums;
	rectWeekNums.SetRectEmpty ();

	//-------------------
	// Draw week numbers:
	//-------------------
	if (m_bWeekNumbers)
	{
		rectWeekNums = rect;
		rectWeekNums.right = rect.left;
		rectWeekNums.left = rectWeekNums.right - m_sizeBox.cx;
	}

	//-----------
	// Draw days:
	//-----------
	CRect rects [42];
	int xStart = 0;
	int nDaysInPrevMonth = 0;

	GetDayRects (nMonthIndex, rects, xStart, nDaysInPrevMonth);

	int nDayOffset = 1 - xStart;
	int yLineBottom = -1;
	int nWeeks = 0;

	COleDateTime date1;
	COleDateTime date2;
	if (m_dateStartDrag != COleDateTime ())
	{
		date1 = min (m_dateTrack, m_dateStartDrag);
		date2 = max (m_dateTrack, m_dateStartDrag);

		BOOL bSelectTruncate = m_bSelectWeekMode ||
			(m_bTruncateSelection && (date2 - date1) > COleDateTimeSpan(6, 0, 0, 0));

		if (bSelectTruncate)
		{
			date1 = GetFirstWeekDay (date1);
			date2 = GetFirstWeekDay (date2) + COleDateTimeSpan (6, 0, 0, 0);
		}
	}

	for (i = 0; i < 42; i++)
	{
		CRect rectDay = rects [i];
		if (rectDay.IsRectEmpty ())
		{
			continue;
		}

		if (!rectInter.IntersectRect (rectDay, rectClip))
		{
			continue;
		}

		pDC->SetTextColor (m_Colors.clrText == (COLORREF)-1 ? globalData.clrWindowText : m_Colors.clrText);

		int nDay = nDayOffset + i;
		BOOL bIsCurrMonth = TRUE;

		if (nDay < 1)
		{
			bIsCurrMonth = FALSE;
			nDay = nDaysInPrevMonth + nDay;
		}
		else if (nDay > nDaysInMonth)
		{
			if (!bDrawNextMonth)
			{
				break;
			}

			nDay -= nDaysInMonth;
			bIsCurrMonth = FALSE;
		}

		BOOL bIsSelected = FALSE;

		if (bIsCurrMonth)
		{
			COleDateTime dateCurr (date.GetYear (), date.GetMonth (), nDay, 0, 0, 0);

			bIsSelected = IsDateSelected (dateCurr);

			if (m_dateStartDrag != COleDateTime ())
			{
				if (date1 <= dateCurr && dateCurr <= date2)
				{
					if (bCtrl && bIsSelected)
					{
						bIsSelected = FALSE;
					}
					else
					{
						bIsSelected = TRUE;
					}
				}
			}

			COLORREF clrDay = GetDateColor (dateCurr);
			if (clrDay != (COLORREF)-1)
			{
				pDC->SetTextColor (clrDay);
			}
		}

		if (!bIsCurrMonth && !bIsSelected)
		{
			pDC->SetTextColor (m_Colors.clrInactiveText == (COLORREF)-1 ? globalData.clrGrayedText : m_Colors.clrInactiveText);
		}

		CRect rectText = rectDay;

		if (!m_bSingleMonth)
		{
			rectText.OffsetRect (-1, 0);
		}

		if (bIsSelected)
		{
			pDC->FillRect (rectDay, &brSelected);
			pDC->SetTextColor (m_Colors.clrSelectedText);
		}

		CString strDay;
		strDay.Format (_T("%d"), nDay);

		BOOL bIsMarked = 
			bIsCurrMonth && 
			IsDateMarked (date.GetYear (), date.GetMonth (), nDay);

		if (bIsMarked)
		{
			if (!bPrevMarked)
			{
				pOldFont = pDC->SelectObject (&globalData.fontBold);
			}
		}
		else
		{
			if (bPrevMarked)
			{
				ASSERT_VALID (pOldFont);
				pDC->SelectObject (pOldFont);
				pOldFont = NULL;
			}
		}

		pDC->DrawText (strDay, rectText, dtFlags);

		bPrevMarked = bIsMarked;

		if (bIsCurrMonth && date.GetYear () == today.GetYear () &&
			date.GetMonth () == today.GetMonth () &&
			nDay == today.GetDay ())
		{
			pDC->Draw3dRect (rectDay, m_Colors.clrTodayBorder, m_Colors.clrTodayBorder);
		}

		if (rectDay.bottom > yLineBottom)
		{
			nWeeks++;
		}

		yLineBottom = rectDay.bottom;
	}

	if (pOldFont != NULL)
	{
		pDC->SelectObject (pOldFont);
	}

	//-------------------
	// Draw week numbers:
	//-------------------
	if (m_bWeekNumbers)
	{
		pOldFont = pDC->SelectObject (&globalData.fontSmall);

		pDC->MoveTo (xLineLeft - 1, yLine);

		pDC->SetTextColor (m_Colors.clrInactiveText == (COLORREF)-1 ? globalData.clrGrayedText : m_Colors.clrInactiveText);

		for (i = 0; i < nWeeks; i++)
		{
			CRect rectNum = rectWeekNums;
			rectNum.top += m_sizeBox.cy * i;
			rectNum.bottom = rectNum.top + m_sizeBox.cy;

			int nWeekNum = m_arStartWeekInMonth [nMonthIndex] + i;
			if (nWeekNum == 54)
			{
				nWeekNum = 1;
			}

			CString strNum;
			strNum.Format (_T("%d"), nWeekNum);

			if (!m_bSingleMonth)
			{
				rectNum.right -= 4;
			}

			pDC->DrawText (strNum, rectNum, dtFlags);

			if (!m_bSingleMonth)
			{
				pDC->LineTo (xLineLeft - 1, rectNum.bottom);
			}
		}

		pDC->SelectObject (pOldFont);
	}

	if (m_bTodayButton && yLineBottom >= 0 && 
		(m_bSingleMonth || GetStyle () & WS_POPUP))
	{
		pDC->MoveTo (xLineLeft, yLineBottom);
		pDC->LineTo (xLineRight, yLineBottom);
	}

	pDC->SelectObject (pOldPen);
	pDC->SetTextColor(cltTextOld);
}

