// ZoomToTopXZQ.h : CZoomToTopXZQ ������

#pragma once
#include "resource.h"       // ������

#include "GisqLandPlanCmd_i.h"

#include "BaseCommand.h"
#include "..\include\GlobalDefine.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CZoomToTopXZQ

class ATL_NO_VTABLE CZoomToTopXZQ :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CZoomToTopXZQ, &CLSID_ZoomToTopXZQ>,
	public IDispatchImpl<IZoomToTopXZQ, &IID_IZoomToTopXZQ, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CZoomToTopXZQ():CBaseCommand(IDB_ZOOMTOXZQ,_T("ȫͼ��ʾ"),NULL,_T("ȫͼ��ʾ"),_T("ȫͼ��ʾ")
		,_T("ȫͼ��ʾ"),_T("ȫͼ��ʾ"))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ZOOMTOTOPXZQ)


BEGIN_COM_MAP(CZoomToTopXZQ)
	COM_INTERFACE_ENTRY(IZoomToTopXZQ)
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
	static IEnvelopePtr m_spFullExtent;
	static CComQIPtr<IFeatureWorkspace> m_ipFtWorkSpace;
	static BOOL m_bIsLayoutJBT;
	STDMETHOD(OnClick)();
	STDMETHOD(SetIsLayoutJBT)(BOOL bIsLayoutJBT);
	STDMETHOD(SetFullExtent)(IUnknown* pEnvelope);
	void ZoomToTop(CComQIPtr<IMap> spMap);
};

OBJECT_ENTRY_AUTO(__uuidof(ZoomToTopXZQ), CZoomToTopXZQ)
