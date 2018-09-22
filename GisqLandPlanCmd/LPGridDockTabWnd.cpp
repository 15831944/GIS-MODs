// LPGridDockTabWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "LPGridDockTabWnd.h"


// CLPGridDockTabWnd 对话框

IMPLEMENT_DYNAMIC(CLPGridDockTabWnd, CGSBCGDialog)

CLPGridDockTabWnd::CLPGridDockTabWnd(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CLPGridDockTabWnd::IDD, pParent)
	, m_strResCount(_T(""))
{

}

CLPGridDockTabWnd::~CLPGridDockTabWnd()
{
}

void CLPGridDockTabWnd::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RESTEXT, m_wndResCount);
	//DDX_Text(pDX, IDC_RESTEXT, m_strResCount);
}


BEGIN_MESSAGE_MAP(CLPGridDockTabWnd, CBCGPDialog)
	//ON_REGISTERED_MESSAGE(BCGM_GRID_SEL_CHANGED,OnSelectColumn)
	//edit by lcc 2016.12.15 添加双击缩放
	//ON_REGISTERED_MESSAGE(BCGM_GRID_ITEM_DBLCLICK,OnDBClickColumn)
	//edit by lcc 2016.12.15 end
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CLPGridDockTabWnd 消息处理程序
void CLPGridDockTabWnd::SetFramework(GisqLib::_DGisqFrameworkCOMPtr ipFramework)
{
	m_ipFramework = ipFramework;
}


void CLPGridDockTabWnd::SetWorkspace(CComQIPtr<IFeatureWorkspace> spFtWorkspace)
{
	m_spFtWorkspace = spFtWorkspace;
}

