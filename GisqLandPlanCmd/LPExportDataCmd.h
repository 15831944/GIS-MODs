// LPExportDataCmd.h : CLPExportDataCmd ������

#pragma once
#include "resource.h"       // ������

#include "GisqLandPlanCmd_i.h"

#include "BaseCommand.h"
#include "LPExportDataDlg.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CLPExportDataCmd

class ATL_NO_VTABLE CLPExportDataCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPExportDataCmd, &CLSID_LPExportDataCmd>,
	public IDispatchImpl<ILPExportDataCmd, &IID_ILPExportDataCmd, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CLPExportDataCmd():CBaseCommand(0,_T("���ݷַ�"),NULL,_T("���ݷַ�"),_T("���ݷַ�")
		,_T("���ݷַ�"),_T("���ݷַ�"))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LPEXPORTDATACMD)


BEGIN_COM_MAP(CLPExportDataCmd)
	COM_INTERFACE_ENTRY(ILPExportDataCmd)
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
	CLPExportDataDlg *m_pExpDataDlg;

};

OBJECT_ENTRY_AUTO(__uuidof(LPExportDataCmd), CLPExportDataCmd)
