// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

#include <afxwin.h>
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

using namespace ATL;


#include <map>
using namespace std;

#define BCGP_EXCLUDE_PNG_SUPPORT    //如果不写这个，BCG编译不过
#include <BCGCBProInc.h>			// BCGPro Control Bar

//引用 Esri的库文件
#include "..\include\IncludeEsriHeader.h"
//框架插件
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
//ADO组件 add by lcc
#import "..\bin\msado15.dll" raw_interfaces_only,no_namespace, named_guids, rename("EOF","adoEOF"), rename("Field","ADOField"), rename("Fields","ADOFields"), rename("Record","ADORecord"), rename("Parameter","ADOParameter")
//ADO连接的端口号暂时配在这里
#define nPort _T("1521")
//配置文件位置
#define CONFIGPATH _T("\\Config\\Rule.xml")
#define CONNECT_CONFIGPATH _T("\\Config\\config.xml")