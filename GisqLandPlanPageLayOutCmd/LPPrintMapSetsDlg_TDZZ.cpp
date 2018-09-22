// LPPrintMapSetsDlg_TDZZ.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LPPrintMapSetsDlg_TDZZ.h"


// CLPPrintMapSetsDlg_TDZZ �Ի���

IMPLEMENT_DYNAMIC(CLPPrintMapSetsDlg_TDZZ, CGSBCGDialog)

CLPPrintMapSetsDlg_TDZZ::CLPPrintMapSetsDlg_TDZZ(CWnd* pParent /*=NULL*/)
	: CGSBCGDialog(CLPPrintMapSetsDlg_TDZZ::IDD, pParent)
	, m_bIsLockScale(FALSE)
	, m_bIsZXCQ(FALSE)
{
	m_spZDQYLayer = (IUnknown *)NULL;
}

CLPPrintMapSetsDlg_TDZZ::~CLPPrintMapSetsDlg_TDZZ()
{
}

void CLPPrintMapSetsDlg_TDZZ::DoDataExchange(CDataExchange* pDX)
{
	CGSBCGDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBX_XZQTREE_TDZZ, m_wndXZQComboTree);
	DDX_Check(pDX, IDC_IS_SAVE_SETS_TDZZ, m_bIsSaveConfige);
	DDX_Text(pDX, IDC_EDIT_MAPNAME_TDZZ, m_strMapName);
	DDX_Text(pDX, IDC_EDIT_MAKEWHERE_ZF_TDZZ, m_strMakeWhere_ZF);
	DDX_Text(pDX, IDC_EDIT_MAKEWHERE_GTJ_TDZZ, m_strMakeWhere_GTJ);
	//DDX_Control(pDX, IDC_EDIT_MAKEDATE, m_wndMakeDate);
	DDX_Control(pDX, IDC_COMBO1_TDZZ, m_wndMonthCombo);
	DDX_Text(pDX, IDC_EDIT1_TDZZ, m_nYear);
	DDV_MinMaxInt(pDX, m_nYear, 2000, 9999);
	DDX_Control(pDX, IDC_CB_TDZZZDQY_LAYER, m_wndZDQYLayerCombo);
	DDX_Control(pDX, IDC_EDIT1_TDZZ, m_editYear);
	DDX_Check(pDX, IDC_CHECK1, m_bIsLockScale);
}


BEGIN_MESSAGE_MAP(CLPPrintMapSetsDlg_TDZZ, CGSBCGDialog)
	ON_BN_CLICKED(IDC_IS_SAVE_SETS_TDZZ, &CLPPrintMapSetsDlg_TDZZ::OnBnClickedIsSaveSets)
END_MESSAGE_MAP()


