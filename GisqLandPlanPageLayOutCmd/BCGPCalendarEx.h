/***************************************************************
�ļ�����CBCGPCalendarEx
�����ߣ�liyuchun
��д���ڣ�2016-05-06
��Ҫ�����������ؼ���д����С���ƺ�today��Ϊ���죩
�޸ļ�¼��who-when-why����
****************************************************************/


#ifndef BCGPCALENDAREX_H_
#define BCGPCALENDAREX_H_


#include <BCGCBProInc.h>

class CBCGPCalendarEx :  public CBCGPCalendar
{
	
public:
	CBCGPCalendarEx(void);
	~CBCGPCalendarEx(void);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	virtual void OnDrawMonth (CDC* pDC, CRect rect, int nMonthIndex);
};

#endif