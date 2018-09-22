// LPSystemSetCmd.h : CLPSystemSetCmd ������

#pragma once
#include "resource.h"       // ������

#include "GisqLandPlanCmd_i.h"

#include "BaseCommand.h"
#include "LPSystemSetDlg.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CLPSystemSetCmd

class ATL_NO_VTABLE CLPSystemSetCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPSystemSetCmd, &CLSID_LPSystemSetCmd>,
	public IDispatchImpl<ILPSystemSetCmd, &IID_ILPSystemSetCmd, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CLPSystemSetCmd():CBaseCommand(0,_T("ϵͳ����"),NULL,_T("ϵͳ����"),_T("ϵͳ����")
		,_T("ϵͳ����"),_T("ϵͳ����"))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LPSYSTEMSETCMD)


BEGIN_COM_MAP(CLPSystemSetCmd)
	COM_INTERFACE_ENTRY(ILPSystemSetCmd)
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
	STDMETHOD(OnClick)();
	CLPSystemSetDlg *m_pSystemSetDlg;
};

OBJECT_ENTRY_AUTO(__uuidof(LPSystemSetCmd), CLPSystemSetCmd)