// CLPPrintMapSetsDlg_TDZZ ��Ϣ�������
BOOL CLPPrintMapSetsDlg_TDZZ::OnInitDialog()
{
	CGSBCGDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	if (m_wndXZQComboTree.m_wndTreeList.GetSafeHwnd() == NULL)
	{
		m_wndXZQComboTree.InitTree(-1,200);
		CImageList *TreeImage = NULL;
		InitImageList(&TreeImage);
		m_wndXZQComboTree.m_wndTreeList.SetImageList(TreeImage,5);
	}
	//m_wndZDQYLayerCombo.SetExtendedStyle();
	InitXZQComboTree(4);
	InitTimeCtrl();
	if (InitTDZZLayerCbx() == FALSE)
	{
		OnCancel();
		return FALSE;
	}


	//InitTimeCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

// void CLPPrintMapSetsDlg::SetGHWorkspace(CComQIPtr<IWorkspace> spGHWorkspace)
// {
// 	if (spGHWorkspace == NULL) return;
// 	m_spGHFtWorkspace = spGHWorkspace;
// }
void CLPPrintMapSetsDlg_TDZZ::SetConfigFile(CString strConfigFilePath)
{
	m_strConfigFilePath = strConfigFilePath;
}

BOOL CLPPrintMapSetsDlg_TDZZ::InitXZQComboTree(int nStopLevel)
{
	m_wndXZQComboTree.DeleteAllItems();
	CComQIPtr<ILPXZQDataTree> ipTree;
	CComQIPtr<ILPXZQDataItem> ipRoot;
	IUnknown *ipTmpItem;
	ipTree.CoCreateInstance(CLSID_LPXZQDataTree);
	ipTree->GetRootItem(NULL,&ipTmpItem);
	ipRoot = (CComQIPtr<ILPXZQDataItem>) ipTmpItem;
	if (ipRoot == NULL) return TRUE;
	BSTR bstrTmp;
	ipRoot->Get_ItemCode(&bstrTmp);
	m_strParent = bstrTmp;
	AddDistrictTreeItem(NULL,ipRoot,nStopLevel);
	HTREEITEM item = m_wndXZQComboTree.m_wndTreeList.GetFirstVisibleItem();
	if (m_pXZQItem != NULL)
	{
		m_wndXZQComboTree.SetSelectString(m_wndXZQComboTree.m_wndTreeList.GetItemText(m_pXZQItem));
	}else
	{
		HTREEITEM pItem = m_wndXZQComboTree.m_wndTreeList.GetRootItem();
		CString strItemText = m_wndXZQComboTree.m_wndTreeList.GetItemText(pItem);
		m_wndXZQComboTree.SetSelectString(strItemText);
		//m_wndXZQComboTree.SetSelectString(_T("[33]�㽭ʡ"));
	}
	m_wndXZQComboTree.m_wndTreeList.Expand(item,TVE_EXPAND);
	UpdateData(FALSE);
	return TRUE;
}

void CLPPrintMapSetsDlg_TDZZ::AddDistrictTreeItem(HTREEITEM parentItem/*=NULL*/,CComQIPtr<ILPXZQDataItem> spDataItem, int nStopLevel)
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
		hItem = m_wndXZQComboTree.AddItem(showValue,0,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 1)//��
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(2),ItemData->strName);
		hItem = m_wndXZQComboTree.AddItem(showValue,1,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 2)//����
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(2),ItemData->strName);
		hItem = m_wndXZQComboTree.AddItem(showValue,2,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 4)//����ֵ�
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(3),ItemData->strName);
		hItem = m_wndXZQComboTree.AddItem(showValue,3,(DWORD)ItemData,parentItem);
	}//else if (ItemData->nLevel  == 6)//��
	// 	{
	// 		CString showValue;
	// 		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(3),ItemData->strName);
	// 		hItem = m_wndXZQComboTree.AddItem(showValue,-1,(DWORD)ItemData,parentItem);
	// 	}
	if (m_strXZQDM.Compare(ItemData->strCode) == 0)
	{
		m_pXZQItem = hItem;
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

void CLPPrintMapSetsDlg_TDZZ::InitImageList(CImageList** ppImageList)
{
	CImageList *TreeImage = new CImageList;
	TreeImage->Create(32, 32, ILC_COLOR16 | ILC_MASK,0,4);
	CBitmap *BitmapTmp = new CBitmap;
	BitmapTmp->LoadBitmap(IDB_DISTRICT_ICON_SHENG);
	TreeImage->Add(BitmapTmp,RGB(255,255,255));

	BitmapTmp = NULL;
	BitmapTmp = new CBitmap;
	BitmapTmp->LoadBitmap(IDB_DISTRICT_ICON_SHI);
	TreeImage->Add(BitmapTmp,RGB(255,255,255));

	BitmapTmp = NULL;
	BitmapTmp = new CBitmap;
	BitmapTmp->LoadBitmap(IDB_DISTRICT_ICON_XIANQU);
	TreeImage->Add(BitmapTmp,RGB(255,255,255));

	BitmapTmp = NULL;
	BitmapTmp = new CBitmap;
	BitmapTmp->LoadBitmap(IDB_DISTRICT_ICON_XIANGZHEN);
	TreeImage->Add(BitmapTmp,RGB(255,255,255));
	*ppImageList = TreeImage;
}

void CLPPrintMapSetsDlg_TDZZ::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData();
	RefleshCofigMap();//���ǰ��ˢ��
	if (m_bIsSaveConfige)
	{
		SaveConfigFile();
	}
	HTREEITEM pItem = m_wndXZQComboTree.GetSelectItem();
	if (pItem == NULL)
	{
		MessageBox(_T("������������"),_T("��ʾ"),MB_OK);
		return;
	}
	_tagDistrictItem* pData = (_tagDistrictItem*)m_wndXZQComboTree.m_wndTreeList.GetItemData(pItem);
	m_strXZQDM = pData->strCode;
	CString strName = pData->strName;
	if (m_strXZQDM.GetLength() != 9)
	{
		MessageBox(_T("��ǰѡ�������������Ǳ�׼��ͼ��Χ"),strName,MB_OK);
		return;
	}
	CString strParentXZQDM = pData->strParentID;

	CComQIPtr<ILPXZQDataTree> spXZQDataTree;
	spXZQDataTree.CoCreateInstance(CLSID_LPXZQDataTree);
	IUnknown* pUnTmp;
	spXZQDataTree->GetXZQMap(&pUnTmp);
	CMapStringToString *pmapXZQTree = (CMapStringToString *)pUnTmp;
	if (pmapXZQTree != NULL)
	{
		CString strParentName;
		if (pData->strName.Compare(_T("���ĳ���")) == 0)
		{//˵����ǰ�����ĳ���
			m_bIsZXCQ = TRUE;
			m_strXZQDM = pData->strParentID;
			m_strXZQDM.Replace(_T(","),_T("','"));
			pmapXZQTree->Lookup(pData->strCode.Left(6),strParentName);
		}else
		{
			m_bIsZXCQ = FALSE;
			pmapXZQTree->Lookup(strParentXZQDM,strParentName);
		}
		CString strTilte;
		CString strMapName;
		m_strMapConfig.Lookup(_T("MAPNAME"),strMapName);
		strTilte.Format(_T("%s%s%s%s"),strParentName,strName,_T("������������滮��2006-2020��\n"),strMapName);
		m_strMapConfig.SetAt(_T("MAPTITLE"),strTilte);

		CString strMapWZ;
		strMapWZ.Format(_T("%s��%sλ��ʾ��ͼ"),strName,strParentName);
		m_strMapConfig.SetAt(_T("MAPWZ"),strMapWZ);
	}
	if (m_strMakeWhere_ZF.GetLength() != 0)
	{
		if (m_strMakeWhere_ZF.Find(_T("��������")) < 0)
		{
			m_strMakeWhere_ZF += _T("��������");
			m_strMapConfig.SetAt(_T("MAPZF"),m_strMakeWhere_ZF);
		}
	}
	if (m_strMakeWhere_GTJ.GetLength() != 0)
	{
		m_strMakeWhere_GTJ += _T("\t��ͼ");
		m_strMapConfig.SetAt(_T("MAPPUBLIC"),m_strMakeWhere_GTJ);
	}
	CString strTime;
	GetTimeString(strTime);
	if (strTime.GetLength() != 0)
	{
		m_strMapConfig.SetAt(_T("MAPRQ"),strTime);
	}
	if (m_wndZDQYLayerCombo.GetCount() > 0)
	{
		int nIndex = 0;
		nIndex = m_wndZDQYLayerCombo.GetCurSel();
		if (m_spZDQYLayer != NULL)
		{
			m_spZDQYLayer.Release();
		}

//		m_spZDQYLayer = (ILayer*)m_wndZDQYLayerCombo.GetItemData(nIndex);
// 		IUnknown* ptmpUnknown = (IUnknown*)m_wndZDQYLayerCombo.GetItemData(nIndex);
// 		m_spZDQYLayer = (CComQIPtr<ILayer>)ptmpUnknown;
// 		IUnknown* ptmpUnknown = (IUnknown*)m_wndZDQYLayerCombo.GetItemData(nIndex);
// 		ILayer*  ipLayer;
// 		ptmpUnknown->QueryInterface(IID_ILayer,(void**)&ipLayer);
// 		CString strName;
// 		m_wndZDQYLayerCombo.GetLBText(nIndex,strName);
// 
		CComQIPtr<IUID> spUID;
		spUID.CoCreateInstance(CLSID_UID);
		CComQIPtr<IEnumLayer> spEnumLayer;
		spUID->put_Value(_variant_t("{40A9E885-5533-11d0-98BE-00805F7CED21}"));
		m_spMap->get_Layers(spUID,VARIANT_TRUE,&spEnumLayer);
		if (spEnumLayer == NULL) return ;
		CComQIPtr<ILayer> spLayer;

		spEnumLayer->Next(&spLayer);
		while(spLayer != NULL)
		{
			CComBSTR bstrLayerName;
			spLayer->get_Name(&bstrLayerName);
			if (strName.Compare(bstrLayerName) == 0)
			{
				m_spZDQYLayer = spLayer;
				break;
			}
			spLayer.Release();
			spEnumLayer->Next(&spLayer);
		}
		if (m_spZDQYLayer == NULL)
		{
			return ;
		}
 	}
	CString strMapZFRQ;
	strMapZFRQ = m_strMakeWhere_ZF + _T("    ") + strTime;
	m_strMapConfig.SetAt(_T("MAPZFRQ"),strMapZFRQ);

	CGSBCGDialog::OnOK();
}



