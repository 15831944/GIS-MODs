// LPGHGXCXDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LPGHGXCXDlg.h"


// CLPGHGXCXDlg 对话框

IMPLEMENT_DYNAMIC(CLPGHGXCXDlg, CBCGPDialog)

CLPGHGXCXDlg::CLPGHGXCXDlg(GisqLib::_DGisqFrameworkCOMPtr ipFramework,CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CLPGHGXCXDlg::IDD, pParent)
	, m_strMJMin(_T(""))
	, m_strMJMax(_T(""))
	, m_strZBMin(_T(""))
	, m_strZBMax(_T(""))
	, m_strMCBH(_T(""))
	, m_strFromTime(_T(""))
	, m_strToTime(_T(""))
{
	m_spFramework = ipFramework;
}

CLPGHGXCXDlg::~CLPGHGXCXDlg()
{
}

void CLPGHGXCXDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GHGXCX_XZQ, m_wndXZQCombo);
	DDX_Control(pDX, IDC_COMBO_FAXMLX, m_wndFALXCombo);
	DDX_Control(pDX, IDC_STARTTIMECTRL, m_wndFromTime);
	DDX_Control(pDX, IDC_STOPTIMECTRL, m_wndToTime);
	DDX_Control(pDX, IDC_COMBO_MJLX, m_wndMJLXCombo);
	DDX_Control(pDX, IDC_COMBO_ZBLX, m_wndZBLXCombo);
	DDX_Control(pDX, IDC_EDIT_MINAREA, m_wndMJMin);
	DDX_Control(pDX, IDC_EDIT_MAXAREA, m_wndMJMax);
	DDX_Control(pDX, IDC_EDIT_MINZB, m_wndZBMin);
	DDX_Control(pDX, IDC_EDIT_MAXZB, m_wndZBMax);
	DDX_Text(pDX, IDC_EDIT_MINAREA, m_strMJMin);
	DDX_Text(pDX, IDC_EDIT_MAXAREA, m_strMJMax);
	DDX_Text(pDX, IDC_EDIT_MINZB, m_strZBMin);
	DDX_Text(pDX, IDC_EDIT_MAXZB, m_strZBMax);
	DDX_Text(pDX, IDC_EDIT_MC_SLH, m_strMCBH);
	DDX_Control(pDX, IDC_EDIT1, m_editFromTime);
	DDX_Control(pDX, IDC_EDIT2, m_editToTime);
	DDX_Text(pDX, IDC_EDIT1, m_strFromTime);
	DDX_Text(pDX, IDC_EDIT2, m_strToTime);
}


