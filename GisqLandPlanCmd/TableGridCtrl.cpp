#include "stdafx.h"
#include "TableGridCtrl.h"
#include "resource.h"
#include "LPGridDockTabWnd.h"


#define FILL_ROW_COUNT                 2000
#define HAS_NOT_SELECTED               -1

int           m_column_sort     = -1;
esriFieldType m_column_type     = esriFieldTypeSmallInteger;
BOOL          m_bSortAscending  = FALSE;

CTableGridCtrl::CTableGridCtrl():CBCGPGridCtrl()
							   ,m_pCursor(NULL)
							   ,m_nCurrentColumn(-1)
							   ,m_pFieldsClone(NULL)
							   ,m_SelColor(RGB(176,255,255))
							   ,m_bStartSelect(FALSE)
							   ,m_nLastSelectedTop(HAS_NOT_SELECTED)
							   ,m_nLastSelectedBottom(HAS_NOT_SELECTED)
							   ,m_bIsFillAllRecords(FALSE)
{

}
CTableGridCtrl::CTableGridCtrl(GisqLib::_DGisqFrameworkCOMPtr pFrameworkCOM,
							   ITablePtr pTable,
							   IQueryFilterPtr pQueryFilter,
							   BOOL bOnlyShowSelected )
	:CBCGPGridCtrl()
	,m_pFrameworkCOM(pFrameworkCOM)
	,m_bOnlyShowSelected(FALSE)
	,m_pTable(pTable)
	,m_pQueryFilter(pQueryFilter)
	,m_pCursor(NULL)
	//,m_nTotalCount(0)
	//,m_nSelectedCount(0)
	,m_nCurrentColumn(-1)
	,m_pFieldsClone(NULL)
	,m_SelColor(RGB(176,255,255))
	,m_bStartSelect(FALSE)
	,m_nLastSelectedTop(HAS_NOT_SELECTED)
	,m_nLastSelectedBottom(HAS_NOT_SELECTED)
	,m_bIsFillAllRecords(FALSE)
	//,m_bFilterSelected(FALSE)
{

}
CTableGridCtrl::~CTableGridCtrl(void)
{
	m_List.RemoveAll();
}

BEGIN_MESSAGE_MAP(CTableGridCtrl,CBCGPGridCtrl)
	//{{AFX_MSG_MAP(CTableGridCtrl)
	ON_WM_CREATE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SHOWWINDOW()
	ON_WM_CHAR()
END_MESSAGE_MAP()

BOOL CALLBACK CallbackGrid (BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT (pdi != NULL);

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item
	CTableGridCtrl * pGrid = (CTableGridCtrl*) lp;
	if (nCol > -1)
	{
		//if (pGrid->IsOnlyShowSelected())
		//{
		//	pdi->item.varValue = pGrid->m_List.GetSelectedAt(nRow)->FieldValue[nCol];
		//	pdi->item.clrBackground = pGrid->m_SelColor;
		//}
		//else
		//{
		    pdi->item.varValue = pGrid->m_List.GetSortAt(nRow) != NULL ? pGrid->m_List.GetSortAt(nRow)->FieldValue[nCol] : _T("");

			if (pGrid->m_List.GetSortAt(nRow) !=NULL && pGrid->m_List.GetSortAt(nRow)->bSelected)
			{
				pdi->item.clrBackground = pGrid->m_SelColor;
			}
			else
			{
				pdi->item.clrBackground = (COLORREF)-1;
			}
		//}
	}

	return TRUE;
}

LRESULT CALLBACK pfnFilterCallback( WPARAM wParam, LPARAM lParam )
{
	CBCGPGridRow* pRow = (CBCGPGridRow*) wParam;
	ASSERT_VALID (pRow);
	
	BOOL bSelected = (BOOL) lParam;
	if (!bSelected) return FALSE;

	CBCGPGridItem* pItem = pRow->GetItem (0);
	ASSERT_VALID (pItem);
	return (pItem->GetBackgroundColor() == (COLORREF)-1);
}

void CTableGridCtrl::LoadTable( void )
{
	if (m_pTable == NULL) return;

	EnableMarkSortedColumn(FALSE,FALSE);

	InitColumns();

	IDisplayTablePtr pDisplayTable = m_pTable;
	if (pDisplayTable == NULL) return ;

	//ITableSelectionPtr pTableSelection(m_pTable);
	//if (pTableSelection != NULL)
	//{
	//	ISelectionSetPtr pSelectionSet = NULL;
	//	pTableSelection->get_SelectionSet(&pSelectionSet);
	//	pSelectionSet->get_Count(&m_nSelectedCount);
	//}

	if (m_bOnlyShowSelected)  //只显示选择的
	{

	}
	else
	{
		pDisplayTable->SearchDisplayTable(m_pQueryFilter,VARIANT_FALSE,&m_pCursor);
	}
	EnableVirtualMode (CallbackGrid,(LPARAM)this);
	
	FillTable();	

	SetVirtualRowsEx();

}

BOOL CTableGridCtrl::InitColumns()
{
	DeleteAllColumns();

	CComQIPtr<ITableFields> pTableFields = m_pTable;
	if (pTableFields == NULL) return FALSE;

	long nFieldCount = 0;
	pTableFields->get_FieldCount(&nFieldCount);
	for (long i=0; i<nFieldCount; i++)
	{
		CComQIPtr<IField> ipField;
		BSTR bstrName;
		pTableFields->get_Field(i,&ipField);
		ipField->get_Name(&bstrName);
		CString strName = bstrName;
		if (strName.Find(m_strOIDFieldName) >= 0 && m_strOIDFieldName.IsEmpty() != true)
		{
			InsertColumn(i,bstrName,0);
			continue;
		}else if (strName.Find(m_strShapeFieldName) >= 0 && m_strShapeFieldName.IsEmpty() != true)
		{
			InsertColumn(i,bstrName,0);
			continue;
		}else if (ipField == m_spAreaField || ipField == m_spLenthField)
		{
			InsertColumn(i,bstrName,0);
			continue;
		}
		InsertColumn(i,bstrName,100);
	}
	m_List.SetColumnCount(nFieldCount);

  //edit by lcc 2016.12.17 添加根据配置隐藏和显示别名
	USES_CONVERSION;
 	//打开配置文件找到图层属性
 	TCHAR szAppFullPath[MAX_PATH];
 	DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(CHAR);
 	GetModuleFileName(NULL,szAppFullPath,dBufferSize);
 	CString strAppPath(szAppFullPath);
 	strAppPath = strAppPath.Mid(0,strAppPath.ReverseFind('\\'));
 	CString strConfigFile = strAppPath + CONFIGPATH;
 
 	TiXmlDocument XmlDoc;
 	TiXmlElement *RootElement = NULL;
 	if (XmlDoc.LoadFile(W2A(strConfigFile.AllocSysString())) == false) return FALSE;
 	RootElement = XmlDoc.FirstChildElement();
 
 	TiXmlElement *DataManageRuleElement = RootElement->FirstChildElement("DataManageRule");
 	TiXmlElement *BuildingRulesElement = DataManageRuleElement->FirstChildElement("BuildingRules");
 	//现在配置里第一个就是规划库，直接取第一个就行
 	TiXmlElement *BuildingRuleElement = BuildingRulesElement->FirstChildElement("BuildingRule");
 	TiXmlElement *LayersElement = BuildingRuleElement->FirstChildElement();
 	TiXmlElement *LayerElement = LayersElement->FirstChildElement();
 	TiXmlElement *FieldsElement = NULL;
 	TiXmlElement *FieldElement = NULL;
 	while(LayersElement != NULL)
 	{
 		TiXmlAttribute *pAttr = LayerElement->FirstAttribute();
 		CString strLayerName = A2W(pAttr->Value());
 		if (strLayerName.Compare(m_strFtClassName) == 0) break;
 		LayerElement = LayerElement->NextSiblingElement();
 	}
 	if (LayerElement != NULL)
 	{
 		FieldsElement = LayerElement->FirstChildElement();
 		FieldElement = FieldsElement->FirstChildElement();
 	}
 
 	CMapStringToString strmapHidden;
 	CMapStringToString strmapName;
 	while(FieldElement != NULL)
 	{
 		CString strFieldName;
 		CString strFieldAliasName;
 		CString strHidden;
 		utf8ToUnicode(FieldElement->Attribute("FieldName"),&strFieldName);
 		utf8ToUnicode(FieldElement->Attribute("AliasName"),&strFieldAliasName);
 		utf8ToUnicode(FieldElement->Attribute("Hidden"),&strHidden);
 		if (strFieldName.IsEmpty() != true && strFieldAliasName.IsEmpty() != true)
 		{
 			strmapName.SetAt(strFieldName,strFieldAliasName);
 		}
 		if (strFieldName.IsEmpty() != true && strHidden.IsEmpty() != true)
 		{
 			strmapHidden.SetAt(strFieldName,strHidden);
 		}
 		FieldElement = FieldElement->NextSiblingElement();
 	}
 
 	int nCount = GetColumnCount();
 	for (int i = 0; i < nCount; i++)
 	{
 		CString strColunmAliasName;
 		CString strIsHidden;
 		CString strColunmName = GetColumnName(i);
 		strmapName.Lookup(strColunmName,strColunmAliasName);
 		if (strColunmAliasName.IsEmpty() != true)
 		{
 			SetColumnName(i,strColunmAliasName);
 		}
 		strmapHidden.Lookup(strColunmName,strIsHidden);
 		if (strIsHidden.Compare(_T("true")) == 0)
 		{
 			SetColumnWidth(i,0);
 		}
 	}
 	//edit by lcc 2016.12.17 添加根据配置隐藏和显示别名 end
	
	return TRUE;
}