BOOL CLPPrintMapSetsDlg_TDZZ::InitViewConfig(CString strConfigFile, CString strRootName)
{
	//��������ļ��Ƿ����
	WIN32_FIND_DATA hFindData;
	HANDLE hFind;
	hFind = ::FindFirstFile(strConfigFile.GetBuffer(),&hFindData);
	//return hFind != INVALID_HANDLE_VALUE;
	if (hFind == INVALID_HANDLE_VALUE)
	{
		CString strTemp;
		strTemp.Format(_T("·����%s\nָ���ļ���Ч��"),strConfigFile);
		BCGPMessageBox(strTemp,_T("��ʾ"),MB_OK);
		return FALSE;
	}

	//WideCharToMultiByte
	int len =WideCharToMultiByte(CP_ACP,0,strConfigFile,-1,NULL,0,NULL,NULL);  
	//char* pszConfigFile =new char[len +1];  //���ﲻ�ü�1��WideCharToMultiByte�ڲ��Ѿ�Ϊ�����Զ���1��
	char* pszConfigFile =new char[len];  
	WideCharToMultiByte(CP_ACP,0,strConfigFile,-1,pszConfigFile,len,NULL,NULL );

	len = WideCharToMultiByte(CP_ACP,0,strRootName,-1,NULL,0,NULL,NULL);
	char* pszRootName = new char[len];
	WideCharToMultiByte(CP_ACP,0,strRootName,-1,pszRootName,len,NULL,NULL);

	TiXmlDocument xmlDoc(pszConfigFile);
	if(xmlDoc.LoadFile())
	{
		TiXmlElement *pRoot = xmlDoc.FirstChildElement();
		TiXmlElement *pNormal = pRoot->FirstChildElement("NORMAL")->FirstChildElement();
		while (pNormal != NULL)
		{
			const char* pszName = pNormal->Value();
			const char* pszValue = pNormal->FirstAttribute()->Value();
			CString strName(pszName);
			CString strValue(pszValue);
			m_strMapConfig.SetAt(strName.GetBuffer(),strValue.GetBuffer());
			if (pNormal->NextSibling() == NULL)break;
			pNormal = pNormal->NextSibling()->ToElement();
		}
		TiXmlElement* pElement = (pRoot->FirstChildElement(pszRootName))->FirstChildElement();
		while (pElement != NULL)
		{
			const char* pszName = pElement->Value();
			const char* pszValue = pElement->FirstAttribute()->Value();
			CString strName(pszName);
			CString strValue(pszValue);
			m_strMapConfig.SetAt(strName.GetBuffer(),strValue.GetBuffer());
			if (pElement->NextSibling() == NULL)break;
			pElement = pElement->NextSibling()->ToElement();
		}
	}
	else
	{
		delete[] pszRootName;
		delete[] pszConfigFile;
		return FALSE;
	}
	delete[] pszRootName;
	delete[] pszConfigFile;

	m_strConfigFilePath = strConfigFile;
	m_strRootName = strRootName;
	InitConfigEdits();
	return TRUE;
}
BOOL CLPPrintMapSetsDlg_TDZZ::InitViewConfig2(CString strConfigFile, CString strRootName)
{
	//��������ļ��Ƿ����
	WIN32_FIND_DATA hFindData;
	HANDLE hFind;
	hFind = ::FindFirstFile(strConfigFile.GetBuffer(),&hFindData);
	//return hFind != INVALID_HANDLE_VALUE;
	if (hFind == INVALID_HANDLE_VALUE)
	{
		CString strTemp;
		strTemp.Format(_T("·����%s\nָ���ļ���Ч��"),strConfigFile);
		BCGPMessageBox(strTemp,_T("��ʾ"),MB_OK);
		return FALSE;
	}
	m_strConfigFilePath = strConfigFile;
	//WideCharToMultiByte
	int len =WideCharToMultiByte(CP_ACP,0,strConfigFile,-1,NULL,0,NULL,NULL);  
	//char* pszConfigFile =new char[len +1];  //���ﲻ�ü�1��WideCharToMultiByte�ڲ��Ѿ�Ϊ�����Զ���1��
	char* pszConfigFile =new char[len];  
	WideCharToMultiByte(CP_ACP,0,strConfigFile,-1,pszConfigFile,len,NULL,NULL );

	len = WideCharToMultiByte(CP_ACP,0,strRootName,-1,NULL,0,NULL,NULL);
	char* pszRootName = new char[len];
	WideCharToMultiByte(CP_ACP,0,strRootName,-1,pszRootName,len,NULL,NULL);

	TiXmlDocument xmlDoc(pszConfigFile);
	if(xmlDoc.LoadFile())
	{
		TiXmlElement *pRoot = xmlDoc.FirstChildElement("Config")->FirstChildElement("PageLayoutConfig");
		TiXmlElement *pNormal = pRoot->FirstChildElement("NORMAL")->FirstChildElement();
		while (pNormal != NULL)
		{
			CString strName;
			CString strValue;
			utf8ToUnicode(pNormal->Value(),&strName);
			utf8ToUnicode(pNormal->FirstAttribute()->Value(),&strValue);
			m_strMapConfig.SetAt(strName.GetBuffer(),strValue.GetBuffer());
			if (pNormal->NextSibling() == NULL)break;
			pNormal = pNormal->NextSibling()->ToElement();
		}
		TiXmlElement* pElement = pRoot->FirstChildElement(pszRootName)->FirstChildElement();
		while (pElement != NULL)
		{
			CString strName;
			CString strValue;
			utf8ToUnicode(pElement->Value(),&strName);
			utf8ToUnicode(pElement->FirstAttribute()->Value(),&strValue);
			m_strMapConfig.SetAt(strName.GetBuffer(),strValue.GetBuffer());
			if (pElement->NextSibling() == NULL)break;
			pElement = pElement->NextSibling()->ToElement();
		}
	}
	else
	{
		delete[] pszRootName;
		delete[] pszConfigFile;
		return FALSE;
	}
	delete[] pszRootName;
	delete[] pszConfigFile;

	m_strConfigFilePath = strConfigFile;
	m_strRootName = strRootName;
	InitConfigEdits();
	return TRUE;
}
void CLPPrintMapSetsDlg_TDZZ::InitConfigEdits()
{
	m_strMapConfig.Lookup(_T("MAPNAME"),m_strMapName);

	m_strMapConfig.Lookup(_T("MAPZF"),m_strMakeWhere_ZF);
	m_strMapConfig.Lookup(_T("MAPPUBLIC"),m_strMakeWhere_GTJ);
	m_strMapConfig.Lookup(_T("MAPXZQ"),m_strXZQDM);
	//���ڵ���������Ϊ��Ҫ��ʽ����ȡ�Ͷ�bcg�ؼ�������

	//edit by lcc 2016.11.24���������domodal֮ǰ���ò���ִ��ˢ�²���
	//UpdateData(FALSE);
}

