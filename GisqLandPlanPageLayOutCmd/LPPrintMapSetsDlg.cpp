// LPPrintMapSetsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LPPrintMapSetsDlg.h"


// CLPPrintMapSetsDlg 对话框

IMPLEMENT_DYNAMIC(CLPPrintMapSetsDlg, CGSBCGDialog)

CLPPrintMapSetsDlg::CLPPrintMapSetsDlg(CWnd* pParent /*=NULL*/)
	: CGSBCGDialog(CLPPrintMapSetsDlg::IDD, pParent)
	, m_bIsSaveConfige(FALSE)
	, m_strMapName(_T(""))
	, m_strMakeWhere_ZF(_T(""))
	, m_strMakeWhere_GTJ(_T(""))
	, m_nYear(0)
	, m_bIsZXCQ(FALSE)
	, m_bIsLockScale(FALSE)
{
	m_strConfigFilePath = _T("");
	m_pXZQItem = NULL;
}

CLPPrintMapSetsDlg::~CLPPrintMapSetsDlg()
{
}

void CLPPrintMapSetsDlg::DoDataExchange(CDataExchange* pDX)
{
	CGSBCGDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBX_XZQTREE, m_wndXZQComboTree);
	DDX_Check(pDX, IDC_IS_SAVE_SETS, m_bIsSaveConfige);
	DDX_Text(pDX, IDC_EDIT_MAPNAME, m_strMapName);
	DDX_Text(pDX, IDC_EDIT_MAKEWHERE_ZF, m_strMakeWhere_ZF);
	DDX_Text(pDX, IDC_EDIT_MAKEWHERE_GTJ, m_strMakeWhere_GTJ);
	//DDX_Control(pDX, IDC_EDIT_MAKEDATE, m_wndMakeDate);
	DDX_Control(pDX, IDC_COMBO1, m_wndMonthCombo);
	DDX_Text(pDX, IDC_EDIT1, m_nYear);
	//DDV_MinMaxInt(pDX, m_nYear, 2000, 9999);
	DDX_Control(pDX, IDC_EDIT1, m_editYear);
	DDX_Check(pDX, IDC_CHECK1, m_bIsLockScale);
}


BEGIN_MESSAGE_MAP(CLPPrintMapSetsDlg, CGSBCGDialog)
	ON_BN_CLICKED(IDC_IS_SAVE_SETS, &CLPPrintMapSetsDlg::OnBnClickedIsSaveSets)
END_MESSAGE_MAP()


// CLPPrintMapSetsDlg 消息处理程序

BOOL CLPPrintMapSetsDlg::OnInitDialog()
{
	CGSBCGDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_wndXZQComboTree.m_wndTreeList.GetSafeHwnd() == NULL)
	{
		m_wndXZQComboTree.InitTree(-1,200);
		CImageList *TreeImage = NULL;
		InitImageList(&TreeImage);
		m_wndXZQComboTree.m_wndTreeList.SetImageList(TreeImage,5);
	}
	InitXZQComboTree(4);
	InitTimeCtrl();
	
	//InitTimeCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// void CLPPrintMapSetsDlg::SetGHWorkspace(CComQIPtr<IWorkspace> spGHWorkspace)