BOOL CTableGridCtrl::FillTable(BOOL bFillAll /*= FALSE*/)
{
	BOOL bResult = FALSE;
	long nCount = 0;
	if (m_pCursor == NULL) return bResult;

	std::map<long,long>idmaps;
	IFeatureSelectionPtr pFeatureSelection = m_pTable;
	if ( pFeatureSelection != NULL )
	{
		ISelectionSetPtr lpSelectionSet = NULL;
		pFeatureSelection->get_SelectionSet(&lpSelectionSet);
		IEnumIDsPtr pEnumids;
		lpSelectionSet->get_IDs(&pEnumids);
		long id =0;
		while(pEnumids->Next(&id) == S_OK)
		{
			idmaps[id] = id;
		}
	}

	//2016-12-13 dengdt 判断要素类型
	IFeatureLayerPtr ipFtLayer(m_pTable);
	IFeatureClassPtr ipFtClss;
	ipFtLayer->get_FeatureClass(&ipFtClss);
	esriGeometryType enumGeoType = esriGeometryNull;
	if (ipFtClss != NULL)
		ipFtClss->get_ShapeType(&enumGeoType);
	
	IRowPtr pRow = NULL;
	ITableFieldsPtr pTableFields(m_pTable);
	long nFieldCount = 0;
	pTableFields->get_FieldCount(&nFieldCount);
	int nCountOld = GetRowCount();
	IFieldInfoPtr pFieldInfo = NULL;
	INumberFormatPtr pNumberFormat = NULL;
	while ((bFillAll || nCount++ < FILL_ROW_COUNT) && m_pCursor->NextRow(&pRow) == S_OK)
	{
		ROWDATATYPE row;
		long oid = 0;
		pRow->get_OID(&oid);
		row.objectid = oid;
		row.bSelected = false;
		for (long i=0; i<nFieldCount; i++)
		{
			_variant_t var;
			pRow->get_Value(i,&var);
			pTableFields->get_FieldInfo(i,&pFieldInfo);
			BSTR bstrValue;

			pFieldInfo->get_NumberFormat(&pNumberFormat);
			if (pNumberFormat != NULL)
			{
				pFieldInfo->get_AsString(var,&bstrValue);
			}
			else
			{
				if (var.vt != VT_NULL && var.vt == VT_EMPTY)
				{
					pNumberFormat->ValueToString((double)var,&bstrValue);
				}
				//2016-12-13 dengdt 解决打开属性表不输出要素类型
				//Geometry对应VT_UNKNOW
				else if (var.vt == VT_UNKNOWN)
				{
					switch(enumGeoType)
					{
					case esriGeometryPoint:
						bstrValue = _bstr_t(_T("Point"));
						break;
					case esriGeometryPolyline:
						bstrValue = _bstr_t(_T("Polyline"));
						break;
					case esriGeometryPolygon:
						bstrValue = _bstr_t(_T("Ploygon"));
						break;
					case esriGeometryMultipoint:
						bstrValue = _bstr_t(_T("Multipoint"));
						break;
					default:
						bstrValue = _bstr_t(_T(""));
						break;
					}
				}
				else
				{
					pFieldInfo->get_AsString(var,&bstrValue);
				}
			}

			DWORD dwTextLen = lstrlen((LPCTSTR)_bstr_t(bstrValue));
			TCHAR* pszTextBuffer = new TCHAR [dwTextLen + 2];
			memset (pszTextBuffer, 0, (dwTextLen+2) * sizeof(TCHAR));
			_tcscpy(pszTextBuffer,(LPCTSTR)_bstr_t(bstrValue));
			row.FieldValue[i] =  pszTextBuffer;
			::SysFreeString(bstrValue);
		}
		std::map<long,long>::iterator iter = idmaps.find(oid);
		if ( iter != idmaps.end() )  row.bSelected = TRUE;
		m_List.AddTail(row);
	}
	CString strInfo;
	int nRowCount = GetRowCount();
	CString strCount;
	strCount.Format(_T("%d"),nRowCount);
	int nLenth = strCount.GetLength();
	EnableRowHeader(TRUE,-1,nLenth*10);

	if (pRow != NULL)   //说明还没有循环到底
	{
		m_bIsFillAllRecords = FALSE;
		strInfo.Format(_T("记录(共*%d条)"),GetRowCount());
		//strInfo.Format(_T("记录(%d个被选中，共*%d条)"),GetSelectedRowCount(),GetRowCount());
	}
	else
	{
		m_bIsFillAllRecords = TRUE;
		strInfo.Format(_T("记录(共%d条)"),GetRowCount());
		//strInfo.Format(_T("记录(%d个被选中，共%d条)"),GetSelectedRowCount(),GetRowCount());
	}
	//CWnd *pParent = this->GetParent();
	if (m_hWnd != NULL)
	{
		//::SetDlgItemText(GetParent()->m_hWnd,IDC_RESTEXT,strInfo);
		CLPGridDockTabWnd* pParent = (CLPGridDockTabWnd*)GetParent();
		pParent->SetResText(strInfo);
	}
	if (nCountOld < GetRowCount())
	{
		m_List.BuildArray();
		IFeatureSelectionPtr lpFeatureSelection(m_pTable);
		if (m_bOnlyShowSelected) m_List.BuildSelectedArray(lpFeatureSelection);
		SetVirtualRowsEx();
	}



	idmaps.clear();
	return TRUE;
}

BOOL CTableGridCtrl::FillTable( ICursorPtr pCursor )
{
	BOOL bResult = FALSE;

	if (pCursor == NULL) return bResult;
	IRowPtr pRow = NULL;
	IFieldsPtr pFields = NULL;
	m_pTable->get_Fields(&pFields);
	long nFieldCount = 0;
	pFields->get_FieldCount(&nFieldCount);
	//int nCountOld = GetRowCount();
	while (pCursor->NextRow(&pRow) == S_OK)
	{
		ROWDATATYPE row;
		long oid = 0;
		pRow->get_OID(&oid);
		row.objectid = oid;
		row.bSelected = false;
		for (long i=0; i<nFieldCount; i++)
		{
			_variant_t var;
			pRow->get_Value(i,&var);
			if (var.vt != VT_EMPTY && var.vt != VT_NULL && var.vt != VT_UNKNOWN)
			{
				var.ChangeType(VT_BSTR,&var);

				DWORD dwTextLen = lstrlen((LPCTSTR)_bstr_t(var.bstrVal));
				TCHAR* pszTextBuffer = new TCHAR [dwTextLen + 2];
				memset (pszTextBuffer, 0, (dwTextLen+2) * sizeof(TCHAR));
				_tcscpy(pszTextBuffer,(LPCTSTR)_bstr_t(var.bstrVal));

				row.FieldValue[i] =  pszTextBuffer;
			}
			else 
				row.FieldValue[i] = _T("");
		}
		m_List.AddTail(row);
	}

	m_List.BuildArray();
	SetVirtualRowsEx();

	CString strInfo;
	if (pRow != NULL)   //说明还没有循环到底
	{
		m_bIsFillAllRecords = FALSE;
		strInfo.Format(_T("记录(共*%d条)"),GetRowCount());
		//strInfo.Format(_T("记录(%d个被选中，共*%d条)"),GetSelectedRowCount(),GetRowCount());
	}
	else
	{
		m_bIsFillAllRecords = TRUE;
		strInfo.Format(_T("记录(共%d条)"),GetRowCount());
		//strInfo.Format(_T("记录(%d个被选中，共%d条)"),GetSelectedRowCount(),GetRowCount());
	}
	CWnd *pParent = GetParent();
	if (pParent->m_hWnd != NULL)
	{
		//::SetDlgItemText(GetParent()->m_hWnd,IDC_RESTEXT,strInfo);
		CLPGridDockTabWnd* pParent = (CLPGridDockTabWnd*)GetParent();
		pParent->SetResText(strInfo);
	}

	return TRUE;
}


void CTableGridCtrl::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{

	static CTime t;
	//获取滚动条的位置
	int nMin,nMax;
	pScrollBar->GetScrollRange(&nMin,&nMax);
	float fCurPos = (float)pScrollBar->GetScrollPos();
	if (nMax > 0 && fCurPos > 0) 
	{
		if (fCurPos / nMax > 0.98)
		//if (fCurPos  ==  nMax)
		{
			CTime tNow;
			tNow = CTime::GetCurrentTime();
			int nNowSecend = tNow.GetSecond();
			int nSecend = t.GetSecond();
			//if (nNowSecend - nSecend >= 1 || nNowSecend - nSecend <= -1)
			{
				t = tNow;
				FillTable();
				//SetVirtualRowsEx();
				//pScrollBar->SetScrollPos((int)fCurPos,TRUE);
			}
		}
	}
	CBCGPGridCtrl::OnVScroll(nSBCode,nPos,pScrollBar);
	::Sleep(20);

}