BEGIN_MESSAGE_MAP(CLPGHGXCXDlg, CBCGPDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &CLPGHGXCXDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CLPGHGXCXDlg 消息处理程序

BOOL CLPGHGXCXDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	CComQIPtr<GisqLib::IGSDataExchangeS> spGSDataExchangeS = m_spFramework->GetDataExchangeS();
	BSTR bstrTag(_T("GHWS"));
	short index = spGSDataExchangeS->FindIndex(bstrTag);
	if (index >= 0)
	{
		CComQIPtr<GisqLib::IGSDataExchange> spGSDataExchange = spGSDataExchangeS->Item(index);
		CComVariant var = spGSDataExchange->Item(0);
		IWorkspacePtr spWorkspace = var.punkVal;
		m_ipFtWorkspace = spWorkspace;
		//TODO…
	}
	//面积类型下拉框
	m_wndMJLXCombo.AddString(_T("面积"));
	m_wndMJLXCombo.AddString(_T("农用地面积"));
	m_wndMJLXCombo.AddString(_T("耕地净面积"));
	m_wndMJLXCombo.AddString(_T("建设用地面积"));
	m_wndMJLXCombo.AddString(_T("未利用地面积"));
	m_wndMJLXCombo.AddString(_T("基本农田净面积"));
	m_wndMJLXCombo.SelectString(0,_T("面积"));
	//指标类型下拉框
	m_wndZBLXCombo.AddString(_T("全部"));
	m_wndZBLXCombo.AddString(_T("新增建设用地省级指标"));
	m_wndZBLXCombo.AddString(_T("新增建设用地市级指标"));
	m_wndZBLXCombo.AddString(_T("新增建设用地县级指标"));
	m_wndZBLXCombo.AddString(_T("新增建设用地乡级指标"));
	m_wndZBLXCombo.AddString(_T("基本农田省级指标"));
	m_wndZBLXCombo.AddString(_T("基本农田市级指标"));
	m_wndZBLXCombo.AddString(_T("基本农田县级指标"));
	m_wndZBLXCombo.AddString(_T("基本农田乡级指标"));
	m_wndZBLXCombo.SelectString(0,_T("全部"));
	//行政区下拉框
	InitXZQComboTree(7);
	m_wndXZQCombo.SetSelectString(_T("[33]浙江省"));
	//方案类型，面积类型，指标类型下拉框
	InitComboBoxs();
	//几个数字编辑框的字符限定
	m_wndMJMax.SetValidChars(_T("0123456789."));
	m_wndMJMin.SetValidChars(_T("0123456789."));
	m_wndZBMax.SetValidChars(_T("0123456789."));
	m_wndZBMin.SetValidChars(_T("0123456789."));
	//初始化时间控件
	InitTimeCtrl();
	SetTimer(1,100,NULL);
	m_dateFromTime = m_wndFromTime.GetDate();
	m_dateToTime = m_wndToTime.GetDate();
	m_editFromTime.SetWindowText(_T("____-__-__"));
	m_editToTime.SetWindowText(_T("____-__-__"));

	// TODO:  在此添加额外的初始化
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLPGHGXCXDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CBCGPDialog::OnCancel();
	//delete this;
}

void CLPGHGXCXDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CBCGPDialog::OnClose();
	//delete this;
}
BOOL CLPGHGXCXDlg::InitXZQComboTree(int nIndex)
{
	if (m_wndXZQCombo.m_wndTreeList.GetSafeHwnd() == NULL)
	{
		m_wndXZQCombo.InitTree(200,200);
	}
	m_wndXZQCombo.DeleteAllItems();
	CComQIPtr<ILPXZQDataTree> ipTree;
	CComQIPtr<ILPXZQDataItem> ipRoot;
	IUnknown *ipTmpItem;
	ipTree.CoCreateInstance(CLSID_LPXZQDataTree);
	ipTree->GetRootItem(m_ipFtWorkspace,&ipTmpItem);
	ipRoot = (CComQIPtr<ILPXZQDataItem>) ipTmpItem;
	if (ipRoot == NULL) return TRUE;
	BSTR bstrTmp;
	ipRoot->Get_ItemCode(&bstrTmp);
	m_strParent = bstrTmp;
	AddDistrictTreeItem(NULL,ipRoot,nIndex);
	HTREEITEM item = m_wndXZQCombo.m_wndTreeList.GetFirstVisibleItem();
	m_wndXZQCombo.m_wndTreeList.Expand(item,TVE_EXPAND);
	UpdateData(FALSE);
	return TRUE;
}

