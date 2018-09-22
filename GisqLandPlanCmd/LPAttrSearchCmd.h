// LPAttrSearchCmd.h : CLPAttrSearchCmd ������

#pragma once
#include "resource.h"       // ������

#include "GisqLandPlanCmd_i.h"
#include "BaseCommand.h"
#include "LPAttrSearchDlg.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CLPAttrSearchCmd

class ATL_NO_VTABLE CLPAttrSearchCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPAttrSearchCmd, &CLSID_LPAttrSearchCmd>,
	public IDispatchImpl<ILPAttrSearchCmd, &IID_ILPAttrSearchCmd, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CLPAttrSearchCmd():CBaseCommand(0,_T("���Բ�ѯ"),NULL,_T("���Բ�ѯ"),_T("���Բ�ѯ")
		,_T("���Բ�ѯ"),_T("���Բ�ѯ"))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LPATTRSEARCHCMD)


BEGIN_COM_MAP(CLPAttrSearchCmd)
	COM_INTERFACE_ENTRY(ILPAttrSearchCmd)
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
// 		HWND hwndImpDlg = NULL;
// 		hwndImpDlg = FindWindow(NULL,_T("�滮Ҫ�ز�ѯ"));
// 		if (hwndImpDlg != NULL)
// 		{
// 			CWnd *CXDlg;
// 			CXDlg = CWnd::FromHandle(hwndImpDlg);
// 			CXDlg->SendMessage(WM_DESTROY);
// 		}
	}

public:
	STDMETHOD(OnClick)();
private:
	CLPAttrSearchDlg *m_AttrSearchDlg;
};

OBJECT_ENTRY_AUTO(__uuidof(LPAttrSearchCmd), CLPAttrSearchCmd)