// {
// 	if (spGHWorkspace == NULL) return;
// 	m_spGHFtWorkspace = spGHWorkspace;
// }
void CLPPrintMapSetsDlg::SetConfigFile(CString strConfigFilePath)
{
	m_strConfigFilePath = strConfigFilePath;
}
// void CLPPrintMapSetsDlg::InitTimeCtrl()
// {
// 	m_wndMakeDate.m_monthFormat = 2;//设置显示风格
// 	//设置哪些配置
// 	const UINT stateMask = 
// 		CBCGPDateTimeCtrl::DTM_SPIN |
// 		CBCGPDateTimeCtrl::DTM_DROPCALENDAR | 
// 		CBCGPDateTimeCtrl::DTM_DATE |
// 		CBCGPDateTimeCtrl::DTM_TIME24H |
// 		CBCGPDateTimeCtrl::DTM_CHECKBOX |
// 		CBCGPDateTimeCtrl::DTM_TIME | 
// 		CBCGPDateTimeCtrl::DTM_TIME24HBYLOCALE |
// 		CBCGPDateTimeCtrl::DTM_SECONDS;
// 	//是否使用这些设置（不用则不写）
// 	UINT DataState = 
// 		/*CBCGPDateTimeCtrl::DTM_SPIN |*/
//  		CBCGPDateTimeCtrl::DTM_DROPCALENDAR; //| 
// // 		CBCGPDateTimeCtrl::DTM_DATE |
// // 		CBCGPDateTimeCtrl::DTM_TIME24H |
// // 		CBCGPDateTimeCtrl::DTM_TIME | 
// // 		CBCGPDateTimeCtrl::DTM_TIME24HBYLOCALE;
// 	m_wndMakeDate.SetState(DataState,stateMask);
// 	//下面是应用配置文件中的日期
// 	CString strMakeDate;
// 	int nYear = 0;
// 	int nMonth = 0;
// 	int nDay = 0;
// 	m_strMapConfig.Lookup(_T("MAPRQ"),strMakeDate);
// 	strMakeDate.Replace(_T("一"),_T("1"));
// 	strMakeDate.Replace(_T("二"),_T("2"));
// 	strMakeDate.Replace(_T("三"),_T("3"));
// 	strMakeDate.Replace(_T("四"),_T("4"));
// 	strMakeDate.Replace(_T("五"),_T("5"));
// 	strMakeDate.Replace(_T("六"),_T("6"));
// 	strMakeDate.Replace(_T("七"),_T("7"));
// 	strMakeDate.Replace(_T("八"),_T("8"));
// 	strMakeDate.Replace(_T("九"),_T("9"));
// 	strMakeDate.Replace(_T("〇"),_T("0"));
// 	swscanf_s(strMakeDate,_T("%d年%d月%d日"),&nYear,&nMonth,&nDay);
// 	if (nYear == 0 || nMonth == 0 || nDay == 0)
// 	{
// 	}else
// 	{
// 		COleDateTime t(nYear,nMonth,nDay,0,0,0);
// 		DATE ConfigDate = t.m_dt;
// 		m_wndMakeDate.SetDate(ConfigDate);
// 	}
// }

BOOL CLPPrintMapSetsDlg::InitXZQComboTree(int nStopLevel)
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
		//m_wndXZQComboTree.SetSelectString(_T("[33]浙江省"));
	}
	m_wndXZQComboTree.m_wndTreeList.Expand(item,TVE_EXPAND);
	UpdateData(FALSE);
	return TRUE;
}

void CLPPrintMapSetsDlg::AddDistrictTreeItem(HTREEITEM parentItem/*=NULL*/,CComQIPtr<ILPXZQDataItem> spDataItem, int nStopLevel)
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
		hItem = m_wndXZQComboTree.AddItem(showValue,0,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 1)//市
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(2),ItemData->strName);
		hItem = m_wndXZQComboTree.AddItem(showValue,1,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 2)//县区
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(2),ItemData->strName);
		hItem = m_wndXZQComboTree.AddItem(showValue,2,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 4)//乡镇街道
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(3),ItemData->strName);
		hItem = m_wndXZQComboTree.AddItem(showValue,3,(DWORD)ItemData,parentItem);
 	}//else if (ItemData->nLevel  == 6)//村
