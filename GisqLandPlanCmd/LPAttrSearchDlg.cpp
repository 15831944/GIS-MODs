// LPAttrSearchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LPAttrSearchDlg.h"

// CLPAttrSearchDlg �Ի���

IMPLEMENT_DYNAMIC(CLPAttrSearchDlg, CGSBCGDialog)

CLPAttrSearchDlg::CLPAttrSearchDlg(GisqLib::_DGisqFrameworkCOMPtr pFrameworkCom, CWnd* pParent /*=NULL*/)
	: CGSBCGDialog(CLPAttrSearchDlg::IDD, pParent)
	, m_strUnit(_T(""))
// 	, m_dbAreaSizeMin(0)
// 	, m_dbAreaSizeMax(0)
	, m_strAreaSizeMin(_T(""))
	, m_strAreaSizeMax(_T(""))
	, m_strDLLX(_T("��������"))
{
	m_pFrameworkCom = pFrameworkCom;
	m_pStaticAreaName = NULL;
	m_pComboboxAreaName = NULL;
	m_strSelXZQDM = _T("");
	m_strSelXZQParent = _T("");
	m_nXZQLevel = 6;
}

CLPAttrSearchDlg::~CLPAttrSearchDlg()
{
}

void CLPAttrSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CGSBCGDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COXTARGETTYPE, m_wndTargetType);
	DDX_Control(pDX, IDC_CBX_XZQ, m_wndXZQTree);
	DDX_Control(pDX, IDC_CBX_DLMC, m_wndDLMC);
	DDX_Text(pDX, IDC_UNITTEXT, m_strUnit);
	DDX_Control(pDX, IDC_AREANAME, m_rectAreaName);
	DDX_Text(pDX, IDC_EDITMINAREA, m_strAreaSizeMin);
	DDX_Text(pDX, IDC_EDITMAXAREA, m_strAreaSizeMax);
	DDX_Control(pDX, IDC_EDITMINAREA, m_editAreaSizeMin);
	DDX_Control(pDX, IDC_EDITMAXAREA, m_editAreaSizeMax);
	DDX_Text(pDX, IDC_DLLX_TEXT, m_strDLLX);
}


BEGIN_MESSAGE_MAP(CLPAttrSearchDlg, CGSBCGDialog)
	ON_CBN_SELCHANGE(IDC_COXTARGETTYPE, &CLPAttrSearchDlg::OnCbnSelchangeCoxtargettype)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_UNIT_METER, &CLPAttrSearchDlg::OnBnClickedUnitMeter)
	ON_BN_CLICKED(IDC_UNIT_MU, &CLPAttrSearchDlg::OnBnClickedUnitMu)
	ON_BN_CLICKED(IDC_UNIT_HECTARE, &CLPAttrSearchDlg::OnBnClickedUnitHectare)
	//ON_BN_CLICKED(ID_GHYS_SEARCH, &CLPAttrSearchDlg::OnBnClickedOk)
	//ON_CBN_SELCHANGE(IDC_CBX_DLMC, &CLPAttrSearchDlg::OnCbnSelchangeCbxDlmc)
	ON_MESSAGE(WM_SELECTITEM_CHANGE,&CLPAttrSearchDlg::OnCbnSelchangeCbxDlmc)
	ON_BN_CLICKED(IDC_GH_SEARCH, &CLPAttrSearchDlg::OnBnClickedGhysSearch)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CLPAttrSearchDlg ��Ϣ�������

BOOL CLPAttrSearchDlg::OnInitDialog()
{
	CGSBCGDialog::OnInitDialog();

	m_editAreaSizeMin.SetValidChars(_T("1234567890."));//�����ַ��б�
	m_editAreaSizeMax.SetValidChars(_T("1234567890."));//�����ַ��б�
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CComQIPtr<GisqLib::IGSDataExchangeS> spGSDataExchangeS = m_pFrameworkCom->GetDataExchangeS();
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
	InitComboTree();

	//��ʼ��ѡ�в�ѯ���Ϊƽ����
	((CButton *)GetDlgItem(IDC_UNIT_METER))->SetCheck(TRUE);
	m_strUnit = _T("(ƽ����)");
	//��ʼ����ѯĿ�������˵�
	m_wndTargetType.AddString(_T("�滮��;"));
	m_wndTargetType.AddString(_T("�����õع�����"));
	m_wndTargetType.AddString(_T("���ڵ���ͼ��"));
	m_wndTargetType.AddString(_T("���ع滮����"));
	m_wndTargetType.AddString(_T("������;��"));
	//��ʼѡ�й滮��;
	m_wndTargetType.SelectString(0,_T("�滮��;"));
	InitXZQComboTree(6);
	HTREEITEM pItem = m_wndXZQTree.m_wndTreeList.GetRootItem();
	CString strItemText = m_wndXZQTree.m_wndTreeList.GetItemText(pItem);
	m_wndXZQTree.SetSelectString(strItemText);
	//��ʼ������������������ѡ��ȫ��
	InitDLMCComboTree(_T("GHYTDM"));
	m_mapAreaName.SetAt(_T("���"),_T("MJ"));
	ReFleshAreaNameCtrl();
	//m_mapAreaName.SetAt(_T("�����"),_T("JMJ"));
// 	AddAreaEditCtrl(_T("���"),_T("MJ"));
// 	AddAreaEditCtrl(_T("�����"),_T("JMJ"));
	m_strFeatureClassName = _T("GHYT");
	m_strDLDMFieldName = _T("GHYTDM");
	m_wndDLMC.SetSelectString(_T("ȫ��"));


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CLPAttrSearchDlg::OnCbnSelchangeCoxtargettype()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	//edit by lcc 2017/2/22 ��ӱ�������������
	//�Ȼ�ȡѡ�������������¼��ǰˢ�µ�����������С����
	m_pSelXZQitem = NULL;
	HTREEITEM pXZQItem = m_wndXZQTree.GetSelectItem();
	if (pXZQItem != NULL)
	{
		_tagDistrictItem *pItemData = (_tagDistrictItem*)m_wndXZQTree.m_wndTreeList.GetItemData(pXZQItem);
		if (pItemData != NULL)
		{
			m_strSelXZQDM = pItemData->strCode;
			m_strSelXZQParent = pItemData->strParentID;
		}else
		{
			m_strSelXZQDM = _T("");
			m_strSelXZQParent = _T("");
		}
	}else
	{
		m_strSelXZQDM = _T("");
		m_strSelXZQParent = _T("");
	}
	int nIndex = m_wndTargetType.GetCurSel();
	if (nIndex >= 0)
	{
		m_mapAreaName.RemoveAll();
//		ResetWnd();
		switch(nIndex)//���ﴫ����������������������볤�ȣ����������ƴ��ֵ��ID
		{
		case 0://
			InitXZQComboTree(6);
			m_nXZQLevel = 6;
			InitDLMCComboTree(_T("GHYTDM"));
			m_mapAreaName.SetAt(_T("���"),_T("MJ"));
			//m_mapAreaName.SetAt(_T("�����"),_T("JMJ"));
// 			AddAreaEditCtrl(_T("���"),_T("MJ"));
// 			AddAreaEditCtrl(_T("�����"),_T("JMJ"));
			m_strFeatureClassName = _T("GHYT");
			m_strDLDMFieldName = _T("GHYTDM");
			((CButton *)GetDlgItem(IDC_UNIT_METER))->SetCheck(TRUE);
			m_strUnit = _T("(ƽ����)");
			m_strDLLX = _T("��������");
			break;
		case 1://�����õع�������ѯ
			InitXZQComboTree(4);
			m_nXZQLevel = 4;
			InitDLMCComboTree(_T("JSYDGZQLX"));
			m_mapAreaName.SetAt(_T("���������"),_T("GZQMJ"));
//			AddAreaEditCtrl(_T("���������"),_T("GZQMJ"));
			m_strFeatureClassName = _T("JSYDGZQ");
			m_strDLDMFieldName = _T("GZQLXDM");
			((CButton *)GetDlgItem(IDC_UNIT_HECTARE))->SetCheck(TRUE);
			m_strUnit = _T("(����)");
			m_strDLLX = _T("����������");
			break;
		case 2://�滮���ڵ���ͼ�߲�ѯ
			InitXZQComboTree(6);
			m_nXZQLevel = 4;
			InitDLMCComboTree(_T("GHDLDM"));
			m_mapAreaName.SetAt(_T("ͼ�����"),_T("TBMJ"));
			m_mapAreaName.SetAt(_T("ͼ�ߵ������"),_T("TBDLMJ"));
// 			AddAreaEditCtrl(_T("ͼ�����"),_T("TBMJ"));
// 			AddAreaEditCtrl(_T("ͼ�ߵ������"),_T("TBDLMJ"));
			m_strFeatureClassName = _T("JQDLTB");
			m_strDLDMFieldName = _T("DLDM");
			((CButton *)GetDlgItem(IDC_UNIT_METER))->SetCheck(TRUE);
			m_strUnit = _T("(ƽ����)");
			m_strDLLX = _T("��������");
			break;
		case 3://�滮����ͼ�߲�ѯ
			InitXZQComboTree(6);
			m_nXZQLevel = 6;
			InitDLMCComboTree(_T("GHDLDM"));
			m_mapAreaName.SetAt(_T("�滮�������"),_T("GHDLMJ"));
//			AddAreaEditCtrl(_T("�滮�������"),_T("GHDLMJ"));
			m_strFeatureClassName = _T("TDGHDL");
			m_strDLDMFieldName = _T("GHDLDM");
			((CButton *)GetDlgItem(IDC_UNIT_METER))->SetCheck(TRUE);
			m_strUnit = _T("(ƽ����)");
			m_strDLLX = _T("��������");
			break;
		case 4://�滮����;�ؿ��ѯ
			InitXZQComboTree(4);
			m_nXZQLevel = 4;
			InitDLMCComboTree(_T("TDYTQLX"));
			m_mapAreaName.SetAt(_T("������;�����"),_T("TDYTQMJ"));
//			AddAreaEditCtrl(_T("������;�����"),_T("TDYTQMJ"));
			m_strFeatureClassName = _T("TDYTQ");
			m_strDLDMFieldName = _T("TDYTQLXDM");
			((CButton *)GetDlgItem(IDC_UNIT_HECTARE))->SetCheck(TRUE);
			m_strUnit = _T("(����)");
			m_strDLLX = _T("��;������");
			break;
		default:
			break;
		}
		m_wndDLMC.ResetContent();
		m_wndDLMC.SetSelectString(_T("ȫ��"));
		//����ˢ��ǰ��ȡ������������ˢ��������
		m_wndXZQTree.ResetContent();
		if (m_pSelXZQitem != NULL)
		{
			//m_wndXZQTree.SetSelectItem(m_pSelXZQitem,NULL);
			m_wndXZQTree.SetSelectString(m_wndXZQTree.m_wndTreeList.GetItemText(m_pSelXZQitem));
		}else
		{
			HTREEITEM pItem = m_wndXZQTree.m_wndTreeList.GetRootItem();
			CString strItemText = m_wndXZQTree.m_wndTreeList.GetItemText(pItem);
			m_wndXZQTree.SetSelectString(strItemText);
			//m_wndXZQTree.SetSelectString(_T("[33]�㽭ʡ"));
		}
		//SelectXZQbyCode(strSelXZQDM,strSelXZQParent,nXZQLevel);
		ReFleshAreaNameCtrl();
		UpdateData(FALSE);
	}
}
HRESULT CLPAttrSearchDlg::SelectXZQbyCode(CString &strCode, CString &strParentCode,int nLevel)
{
	//�Ȱ��յȼ�����
	CString strProvinceCode;
	CString strCityCode;
	CString strCountyCode;
	CString strTownCode;
	CString strVillageCode;
	if (strCode.GetLength() == 12)//�弶������
	{
	}else
	{

	}
	
	return S_OK;
}