void CLPGHGXCXDlg::AddDistrictTreeItem(HTREEITEM parentItem/*=NULL*/,CComQIPtr<ILPXZQDataItem> spDataItem, int nStopLevel)
{
	//组织当前节点数据
	BSTR bstrTmp;
	short nTmpLevel;
	_tagDistrictItem *ItemData = new _tagDistrictItem;
	spDataItem->Get_ItemName(&bstrTmp);
	ItemData->strName = bstrTmp;
	spDataItem->Get_ItemCode(&bstrTmp);
	ItemData->strCode = bstrTmp;
	spDataItem->Get_ItemParentID(&bstrTmp);
	ItemData->strParentID = bstrTmp;
	spDataItem->Get_ItemLevel(&nTmpLevel);
	ItemData->nLevel = (int)nTmpLevel;
	//插入当前节点
	if (ItemData->nLevel > nStopLevel)
	{
		delete ItemData;
		return;
	}
	HTREEITEM hItem;
	if (ItemData->nLevel == 0)//省
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(2),ItemData->strName);
		hItem = m_wndXZQCombo.AddItem(showValue,0,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 1)//市
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(2),ItemData->strName);
		hItem = m_wndXZQCombo.AddItem(showValue,1,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 2)//县区
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(2),ItemData->strName);
		hItem = m_wndXZQCombo.AddItem(showValue,2,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 4)//乡镇街道
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(3),ItemData->strName);
		hItem = m_wndXZQCombo.AddItem(showValue,3,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 6)//村
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(3),ItemData->strName);
		hItem = m_wndXZQCombo.AddItem(showValue,-1,(DWORD)ItemData,parentItem);
	}
	//遍历递归插入子节点
	IUnknown *ipTmpItem = NULL;
	CComQIPtr<ILPXZQDataEnum> ipEnum;
	spDataItem->Get_ChildItems(&ipTmpItem);
	ipEnum = (CComQIPtr<ILPXZQDataEnum>) ipTmpItem;
	if (ipEnum == NULL) return ;
	int nCount;
	ipEnum->GetItemCount(&nCount);
	for (int i = 0; i < nCount; i++)
	{
		IUnknown* pItemTmp = NULL;
		ipEnum->GetItemAt(i,&pItemTmp);
		CComQIPtr<ILPXZQDataItem> spChlidDataItem = (CComQIPtr<ILPXZQDataItem>)pItemTmp;
		AddDistrictTreeItem(hItem,spChlidDataItem,nStopLevel);
	}

}

