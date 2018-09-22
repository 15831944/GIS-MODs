// CLPXZQWindowCmd.h : CLPXZQWindowCmd ������

#pragma once
#include "resource.h"       // ������

#include "GisqLandPlanCmd_i.h"
#include "_ICLPXZQWindowCmdEvents_CP.h"
#include "LPXZQWindowDlg.h"
#include "..\include\GlobalDefine.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
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
		m_strName = _T("����������");
		m_strCaption = _T("����������");
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
