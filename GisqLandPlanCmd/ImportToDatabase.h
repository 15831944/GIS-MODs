// ImportToDatabase.h : CImportToDatabase ������

#pragma once
#include "resource.h"       // ������

#include "GisqLandPlanCmd_i.h"

#include "BaseCommand.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CImportToDatabase

class ATL_NO_VTABLE CImportToDatabase :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CImportToDatabase, &CLSID_ImportToDatabase>,
	public IDispatchImpl<IImportToDatabase, &IID_IImportToDatabase, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CImportToDatabase():CBaseCommand(0,_T("�������"),NULL,_T("�������"),_T("�������")
		,_T("�������"),_T("�������"))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_IMPORTTODATABASE)


BEGIN_COM_MAP(CImportToDatabase)
	COM_INTERFACE_ENTRY(IImportToDatabase)
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

};

OBJECT_ENTRY_AUTO(__uuidof(ImportToDatabase), CImportToDatabase)