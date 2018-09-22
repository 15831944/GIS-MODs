// LPGHJQDLTBCXCmd.h : CLPGHJQDLTBCXCmd ������

#pragma once
#include "resource.h"       // ������

#include "GisqLandPlanCmd_i.h"

#include "BaseCommand.h"
#include "..\GisqLandPlanPlatform\FrameSink.h"
#include "LPGHJQDLTBCXDlg.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CLPGHJQDLTBCXCmd

class ATL_NO_VTABLE CLPGHJQDLTBCXCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPGHJQDLTBCXCmd, &CLSID_LPGHJQDLTBCXCmd>,
	public IDispatchImpl<ILPGHJQDLTBCXCmd, &IID_ILPGHJQDLTBCXCmd, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CLPGHJQDLTBCXCmd():CBaseCommand(0,_T("�滮���ڵ���ͼ�߲�ѯ"),NULL,_T("�滮���ڵ���ͼ�߲�ѯ"),_T("�滮���ڵ���ͼ�߲�ѯ")
		,_T("�滮���ڵ���ͼ�߲�ѯ"),_T("�滮���ڵ���ͼ�߲�ѯ"))
	{

	}

	DECLARE_REGISTRY_RESOURCEID(IDR_LPGHJQDLTBCXCMD)


	BEGIN_COM_MAP(CLPGHJQDLTBCXCmd)
		COM_INTERFACE_ENTRY(ILPGHJQDLTBCXCmd)
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

	// IGSCommandEx Methods
public:
	STDMETHOD (OnClick)( );
	CLPGHJQDLTBCXDlg *LPGHJQDLTBCXdlg;
};

OBJECT_ENTRY_AUTO(__uuidof(LPGHJQDLTBCXCmd), CLPGHJQDLTBCXCmd)
