// ZoomToTopXZQ.cpp : CZoomToTopXZQ 的实现

#include "stdafx.h"
#include "ZoomToTopXZQ.h"


// CZoomToTopXZQ
IEnvelopePtr CZoomToTopXZQ::m_spFullExtent = (IUnknown*)NULL;
CComQIPtr<IFeatureWorkspace> CZoomToTopXZQ::m_ipFtWorkSpace = (IUnknown*)NULL;
BOOL  CZoomToTopXZQ::m_bIsLayoutJBT = FALSE;

STDMETHODIMP CZoomToTopXZQ::OnClick()
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComQIPtr<GisqLib::IGSActiveView> ipGSActiveView;
	short sType;

	ipGSActiveView = m_ipFramework->GetActiveView();
	sType = ipGSActiveView->GetViewType();
	if (sType == VT_MapControlView)
	{
		CComQIPtr<GisqLib::IGSDockableWindowManager> spGSDockableWindowManager = m_ipFramework->GetDockableWindowManager();
		short nCount = 0;
		nCount = spGSDockableWindowManager->Count();
		for (short i = 0; i < nCount; i++)
		{
			CComQIPtr<GisqLib::IGSDockableWindow> spDockwin = spGSDockableWindowManager->Item(i);
			_bstr_t bstrGUID = spDockwin->GetDockWindowDefGUID();
			if (bstrGUID == _bstr_t("GisqLandPlanCmd.CLPXZQWindowCmd.1"))
			{
				CComQIPtr<ICLPXZQWindowCmd> spXZQDockWnd = spDockwin->GetDockableWindowTabDef();
				spXZQDockWnd->ZoomToTopXZQ();
			}
		}
	}else if (sType == VT_PageLayoutView)
	{
		CComQIPtr<IMap> ipMap;

		//遍历找到主示图，如果找不到那么保存的是最后一个找到的mapframe
		CComQIPtr<IPageLayoutControl> spPageLayoutCtrl = (CComQIPtr<IPageLayoutControl>)ipGSActiveView->GetControl();
		CComQIPtr<IGraphicsContainer> ipGraphicsContainer;
		spPageLayoutCtrl->get_GraphicsContainer(&ipGraphicsContainer);
		ipGraphicsContainer->Reset();
		CComQIPtr<IElement> ipElement;
		ipGraphicsContainer->Next(&ipElement);
		while(ipElement != NULL)
		{
			CComQIPtr<IMap> spMap;
			CComQIPtr<IMapFrame> spMapFrame = ipElement;
			if (spMapFrame != NULL)
			{
				spMapFrame->get_Map(&spMap);
				if (spMap == NULL)
				{
					continue;
				}
				CComBSTR sMapName;
				spMap->get_Name(&sMapName);
				CString strMapName(sMapName);
				if (strMapName.Trim() == _T("主示图") || strMapName.Trim() == _T("总体规划图"))
				{
					ipMap = spMap;
					break;
				}
			}
			ipElement.Release();
			ipGraphicsContainer->Next(&ipElement);
		}
		if (ipMap == NULL)
		{
			return S_OK;
		}
		//找到map，确定是否是局部图
		if (m_bIsLayoutJBT)
		{
			ZoomToTop(ipMap);
		}else
		{
			CComQIPtr<IActiveView> spActiveView;
			spActiveView = ipMap;
			spActiveView->put_Extent(m_spFullExtent);
			spActiveView->Refresh();
		}
	}

	return S_OK;
}

STDMETHODIMP CZoomToTopXZQ::SetIsLayoutJBT(BOOL bIsLayoutJBT)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	m_bIsLayoutJBT = bIsLayoutJBT;
	return S_OK;
}

STDMETHODIMP CZoomToTopXZQ::SetFullExtent(IUnknown* pEnvelope)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码

	if (m_spFullExtent != NULL)
	{
		m_spFullExtent.Release();
	}
	if (pEnvelope == NULL)
	{
		return S_OK;
	}
	CComQIPtr<IEnvelope> spEnvelope = pEnvelope;
	CComQIPtr<IClone> spClone = spEnvelope;
	IClonePtr spNewClone;
	spClone->Clone(&spNewClone);
	m_spFullExtent = spNewClone;
	m_spFullExtent->AddRef();

	return S_OK;
}

