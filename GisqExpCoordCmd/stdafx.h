// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#define _ATL_APARTMENT_THREADED 
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO 数据库类
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT



#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

#define BCGP_EXCLUDE_PNG_SUPPORT    //如果不写这个，BCG编译不过
#include <BCGCBProInc.h>			// BCGPro Control Bar
//引用 Esri的库文件
#include "..\include\IncludeEsriHeader.h"
//框架插件
#import "..\bin\GisqFrameworkCOM.ocx"  raw_native_types,raw_interfaces_only, no_namespace, named_guids, rename("GetMessage","GisqGetMessage"),rename("AppendMenu","GisqAppendMenu")
#import "..\bin\GisqFrameworkCOM2.ocx" raw_dispinterfaces, rename_namespace("GisqLib"), rename("GetMessage","GisqGetMessage"),rename("AppendMenu","GisqAppendMenu")