HRESULT CLPGridDockTabWnd::SetQueryFilter(IUnknown* pQueryFilter, BSTR bstrFeatureClassName, BSTR bstrTabName)
{
	USES_CONVERSION;
	if (pQueryFilter == NULL || bstrFeatureClassName == NULL)
	{
		return E_FAIL;
	}
	if (m_spFtWorkspace == NULL)
	{
		return E_FAIL;
	}

// 	m_strLayerName.Format(_T("%s"),bstrFeatureClassName);
// 	IQueryFilterPtr ipQueryFilter = pQueryFilter;
// 	ITablePtr ipTable;
// 	m_spFtWorkspace->OpenTable(bstrFeatureClassName,&ipTable);
// 	if (ipTable == NULL) return FALSE;

	CComQIPtr<IMap> spMap;
	CComQIPtr<IMapControl2> spMapCtrl;
	CComQIPtr<GisqLib::IGSActiveView> ipGSActiveView;
	CComQIPtr<GisqLib::_DGisqFrameworkCOM> spGisqFramework = m_ipFramework;
	for (int i = 0;i < spGisqFramework->GetViewCount();i++)
	{
		ipGSActiveView =spGisqFramework->GetViewItem(i);
		if (ipGSActiveView->GetViewType() == (short)VT_MapControlView)
		{
			spMapCtrl = ipGSActiveView->GetControl();
			spMapCtrl->get_Map(&spMap);
			break;
		}
	}
	CComQIPtr<IUID> spUID;
	spUID.CoCreateInstance(CLSID_UID);
	CComQIPtr<IEnumLayer> spEnumLayer;
	spUID->put_Value(_variant_t("{40A9E885-5533-11d0-98BE-00805F7CED21}"));
	spMap->get_Layers(spUID,VARIANT_TRUE,&spEnumLayer);
	if (spEnumLayer == NULL) return FALSE;
	CComQIPtr<ILayer> spLayer;

	spEnumLayer->Next(&spLayer);
	while(spLayer != NULL)
	{
		CComQIPtr<IFeatureLayer> spFtLayer;
		spFtLayer = spLayer;
		CComQIPtr<IFeatureClass> spFtClass;
		spFtLayer->get_FeatureClass(&spFtClass);
		if (spFtClass == NULL)
		{
			spLayer.Release();
			spEnumLayer->Next(&spLayer);
			continue;
		}
		CComBSTR bstrFtName;
		//spFtClass->get_AliasName(&bstrFtName);
		CComQIPtr<IDataset> spDataset = spFtClass;
		spDataset->get_Name(&bstrFtName);
		CString strFtName(bstrFtName);
		if (strFtName.Find(bstrFeatureClassName) >=0)
		{
			break;
		}
		spLayer.Release();
		spEnumLayer->Next(&spLayer);
	}
	if (spLayer == NULL)
	{
		return E_FAIL;
	}
	CComQIPtr<IFeatureLayer> spFtLayer = spLayer;
	CComQIPtr<IFeatureClass> ipFtClass;
	spFtLayer->get_FeatureClass(&ipFtClass);
	IFeatureLayerPtr ipFtLayer = (IUnknown*)spFtLayer;
	ITablePtr ipTable = ipFtLayer;
	IQueryFilterPtr ipQueryFilter = pQueryFilter;
	CComQIPtr<ITable> spTable = ipTable;
	CComBSTR bstrOIDFieldName;
	spTable->get_OIDFieldName(&bstrOIDFieldName);
	m_strOIDFieldName = bstrOIDFieldName;
	spTable->FindField(bstrOIDFieldName,&m_nOIDFieldNameColumn);
	m_strLayerName = bstrFeatureClassName;
	BSTR bstrName;

	ipFtClass->get_ShapeFieldName(&bstrName);
	IFeatureCursorPtr ipFtCursor;
	ipFtClass->Search(NULL,VARIANT_TRUE,&ipFtCursor);

	if (ipFtCursor == NULL) return FALSE;

	int nResult = 0;
	IFeaturePtr ipFt;
	ipFtCursor->NextFeature(&ipFt);

	//edit by lcc 2016.12.17 结果显示中隐藏系统字段
	BSTR bstrOIDName;
	BSTR bstrShapeFieldName;
	ipFtClass->get_OIDFieldName(&bstrOIDName);
	ipFtClass->get_ShapeFieldName(&bstrShapeFieldName);
	CComQIPtr<IField> spAreaField;
	CComQIPtr<IField> spLenthField;
	ipFtClass->get_AreaField(&spAreaField);
	ipFtClass->get_LengthField(&spLenthField);

	CRect rc;
	GetClientRect(&rc);
	rc.right = rc.right - 5;
	rc.bottom = rc.bottom - 30;
	//ClientToScreen(&rc);
 	m_wndResultGrid.Create(WS_CHILD |WS_VISIBLE, rc, this, 1);

	m_wndResultGrid.SetFtClassName(m_strLayerName);
	m_wndResultGrid.setColumHide(bstrOIDName,bstrShapeFieldName,spAreaField,spLenthField);
	m_wndResultGrid.SetInitValue(m_ipFramework,ipTable,ipQueryFilter,FALSE);

 	m_wndResultGrid.EnableMarkSortedColumn (FALSE);//设置表背景初始化为无
	m_wndResultGrid.EnableRowHeader (TRUE);//显示行头
	//m_wndResultGrid.EnableRowHeader (TRUE,-1,50);//显示行头
 	m_wndResultGrid.EnableLineNumbers ();//显示行号
 	m_wndResultGrid.SetReadOnly(TRUE);
	//add by lcc 2016.12.15 设置单选
	m_wndResultGrid.SetSingleSel(TRUE);
	m_wndResultGrid.SetWholeRowSel(TRUE);
	//add by lcc 2016.12.15 end
 	CBCGPGridColors GridTheme;
 	GridTheme = m_wndResultGrid.GetColorTheme();
 	GridTheme.m_clrHorzLine = RGB (0, 0, 0);//设置水平网格线颜色
 	GridTheme.m_clrVertLine = RGB (0, 0, 0);//设置垂直网格线颜色
	CBCGPGridColors::ColorData SelColors;
	SelColors.m_clrBackground = RGB (168,205,241);
	SelColors.m_clrBorder = RGB (168,205,241);
	SelColors.m_clrGradient = -1;
	SelColors.m_clrText = RGB (0, 0, 0);
	SelColors.m_nGradientAngle = 0;
	GridTheme.m_SelColors = SelColors;//设置选中颜色
 	m_wndResultGrid.SetColorTheme(GridTheme);



//  	//写表头
//  	if (ipFt != NULL)
//  	{
//  		IFieldsPtr ipFields;
//  		long nNumField;
//  		ipFt->get_Fields(&ipFields);
//  		if (ipFields == NULL)return FALSE;
//  
//  		ipFields->get_FieldCount(&nNumField);
//  		for (int i = 0; i < nNumField -1; i++)
//  		{
//  			IFieldPtr ipField;
//  			BSTR bstrName;
//  			ipFields->get_Field(i,&ipField);
//  			ipField->get_Name(&bstrName);
//  			CString strName = bstrName;
//  			if (strName.Find(_T("MJ")) >= 0)
//  			{
// 				if (nMJIndex < 0)
// 				{
// 					ipField->get_AliasName(&bstrName);
// 					nMJIndex = i;
// 					strMJMC = bstrName;
// 				}
// 
//  			}else if (strName.Find(bstrOIDName) >= 0 && bstrOIDName != NULL)
//  			{
//  				m_nOIDFieldNameColumn = i;
//  				m_strOIDFieldName = bstrOIDName;
// 				m_wndResultGrid.InsertColumn(i,bstrName,0);
// 				continue;
//  			}else if (strName.Find(bstrShapeFieldName) >= 0 && bstrShapeFieldName != NULL)
//  			{
// 				m_wndResultGrid.InsertColumn(i,bstrName,0);
// 				continue;
//  			}else if (ipField == spAreaField || ipField == spLenthField)
//  			{
// 				m_wndResultGrid.InsertColumn(i,bstrName,0);
// 				continue;
//  			}
//   			m_wndResultGrid.InsertColumn(i,bstrName,100);
//  		}
//  	}
// 	//edit by lcc 2016.12.17 结果显示中隐藏系统字段 end
// 
//  	//遍历结果集合
//  	while(ipFt != NULL)
//  	{
//  		nResult ++;
//  
//  		IFieldsPtr ipFields;
//  		long nNumField;
//  		ipFt->get_Fields(&ipFields);
//  		if (ipFields == NULL)return FALSE;
//  
//  		ipFields->get_FieldCount(&nNumField);
//  		CBCGPGridRow *pGridRow = m_wndResultGrid.CreateRow(nNumField);
//  		for (int i = 0; i < nNumField -1; i++)
//  		{
//  			IFieldPtr ipField;
//  			VARIANT vtTmp;
//  			ipFields->get_Field(i,&ipField);
//  			ipFt->get_Value(i,&vtTmp);
//  			pGridRow->GetItem(i)->SetValue(vtTmp);
//  			if (i == nMJIndex && vtTmp.vt == VT_R8)
//  			{
//  				dbMJ += vtTmp.dblVal;
//  			}
//  		}
//  		m_wndResultGrid.AddRow(pGridRow);
//  
//  		ipFtCursor->NextFeature(&ipFt);
//  	}
//  //edit by lcc 2016.12.17 添加根据配置隐藏和显示别名
// 	//打开配置文件找到图层属性
// 	TCHAR szAppFullPath[MAX_PATH];
// 	DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(CHAR);
// 	GetModuleFileName(NULL,szAppFullPath,dBufferSize);
// 	CString strAppPath(szAppFullPath);
// 	strAppPath = strAppPath.Mid(0,strAppPath.ReverseFind('\\'));
// 	CString strConfigFile = strAppPath + CONFIGPATH;
// 
// 	TiXmlDocument XmlDoc;
// 	TiXmlElement *RootElement = NULL;
// 	if (XmlDoc.LoadFile(W2A(strConfigFile.AllocSysString())) == false) return FALSE;
// 	RootElement = XmlDoc.FirstChildElement();
// 
// 	TiXmlElement *DataManageRuleElement = RootElement->FirstChildElement("DataManageRule");
// 	TiXmlElement *BuildingRulesElement = DataManageRuleElement->FirstChildElement("BuildingRules");
// 	//现在配置里第一个就是规划库，直接取第一个就行
// 	TiXmlElement *BuildingRuleElement = BuildingRulesElement->FirstChildElement("BuildingRule");
// 	TiXmlElement *LayersElement = BuildingRuleElement->FirstChildElement();
// 	TiXmlElement *LayerElement = LayersElement->FirstChildElement();
// 	TiXmlElement *FieldsElement = NULL;
// 	TiXmlElement *FieldElement = NULL;
// 	while(LayersElement != NULL)
// 	{
// 		TiXmlAttribute *pAttr = LayerElement->FirstAttribute();
// 		CString strLayerName = A2W(pAttr->Value());
// 		if (strLayerName.Compare(bstrFeatureClassName) == 0) break;
// 		LayerElement = LayerElement->NextSiblingElement();
// 	}
// 	if (LayerElement != NULL)
// 	{
// 		FieldsElement = LayerElement->FirstChildElement();
// 		FieldElement = FieldsElement->FirstChildElement();
// 	}
// 
// 	CMapStringToString strmapHidden;
// 	CMapStringToString strmapName;
// 	while(FieldElement != NULL)
// 	{
// 		CString strFieldName;
// 		CString strFieldAliasName;
// 		CString strHidden;
// 		utf8ToUnicode(FieldElement->Attribute("FieldName"),&strFieldName);
// 		utf8ToUnicode(FieldElement->Attribute("AliasName"),&strFieldAliasName);
// 		utf8ToUnicode(FieldElement->Attribute("Hidden"),&strHidden);
// 		if (strFieldName.IsEmpty() != true && strFieldAliasName.IsEmpty() != true)
// 		{
// 			strmapName.SetAt(strFieldName,strFieldAliasName);
// 		}
// 		if (strFieldName.IsEmpty() != true && strHidden.IsEmpty() != true)
// 		{
// 			strmapHidden.SetAt(strFieldName,strHidden);
// 		}
// 		FieldElement = FieldElement->NextSiblingElement();
// 	}
// 
// 	int nCount = m_wndResultGrid.GetColumnCount();
// 	for (int i = 0; i < nCount; i++)
// 	{
// 		CString strColunmAliasName;
// 		CString strIsHidden;
// 		CString strColunmName = m_wndResultGrid.GetColumnName(i);
// 		strmapName.Lookup(strColunmName,strColunmAliasName);
// 		if (strColunmAliasName.IsEmpty() != true)
// 		{
// 			m_wndResultGrid.SetColumnName(i,strColunmAliasName);
// 		}
// 		strmapHidden.Lookup(strColunmName,strIsHidden);
// 		if (strIsHidden.Compare(_T("true")) == 0)
// 		{
// 			m_wndResultGrid.SetColumnWidth(i,0);
// 		}
// 	}
// 	//edit by lcc 2016.12.17 添加根据配置隐藏和显示别名 end
// 
//  	m_wndResultGrid.AdjustLayout();
//  	m_wndResultGrid.SetFocus();
//  
	GetClientRect(&rc);
	if (rc.Height() > 0 && rc.Width() > 0)
	{
		rc.top = rc.bottom - 30;
		rc.bottom = rc.bottom - 10;
		if (m_wndResCount.GetSafeHwnd() == NULL)
		{
			m_wndResCount.Create(NULL, WS_CHILD | WS_VISIBLE, rc, this);
			CFont *ft = this->GetFont();
			m_wndResCount.SetFont(ft);
		}else
		{
			m_wndResCount.MoveWindow(rc);
		}
	}
	SetResText(_T(""));
// 	CString strCompare(bstrFeatureClassName);
// 	if (strCompare.Compare(_T("JSYDGZQ")) != 0 && strCompare.Compare(_T("TDYTQ")) != 0)
// 	{
// 		dbMJ /= 10000.0;
// 	}	
//  	m_strResCount.Format(_T("共%d条结果,总面积约%.4lf公顷(从'%s'字段统计)"),nResult,dbMJ,strMJMC);
// 
 	//m_wndResCount.SetWindowText(m_strResCount);
	//UpdateData(FALSE);

	return S_OK;
}