void CLPPrintMapSetsDlg_TDZZ::SaveConfigFile()
{
	USES_CONVERSION;
	WIN32_FIND_DATA hFindData;
	HANDLE hFind;
	hFind = ::FindFirstFile(m_strConfigFilePath.GetBuffer(),&hFindData);
	//return hFind != INVALID_HANDLE_VALUE;
	if (hFind == INVALID_HANDLE_VALUE)
	{
		CString strTemp;
		strTemp.Format(_T("·����%s\nָ���ļ���Ч��"),m_strConfigFilePath);
		BCGPMessageBox(strTemp,_T("��ʾ"),MB_OK);
		return ;
	}

	//WideCharToMultiByte
	int len =WideCharToMultiByte(CP_ACP,0,m_strConfigFilePath,-1,NULL,0,NULL,NULL);  
	//char* pszConfigFile =new char[len +1];  //���ﲻ�ü�1��WideCharToMultiByte�ڲ��Ѿ�Ϊ�����Զ���1��
	char* pszConfigFile =new char[len];  
	WideCharToMultiByte(CP_ACP,0,m_strConfigFilePath,-1,pszConfigFile,len,NULL,NULL );

	len = WideCharToMultiByte(CP_ACP,0,m_strRootName,-1,NULL,0,NULL,NULL);
	char* pszRootName = new char[len];
	WideCharToMultiByte(CP_ACP,0,m_strRootName,-1,pszRootName,len,NULL,NULL);

	TiXmlDocument xmlDoc(pszConfigFile);
	if(xmlDoc.LoadFile())
	{
		TiXmlElement *pRoot = xmlDoc.FirstChildElement("Config")->FirstChildElement("PageLayoutConfig");
		TiXmlElement *pNormal = pRoot->FirstChildElement("NORMAL");
		TiXmlElement* pElement = pRoot->FirstChildElement(pszRootName);
		POSITION pItem = m_strMapConfig.GetStartPosition();
		while(pItem != NULL)
		{
			CString strKey;
			CString strValue;
			m_strMapConfig.GetNextAssoc(pItem,strKey,strValue);
			TiXmlElement *pSet = pElement->FirstChildElement((UnicodeToUTF8(strKey).c_str()));
			TiXmlElement *pNormalSet = pNormal->FirstChildElement(UnicodeToUTF8(strKey).c_str());
			if (pNormalSet != NULL)
			{
				pNormalSet->SetAttribute("value",(UnicodeToUTF8(strValue).c_str()));
			}else if (pSet != NULL)
			{
				pSet->SetAttribute("value",(UnicodeToUTF8(strValue).c_str()));
			}else
			{
				continue;
			}
		}
		xmlDoc.SaveFile();
	}
}