// 	{
// 		CString showValue;
// 		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(3),ItemData->strName);
// 		hItem = m_wndXZQComboTree.AddItem(showValue,-1,(DWORD)ItemData,parentItem);
// 	}
	if (m_strXZQDM.Compare(ItemData->strCode) == 0)
	{
		m_pXZQItem = hItem;
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

void CLPPrintMapSetsDlg::InitImageList(CImageList** ppImageList)
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

void CLPPrintMapSetsDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData();
	RefleshCofigMap();//输出前作刷新
	if (m_bIsSaveConfige)
	{
		SaveConfigFile();
	}
	HTREEITEM pItem = m_wndXZQComboTree.GetSelectItem();
	if (pItem == NULL)
	{
		MessageBox(_T("请设置行政区"),_T("提示"),MB_OK);
		return;
	}
	_tagDistrictItem* pData = (_tagDistrictItem*)m_wndXZQComboTree.m_wndTreeList.GetItemData(pItem);
	m_strXZQDM = pData->strCode;
	CString strName = pData->strName;
	//if (m_strXZQDM.GetLength() < 6 || m_strXZQDM.GetLength() > 9)
	if (m_strXZQDM.GetLength() != 9)
	{
		MessageBox(_T("当前选定的行政区不是标准出图范围"),strName,MB_OK);
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
		if (pData->strName.Compare(_T("中心城区")) == 0)
		{//说明当前是中心城区
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
		strTilte.Format(_T("%s%s%s%s"),strParentName,strName,_T("土地利用总体规划（2006-2020）\n"),strMapName);
		m_strMapConfig.SetAt(_T("MAPTITLE"),strTilte);

		CString strMapWZ;
		strMapWZ.Format(_T("%s在%s位置示意图"),strName,strParentName);
		m_strMapConfig.SetAt(_T("MAPWZ"),strMapWZ);
	}
	if (m_strMakeWhere_ZF.GetLength() != 0)
	{
		if (m_strMakeWhere_ZF.Find(_T("人民政府")) < 0)
		{
			m_strMakeWhere_ZF += _T("人民政府");
			m_strMapConfig.SetAt(_T("MAPZF"),m_strMakeWhere_ZF);
		}
	}
	if (m_strMakeWhere_GTJ.GetLength() != 0)
	{
		m_strMakeWhere_GTJ += _T("\t制图");
		m_strMapConfig.SetAt(_T("MAPPUBLIC"),m_strMakeWhere_GTJ);
	}
	CString strTime;
	GetTimeString(strTime);
	if (strTime.GetLength() != 0)
	{
		m_strMapConfig.SetAt(_T("MAPRQ"),strTime);
	}
	CString strMapZFRQ;
	strMapZFRQ = m_strMakeWhere_ZF + _T("    ") + strTime;
	m_strMapConfig.SetAt(_T("MAPZFRQ"),strMapZFRQ);

	CGSBCGDialog::OnOK();
}



BOOL CLPPrintMapSetsDlg::InitViewConfig(CString strConfigFile, CString strRootName)
{
	//检查配置文件是否存在
	WIN32_FIND_DATA hFindData;
	HANDLE hFind;
	hFind = ::FindFirstFile(strConfigFile.GetBuffer(),&hFindData);
	//return hFind != INVALID_HANDLE_VALUE;
	if (hFind == INVALID_HANDLE_VALUE)
	{
		CString strTemp;
		strTemp.Format(_T("路径：%s\n指定文件无效！"),strConfigFile);
		BCGPMessageBox(strTemp,_T("提示"),MB_OK);
		return FALSE;
	}

	//WideCharToMultiByte
	int len =WideCharToMultiByte(CP_ACP,0,strConfigFile,-1,NULL,0,NULL,NULL);  
	//char* pszConfigFile =new char[len +1];  //这里不用加1，WideCharToMultiByte内部已经为我们自动加1了
	char* pszConfigFile =new char[len];  
	WideCharToMultiByte(CP_ACP,0,strConfigFile,-1,pszConfigFile,len,NULL,NULL );
	
	len = WideCharToMultiByte(CP_ACP,0,strRootName,-1,NULL,0,NULL,NULL);
	char* pszRootName = new char[len];
	WideCharToMultiByte(CP_ACP,0,strRootName,-1,pszRootName,len,NULL,NULL);

	TiXmlDocument xmlDoc(pszConfigFile);
	if(xmlDoc.LoadFile())
	{
		TiXmlElement *pRoot = xmlDoc.FirstChildElement();
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
BOOL CLPPrintMapSetsDlg::InitViewConfig2(CString strConfigFile, CString strRootName)
{
	USES_CONVERSION;
	//检查配置文件是否存在
	WIN32_FIND_DATA hFindData;
	HANDLE hFind;
	hFind = ::FindFirstFile(strConfigFile.GetBuffer(),&hFindData);
	//return hFind != INVALID_HANDLE_VALUE;
	if (hFind == INVALID_HANDLE_VALUE)
	{
		CString strTemp;
		strTemp.Format(_T("路径：%s\n指定文件无效！"),strConfigFile);
		BCGPMessageBox(strTemp,_T("提示"),MB_OK);
		return FALSE;
	}
	m_strConfigFilePath = strConfigFile;
	//WideCharToMultiByte
	int len =WideCharToMultiByte(CP_ACP,0,strConfigFile,-1,NULL,0,NULL,NULL);  
	//char* pszConfigFile =new char[len +1];  //这里不用加1，WideCharToMultiByte内部已经为我们自动加1了
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
void CLPPrintMapSetsDlg::InitConfigEdits()
{
	m_strMapConfig.Lookup(_T("MAPNAME"),m_strMapName);

	m_strMapConfig.Lookup(_T("MAPZF"),m_strMakeWhere_ZF);
	m_strMapConfig.Lookup(_T("MAPPUBLIC"),m_strMakeWhere_GTJ);
	m_strMapConfig.Lookup(_T("MAPXZQ"),m_strXZQDM);
	//日期单独处理，因为需要格式化读取和对bcg控件的设置
	
	//edit by lcc 2016.11.24这个函数在domodal之前调用不能执行刷新操作
	//UpdateData(FALSE);
}

void CLPPrintMapSetsDlg::SaveConfigFile()
{
	USES_CONVERSION;
	WIN32_FIND_DATA hFindData;
	HANDLE hFind;
	hFind = ::FindFirstFile(m_strConfigFilePath.GetBuffer(),&hFindData);
	//return hFind != INVALID_HANDLE_VALUE;
	if (hFind == INVALID_HANDLE_VALUE)
	{
		CString strTemp;
		strTemp.Format(_T("路径：%s\n指定文件无效！"),m_strConfigFilePath);
		BCGPMessageBox(strTemp,_T("提示"),MB_OK);
		return ;
	}

	//WideCharToMultiByte
	int len =WideCharToMultiByte(CP_ACP,0,m_strConfigFilePath,-1,NULL,0,NULL,NULL);  
	//char* pszConfigFile =new char[len +1];  //这里不用加1，WideCharToMultiByte内部已经为我们自动加1了
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

void CLPPrintMapSetsDlg::RefleshCofigMap()
{
	UpdateData();
	//获取设置的时间
// 	COleDateTime oledatatime((DATE)m_wndMakeDate.GetDate());
// 	int nYear = 0;
// 	int nMonth = 0;
// 	int nDay = 0;
// 	nYear = oledatatime.GetYear();
// 	nMonth = oledatatime.GetMonth();
// 	nDay = oledatatime.GetDay();
// 	CString strdate;
// 	strdate.Format(_T("%d年%d月%d日"),nYear,nMonth,nDay);
	//更新map中的设置
	m_strMapConfig.SetAt(_T("MAPNAME"),m_strMapName);
	m_strMapConfig.SetAt(_T("MAPZF"),m_strMakeWhere_ZF);
	m_strMapConfig.SetAt(_T("MAPPUBLIC"),m_strMakeWhere_GTJ);
	HTREEITEM pItem = m_wndXZQComboTree.GetSelectItem();
	_tagDistrictItem* pData = (_tagDistrictItem*)m_wndXZQComboTree.m_wndTreeList.GetItemData(pItem);
	m_strXZQDM = pData->strCode;
	m_strMapConfig.SetAt(_T("MAPXZQ"),m_strXZQDM);
//	m_strMapConfig.SetAt(_T("MAPRQ"),strdate);
}

void CLPPrintMapSetsDlg::OnBnClickedIsSaveSets()
{
	// TODO: 在此添加控件通知处理程序代码
// 	UpdateData();
// 	if (m_bIsSaveConfige)
// 	{
// 		SaveConfigFile();
// 	}
}


void CLPPrintMapSetsDlg::GetTimeString(CString &strTime)
{

	CString strYear;
	strYear.Format(_T("%d"),m_nYear);

	strYear.Replace(_T("1"),_T("一"));
	strYear.Replace(_T("2"),_T("二"));
	strYear.Replace(_T("3"),_T("三"));
	strYear.Replace(_T("4"),_T("四"));
	strYear.Replace(_T("5"),_T("五"));
	strYear.Replace(_T("6"),_T("六"));
	strYear.Replace(_T("7"),_T("七"));
	strYear.Replace(_T("8"),_T("八"));
	strYear.Replace(_T("9"),_T("九"));
	strYear.Replace(_T("0"),_T("〇"));

	CComBSTR bstrMon[12] = {_T("一"),_T("二"),_T("三"),_T("四"),_T("五"),_T("六"),
							_T("七"),_T("八"),_T("九"),_T("十"),_T("十一"),_T("十二")};
	int nMonth = m_wndMonthCombo.GetCurSel();
	CString strMonth(bstrMon[nMonth]);
	strTime.Format(_T("%s年%s月 编制"),strYear,strMonth);
}

void CLPPrintMapSetsDlg::InitTimeCtrl()
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
	m_wndMonthCombo.SelectString(-1,strMonth);
	
	UpdateData(FALSE);
}
void CLPPrintMapSetsDlg::utf8ToUnicode(LPCSTR src, CString *Result)  
{  
	int wcsLen = MultiByteToWideChar( CP_UTF8, 0, src, -1, NULL, 0 );  
	LPWSTR result = new wchar_t[wcsLen + 1];
	::MultiByteToWideChar( CP_UTF8, 0, src, -1, result, wcsLen); 
	Result->Format(_T("%s"),result);
	delete result;
} 
string CLPPrintMapSetsDlg::UnicodeToUTF8( CString &strIn )
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