LRESULT CLPGridDockTabWnd::OnSelectColumn(WPARAM, LPARAM lp)
{
 	CBCGPGridRow *pRow = NULL;
 	pRow = m_wndResultGrid.GetCurSel();
 	if (pRow == NULL) return S_OK;
	//add by lcc 2016.12.15 点击任意位置选中行
	//m_wndResultGrid.SetCurSel(pRow);
	//add by lcc 2016.12.15 end
 	if (m_wndResultGrid.IsRowSelected(pRow->GetRowId(),TRUE))
 	{
 		VARIANT vt = pRow->GetItem(m_nOIDFieldNameColumn)->GetValue();
 		CComQIPtr<IQueryFilter> spQuery;
 		spQuery.CoCreateInstance(CLSID_QueryFilter);
 		spQuery->put_SubFields(_T("*"));
 		CString strWhere;
 		strWhere.Format(_T("%s = '%s'"),m_strOIDFieldName,VariantToString(vt));
 		spQuery->put_WhereClause(strWhere.AllocSysString());
 
 		CComQIPtr<IMapControl2> spMapCtrl;
 		spMapCtrl = m_ipFramework->GetActiveControl();
 		if (spMapCtrl == NULL) return S_OK;
 		CComQIPtr<IMap> spMap;
 		spMapCtrl->get_Map(&spMap);
		spMap->ClearSelection();
		ClearLastActiveTabSel();
 
 		CComQIPtr<IUID> spUID;
 		spUID.CoCreateInstance(CLSID_UID);
 		CComQIPtr<IEnumLayer> spEnumLayer;
 		spUID->put_Value(_variant_t("{40A9E885-5533-11d0-98BE-00805F7CED21}"));
 		spMap->get_Layers(spUID,VARIANT_TRUE,&spEnumLayer);
 		if (spEnumLayer == NULL) return S_OK;
 		CComQIPtr<ILayer> spLayer;
 
 		spEnumLayer->Next(&spLayer);
 		while(spLayer != NULL)
 		{
 			BSTR bstrLayerName;
 			CComQIPtr<IFeatureLayer> spFtLayer;
 			spFtLayer = spLayer;
 			CComQIPtr<IFeatureClass> spFtClass;
 			spFtLayer->get_FeatureClass(&spFtClass);
			if (spFtClass == NULL)
			{
				spLayer.Release();
				spEnumLayer->Next(&spLayer);
				continue;
			}
 			spFtClass->get_AliasName(&bstrLayerName);
 			CComQIPtr<IDataset> spDataset;
 			spDataset = spFtClass;
 			spDataset->get_Name(&bstrLayerName);
			CString strLayerName;
			strLayerName.Format(_T("%s"),bstrLayerName);
			strLayerName = strLayerName.Right(strLayerName.GetLength() - strLayerName.Find(_T(".")) - 1);
 			if (m_strLayerName.Compare(strLayerName) == 0)
 			{
 				CComQIPtr<IFeatureCursor> spFtCursor;
 				spFtLayer->Search(spQuery,VARIANT_TRUE,&spFtCursor);
 				if (spFtCursor == NULL) return S_OK;
 				CComQIPtr<IFeature> spFeature;
 				spFtCursor->NextFeature(&spFeature);
 				if (spFeature == NULL) return S_OK;
 				HRESULT hr = spMap->SelectFeature(spLayer,spFeature);
 				//ZoomToScale(spMapCtrl,spFeature);
 				break;
 			}
 			spLayer.Release();
 			spEnumLayer->Next(&spLayer);
 		}
		CComQIPtr<IActiveView> ipActiveView;
		spMapCtrl->get_ActiveView(&ipActiveView);
		ipActiveView->Refresh();
 	}
	return S_OK;
}