void CTableGridCtrl::Sort(BOOL bAscending /*= TRUE*/, BOOL bAdd /*= FALSE*/ )
{
	if (m_pTable != NULL && m_nCurrentColumn > -1)
	{
		//如果没有加载完全，先加载数据
		FillTable(TRUE);

		IFieldsPtr pFields;
		m_pTable->get_Fields(&pFields);
		IFieldPtr pField;
		pFields->get_Field(m_nCurrentColumn,&pField);
		esriFieldType fieldType;
		pField->get_Type(&fieldType);
		m_List.Sort(m_nCurrentColumn,fieldType,bAscending,bAdd);

		SetVirtualRowsEx();

		//设置Mark
		EnableMarkSortedColumn(TRUE,FALSE);
		SetSortColumn(m_nCurrentColumn,bAscending);
		EnableMarkSortedColumn(FALSE,TRUE);
	}
}

//从1开始
void CTableGridCtrl::MoveToIndex( int nIndex )
{
	int nRowCount = m_bOnlyShowSelected ? GetSelectedRowCount() : GetRowCount();
	nIndex--;

	if (nIndex < 0) return;

	if (nRowCount < nIndex)
	{
		FillTable();
		nRowCount = m_bOnlyShowSelected ? GetSelectedRowCount() : GetRowCount();
	}
	if (nRowCount > nIndex)
	{
		CBCGPGridItemID id;
		GetCurSel(id);
		if (!id.IsNull())
		{
			id.m_nRow = nIndex;
		}
		else
		{
			id.m_nColumn = 0;
			id.m_nRow = nIndex;
		}
		CBCGPGridRow* pGridRow = GetRow(nIndex);
		if (pGridRow)
		{
			EnsureVisible(pGridRow);
			SetCurSel(id);
		}
	}
}

int CTableGridCtrl::GetRowCount()
{
	return m_List.GetCount();
}

void CTableGridCtrl::OnRButtonUp( UINT nFlags, CPoint point )
{
	CBCGPGridCtrl::OnRButtonUp(nFlags,point);

	////弹出右键菜单
// 	if (CBCGPPopupMenu::GetActiveMenu() != NULL)
// 	{
// 		CBCGPPopupMenu::GetActiveMenu()->SendMessage(WM_CLOSE);
// 	}
// 
// 	//列头的右键菜单
// 	int nCol =m_Columns.HitTestColumn(point);
// 	m_nCurrentColumn = nCol;
// 	if (nCol > -1)
// 	{
// 		//SelectColumn(nCol);
// 		//m_Columns.SetColumnImage(nCol,1);
// 		PopupColumnMenu(nCol);
// 	}
// 
// 	//行头的右键菜单
// 	CBCGPGridItemID itemId;
// 	CBCGPGridItem* pItem = NULL;
// 	CBCGPGridRow::ClickArea pClickArea;
// 	HitTest(point, itemId, pItem, &pClickArea);
// 	if (pClickArea == CBCGPGridRow::ClickName &&  !itemId.IsNull())
// 	{
// 		PopupRowMenu(0);
// 	}
// 
// 	//2013-08-02 chaizb
// 	//CRect rectRowHeader;
// 	//CBCGPGridRow* pGridRow = HitTestRowHeader (point, rectRowHeader);
//  //   if ( pGridRow != NULL )
// 	//{
// 	//	PopupRowMenu(0);
// 	//}
// 
// 	//未启动编辑时，允许COPY操作到剪切板，已经启动编辑的则利用windows自带的右键菜单 2012-06-15
// 	if ( !IsBeingEdit() && pClickArea == CBCGPGridRow::ClickValue && !itemId.IsNull()  )
// 	{
// 		CMenu menu;
// 		menu.CreatePopupMenu();
// 		menu.AppendMenu(MF_STRING | MF_ENABLED,ID_CELL_COPY,_T("拷贝到剪切板"));
// 
// 		CPoint point;
// 		GetCursorPos(&point);
// 		if (CBCGPPopupMenu::GetActiveMenu() == NULL)
// 		{
// 			CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
// 			pPopupMenu->Create(GetParent(), point.x, point.y, menu.m_hMenu, FALSE, TRUE);
// 		}
// 	}
}


int CTableGridCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//InitControls();

	return 0;
}


void CTableGridCtrl::PopupColumnMenu(int nColumn)
{
	//弹出菜单...........................
// 	if (nColumn >=0)
// 	{
// 		//构造菜单....
// 		CMenu menu;
// 		menu.CreatePopupMenu();
// 
// 		DWORD dwStyle   = MF_STRING/*MF_ENABLED|MF_OWNERDRAW*/;
// 
// 		DWORD dwStyleEx = IsColumnCanSort(nColumn) ? MF_ENABLED : MF_GRAYED | MF_DISABLED;
// 		menu.AppendMenu(dwStyle | dwStyleEx,ID_Ascending,_T("升序")/*,CBitmap::FromHandle(hBitmap)*/);
// 		menu.AppendMenu(dwStyle | dwStyleEx,ID_Desending,_T("降序")/*,CBitmap::FromHandle(hBitmap)*/);
// 
// 		dwStyleEx = IsColumnCanStat(nColumn) ? MF_ENABLED : MF_DISABLED | MF_GRAYED;
// 		menu.AppendMenu(dwStyle | dwStyleEx,ID_Summarize,_T("统计...")/*,CBitmap::FromHandle(hBitmap)*/);
// 
// 		menu.AppendMenu(MF_SEPARATOR);
// 
// 		dwStyleEx = IsColumnReadOnly(nColumn) || IsReadOnly() ? MF_GRAYED | MF_GRAYED : MF_ENABLED;
// 		menu.AppendMenu(dwStyle |dwStyleEx ,ID_FieldCalculator,_T("字段计算...")/*,CBitmap::FromHandle(hBitmap)*/);
// 		menu.AppendMenu(dwStyle |dwStyleEx ,ID_CalculatorGeometry,_T("计算几何..."));
// 
// 		menu.AppendMenu(MF_SEPARATOR);
// 		menu.AppendMenu(dwStyle ,ID_TurnOffField,_T("关闭字段"));
// 
// 		dwStyleEx = !IsBeingEdit() && !IsColumnReadOnly(nColumn) /*&& IsReadOnly()*/ ? MF_ENABLED : MF_DISABLED | MF_GRAYED;
// 		menu.AppendMenu(dwStyleEx,ID_DeleteField,_T("删除字段"));
// 
// 		menu.AppendMenu(MF_SEPARATOR);
// 
// 		menu.AppendMenu(dwStyle,ID_FieldProperty,_T("字段属性..."));	
// 
// 		CPoint point;
// 		GetCursorPos(&point);
// 		if (CBCGPPopupMenu::GetActiveMenu() == NULL)
// 		{
// 			CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;	//Debug点击对话框外面时崩,Release不崩
// 			pPopupMenu->Create(GetParent(), point.x, point.y, menu.m_hMenu, FALSE, TRUE);
// 		}
		
		//CBCGPContextMenuManager* pContextMenuManager = ::GetWorkspace()->GetContextMenuManager();
		//UINT nID = pContextMenuManager->TrackPopupMenu(menu.GetSafeHmenu(),
		//	point.x, point.y, this);

		//::SendMessage(GetParent()->m_hWnd,WM_EXECUTE_COMMAND,(WPARAM)nID,0);

//	}
}

void CTableGridCtrl::OnLButtonDown( UINT nFlags, CPoint point )
{
	CBCGPGridCtrl::OnLButtonDown(nFlags,point);
	CLPGridDockTabWnd* pParent = (CLPGridDockTabWnd*)GetParent();
	pParent->OnSelectColumn(0,0);
}