void CLPPrintMapSetsDlg_TDZZ::RefleshCofigMap()
{
	UpdateData();
	//��ȡ���õ�ʱ��
	// 	COleDateTime oledatatime((DATE)m_wndMakeDate.GetDate());
	// 	int nYear = 0;
	// 	int nMonth = 0;
	// 	int nDay = 0;
	// 	nYear = oledatatime.GetYear();
	// 	nMonth = oledatatime.GetMonth();
	// 	nDay = oledatatime.GetDay();
	// 	CString strdate;
	// 	strdate.Format(_T("%d��%d��%d��"),nYear,nMonth,nDay);
	//����map�е�����
	m_strMapConfig.SetAt(_T("MAPNAME"),m_strMapName);
	m_strMapConfig.SetAt(_T("MAPZF"),m_strMakeWhere_ZF);
	m_strMapConfig.SetAt(_T("MAPPUBLIC"),m_strMakeWhere_GTJ);
	HTREEITEM pItem = m_wndXZQComboTree.GetSelectItem();
	_tagDistrictItem* pData = (_tagDistrictItem*)m_wndXZQComboTree.m_wndTreeList.GetItemData(pItem);
	m_strXZQDM = pData->strCode;
	m_strMapConfig.SetAt(_T("MAPXZQ"),m_strXZQDM);
	//	m_strMapConfig.SetAt(_T("MAPRQ"),strdate);
}