void CLPGridDockTabWnd::ZoomToScale(IMapControl2* ipMapControl,IFeature* ipFt)
{
	if (ipFt != NULL)
	{	
		CComQIPtr<IMap> ipMap;
		ipMapControl->get_Map(&ipMap);
		ISpatialReferencePtr ipSpatial;
		ipMap->get_SpatialReference(&ipSpatial);
		BSTR spatName;
		ipSpatial->get_Name(&spatName);

		CComQIPtr<IGeometry> ipGeo;		
		ipFt->get_Shape(&ipGeo);
		CComQIPtr<IGeometryCollection> spGeoCollection;
		ipGeo->QueryInterface(&spGeoCollection);
		long nCount = 0;
		spGeoCollection->get_GeometryCount(&nCount);
//edit by lcc 2016.12.14 修改缩放到要素的逻辑
		CComQIPtr<IEnvelope> ipZoomToEnv;
		ipZoomToEnv.CoCreateInstance(CLSID_Envelope);
		for(int i = 0; i < nCount; i++)
		{
			CComQIPtr<IGeometry> spGeo;
			CComQIPtr<IEnvelope> ipEnvelope;
			spGeoCollection->get_Geometry(i,&spGeo);
			spGeo->get_Envelope(&ipEnvelope);
			ipZoomToEnv->Union(ipEnvelope);			
		}
// 		esriGeometryType Geotype;
// 		ipGeo->get_GeometryType(&Geotype);
// 		ipGeo->get_Envelope(&ipEnvelope);
// 		if (ipEnvelope != NULL && Geotype == esriGeometryPolygon)
// 		{
// 
// 			double XMax,XMin,YMax,YMin;
// 
// 			ipEnvelope->get_XMax(&XMax);
// 			ipEnvelope->get_XMin(&XMin);
// 			ipEnvelope->get_YMax(&YMax);
// 			ipEnvelope->get_YMin(&YMin);
// 			//获取要素的外接矩形
// 			ipZoomToEnv->Union(ipEnvelope);
// 		}
		SetFocusLayer((IUnknown*)NULL,ipMap);
// 		CComQIPtr<IUID> spUID;
// 		spUID.CoCreateInstance(CLSID_UID);
// 		CComQIPtr<IEnumLayer> spEnumLayer;
// 		spUID->put_Value(_variant_t("{EDAD6644-1810-11D1-86AE-0000F8751720}"));
// 		//spUID->put_Value(_variant_t("{40A9E885-5533-11d0-98BE-00805F7CED21}"));
// 		ipMap->get_Layers(spUID,VARIANT_TRUE,&spEnumLayer);
// 		if (spEnumLayer == NULL) return ;
// 		CComQIPtr<ILayer> spLayer;
// 
// 		spEnumLayer->Next(&spLayer);
// 		while(spLayer != NULL)
// 		{
// 			BSTR bstrLayerName;
// 			spLayer->get_Name(&bstrLayerName);
// 			CComQIPtr<IFeatureLayer> spFtLayer;
// 			spFtLayer = spLayer;
// 			CComQIPtr<IFeatureClass> spFtClass;
// 			spFtLayer->get_FeatureClass(&spFtClass);
// 			//edit by lcc 2016.12.06 添加验证防止有图层没有匹配上导致错误
// 			if (spFtClass == NULL)
// 			{
// 				spLayer.Release();
// 				spEnumLayer->Next(&spLayer);
// 				continue;
// 			}
// 			//edit by lcc 2016.12.06 end
// 			spFtClass->get_AliasName(&bstrLayerName);
// 			CComQIPtr<IDataset> spDataset;
// 			spDataset = spFtClass;
// 			spDataset->get_Name(&bstrLayerName);
// 			CString strFtName;
// 			strFtName.Format(_T("%s"),bstrLayerName);
// 			strFtName.Replace(_T("TDGH."),_T(""));
// 			if (m_strLayerName.Compare(strFtName) == 0 || strFtName.Find(_T("XZQ")) >= 0)
// 			{
// 				spLayer->put_Visible(VARIANT_TRUE);
// 			}else
// 			{
// 				spLayer->put_Visible(VARIANT_FALSE);
// 			}
// 			spLayer.Release();
// 			spEnumLayer->Next(&spLayer);
// 		}
		IActiveViewPtr ipActiveView;
		ipMapControl->get_ActiveView(&ipActiveView);
		//以这个矩形为边界进行缩放
		ipActiveView->put_Extent(ipZoomToEnv);
		ipActiveView->PartialRefresh(esriViewAll,NULL,NULL);
		FlashShapes(ipMapControl,ipGeo);
//edit by lcc 2016.12.14end
	}
}