void CTableGridCtrl::PopupRowMenu( int nRow )
{
// 	CMenu menu;
// 	menu.CreatePopupMenu();
// 
// 	//int nRowCount = m_pStaticListCtrl->GetItemCount();
// 	DWORD dwStyle		= MF_STRING;
// 	//防止在空行执行右键菜单项使程序崩溃
// 	DWORD dwStyleNull   = MF_STRING /*| ( m_nCurrentRow>0 ? MF_ENABLED : MF_GRAYED )*/ ;
// 	//HBITMAP hBitmap = ::LoadBitmap(_Module.m_hInst, MAKEINTRESOURCE(IDB_Flash));
// 	menu.AppendMenu(dwStyleNull,ID_Flash,_T("闪烁")/*,CBitmap::FromHandle(hBitmap)*/);
// 
// 	//hBitmap = ::LoadBitmap(_Module.m_hInst, MAKEINTRESOURCE(IDB_ZoomTo));
// 	menu.AppendMenu(dwStyleNull,ID_ScaleToSelected,_T("缩放到当前选择")/*,CBitmap::FromHandle(hBitmap)*/);
// 
// 	//hBitmap = ::LoadBitmap(_Module.m_hInst, MAKEINTRESOURCE(IDB_PanTo));
// 	menu.AppendMenu(dwStyleNull,ID_PanTo,_T("移动到当前选择")/*,CBitmap::FromHandle(hBitmap)*/);
// 
// 	//hBitmap = ::LoadBitmap(_Module.m_hInst, MAKEINTRESOURCE(IDB_Indentify));
// 	menu.AppendMenu(dwStyleNull,ID_Indentify,_T("查看要素属性")/*,CBitmap::FromHandle(hBitmap)*/);
// 
// 	//hBitmap = ::LoadBitmap(_Module.m_hInst, MAKEINTRESOURCE(IDB_Select));
// 	menu.AppendMenu(dwStyle,ID_SelectOrUnSelect,_T("选择/反选")/*,CBitmap::FromHandle(hBitmap)*/);
// 
// 	menu.AppendMenu(MF_SEPARATOR);
// 
// 	//long lSelCount = m_pStaticListCtrl->CalculateSelectedRow();
// 	dwStyle &=(GetSelectedRowCount() >0 ? MF_ENABLED : MF_GRAYED);
// 
// 	//hBitmap = ::LoadBitmap(_Module.m_hInst, MAKEINTRESOURCE(IDB_ZoomToSelected));
// 	menu.AppendMenu(dwStyle,ID_ZoomtoSelected,_T("缩放到选择集")/*,CBitmap::FromHandle(hBitmap)*/);
// 
// 	//hBitmap = ::LoadBitmap(_Module.m_hInst, MAKEINTRESOURCE(IDB_ClearSelection));
// 	menu.AppendMenu(dwStyle,ID_ClearSelect,_T("清除选择")/*,CBitmap::FromHandle(hBitmap)*/);
// 
// 	//hBitmap = ::LoadBitmap(_Module.m_hInst, MAKEINTRESOURCE(IDB_Delete));
// 	menu.AppendMenu(dwStyle | (IsBeingEdit() ? MF_ENABLED : MF_GRAYED ),ID_DeleteSelect,_T("删除选择")/*,CBitmap::FromHandle(hBitmap)*/);
// 
// 	CPoint point;
// 	GetCursorPos(&point);
// 	if (CBCGPPopupMenu::GetActiveMenu() == NULL)
// 	{
// 		CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;	//Debug点击对话框外面时崩,Release不崩
// 		pPopupMenu->Create(GetParent(), point.x, point.y, menu.m_hMenu, FALSE, TRUE);
// 	}
}

int CTableGridCtrl::GetCurrentColumn()
{
	return m_nCurrentColumn;
}

BOOL CTableGridCtrl::IsColumnReadOnly(int nColumn)
{
	//if (!IsBeingEdit()) return TRUE;

	BOOL bRusult = FALSE;
	if (m_pFieldsClone != NULL)
	{
		//返回该表的base表，判断字段是否为base表的字段
		ITablePtr pTable;
		((IAttributeTablePtr)m_pTable)->get_AttributeTable(&pTable);
		IFieldsPtr pFields;
		pTable->get_Fields(&pFields);
		long nFieldCount;
		pFields->get_FieldCount(&nFieldCount);
		//如果nColumn大于nFieldCount，说明不是base表的
		if (nColumn >=  nFieldCount) return TRUE;

		IFieldPtr pField = NULL;
		m_pFieldsClone->get_Field(nColumn,&pField);
		if (pField != NULL)
		{
			esriFieldType fieldType;
			pField->get_Type(&fieldType);
			switch (fieldType)
			{
			case esriFieldTypeOID:
			case esriFieldTypeGeometry:
			case esriFieldTypeBlob:
			case esriFieldTypeRaster:
			case esriFieldTypeXML:
				{
					return TRUE;
				}
				break;
			default:
				break;
			}

			//如果字段类型为shape_length,shape_area也是只读的
			BSTR name;
			pField->get_Name(&name);
			CString strName = (LPCTSTR)_bstr_t(name);
			::SysFreeString(name);
			if (strName.CompareNoCase(_T("shape_length")) == 0 || 
				strName.CompareNoCase(_T("shape_area")) == 0)
			{
				return TRUE;
			}
		}
	}


	return bRusult;
}

IFieldsPtr CTableGridCtrl::GetOldFields()
{
	return m_pFieldsClone;
}

BOOL CTableGridCtrl::IsColumnCanSort( int nColumn )
{
	BOOL bRusult = TRUE;
	if (m_pFieldsClone != NULL)
	{
		IFieldPtr pField = NULL;
		m_pFieldsClone->get_Field(nColumn,&pField);
		if (pField != NULL)
		{
			esriFieldType fieldType;
			pField->get_Type(&fieldType);
			switch (fieldType)
			{
			case esriFieldTypeGeometry:
			case esriFieldTypeBlob:
			case esriFieldTypeRaster:
			case esriFieldTypeXML:
				{
					return FALSE;
				}
				break;
			default:
				break;
			}
		}
	}

	return bRusult;
}

BOOL CTableGridCtrl::IsColumnCanStat( int nColumn )
{
	BOOL bRusult = FALSE;
	if (m_pFieldsClone != NULL)
	{
		IFieldPtr pField = NULL;
		m_pFieldsClone->get_Field(nColumn,&pField);
		if (pField != NULL)
		{
			esriFieldType fieldType;
			pField->get_Type(&fieldType);
			switch (fieldType)
			{
			case esriFieldTypeSmallInteger:
			case esriFieldTypeInteger:
			case esriFieldTypeSingle:
			case esriFieldTypeDouble:
				{
					return TRUE;
				}
				break;
			default:
				break;
			}
		}
	}
	return bRusult;
}

void CTableGridCtrl::SetSelectedRowByOID( long nOid, bool bSelected )
{
	int nCount = m_List.GetCount();
	for (int i=0; i<nCount; i++)
	{
		if (m_List.GetAt(i) != NULL && m_List.GetAt(i)->objectid == nOid)
		{
			m_List.GetAt(i)->bSelected = bSelected;
			break;
		}
	}
}

void CTableGridCtrl::ClearAllSelectedRow(BOOL bClearFeatureSlectionSet)
{
	try
	{
		int nCount = m_bOnlyShowSelected ? m_List.GetSelectedCount() : m_List.GetCount();
		for (int i=0; i<nCount; i++)
		{
			ROWDATATYPE *pRow = m_List.GetSortAt(i);
			if ( pRow != NULL )  pRow->bSelected = false;
		}

		//清除选择集,这样不会触发选择事件
		if (!bClearFeatureSlectionSet)  return;
		ITableSelectionPtr lpTableSelection(m_pTable);

		ISelectionSetPtr lpSelectionSet = NULL;
		if (lpTableSelection != NULL)
		{
			//lpTableSelection->SelectRows(NULL,esriSelectionResultXOR,0);
			lpTableSelection->get_SelectionSet(&lpSelectionSet);

			if (lpSelectionSet != NULL)
			{
				IEnumIDsPtr pEnumIDs;
				long nCount = 0;
				lpSelectionSet->get_Count(&nCount);
				if (nCount > 0)
				{
					long *pIDList = new long[nCount];
					lpSelectionSet->get_IDs(&pEnumIDs);
					pEnumIDs->Reset();
					long nID;
					int nIndex = 0;
					while (pEnumIDs->Next(&nID) == S_OK)
					{
						pIDList[nIndex++] = nID;
					}
					lpSelectionSet->RemoveList(nCount,pIDList);
					delete [] pIDList;
				}
			}
		}
		ResetLastSelected();
	}
	catch(...)
	{
	}
}

void CTableGridCtrl::OnLButtonUp( UINT nFlags, CPoint point )
{
	CBCGPGridCtrl::OnLButtonUp(nFlags,point);
}

void CTableGridCtrl::OnLButtonDblClk( UINT nFlags, CPoint point )
{
 
 	CBCGPGridCtrl::OnLButtonDblClk(nFlags,point);

	int nColumnHit = m_Columns.HitTestColumn (point, FALSE, 2);
	if (nColumnHit >= 0)
	{
		HCURSOR  hCursor = SetCursor(LoadCursor(NULL,IDC_WAIT));

		m_nCurrentColumn = nColumnHit;

		Sort(!m_bSortAscending,FALSE);

		SetCursor(hCursor);
	}else
	{
		CLPGridDockTabWnd* pParent = (CLPGridDockTabWnd*)GetParent();
		pParent->OnDBClickColumn(0,0);	
	}
 

// 	CTableGridDlg* pDlg = (CTableGridDlg*)GetParent();
}

void CTableGridCtrl::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	//只读的列需要过滤
	CBCGPGridItemID id;
	GetCurSel(id);
	if (!id.IsNull())
	{
		if (!IsBeingEdit() || IsColumnReadOnly(id.m_nColumn)) return;
	}
	CBCGPGridCtrl::OnChar(nChar,nRepCnt,nFlags);
}