void CLPPrintMapSetsDlg_TDZZ::OnBnClickedIsSaveSets()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// 	UpdateData();
	// 	if (m_bIsSaveConfige)
	// 	{
	// 		SaveConfigFile();
	// 	}
}


void CLPPrintMapSetsDlg_TDZZ::GetTimeString(CString &strTime)
{

	CString strYear;
	strYear.Format(_T("%d"),m_nYear);

	strYear.Replace(_T("1"),_T("һ"));
	strYear.Replace(_T("2"),_T("��"));
	strYear.Replace(_T("3"),_T("��"));
	strYear.Replace(_T("4"),_T("��"));
	strYear.Replace(_T("5"),_T("��"));
	strYear.Replace(_T("6"),_T("��"));
	strYear.Replace(_T("7"),_T("��"));
	strYear.Replace(_T("8"),_T("��"));
	strYear.Replace(_T("9"),_T("��"));
	strYear.Replace(_T("0"),_T("��"));

	CComBSTR bstrMon[12] = {_T("һ"),_T("��"),_T("��"),_T("��"),_T("��"),_T("��"),
		_T("��"),_T("��"),_T("��"),_T("ʮ"),_T("ʮһ"),_T("ʮ��")};
	int nMonth = m_wndMonthCombo.GetCurSel();
	CString strMonth(bstrMon[nMonth]);
	strTime.Format(_T("%s��%s�� ����"),strYear,strMonth);
}