void CLPGridDockTabWnd::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect(&rc);
	if (rc.Width() != 0 &&rc.Height() != 0 && m_wndResultGrid.GetSafeHwnd() != NULL)
	{
		rc.right = rc.right - 5;
		rc.bottom = rc.bottom - 20;
		m_wndResultGrid.MoveWindow(rc);
	}
	GetClientRect(&rc);
	if (rc.Height() > 0 && rc.Width() > 0 && m_wndResCount.GetSafeHwnd() != NULL)
	{
		rc.top = rc.bottom - 20;
		rc.bottom = rc.bottom;
		m_wndResCount.MoveWindow(rc);
	}
	// TODO: 在此处添加消息处理程序代码
}
//add by lcc 2016.12.15 添加双击缩放到选中地块
LRESULT CLPGridDockTabWnd::OnDBClickColumn(WPARAM, LPARAM lp)
{
	CComQIPtr<IMapControl2> spMapCtrl;
	spMapCtrl = m_ipFramework->GetActiveControl();
	if (spMapCtrl == NULL) return S_OK;
	CComQIPtr<IMap> spMap;
	spMapCtrl->get_Map(&spMap);
	CComQIPtr<ISelection> spSelection;
	spMap->get_FeatureSelection(&spSelection);
	CComQIPtr<IEnumFeature> spEnumFt;
	spEnumFt = spSelection;
	CComQIPtr<IFeature> spFeature;
	spEnumFt->Next(&spFeature);
	//add by lcc 2016.12.27添加数据为空时提示，主要针对启动模板的数据过滤
	if (spFeature == NULL)
	{
		MessageBox(_T("当前选中的要素未载入系统"),_T("错误！"),MB_OK);
	}
	//add by lcc 2016.12.27 end
	ZoomToScale(spMapCtrl,spFeature);
	return S_OK;
}
//edit by lcc 2016.12.15 end

