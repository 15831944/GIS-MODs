/*********************************************
文件名:  LPJSYDGZQCXDlg
创建者:  luchencheng
编写日期:2016-10-13
简要描述:建设用地管制区查询功能窗口
修改记录(who-when-why):
********************************************/
// LPJSYDGZQCXDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LPJSYDGZQCXDlg.h"


// CLPJSYDGZQCXDlg 对话框

IMPLEMENT_DYNAMIC(CLPJSYDGZQCXDlg, CGSBCGDialog)

CLPJSYDGZQCXDlg::CLPJSYDGZQCXDlg(GisqLib::_DGisqFrameworkCOMPtr	ipFramework, CWnd* pParent /*=NULL*/)
	: CGSBCGDialog(CLPJSYDGZQCXDlg::IDD, pParent)
	, m_strGZQLX(_T(""))
	, m_nTDYTQMJ_Min(0)
	, m_nTDYTQMJ_Max(0)
	, m_strUnit(_T("平方米"))
	, m_strResultNum(_T(""))
{
	m_ipFramework = ipFramework;
}

CLPJSYDGZQCXDlg::~CLPJSYDGZQCXDlg()
{
}

void CLPJSYDGZQCXDlg::DoDataExchange(CDataExchange* pDX)
{
	CGSBCGDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GZQLXCOMBO, m_cbxGZQLX);
	DDX_CBString(pDX, IDC_GZQLXCOMBO, m_strGZQLX);

	DDX_Text(pDX, IDC_TDYTQMJMAX, m_nTDYTQMJ_Max);
	DDX_Text(pDX, IDC_TDYTQMJMIN, m_nTDYTQMJ_Min);
	DDX_Control(pDX, IDC_TDYTQMJMAX, m_editTDYTQMJ_Max);
	DDX_Control(pDX, IDC_TDYTQMJMIN, m_editTDYTQMJ_Min);	

	DDX_Text(pDX, IDC_UNITTEXT41, m_strUnit);
	DDX_Text(pDX, IDC_UNITTEXT42, m_strUnit);

	DDX_Control(pDX, IDC_GRIDAREA_4, m_RectForGrid);
	DDX_Text(pDX, IDC_RESCOUNT_4, m_strResultNum);
}


BEGIN_MESSAGE_MAP(CLPJSYDGZQCXDlg, CGSBCGDialog)
	ON_BN_CLICKED(IDC_METER_RD_4, &CLPJSYDGZQCXDlg::OnBnClickedMeterRd4)
	ON_BN_CLICKED(IDC_MU_RD_4, &CLPJSYDGZQCXDlg::OnBnClickedMuRd4)
	ON_BN_CLICKED(IDC_HECTARE_RD_4, &CLPJSYDGZQCXDlg::OnBnClickedHectareRd4)
	ON_BN_CLICKED(IDC_CXBUTTON_4, &CLPJSYDGZQCXDlg::OnBnClickedCxbutton4)
END_MESSAGE_MAP()


// CLPJSYDGZQCXDlg 消息处理程序

BOOL CLPJSYDGZQCXDlg::OnInitDialog()
{
	CGSBCGDialog::OnInitDialog();

	//初始化选中查询面积为平方米
	((CButton *)GetDlgItem(IDC_METER_RD_4))->SetCheck(TRUE);
	//初始化文本框控件
	m_editTDYTQMJ_Min.SetValidChars(_T("1234567890."));//可用字符列表
	m_editTDYTQMJ_Max.SetValidChars(_T("1234567890."));//可用字符列表

	m_editTDYTQMJ_Min.SetWindowText(_T("0"));
	m_editTDYTQMJ_Max.SetWindowText(_T("0"));

	//初始化管制区类型
	m_cbxGZQLX.AddString(_T("全部"));
	m_cbxGZQLX.AddString(_T("允许建设用地区"));
	m_cbxGZQLX.AddString(_T("有条件建设用地区"));
	m_cbxGZQLX.AddString(_T("限制建设用地区"));
	m_cbxGZQLX.AddString(_T("禁止建设用地区"));
	m_cbxGZQLX.SelectString(0,_T("全部"));

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
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CLPJSYDGZQCXDlg::InitGridCtrl()
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

void CLPJSYDGZQCXDlg::OnBnClickedMeterRd4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strUnit = _T("(平方米)");
	UpdateData(FALSE);
}

void CLPJSYDGZQCXDlg::OnBnClickedMuRd4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strUnit = _T("(亩)");
	UpdateData(FALSE);
}

void CLPJSYDGZQCXDlg::OnBnClickedHectareRd4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strUnit = _T("(公顷)");
	UpdateData(FALSE);
}

void CLPJSYDGZQCXDlg::OnBnClickedCxbutton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	//组织查询条件
	CString strArea = _T("");
	CString strGZQLX = _T("");
	CString strWhere = _T("");

	if (m_strGZQLX != _T("全部"))
	{
		if (m_strGZQLX == "允许建设用地区")
		{
			strGZQLX += "GZQLXDM='010'";
		}
		if (m_strGZQLX == "有条件建设用地区")
		{
			strGZQLX += "GZQLXDM='020'";
		}
		if (m_strGZQLX == "限制建设用地区")
		{
			strGZQLX += "GZQLXDM='030'";
		}
		if (m_strGZQLX == "禁止建设用地区")
		{
			strGZQLX += "GZQLXDM='040'";
		}
	}else
	{
		strGZQLX += "GZQLXDM='*'";
	}
	//查询条件,换算为平方米
	double nAREA_Max,nAREA_Min;
	if (m_strUnit == _T("米"))
	{
		nAREA_Max = m_nTDYTQMJ_Max;
		nAREA_Min = m_nTDYTQMJ_Min;
	}else if (m_strUnit == _T("(亩)"))
	{
		nAREA_Max = m_nTDYTQMJ_Max * 666.67;
		nAREA_Min = m_nTDYTQMJ_Min * 666.67;

	}else if (m_strUnit == _T("(公顷)"))
	{
		nAREA_Max = m_nTDYTQMJ_Max * 10000.0;
		nAREA_Min = m_nTDYTQMJ_Min * 10000.0;
	}
	//排除查询条件空值
	if (m_nTDYTQMJ_Min != 0)
	{
		CString strTmp;
		strTmp.Format(_T(" and TBMJ >= %lf "), nAREA_Min);
		strArea += strTmp;
	}
	if (m_nTDYTQMJ_Max != 0)
	{
		CString strTmp;
		strTmp.Format(_T(" and TBMJ <= %lf "), nAREA_Max);
		strArea += strTmp;
	}

	//合并查询条件
	strWhere +=  strGZQLX + strArea;

	IQueryFilterPtr ipQuery(CLSID_QueryFilter);
	ipQuery->put_SubFields(_T("*"));
	ipQuery->put_WhereClause(_bstr_t(strWhere));
	InitGridCtrl();
	ShowResrult(ipQuery);
}

BOOL CLPJSYDGZQCXDlg::ShowResrult(IQueryFilterPtr ipQuery)
{
	IFeatureClassPtr ipFtClass;
	m_ipFtWorkspace->OpenFeatureClass(_T("JSYDGZQ"),&ipFtClass);
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
