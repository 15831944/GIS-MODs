// GridDockWinDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LPGridDockWinDlg.h"


// CGridDockWinDlg �Ի���

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


// CGridDockWinDlg ��Ϣ�������

BOOL CLPGridDockWinDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	//STYLE_3D_SCROLLED�Ҳ࣬STYLE_FLAT���,STYLE_3D_ONENOTE����,STYLE_3D_VS2005���࣬STYLE_3D_ROUNDED_SCROLL�Ҳ�
	m_wndResultTab.Create(CBCGPTabWnd::STYLE_FLAT , rc, this, IDC_RESGRIDTAB, CBCGPTabWnd::LOCATION_TOP, TRUE);
	m_wndResultTab.EnableActiveTabCloseButton(TRUE);
	m_wndResultTab.m_pFrameworkCom = m_ipFramework;
	//InitGridCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
	//add by lcc 2016.12.14 ���������ӵ�tabҳΪ���ͼ
	int nCount = m_wndResultTab.GetTabsNum();
	m_wndResultTab.SetActiveTab(nCount-1);
	m_nActiveTab = nCount-1;
	//pTabGridwnd->SetResText(_T(""));
	//add by lcc 2016.12.14 end

// 	pGrid->Create(WS_CHILD |WS_VISIBLE, rectTabArea, pParent, 1);
// 	pGrid->EnableMarkSortedColumn (FALSE);//���ñ�����ʼ��Ϊ��
// 	pGrid->EnableRowHeader (TRUE);//��ʾ��ͷ
// 	pGrid->EnableLineNumbers ();//��ʾ�к�
// 	pGrid->SetReadOnly(TRUE);
// 	CBCGPGridColors GridTheme;
// 	GridTheme = pGrid->GetColorTheme();
// 	GridTheme.m_clrHorzLine = RGB (0, 0, 0);//����ˮƽ��������ɫ
// 	GridTheme.m_clrVertLine = RGB (0, 0, 0);//���ô�ֱ��������ɫ
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
// 	//�����ͷ
// 	int nMJIndex = -1;//���ڱ�������ֶ�����
// 	double dbMJ = 0;//����ͳ�ƽ���ؿ������
// 	CString strMJMC;
// 	BSTR bstrOIDName;
// 	ipFtClass->get_OIDFieldName(&bstrOIDName);
// 	//д��ͷ
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
// 	//�����������
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
// //	m_strResCount.Format(_T("��%d�����,�����%lfƽ����(��%s�ֶ�ͳ��)"),nResult,dbMJ,strMJMC);
	UpdateData(FALSE);
	SendMessage(WM_SIZE);

	return S_OK;
}

void CLPGridDockWinDlg::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
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
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

void CLPGridDockWinDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CBCGPDialog::OnClose();
}

void CLPGridDockWinDlg::OnDestroy()
{
	CBCGPDialog::OnDestroy();
	delete this;

	// TODO: �ڴ˴������Ϣ����������
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
			//��ȡҪ�ص���Ӿ���
			ipZoomToEnv->Union(ipEnvelope);
			IActiveViewPtr ipActiveView;
			ipMapControl->get_ActiveView(&ipActiveView);
			//���������Ϊ�߽��������
			ipActiveView->put_FullExtent(ipZoomToEnv);
			ipActiveView->PartialRefresh(esriViewAll,NULL,NULL);
		}
	}
}
//edit by lcc 2016.12.17 ��ӹر�tabҳ����
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
// 	pParent->m_strResCount.Format(_T("��%d�����,�����%lfƽ����(��%s�ֶ�ͳ��)"),pGrid->nCount,pGrid->dbMJ,pGrid->m_strMJFieldName);
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