void CZoomToTopXZQ::ZoomToTop(CComQIPtr<IMap> spMap)
{
	//缩放至最顶级的行政区
	CString strCode;
	CString strParentCode;
	short sType;

	HRESULT hr;
	//获取工作空间
	CComQIPtr<GisqLib::IGSDataExchangeS> spGSDataExchangeS = m_ipFramework->GetDataExchangeS();
	BSTR bstrTag(_T("GHWS"));
	short index = spGSDataExchangeS->FindIndex(bstrTag);
	if (index >= 0)
	{
		CComQIPtr<GisqLib::IGSDataExchange> spGSDataExchange = spGSDataExchangeS->Item(index);
		CComVariant var = spGSDataExchange->Item(0);
		IWorkspacePtr spWorkspace = var.punkVal;
		if (m_ipFtWorkSpace == NULL)
		{
			m_ipFtWorkSpace = spWorkspace;
		}
		//TODO…
	}
	CComQIPtr<ILPXZQDataTree> spXZQTree;
	CComQIPtr<ILPXZQDataItem> spXZQItem;
	IUnknown* pUnknown = NULL;
	spXZQTree.CoCreateInstance(CLSID_LPXZQDataTree);
	spXZQTree->GetRootItem(m_ipFtWorkSpace,&pUnknown);
	spXZQItem = pUnknown;
	if (spXZQItem == NULL)
	{
		return;
	}
	CComBSTR bstrCode;
	CComBSTR bstrParentCode;
	spXZQItem->Get_ItemCode(&bstrCode);
	spXZQItem->Get_ItemParentID(&bstrParentCode);
	strCode = bstrCode;
	strParentCode = bstrParentCode;
	//HTREEITEM hItem = m_wndXZQTree.GetRootItem();
	//if (hItem == NULL)
// 		{
// 			return;
// 		}
// 	_tagDistrictItem* DisItem; //= (_tagDistrictItem*)m_wndXZQTree.GetItemData(hItem);
// 	//ZoomToXZQ(ipMapControl,DisItem->strCode,DisItem->strParentID);
// 	strCode = DisItem->strCode;
// 	strParentCode = DisItem->strParentID;


	CString strWhere = _T("");
	CString strLayerName = _T("");
	switch(strCode.GetLength())
	{
	case 2:
		strLayerName = _T("XZQZ");
		strWhere = _T("XZQDM='") + strCode + _T("'");
		break;
	case 4:
		strLayerName = _T("XZQS");
		strWhere = _T("XZQDM='") + strCode + _T("'");
		break;
	case 6:
		strLayerName = _T("XZQX");
		strWhere = _T("XZQDM='") + strCode + _T("'");
		break;
	case 9:
		strLayerName = _T("XZQXZ");
		strWhere = _T("XZQDM='") + strCode + _T("'");
		break;
	case 12:
		strLayerName = _T("XZQ");
		strWhere = _T("XZQDM='") + strCode + _T("' and XZDM='") + strParentCode + _T("'");
		break;
	default:
		strLayerName = _T("XZQXZ");
		strCode.Replace(_T(","),_T("','"));
		strWhere = _T("XZQDM in ('") + strCode + _T("')");
		break;
	}
	CComQIPtr<ISpatialReference> spSpatialRefrence;
	spMap->get_SpatialReference(&spSpatialRefrence);
	CComQIPtr<IFeatureClass> spFtClass;
	m_ipFtWorkSpace->OpenFeatureClass(_bstr_t(strLayerName),&spFtClass);
	IQueryFilterPtr ipQuery(CLSID_QueryFilter);
	ipQuery->put_SubFields(_T("*"));
	ipQuery->put_WhereClause(strWhere.AllocSysString());
	CComQIPtr<IFeatureCursor> ipCursor;
	spFtClass->Search(ipQuery,VARIANT_FALSE,&ipCursor);

	CComQIPtr<IFeature> spFt;
	ipCursor->NextFeature(&spFt);
	if (spFt == NULL) return;
	CComQIPtr<IEnvelope> ipZoomToEnv;
	ipZoomToEnv.CoCreateInstance(CLSID_Envelope);
	ipZoomToEnv->putref_SpatialReference(spSpatialRefrence);
	while(spFt != NULL)
	{
		CComQIPtr<IGeometry> spGeo;
		CComQIPtr<IEnvelope> spEnv;
		spFt->get_ShapeCopy(&spGeo);
		spGeo->Project(spSpatialRefrence);
		spGeo->get_Envelope(&spEnv);
		ipZoomToEnv->Union(spEnv);
		spFt.Release();
		ipCursor->NextFeature(&spFt);
	}

	//以这个矩形为边界进行缩放			
	CComQIPtr<IActiveView> ipActiveView = spMap;
	ipActiveView->put_Extent(ipZoomToEnv);
	ipActiveView->Refresh();
}