BOOL CLPAttrSearchDlg::InitXZQComboTree(int nIndex)
{
	m_wndXZQTree.DeleteAllItems();
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
	HTREEITEM item = m_wndXZQTree.m_wndTreeList.GetFirstVisibleItem();
	m_wndXZQTree.m_wndTreeList.Expand(item,TVE_EXPAND);
	UpdateData(FALSE);
	return TRUE;
}

BOOL CLPAttrSearchDlg::InitDLMCComboTree(CString DictID)
{
	m_wndDLMC.DeleteAllItems();
	USES_CONVERSION;

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
	TiXmlElement *DictRootElement;
	DictRootElement = RootElement->FirstChildElement("DictionaryRule");
	TiXmlElement *DictElement;
	DictElement = DictRootElement->FirstChildElement();
	while(DictElement != NULL)
	{//ѭ�����������ֵ�������ҵ���Ӧ�ı�
		CString DictIDTmp;
		utf8ToUnicode(DictElement->FirstAttribute()->Value(),&DictIDTmp);
		if (DictIDTmp.Compare(DictID) == 0) break;
		DictElement = DictElement->NextSiblingElement();
	}
	if (DictElement == NULL) return FALSE;// û�ҵ�
	m_wndDLMC.AddItem(_T("ȫ��"),-1,0,NULL);
	InsertDLMCTreeItems(DictElement,NULL);

	return TRUE;
}
void CLPAttrSearchDlg::InsertDLMCTreeItems(TiXmlElement *CodeNamesElement, HTREEITEM pParentItem)
{
	TiXmlElement *CodeElement;
	CodeElement = CodeNamesElement->FirstChildElement()->FirstChildElement();
	while(CodeElement != NULL)
	{
		CString strCode;
		CString strName;
		utf8ToUnicode(CodeElement->Attribute("Code"),&strCode);
		utf8ToUnicode(CodeElement->Attribute("Name"),&strName);
		//add by lcc 2016.12.28
		CString strShowValue = _T("");
		strShowValue = _T("(") + strCode +_T(")") + strName;

		HTREEITEM pItem = m_wndDLMC.AddItem(strShowValue,0,(DWORD)strCode.AllocSysString(),pParentItem);
		if (CodeElement->FirstChildElement() != NULL) InsertDLMCTreeItems(CodeElement,pItem);
		CodeElement = CodeElement->NextSiblingElement();
	}

}

void CLPAttrSearchDlg::InitComboTree()
{
	if (m_wndXZQTree.m_wndTreeList.GetSafeHwnd() == NULL)
	{
		m_wndXZQTree.InitTree(200,200);
		CImageList *TreeImage = NULL;
		InitImageList(&TreeImage);
		m_wndXZQTree.m_wndTreeList.SetImageList(TreeImage,4);
	}
	if (m_wndDLMC.m_wndTreeList.GetSafeHwnd() == NULL)
	{
		m_wndDLMC.InitTree(200,200);
	}
}
void CLPAttrSearchDlg::InitImageList(CImageList** ppImageList)
{
	CImageList *TreeImage = new CImageList;
	TreeImage->Create(32, 32, ILC_COLOR16 | ILC_MASK,0,4);
	CBitmap *BitmapTmp = new CBitmap;
	BitmapTmp->LoadBitmap(IDB_BMP_ICON_SHENG);
	TreeImage->Add(BitmapTmp,RGB(255,255,255));

	BitmapTmp = NULL;
	BitmapTmp = new CBitmap;
	BitmapTmp->LoadBitmap(IDB_BMP_ICON_SHI);
	TreeImage->Add(BitmapTmp,RGB(255,255,255));

	BitmapTmp = NULL;
	BitmapTmp = new CBitmap;
	BitmapTmp->LoadBitmap(IDB_BMP_ICON_XIANQU);
	TreeImage->Add(BitmapTmp,RGB(255,255,255));

	BitmapTmp = NULL;
	BitmapTmp = new CBitmap;
	BitmapTmp->LoadBitmap(IDB_BMP_ICON_XIANGZHEN);
	TreeImage->Add(BitmapTmp,RGB(255,255,255));
	*ppImageList = TreeImage;
}

