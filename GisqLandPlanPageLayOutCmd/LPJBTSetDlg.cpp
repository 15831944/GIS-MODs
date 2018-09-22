// LPJBTSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LPJBTSetDlg.h"


// CLPJBTSetDlg 对话框

IMPLEMENT_DYNAMIC(CLPJBTSetDlg, CGSBCGDialog)

CLPJBTSetDlg::CLPJBTSetDlg(CWnd* pParent /*=NULL*/)
	: CGSBCGDialog(CLPJBTSetDlg::IDD, pParent)
	, m_MapPublic(_T(""))
	, m_strProducer(_T(""))
	, m_strChecker(_T(""))
	, m_bIsAnalysis(FALSE)
	, m_bIsSaveConfig(FALSE)
{

}

CLPJBTSetDlg::~CLPJBTSetDlg()
{
}

void CLPJBTSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CGSBCGDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MAKEWHERE_JBT, m_MapPublic);
	DDX_Text(pDX, IDC_EDIT_JBR, m_strProducer);
	DDX_Text(pDX, IDC_EDIT_SHR, m_strChecker);
	DDX_Check(pDX, IDC_ISANALYSIS, m_bIsAnalysis);
	DDX_Control(pDX, IDC_MAKEDATE, m_wndMakeTime);
	DDX_Check(pDX, IDC_SAVECONFIG, m_bIsSaveConfig);
	DDX_Control(pDX, IDC_DKLX_CBX, m_wndDKLXCombo);
}


BEGIN_MESSAGE_MAP(CLPJBTSetDlg, CGSBCGDialog)
	ON_BN_CLICKED(IDC_ISANALYSIS, &CLPJBTSetDlg::OnBnClickedIsanalysis)
END_MESSAGE_MAP()


// CLPJBTSetDlg 消息处理程序

BOOL CLPJBTSetDlg::OnInitDialog()
{
	CGSBCGDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_bIsAnalysisVisible == FALSE)
	{
		((CButton*)GetDlgItem(IDC_ISANALYSIS))->EnableWindow(FALSE);
	}
	GetDlgItem(IDC_DKLX_CBX)->EnableWindow(FALSE);
	if (m_wndDKLXCombo.m_wndTreeList.GetSafeHwnd() == NULL)
	{
		m_wndDKLXCombo.InitTree(200,200);
	}
	InitDLMCComboTree(_T("GHYTDM"));
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CLPJBTSetDlg::InitViewConfig(CString strConfigFile, CString strRootName)
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
	InitTimeCtrl();
;	return TRUE;
}

void CLPJBTSetDlg::InitConfigEdits()
{
	m_strMapConfig.Lookup(_T("MAPPUBLIC"),m_MapPublic);
	m_strMapConfig.Lookup(_T("PRODUCER"),m_strProducer);
	m_strMapConfig.Lookup(_T("CHECKER"),m_strChecker);
	//日期单独处理，因为需要格式化读取和对bcg控件的设置

	//edit by lcc 2016.11.24这个函数在domodal之前调用不能执行刷新操作
	//UpdateData(FALSE);
}

void CLPJBTSetDlg::InitTimeCtrl()
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
		/*CBCGPDateTimeCtrl::DTM_SPIN |*/
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR | 
		CBCGPDateTimeCtrl::DTM_DATE;
	m_wndMakeTime.SetState(DataState,stateMask);
	m_wndMakeTime.m_monthFormat = 2;
}
void CLPJBTSetDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData();
	m_strMapConfig.SetAt(_T("MAPPUBLIC"),m_MapPublic);
	m_strMapConfig.SetAt(_T("PRODUCER"),m_strProducer);
	m_strMapConfig.SetAt(_T("CHECKER"),m_strChecker);
	RefreshConfigDLMC();
	SetMakeTimetoMap();

	if (m_bIsSaveConfig)
	{
		SaveConfigFile();
	}
	CGSBCGDialog::OnOK();
}

void CLPJBTSetDlg::SetMakeTimetoMap()
{
	//获取设置的时间
 	COleDateTime oledatatime((DATE)m_wndMakeTime.GetDate());
 	int nYear = 0;
 	int nMonth = 0;
 	int nDay = 0;
 	nYear = oledatatime.GetYear();
 	nMonth = oledatatime.GetMonth();
 	nDay = oledatatime.GetDay();
 	CString strdate;
 	strdate.Format(_T("%d年%d月%d日"),nYear,nMonth,nDay);
	m_strMapConfig.SetAt(_T("MAPRQ"),strdate);

}

