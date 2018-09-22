/*********************************************
文件名:  LPGHQYTDKCXDlg
创建者:  luchencheng
编写日期:2016-10-013
简要描述:规划期用途地块查询功能窗口
修改记录(who-when-why):
********************************************/
// LPGHQYTDKCXDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LPGHQYTDKCXDlg.h"


// CLPGHQYTDKCXDlg 对话框

IMPLEMENT_DYNAMIC(CLPGHQYTDKCXDlg, CGSBCGDialog)

CLPGHQYTDKCXDlg::CLPGHQYTDKCXDlg(GisqLib::_DGisqFrameworkCOMPtr	ipFramework, CWnd* pParent /*=NULL*/)
	: CGSBCGDialog(CLPGHQYTDKCXDlg::IDD, pParent)
	, m_strTown(_T(""))
	, m_strGHDL(_T(""))
	, m_nAREA_Min(0)
	, m_nAREA_Max(0)
	, m_strUnit(_T("(平方米)"))
	, m_strResultNum(_T(""))
{
	m_ipFramework = ipFramework;
	m_parrDistrict = new CArray<_tagDistrictItem *,_tagDistrictItem *>;
}

CLPGHQYTDKCXDlg::~CLPGHQYTDKCXDlg()
{
	if (m_parrDistrict != NULL)
	{
		m_parrDistrict->RemoveAll();
		delete m_parrDistrict;
	}
}

void CLPGHQYTDKCXDlg::DoDataExchange(CDataExchange* pDX)
{
	CGSBCGDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_XZCOMBO_3, m_strTown);
	DDX_CBString(pDX, IDC_GHDLCOMBO_3, m_strGHDL);
	DDX_Control(pDX, IDC_XZCOMBO_3, m_cbxTown);
	DDX_Control(pDX, IDC_GHDLCOMBO_3, m_cbxGHDL);

	DDX_Text(pDX, IDC_TDYTQMJMIN, m_nAREA_Min);
	DDX_Text(pDX, IDC_TDYTQMJMAX, m_nAREA_Max);
	DDX_Control(pDX, IDC_TDYTQMJMIN, m_editAREA_Min);
	DDX_Control(pDX, IDC_TDYTQMJMAX, m_editAREA_Max);

	DDX_Text(pDX, IDC_UNITTEXT31, m_strUnit);
	DDX_Text(pDX, IDC_UNITTEXT32, m_strUnit);

	DDX_Control(pDX, IDC_GRIDAREA_3, m_RectForGrid);
	DDX_Text(pDX, IDC_RESCOUNT_3, m_strResultNum);
}


BEGIN_MESSAGE_MAP(CLPGHQYTDKCXDlg, CGSBCGDialog)
	ON_BN_CLICKED(IDC_METER_RD_3, &CLPGHQYTDKCXDlg::OnBnClickedMeterRd3)
	ON_BN_CLICKED(IDC_MU_RD_3, &CLPGHQYTDKCXDlg::OnBnClickedMuRd3)
	ON_BN_CLICKED(IDC_HECTARE_RD_3, &CLPGHQYTDKCXDlg::OnBnClickedHectareRd3)
	ON_BN_CLICKED(IDC_CXBUTTON_3, &CLPGHQYTDKCXDlg::OnBnClickedCxbutton3)
END_MESSAGE_MAP()


// CLPGHQYTDKCXDlg 消息处理程序