void CLPAttrSearchDlg::AddDistrictTreeItem(HTREEITEM parentItem/*=NULL*/,CComQIPtr<ILPXZQDataItem> spDataItem, int nStopLevel)
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
		hItem = m_wndXZQTree.AddItem(showValue,0,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 1)//��
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(2),ItemData->strName);
		hItem = m_wndXZQTree.AddItem(showValue,1,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 2)//����
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(2),ItemData->strName);
		hItem = m_wndXZQTree.AddItem(showValue,2,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 4)//����ֵ�
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(3),ItemData->strName);
		hItem = m_wndXZQTree.AddItem(showValue,3,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 6)//��
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(3),ItemData->strName);
		hItem = m_wndXZQTree.AddItem(showValue,-1,(DWORD)ItemData,parentItem);
	}
	//�������������͸��ڵ�����뵱ǰ��¼����ͬ����¼��ǰ�ڵ�
	if (m_strSelXZQDM.Compare(ItemData->strCode) == 0 && m_strSelXZQParent.Compare(ItemData->strParentID) == 0)
	{
		m_pSelXZQitem = hItem;
		//���ѡ���˴壬����ǰ��ʼ����ͼ���������ѡ�񸸽ڵ���������
	}else if (m_strSelXZQParent.Compare(ItemData->strCode) == 0 && m_nXZQLevel > nStopLevel)
	{
		m_pSelXZQitem = hItem;
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
void CLPAttrSearchDlg::utf8ToUnicode(LPCSTR src, CString *Result)  
{  
	int wcsLen = MultiByteToWideChar( CP_UTF8, 0, src, -1, NULL, 0 );  
	LPWSTR result = new wchar_t[wcsLen + 1];
	::MultiByteToWideChar( CP_UTF8, 0, src, -1, result, wcsLen); 
	Result->Format(_T("%s"),result);
	delete result;
} 

// void CLPAttrSearchDlg::AddAreaEditCtrl(CString strFieldAliaName, CString strFieldName)
// {
// 	CRect rc;
// 	GetWindowRect(&rc);
// 	if (rc.Height() <= 0 || rc.Width() <= 0) return;
// 	rc.bottom += 30;
// 	MoveWindow(rc);
// 	//���������ƶ�������ť
// 	CRect ButtonRc;
// 	CWnd *wndButton = NULL;
// 	wndButton = GetDlgItem(IDOK);
// 	wndButton->GetWindowRect(&ButtonRc);
// 	ScreenToClient(&ButtonRc);
// 	ButtonRc.top += 30;
// 	ButtonRc.bottom += 30;
// 	wndButton->MoveWindow(ButtonRc);
// 
// 	wndButton = GetDlgItem(IDCANCEL);
// 	wndButton->GetWindowRect(&ButtonRc);
// 	ScreenToClient(&ButtonRc);
// 	ButtonRc.top += 30;
// 	ButtonRc.bottom += 30;
// 	wndButton->MoveWindow(ButtonRc);
// 	//����һ�����������
// 	CFont *ft = this->GetFont();
// 	GetClientRect(rc);
// 	CAreaSetCtrl *NewCtrlLine = new CAreaSetCtrl(((rc.left + rc.right) / 2),(rc.bottom - 60),strFieldAliaName,strFieldName,this,ft,m_strUnit);
// 	m_AreaSetArray.Add(NewCtrlLine);
//
//}

// void CLPAttrSearchDlg::ResetWnd()
// {
// 	if (m_AreaSetArray.GetCount() == 0) return;
// 	CRect rc;
// 	GetWindowRect(&rc);
// 	if (rc.Height() <= 0 || rc.Width() <= 0) return;
// 	rc.bottom -= (30 * m_AreaSetArray.GetCount());
// 	MoveWindow(rc);
// 	//���������ƶ�������ť
// 	CRect ButtonRc;
// 	CWnd *wndButton = NULL;
// 	wndButton = GetDlgItem(IDOK);
// 	wndButton->GetWindowRect(&ButtonRc);
// 	ScreenToClient(&ButtonRc);
// 	ButtonRc.top -= (30 * m_AreaSetArray.GetCount());
// 	ButtonRc.bottom -= (30 * m_AreaSetArray.GetCount());
// 	wndButton->MoveWindow(ButtonRc);
// 
// 	wndButton = GetDlgItem(IDCANCEL);
// 	wndButton->GetWindowRect(&ButtonRc);
// 	ScreenToClient(&ButtonRc);
// 	ButtonRc.top -= (30 * m_AreaSetArray.GetCount());
// 	ButtonRc.bottom -= (30 * m_AreaSetArray.GetCount());
// 	wndButton->MoveWindow(ButtonRc);
// 
// 	while(m_AreaSetArray.GetCount() != 0)
// 	{
// 		delete m_AreaSetArray.GetAt(0);
// 		m_AreaSetArray.RemoveAt(0);
// 	}
//}

void CLPAttrSearchDlg::OnSize(UINT nType, int cx, int cy)
{
	CGSBCGDialog::OnSize(nType, cx, cy);
	
	// TODO: �ڴ˴������Ϣ����������
}

// CAreaSetCtrl::CAreaSetCtrl(int nCenter, int ntop, CString strFieldAliaName, CString strFieldName, CWnd *pParent, CFont *AreaCtrlFont, CString strUnit)
// {
// 	if (nCenter == 0 || ntop == 0) return;
// 	m_strMJFieldName = strFieldName;//�����ֶ������ڲ�ѯ
// 	m_font = AreaCtrlFont;
// 	//�����������
// 	CRect AreaNameRect;
// 	AreaNameRect.top = ntop;
// 	AreaNameRect.bottom = ntop + 20;
// 	AreaNameRect.left = nCenter - 60;
// 	AreaNameRect.right = nCenter + 60;
// 	CString showvalue;
// 	showvalue.Format(_T("<=%s<="),strFieldAliaName);
// 	m_wndAreaName = new CStatic;
// 	m_wndAreaName->Create(showvalue, WS_CHILD | WS_VISIBLE | ES_CENTER, AreaNameRect, pParent);
// 	m_wndAreaName->SetFont(AreaCtrlFont);
// 	//��Сֵ�༭��͵�λ�ı�
// // 	CRect MinAreaUnitRect;
// // 	MinAreaUnitRect.top = ntop;
// // 	MinAreaUnitRect.bottom = ntop + 20;
// // 	MinAreaUnitRect.left = nCenter - 120;
// // 	MinAreaUnitRect.right = nCenter - 60;
// // 	m_wndMinAreaUnit = new CStatic;
// // 	showvalue.Format(_T("(%s)"),strUnit);
// // 	m_wndMinAreaUnit->Create(showvalue, WS_CHILD | WS_VISIBLE , MinAreaUnitRect, pParent);
// // 	m_wndMinAreaUnit->SetFont(AreaCtrlFont);
// 
// 	CRect MinAreaEditRect;
// 	MinAreaEditRect.top = ntop;
// 	MinAreaEditRect.bottom = ntop + 20;
// 	MinAreaEditRect.left = nCenter - 200;
// 	MinAreaEditRect.right = nCenter - 140;
// 	m_wndMinArea = new CBCGPMaskEdit;
// 	m_wndMinArea->Create( WS_CHILD | WS_VISIBLE, MinAreaEditRect, pParent, uIDEdit);
// 	m_wndMinArea->SetFont(AreaCtrlFont);
// 	m_wndMinArea->SetValidChars(_T("1234567890."));//�����ַ��б�
// 	uIDEdit++;
// 	//���ֵ�༭��͵�λ�ı�
// 	CRect MaxAreaUnitRect;
// 	MaxAreaUnitRect.top = ntop;
// 	MaxAreaUnitRect.bottom = ntop + 20;
// 	MaxAreaUnitRect.left = nCenter + 160;
// 	MaxAreaUnitRect.right = nCenter + 220;
// 	m_wndMaxAreaUnit = new CStatic;
// 	showvalue.Format(_T("(%s)"),strUnit);
// 	m_wndMaxAreaUnit->Create(showvalue, WS_CHILD | WS_VISIBLE, MaxAreaUnitRect, pParent);
// 	m_wndMaxAreaUnit->SetFont(AreaCtrlFont);
// 
// 	CRect MaxAreaEditRect;
// 	MaxAreaEditRect.top = ntop;
// 	MaxAreaEditRect.bottom = ntop + 20;
// 	MaxAreaEditRect.left = nCenter + 80;
// 	MaxAreaEditRect.right = nCenter + 140;
// 	m_wndMaxArea = new CBCGPMaskEdit;
// 	m_wndMaxArea->Create( WS_CHILD | WS_VISIBLE, MaxAreaEditRect, pParent, uIDEdit);
// 	m_wndMaxArea->SetFont(AreaCtrlFont);
// 	m_wndMaxArea->SetValidChars(_T("1234567890."));//�����ַ��б�
// 	uIDEdit++;
// }
// CAreaSetCtrl::~CAreaSetCtrl()
// {
// 	if (m_wndAreaName->GetSafeHwnd() != NULL)
// 	{
// 		delete m_wndAreaName;
// 	}
// 	if (m_wndMaxArea->GetSafeHwnd() != NULL)
// 	{
// 		delete m_wndMaxArea;
// 		uIDEdit--;
// 	}
// 	if (m_wndMaxAreaUnit->GetSafeHwnd() != NULL)
// 	{
// 		delete m_wndMaxAreaUnit;
// 	}
// 	if (m_wndMinArea->GetSafeHwnd() != NULL)
// 	{
// 		delete m_wndMinArea;
// 		uIDEdit--;
// 	}
// // 	if (m_wndMinAreaUnit != NULL)
// // 	{
// // 		delete m_wndMinAreaUnit;
// // 	}
// }
// void CAreaSetCtrl::setUnitText(CString strUnit, CWnd *pParent)
// {
// 	CRect rc;
// 	CString showvalue;
// 	showvalue.Format(_T("(%s)"),strUnit);
// // 	m_wndMinAreaUnit->GetWindowRect(&rc);	
// // 	pParent->ScreenToClient(rc);
// // 	delete m_wndMinAreaUnit;
// // 	m_wndMinAreaUnit = new CStatic;
// // 	m_wndMinAreaUnit->Create(showvalue,WS_CHILD | WS_VISIBLE,rc,pParent);
// // 	m_wndMinAreaUnit->SetFont(m_font);
// 
// 	m_wndMaxAreaUnit->GetWindowRect(&rc);
// 	pParent->ScreenToClient(rc);
// 	delete m_wndMaxAreaUnit;
// 	m_wndMaxAreaUnit = new CStatic;
// 	m_wndMaxAreaUnit->Create(showvalue,WS_CHILD | WS_VISIBLE,rc,pParent);
// 	m_wndMaxAreaUnit->SetFont(m_font);
// }
void CLPAttrSearchDlg::OnBnClickedUnitMeter()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	m_strUnit = _T("(ƽ����)");
// 	for (int i = 0; i < m_AreaSetArray.GetCount(); i++)
// 	{
// 		m_AreaSetArray.GetAt(i)->setUnitText(_T("ƽ����"),this);
// 	}
	UpdateData(FALSE);
}