void CLPGHGXCXDlg::InitComboBoxs()
{
	USES_CONVERSION;

	//放弃将所有combobox写入配置的想法，将方案类型写入字典表，仅对方案类型进行读配置操作
	TCHAR szAppFullPath[MAX_PATH];
	DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(CHAR);
	GetModuleFileName(NULL,szAppFullPath,dBufferSize);
	CString strAppPath(szAppFullPath);
	strAppPath = strAppPath.Mid(0,strAppPath.ReverseFind('\\'));
	CString strConfigFile = strAppPath + _T("\\Config\\Rule.xml");

	TiXmlDocument XmlDoc;
	TiXmlElement *RootElement = NULL;
	if (XmlDoc.LoadFile(W2A(strConfigFile.AllocSysString())) == false) return;
	RootElement = XmlDoc.FirstChildElement();
	TiXmlElement *DictRootElement = RootElement->FirstChildElement("DictionaryRule");
	TiXmlElement *DictElement = DictRootElement->FirstChildElement();
	while(DictElement != NULL)
	{//循环遍历所有字典表名，找到对应的表
		CString DictIDTmp;
		utf8ToUnicode(DictElement->FirstAttribute()->Value(),&DictIDTmp);
		if (DictIDTmp.Compare(_T("GXFALX")) == 0) break;
		DictElement = DictElement->NextSiblingElement();
	}
	if (DictElement != NULL)
	{
		TiXmlElement *FACodeNamesElement = DictElement->FirstChildElement("CodeNames");
		TiXmlElement *CodeNameElement = FACodeNamesElement->FirstChildElement();

		m_wndFALXCombo.AddString(_T("全部"));
		while(CodeNameElement != NULL)
		{
			CString *strFALXDM = new CString;
			CString strFALXMC;
			utf8ToUnicode(CodeNameElement->Attribute("Code"),strFALXDM);
			utf8ToUnicode(CodeNameElement->Attribute("Name"),&strFALXMC);
			m_wndFALXCombo.SetItemData(m_wndFALXCombo.AddString(strFALXMC),(DWORD_PTR)strFALXDM);
			CodeNameElement = CodeNameElement->NextSiblingElement();
		}
		m_wndFALXCombo.SelectString(0,_T("全部"));
	}

// 	TCHAR szAppFullPath[MAX_PATH];
// 	DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(CHAR);
// 	GetModuleFileName(NULL,szAppFullPath,dBufferSize);
// 	CString strAppPath(szAppFullPath);
// 	strAppPath = strAppPath.Mid(0,strAppPath.ReverseFind('\\'));
// 	CString strConfigFile = strAppPath + _T("\\Config\\Config.xml");
// 
// 	TiXmlDocument XmlDoc;
// 	TiXmlElement *RootElement = NULL;
// 	if (XmlDoc.LoadFile(W2A(strConfigFile.AllocSysString())) == false) return;
// 	RootElement = XmlDoc.FirstChildElement();
// 	TiXmlElement *GHGXCXConfigElement = RootElement->FirstChildElement("GHGXCXConfig");
// 	//方案类型
// 	TiXmlElement *FALXDictElement = GHGXCXConfigElement->FirstChildElement("FALXDict");
// 	TiXmlElement *FACodeNamesElement = FALXDictElement->FirstChildElement("CodeNames");
// 	TiXmlElement *CodeNameElement = FACodeNamesElement->FirstChildElement();
// 
// 	m_wndFALXCombo.AddString(_T("全部"));
// 	while(CodeNameElement != NULL)
// 	{
// 		CString *strFALXDM = new CString;
// 		CString strFALXMC;
// 		utf8ToUnicode(CodeNameElement->Attribute("Code"),strFALXDM);
// 		utf8ToUnicode(CodeNameElement->Attribute("Name"),&strFALXMC);
// 		m_wndFALXCombo.SetItemData(m_wndFALXCombo.AddString(strFALXMC),(DWORD_PTR)strFALXDM);
// 		CodeNameElement = CodeNameElement->NextSiblingElement();
// 	}
// 	m_wndFALXCombo.SelectString(0,_T("全部"));
// 
// 	//面积类型
// 	TiXmlElement *MJLXDictElement = GHGXCXConfigElement->FirstChildElement("MJLXDict");
// 	TiXmlElement *MJCodeNamesElement = MJLXDictElement->FirstChildElement("CodeNames");
// 	CodeNameElement = MJCodeNamesElement->FirstChildElement();
// 
// 	while(CodeNameElement != NULL)
// 	{
// 		CString *strMJLXDM = new CString;
// 		CString strMJLXMC;
// 		utf8ToUnicode(CodeNameElement->Attribute("Code"),strMJLXDM);
// 		utf8ToUnicode(CodeNameElement->Attribute("Name"),&strMJLXMC);
// 		m_wndMJLXCombo.SetItemData(m_wndMJLXCombo.AddString(strMJLXMC),(DWORD_PTR)strMJLXDM);
// 		CodeNameElement = CodeNameElement->NextSiblingElement();
// 	}
// 	m_wndMJLXCombo.SelectString(0,_T("面积"));
// 
// 	//指标类型
// 	TiXmlElement *ZBLXDictElement = GHGXCXConfigElement->FirstChildElement("ZBLXDict");
// 	TiXmlElement *ZBCodeNamesElement = ZBLXDictElement->FirstChildElement("CodeNames");
// 	CodeNameElement = ZBCodeNamesElement->FirstChildElement();
// 
// 	m_wndZBLXCombo.AddString(_T("全部"));
// 	while(CodeNameElement != NULL)
// 	{
// 		CString *strZBLXDM = new CString;
// 		CString strZBLXMC;
// 		utf8ToUnicode(CodeNameElement->Attribute("Code"),strZBLXDM);
// 		utf8ToUnicode(CodeNameElement->Attribute("Name"),&strZBLXMC);
// 		m_wndZBLXCombo.SetItemData(m_wndZBLXCombo.AddString(strZBLXMC),(DWORD_PTR)strZBLXDM);
// 		CodeNameElement = CodeNameElement->NextSiblingElement();
// 	}
// 	m_wndZBLXCombo.SelectString(0,_T("全部"));
}

