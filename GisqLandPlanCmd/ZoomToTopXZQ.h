// ZoomToTopXZQ.h : CZoomToTopXZQ 的声明

#pragma once
#include "resource.h"       // 主符号

#include "GisqLandPlanCmd_i.h"

#include "BaseCommand.h"
#include "..\include\GlobalDefine.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif



// CZoomToTopXZQ

class ATL_NO_VTABLE CZoomToTopXZQ :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CZoomToTopXZQ, &CLSID_ZoomToTopXZQ>,
	public IDispatchImpl<IZoomToTopXZQ, &IID_IZoomToTopXZQ, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CZoomToTopXZQ():CBaseCommand(IDB_ZOOMTOXZQ,_T("全图显示"),NULL,_T("全图显示"),_T("全图显示")
		,_T("全图显示"),_T("全图显示"))
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
