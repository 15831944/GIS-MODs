// LPGHGXCXCmd.h : CLPGHGXCXCmd ������

#pragma once
#include "resource.h"       // ������

#include "GisqLandPlanCmd_i.h"
#include "BaseCommand.h"
#include "LPGHGXCXDlg.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CLPGHGXCXCmd

class ATL_NO_VTABLE CLPGHGXCXCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPGHGXCXCmd, &CLSID_LPGHGXCXCmd>,
	public IDispatchImpl<ILPGHGXCXCmd, &IID_ILPGHGXCXCmd, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CLPGHGXCXCmd():CBaseCommand(0,_T("�滮�޸Ĳ�ѯ"),NULL,_T("�滮�޸Ĳ�ѯ"),_T("�滮�޸Ĳ�ѯ")
		,_T("�滮�޸Ĳ�ѯ"),_T("�滮�޸Ĳ�ѯ"))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LPGHGXCXCMD)


BEGIN_COM_MAP(CLPGHGXCXCmd)
	COM_INTERFACE_ENTRY(ILPGHGXCXCmd)
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
	CLPGHGXCXDlg *LPGHGXCXDlg;

};

OBJECT_ENTRY_AUTO(__uuidof(LPGHGXCXCmd), CLPGHGXCXCmd)
