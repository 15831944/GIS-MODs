// GisqConvertXYDataToGeometry.h : GisqConvertXYDataToGeometry DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "GisqConvertXYDataToGeometry_i.h"


// CGisqConvertXYDataToGeometryApp
// �йش���ʵ�ֵ���Ϣ������� GisqConvertXYDataToGeometry.cpp
//

class CGisqConvertXYDataToGeometryApp : public CWinApp
{
public:
	CGisqConvertXYDataToGeometryApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