int CTableGridCtrl::GetSelectedRowCount()
{
	if ( m_bOnlyShowSelected ) //add chaizb 2010-05-18
	{
		IFeatureSelectionPtr lpFeatureSelection = m_pTable;
		long nIDCount =0;
		if ( lpFeatureSelection != NULL )
		{
			ISelectionSetPtr lpSelectionSet = NULL;
			lpFeatureSelection->get_SelectionSet(&lpSelectionSet);
			lpSelectionSet->get_Count(&nIDCount);
		}
		return nIDCount == 0 ? m_List.GetSelectedCount() : nIDCount;
	}
	else //end add
	   return m_List.GetSelectedCount();
}

//void CTableGridCtrl::SetRowCount(int nCount)
//{
//	m_nTotalCount = nCount;
//}

BOOL CTableGridCtrl::EndEditItem( BOOL bUpdateData /*= TRUE*/ )
{
	CBCGPGridCtrl::EndEditItem(bUpdateData);

	if (bUpdateData)
	{
		CBCGPGridItemID id;
		GetCurSel(id);
		if (!id.IsNull())
		{
			UpdatetValueByID(id);
		}
	}
	return TRUE;

}

BOOL CTableGridCtrl::EditItem( CBCGPGridRow* pItem, LPPOINT lptClick /*= NULL*/ )
{
	CBCGPGridItemID id;
	GetCurSel(id);
	if (!id.IsNull())
	{
		if (IsBeingEdit() && !IsColumnReadOnly(id.m_nColumn))
			CBCGPGridCtrl::EditItem(pItem,lptClick);
	}
	return TRUE;
}

BOOL CTableGridCtrl::UpdatetValueByID( CBCGPGridItemID id )
{
	if (id.m_nColumn < 0 || id.m_nRow <0) return FALSE;
	//判断是否可编辑
	IGSEditorPtr lpEditor  = m_pFrameworkCOM->GetEditor();
	long bBeingEdited = 0;

	//有可能存在表的join，所以用AtrributeTable;
	IAttributeTablePtr pAttributeTable(m_pTable);
	if (pAttributeTable == NULL ) return FALSE;
	ITablePtr pTable = NULL;
	pAttributeTable->get_AttributeTable(&pTable);
	if (pTable == NULL) return FALSE;

	lpEditor->IsBeingEditedByDataset((IDatasetPtr)pTable,&bBeingEdited);
	if (bBeingEdited == 0) return FALSE;

	CBCGPGridRow* pSel = GetRow(id.m_nRow);
	if (pSel != NULL)
	{
		_variant_t var = pSel->GetItem(id.m_nColumn)->GetValue();
		
		if (var.vt == VT_EMPTY || var.vt == VT_NULL || var.vt == VT_UNKNOWN)
		{
			return FALSE;
		}

		//如果是一样的，就不用更新了
		var.ChangeType(VT_BSTR,&var);

		//防止数据被移除以后，再取链表中的数据
		if (GetRowCount() < id.m_nRow + 1) return FALSE;

		CString strValue = (LPCTSTR)_bstr_t(var.bstrVal);
		if (strValue.Compare(m_List.GetSortAt(id.m_nRow)->FieldValue[id.m_nColumn]) == 0)
			return FALSE;

		//验证值是否有效
		IFieldsPtr pFields = NULL;
		pTable->get_Fields(&pFields);
		IFieldPtr pField = NULL;
		pFields->get_Field(id.m_nColumn,&pField);
		if (pField == NULL) return FALSE;

		//为了验证，转换类型
		_variant_t varCheck = var;
		ChangeValueTypeByFieldType(pField,varCheck);
		VARIANT_BOOL bValidValue;
		((IField2Ptr)pField)->CheckValue(varCheck.GetVARIANT(),&bValidValue);
		
		if (!bValidValue) 
		{
			CString strMsg(_T("您输入的值无效，请输入合法的值。"));
			MessageBox(strMsg,_T("合法性检查"),MB_OK);
			//还原值
			pSel->GetItem(id.m_nColumn)->SetValue(m_List.GetSortAt(id.m_nRow)->FieldValue[id.m_nColumn]);
			return FALSE;
		}

		IRowPtr pRow = NULL;
		pTable->GetRow(m_List.GetSortAt(id.m_nRow)->objectid,&pRow);
		if (pRow != NULL)
		{
			//更新数据
			long bSuccess = 0;
			lpEditor->StartEditOperationByDataset((IDatasetPtr)pTable,&bSuccess);
			
			HRESULT hr;
		    hr = pRow->put_Value(id.m_nColumn,varCheck);
			if (SUCCEEDED(hr))
			{
				hr = pRow->Store();
				//更新链表
				DWORD dwTextLen = lstrlen((LPCTSTR)_bstr_t(var.bstrVal));
				TCHAR* pszTextBuffer = new TCHAR [dwTextLen + 2];
				memset (pszTextBuffer, 0, (dwTextLen+2) * sizeof(TCHAR));
				_tcscpy(pszTextBuffer,(LPCTSTR)_bstr_t(var.bstrVal));

				m_List.GetSortAt(id.m_nRow)->FieldValue[id.m_nColumn] = pszTextBuffer;//(TCHAR*)_bstr_t(var.bstrVal);
			}
			else
			{
				CString strMsg(_T("您输入的值无效，请输入合法的值。"));
				MessageBox(strMsg,_T("合法性检查"),MB_OK);
				//还原值
				pSel->GetItem(id.m_nColumn)->SetValue(m_List.GetSortAt(id.m_nRow)->FieldValue[id.m_nColumn]);
				lpEditor->AbortEditOperation(&bSuccess);
			}
			
			lpEditor->StopEditOperationByDataset((IDatasetPtr)pTable,TRUE,&bSuccess);
		}
	}
	return TRUE;
}

// 根据字段的类型将变体类型改为相对应得类型
void CTableGridCtrl::ChangeValueTypeByFieldType( IField* ipField, _variant_t &varValue )
{
	try
	{
		long varType;
		ipField->get_VarType(&varType);
		switch (varType)
		{
		case 2:								// VT_I2
			varValue.ChangeType(VT_I2);
			break;
		case 3:								// VT_I4
			varValue.ChangeType(VT_I4);
			break;
		case 4:								// VT_R4
			varValue.ChangeType(VT_R4);
			break;
		case 5:								// VT_R8
			varValue.ChangeType(VT_R8);
			break;
		case 6:								// VT_CY
			varValue.ChangeType(VT_CY);
			break;
		case 7:								// VT_DATE
			varValue.ChangeType(VT_DATE);
			break;
		case 8:								// VT_BSTR
			varValue.ChangeType(VT_BSTR);
			break;
		case 11:							// VT_BOOL
			varValue.ChangeType(VT_BOOL);
			break;
		}
	}
	catch(...)
	{

	}
}

BOOL CTableGridCtrl::IsBeingEdit()
{
	if (m_pFrameworkCOM == NULL) return FALSE;
	IGSEditorPtr lpEditor  = m_pFrameworkCOM->GetEditor();
	long bIsEditable;
	lpEditor->IsBeingEditedByDataset((IDatasetPtr)m_pTable,&bIsEditable);
	return bIsEditable == 0 ? FALSE : TRUE;	
}

void CTableGridCtrl::FilterSelectedItem( BOOL bFilterSelected )
{
	m_bOnlyShowSelected = bFilterSelected;
	if (bFilterSelected)
	{
		IFeatureSelectionPtr lpFeatureSelection(m_pTable);
		m_List.BuildSelectedArray(lpFeatureSelection);
	}
	EnableFilter(pfnFilterCallback,(LPARAM) bFilterSelected);
	SetVirtualRowsEx();
}

BOOL CTableGridCtrl::IsOnlyShowSelected()
{
	return m_bOnlyShowSelected;
}

void CTableGridCtrl::SetVirtualRowsEx()
{
	m_List.ShowSelectedOnly(m_bOnlyShowSelected);
	IFeatureSelectionPtr lpFeatureSelection(m_pTable);
	if (m_bOnlyShowSelected) m_List.BuildSelectedArray(lpFeatureSelection);
	if (m_pTable != NULL) SetVirtualRows(m_bOnlyShowSelected ? GetSelectedRowCount() : GetRowCount());
}

void CTableGridCtrl::InitControls(ITablePtr pTable , IQueryFilterPtr pQueryFilter)
{
	//在外面调用的时候才传pTable
	if (pTable != NULL)
	{
		m_pTable = pTable;
		m_pQueryFilter = pQueryFilter;
	}
	if (m_pTable != NULL)
	{
		//克隆字段信息
		IFieldsPtr pFields = NULL;
		m_pTable->get_Fields(&pFields);
		IClonePtr pClone(pFields), pCloned;
		pClone->Clone(&pCloned);
		m_pFieldsClone = pCloned;

		m_List.RemoveAll();

		//初始化
	//	EnableRowHeader (TRUE);
	//	EnableLineNumbers ();

		EnableMarkSortedColumn(FALSE,FALSE);
		InitColumns();
		IDisplayTablePtr pDisplayTable = m_pTable;
		if (pDisplayTable == NULL) return  ;

		//if (m_bOnlyShowSelected)  //只显示选择的
		//{

		//}
		//else
		//{
			HRESULT hr = pDisplayTable->SearchDisplayTable(m_pQueryFilter,VARIANT_TRUE,&m_pCursor);
		//}
		EnableVirtualMode (CallbackGrid,(LPARAM)this);
	}
}

