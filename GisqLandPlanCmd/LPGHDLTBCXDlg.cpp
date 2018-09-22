/*********************************************
文件名:  LPGHDLTBCXDlg
创建者:  luchencheng
编写日期:2016-10-09
简要描述:规划地类图斑查询功能窗口
修改记录(who-when-why):
********************************************/
// LPGHDLTBCXDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LPGHDLTBCXDlg.h"
#include "LPGridDockWinCmd.h"

// CLPGHDLTBCXDlg 对话框

IMPLEMENT_DYNAMIC(CLPGHDLTBCXDlg, CGSBCGDialog)

CLPGHDLTBCXDlg::CLPGHDLTBCXDlg(GisqLib::_DGisqFrameworkCOMPtr ipFramework, CWnd* pParent /*=NULL*/)
	: CGSBCGDialog(CLPGHDLTBCXDlg::IDD, pParent)
	, m_strUnit(_T("(平方米)"))
	, m_nGHDL_Min(0)
	, m_nGHDL_Max(0)
	, m_strResultNum(_T(""))
	, m_strTown(_T(""))
	, m_strVillage(_T(""))
	, m_strGHDL(_T(""))
{
	m_ipFramework = ipFramework;
	m_parrDistrict = new CArray<_tagDistrictItem *,_tagDistrictItem *>;
}

CLPGHDLTBCXDlg::~CLPGHDLTBCXDlg()
{
	if (m_parrDistrict != NULL)
	{
		m_parrDistrict->RemoveAll();
		delete m_parrDistrict;
	}
	if (m_wndResultGrid.GetSafeHwnd() != NULL)
	{
		m_wndResultGrid.DeleteAllColumns();//删除所有列
		m_wndResultGrid.RemoveAll();//删除所有行
	}
}

void CLPGHDLTBCXDlg::DoDataExchange(CDataExchange* pDX)
{
	CGSBCGDialog::DoDataExchange(pDX);
	//查找范围
	DDX_Control(pDX, IDC_XZCOMBO_2, m_cbxTown);
	DDX_Control(pDX, IDC_CUNCOMBO_2, m_cbxVillage);
	DDX_Control(pDX, IDC_GHDLCOMBO_2, m_cbxGHDL);
	DDX_CBString(pDX, IDC_XZCOMBO_2, m_strTown);
	DDX_CBString(pDX, IDC_CUNCOMBO_2, m_strVillage);
	DDX_CBString(pDX, IDC_GHDLCOMBO_2, m_strGHDL);
	//单位文本
	DDX_Text(pDX, IDC_UNITTEXT22, m_strUnit);
	DDX_Text(pDX, IDC_UNITTEXT21, m_strUnit);	
	//查找范围
	DDX_Text(pDX, IDC_GHDLMIN_2, m_nGHDL_Min);
	DDX_Text(pDX, IDC_GHDLMAX_2, m_nGHDL_Max);
	DDX_Control(pDX, IDC_GHDLMIN_2, m_editGHDLMJ_Min);
	DDX_Control(pDX, IDC_GHDLMAX_2, m_editGHDLMJ_Max);
	//结果显示
	DDX_Control(pDX, IDC_GRIDAREA_2, m_RectForGrid);
	DDX_Text(pDX, IDC_RESCOUNT_2, m_strResultNum);


}


BEGIN_MESSAGE_MAP(CLPGHDLTBCXDlg, CGSBCGDialog)
	ON_BN_CLICKED(IDC_METER_RD_2, &CLPGHDLTBCXDlg::OnBnClickedMeterRd2)
	ON_BN_CLICKED(IDC_MU_RD_2, &CLPGHDLTBCXDlg::OnBnClickedMuRd2)
	ON_BN_CLICKED(IDC_HECTARE_RD_2, &CLPGHDLTBCXDlg::OnBnClickedHectareRd2)
	ON_BN_CLICKED(IDC_CXBUTTON_2, &CLPGHDLTBCXDlg::OnBnClickedCxbutton2)
	ON_CBN_SELCHANGE(IDC_XZCOMBO_2, &CLPGHDLTBCXDlg::OnCbnSelchangeXzcombo2)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CLPGHDLTBCXDlg 消息处理程序

