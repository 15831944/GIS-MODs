// GisqImpCoordCmd.h : GisqImpCoordCmd DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "GisqImpCoordCmd_i.h"


// CGisqImpCoordCmdApp
// �йش���ʵ�ֵ���Ϣ������� GisqImpCoordCmd.cpp
//

class CGisqImpCoordCmdApp : public CWinApp
{
public:
	CGisqImpCoordCmdApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