void CLPGHGXCXDlg::utf8ToUnicode(LPCSTR src, CString *Result)  
{  
	int wcsLen = MultiByteToWideChar( CP_UTF8, 0, src, -1, NULL, 0 );  
	LPWSTR result = new wchar_t[wcsLen + 1];
	::MultiByteToWideChar( CP_UTF8, 0, src, -1, result, wcsLen); 
	Result->Format(_T("%s"),result);
	delete result;
} 

void CLPGHGXCXDlg::InitTimeCtrl()
{
	//设置哪些配置
	const UINT stateMask = 
		CBCGPDateTimeCtrl::DTM_SPIN |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR | 
		CBCGPDateTimeCtrl::DTM_DATE |
		CBCGPDateTimeCtrl::DTM_TIME24H |
		CBCGPDateTimeCtrl::DTM_CHECKBOX |
		CBCGPDateTimeCtrl::DTM_TIME | 
		CBCGPDateTimeCtrl::DTM_TIME24HBYLOCALE |
		CBCGPDateTimeCtrl::DTM_SECONDS;
	//是否使用这些设置（不用则不写）
	UINT DataState = 
		CBCGPDateTimeCtrl::DTM_SPIN |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR | 
		CBCGPDateTimeCtrl::DTM_DATE;
	m_wndFromTime.SetState(DataState,stateMask);
	m_wndFromTime.m_monthFormat = 2;
	m_wndToTime.SetState(DataState,stateMask);
	m_wndToTime.m_monthFormat = 2;
	//初始化时间文本框
	m_editFromTime.EnableMask(
		_T("dddd dd dd"),	// The mask string
		_T("____-__-__"),	// Literal, "_" char = character entry
		_T('_'));		// Backspace replace char
	m_editFromTime.SetValidChars(_T("1234567890")); // Valid string characters 
	m_editFromTime.SetWindowText(_T("____-__-__"));
	m_editToTime.EnableMask(
		_T("DDDD DD DD"),	// The mask string
		_T("____-__-__"),	// Literal, "_" char = character entry
		_T('_'));		// Backspace replace char
	m_editToTime.SetValidChars(_T("1234567890")); // Valid string characters 
	m_editToTime.SetWindowText(_T("____-__-__"));
}
void CLPGHGXCXDlg::OnBnClickedOk()
{
	UpdateData();
	//用于最后赋值
	CComBSTR bstrSQLWhere;
	//查询语句汇总
	CString strSQL = _T("");
	//查询字段
	CString strSelect = _T("");
	//表名
	CString strFromWhere = _T("");
	//查询条件汇总
	CString strWhere = _T("");
	//排序方式
	CString strOrderBy = _T("");
	//名称条件
	CString strMCDM = _T("");
	//行政区条件
	CString strXZQ = _T("");
	//方案类型条件
	CString strFALX = _T("");
	//时间条件
	CString strFromTime = _T("");
	CString strToTime = _T("");
	//面积条件
	CString strMJ = _T("");
	//指标条件
	CString strZB = _T("");
	
	strSelect = _T("select distinct(f.bsm),f.famc\r\n ");
	strFromWhere = _T(" from ghgxfa f,ghgxxm x,ghgxdk d\r\n ");
	strWhere = _T(" where d.xmbsm = x.bsm and x.fabsm = f.bsm\r\n ");
	strOrderBy = _T(" order by f.famc");

	//先处理行政区条件
	HTREEITEM pItem = m_wndXZQCombo.GetSelectItem();
	if (pItem != NULL)
	{
		_tagDistrictItem *pDistrict = (_tagDistrictItem *)m_wndXZQCombo.m_wndTreeList.GetItemData(pItem);
		if (pDistrict != NULL)
		{
			int nLenth = pDistrict->strCode.GetLength();
			switch (nLenth)
			{
			case 2:
				strXZQ = _T("");//省级查全表
				break;
			case 4:
				strXZQ.Format(_T("f.SHIDM = '%s'"),pDistrict->strCode);
				break;
			case 6:
				strXZQ.Format(_T("f.XDM = '%s'"),pDistrict->strCode);
				break;
			case 9:
				strXZQ.Format(_T("d.XZDM = '%s'"),pDistrict->strCode);
				break;
			case 12:
				strXZQ.Format(_T("d.XZQDM = '%s'"),pDistrict->strCode);
				break;
			default:
				BCGPMessageBox(_T("行政区范围错误"),_T("错误"),MB_OK);
				return;
				break;
			}
		}
		if (strXZQ.GetLength() > 0)
		{
			strWhere += _T(" and ") + strXZQ;
		}
	}
	
	//名称条件不为空时处理名称条件
	if (m_strMCBH.IsEmpty() == FALSE)
	{
		//处理名称代码条件
		CString strMCDM_FA;
		CString strMCDM_XM;
		CString strMCDM_DK;
		strMCDM_FA.Format(_T(" f.FAMC like '%%%s%%' and f.BSM like '%%%s%%' and f.SLH like '%%%s%%' ")
			,m_strMCBH,m_strMCBH,m_strMCBH);
		strMCDM_XM.Format(_T(" x.XMMC like '%%%s%%' and x.BSM like '%%%s%%' "),m_strMCBH,m_strMCBH);
		strMCDM_DK.Format(_T("d.BSM like '%%%s%%'"),m_strMCBH);
		strMCDM = strMCDM_FA + _T(" and ") + strMCDM_XM + _T(" and ") + strMCDM_DK;

		strWhere += _T(" and ") + strMCDM;
	}

	//处理方案类型条件
	int nFALX = m_wndFALXCombo.GetCurSel();
	if (nFALX != CB_ERR)
	{
		//如果选中的项为0行，说明选的是全部，这个条件可以不写
		if (nFALX != 0)
		{
			CString *strFALXDM = (CString*)m_wndFALXCombo.GetItemData(nFALX);
			strFALX.Format(_T(" FALX = '%s'"),*strFALXDM);
			strWhere += _T(" and ") + strFALX;
		}
	}else
	{
		BCGPMessageBox(_T("请选择方案类型"),_T("错误"),MB_OK);
		return;
	}

	//处理时间条件，这个要另外写函数处理，因为控件可能会换
	GetTimeSet(strFromTime,strToTime);
	if (strFromTime.GetLength() > 0)
	{
		strWhere += _T(" and ") + strFromTime;
	}
	if (strToTime.GetLength() > 0)
	{
		strWhere += _T(" and ") + strToTime;
	}

	//处理面积条件
	int nMJIndex = m_wndMJLXCombo.GetCurSel();
	if (nMJIndex != CB_ERR)
	{
		CString *strMJFieldName = (CString*)m_wndMJLXCombo.GetItemData(nMJIndex);
		if (m_strMJMin.IsEmpty() != TRUE)
		{
			CString strMJMin;
			strMJMin.Format(_T(" f.%s >= %s "),*strMJFieldName,m_strMJMin);
			strWhere += _T(" and ") + strMJMin;
		}
		if (m_strMJMax.IsEmpty() != TRUE)
		{
			CString strMJMax;
			strMJMax.Format(_T(" f.%s <= %s"), *strMJFieldName,m_strMJMax);
			strWhere += _T(" and ") + strMJMax;
		}
	}else
	{
		BCGPMessageBox(_T("未选择面积类型"),_T("错误"),MB_OK);
		return;
	}
	
	//处理指标条件
	int nZBIndex = m_wndZBLXCombo.GetCurSel();
	if (nZBIndex > 0)
	{
		CString *strZBFieldName = (CString*)m_wndZBLXCombo.GetItemData(nZBIndex);
		if (m_strZBMin.IsEmpty() != TRUE)
		{
			CString strZBMin;
			strZBMin.Format(_T("f.%s >= %s"),*strZBFieldName,m_strZBMin);
			strWhere += _T(" and ") + strZBMin;
		}
		if (m_strZBMax.IsEmpty() != TRUE)
		{
			CString strZBMax;
			strZBMax.Format(_T("f.%s >= %s"),*strZBFieldName,m_strZBMax);
			strWhere += _T(" and ") + strZBMax;
		}
	}else if (nZBIndex == 0)//选择是全部，这里要遍历列出所有指标类型条件
	{
		for(int i = 1; i < m_wndZBLXCombo.GetCount(); i++)
		{
			CString *strZBFieldName = (CString*)m_wndZBLXCombo.GetItemData(i);
			if (m_strZBMin.IsEmpty() != TRUE)
			{
				CString strZBMin;
				strZBMin.Format(_T("f.%s >= %s"),*strZBFieldName,m_strZBMin);
				strWhere += _T(" and ") + strZBMin;
			}
			if (m_strZBMax.IsEmpty() != TRUE)
			{
				CString strZBMax;
				strZBMax.Format(_T("f.%s >= %s"),*strZBFieldName,m_strZBMax);
				strWhere += _T(" and ") + strZBMax;
			}
		}
	}else
	{
		BCGPMessageBox(_T("请选择指标类型"),_T("错误"),MB_OK);
		return;
	}
	strSQL = strSelect + strFromWhere + strWhere + strOrderBy;
	bstrSQLWhere = strSQL;
	//下面是创建停靠窗口
	CComQIPtr<GisqLib::IGSDockableWindow> spResDockwin;
	CComQIPtr<GisqLib::IGSDockableWindowManager> spGSDockableWindowManager = m_spFramework->GetDockableWindowManager();
	short nCount = 0;
	nCount = spGSDockableWindowManager->Count();
	for (short i = 0; i < nCount; i++)
	{
		CComQIPtr<GisqLib::IGSDockableWindow> spDockwin = spGSDockableWindowManager->Item(i);
		_bstr_t bstrGUID = spDockwin->GetDockWindowDefGUID();
		if (bstrGUID == _bstr_t("GisqLandPlanCmd.LPGHGXRstCmd.1"))
		{
			spResDockwin = spDockwin;
		}
	}
	if (spResDockwin == NULL)
	{
		CComQIPtr<GisqLib::IGSDockableWindow> spGHGXRstWnd = 
			spGSDockableWindowManager->CreateDockableWindowFromGUID(_bstr_t("GisqLandPlanCmd.LPGHGXRstCmd.1"));
		spGHGXRstWnd->Dock(GisqLib::DWS_ALIGN_BOTTOM);
		spGHGXRstWnd->ShowWindow(VARIANT_TRUE);
		CComQIPtr<ILPGHGXRstCmd> spResDlg = spGHGXRstWnd->GetDockableWindowTabDef();
		spResDlg->SearchFA(bstrSQLWhere);
	}else
	{
		spResDockwin->ShowWindow(VARIANT_TRUE);
		CComQIPtr<ILPGHGXRstCmd> spResDlg = spResDockwin->GetDockableWindowTabDef();
		spResDlg->SearchFA(bstrSQLWhere);
	}
}

