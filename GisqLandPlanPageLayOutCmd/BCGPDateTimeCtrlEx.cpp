#include "stdafx.h"

#include "BCGPDateTimeCtrlEx.h"


static const int iSpinID = 1;
 CBCGPDateTimeCtrlEx::CBCGPDateTimeCtrlEx(void)
 {
	 m_bAutoResize = FALSE;
	// m_bEnabledInCustomizeMode = TRUE;
	// m_pPopup = NULL;
 }
 
 CBCGPDateTimeCtrlEx::~CBCGPDateTimeCtrlEx(void)
 {
 }

BEGIN_MESSAGE_MAP(CBCGPDateTimeCtrlEx,CBCGPDateTimeCtrl)
	ON_WM_SIZE()
	ON_WM_CREATE()
//	ON_WM_DESTROY()
//	ON_WM_MOUSEHWHEEL()
//	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CBCGPDateTimeCtrlEx::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDateTimeCtrl::OnSize(nType,cx,cy);
}

 void CBCGPDateTimeCtrlEx::OnShowCalendarPopup()
 {
 	if (!m_dropCalendar)
 		return;
 
 	if (m_pPopup != NULL)
 	{
 		ClosePopupCalendar ();
 		return;
 	}
 
 	m_bDropButtonIsPressed = TRUE;
 
 	CRect rectRedraw = m_rectDropButton;
 	rectRedraw.InflateRect (2, 2);
 
 	RedrawWindow (rectRedraw);
 
 	CRect rectWindow;
 	GetWindowRect (rectWindow);
 
 	CRect rect;
 	GetClientRect(rect);
 
 	m_pPopup = new CBCGPCalendarEx();
 	m_pPopup->SetDontChangeLocale(FALSE);
 	m_pPopup->SetDate (m_Date);
 	m_pPopup->SetFirstDayOfWeek (m_weekStart - 1);
 	m_pPopup->EnableTodayButton ();
 
 	m_pPopup->EnableMutipleSelection(FALSE);
 	m_pPopup->EnableWeekNumbers(FALSE);
 	m_pPopup->EnableVisualManagerStyle(m_bVisualManagerStyle);
	//m_pPopup->EnableGradientFillCaption(FALSE);
 	
 	m_pPopup->SetMinDate(m_MinDate);
 	m_pPopup->SetMaxDate(m_MaxDate);
 
  	CRect rectCalendar (CPoint (rectWindow.left-100,rectWindow.bottom),
  		CSize (0, 0));
 
 	BOOL bIsCreated = m_pPopup->Create (
 		WS_POPUP | WS_VISIBLE, rectCalendar, this, 0);
 
 	if (!bIsCreated)
 	{
 		ASSERT (FALSE);
 		m_pPopup = NULL;
 
 		TRACE(_T ("Calendar menu can't be used in the customization mode. You need to set CBCGPDateTimeCtrl::m_bEnabledInCustomizeMode\n"));
 	}
 }

int CBCGPDateTimeCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectSpin (0, 0, 1, 1);

	if (!m_wndSpin.Create (WS_CHILD | WS_VISIBLE | UDS_ALIGNRIGHT | UDS_AUTOBUDDY,
		rectSpin, this, iSpinID))
	{
		TRACE (_T ("CBCGPDateTimeCtrl: Can't create spin button!\n"));
		return -1;
	}

	m_wndSpin.m_bIsDateTimeControl = TRUE;

	CRect rectClient;
	GetClientRect (rectClient);

	AdjustControl (rectClient);

	RedrawWindow ();

	return 0;
}