BOOL CLPGHDLTBCXDlg::OnInitDialog()
{
	CGSBCGDialog::OnInitDialog();

	//初始化选中查询面积为平方米
	((CButton *)GetDlgItem(IDC_METER_RD_2))->SetCheck(TRUE);
	//初始化文本框控件
	m_editGHDLMJ_Min.SetValidChars(_T("1234567890."));//可用字符列表
	m_editGHDLMJ_Max.SetValidChars(_T("1234567890."));//可用字符列表

	m_editGHDLMJ_Min.SetWindowText(_T("0"));
	m_editGHDLMJ_Max.SetWindowText(_T("0"));

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
	m_cbxVillage.EnableWindow(FALSE);
	m_cbxGHDL.SelectString(0,_T("全部"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CLPGHDLTBCXDlg::InitTownComboBox()
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

BOOL CLPGHDLTBCXDlg::GetXzqDm()
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

void CLPGHDLTBCXDlg::AddToArray(BSTR bstrDM,BSTR bstrMC)
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

void CLPGHDLTBCXDlg::SortArray(int left, int right)
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

BOOL CLPGHDLTBCXDlg::InitDlComboBox()
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

BOOL CLPGHDLTBCXDlg::GetGHDLMC()
{
	ITablePtr ipTable;
	m_ipFtWorkspace->OpenTable(_T("T_EnumValue"),&ipTable);

	if (ipTable == NULL) return FALSE;

	IQueryFilterPtr ipQuery(CLSID_QueryFilter);
	ipQuery->put_SubFields(_T("*"));
	ipQuery->put_WhereClause(_T("ENUMNAME = 'GHDLDM'"));
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

BOOL CLPGHDLTBCXDlg::InitGridCtrl()
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
void CLPGHDLTBCXDlg::OnBnClickedMeterRd2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strUnit = _T("(平方米)");
	UpdateData(FALSE);
}

void CLPGHDLTBCXDlg::OnBnClickedMuRd2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strUnit = _T("(亩)");
	UpdateData(FALSE);
}

void CLPGHDLTBCXDlg::OnBnClickedHectareRd2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strUnit = _T("(公顷)");
	UpdateData(FALSE);
}

void CLPGHDLTBCXDlg::OnCbnSelchangeXzcombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	int nindex = m_cbxTown.GetCurSel();
	if (nindex < 0) return;
	CString strTmp;
	m_cbxTown.GetLBText(nindex,strTmp);
	if (strTmp == _T("全部"))
	{
		m_cbxVillage.SelectString(0,_T("全部"));
		m_cbxVillage.EnableWindow(FALSE);
	}

	_tagDistrictItem *item;
	item = (_tagDistrictItem* )m_cbxTown.GetItemData(nindex);
	if (item != NULL)
	{
		m_cbxVillage.EnableWindow(TRUE);
		FlashComboBox(item);
	}	
}

BOOL CLPGHDLTBCXDlg::FlashComboBox(_tagDistrictItem *tagParent)
{
	_tagDistrictItem *item;
	//现在只需要用到村级，根据父节点级别，初始化子集
	switch(tagParent->nLevel)
	{
	case DISTRICT_TREE_SHENG_ICON:
	case DISTRICT_TREE_SHI_ICON:
	case DISTRICT_TREE_XIAN_ICON:
	case DISTRICT_TREE_QU_ICON:
		break;
	case DISTRICT_TREE_XIANG_ICON:		
	case DISTRICT_TREE_ZHEN_ICON:
		m_cbxVillage.ResetContent();
		m_cbxVillage.AddString(_T("全部"));
		for (int i = 0; i < m_parrDistrict->GetCount() - 1; i++)
		{
			item = m_parrDistrict->GetAt(i);
			if (item->strParentID == tagParent->strCode)
			{
				m_cbxVillage.AddString(item->strName);
				m_cbxVillage.SetItemData(m_cbxVillage.GetCount() - 1, (DWORD_PTR)item);
			}
		}
		m_cbxVillage.SelectString(0,_T("全部"));
		break;
	default:
		break;
	}
	return TRUE;
}

