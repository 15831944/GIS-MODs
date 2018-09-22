// LPZTGHJBTCmd.h : CLPZTGHJBTCmd ������

#pragma once
#include "resource.h"       // ������

#include "GisqLandPlanPageLayOutCmd_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

#include "BaseCommand.h"
#include "ExportThemeMap.h"
#include "..\include\Global.h"

// CLPZTGHJBTCmd

class ATL_NO_VTABLE CLPZTGHJBTCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPZTGHJBTCmd, &CLSID_LPZTGHJBTCmd>,
	public IDispatchImpl<ILPZTGHJBTCmd, &IID_ILPZTGHJBTCmd, &LIBID_GisqLandPlanPageLayOutCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CLPZTGHJBTCmd():CBaseCommand(0,_T("����滮�ֲ�ͼ"),_T("CLPZTGHJBTCmd"),_T("����滮�ֲ�ͼ"),_T("����滮�ֲ�ͼ")
		,_T("����滮�ֲ�ͼ"),_T("����滮�ֲ�ͼ"))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LPZTGHJBTCMD)


BEGIN_COM_MAP(CLPZTGHJBTCmd)
	COM_INTERFACE_ENTRY(ILPZTGHJBTCmd)
	//COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY2(IDispatch,ILPZTGHJBTCmd)
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
	HRESULT CreateGDBResult();
	HRESULT CreateXMDKFeatureClass();
	HRESULT AddCallout(CComQIPtr<IPageLayoutControl> &spPageLayoutControl, CComQIPtr<IMapFrame> spMapFrame, CComQIPtr<IFeature> spFt);
	void GetFeatureName(CComQIPtr<IFeature> spFt, CComBSTR &bstrName);
	void SetFontType(CComQIPtr<ITextElement> &spTextElement);
	void SetBalloonCalloutSymbol(CComQIPtr<IBalloonCallout> &spBallonCallout);
	void GetElementAnchorPoint(CComQIPtr<IPageLayoutControl> spPageLayoutControl, CComQIPtr<IMapFrame> spMapFrame, CComQIPtr<IFeature> spFt,
								CComQIPtr<IPoint> &spBalloonCalloutAnchorPoint,
								CComQIPtr<IPoint> &spTextElementAnchorPoint);
	CComQIPtr<IWorkspace> m_spGDBWorkspace;
	CComQIPtr<IFeatureClass> m_spXMDKFeatureClass;
	CComQIPtr<IFeatureDataset> m_spDSCurFeatureDataset;
	//edit by lcc 2016.11.23 ����������ȡ��ʽ��Ϊ���ô��ڻ�ȡ
// 	STDMETHOD(get_Enabled)(VARIANT_BOOL * bEnabled );
// 	CComBSTR m_bstrXZQName;
// 	CComBSTR m_bstrXZQCode;
};

OBJECT_ENTRY_AUTO(__uuidof(LPZTGHJBTCmd), CLPZTGHJBTCmd)