void CLPAttrSearchDlg::OnBnClickedUnitMu()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	m_strUnit = _T("(Ķ)");
// 	for (int i = 0; i < m_AreaSetArray.GetCount(); i++)
// 	{
// 		m_AreaSetArray.GetAt(i)->setUnitText(_T("Ķ"),this);
// 	}
	UpdateData(FALSE);
}

void CLPAttrSearchDlg::OnBnClickedUnitHectare()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	m_strUnit = _T("(����)");
// 	for (int i = 0; i < m_AreaSetArray.GetCount(); i++)
// 	{
// 		m_AreaSetArray.GetAt(i)->setUnitText(_T("����"),this);
// 	}
	UpdateData(FALSE);
}

// void CLPAttrSearchDlg::OnBnClickedOk()
// {
// 
// }

// void CAreaSetCtrl::GetSQLText(BSTR *SQLText)
// {
// 	if (SQLText == NULL) return;
// 	CString strSQLWhere = _T("");
// 	CString strSQLMax = _T("");
// 	CString strSQLMin = _T("");
// 	CString strUint = _T("");
// 	CString strMaxAreaSize = _T("");
// 	CString strMinAreaSize = _T("");
// 	m_wndMaxAreaUnit->GetWindowText(strUint);
// 	m_wndMaxArea->GetWindowText(strMaxAreaSize);
// 	m_wndMinArea->GetWindowText(strMinAreaSize);
// 	double dbMaxAreaSize = 0;
// 	double dbMinAreaSize = 0;
// 	swscanf_s(strMaxAreaSize,_T("%lf"),&dbMaxAreaSize);
// 	swscanf_s(strMinAreaSize,_T("%lf"),&dbMinAreaSize);
// 	if (strUint.Compare(_T("(ƽ����)")) == 0)
// 	{
// 
// 	}else if (strUint.Compare(_T("(Ķ)")) == 0)
// 	{
// 		dbMaxAreaSize *= 666.67;
// 		dbMinAreaSize *= 666.67;
// 	}else if (strUint.Compare(_T("(����)")) == 0)
// 	{
// 		dbMaxAreaSize *= 10000.0;
// 		dbMinAreaSize *= 10000.0;
// 	}
// 	if (dbMaxAreaSize != 0)
// 	{
// 		strSQLMax.Format(_T(" and %s<= %lf "),m_strMJFieldName,dbMaxAreaSize);
// 	}
// 	if (dbMinAreaSize != 0)
// 	{
// 		strSQLMin.Format(_T(" and %s>= %lf "),m_strMJFieldName,dbMinAreaSize);
// 	}
// 	strSQLWhere = strSQLMax + strSQLMin;
// 	*SQLText = strSQLWhere.AllocSysString();
// }
LRESULT CLPAttrSearchDlg::OnCbnSelchangeCbxDlmc( WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_strFeatureClassName.Compare(_T("GHYT")) == 0)
	{
		HTREEITEM pItem= m_wndDLMC.GetSelectItem();
		CString strName = m_wndDLMC.m_wndTreeList.GetItemText(pItem);
		if (strName.Find(_T("����ũ��")) >= 0 || strName.Find(_T("һ��ũ��")) >= 0)
		{
			m_mapAreaName.SetAt(_T("�����"),_T("JMJ"));
			ReFleshAreaNameCtrl();
		}else
		{
			m_mapAreaName.RemoveKey(_T("�����"));
			ReFleshAreaNameCtrl();
		}
	}
	return S_OK;
}

