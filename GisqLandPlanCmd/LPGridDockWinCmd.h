// LPGridDockWinCmd.h : CLPGridDockWinCmd ������

#pragma once
#include "resource.h"       // ������

#include "GisqLandPlanCmd_i.h"

#include "LPGridDockWinDlg.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CLPGridDockWinCmd

class ATL_NO_VTABLE CLPGridDockWinCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPGridDockWinCmd, &CLSID_LPGridDockWinCmd>,
	public IDispatchImpl<ILPGridDockWinCmd, &IID_ILPGridDockWinCmd, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IGSDockableWindowTabDef, &__uuidof(IGSDockableWindowTabDef), &LIBID_GISQFRAMEWORKCOMLib, /* wMajor = */ 1>
{
public:
	CLPGridDockWinCmd()
	{
		m_pFrameworkCom = NULL;
		m_pGridDiolog = NULL;
		m_hWnd = NULL;
		m_strName = _T("�������");
		m_strCaption = _T("�������");
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_LPGRIDDOCKWINCMD)


	BEGIN_COM_MAP(CLPGridDockWinCmd)
		COM_INTERFACE_ENTRY(ILPGridDockWinCmd)
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
	GisqLib::_DGisqFrameworkCOMPtr	m_pFrameworkCom;
	CLPGridDockWinDlg *m_pGridDiolog;
	CString m_strCaption;
	CString m_strName;
	HWND *m_hWnd;
	STDMETHOD(SetQueryFilter)(IUnknown* spQueryFilter, BSTR bstrFeatureClassName, BSTR bstrTabName);
	STDMETHOD(SetDlgCaption)(BSTR bstrCaption);
};

OBJECT_ENTRY_AUTO(__uuidof(LPGridDockWinCmd), CLPGridDockWinCmd)