void CLPGridDockTabWnd::utf8ToUnicode(LPCSTR src, CString *Result)  
{  
	int wcsLen = MultiByteToWideChar( CP_UTF8, 0, src, -1, NULL, 0 );  
	LPWSTR result = new wchar_t[wcsLen + 1];
	::MultiByteToWideChar( CP_UTF8, 0, src, -1, result, wcsLen); 
	Result->Format(_T("%s"),result);
	delete result;
} 

BOOL CLPGridDockTabWnd::SetFocusLayer(CComQIPtr<ILayer> spParentLayer, CComQIPtr<IMap> spMap)
{
	//如果是空，是第一次进入这个函数，从map获取所有图层组，开始遍历
	BOOL bIsSelect = FALSE;
	if (spParentLayer == NULL)
	{
		CComQIPtr<IUID> spUID;
		spUID.CoCreateInstance(CLSID_UID);
		CComQIPtr<IEnumLayer> spEnumLayer;
		spUID->put_Value(_variant_t("{EDAD6644-1810-11D1-86AE-0000F8751720}"));		
		spMap->get_Layers(spUID,VARIANT_TRUE,&spEnumLayer);
		if (spEnumLayer == NULL) return FALSE;
		CComQIPtr<ILayer> spLayer;

		spEnumLayer->Next(&spLayer);
		while(spLayer != NULL)
		{
			BSTR bstrLayerName;
			spLayer->get_Name(&bstrLayerName);
			CComQIPtr<IGroupLayer> spLayerGroup = spLayer;
			//不为空说明有下一层
			if (spLayerGroup != NULL)
			{
				CComQIPtr<ICompositeLayer> spCompLayer = spLayerGroup;
				if (spCompLayer == NULL) return FALSE;
				CComQIPtr<ILayer> spChlidLayer;
				long nCount = 0;
				spCompLayer->get_Count(&nCount);
				for (int i = 0; i < nCount; i++)
				{
					spCompLayer->get_Layer(i,&spChlidLayer);
					if (spChlidLayer != NULL)
					{
						if (SetFocusLayer(spChlidLayer,spMap))
						{
							spLayer->put_Visible(VARIANT_TRUE);
							bIsSelect = TRUE;
						}
					}
					spChlidLayer.Release();
				}
			}else//为空说明本层是要素层
			{
				CComQIPtr<IFeatureLayer> spFtLayer = spLayer;
				if (spFtLayer == NULL) return FALSE;
				CComQIPtr<IFeatureClass> spFtClass;
				spFtLayer->get_FeatureClass(&spFtClass);
				if (spFtClass == NULL)
	 			{
	 				spLayer.Release();
	 				spEnumLayer->Next(&spLayer);
	 				continue;
	 			}
	 			spFtClass->get_AliasName(&bstrLayerName);
	 			CComQIPtr<IDataset> spDataset;
	 			spDataset = spFtClass;
	 			spDataset->get_Name(&bstrLayerName);
	 			CString strFtName;
	 			strFtName.Format(_T("%s"),bstrLayerName);
				strFtName = strFtName.Right(strFtName.GetLength() - strFtName.Find(_T(".")) - 1);
	 			if (m_strLayerName.Compare(strFtName) == 0 || strFtName.Find(_T("XZQ")) >= 0)
	 			{
	 				spLayer->put_Visible(VARIANT_TRUE);
	 			}else
	 			{
	 				spLayer->put_Visible(VARIANT_FALSE);
	 			}

			}
			spLayer.Release();
			spEnumLayer->Next(&spLayer);
		}
	}else
	{
		BSTR bstrLayerName;
		CComQIPtr<IGroupLayer> spLayerGroup = spParentLayer;
		//不为空说明有下一层
		if (spLayerGroup != NULL)
		{
			CComQIPtr<ICompositeLayer> spCompLayer = spLayerGroup;
			if (spCompLayer == NULL) return FALSE;
			CComQIPtr<ILayer> spChlidLayer;
			long nCount = 0;
			spCompLayer->get_Count(&nCount);
			for (int i = 0; i < nCount; i++)
			{
				spCompLayer->get_Layer(i,&spChlidLayer);
				if (spChlidLayer != NULL)
				{
					if (SetFocusLayer(spChlidLayer,spMap))
					{
						spParentLayer->put_Visible(VARIANT_TRUE);
						return TRUE;
					}
// 					else
// 					{
// 						return FALSE;
// 					}
				}
				spChlidLayer.Release();
			}
		}else//为空说明本层是要素层
		{
			CComQIPtr<IFeatureLayer> spFtLayer = spParentLayer;
			if (spFtLayer == NULL) return FALSE;
			CComQIPtr<IFeatureClass> spFtClass;
			spFtLayer->get_FeatureClass(&spFtClass);
			if (spFtClass == NULL)
			{
				return FALSE;
			}
			spFtClass->get_AliasName(&bstrLayerName);
			CComQIPtr<IDataset> spDataset;
			spDataset = spFtClass;
			spDataset->get_Name(&bstrLayerName);
			CString strFtName;
			strFtName.Format(_T("%s"),bstrLayerName);
			strFtName = strFtName.Right(strFtName.GetLength() - strFtName.Find(_T(".")) - 1);
			//strFtName.Replace(_T("TDGH."),_T(""));
			if (m_strLayerName.Compare(strFtName) == 0 || strFtName.Find(_T("XZQ")) >= 0)
			{
				spParentLayer->put_Visible(VARIANT_TRUE);
				return TRUE;
			}else
			{
				spParentLayer->put_Visible(VARIANT_FALSE);
				return FALSE;
			}

		}
	}
	return bIsSelect;
}

