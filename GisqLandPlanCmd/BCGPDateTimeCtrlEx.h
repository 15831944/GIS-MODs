/***************************************************************
�ļ�����CBCGPDateTimeCtrlEx
�����ߣ�liyuchun
��д���ڣ�2016-05-06
��Ҫ���������ڿؼ���д
�޸ļ�¼��who-when-why����
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