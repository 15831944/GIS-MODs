// LPJSYDGZQCXCmd.h : CLPJSYDGZQCXCmd ������

#pragma once
#include "resource.h"       // ������

#include "GisqLandPlanCmd_i.h"

#include "BaseCommand.h"
#include "..\GisqLandPlanPlatform\FrameSink.h"
#include "LPJSYDGZQCXDlg.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CLPJSYDGZQCXCmd

class ATL_NO_VTABLE CLPJSYDGZQCXCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPJSYDGZQCXCmd, &CLSID_LPJSYDGZQCXCmd>,
	public IDispatchImpl<ILPJSYDGZQCXCmd, &IID_ILPJSYDGZQCXCmd, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CLPJSYDGZQCXCmd():CBaseCommand(0,_T("�����õع�������ѯ"),NULL,_T("�����õع�������ѯ"),_T("�����õع�������ѯ")
		,_T("�����õع�������ѯ"),_T("�����õع�������ѯ"))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LPJSYDGZQCXCMD)


BEGIN_COM_MAP(CLPJSYDGZQCXCmd)
	COM_INTERFACE_ENTRY(ILPJSYDGZQCXCmd)
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
	CLPJSYDGZQCXDlg *LPJSYDGZQCXDlg;
};

OBJECT_ENTRY_AUTO(__uuidof(LPJSYDGZQCXCmd), CLPJSYDGZQCXCmd)
