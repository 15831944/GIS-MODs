// LPZXCQ_JSYDKJGZQCmd.h : CLPZXCQ_JSYDKJGZQCmd ������

#pragma once
#include "resource.h"       // ������

#include "GisqLandPlanPageLayOutCmd_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

#include "BaseCommand.h"
#include "ExportThemeMap.h"
#include "..\include\Global.h"

// CLPZXCQ_JSYDKJGZQCmd

class ATL_NO_VTABLE CLPZXCQ_JSYDKJGZQCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPZXCQ_JSYDKJGZQCmd, &CLSID_LPZXCQ_JSYDKJGZQCmd>,
	public IDispatchImpl<ILPZXCQ_JSYDKJGZQCmd, &IID_ILPZXCQ_JSYDKJGZQCmd, &LIBID_GisqLandPlanPageLayOutCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CLPZXCQ_JSYDKJGZQCmd():CBaseCommand(0,_T("�����õؿռ������ͼ"),_T("CLPZXCQ_JSYDKJGZQCmd"),_T("�����õؿռ������ͼ"),_T("�����õؿռ������ͼ")
		,_T("�����õؿռ������ͼ"),_T("�����õؿռ������ͼ"))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LPZXCQ_JSYDKJGZQCMD)


BEGIN_COM_MAP(CLPZXCQ_JSYDKJGZQCmd)
	COM_INTERFACE_ENTRY(ILPZXCQ_JSYDKJGZQCmd)
	//COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY2(IDispatch,ILPZXCQ_JSYDKJGZQCmd)
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
// 	STDMETHOD(get_Enabled)(VARIANT_BOOL * bEnabled );
// 	CComBSTR m_bstrXZQName;
// 	CComBSTR m_bstrXZQCode;
};

OBJECT_ENTRY_AUTO(__uuidof(LPZXCQ_JSYDKJGZQCmd), CLPZXCQ_JSYDKJGZQCmd)
