// LPZXCQ_JBNTBHCmd.h : CLPZXCQ_JBNTBHCmd 的声明

#pragma once
#include "resource.h"       // 主符号

#include "GisqLandPlanPageLayOutCmd_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

#include "BaseCommand.h"
#include "ExportThemeMap.h"
#include "..\include\Global.h"

// CLPZXCQ_JBNTBHCmd

class ATL_NO_VTABLE CLPZXCQ_JBNTBHCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPZXCQ_JBNTBHCmd, &CLSID_LPZXCQ_JBNTBHCmd>,
	public IDispatchImpl<ILPZXCQ_JBNTBHCmd, &IID_ILPZXCQ_JBNTBHCmd, &LIBID_GisqLandPlanPageLayOutCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CLPZXCQ_JBNTBHCmd():CBaseCommand(0,_T("基本农田保护规划图"),_T("CLPZXCQ_JBNTBHCmd"),_T("基本农田保护规划图"),_T("基本农田保护规划图")
		,_T("基本农田保护规划图"),_T("基本农田保护规划图"))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LPZXCQ_JBNTBHCMD)


BEGIN_COM_MAP(CLPZXCQ_JBNTBHCmd)
	COM_INTERFACE_ENTRY(ILPZXCQ_JBNTBHCmd)
	//COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY2(IDispatch,ILPZXCQ_JBNTBHCmd)
	COM_INTERFACE_ENTRY(IGSCommandEx)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(OnClick)();
	//edit by lcc 2016.11.23 将行政区获取方式改为设置窗口获取
// 	STDMETHOD(get_Enabled)(VARIANT_BOOL * bEnabled );
// 	CComBSTR m_bstrXZQName;
// 	CComBSTR m_bstrXZQCode;
};

OBJECT_ENTRY_AUTO(__uuidof(LPZXCQ_JBNTBHCmd), CLPZXCQ_JBNTBHCmd)