void CLPGHDLTBCXDlg::OnBnClickedCxbutton2()
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
		strXZQ += _T("XZQMC = '") + m_strParentMC + _T("'");
	}else if (m_strVillage == _T("全部"))
	{
		strXZQ += _T("XZQMC = '") + m_strTown + _T("'");
	}else
	{
		strXZQ += _T("XZQMC = '") + m_strVillage + _T("'");
	}
	//查询地类名称
	if (m_strGHDL == _T("全部"))
	{

	}else
	{
		strDLMC += _T(" and GHDLMC = '") + m_strGHDL + _T("'");
	}	
	//查询条件,换算为平方米
	double nGHDL_Max,nGHDL_Min;
	if (m_strUnit == _T("米"))
	{
		nGHDL_Max = m_nGHDL_Max;
		nGHDL_Min = m_nGHDL_Min;
	}
	if (m_strUnit == _T("(亩)"))
	{
		nGHDL_Max = m_nGHDL_Max * 666.67;
		nGHDL_Min = m_nGHDL_Min * 666.67;
	}else if (m_strUnit == _T("(公顷)"))
	{
		nGHDL_Max = m_nGHDL_Max * 10000.0;
		nGHDL_Min = m_nGHDL_Min * 10000.0;
	}
	//排除查询条件空值
	if (m_nGHDL_Min != 0)
	{
		CString strTmp;
		strTmp.Format(_T(" and TBMJ >= %lf "), nGHDL_Min);
		strArea += strTmp;
	}
	if (m_nGHDL_Max != 0)
	{
		CString strTmp;
		strTmp.Format(_T(" and TBMJ <= %lf "), nGHDL_Max);
		strArea += strTmp;
	}

	//合并查询条件
	strWhere += strXZQ + strDLMC + strArea;

	IQueryFilterPtr ipQuery(CLSID_QueryFilter);
	ipQuery->put_SubFields(_T("*"));
	ipQuery->put_WhereClause(_bstr_t(strWhere));

	CString strDlgCaption;
	strDlgCaption.Format(_T("行政区:%s;地类:%s;面积范围:%d-%d(%s)"),strXZQ.Mid(5),m_strGHDL,m_nGHDL_Min,m_nGHDL_Max,m_strUnit);
	CComQIPtr<GisqLib::_DGisqFrameworkCOM> spGisqFramework = m_ipFramework;
	CComQIPtr<GisqLib::IGSDockableWindow> spResDockwin;
	CComQIPtr<GisqLib::IGSDockableWindowManager> spGSDockableWindowManager = spGisqFramework->GetDockableWindowManager();
	short nCount = 0;
	nCount = spGSDockableWindowManager->Count();
	for (short i = 0; i < nCount; i++)
	{
		CComQIPtr<GisqLib::IGSDockableWindow> spDockwin = spGSDockableWindowManager->Item(i);
		_bstr_t bstrGUID = spDockwin->GetDockWindowDefGUID();
		if (bstrGUID == _bstr_t("GisqLandPlanCmd.LPGridDockWinCmd.1"))
		{
			spResDockwin = spDockwin;
		}
	}
	if (spResDockwin == NULL)
	{
		CComQIPtr<GisqLib::IGSDockableWindow> spGridDockWin = 
			spGSDockableWindowManager->CreateDockableWindowFromGUID(_bstr_t("GisqLandPlanCmd.LPGridDockWinCmd.1"));
		spGridDockWin->Dock(GisqLib::DWS_ALIGN_BOTTOM);
		CComQIPtr<ILPGridDockWinCmd> spResDlg = spGridDockWin->GetDockableWindowTabDef();
		spResDlg->SetQueryFilter(ipQuery,_T("TDGHDL"),NULL);
		//spGridDockWin->SetName(strDlgCaption.AllocSysString());
		//spResDlg->SetDlgCaption(strDlgCaption.AllocSysString());
	}else
	{
		CComQIPtr<GisqLib::IGSDockableWindow> spGridDockWin = 
			spGSDockableWindowManager->CreateDockableWindowFromGUID(_bstr_t("GisqLandPlanCmd.LPGridDockWinCmd.1"));
		spGridDockWin->Dock(GisqLib::DWS_ALIGN_BOTTOM);
		spGridDockWin->AddTab(spResDockwin);
		CComQIPtr<ILPGridDockWinCmd> spResDlg = spGridDockWin->GetDockableWindowTabDef();
		spResDlg->SetQueryFilter(ipQuery,_T("TDGHDL"),NULL);
	}

	
	InitGridCtrl();
	ShowResrult(ipQuery);
}

BOOL CLPGHDLTBCXDlg::ShowResrult(IQueryFilterPtr ipQuery)
{
	IFeatureClassPtr ipFtClass;
	m_ipFtWorkspace->OpenFeatureClass(_T("TDGHDL"),&ipFtClass);
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

CString CLPGHDLTBCXDlg::VTtoCString(VARIANT m_variant)
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
//下面是退出时释放窗口资源
void CLPGHDLTBCXDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CGSBCGDialog::OnClose();
	delete this;
}

void CLPGHDLTBCXDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CGSBCGDialog::OnCancel();
	delete this;
}

void CLPGHDLTBCXDlg::OnDestroy()
{
	CGSBCGDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	delete this;
}