void CLPAttrSearchDlg::ReFleshAreaNameCtrl()
{
	if (m_pStaticAreaName != NULL)
	{
		delete m_pStaticAreaName;
		m_pStaticAreaName = NULL;
	}
	if (m_pComboboxAreaName != NULL)
	{
		delete m_pComboboxAreaName;
		m_pComboboxAreaName = NULL;
	}
	CFont *ft = this->GetFont();
	CRect rc;
	m_rectAreaName.GetWindowRect(&rc);
	ScreenToClient(&rc);	
	if (m_mapAreaName.GetCount() == 1)
	{
		rc.top += 5;
		POSITION pItem = m_mapAreaName.GetStartPosition();
		CString strName;
		CString strValue;
		m_mapAreaName.GetNextAssoc(pItem,strName,strValue);
		m_pStaticAreaName = new CStatic;
		m_pStaticAreaName->Create(strName,WS_CHILD | WS_VISIBLE | SS_CENTER, rc, this ,IDC_AREANAME);
		m_pStaticAreaName->SetFont(ft);

	}else
	{
		m_pComboboxAreaName = new CBCGPComboBox;
		m_pComboboxAreaName->Create(WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST , rc, this, IDC_AREANAME);
		m_pComboboxAreaName->SetFont(ft);
		POSITION pItem = m_mapAreaName.GetStartPosition();
		CString strName;
		CString strValue;
		while(pItem != NULL)
		{
			m_mapAreaName.GetNextAssoc(pItem,strName,strValue);
			m_pComboboxAreaName->AddString(strName);
		}
		m_pComboboxAreaName->SelectString(0,strName);
	}
}



