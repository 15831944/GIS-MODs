// LPZDJSXMGHCmd.h : CLPZDJSXMGHCmd ������

#pragma once
#include "resource.h"       // ������

#include "GisqLandPlanPageLayOutCmd_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

#include "BaseCommand.h"
#include "ExportThemeMap.h"
#include "..\include\Global.h"

// CLPZDJSXMGHCmd

class ATL_NO_VTABLE CLPZDJSXMGHCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPZDJSXMGHCmd, &CLSID_LPZDJSXMGHCmd>,
	public IDispatchImpl<ILPZDJSXMGHCmd, &IID_ILPZDJSXMGHCmd, &LIBID_GisqLandPlanPageLayOutCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CLPZDJSXMGHCmd():CBaseCommand(0,_T("�ص㽨����Ŀ�滮ͼ"),_T("CLPZDJSXMGHCmd"),_T("�ص㽨����Ŀ�滮ͼ"),_T("�ص㽨����Ŀ�滮ͼ")
		,_T("�ص㽨����Ŀ�滮ͼ"),_T("�ص㽨����Ŀ�滮ͼ"))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LPZDJSXMGHCMD)


BEGIN_COM_MAP(CLPZDJSXMGHCmd)
	COM_INTERFACE_ENTRY(ILPZDJSXMGHCmd)
	//COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY2(IDispatch,ILPZDJSXMGHCmd)
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
	//edit by lcc 2016.11.23 ����������ȡ��ʽ��Ϊ���ô��ڻ�ȡ
// 	STDMETHOD(get_Enabled)(VARIANT_BOOL * bEnabled );//�����������û��������ʱ������
// 	CComBSTR m_bstrXZQName;
// 	CComBSTR m_bstrXZQCode;
};

OBJECT_ENTRY_AUTO(__uuidof(LPZDJSXMGHCmd), CLPZDJSXMGHCmd)
