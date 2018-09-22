// ExpCoordCmd.h : CExpCoordCmd ������

#pragma once
#include "resource.h"       // ������

#include "GisqExpCoordCmd_i.h"
#include "ExpCoordDlg.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif


#include "BaseCommand.h"
// CExpCoordCmd

class ATL_NO_VTABLE CExpCoordCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CExpCoordCmd, &CLSID_ExpCoordCmd>,
	public IDispatchImpl<IExpCoordCmd, &IID_IExpCoordCmd, &LIBID_GisqExpCoordCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CExpCoordCmd():CBaseCommand(0,_T("��ַ�㵼��"),NULL,_T("��ַ�㵼��"),_T("��ַ�㵼��")
		,_T("��ַ�㵼��"),_T("��ַ�㵼��"))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_EXPCOORDCMD)


BEGIN_COM_MAP(CExpCoordCmd)
	COM_INTERFACE_ENTRY(IExpCoordCmd)
	COM_INTERFACE_ENTRY2(IDispatch,IExpCoordCmd)
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
// 			CWnd *ExpDlg;
// 			ExpDlg = CWnd::FromHandle(hwndExpDlg);
// 			ExpDlg->SendMessage(WM_DESTROY);
// 		}
	}

public:
	CExpCoordDlg *ExportDlg;
	STDMETHOD (OnClick)( );
};

OBJECT_ENTRY_AUTO(__uuidof(ExpCoordCmd), CExpCoordCmd)