void CLPAttrSearchDlg::OnBnClickedGhysSearch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	((CButton*)GetDlgItem(IDC_DLLX_TEXT))->SetFocus();
	UpdateData();
	//ƴ�Ӳ�ѯ���
	CString strSQLWhere = _T("");
	CString strSQLXZQ = _T("");
	CString strDLDM = _T("");
	//CString strAreaSize;
	CString strTabText = _T("");
	//��ȡѡ�е�������
	HTREEITEM xzqItem = m_wndXZQTree.GetSelectItem();
	if (xzqItem == NULL)
	{
		MessageBox(_T("��ѡ��������"),_T("��ʾ"),MB_OK);
		return;
	}
	_tagDistrictItem* pItem = (_tagDistrictItem*)m_wndXZQTree.m_wndTreeList.GetItemData(xzqItem);
	if (pItem != NULL)
	{
//  		if (pItem->strCode.GetLength() < 6)
//  		{
//  			MessageBox(_T("��ѡ������������ѡ���С����������Χ"),_T("��ʾ"),MB_OK);
//  			return;
//  		}
		//edit by lcc 2016.12.14 ���֧�����ĳ���
		if (pItem->strName.Compare(_T("���ĳ���")) == 0)
		{
			if (m_strFeatureClassName.Compare(_T("JSYDGZQ")) == 0 )
			{
				strSQLXZQ.Format(_T("XZQDM in ('%s,%s')"),pItem->strParentID,pItem->strCode);
				strSQLXZQ.Replace(_T(","),_T("','"));
				strTabText += pItem->strName;
				strSQLWhere += strSQLXZQ;
			}else
			{
				strSQLXZQ.Format(_T("(XZQDM like '%s%%')"),pItem->strParentID);
				strSQLXZQ.Replace(_T(","),_T("%' or XZQDM like '"));
				strTabText += pItem->strName;
				strSQLWhere += strSQLXZQ;
			}
		}else
		{
			strSQLXZQ.Format(_T("XZQDM like'%s%%'"),pItem->strCode);
			strTabText += pItem->strName;
			strSQLWhere += strSQLXZQ;
		}
		//edit by lcc 2016.12.14 end
	}else
	{
		strSQLXZQ = _T("XZQDM like '%%'");
		strSQLWhere += strSQLXZQ;
	}
	//��ȡѡ�е�ͼ�㣬��ӵ���ǩҳ����
	int nType = m_wndTargetType.GetCurSel();
	CString strSaerchLayer;
	m_wndTargetType.GetLBText(nType,strSaerchLayer);
	strTabText += _T("<");
	strTabText += strSaerchLayer;
	strTabText += _T(">");
	//��ȡѡ�еĵ���
	HTREEITEM dldmItem = m_wndDLMC.GetSelectItem();
	if (dldmItem != NULL)
	{
		//������ֽڵ㣬ѭ����������֯or���
		HTREEITEM dldmChildItem = m_wndDLMC.m_wndTreeList.GetChildItem(dldmItem);
		if (m_wndDLMC.m_wndTreeList.GetItemText(dldmItem).Compare(_T("ȫ��")) == 0)
		{
			strDLDM.Format(_T(" and %s like '%%'"),m_strDLDMFieldName);
		}else if (dldmChildItem != NULL)
		{
			CString strDLDMTmp = _T("");
			BSTR* bstrCode = (BSTR*)m_wndDLMC.m_wndTreeList.GetItemData(dldmItem);
			strDLDMTmp.Format(_T(" and ( %s = '%s'"),m_strDLDMFieldName,bstrCode);
			strDLDM += strDLDMTmp;
			while(dldmChildItem != NULL)
			{
				bstrCode = (BSTR*)m_wndDLMC.m_wndTreeList.GetItemData(dldmChildItem);
				strDLDMTmp.Format(_T(" or %s = '%s'"),m_strDLDMFieldName,bstrCode);
				strDLDM += strDLDMTmp;
				//edit by lcc 2016.12.07 ������������룬����������ѭ��
				//������ֽڵ㣬ѭ����������֯or���
				HTREEITEM dldmLastItem = m_wndDLMC.m_wndTreeList.GetChildItem(dldmChildItem);
				while(dldmLastItem != NULL)
				{
					bstrCode = (BSTR*)m_wndDLMC.m_wndTreeList.GetItemData(dldmLastItem);
					strDLDMTmp.Format(_T(" or %s = '%s'"),m_strDLDMFieldName,bstrCode);
					strDLDM += strDLDMTmp;
					dldmLastItem = m_wndDLMC.m_wndTreeList.GetNextSiblingItem(dldmLastItem);				
				}
				//edit by lcc 2016.12.07 end
				dldmChildItem = m_wndDLMC.m_wndTreeList.GetNextSiblingItem(dldmChildItem);				
			}
			strDLDM += _T(")");
		}else
		{
			BSTR* bstrCode = (BSTR*)m_wndDLMC.m_wndTreeList.GetItemData(dldmItem);
			strDLDM.Format(_T(" and %s = '%s'"),m_strDLDMFieldName,bstrCode);
		}
		strSQLWhere += strDLDM;
	}
