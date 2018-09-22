// LPGHGXRstCmd.h : CLPGHGXRstCmd ������

#pragma once
#include "resource.h"       // ������

#include "GisqLandPlanCmd_i.h"
#include "LPGHGXCXRstDlg.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
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
		m_strName = _T("�滮�޸���Ŀ��ѯ���");
		m_strCaption = _T("�滮�޸���Ŀ��ѯ���");
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
