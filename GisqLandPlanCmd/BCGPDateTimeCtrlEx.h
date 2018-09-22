/***************************************************************
文件名：CBCGPDateTimeCtrlEx
创建者：liyuchun
编写日期：2016-05-06
简要描述：日期控件改写
修改记录（who-when-why）：
****************************************************************/

#ifndef BCGPDATETIMECTRLEX_H_
#define BCGPDATETIMECTRLEX_H_

#include <BCGCBProInc.h>
#include "BCGPCalendarEx.h"

class CBCGPDateTimeCtrlEx : public CBCGPDateTimeCtrl
{
	
public:
	CBCGPDateTimeCtrlEx(void);
	~CBCGPDateTimeCtrlEx(void);
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);


	virtual void OnShowCalendarPopup ();


};

#endif