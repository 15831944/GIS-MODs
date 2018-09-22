/*********************************************
文件名:  LPGHDLTBCXCmd
创建者:  luchencheng
编写日期:2016-10-09
简要描述:规划地类图斑查询功能接口
修改记录(who-when-why):
********************************************/
// LPGHDLTBCXCmd.h : CLPGHDLTBCXCmd 的声明

#pragma once
#include "resource.h"       // 主符号

#include "GisqLandPlanCmd_i.h"

#include "BaseCommand.h"
#include "..\GisqLandPlanPlatform\FrameSink.h"
#include "LPGHDLTBCXDlg.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif



// CLPGHDLTBCXCmd

class ATL_NO_VTABLE CLPGHDLTBCXCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPGHDLTBCXCmd, &CLSID_LPGHDLTBCXCmd>,
	public IDispatchImpl<ILPGHDLTBCXCmd, &IID_ILPGHDLTBCXCmd, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CLPGHDLTBCXCmd():CBaseCommand(0,_T("规划地类图斑查询"),NULL,_T("规划地类图斑查询"),_T("规划地类图斑查询")
		,_T("规划地类图斑查询"),_T("规划地类图斑查询"))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LPGHDLTBCXCMD)


BEGIN_COM_MAP(CLPGHDLTBCXCmd)
	COM_INTERFACE_ENTRY(ILPGHDLTBCXCmd)
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
	STDMETHOD (OnClick)( );
	CLPGHDLTBCXDlg *LPGHDLTBCXdlg;
};

OBJECT_ENTRY_AUTO(__uuidof(LPGHDLTBCXCmd), CLPGHDLTBCXCmd)
