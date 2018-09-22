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


//引用 Esri的库文件
#include "..\include\IncludeEsriHeader.h"
//框架插件
#import "..\bin\GisqFrameworkCOM.ocx"  raw_native_types,raw_interfaces_only, no_namespace, named_guids, rename("GetMessage","GisqGetMessage"),rename("AppendMenu","GisqAppendMenu")
#import "..\bin\GisqFrameworkCOM2.ocx" raw_dispinterfaces, rename_namespace("GisqLib"), rename("GetMessage","GisqGetMessage"),rename("AppendMenu","GisqAppendMenu")

#pragma comment (lib,"..\\lib\\tinyxml.lib")

#import "..\bin\GisqLandPlanCmd.dll" raw_interfaces_only,no_namespace,named_guids
//add by lcc 2016-11-19 用于支持出图设置
#include <BCGCBProInc.h>
#include <bcgptargetver.h>

//选址查询组件 add by lcc 2016.12.26
#import "..\bin\GisqLandPlanAnslysisCOM.dll" raw_interfaces_only,no_namespace, named_guids
//出图配置文件路径
#define CONFIGPATH _T("\\Config\\Config.xml")
#define RULEPATH _T("\\Config\\Rule.xml")