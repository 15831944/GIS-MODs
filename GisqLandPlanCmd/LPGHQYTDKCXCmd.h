// LPGHQYTDKCXCmd.h : CLPGHQYTDKCXCmd ������

#pragma once
#include "resource.h"       // ������

#include "GisqLandPlanCmd_i.h"

#include "BaseCommand.h"
#include "..\GisqLandPlanPlatform\FrameSink.h"
#include "LPGHQYTDKCXDlg.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CLPGHQYTDKCXCmd

class ATL_NO_VTABLE CLPGHQYTDKCXCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPGHQYTDKCXCmd, &CLSID_LPGHQYTDKCXCmd>,
	public IDispatchImpl<ILPGHQYTDKCXCmd, &IID_ILPGHQYTDKCXCmd, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CLPGHQYTDKCXCmd():CBaseCommand(0,_T("�滮����;�ؿ��ѯ"),NULL,_T("�滮����;�ؿ��ѯ"),_T("�滮����;�ؿ��ѯ")
		,_T("�滮����;�ؿ��ѯ"),_T("�滮����;�ؿ��ѯ"))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LPGHQYTDKCXCMD)


BEGIN_COM_MAP(CLPGHQYTDKCXCmd)
	COM_INTERFACE_ENTRY(ILPGHQYTDKCXCmd)
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
	CLPGHQYTDKCXDlg *LPGHQYTDKCXDlg;
};

OBJECT_ENTRY_AUTO(__uuidof(LPGHQYTDKCXCmd), CLPGHQYTDKCXCmd)