//void CTableGridCtrl::DoSelect()
//{
// 	ClearAllSelectedRow();
// 	BOOL bSelectionChange = FALSE;
// 	long *pIDList4Add;
// 	POSITION pos = m_lstSel.GetHeadPosition();
// 	if (pos)
// 	{
// 		CBCGPGridRange* pRange = (CBCGPGridRange*) m_lstSel.GetNext (pos);
// 		if (!pRange->IsEmpty())
// 		{
// 			bSelectionChange = TRUE;
// 			pIDList4Add = new long [pRange->m_nBottom - pRange->m_nTop + 1];
// 			int nIndex = 0;
// 
// 			m_nLastSelectedTop    = pRange->m_nTop;
// 			m_nLastSelectedBottom = pRange->m_nBottom;
// 
// 			for (int i=pRange->m_nTop; i<=pRange->m_nBottom; i++)
// 			{
// 				pIDList4Add[nIndex++] = m_List.GetSortAt(i)->objectid;
// 				m_List.GetSortAt(i)->bSelected = true;
// 			}
// 
// 			//刷新
// 			CTableGridDlg* pDlg = (CTableGridDlg*) GetParent();
// 			IFeatureLayerPtr pFeatureLayer(m_pTable);
// 			if (pFeatureLayer != NULL) pDlg->RefreshMap(esriViewGeoSelection,(ILayerPtr)m_pTable,NULL);
// 
// 			ITableSelectionPtr lpTableSelection(m_pTable);
// 			lpTableSelection->Clear();
// 			if (nIndex > 0)
// 			{
// 				ISelectionSetPtr lpSelectionSet = NULL;
// 				if (lpTableSelection != NULL)
// 				{
// 					lpTableSelection->get_SelectionSet(&lpSelectionSet);
// 					lpSelectionSet->AddList(nIndex,pIDList4Add);
// 				}
// 			}
// 			lpTableSelection->SelectionChanged();
// 			if (pFeatureLayer != NULL) 
// 				pDlg->RefreshMap(esriViewGeoSelection,(ILayerPtr)m_pTable,NULL);
// 			else
// 				SetVirtualRowsEx();
// 
// 			m_bStartSelect = FALSE;
// 		}
// 	}
//}

void CTableGridCtrl::DoShiftSelect(CBCGPGridItemID idCurrent)
{
	ITableSelectionPtr lpTableSelection(m_pTable);
	ISelectionSetPtr lpSelectionSet = NULL;
	if (lpTableSelection != NULL)
	{
		lpTableSelection->get_SelectionSet(&lpSelectionSet);
	}

	POSITION pos = m_lstSel.GetHeadPosition();
	BOOL bSelectionChange = FALSE;
	while (pos)
	{
		CBCGPGridRange* pRange = (CBCGPGridRange*) m_lstSel.GetNext (pos);
		if (!pRange->IsEmpty())
		{
			bSelectionChange = TRUE;
			//获取增加和删除的选择集个数
			int nCount4Add = 0;
			int nCount4Del = 0;

			//增加或删除选择集
			long *pIDList4Add;
			long *pIDList4Del;

			int nTop    = pRange->m_nTop;
			int nBottom = pRange->m_nBottom;

			int nCount = GetRowCount();
			if (nCount - m_nLastSelectedBottom < 1)
			{
				m_nLastSelectedBottom = nCount -1;
			}
			if (nCount - m_nLastSelectedTop < 1)
			{
				m_nLastSelectedTop = nCount -1;
			}
			if (m_nLastSelectedTop > m_nLastSelectedBottom) 
				m_nLastSelectedTop = m_nLastSelectedBottom ;


			if (m_nLastSelectedBottom == HAS_NOT_SELECTED && m_nLastSelectedTop == HAS_NOT_SELECTED)
			{
				m_nLastSelectedTop    = pRange->m_nTop;
				m_nLastSelectedBottom = pRange->m_nBottom;

				//增加选择集合
				nCount4Add = pRange->m_nBottom - pRange->m_nTop + 1;
				if (nCount4Add > 0)
				{
					pIDList4Add = new long[nCount4Add];
					nCount4Add = 0;
					for (int i=nTop; i<=nBottom; i++)
					{
						m_List.GetSortAt(i)->bSelected = TRUE;
						pIDList4Add[nCount4Add++] = m_List.GetSortAt(i)->objectid;
					}
				}
			}
			else
			{
				//上一次选择与当前鼠标位置的比较
				if (idCurrent.m_nRow < m_nLastSelectedTop && idCurrent.m_nRow < m_nLastSelectedBottom)
				{
					//新增
					nCount4Add = m_nLastSelectedTop - idCurrent.m_nRow;
					if (nCount4Add > 0)
					{
						pIDList4Add = new long[nCount4Add];
						nCount4Add = 0;
						for (int i= idCurrent.m_nRow; i< m_nLastSelectedTop; i++)
						{
							m_List.GetSortAt(i)->bSelected = TRUE;
							pIDList4Add[nCount4Add++] = m_List.GetSortAt(i)->objectid;
						}
					}

					////移除
					//nCount4Del = m_nLastSelectedBottom - m_nLastSelectedTop;
					//if (nCount4Del > 0)
					//{
					//	pIDList4Del = new long[nCount4Del];
					//	nCount4Del = 0;
					//	for (int i= m_nLastSelectedTop + 1; i<= m_nLastSelectedBottom; i++)
					//	{
					//		m_List.GetSortAt(i)->bSelected = FALSE;
					//		pIDList4Del[nCount4Del++] = m_List.GetSortAt(i)->objectid;
					//	}
					//}
					//m_nLastSelectedBottom = m_nLastSelectedTop;
					//m_nLastSelectedTop = idCurrent.m_nRow;

				}
				else if (idCurrent.m_nRow > m_nLastSelectedTop && idCurrent.m_nRow > m_nLastSelectedBottom)
				{
					//新增
					nCount4Add = idCurrent.m_nRow - m_nLastSelectedBottom;
					if (nCount4Add > 0)
					{
						pIDList4Add = new long[nCount4Add];
						nCount4Add = 0;
						for (int i= m_nLastSelectedBottom+1; i<=idCurrent.m_nRow; i++)
						{
							m_List.GetSortAt(i)->bSelected = TRUE;
							pIDList4Add[nCount4Add++] = m_List.GetSortAt(i)->objectid;
						}
					}
					
					////移除
					//nCount4Del = m_nLastSelectedBottom - m_nLastSelectedTop;
					//if (nCount4Del > 0)
					//{
					//	pIDList4Del = new long[nCount4Del];
					//	nCount4Del = 0;
					//	for (int i= m_nLastSelectedTop; i< m_nLastSelectedBottom; i++)
					//	{
					//		m_List.GetSortAt(i)->bSelected = FALSE;
					//		pIDList4Del[nCount4Del++] = m_List.GetSortAt(i)->objectid;
					//	}
					//}
					//
					//m_nLastSelectedTop    = m_nLastSelectedTop;
					//m_nLastSelectedBottom = idCurrent.m_nRow;
				}
				else
				{
					//移除
					nCount4Del = m_nLastSelectedBottom - idCurrent.m_nRow;
					if (nCount4Del > 0)
					{
						pIDList4Del = new long[nCount4Del];
						nCount4Del = 0;
						for (int i= idCurrent.m_nRow + 1; i<= m_nLastSelectedBottom; i++)
						{
							m_List.GetSortAt(i)->bSelected = FALSE;
							pIDList4Del[nCount4Del++] = m_List.GetSortAt(i)->objectid;
						}
					}
					
					//m_nLastSelectedBottom = idCurrent.m_nRow;
				}
			}

			if (lpSelectionSet != NULL)   //加入选择集
			{
				//刷新选择
// 				CTableGridDlg* pDlg = (CTableGridDlg*) GetParent();
// 				IFeatureLayerPtr pFeatureLayer(m_pTable);
// 				if (pFeatureLayer != NULL) pDlg->RefreshMap(esriViewGeoSelection,(ILayerPtr)m_pTable,NULL);
// 
// 				if (nCount4Add > 0)
// 				{
// 					lpSelectionSet->AddList(nCount4Add,pIDList4Add);
// 					delete [] pIDList4Add;
// 				}
// 				if (nCount4Del > 0)
// 				{
// 					lpSelectionSet->RemoveList(nCount4Del,pIDList4Del);
// 					delete [] pIDList4Del;
// 				}

// 				//刷新
// 				lpTableSelection->SelectionChanged();
// 				if (pFeatureLayer != NULL)
// 					pDlg->RefreshMap(esriViewGeoSelection,(ILayerPtr)m_pTable,NULL);
// 				else
// 					SetVirtualRowsEx();
			}
		}
	}
}

