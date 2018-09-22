/***************************************************************
文件名：CBCGPCalendarEx
创建者：liyuchun
编写日期：2016-05-06
简要描述：日历控件改写（大小控制和today改为今天）
修改记录（who-when-why）：
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