void CLPGHGXCXDlg::GetTimeSet(CString &strFromTimeSQL,CString &strToTimeSQL)
{
	CString strFormTime = m_strFromTime;
	CString strToTime = m_strToTime;
	int nDate;
	int nYear = 0;
	int nMonth = 0;
	int nDay = 0;
	if (strFormTime.Compare(_T("________")) != 0)//不为空
	{
		strFormTime.Replace(_T("_"),_T("0"));
		nDate = _wtoi(strFormTime);
		nYear = nDate / 10000;
		nMonth = (nDate / 100) % 100;
		nDay = nDate % 100;
		strFormTime.Format(_T("'%d-%d-%d'"),nYear,nMonth,nDay);
		strFromTimeSQL.Format(_T(" f.PZSJ >= to_date(%s,'YYYY-MM-DD') "),strFormTime);
	}
	if (strToTime.Compare(_T("________")) != 0)//不为空
	{
		strToTime.Replace(_T("_"),_T("0"));
		nDate = _wtoi(strToTime);
		nYear = nDate / 10000;
		nMonth = (nDate / 100) % 100;
		nDay = nDate % 100;
		strToTime.Format(_T("'%d-%d-%d'"),nYear,nMonth,nDay);
		strToTimeSQL.Format(_T(" f.PZSJ <= to_date(%s,'YYYY-MM-DD') "),strToTime);
	}

// 	CString strFormTime = _T("");
// 	CString strToTime = _T("");

// 	DATE dFromTime;
// 	DATE dToTime;
// 
// 	dFromTime = m_wndFromTime.GetDate();
// 	dToTime = m_wndToTime.GetDate();
// 	COleDateTime tFromTime(dFromTime);
// 	COleDateTime tToTime(dToTime);
// 
// 	//起始时间
// 	nYear = tFromTime.GetYear();
// 	nMonth = tFromTime.GetMonth();
// 	nDay = tFromTime.GetDay();
// 	strFormTime.Format(_T("'%d-%d-%d'"),nYear,nMonth,nDay);
// 	//截止时间
// 	nYear = tToTime.GetYear();
// 	nMonth = tToTime.GetMonth();
// 	nDay = tToTime.GetDay();
// 	strToTime.Format(_T("'%d-%d-%d'"),nYear,nMonth,nDay);

	//拼接时间查询条件
	//strTimeSQL.Format(_T(" f.PZSJ >= to_date(%s,'YYYY-MM-DD') and f.PZSJ <= to_date(%s,'YYYY-MM-DD')"),strFormTime,strToTime);
}
void CLPGHGXCXDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//起始时间
	if (m_dateFromTime != m_wndFromTime.GetDate())
	{
		UpdateData();
		CString strFormTime = _T("");
		int nYear = 0;
		int nMonth = 0;
		int nDay = 0;

		m_dateFromTime = m_wndFromTime.GetDate();
		COleDateTime tFromTime(m_dateFromTime);
		nYear = tFromTime.GetYear();
		nMonth = tFromTime.GetMonth();
		nDay = tFromTime.GetDay();
		strFormTime.Format(_T("%d-%02d-%02d"),nYear,nMonth,nDay);
		m_editFromTime.SetWindowText(strFormTime);
	}
	//截止时间
	if (m_dateToTime != m_wndToTime.GetDate())
	{
		UpdateData();
		CString strToTime = _T("");
		int nYear = 0;
		int nMonth = 0;
		int nDay = 0;

		m_dateToTime = m_wndToTime.GetDate();
		COleDateTime tToTime(m_dateToTime);
		nYear = tToTime.GetYear();
		nMonth = tToTime.GetMonth();
		nDay = tToTime.GetDay();
		strToTime.Format(_T("%d-%02d-%02d"),nYear,nMonth,nDay);
		m_strToTime;
		m_editToTime.SetWindowText(strToTime);
		UpdateData(FALSE);
	}

	CBCGPDialog::OnTimer(nIDEvent);

}

void CLPGHGXCXDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CBCGPDialog::OnActivate(nState, pWndOther, bMinimized);
	((CWnd*)GetDlgItem(IDC_EDIT_MC_SLH))->SetFocus();
	// TODO: 在此处添加消息处理程序代码
}