void CLPJBTSetDlg::SetIsAnalysisVisible(BOOL bIsAnalysis)
{
	m_bIsAnalysisVisible = bIsAnalysis;
}

void CLPJBTSetDlg::SaveConfigFile()
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
			string strtmp;
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

void CLPJBTSetDlg::utf8ToUnicode(LPCSTR src, CString *Result)  
{  
	int wcsLen = MultiByteToWideChar( CP_UTF8, 0, src, -1, NULL, 0 );  
	LPWSTR result = new wchar_t[wcsLen + 1];
	::MultiByteToWideChar( CP_UTF8, 0, src, -1, result, wcsLen); 
	Result->Format(_T("%s"),result);
	delete result;
} 
string CLPJBTSetDlg::UnicodeToUTF8( CString &strIn )
{
	char*     pElementText;
	int    iTextLen;
	LPCWSTR str = strIn;
	//wstring str = strIn;
	// wide char to multi char
	iTextLen = WideCharToMultiByte( CP_UTF8,0,str,	-1,NULL,0,	NULL,NULL );
	pElementText = new char[iTextLen + 1];
	memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
	::WideCharToMultiByte( CP_UTF8,	0,str,-1,pElementText,iTextLen,	NULL,NULL );
	string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}
void CLPJBTSetDlg::OnBnClickedIsanalysis()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (m_bIsAnalysis == TRUE)
	{
		GetDlgItem(IDC_DKLX_CBX)->EnableWindow(TRUE);
	}else
	{
		GetDlgItem(IDC_DKLX_CBX)->EnableWindow(FALSE);
	}
}

BOOL CLPJBTSetDlg::InitDLMCComboTree(CString DictID)
{
	m_wndDKLXCombo.DeleteAllItems();
	USES_CONVERSION;

	TCHAR szAppFullPath[MAX_PATH];
	DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(CHAR);
	GetModuleFileName(NULL,szAppFullPath,dBufferSize);
	CString strAppPath(szAppFullPath);
	strAppPath = strAppPath.Mid(0,strAppPath.ReverseFind('\\'));
	CString strConfigFile = strAppPath + RULEPATH;

	TiXmlDocument XmlDoc;
	TiXmlElement *RootElement = NULL;
	if (XmlDoc.LoadFile(W2A(strConfigFile.AllocSysString())) == false) return FALSE;
	RootElement = XmlDoc.FirstChildElement();
	TiXmlElement *DictRootElement;
	DictRootElement = RootElement->FirstChildElement("DictionaryRule");
	TiXmlElement *DictElement;
	DictElement = DictRootElement->FirstChildElement();
	while(DictElement != NULL)
	{//循环遍历所有字典表名，找到对应的表
		CString DictIDTmp;
		utf8ToUnicode(DictElement->FirstAttribute()->Value(),&DictIDTmp);
		if (DictIDTmp.Compare(DictID) == 0) break;
		DictElement = DictElement->NextSiblingElement();
	}
	if (DictElement == NULL) return FALSE;// 没找到
	m_wndDKLXCombo.AddItem(_T("全部"),-1,0,NULL);
	InsertDLMCTreeItems(DictElement,NULL);
	//根据配置选中
	CString strDLDM;
	m_strMapConfig.Lookup(_T("MAPDL"),strDLDM);
	m_wndDKLXCombo.SetSelectString(strDLDM,FALSE);

	return TRUE;
}
void CLPJBTSetDlg::InsertDLMCTreeItems(TiXmlElement *CodeNamesElement, HTREEITEM pParentItem)
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

		HTREEITEM pItem = m_wndDKLXCombo.AddItem(strShowValue,0,(DWORD)strCode.AllocSysString(),pParentItem);
		if (CodeElement->FirstChildElement() != NULL) InsertDLMCTreeItems(CodeElement,pItem);
		CodeElement = CodeElement->NextSiblingElement();
	}

}

void CLPJBTSetDlg::RefreshConfigDLMC()
{
	//获取代码记入配置
	HTREEITEM hItem = m_wndDKLXCombo.GetSelectItem();
	CComBSTR bstrCode = (BSTR)m_wndDKLXCombo.m_wndTreeList.GetItemData(hItem);
	m_strMapConfig.SetAt(_T("MAPDL"),bstrCode);
	//获取名称写入出图属性
	CString strName = m_wndDKLXCombo.m_wndTreeList.GetItemText(hItem);
	strName = strName.Right(strName.GetLength() - strName.Find(_T(")")) -1);
	m_strMapConfig.SetAt(_T("GHYTMC"),strName);
}
