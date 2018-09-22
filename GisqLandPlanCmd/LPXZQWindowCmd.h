// CLPXZQWindowCmd.h : CLPXZQWindowCmd 的声明

#pragma once
#include "resource.h"       // 主符号

#include "GisqLandPlanCmd_i.h"
#include "_ICLPXZQWindowCmdEvents_CP.h"
#include "LPXZQWindowDlg.h"
#include "..\include\GlobalDefine.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif



// CLPXZQWindowCmd

class ATL_NO_VTABLE CLPXZQWindowCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPXZQWindowCmd, &CLSID_CLPXZQWindowCmd>,
	public IConnectionPointContainerImpl<CLPXZQWindowCmd>,
	public CProxy_ICLPXZQWindowCmdEvents<CLPXZQWindowCmd>,
	public IDispatchImpl<ICLPXZQWindowCmd, &IID_ICLPXZQWindowCmd, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IGSDockableWindowTabDef, &__uuidof(IGSDockableWindowTabDef), &LIBID_GISQFRAMEWORKCOMLib, /* wMajor = */ 1>
{
public:
	CLPXZQWindowCmd()
	{
		m_pFrameworkCom = NULL;
		m_pXZQDiolog = NULL;
		m_hWnd = NULL;
		m_strName = _T("行政区索引");
		m_strCaption = _T("行政区索引");
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_CLPXZQWINDOWCMD)


	BEGIN_COM_MAP(CLPXZQWindowCmd)
		COM_INTERFACE_ENTRY(ICLPXZQWindowCmd)
		COM_INTERFACE_ENTRY2(IDispatch, IGSDockableWindowTabDef)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY(IGSDockableWindowTabDef)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CLPXZQWindowCmd)
		CONNECTION_POINT_ENTRY(__uuidof(_ICLPXZQWindowCmdEvents))
	END_CONNECTION_POINT_MAP()


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
	CLPXZQWindowDlg *m_pXZQDiolog;
	CString m_strCaption;
	CString m_strName;
	HWND *m_hWnd;
	//add by lcc 20160919
	STDMETHOD(GetSelectedRegion)(BSTR* strName, BSTR* strCode);
	STDMETHOD(ZoomToTopXZQ)(void);
};

OBJECT_ENTRY_AUTO(__uuidof(CLPXZQWindowCmd), CLPXZQWindowCmd)