void CLPPrintMapSetsDlg_TDZZ::InitTimeCtrl()
{
	m_editYear.EnableMask(
		_T("AAAA"),	// The mask string
		_T("____"),	// Literal, "_" char = character entry
		_T('_'));		// Backspace replace char
	m_editYear.SetValidChars(_T("1234567890")); // Valid string characters 
	CTime t;
	t = CTime::GetCurrentTime();
	m_nYear = t.GetYear();
	CString strMonth;
	strMonth.Format(_T("%d"),t.GetMonth());

	m_wndMonthCombo.AddString(_T("1"));
	m_wndMonthCombo.AddString(_T("2"));
	m_wndMonthCombo.AddString(_T("3"));
	m_wndMonthCombo.AddString(_T("4"));
	m_wndMonthCombo.AddString(_T("5"));
	m_wndMonthCombo.AddString(_T("6"));
	m_wndMonthCombo.AddString(_T("7"));
	m_wndMonthCombo.AddString(_T("8"));
	m_wndMonthCombo.AddString(_T("9"));
	m_wndMonthCombo.AddString(_T("10"));
	m_wndMonthCombo.AddString(_T("11"));
	m_wndMonthCombo.AddString(_T("12"));
	m_wndMonthCombo.SelectString(0,strMonth);

	UpdateData(FALSE);
}

