// LPJBTSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LPJBTSetDlg.h"


// CLPJBTSetDlg �Ի���

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


// CLPJBTSetDlg ��Ϣ�������

BOOL CLPJBTSetDlg::OnInitDialog()
{
	CGSBCGDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CLPJBTSetDlg::InitViewConfig(CString strConfigFile, CString strRootName)
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
	//���ڵ���������Ϊ��Ҫ��ʽ����ȡ�Ͷ�bcg�ؼ�������

	//edit by lcc 2016.11.24���������domodal֮ǰ���ò���ִ��ˢ�²���
	//UpdateData(FALSE);
}

void CLPJBTSetDlg::InitTimeCtrl()
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
		/*CBCGPDateTimeCtrl::DTM_SPIN |*/
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR | 
		CBCGPDateTimeCtrl::DTM_DATE;
	m_wndMakeTime.SetState(DataState,stateMask);
	m_wndMakeTime.m_monthFormat = 2;
}
void CLPJBTSetDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
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
	//��ȡ���õ�ʱ��
 	COleDateTime oledatatime((DATE)m_wndMakeTime.GetDate());
 	int nYear = 0;
 	int nMonth = 0;
 	int nDay = 0;
 	nYear = oledatatime.GetYear();
 	nMonth = oledatatime.GetMonth();
 	nDay = oledatatime.GetDay();
 	CString strdate;
 	strdate.Format(_T("%d��%d��%d��"),nYear,nMonth,nDay);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	{//ѭ�����������ֵ�������ҵ���Ӧ�ı�
		CString DictIDTmp;
		utf8ToUnicode(DictElement->FirstAttribute()->Value(),&DictIDTmp);
		if (DictIDTmp.Compare(DictID) == 0) break;
		DictElement = DictElement->NextSiblingElement();
	}
	if (DictElement == NULL) return FALSE;// û�ҵ�
	m_wndDKLXCombo.AddItem(_T("ȫ��"),-1,0,NULL);
	InsertDLMCTreeItems(DictElement,NULL);
	//��������ѡ��
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
	//��ȡ�����������
	HTREEITEM hItem = m_wndDKLXCombo.GetSelectItem();
	CComBSTR bstrCode = (BSTR)m_wndDKLXCombo.m_wndTreeList.GetItemData(hItem);
	m_strMapConfig.SetAt(_T("MAPDL"),bstrCode);
	//��ȡ����д���ͼ����
	CString strName = m_wndDKLXCombo.m_wndTreeList.GetItemText(hItem);
	strName = strName.Right(strName.GetLength() - strName.Find(_T(")")) -1);
	m_strMapConfig.SetAt(_T("GHYTMC"),strName);
}
