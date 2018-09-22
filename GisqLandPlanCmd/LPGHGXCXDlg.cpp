// LPGHGXCXDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LPGHGXCXDlg.h"


// CLPGHGXCXDlg �Ի���

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


// CLPGHGXCXDlg ��Ϣ�������

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
		//TODO��
	}
	//�������������
	m_wndMJLXCombo.AddString(_T("���"));
	m_wndMJLXCombo.AddString(_T("ũ�õ����"));
	m_wndMJLXCombo.AddString(_T("���ؾ����"));
	m_wndMJLXCombo.AddString(_T("�����õ����"));
	m_wndMJLXCombo.AddString(_T("δ���õ����"));
	m_wndMJLXCombo.AddString(_T("����ũ�ﾻ���"));
	m_wndMJLXCombo.SelectString(0,_T("���"));
	//ָ������������
	m_wndZBLXCombo.AddString(_T("ȫ��"));
	m_wndZBLXCombo.AddString(_T("���������õ�ʡ��ָ��"));
	m_wndZBLXCombo.AddString(_T("���������õ��м�ָ��"));
	m_wndZBLXCombo.AddString(_T("���������õ��ؼ�ָ��"));
	m_wndZBLXCombo.AddString(_T("���������õ��缶ָ��"));
	m_wndZBLXCombo.AddString(_T("����ũ��ʡ��ָ��"));
	m_wndZBLXCombo.AddString(_T("����ũ���м�ָ��"));
	m_wndZBLXCombo.AddString(_T("����ũ���ؼ�ָ��"));
	m_wndZBLXCombo.AddString(_T("����ũ���缶ָ��"));
	m_wndZBLXCombo.SelectString(0,_T("ȫ��"));
	//������������
	InitXZQComboTree(7);
	m_wndXZQCombo.SetSelectString(_T("[33]�㽭ʡ"));
	//�������ͣ�������ͣ�ָ������������
	InitComboBoxs();
	//�������ֱ༭����ַ��޶�
	m_wndMJMax.SetValidChars(_T("0123456789."));
	m_wndMJMin.SetValidChars(_T("0123456789."));
	m_wndZBMax.SetValidChars(_T("0123456789."));
	m_wndZBMin.SetValidChars(_T("0123456789."));
	//��ʼ��ʱ��ؼ�
	InitTimeCtrl();
	SetTimer(1,100,NULL);
	m_dateFromTime = m_wndFromTime.GetDate();
	m_dateToTime = m_wndToTime.GetDate();
	m_editFromTime.SetWindowText(_T("____-__-__"));
	m_editToTime.SetWindowText(_T("____-__-__"));

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CLPGHGXCXDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CBCGPDialog::OnCancel();
	//delete this;
}

void CLPGHGXCXDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
	//��֯��ǰ�ڵ�����
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
	//���뵱ǰ�ڵ�
	if (ItemData->nLevel > nStopLevel)
	{
		delete ItemData;
		return;
	}
	HTREEITEM hItem;
	if (ItemData->nLevel == 0)//ʡ
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(2),ItemData->strName);
		hItem = m_wndXZQCombo.AddItem(showValue,0,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 1)//��
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(2),ItemData->strName);
		hItem = m_wndXZQCombo.AddItem(showValue,1,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 2)//����
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(2),ItemData->strName);
		hItem = m_wndXZQCombo.AddItem(showValue,2,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 4)//����ֵ�
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(3),ItemData->strName);
		hItem = m_wndXZQCombo.AddItem(showValue,3,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 6)//��
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(3),ItemData->strName);
		hItem = m_wndXZQCombo.AddItem(showValue,-1,(DWORD)ItemData,parentItem);
	}
	//�����ݹ�����ӽڵ�
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

	//����������comboboxд�����õ��뷨������������д���ֵ�����Է������ͽ��ж����ò���
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
	{//ѭ�����������ֵ�������ҵ���Ӧ�ı�
		CString DictIDTmp;
		utf8ToUnicode(DictElement->FirstAttribute()->Value(),&DictIDTmp);
		if (DictIDTmp.Compare(_T("GXFALX")) == 0) break;
		DictElement = DictElement->NextSiblingElement();
	}
	if (DictElement != NULL)
	{
		TiXmlElement *FACodeNamesElement = DictElement->FirstChildElement("CodeNames");
		TiXmlElement *CodeNameElement = FACodeNamesElement->FirstChildElement();

		m_wndFALXCombo.AddString(_T("ȫ��"));
		while(CodeNameElement != NULL)
		{
			CString *strFALXDM = new CString;
			CString strFALXMC;
			utf8ToUnicode(CodeNameElement->Attribute("Code"),strFALXDM);
			utf8ToUnicode(CodeNameElement->Attribute("Name"),&strFALXMC);
			m_wndFALXCombo.SetItemData(m_wndFALXCombo.AddString(strFALXMC),(DWORD_PTR)strFALXDM);
			CodeNameElement = CodeNameElement->NextSiblingElement();
		}
		m_wndFALXCombo.SelectString(0,_T("ȫ��"));
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
// 	//��������
// 	TiXmlElement *FALXDictElement = GHGXCXConfigElement->FirstChildElement("FALXDict");
// 	TiXmlElement *FACodeNamesElement = FALXDictElement->FirstChildElement("CodeNames");
// 	TiXmlElement *CodeNameElement = FACodeNamesElement->FirstChildElement();
// 
// 	m_wndFALXCombo.AddString(_T("ȫ��"));
// 	while(CodeNameElement != NULL)
// 	{
// 		CString *strFALXDM = new CString;
// 		CString strFALXMC;
// 		utf8ToUnicode(CodeNameElement->Attribute("Code"),strFALXDM);
// 		utf8ToUnicode(CodeNameElement->Attribute("Name"),&strFALXMC);
// 		m_wndFALXCombo.SetItemData(m_wndFALXCombo.AddString(strFALXMC),(DWORD_PTR)strFALXDM);
// 		CodeNameElement = CodeNameElement->NextSiblingElement();
// 	}
// 	m_wndFALXCombo.SelectString(0,_T("ȫ��"));
// 
// 	//�������
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
// 	m_wndMJLXCombo.SelectString(0,_T("���"));
// 
// 	//ָ������
// 	TiXmlElement *ZBLXDictElement = GHGXCXConfigElement->FirstChildElement("ZBLXDict");
// 	TiXmlElement *ZBCodeNamesElement = ZBLXDictElement->FirstChildElement("CodeNames");
// 	CodeNameElement = ZBCodeNamesElement->FirstChildElement();
// 
// 	m_wndZBLXCombo.AddString(_T("ȫ��"));
// 	while(CodeNameElement != NULL)
// 	{
// 		CString *strZBLXDM = new CString;
// 		CString strZBLXMC;
// 		utf8ToUnicode(CodeNameElement->Attribute("Code"),strZBLXDM);
// 		utf8ToUnicode(CodeNameElement->Attribute("Name"),&strZBLXMC);
// 		m_wndZBLXCombo.SetItemData(m_wndZBLXCombo.AddString(strZBLXMC),(DWORD_PTR)strZBLXDM);
// 		CodeNameElement = CodeNameElement->NextSiblingElement();
// 	}
// 	m_wndZBLXCombo.SelectString(0,_T("ȫ��"));
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
	//������Щ����
	const UINT stateMask = 
		CBCGPDateTimeCtrl::DTM_SPIN |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR | 
		CBCGPDateTimeCtrl::DTM_DATE |
		CBCGPDateTimeCtrl::DTM_TIME24H |
		CBCGPDateTimeCtrl::DTM_CHECKBOX |
		CBCGPDateTimeCtrl::DTM_TIME | 
		CBCGPDateTimeCtrl::DTM_TIME24HBYLOCALE |
		CBCGPDateTimeCtrl::DTM_SECONDS;
	//�Ƿ�ʹ����Щ���ã�������д��
	UINT DataState = 
		CBCGPDateTimeCtrl::DTM_SPIN |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR | 
		CBCGPDateTimeCtrl::DTM_DATE;
	m_wndFromTime.SetState(DataState,stateMask);
	m_wndFromTime.m_monthFormat = 2;
	m_wndToTime.SetState(DataState,stateMask);
	m_wndToTime.m_monthFormat = 2;
	//��ʼ��ʱ���ı���
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
	//�������ֵ
	CComBSTR bstrSQLWhere;
	//��ѯ������
	CString strSQL = _T("");
	//��ѯ�ֶ�
	CString strSelect = _T("");
	//����
	CString strFromWhere = _T("");
	//��ѯ��������
	CString strWhere = _T("");
	//����ʽ
	CString strOrderBy = _T("");
	//��������
	CString strMCDM = _T("");
	//����������
	CString strXZQ = _T("");
	//������������
	CString strFALX = _T("");
	//ʱ������
	CString strFromTime = _T("");
	CString strToTime = _T("");
	//�������
	CString strMJ = _T("");
	//ָ������
	CString strZB = _T("");
	
	strSelect = _T("select distinct(f.bsm),f.famc\r\n ");
	strFromWhere = _T(" from ghgxfa f,ghgxxm x,ghgxdk d\r\n ");
	strWhere = _T(" where d.xmbsm = x.bsm and x.fabsm = f.bsm\r\n ");
	strOrderBy = _T(" order by f.famc");

	//�ȴ�������������
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
				strXZQ = _T("");//ʡ����ȫ��
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
				BCGPMessageBox(_T("��������Χ����"),_T("����"),MB_OK);
				return;
				break;
			}
		}
		if (strXZQ.GetLength() > 0)
		{
			strWhere += _T(" and ") + strXZQ;
		}
	}
	
	//����������Ϊ��ʱ������������
	if (m_strMCBH.IsEmpty() == FALSE)
	{
		//�������ƴ�������
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

	//��������������
	int nFALX = m_wndFALXCombo.GetCurSel();
	if (nFALX != CB_ERR)
	{
		//���ѡ�е���Ϊ0�У�˵��ѡ����ȫ��������������Բ�д
		if (nFALX != 0)
		{
			CString *strFALXDM = (CString*)m_wndFALXCombo.GetItemData(nFALX);
			strFALX.Format(_T(" FALX = '%s'"),*strFALXDM);
			strWhere += _T(" and ") + strFALX;
		}
	}else
	{
		BCGPMessageBox(_T("��ѡ�񷽰�����"),_T("����"),MB_OK);
		return;
	}

	//����ʱ�����������Ҫ����д����������Ϊ�ؼ����ܻỻ
	GetTimeSet(strFromTime,strToTime);
	if (strFromTime.GetLength() > 0)
	{
		strWhere += _T(" and ") + strFromTime;
	}
	if (strToTime.GetLength() > 0)
	{
		strWhere += _T(" and ") + strToTime;
	}

	//�����������
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
		BCGPMessageBox(_T("δѡ���������"),_T("����"),MB_OK);
		return;
	}
	
	//����ָ������
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
	}else if (nZBIndex == 0)//ѡ����ȫ��������Ҫ�����г�����ָ����������
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
		BCGPMessageBox(_T("��ѡ��ָ������"),_T("����"),MB_OK);
		return;
	}
	strSQL = strSelect + strFromWhere + strWhere + strOrderBy;
	bstrSQLWhere = strSQL;
	//�����Ǵ���ͣ������
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
	if (strFormTime.Compare(_T("________")) != 0)//��Ϊ��
	{
		strFormTime.Replace(_T("_"),_T("0"));
		nDate = _wtoi(strFormTime);
		nYear = nDate / 10000;
		nMonth = (nDate / 100) % 100;
		nDay = nDate % 100;
		strFormTime.Format(_T("'%d-%d-%d'"),nYear,nMonth,nDay);
		strFromTimeSQL.Format(_T(" f.PZSJ >= to_date(%s,'YYYY-MM-DD') "),strFormTime);
	}
	if (strToTime.Compare(_T("________")) != 0)//��Ϊ��
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
// 	//��ʼʱ��
// 	nYear = tFromTime.GetYear();
// 	nMonth = tFromTime.GetMonth();
// 	nDay = tFromTime.GetDay();
// 	strFormTime.Format(_T("'%d-%d-%d'"),nYear,nMonth,nDay);
// 	//��ֹʱ��
// 	nYear = tToTime.GetYear();
// 	nMonth = tToTime.GetMonth();
// 	nDay = tToTime.GetDay();
// 	strToTime.Format(_T("'%d-%d-%d'"),nYear,nMonth,nDay);

	//ƴ��ʱ���ѯ����
	//strTimeSQL.Format(_T(" f.PZSJ >= to_date(%s,'YYYY-MM-DD') and f.PZSJ <= to_date(%s,'YYYY-MM-DD')"),strFormTime,strToTime);
}
void CLPGHGXCXDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//��ʼʱ��
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
	//��ֹʱ��
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
	// TODO: �ڴ˴������Ϣ����������
}
