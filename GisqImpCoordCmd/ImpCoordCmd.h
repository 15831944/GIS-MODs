// ImpCoordCmd.h : CImpCoordCmd ������

#pragma once
#include "resource.h"       // ������

#include "GisqImpCoordCmd_i.h"
#include "BaseCommand.h"
#include "ImpCoordDlg.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CImpCoordCmd

class ATL_NO_VTABLE CImpCoordCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CImpCoordCmd, &CLSID_ImpCoordCmd>,
	public IDispatchImpl<IImpCoordCmd, &IID_IImpCoordCmd, &LIBID_GisqImpCoordCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CImpCoordCmd():CBaseCommand(0,_T("��ַ�㵼��"),NULL,_T("��ַ�㵼��"),_T("��ַ�㵼��")
		,_T("��ַ�㵼��"),_T("��ַ�㵼��"))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_IMPCOORDCMD)


BEGIN_COM_MAP(CImpCoordCmd)
	COM_INTERFACE_ENTRY(IImpCoordCmd)
	COM_INTERFACE_ENTRY2(IDispatch,IImpCoordCmd)
	COM_INTERFACE_ENTRY(IGSCommandEx)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
// 		HWND hwndExpDlg = NULL;
// 		hwndExpDlg = FindWindow(NULL,_T("��ַ�㵼��"));
// 		if (hwndExpDlg != NULL)
// 		{
// 			CWnd *ImpDlg;
// 			ImpDlg = CWnd::FromHandle(hwndExpDlg);
// 			//ImpDlg->SendMessage(WM_DESTROY);
// 			ImportDlg = (CImpCoordDlg *)ImpDlg;
// 			delete ImportDlg;
// 		}
	}

public:
	/*static */CImpCoordDlg *ImportDlg;
	STDMETHOD (OnClick)( );
};

OBJECT_ENTRY_AUTO(__uuidof(ImpCoordCmd), CImpCoordCmd)