// 	if (strDLDM.GetLength() != 0)
// 	{
// 		BSTR* bstrCode = (BSTR*)m_wndDLMC.m_wndTreeList.GetItemData(dldmItem);
// 		CString strTabTextTmp;
// 		strTabTextTmp.Format(_T("<%s>"),bstrCode);
// 		strTabText += strTabTextTmp;
// 	}else
// 	{
// 		strTabText += _T("<ȫ������>");
// 	}
	//��ȡд������
	// 	for (int i = 0; i < m_AreaSetArray.GetCount(); i++)
	// 	{
	// 		BSTR SQLAreaSize;
	// 		m_AreaSetArray.GetAt(i)->GetSQLText(&SQLAreaSize);
	// 		strSQLWhere += SQLAreaSize;
	// 	}
	//edit by lcc 2016.12.06 �޸�����ֶ�sql�����֯��ʽ
	CString strAreaName;
	CString strAreaFieldName;
	if (m_pStaticAreaName != NULL)
	{
		m_pStaticAreaName->GetWindowText(strAreaName);
		m_mapAreaName.Lookup(strAreaName,strAreaFieldName);
	}else
	{
		int nIndex = m_pComboboxAreaName->GetCurSel();
		m_pComboboxAreaName->GetLBText(nIndex,strAreaName);
		m_mapAreaName.Lookup(strAreaName,strAreaFieldName);
	}

	//edit by lcc 2016.12.27 �޸Ļ�ȡ��ֵ�ķ�ʽ
	double dbAreaSizeMin = 0.0;
	double dbAreaSizeMax = 0.0;
	if (m_strAreaSizeMax.GetLength() > 0 /*&& m_strAreaSizeMax.Compare(_T("0")) != 0*/)
	{
		dbAreaSizeMax = _wtof(_bstr_t(m_strAreaSizeMax));
	}
	if (m_strAreaSizeMin.GetLength() > 0/* && m_strAreaSizeMin.Compare(_T("0")) != 0*/)
	{
		dbAreaSizeMin = _wtof(_bstr_t(m_strAreaSizeMin));
	}

	if (m_strFeatureClassName.Compare(_T("JSYDGZQ")) == 0 || m_strFeatureClassName.Compare(_T("TDYTQ")) == 0)
	{
		if (m_strUnit.Find(_T("ƽ����")) >= 0)
		{
			dbAreaSizeMax /= 10000.0;
			dbAreaSizeMin /= 10000.0;

		}else if (m_strUnit.Find(_T("Ķ")) >= 0)
		{
			dbAreaSizeMax /= 15.0;
			dbAreaSizeMin /= 15.0;
		}else if (m_strUnit.Find(_T("����")) >= 0)
		{
		}
	}else
	{
		if (m_strUnit.Find(_T("ƽ����")) >= 0)
		{

		}else if (m_strUnit.Find(_T("Ķ")) >= 0)
		{
			dbAreaSizeMax = (dbAreaSizeMax*10000.0)/15.0;
			dbAreaSizeMin = (dbAreaSizeMin*10000.0)/15.0;
		}else if (m_strUnit.Find(_T("����")) >= 0)
		{
			dbAreaSizeMax *= 10000.0;
			dbAreaSizeMin *= 10000.0;
		}
	}

  	if (m_strAreaSizeMax.IsEmpty() != TRUE)
  	{
  		CString strAreaSizeMax;
  		strAreaSizeMax.Format(_T(" and %s <= %lf "),strAreaFieldName,dbAreaSizeMax);
  		strSQLWhere += strAreaSizeMax;
  	}
  	if (m_strAreaSizeMin.IsEmpty() != TRUE)
  	{
  		CString strAreaSizeMin;
  		strAreaSizeMin.Format(_T(" and %s >= %lf "),strAreaFieldName,dbAreaSizeMin);
  		strSQLWhere += strAreaSizeMin;
  	}
// 	edit by lcc 2016.12.06 end
// 	strAreaSize.Format(_T(" and %s <= %lf and %s >= %lf "),strAreaFieldName,m_dbAreaSizeMax,strAreaFieldName,m_dbAreaSizeMin);
// 	strSQLWhere += strAreaSize;

	CComQIPtr<ITable> spTable;
	m_ipFtWorkspace->OpenTable(m_strFeatureClassName.AllocSysString(),&spTable);
	if (spTable == NULL) return;
	CComQIPtr<IQueryFilter> spQueryFilter;
	spQueryFilter.CoCreateInstance(CLSID_QueryFilter);
	spQueryFilter->put_SubFields(_T("*"));
	spQueryFilter->put_WhereClause(strSQLWhere.AllocSysString());


	CComQIPtr<GisqLib::IGSDockableWindow> spResDockwin;
	CComQIPtr<GisqLib::IGSDockableWindowManager> spGSDockableWindowManager = m_pFrameworkCom->GetDockableWindowManager();
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
		//spGridDockWin->ShowWindow(VARIANT_TRUE);
		CComQIPtr<ILPGridDockWinCmd> spResDlg = spGridDockWin->GetDockableWindowTabDef();
		spResDlg->SetQueryFilter(spQueryFilter,m_strFeatureClassName.AllocSysString(),strTabText.AllocSysString());
	}else
	{
		spResDockwin->ShowWindow(VARIANT_TRUE);
		CComQIPtr<ILPGridDockWinCmd> spResDlg = spResDockwin->GetDockableWindowTabDef();
		spResDlg->SetQueryFilter(spQueryFilter,m_strFeatureClassName.AllocSysString(),strTabText.AllocSysString());
	}	
}



void CLPAttrSearchDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	CGSBCGDialog::PostNcDestroy();
	delete this;
}

void CLPAttrSearchDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CGSBCGDialog::OnCancel();
	CGSBCGDialog::DestroyWindow();

}

void CLPAttrSearchDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CGSBCGDialog::OnClose();
	CGSBCGDialog::DestroyWindow();
}