void CLPGridDockTabWnd::FlashShapes(CComQIPtr<IMapControl2> ipMapControl, CComQIPtr<IGeometry> ipGeo )
{
	if (ipGeo == NULL)	return;

	IActiveViewPtr ipActiveView;
	ipMapControl->get_ActiveView(&ipActiveView);
	IMapPtr ipMap;
	ipActiveView->get_FocusMap(&ipMap);
	IScreenDisplayPtr ipScreenDisp;
	ipActiveView->get_ScreenDisplay(&ipScreenDisp);
	OLE_HANDLE hdc;
	ipScreenDisp->get_hDC(&hdc);

	// 为了提高绘制效率, 先在CacheMemDC里绘, 然后再绘制到ScreenDisplay上
	short nMemCacheID;
	ipScreenDisp->AddCache(&nMemCacheID);
	OLE_HANDLE hMemDC;
	ipScreenDisp->get_CacheMemDC(nMemCacheID, &hMemDC);

	// 设置symbol
	IColorPtr ipRgbColor(CLSID_RgbColor);
	ISimpleFillSymbolPtr ipStylePg(CLSID_SimpleFillSymbol);
	ISimpleLineSymbolPtr ipStyLn(CLSID_SimpleLineSymbol);
	//边界样式
	ipRgbColor->put_RGB(RGB(255, 123, 128));
	ipStyLn->put_Style(esriSLSSolid);
	ipStyLn->put_Color(ipRgbColor);
	ipStyLn->put_Width(4);
	//多边形样式
	ipStylePg->put_Color(ipRgbColor);
	ipStylePg->put_Style(esriSFSBackwardDiagonal);
	ipStylePg->put_Outline(ipStyLn);

	ISymbolPtr ipSymbol = ipStylePg;

	// 绘图形
	double dPointBufferDis = 6;
	double dLineBufferDis = 2;

	IDisplayTransformationPtr ipDisplayTransformation;
	ipScreenDisp->get_DisplayTransformation(&ipDisplayTransformation);
	ipSymbol->SetupDC(hMemDC, ipDisplayTransformation);

	//先更新屏幕到缩放后的位置
	ipScreenDisp->UpdateWindow();
	//edit by lcc 2016.12.14 修改行政区缩放逻辑
	esriGeometryType eShapeType;
	ipGeo->get_GeometryType(&eShapeType);

	CComQIPtr<IGeometryCollection> spGeoCollection;
	ipGeo->QueryInterface(&spGeoCollection);
	long nCount;
	spGeoCollection->get_GeometryCount(&nCount);
	for (long i = 0;i < nCount; i++)
	{
		IGeometryPtr ipShape, ipBufferShape;
		spGeoCollection->get_Geometry(i, &ipShape);
		ipBufferShape = ipShape;
// 		esriGeometryType eShapeType;
// 		ipShape->get_GeometryType(&eShapeType);
// 		if (eShapeType != esriGeometryPolygon)
// 		{
// 			double dBufferDis = 0;
// 			if (eShapeType == esriGeometryPoint)
// 				dBufferDis = dPointBufferDis;
// 			else if (eShapeType == esriGeometryPolyline)
// 				dBufferDis = dLineBufferDis;
// 
// 			ITopologicalOperatorPtr ipTopOper = ipShape;
// 			ipTopOper->Buffer(dBufferDis, &ipBufferShape);
// 		}
		ipSymbol->Draw(ipGeo);
	}

	//遍历集合
	ipSymbol->ResetDC();
	ipScreenDisp->DrawCache(hdc, nMemCacheID, NULL, NULL);
	ipScreenDisp->RemoveCache(nMemCacheID);
	// 延时100毫秒擦掉绘制
	::Sleep(200);
	ipActiveView->PartialRefresh(esriViewAll, NULL, NULL);

	//edit by lcc 2016.12.14 end

}

