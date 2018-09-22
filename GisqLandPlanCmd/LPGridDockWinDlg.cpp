// GridDockWinDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LPGridDockWinDlg.h"


// CGridDockWinDlg 对话框

IMPLEMENT_DYNAMIC(CLPGridDockWinDlg, CBCGPDialog)

CLPGridDockWinDlg::CLPGridDockWinDlg(_DGisqFrameworkCOMPtr	ipFrameworkCom,CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CLPGridDockWinDlg::IDD, pParent)
	/*, m_strResCount(_T(""))*/
{
	m_ipFramework = ipFrameworkCom;
	m_nActiveTab = -1;
}

CLPGridDockWinDlg::~CLPGridDockWinDlg()
{
}

void CLPGridDockWinDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	/*DDX_Text(pDX, IDC_RESCOUNT, m_strResCount);*/
	/*DDX_Control(pDX, IDC_RESULTTAB_RECT, m_ResultTabRect);*/
	/*DDX_Control(pDX, IDC_RESCOUNT, m_wndResCount);*/
}


BEGIN_MESSAGE_MAP(CLPGridDockWinDlg, CBCGPDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(BCGM_GRID_SEL_CHANGED,OnSelectColumn)
	ON_REGISTERED_MESSAGE(BCGM_CHANGE_ACTIVE_TAB, OnChangeActiveTab)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CGridDockWinDlg 消息处理程序

BOOL CLPGridDockWinDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	CComQIPtr<IWorkspace> spWorkspace;
	CComQIPtr<GisqLib::IGSDataExchangeS> spGSDataExchangeS = m_ipFramework->GetDataExchangeS();
	BSTR bstrTag(_T("GHWS"));
	short index = spGSDataExchangeS->FindIndex(bstrTag);
	if (index >= 0)
	{
		CComQIPtr<GisqLib::IGSDataExchange> spGSDataExchange = spGSDataExchangeS->Item(index);
		CComVariant var = spGSDataExchange->Item(0);
		spWorkspace = var.punkVal;
	}
	if (spWorkspace != NULL) m_spFtWorkspace = spWorkspace;
	
	CRect rc;
	GetClientRect(&rc);
	//STYLE_3D_SCROLLED右侧，STYLE_FLAT左侧,STYLE_3D_ONENOTE两侧,STYLE_3D_VS2005两侧，STYLE_3D_ROUNDED_SCROLL右侧
	m_wndResultTab.Create(CBCGPTabWnd::STYLE_FLAT , rc, this, IDC_RESGRIDTAB, CBCGPTabWnd::LOCATION_TOP, TRUE);
	m_wndResultTab.EnableActiveTabCloseButton(TRUE);
	m_wndResultTab.m_pFrameworkCom = m_ipFramework;
	//InitGridCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

HRESULT CLPGridDockWinDlg::SetQueryFilter(IUnknown* pQueryFilter, BSTR bstrFeatureClassName, BSTR bstrTabName)
{
	if (pQueryFilter == NULL || bstrFeatureClassName == NULL)
	{
		return E_FAIL;
	}
	if (m_spFtWorkspace == NULL)
	{
		return E_FAIL;
	}
	//CBCGPGridCtrl * pGrid = new CBCGPGridCtrl;
	CRect rectTabArea;
	CRect rectTabButtonArea;
	m_wndResultTab.GetTabArea(rectTabArea,rectTabButtonArea);
	CWnd *pParent = &m_wndResultTab;

	m_wndResultTab.GetClientRect(&rectTabArea);
	ClientToScreen(rectTabArea);
	CLPGridDockTabWnd * pTabGridwnd = new CLPGridDockTabWnd;
	CWnd *pTabwnd = pTabGridwnd;
	pTabwnd->Create(NULL,bstrTabName,WS_CHILD |WS_VISIBLE, rectTabArea, pParent, IDD_GRIDDOCKTABWND,NULL);
	pTabGridwnd->SetWorkspace(m_spFtWorkspace);
	pTabGridwnd->SetFramework(m_ipFramework);
	m_wndResultTab.AddTab(pTabwnd,bstrTabName);
	pTabGridwnd->SetQueryFilter(pQueryFilter,bstrFeatureClassName,bstrTabName);
	//add by lcc 2016.12.14 设置最后添加的tab页为活动视图
	int nCount = m_wndResultTab.GetTabsNum();
	m_wndResultTab.SetActiveTab(nCount-1);
	m_nActiveTab = nCount-1;
	//pTabGridwnd->SetResText(_T(""));
	//add by lcc 2016.12.14 end

// 	pGrid->Create(WS_CHILD |WS_VISIBLE, rectTabArea, pParent, 1);
// 	pGrid->EnableMarkSortedColumn (FALSE);//设置表背景初始化为无
// 	pGrid->EnableRowHeader (TRUE);//显示行头
// 	pGrid->EnableLineNumbers ();//显示行号
// 	pGrid->SetReadOnly(TRUE);
// 	CBCGPGridColors GridTheme;
// 	GridTheme = pGrid->GetColorTheme();
// 	GridTheme.m_clrHorzLine = RGB (0, 0, 0);//设置水平网格线颜色
// 	GridTheme.m_clrVertLine = RGB (0, 0, 0);//设置垂直网格线颜色
// 	pGrid->SetColorTheme(GridTheme);
// 	m_wndResultTab.AddTab(pGrid,bstrTabName);
// 	m_wndResultTab.SetActiveTab(m_wndResultTab.GetTabsNum());
// 
// 	m_strLayerName.Format(_T("SDE.%s"),bstrFeatureClassName);
// 	CComQIPtr<IQueryFilter> spQueryFilter = pQueryFilter;
// 	IFeatureClassPtr ipFtClass;
// 	m_spFtWorkspace->OpenFeatureClass(bstrFeatureClassName,&ipFtClass);
// 	if (ipFtClass == NULL) return FALSE;
// 	BSTR bstrName;
// 	ipFtClass->get_ShapeFieldName(&bstrName);
// 	IFeatureCursorPtr ipFtCursor;
// 	ipFtClass->Search(spQueryFilter,VARIANT_TRUE,&ipFtCursor);
// 
// 	if (ipFtCursor == NULL) return FALSE;
// 
// 	int nResult = 0;
// 	IFeaturePtr ipFt;
// 	ipFtCursor->NextFeature(&ipFt);
// 
// 	//添加列头
// 	int nMJIndex = -1;//用于保存面积字段索引
// 	double dbMJ = 0;//用于统计结果地块总面积
// 	CString strMJMC;
// 	BSTR bstrOIDName;
// 	ipFtClass->get_OIDFieldName(&bstrOIDName);
// 	//写表头
// 	if (ipFt != NULL)
// 	{
// 		IFieldsPtr ipFields;
// 		long nNumField;
// 		ipFt->get_Fields(&ipFields);
// 		if (ipFields == NULL)return FALSE;
// 
// 		ipFields->get_FieldCount(&nNumField);
// 		for (int i = 0; i < nNumField -1; i++)
// 		{
// 			IFieldPtr ipField;
// 			BSTR bstrName;
// 			ipFields->get_Field(i,&ipField);
// 			ipField->get_Name(&bstrName);
// 			CString strName = bstrName;
// 			if (strName.Find(_T("MJ")) >= 0)
// 			{
// 				nMJIndex = i;
// 				strMJMC = strName;
// 			}
// 			if (strName.Find(bstrOIDName) >= 0 && bstrOIDName != NULL)
// 			{
// 				m_nOIDFieldNameColumn = i;
// 				m_strOIDFieldName = bstrOIDName;
// 			}
// 			ipField->get_AliasName(&bstrName);
// 			pGrid->InsertColumn(i,bstrName,100);
// 		}
// 	}
// 	//遍历结果集合
// 	while(ipFt != NULL)
// 	{
// 		nResult ++;
// 
// 		IFieldsPtr ipFields;
// 		long nNumField;
// 		ipFt->get_Fields(&ipFields);
// 		if (ipFields == NULL)return FALSE;
// 
// 		ipFields->get_FieldCount(&nNumField);
// 		CBCGPGridRow *pGridRow = pGrid->CreateRow(nNumField);
// 		for (int i = 0; i < nNumField -1; i++)
// 		{
// 			IFieldPtr ipField;
// 			VARIANT vtTmp;
// 			ipFields->get_Field(i,&ipField);
// 			ipFt->get_Value(i,&vtTmp);
// 			pGridRow->GetItem(i)->SetValue(vtTmp);
// 			if (i == nMJIndex && vtTmp.vt == VT_R8)
// 			{
// 				dbMJ += vtTmp.dblVal;
// 			}
// 		}
// 		pGrid->AddRow(pGridRow);
// 
// 		ipFtCursor->NextFeature(&ipFt);
// 	}
// 
// 	pGrid->AdjustLayout();
// 	pGrid->SetFocus();
// // 	pGrid->m_ipFramework = m_ipFramework;
// // 	pGrid->m_nOIDFieldNameColumn = m_nOIDFieldNameColumn;
// // 	pGrid->m_strOIDFieldName = m_strOIDFieldName;
// // 	pGrid->m_strLayerName = m_strLayerName;
// // 	pGrid->dbMJ = dbMJ;
// // 	pGrid->nCount = nResult;
// // 	pGrid->m_strMJFieldName = strMJMC;
// 
// //	m_strResCount.Format(_T("共%d条结果,总面积%lf平方米(从%s字段统计)"),nResult,dbMJ,strMJMC);
	UpdateData(FALSE);
	SendMessage(WM_SIZE);

	return S_OK;
}

void CLPGridDockWinDlg::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect DlgRect;
	GetWindowRect(&DlgRect);
	ScreenToClient(&DlgRect);
	if (DlgRect.Width() == 0 || DlgRect.Height() == 0) return;
	//DlgRect.bottom = DlgRect.bottom - 20;
	if (m_wndResultTab.GetSafeHwnd() != NULL)
	{
		m_wndResultTab.MoveWindow(DlgRect);
		CRect rectTabArea;
		CRect rectTabBottom;
		m_wndResultTab.GetTabArea(rectTabArea,rectTabBottom);
	}
// 	if (m_wndResCount.GetSafeHwnd() != NULL)
// 	{
// 		GetWindowRect(&DlgRect);
// 		ScreenToClient(&DlgRect);
// 		DlgRect.top = DlgRect.bottom - 20;
// 		DlgRect.left = DlgRect.left + 5;
// 		m_wndResCount.MoveWindow(DlgRect);
// 	}

}

void CLPGridDockWinDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}

void CLPGridDockWinDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CBCGPDialog::OnClose();
}

void CLPGridDockWinDlg::OnDestroy()
{
	CBCGPDialog::OnDestroy();
	delete this;

	// TODO: 在此处添加消息处理程序代码
}

LRESULT CLPGridDockWinDlg::OnSelectColumn(WPARAM, LPARAM lp)
{

// 	
// 	CBCGPGridRow *pRow = NULL;
// 	pRow = m_wndResultGrid.GetCurSel();
// 	if (pRow == NULL) return S_OK;
// 	if (m_wndResultGrid.IsRowSelected(pRow->GetRowId(),TRUE))
// 	{
// 		VARIANT vt = pRow->GetItem(m_nOIDFieldNameColumn)->GetValue();
// 		CComQIPtr<IQueryFilter> spQuery;
// 		spQuery.CoCreateInstance(CLSID_QueryFilter);
// 		spQuery->put_SubFields(_T("*"));
// 		CString strWhere;
// 		strWhere.Format(_T("%s = \'%s\'"),m_strOIDFieldName,VTtoCString(vt));
// 		spQuery->put_WhereClause(strWhere.AllocSysString());
// 
// 		CComQIPtr<IMapControl2> spMapCtrl;
// 		spMapCtrl = m_ipFramework->GetActiveControl();
// 		if (spMapCtrl == NULL) return S_OK;
// 		CComQIPtr<IMap> spMap;
// 		spMapCtrl->get_Map(&spMap);
// 
// 		CComQIPtr<IUID> spUID;
// 		spUID.CoCreateInstance(CLSID_UID);
// 		CComQIPtr<IEnumLayer> spEnumLayer;
// 		spUID->put_Value(_variant_t("{40A9E885-5533-11d0-98BE-00805F7CED21}"));
// 		spMap->get_Layers(spUID,VARIANT_TRUE,&spEnumLayer);
// 		if (spEnumLayer == NULL) return S_OK;
// 		CComQIPtr<ILayer> spLayer;
// 
// 		spEnumLayer->Next(&spLayer);
// 		while(spLayer != NULL)
// 		{
// 			BSTR bstrLayerName;
// 			CComQIPtr<IFeatureLayer> spFtLayer;
// 			spFtLayer = spLayer;
// 			CComQIPtr<IFeatureClass> spFtClass;
// 			spFtLayer->get_FeatureClass(&spFtClass);
// 			spFtClass->get_AliasName(&bstrLayerName);
// 			CComQIPtr<IDataset> spDataset;
// 			spDataset = spFtClass;
// 			spDataset->get_Name(&bstrLayerName);
// 			if (m_strLayerName.Find(bstrLayerName) >= 0)
// 			{				
// 				CComQIPtr<IFeatureCursor> spFtCursor;
// 				spFtLayer->Search(spQuery,VARIANT_FALSE,&spFtCursor);
// 				if (spQuery == NULL) return S_OK;
// 				CComQIPtr<IFeature> spFeature;
// 				spFtCursor->NextFeature(&spFeature);
// 				if (spFeature == NULL) return S_OK;
// 				HRESULT hr = spMap->SelectFeature(spLayer,spFeature);
// 				ZoomToScale(spMapCtrl,spFeature);
// 				break;
// 			}
// 			spLayer.Release();
// 			spEnumLayer->Next(&spLayer);
// 		}
// 	}
	return S_OK;
}
CString CLPGridDockWinDlg::VTtoCString(VARIANT m_variant)
{
	CString temp;
	switch(m_variant.vt)
	{
	case 3:
		temp.Format(L"%ld",m_variant.iVal);
		break;//long
	case 5:
		temp.Format(L"%lf",m_variant.dblVal);
		break;//double
	case 8:
		temp = m_variant.bstrVal;
		break;//BSTR
	case 13:
		temp = L"PolyGon";
		break;//IUnknown FAR*
	default:
		temp.Empty();
		break;
	}
	return temp;
}
void CLPGridDockWinDlg::ZoomToScale(IMapControl2* ipMapControl,IFeature* ipFt)
{
	if (ipFt != NULL)
	{	
		IMapPtr ipMap;
		ipMapControl->get_Map(&ipMap);
		ISpatialReferencePtr ipSpatial;
		ipMap->get_SpatialReference(&ipSpatial);
		BSTR spatName;
		ipSpatial->get_Name(&spatName);

		IGeometryPtr ipGeo;
		IEnvelopePtr ipEnvelope;
		IEnvelopePtr ipZoomToEnv(CLSID_Envelope);
		ipFt->get_Shape(&ipGeo);

		esriGeometryType Geotype;
		ipGeo->get_GeometryType(&Geotype);
		ipGeo->get_Envelope(&ipEnvelope);
		if (ipEnvelope != NULL && Geotype == esriGeometryPolygon)
		{

			double XMax,XMin,YMax,YMin;

			ipEnvelope->get_XMax(&XMax);
			ipEnvelope->get_XMin(&XMin);
			ipEnvelope->get_YMax(&YMax);
			ipEnvelope->get_YMin(&YMin);
			//获取要素的外接矩形
			ipZoomToEnv->Union(ipEnvelope);
			IActiveViewPtr ipActiveView;
			ipMapControl->get_ActiveView(&ipActiveView);
			//以这个矩形为边界进行缩放
			ipActiveView->put_FullExtent(ipZoomToEnv);
			ipActiveView->PartialRefresh(esriViewAll,NULL,NULL);
		}
	}
}
//edit by lcc 2016.12.17 添加关闭tab页功能
void CBCGPTabWndEx::OnClickCloseButton(CWnd* pWndActive)
{
	if (1 < GetTabsNum())
	{
		int iTab = GetActiveTab();
		if(iTab!=0)
			SetActiveTab(0);
		RemoveTab(iTab, TRUE);
	}else if (1 == GetTabsNum())
	{
		int iTab = GetActiveTab();
		if(iTab!=0)
			SetActiveTab(0);
		RemoveTab(iTab, TRUE);
		CComQIPtr<GisqLib::IGSDockableWindowManager> spGSDockableWindowManager = m_pFrameworkCom->GetDockableWindowManager();
		short nCount = 0;
		nCount = spGSDockableWindowManager->Count();
		for (short i = 0; i < nCount; i++)
		{
			CComQIPtr<GisqLib::IGSDockableWindow> spDockwin = spGSDockableWindowManager->Item(i);
			_bstr_t bstrGUID = spDockwin->GetDockWindowDefGUID();
			if (bstrGUID == _bstr_t("GisqLandPlanCmd.LPGridDockWinCmd.1"))
			{
				spDockwin->ShowWindow(VARIANT_FALSE);
			}
		}
	}
	Invalidate(TRUE);
}
//edit by lcc 2016.12.17 end