BOOL CLPPrintMapSetsDlg_TDZZ::InitTDZZLayerCbx()
{
	if (m_spMap == NULL) return FALSE;

	CComPtr<IPropertySet> spPropertySet;
	m_spGHWorkspace->get_ConnectionProperties(&spPropertySet);
	VARIANT vtINSTANCE = vtMissing;
	spPropertySet->GetProperty(_bstr_t(_T("INSTANCE")),&vtINSTANCE);
	CString strGHINSTANCE(vtINSTANCE.bstrVal);

	CComQIPtr<IUID> spUID;
	spUID.CoCreateInstance(CLSID_UID);
	CComQIPtr<IEnumLayer> spEnumLayer;
	spUID->put_Value(_variant_t("{40A9E885-5533-11d0-98BE-00805F7CED21}"));
	m_spMap->get_Layers(spUID,VARIANT_TRUE,&spEnumLayer);
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
		long nDMIndex = -1;
		spFtClass->FindField(_T("TDZZLXDM"),&nDMIndex);
		if (nDMIndex < 0)//���û��TDZZLXDM�ֶΣ������ò�
		{
			spLayer.Release();
			spEnumLayer->Next(&spLayer);
			continue;
		}
		CComQIPtr<IDataset> spDataset = spFtClass;
		if (spDataset != NULL)
		{
			CComQIPtr<IWorkspace> spTempWorkspace;
			spDataset->get_Workspace(&spTempWorkspace);
			//���˵��滮���еĲ�
			esriWorkspaceType enumWorkspaceType;
			spTempWorkspace->get_Type(&enumWorkspaceType);
			if (enumWorkspaceType != esriRemoteDatabaseWorkspace)
			{
				esriGeometryType enumGeometryType;
				spFtClass->get_ShapeType(&enumGeometryType);
				//���˵������
				if (enumGeometryType == esriGeometryPolygon)
				{
					CComBSTR bstrLayerName;
					spLayer->get_Name(&bstrLayerName);
					((IUnknown*)spLayer)->AddRef();
					m_wndZDQYLayerCombo.SetItemData(m_wndZDQYLayerCombo.AddString(bstrLayerName),(DWORD_PTR)(IUnknown*)spLayer);
					
				}
			}else
			{
				CComQIPtr<IPropertySet> spTmpPropertySet;
				spTempWorkspace->get_ConnectionProperties(&spTmpPropertySet);
				VARIANT vtTmpINSTANCE = vtMissing;
				spTmpPropertySet->GetProperty(_bstr_t(_T("INSTANCE")),&vtTmpINSTANCE);
				CString strTmpINSTANCE(vtTmpINSTANCE.bstrVal);
				if (strGHINSTANCE.Compare(strTmpINSTANCE) != 0)
				{
					esriGeometryType enumGeometryType;
					spFtClass->get_ShapeType(&enumGeometryType);
					//���˵������
					if (enumGeometryType == esriGeometryPolygon)
					{
						BSTR bstrLayerName;
						spLayer->get_Name(&bstrLayerName);
						((IUnknown*)spLayer)->AddRef();
						m_wndZDQYLayerCombo.SetItemData(m_wndZDQYLayerCombo.AddString(bstrLayerName),(DWORD_PTR)(IUnknown*)spLayer);
					}
				}
			}
		}
		spLayer.Release();
		spEnumLayer->Next(&spLayer);
	}

	if (m_wndZDQYLayerCombo.GetCount() > 0)
	{
		m_wndZDQYLayerCombo.SetCurSel(0);
	}else
	{
		MessageBox(_T("δ���롮���������ص�滮�����ݣ������������ȱʧ��TDZZLXDM���ֶ�"),_T("û�з�������������Դ"),MB_OK);
		return FALSE;	
	}
	return TRUE;
}

void CLPPrintMapSetsDlg_TDZZ::SetDataMap(CComQIPtr<IMap> spMap)
{
	if (spMap != NULL)
	{
		m_spMap = spMap;
	}
}

void CLPPrintMapSetsDlg_TDZZ::SetGHWorkspace(CComQIPtr<IWorkspace> spWorkspace)
{
	if (spWorkspace != NULL)
	{
		m_spGHWorkspace = spWorkspace;
	}
}

void CLPPrintMapSetsDlg_TDZZ::utf8ToUnicode(LPCSTR src, CString *Result)  
{  
	int wcsLen = MultiByteToWideChar( CP_UTF8, 0, src, -1, NULL, 0 );  
	LPWSTR result = new wchar_t[wcsLen + 1];
	::MultiByteToWideChar( CP_UTF8, 0, src, -1, result, wcsLen); 
	Result->Format(_T("%s"),result);
	delete result;
} 
string CLPPrintMapSetsDlg_TDZZ::UnicodeToUTF8( CString &strIn )
{
	char*     pElementText;
	int    iTextLen;
	wstring str = strIn;
	// wide char to multi char
	iTextLen = WideCharToMultiByte( CP_UTF8,0,str.c_str(),	-1,NULL,0,	NULL,NULL );
	pElementText = new char[iTextLen + 1];
	memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
	::WideCharToMultiByte( CP_UTF8,	0,str.c_str(),-1,pElementText,iTextLen,	NULL,NULL );
	string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}