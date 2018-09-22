// LPRealtimeUpdateCmd.h : CLPRealtimeUpdateCmd 的声明

#pragma once
#include "resource.h"       // 主符号

#include "GisqLandPlanCmd_i.h"
#include "BaseCommand.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif



// CLPRealtimeUpdateCmd

class ATL_NO_VTABLE CLPRealtimeUpdateCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPRealtimeUpdateCmd, &CLSID_LPRealtimeUpdateCmd>,
	public IDispatchImpl<ILPRealtimeUpdateCmd, &IID_ILPRealtimeUpdateCmd, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CLPRealtimeUpdateCmd():CBaseCommand(0,_T("实时更新"),NULL,_T("实时更新"),_T("实时更新")
		,_T("实时更新"),_T("实时更新"))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LPREALTIMEUPDATECMD)


BEGIN_COM_MAP(CLPRealtimeUpdateCmd)
	COM_INTERFACE_ENTRY(ILPRealtimeUpdateCmd)
	COM_INTERFACE_ENTRY2(IDispatch, IGSCommandEx)
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
};

OBJECT_ENTRY_AUTO(__uuidof(LPRealtimeUpdateCmd), CLPRealtimeUpdateCmd)