void CTableGridCtrl::DoCtrlSelect()
{
	ITableSelectionPtr lpTableSelection(m_pTable);
	ISelectionSetPtr lpSelectionSet = NULL;
	if (lpTableSelection != NULL)
	{
		lpTableSelection->get_SelectionSet(&lpSelectionSet);
	}

	POSITION pos = m_lstSel.GetHeadPosition();
	BOOL bSelectionChange = FALSE;
	while (pos)
	{
		CBCGPGridRange* pRange = (CBCGPGridRange*) m_lstSel.GetNext (pos);
		if (!pRange->IsEmpty())
		{
			bSelectionChange = TRUE;
			//获取增加和删除的选择集个数
			int nCount4Add = 0;
			int nCount4Del = 0;

			BOOL bHasSelected = FALSE;  //是否已经选择过
			for (int i=pRange->m_nTop; i<=pRange->m_nBottom; i++)
			{
				bool bSelected = m_List.GetSortAt(i)->bSelected;
				bSelected ? nCount4Del++ : nCount4Add++;
				m_List.GetSortAt(i)->bSelected = !bSelected;

				//记录上一次选择的
				if (!bSelected && !bHasSelected)
				{
					m_nLastSelectedTop    = i;
					m_nLastSelectedBottom = i;
					bHasSelected          = TRUE;
				}
			}

			if (!bHasSelected)
			{
				m_nLastSelectedTop    = HAS_NOT_SELECTED;
				m_nLastSelectedBottom = HAS_NOT_SELECTED;
			}

			//增加或删除选择集
			long *pIDList4Add;
			if (nCount4Add > 0)
			{
				pIDList4Add = new long[nCount4Add];
				//m_nSelectedCount += nCount4Add;
			}

			long *pIDList4Del;
			if (nCount4Del > 0)
			{
				pIDList4Del = new long[nCount4Del];
				//m_nSelectedCount -= nCount4Del;
			}
			nCount4Add = 0;
			nCount4Del = 0;
			for (int i=pRange->m_nTop; i<=pRange->m_nBottom; i++)
			{
				if (m_List.GetSortAt(i)->bSelected)
				{
					pIDList4Add[nCount4Add++] = m_List.GetSortAt(i)->objectid;
				}
				else
				{
					pIDList4Del[nCount4Del++] = m_List.GetSortAt(i)->objectid;
				}
			}

			if (lpSelectionSet != NULL)   //加入选择集
			{
				//刷新选择
// 				CTableGridDlg* pDlg = (CTableGridDlg*) GetParent();
// 				IFeatureLayerPtr pFeatureLayer(m_pTable);
// 				if (pFeatureLayer != NULL) pDlg->RefreshMap(esriViewGeoSelection,(ILayerPtr)m_pTable,NULL);
// 
// 				if (nCount4Add > 0)
// 				{
// 					lpSelectionSet->AddList(nCount4Add,pIDList4Add);
// 					delete [] pIDList4Add;
// 				}
// 				if (nCount4Del > 0)
// 				{
// 					lpSelectionSet->RemoveList(nCount4Del,pIDList4Del);
// 					delete [] pIDList4Del;
// 				}
// 
// 				//刷新
// 				lpTableSelection->SelectionChanged();
// 				if (pFeatureLayer != NULL) 
// 					pDlg->RefreshMap(esriViewGeoSelection,(ILayerPtr)m_pTable,NULL);
// 				else
// 					SetVirtualRowsEx();
			}
		}
	}
}

void CTableGridCtrl::ResetLastSelected()
{
	m_nLastSelectedTop    = HAS_NOT_SELECTED;
	m_nLastSelectedBottom = HAS_NOT_SELECTED;
}

BOOL CTableGridCtrl::IsFillAllRecords()
{
	return m_bIsFillAllRecords;
}

CSortedList::CSortedList()
{
	m_column_sort     = -1;
	m_column_type     = esriFieldTypeSmallInteger;
	m_bSortAscending  = FALSE;
	m_bShowSelectedOnly = FALSE;
	m_nColumnCount    = 0;
}

void CSortedList::AddTail( ROWDATATYPE t )
{
	m_list.AddTail(t); 
}


POSITION CSortedList::GetHeadPosition()
{
	return m_list.GetHeadPosition();
}

ROWDATATYPE CSortedList::GetNext( POSITION & p )
{ 
	return m_list.GetNext(p);
}