BOOL CLPGHQYTDKCXDlg::OnInitDialog()
{
	CGSBCGDialog::OnInitDialog();

	//初始化选中查询面积为平方米
	((CButton *)GetDlgItem(IDC_METER_RD_3))->SetCheck(TRUE);
	//初始化文本框控件
	m_editAREA_Min.SetValidChars(_T("1234567890."));//可用字符列表
	m_editAREA_Max.SetValidChars(_T("1234567890."));//可用字符列表

	m_editAREA_Min.SetWindowText(_T("0"));
	m_editAREA_Max.SetWindowText(_T("0"));

	UpdateData(FALSE);

	//初始化表格控件
	InitGridCtrl();

	if (m_ipFramework == NULL) return FALSE;
	//获取工作空间
	IWorkspacePtr ipWorkspace;
	CComQIPtr<GisqLib::IGSDataExchangeS> spGSDataExchangeS = m_ipFramework->GetDataExchangeS();
	BSTR bstrTag(_T("GHWS"));
	short index = spGSDataExchangeS->FindIndex(bstrTag);
	if (index >= 0)
	{
		CComQIPtr<GisqLib::IGSDataExchange> spGSDataExchange = spGSDataExchangeS->Item(index);
		CComVariant var = spGSDataExchange->Item(0);
		ipWorkspace = var.punkVal;
	}
	if (ipWorkspace == NULL) return FALSE;
	m_ipFtWorkspace = ipWorkspace;
	//初始化查询范围控件
	InitTownComboBox();
	InitDlComboBox();
	m_cbxTown.SelectString(0,_T("全部"));
	m_cbxGHDL.SelectString(0,_T("全部"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CLPGHQYTDKCXDlg::InitTownComboBox()
{



	if (GetXzqDm() == TRUE)
	{
		m_cbxTown.AddString(_T("全部"));
	}	
	SortArray(0,m_parrDistrict->GetCount() - 1);

	for (int i = 0; i < m_parrDistrict->GetCount() - 1; i++)
	{
		_tagDistrictItem *item;
		item = m_parrDistrict->GetAt(i);
		if (item->nLevel == DISTRICT_TREE_XIANG_ICON ||item->nLevel == DISTRICT_TREE_ZHEN_ICON)
		{
			m_cbxTown.AddString(item->strName);
			m_cbxTown.SetItemData(m_cbxTown.GetCount() - 1, (DWORD_PTR)item);
		}
	}

	return TRUE;
}

BOOL CLPGHQYTDKCXDlg::GetXzqDm()
{
	CMapStringToString mapTMP;//定义一个临时map用于查重时快速检索
	mapTMP.InitHashTable(601);
	//遍历表
	ITablePtr ipTable;
	m_ipFtWorkspace->OpenTable(_T("T_XZQMC"),&ipTable);
	if (ipTable != NULL)
	{
		ICursorPtr ipCursor = NULL;
		ipTable->Search(NULL,VARIANT_FALSE,&ipCursor);
		if (ipCursor == NULL)
		{return FALSE;}
		IRowPtr ipRow = NULL;
		ipCursor->NextRow(&ipRow);
		while(ipRow != NULL)
		{
			IFieldsPtr ipFields;
			ipRow->get_Fields(&ipFields);
			long nXZQIndex;

			ipFields->FindField(_T("XZQDM"),&nXZQIndex);
			VARIANT DMtmp;
			ipRow->get_Value(nXZQIndex,&DMtmp);

			ipFields->FindField(_T("XZQMC"),&nXZQIndex);
			VARIANT MCtmp;
			ipRow->get_Value(nXZQIndex,&MCtmp);
			//插入到数组
			if (DMtmp.bstrVal != NULL && MCtmp.bstrVal != NULL)
			{
				AddToArray(DMtmp.bstrVal,MCtmp.bstrVal);
				mapTMP.SetAt(DMtmp.bstrVal,MCtmp.bstrVal);
			}

			ipCursor->NextRow(&ipRow);
		}		
	}

	//遍历要素集，补全表中没有的值，并建立一个只与map相关的值

	IFeatureClassPtr ipFtClass = NULL;
	m_ipFtWorkspace->OpenFeatureClass(_T("XZQ"),&ipFtClass);
	if (ipFtClass != NULL)
	{
		//保存为全局，在查找时使用
		IFeatureCursorPtr ipFtCursor = NULL;
		ipFtClass->Search(NULL,FALSE,&ipFtCursor);
		if (ipFtCursor != NULL)
		{
			IFeaturePtr ipFt = NULL;
			ipFtCursor->NextFeature(&ipFt);
			while(ipFt != NULL)
			{
				IFieldsPtr ipFields;
				ipFt->get_Fields(&ipFields);
				long nXZQIndex;

				ipFields->FindField(_T("XZQDM"),&nXZQIndex);
				VARIANT DMtmp;
				ipFt->get_Value(nXZQIndex,&DMtmp);

				ipFields->FindField(_T("XZQMC"),&nXZQIndex);
				VARIANT MCtmp;
				ipFt->get_Value(nXZQIndex,&MCtmp);

				if (DMtmp.bstrVal != NULL && MCtmp.bstrVal != NULL)
				{
					//如果表中没有，添加到数组
					CString tmp;
					if (mapTMP.Lookup(DMtmp.bstrVal,tmp) == FALSE)
					{
						mapTMP.SetAt(DMtmp.bstrVal,MCtmp.bstrVal);
						AddToArray(DMtmp.bstrVal,MCtmp.bstrVal);
					}
				}
				ipFtCursor->NextFeature(&ipFt);
			}
		}

	}
	mapTMP.RemoveAll();
	//如果数据库中不存在表和要素集，返回空
	if (ipTable == NULL/* && ipFtClass == NULL*/)
	{
		return FALSE;
	}
	return TRUE;
};

void CLPGHQYTDKCXDlg::AddToArray(BSTR bstrDM,BSTR bstrMC)
{
	CString strDM = bstrDM;
	CString strMC = bstrMC;
	if (strDM.GetLength() == 2)//省级
	{
		_tagDistrictItem *tmp = new _tagDistrictItem;
		tmp->nLevel = DISTRICT_TREE_SHENG_ICON;
		tmp->strCode = strDM;
		tmp->strName = strMC;
		tmp->strParentID = _T("");
		m_parrDistrict->Add(tmp);
		if (strDM.GetLength() < m_strParentDM.GetLength())
		{
			m_strParentDM = strDM;
			m_strParentMC = strMC;
		}
	} else if(strDM.GetLength() == 4)//市级
	{
		_tagDistrictItem *tmp = new _tagDistrictItem;
		tmp->nLevel = DISTRICT_TREE_SHI_ICON;
		tmp->strCode = strDM;
		tmp->strName = strMC;
		tmp->strParentID = strDM.Left(2);
		m_parrDistrict->Add(tmp);
		if (strDM.GetLength() < m_strParentDM.GetLength())
		{
			m_strParentDM = strDM;
			m_strParentMC = strMC;
		}
	} else if(strDM.GetLength() == 6)//县区
	{
		if (strMC.Find(_T("县"),0) > 0)//县
		{
			_tagDistrictItem *tmp = new _tagDistrictItem;
			tmp->nLevel = DISTRICT_TREE_XIAN_ICON;
			tmp->strCode = strDM;
			tmp->strName = strMC;
			tmp->strParentID = strDM.Left(4);
			m_parrDistrict->Add(tmp);
			if (strDM.GetLength() < m_strParentDM.GetLength())
			{
				m_strParentDM = strDM;
				m_strParentMC = strMC;
			}
		} 
		else//区
		{
			_tagDistrictItem *tmp = new _tagDistrictItem;
			tmp->nLevel = DISTRICT_TREE_QU_ICON;
			tmp->strCode = strDM;
			tmp->strName = strMC;
			tmp->strParentID = strDM.Left(4);
			tmp->strParentID = _T("");
			m_parrDistrict->Add(tmp);
			if (strDM.GetLength() < m_strParentDM.GetLength())
			{
				m_strParentDM = strDM;
				m_strParentMC = strMC;
			}
		}

	} else if(strDM.GetLength() == 9)//乡镇
	{
		if (strMC.Find(_T("镇"),0) > 0)//镇
		{
			_tagDistrictItem *tmp = new _tagDistrictItem;
			tmp->nLevel = DISTRICT_TREE_ZHEN_ICON;
			tmp->strCode = strDM;
			tmp->strName = strMC;
			tmp->strParentID = strDM.Left(6);
			m_parrDistrict->Add(tmp);
			if (strDM.GetLength() < m_strParentDM.GetLength())
			{
				m_strParentDM = strDM;
				m_strParentMC = strMC;
			}
		} 
		else//乡
		{
			_tagDistrictItem *tmp = new _tagDistrictItem;
			tmp->nLevel = DISTRICT_TREE_XIANG_ICON;
			tmp->strCode = strDM;
			tmp->strName = strMC;
			tmp->strParentID = strDM.Left(6);
			m_parrDistrict->Add(tmp);
			if (strDM.GetLength() < m_strParentDM.GetLength())
			{
				m_strParentDM = strDM;
				m_strParentMC = strMC;
			}
		}
	} else if(strDM.GetLength() == 12)//村
	{
		_tagDistrictItem *tmp = new _tagDistrictItem;
		tmp->nLevel = DISTRICT_TREE_CUN_ICON;
		tmp->strCode = strDM;
		tmp->strName = strMC;
		tmp->strParentID = strDM.Left(9);
		m_parrDistrict->Add(tmp);
		if (strDM.GetLength() < m_strParentDM.GetLength())
		{
			m_strParentDM = strDM;
			m_strParentMC = strMC;
		}
	}
}

void CLPGHQYTDKCXDlg::SortArray(int left, int right)
{
	//如果左右索引相遇则结束
	if (left>=right)
	{
		return;
	}

	int i = left;
	int j = right;
	//参考点
	_tagDistrictItem *keyitem;
	CString strkeyCode;

	//参考值
	keyitem = m_parrDistrict->GetAt(left);
	strkeyCode = keyitem->strCode;

	while(i < j)
	{
		CString strtmpCode;
		strtmpCode = m_parrDistrict->GetAt(j)->strCode;
		while(i < j && strkeyCode < strtmpCode)
		{
			j--;
			strtmpCode = m_parrDistrict->GetAt(j)->strCode;
		}
		m_parrDistrict->SetAt(i,m_parrDistrict->GetAt(j));
		strtmpCode = m_parrDistrict->GetAt(i)->strCode;
		while(i < j && strkeyCode > strtmpCode)
		{
			i++;
			strtmpCode = m_parrDistrict->GetAt(i)->strCode;
		}
		m_parrDistrict->SetAt(j,m_parrDistrict->GetAt(i));
	}
	m_parrDistrict->SetAt(i,keyitem);

	SortArray(left, i - 1);/*用同样的方式对分出来的左边的小组进行快排*/
	SortArray(i + 1, right);/*用同样的方式对分出来的右边的小组进行快排*/

}

BOOL CLPGHQYTDKCXDlg::InitDlComboBox()
{
	if (GetGHDLMC() == TRUE)
	{
		m_cbxGHDL.AddString(_T("全部"));
	}

	for (int i = 0; i < m_arrDLMC.GetCount() - 1; i++)
	{
		m_cbxGHDL.AddString(m_arrDLMC.GetAt(i));
	}

	return TRUE;
}

BOOL CLPGHQYTDKCXDlg::GetGHDLMC()
{
	ITablePtr ipTable;
	m_ipFtWorkspace->OpenTable(_T("T_EnumValue"),&ipTable);

	if (ipTable == NULL) return FALSE;

	IQueryFilterPtr ipQuery(CLSID_QueryFilter);
	ipQuery->put_SubFields(_T("*"));
	ipQuery->put_WhereClause(_T("ENUMNAME = 'TDYTQLXDM'"));
	ICursorPtr ipCursor;
	ipTable->Search(ipQuery,VARIANT_FALSE,&ipCursor);

	if (ipCursor == NULL) return FALSE;

	IRowPtr ipRow = NULL;
	ipCursor->NextRow(&ipRow);

	while(ipRow != NULL)
	{
		IFieldsPtr ipFields;
		ipRow->get_Fields(&ipFields);
		long nXZQIndex;

		ipFields->FindField(_T("ENUMMC"),&nXZQIndex);
		VARIANT MCtmp;
		ipRow->get_Value(nXZQIndex,&MCtmp);


		//插入到数组
		if (MCtmp.bstrVal != NULL)
		{
			m_arrDLMC.Add(MCtmp.bstrVal);
		}
		ipCursor->NextRow(&ipRow);
	}	

	return TRUE;
}

BOOL CLPGHQYTDKCXDlg::InitGridCtrl()
{
	CRect rect;
	CBCGPGridColors GridTheme;

	if (m_wndResultGrid.GetSafeHwnd() == NULL)
	{	
		m_RectForGrid.GetClientRect(&rect);
		m_RectForGrid.MapWindowPoints (this, &rect);
		if (rect == NULL)
		{
			return FALSE;
		}

		m_wndResultGrid.Create (WS_CHILD |WS_VISIBLE, rect, this, 1);
		m_wndResultGrid.EnableMarkSortedColumn (FALSE);//设置表背景初始化为无
		m_wndResultGrid.EnableRowHeader (TRUE);//显示行头
		m_wndResultGrid.EnableLineNumbers ();//显示行号
		m_wndResultGrid.SetReadOnly(TRUE);
		GridTheme = m_wndResultGrid.GetColorTheme();
		GridTheme.m_clrHorzLine = RGB (0, 0, 0);//设置水平网格线颜色
		GridTheme.m_clrVertLine = RGB (0, 0, 0);//设置垂直网格线颜色
		m_wndResultGrid.SetColorTheme(GridTheme);

		//仅在第一次调用时，添加一个空壳
		const int nColumns = 6;
		//添加列头
		for (int nColumn = 0; nColumn < nColumns; nColumn++)
		{
			CString strColumn;
			strColumn.Format (_T("%c"), _T('A') + nColumn);

			m_wndResultGrid.InsertColumn (nColumn, strColumn, 80);
		}
		//添加行头
		for (int nRow = 0; nRow < 10; nRow++)
		{
			CBCGPGridRow* pRow = m_wndResultGrid.CreateRow (m_wndResultGrid.GetColumnCount ());
			m_wndResultGrid.AddRow (pRow, FALSE);
		}
	}else{
		//如果不是第一次调用，那么删除所有之前的数据
		m_wndResultGrid.DeleteAllColumns();//删除所有列
		m_wndResultGrid.RemoveAll();//删除所有行
	}
	//刷新显示并设置焦点
	m_wndResultGrid.AdjustLayout ();
	m_wndResultGrid.SetFocus ();

	return TRUE;
}
void CLPGHQYTDKCXDlg::OnBnClickedMeterRd3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strUnit = _T("(平方米)");
	UpdateData(FALSE);
}

void CLPGHQYTDKCXDlg::OnBnClickedMuRd3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strUnit = _T("(亩)");
	UpdateData(FALSE);
}

void CLPGHQYTDKCXDlg::OnBnClickedHectareRd3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strUnit = _T("(公顷)");
	UpdateData(FALSE);
}

