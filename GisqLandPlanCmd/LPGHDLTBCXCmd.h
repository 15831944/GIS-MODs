/*********************************************
�ļ���:  LPGHDLTBCXCmd
������:  luchencheng
��д����:2016-10-09
��Ҫ����:�滮����ͼ�߲�ѯ���ܽӿ�
�޸ļ�¼(who-when-why):
********************************************/
// LPGHDLTBCXCmd.h : CLPGHDLTBCXCmd ������

#pragma once
#include "resource.h"       // ������

#include "GisqLandPlanCmd_i.h"

#include "BaseCommand.h"
#include "..\GisqLandPlanPlatform\FrameSink.h"
#include "LPGHDLTBCXDlg.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CLPGHDLTBCXCmd

class ATL_NO_VTABLE CLPGHDLTBCXCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPGHDLTBCXCmd, &CLSID_LPGHDLTBCXCmd>,
	public IDispatchImpl<ILPGHDLTBCXCmd, &IID_ILPGHDLTBCXCmd, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CLPGHDLTBCXCmd():CBaseCommand(0,_T("�滮����ͼ�߲�ѯ"),NULL,_T("�滮����ͼ�߲�ѯ"),_T("�滮����ͼ�߲�ѯ")
		,_T("�滮����ͼ�߲�ѯ"),_T("�滮����ͼ�߲�ѯ"))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LPGHDLTBCXCMD)


BEGIN_COM_MAP(CLPGHDLTBCXCmd)
	COM_INTERFACE_ENTRY(ILPGHDLTBCXCmd)
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
	CLPGHDLTBCXDlg *LPGHDLTBCXdlg;
};

OBJECT_ENTRY_AUTO(__uuidof(LPGHDLTBCXCmd), CLPGHDLTBCXCmd)
