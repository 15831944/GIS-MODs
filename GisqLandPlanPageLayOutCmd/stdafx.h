// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

#include <afxwin.h>
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdisp.h>        // MFC �Զ�����
#endif // _AFX_NO_OLE_SUPPORT

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

using namespace ATL;


//���� Esri�Ŀ��ļ�
#include "..\include\IncludeEsriHeader.h"
//��ܲ��
#import "..\bin\GisqFrameworkCOM.ocx"  raw_native_types,raw_interfaces_only, no_namespace, named_guids, rename("GetMessage","GisqGetMessage"),rename("AppendMenu","GisqAppendMenu")
#import "..\bin\GisqFrameworkCOM2.ocx" raw_dispinterfaces, rename_namespace("GisqLib"), rename("GetMessage","GisqGetMessage"),rename("AppendMenu","GisqAppendMenu")

#pragma comment (lib,"..\\lib\\tinyxml.lib")

#import "..\bin\GisqLandPlanCmd.dll" raw_interfaces_only,no_namespace,named_guids
//add by lcc 2016-11-19 ����֧�ֳ�ͼ����
#include <BCGCBProInc.h>
#include <bcgptargetver.h>

//ѡַ��ѯ��� add by lcc 2016.12.26
#import "..\bin\GisqLandPlanAnslysisCOM.dll" raw_interfaces_only,no_namespace, named_guids
//��ͼ�����ļ�·��
#define CONFIGPATH _T("\\Config\\Config.xml")
#define RULEPATH _T("\\Config\\Rule.xml")