void CLPGHQYTDKCXDlg::OnBnClickedCxbutton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CString strXZQ = _T("");//组织查询条件
	CString strArea = _T("");
	CString strDLMC = _T("");
	CString strWhere = _T("");

	//查询行政区范围
	if (m_strTown == _T("全部"))
	{
		strXZQ += _T("XZQMC = '*'");
	}else
	{
		strXZQ += _T("XZQMC = '") + m_strTown + _T("'");
	}
	//查询地类名称
	if (m_strGHDL == _T("全部"))
	{
		
	}else
	{
		strDLMC += _T(" and TDYTQLX = '") + m_strGHDL + _T("'");
	}	
	//查询条件,换算为平方米
	double nAREA_Max,nAREA_Min;
	if (m_strUnit == _T("米"))
	{
		nAREA_Max = m_nAREA_Max;
		nAREA_Min = m_nAREA_Min;
	}else if (m_strUnit == _T("(亩)"))
	{
		nAREA_Max = m_nAREA_Max * 666.67;
		nAREA_Min = m_nAREA_Min * 666.67;

	}else if (m_strUnit == _T("(公顷)"))
	{
		nAREA_Max = m_nAREA_Max * 10000.0;
		nAREA_Min = m_nAREA_Min * 10000.0;
	}
	//排除查询条件空值
	if (m_nAREA_Min != 0)
	{
		CString strTmp;
		strTmp.Format(_T(" and TBMJ >= %lf "), nAREA_Min);
		strArea += strTmp;
	}
	if (m_nAREA_Max != 0)
	{
		CString strTmp;
		strTmp.Format(_T(" and TBMJ <= %lf "), nAREA_Max);
		strArea += strTmp;
	}
	
	//合并查询条件
	strWhere += strXZQ + strDLMC + strArea;

	IQueryFilterPtr ipQuery(CLSID_QueryFilter);
	ipQuery->put_SubFields(_T("*"));
	ipQuery->put_WhereClause(_bstr_t(strWhere));
	InitGridCtrl();
	ShowResrult(ipQuery);
}

