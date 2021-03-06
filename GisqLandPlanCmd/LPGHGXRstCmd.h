// LPGHGXRstCmd.h : CLPGHGXRstCmd 的声明

#pragma once
#include "resource.h"       // 主符号

#include "GisqLandPlanCmd_i.h"
#include "LPGHGXCXRstDlg.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif



// CLPGHGXRstCmd

class ATL_NO_VTABLE CLPGHGXRstCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPGHGXRstCmd, &CLSID_LPGHGXRstCmd>,
	public IDispatchImpl<ILPGHGXRstCmd, &IID_ILPGHGXRstCmd, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IGSDockableWindowTabDef, &__uuidof(IGSDockableWindowTabDef), &LIBID_GISQFRAMEWORKCOMLib, /* wMajor = */ 1>
{
public:
	CLPGHGXRstCmd()
	{
		m_pFrameworkCom = NULL;
		m_pGHGXCXRstDlg = NULL;
		m_hWnd = NULL;
		m_strName = _T("规划修改项目查询结果");
		m_strCaption = _T("规划修改项目查询结果");
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_LPGHGXRSTCMD)


	BEGIN_COM_MAP(CLPGHGXRstCmd)
		COM_INTERFACE_ENTRY(ILPGHGXRstCmd)
		COM_INTERFACE_ENTRY2(IDispatch, IGSDockableWindowTabDef)
		COM_INTERFACE_ENTRY(IGSDockableWindowTabDef)
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


	// IGSDockableWindowTabDef Methods
public:
	STDMETHOD(get_Caption)(BSTR * strCaption);
	STDMETHOD(get_ChildHWnd)(OLE_HANDLE * hWnd);
	STDMETHOD(get_Name)(BSTR * strName);
	STDMETHOD(OnCreate)(LPDISPATCH lpFrameworkCOM);
	STDMETHOD(OnDestroy)();
	_DGisqFrameworkCOMPtr	m_pFrameworkCom;
	CLPGHGXCXRstDlg *m_pGHGXCXRstDlg;
	CString m_strCaption;
	CString m_strName;
	HWND *m_hWnd;
	STDMETHOD(SearchFA)(BSTR bstrQueryFilter);
};

OBJECT_ENTRY_AUTO(__uuidof(LPGHGXRstCmd), CLPGHGXRstCmd)
