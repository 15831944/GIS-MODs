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


#include <map>
using namespace std;

#define BCGP_EXCLUDE_PNG_SUPPORT    //�����д�����BCG���벻��
#include <BCGCBProInc.h>			// BCGPro Control Bar

//���� Esri�Ŀ��ļ�
#include "..\include\IncludeEsriHeader.h"
//��ܲ��
#import "..\bin\GisqFrameworkCOM.ocx"  raw_native_types,raw_interfaces_only, no_namespace, named_guids, rename("GetMessage","GisqGetMessage"),rename("AppendMenu","GisqAppendMenu")
#import "..\bin\GisqFrameworkCOM2.ocx" raw_dispinterfaces, rename_namespace("GisqLib"), rename("GetMessage","GisqGetMessage"),rename("AppendMenu","GisqAppendMenu")
#import "..\bin\GisqSolutionManage.dll" no_namespace, rename("IRow","esriIRow")
#import "..\bin\GisqMapTOC.dll" raw_interfaces_only,no_namespace, named_guids
#import "..\bin\GisqAddData.dll" raw_interfaces_only,no_namespace, named_guids
#import "..\bin\GisqGraphicsBaseOperate.dll" raw_interfaces_only,no_namespace, named_guids

#import "..\bin\GisqTrackDrawOperate.dll" raw_interfaces_only,no_namespace, named_guids
#import "..\bin\GisqTrackDraw.dll" raw_interfaces_only,no_namespace, named_guids

#import "..\bin\GisqEditorOperate.dll" raw_interfaces_only,no_namespace, named_guids
#import "..\bin\GisqEditor.dll" raw_interfaces_only,no_namespace, named_guids
//ADO��� add by lcc
#import "..\bin\msado15.dll" raw_interfaces_only,no_namespace, named_guids, rename("EOF","adoEOF"), rename("Field","ADOField"), rename("Fields","ADOFields"), rename("Record","ADORecord"), rename("Parameter","ADOParameter")
//ADO���ӵĶ˿ں���ʱ��������
#define nPort _T("1521")
//�����ļ�λ��
#define CONFIGPATH _T("\\Config\\Rule.xml")
#define CONNECT_CONFIGPATH _T("\\Config\\config.xml")