BOOL CLPGHQYTDKCXDlg::ShowResrult(IQueryFilterPtr ipQuery)
{
	IFeatureClassPtr ipFtClass;
	m_ipFtWorkspace->OpenFeatureClass(_T("TDYTQ"),&ipFtClass);
	if (ipFtClass == NULL) return FALSE;

	IFeatureCursorPtr ipFtCursor;
	ipFtClass->Search(ipQuery,VARIANT_FALSE,&ipFtCursor);

	if (ipFtCursor == NULL) return FALSE;

	int nResult = 0;
	IFeaturePtr ipFt;
	ipFtCursor->NextFeature(&ipFt);

	//添加列头
	if (ipFt != NULL)
	{
		IFieldsPtr ipFields;
		long nNumField;
		ipFt->get_Fields(&ipFields);
		if (ipFields == NULL)return FALSE;

		ipFields->get_FieldCount(&nNumField);
		for (int i = 0; i < nNumField -1; i++)
		{
			IFieldPtr ipField;
			BSTR bstrName;
			ipFields->get_Field(i,&ipField);
			ipField->get_Name(&bstrName);
			m_wndResultGrid.InsertColumn(i,bstrName,80);
		}
	}
	//遍历结果集合
	while(ipFt != NULL)
	{
		nResult ++;

		IFieldsPtr ipFields;
		long nNumField;
		ipFt->get_Fields(&ipFields);
		if (ipFields == NULL)return FALSE;

		ipFields->get_FieldCount(&nNumField);
		CBCGPGridRow *pGridRow = m_wndResultGrid.CreateRow(nNumField);
		for (int i = 0; i < nNumField -1; i++)
		{
			IFieldPtr ipField;
			VARIANT vtTmp;
			ipFields->get_Field(i,&ipField);
			ipFt->get_Value(i,&vtTmp);
			pGridRow->GetItem(i)->SetValue(vtTmp);
		}
		m_wndResultGrid.AddRow(pGridRow);

		ipFtCursor->NextFeature(&ipFt);
	}

	m_wndResultGrid.AdjustLayout();
	m_wndResultGrid.SetFocus();

	m_strResultNum.Format(_T("共%d条数据"),nResult);
	UpdateData(FALSE);

	return TRUE;
}
