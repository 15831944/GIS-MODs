// GisqExpCoordCmd.h : GisqExpCoordCmd DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "GisqExpCoordCmd_i.h"


// CGisqExpCoordCmdApp
// �йش���ʵ�ֵ���Ϣ������� GisqExpCoordCmd.cpp
//

class CGisqExpCoordCmdApp : public CWinApp
{
public:
	CGisqExpCoordCmdApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