void CSortedList::RemoveAll()
{
	//释放内存
	int nCount = m_list.GetCount();
	for (int i=0; i< nCount; i++)
	{
		for (int j=0; j<m_nColumnCount; j++)
		{
			wchar_t* pChar = GetAt(i)->FieldValue[j];
			if (pChar != NULL &&  lstrcmp(pChar, _T("")) != 0)
			{
				delete pChar;
			}
		}
	}

	m_list.RemoveAll(); 
	m_vector.RemoveAll();
	m_Selectedvector.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
inline int GetExpoBase2(double d)
{
	int i = 0;
	((short *)(&i))[0] = (((short *)(&d))[3] & (short)32752); // _123456789ab____ & 0111111111110000
	return (i >> 4) - 1023;
}

inline bool Equals(double d1, double d2)
{
	if (d1 == d2)
		return true;
	int e1 = GetExpoBase2(d1);
	int e2 = GetExpoBase2(d2);
	int e3 = GetExpoBase2(d1 - d2);
	if ((e3 - e2 < -48) && (e3 - e1 < -48))
		return true;
	return false;
}

inline int Compare(double d1, double d2)
{
	if (Equals(d1, d2) == true)
		return 0;
	if (d1 > d2)
		return 1;
	return -1;
}

int compare(const void * v1, const void * v2)
{
	ROWDATATYPE **nn1, **nn2, *n1, *n2;
	int equal = 0;

	nn1 = (ROWDATATYPE **)v1;
	nn2 = (ROWDATATYPE **)v2;

	n1 = (ROWDATATYPE*)*nn1;
	n2 = (ROWDATATYPE*)*nn2;

	if(n1 == NULL || n2 == NULL)
		return 0;

	switch(m_column_type)//column_sort)
	{
	case esriFieldTypeOID:
	case esriFieldTypeSmallInteger:
		{
			wchar_t *pSource = n1->FieldValue[m_column_sort];
			wchar_t *pDest   = n2->FieldValue[m_column_sort];
			if (pSource != NULL && pDest !=NULL)
			{
				if (_tstoi(n1->FieldValue[m_column_sort]) == _tstoi(n2->FieldValue[m_column_sort]))
					equal = 0;
				else if (_tstoi(n1->FieldValue[m_column_sort]) > _tstoi(n2->FieldValue[m_column_sort]))
					equal = 1;
				else 
					equal = -1;
			}
		}
		break;
	case esriFieldTypeInteger:
		{
			wchar_t *pSource = n1->FieldValue[m_column_sort];
			wchar_t *pDest   = n2->FieldValue[m_column_sort];
			if (pSource != NULL && pDest !=NULL)
			{
				if (_tstol(n1->FieldValue[m_column_sort]) == _tstol(n2->FieldValue[m_column_sort]))
					equal = 0;
				else if (_tstol(n1->FieldValue[m_column_sort]) > _tstol(n2->FieldValue[m_column_sort]))
					equal = 1;
				else 
					equal = -1;
			}
		}
		break;
	case esriFieldTypeSingle:
	case esriFieldTypeDouble:
		{
			/*   
			if (_tstof(n1->FieldValue[column_sort]) == _tstof(n2->FieldValue[column_sort]))
			equal = 0;
			else if (_tstof(n1->FieldValue[column_sort]) > _tstof(n2->FieldValue[column_sort]))
			equal = 1;
			else 
			equal = -1;
			*/	
			wchar_t *pSource = n1->FieldValue[m_column_sort];
			wchar_t *pDest   = n2->FieldValue[m_column_sort];
			if (pSource != NULL && pDest !=NULL)
				equal = Compare(_tstof(pSource),_tstof(pDest));
		}
		break;
	case esriFieldTypeString:
		{
			wchar_t *pSource = n1->FieldValue[m_column_sort];
			wchar_t *pDest   = n2->FieldValue[m_column_sort];
			if (pSource != NULL && pDest !=NULL)
				equal = lstrcmp(pSource, pDest);
		}
		break;
	case esriFieldTypeDate:
		{
			COleDateTime t1,t2;
			if (t1.ParseDateTime(n1->FieldValue[m_column_sort]) && t2.ParseDateTime(n2->FieldValue[m_column_sort]))
			{
				if (t1 == t2)
					equal = 0;
				else if (t1 > t2)
					equal = 1;
				else
					equal = -1;
			}
		}
		break;
	default:
		break;
	}

	if(m_bSortAscending == FALSE)
		equal *= -1;

	return equal;
}


void CSortedList::Sort( int nColumnSorted,esriFieldType fieldType,BOOL bAsending,BOOL bAdd )
{
	m_column_sort = nColumnSorted;
	m_column_type = fieldType;
	m_bSortAscending = bAsending;

	BuildArray();
	qsort(m_vector.GetData(), m_vector.GetSize(), sizeof(ROWDATATYPE*), compare); // 
	if (m_bShowSelectedOnly) BuildSelectedArray();
}


void CSortedList::BuildArray( BOOL bRebuild /*= FALSE*/ )
{
	if (bRebuild) m_vector.RemoveAll();
	int nSize = m_vector.GetSize();
	if ( nSize < m_list.GetCount())
	{
		m_vector.SetSize(m_list.GetCount());
		POSITION p = m_list.FindIndex(nSize);
		while(p != NULL)
		{ /* fill vector */
			m_vector[nSize++] = &m_list.GetNext(p);
		} /* fill vector */  
	}
	
	//if (m_bShowSelectedOnly) BuildSelectedArray();

}
int CSortedList::GetCount()
{
	return m_list.GetCount();
	//return m_vector.GetSize();
}

void CSortedList::RemoveAt( int nIndex )
{
	int nCount = m_vector.GetSize();
	if (nIndex < nCount)
	{
		//维护链表
		BOOL bExist = FALSE;
		ROWDATATYPE * pRowData = m_vector[nIndex];
		//for (int i=0; i<nCount; i++)
		//{
		//	POSITION pos = m_list.FindIndex(i);
		//	if (pRowData == &m_list.GetAt(pos))
		//	{
		//		m_list.RemoveAt(pos);
		//		bExist = TRUE;
		//		break;
		//	}
		//}
		POSITION pos = m_list.GetHeadPosition();
		POSITION posLast;
		while (pos)
		{
			posLast = pos;
			if (pRowData == &m_list.GetNext(pos))
			{
				m_list.RemoveAt(posLast);
				bExist = TRUE;
				break;
			}
		}

		if (!bExist) return;

		m_vector.RemoveAt(nIndex);

		//维护选择链表
		nCount = m_Selectedvector.GetSize();
		for (int i=0; i<nCount; i++)
		{
			if (m_Selectedvector[i] == pRowData)
			{
				m_Selectedvector.RemoveAt(i);
				//m_nSelectedCount--;
				break;
			}
		}
	}
}

//如果被选中的数据还未读出来，需要另外加载(所以我传进变量lpFeatureSelection)..
//在CSortedList::Sort也在调用，这个仅仅为已读出数据排序而已，所以允许lpFeatureSelection = NULL
// chaizb 2010-05-18
void CSortedList::BuildSelectedArray(IFeatureSelectionPtr lpFeatureSelection /*=NULL*/)
{
	long nIDCount =0;
	if ( lpFeatureSelection != NULL )
	{
		ISelectionSetPtr lpSelectionSet = NULL;
		lpFeatureSelection->get_SelectionSet(&lpSelectionSet);
		lpSelectionSet->get_Count(&nIDCount);
	}

	int nCount = m_list.GetCount();
	int nSelectedCount = GetSelectedCount();

	if ( nSelectedCount >= nIDCount ) //已经读出的数据
	{
		m_Selectedvector.RemoveAll();
		if (nSelectedCount == 0 ) return;
		m_Selectedvector.SetSize(nSelectedCount);

		int nIndex = 0;
		for (int i=0; i<nCount; i++)
		{
			if (m_vector[i]->bSelected) 
			{
				m_Selectedvector[nIndex++] = m_vector[i];
			}
		}
	}
	else //表示选中的数据还未在gridctrl 
	{
		m_Selectedvector.RemoveAll();
		if (nIDCount == 0  || lpFeatureSelection == NULL) return;
		ISelectionSetPtr lpSelectionSet = NULL;
		lpFeatureSelection->get_SelectionSet(&lpSelectionSet);
		IQueryFilterPtr lpFilter(CLSID_QueryFilter);
		ICursorPtr lpCursor = NULL;
		lpSelectionSet->Search(lpFilter,VARIANT_FALSE,&lpCursor);
		IFeatureCursorPtr lpFeatureCursor = lpCursor;
        if ( lpFeatureCursor != NULL )
		{
			IFieldsPtr pFields = NULL;
			lpFeatureCursor->get_Fields(&pFields);
			long nFieldCount = 0;
			pFields->get_FieldCount(&nFieldCount);

			IFeaturePtr lpFeature = NULL;
			while(lpFeatureCursor->NextFeature(&lpFeature) == S_OK)
			{
				IRowPtr pRow = lpFeature;
				//这里可能会引起内存泄漏,但也不能把m_Selectedvector的item进行delete，因为有可能是来自m_vector
				ROWDATATYPE *row = new ROWDATATYPE; 
				long oid = 0;	
				pRow->get_OID(&oid);
				row->objectid = oid;
				row->bSelected = true;
				for (long i=0; i<nFieldCount; i++)
				{
					_variant_t var;
					pRow->get_Value(i,&var);
					if (var.vt != VT_EMPTY && var.vt != VT_NULL && var.vt != VT_UNKNOWN)
					{
						var.ChangeType(VT_BSTR,&var);

						DWORD dwTextLen = lstrlen((LPCTSTR)_bstr_t(var.bstrVal));
						TCHAR* pszTextBuffer = new TCHAR [dwTextLen + 2];
						memset (pszTextBuffer, 0, (dwTextLen+2) * sizeof(TCHAR));
						_tcscpy(pszTextBuffer,(LPCTSTR)_bstr_t(var.bstrVal));
						row->FieldValue[i] =  pszTextBuffer;//(TCHAR*)_bstr_t(var.bstrVal);
					}
					else 
						row->FieldValue[i] = _T("");
				}
				m_Selectedvector.Add(row);
			}
		}
	}
}

//获取选择行的个数
int CSortedList::GetSelectedCount()
{
	int nSelectedCount = 0;
	POSITION pos = m_list.GetHeadPosition();
	while (pos)
	{
		if (m_list.GetNext(pos).bSelected)
		{
			nSelectedCount++;
		}
	}

	return nSelectedCount;
}

void CSortedList::ShowSelectedOnly( BOOL bShowSelectedOnly )
{
	m_bShowSelectedOnly = bShowSelectedOnly;
}

BOOL CSortedList::IsShowSelectedOnly()
{
	return m_bShowSelectedOnly;
}

ROWDATATYPE * CSortedList::GetSortAt( int n )
{
	if ( m_bShowSelectedOnly )
	{
		if ( n >=0 && n < m_Selectedvector.GetSize() )
			return m_Selectedvector.GetAt(n);
		return NULL;
	}
	else
	{
		if ( n>=0 && n < m_vector.GetSize() )
			return m_vector.GetAt(n);
		return NULL;
	}
	return NULL;
	//return m_bShowSelectedOnly ? m_Selectedvector.GetAt(n) : m_vector.GetAt(n);
}

ROWDATATYPE * CSortedList::GetAt( int n )
{
	if ( n >=0 && n < m_vector.GetSize() )
		return m_vector.GetAt(n);
	return NULL;
}

void CSortedList::SetColumnCount( int nCount )
{
	m_nColumnCount = nCount;
}

void CTableGridCtrl::SetInitValue(GisqLib::_DGisqFrameworkCOMPtr pFrameworkCOM, ITablePtr pTable, IQueryFilterPtr pQueryFilter, BOOL bOnlyShowSelected)
{
	m_pFrameworkCOM = pFrameworkCOM;
	m_bOnlyShowSelected = bOnlyShowSelected;
	m_pTable = pTable;
	m_pQueryFilter = pQueryFilter;
	InitControls();
	FillTable();

}

BOOL CTableGridCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	BOOL  bResult;
	bResult=CBCGPGridCtrl::OnMouseWheel(nFlags, zDelta, pt);
	//获取滚动条的位置
	int nMin,nMax;
	CScrollBar* pVScrollbar=GetScrollBarCtrl(SB_VERT);
	if (pVScrollbar)
	{
		pVScrollbar->GetScrollRange(&nMin,&nMax);
		float fCurPos = (float)pVScrollbar->GetScrollPos();
		if (nMax > 0 && fCurPos > 0) 
		{
			if (fCurPos / nMax > 0.8)
			{
				FillTable();
			}
		}
	}
	return bResult;

}

void CTableGridCtrl::setColumHide(CString strOIDFieldName,CString strShapeFieldName,CComQIPtr<IField> spLenthField,CComQIPtr<IField> spAreaField)
{

	m_strOIDFieldName = strOIDFieldName;
	m_strShapeFieldName = strShapeFieldName;
	m_spAreaField = spAreaField;
	m_spLenthField = spLenthField;
}

void CTableGridCtrl::utf8ToUnicode(LPCSTR src, CString *Result)  
{  
	int wcsLen = MultiByteToWideChar( CP_UTF8, 0, src, -1, NULL, 0 );  
	LPWSTR result = new wchar_t[wcsLen + 1];
	::MultiByteToWideChar( CP_UTF8, 0, src, -1, result, wcsLen); 
	Result->Format(_T("%s"),result);
	delete result;
} 

void CTableGridCtrl::SetFtClassName(CString strFtClassName)
{
	m_strFtClassName = strFtClassName;
}