void CLPGridDockTabWnd::ClearLastActiveTabSel()
{
	CBCGPTabWndEx* pParent = (CBCGPTabWndEx*)GetParent();
	for (int i = 0; i < pParent->GetTabsNum(); i++)
	{
		CLPGridDockTabWnd * pTabGridwnd = (CLPGridDockTabWnd *)pParent->GetTabWnd(i);
		if (pTabGridwnd != NULL && i != pParent->GetActiveTab())
		{
			pTabGridwnd->m_wndResultGrid.SetCurSel(NULL);
		}
	}
}

void CLPGridDockTabWnd::SetResText(CString strValue)
{
	if (strValue.IsEmpty() != true)
	{
		m_strResCount = strValue;
	}
	if (m_wndResCount.GetSafeHwnd() != NULL)
	{
		m_wndResCount.SetWindowText(m_strResCount);
	}
}



void CLPGridDockTabWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CBCGPDialog::OnLButtonUp(nFlags, point);
// 	int nMin,nMax;
// 	CScrollBar* pVScrollbar=m_wndResultGrid.GetScrollBarCtrl(SB_VERT);
// 	if (pVScrollbar)
// 	{
// 		pVScrollbar->GetScrollRange(&nMin,&nMax);
// 		float fCurPos = (float)pVScrollbar->GetScrollPos();
// 		if (nMax > 0 && fCurPos > 0) 
// 		{
// 			if (fCurPos / nMax > 0.8)
// 			{
// 				m_wndResultGrid.FillTable();
// 			}
// 		}
// 	}
}