// 	CLPGridDockWinDlg *pParent = (CLPGridDockWinDlg*)GetParent();
// 	pParent->m_strResCount.Format(_T("共%d条结果,总面积%lf平方米(从%s字段统计)"),pGrid->nCount,pGrid->dbMJ,pGrid->m_strMJFieldName);
// 	pParent->UpdateData(FALSE);
LRESULT CLPGridDockWinDlg::OnChangeActiveTab(WPARAM wp, LPARAM lp)
{
// 	if (m_nActiveTab >= 0)
// 	{
// 		CLPGridDockTabWnd * pTabGridwnd = (CLPGridDockTabWnd *)m_wndResultTab.GetTabWnd(m_nActiveTab);
// 		if (pTabGridwnd == NULL)
// 		{
// 			return S_OK;
// 		}
// 		pTabGridwnd->m_wndResultGrid.SetCurSel(NULL);
// 		m_nActiveTab = (int)wp;
// 
// 		CComQIPtr<IMapControl2> spMapCtrl;
// 		spMapCtrl = m_ipFramework->GetActiveControl();
// 		if (spMapCtrl == NULL) return S_OK;
// 		CComQIPtr<IMap> spMap;
// 		spMapCtrl->get_Map(&spMap);
// 		spMap->ClearSelection();
// 		IActiveViewPtr ipActiveView;
// 		spMapCtrl->get_ActiveView(&ipActiveView);
// 		ipActiveView->PartialRefresh(esriViewAll,NULL,NULL);
// 
// 	}
	return S_OK;
}

void CLPGridDockWinDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CBCGPDialog::OnLButtonUp(nFlags, point);
	if (m_wndResultTab.GetSafeHwnd() == NULL)
	{
		return;
	}
	int nIndex = m_wndResultTab.GetActiveTab();
	if (nIndex < 0)
	{
		return;
	}
	CLPGridDockTabWnd *pTab = (CLPGridDockTabWnd*)m_wndResultTab.GetTabWnd(nIndex);
	int nMin,nMax;
	CScrollBar* pVScrollbar=pTab->m_wndResultGrid.GetScrollBarCtrl(SB_VERT);
	if (pVScrollbar)
	{
		pVScrollbar->GetScrollRange(&nMin,&nMax);
		float fCurPos = (float)pVScrollbar->GetScrollPos();
		if (nMax > 0 && fCurPos > 0) 
		{
			if (fCurPos / nMax > 0.8)
			{
				pTab->m_wndResultGrid.FillTable();
			}
		}
	}
}
