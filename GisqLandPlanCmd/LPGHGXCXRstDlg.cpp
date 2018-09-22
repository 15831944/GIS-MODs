// LPGHGXCXRstDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LPGHGXCXRstDlg.h"


// CLPGHGXCXRstDlg �Ի���

IMPLEMENT_DYNAMIC(CLPGHGXCXRstDlg, CBCGPDialog)

CLPGHGXCXRstDlg::CLPGHGXCXRstDlg(GisqLib::_DGisqFrameworkCOMPtr	ipFramework,CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CLPGHGXCXRstDlg::IDD, pParent)
{
	m_spFramework = ipFramework;
	m_pwndFADKGrid = NULL;
	m_pwndFAInfoGrid = NULL;
	m_pwndResTabWnd = NULL;
	m_nFASelectIndex = CB_ERR;
}
CLPGHGXCXRstDlg::CLPGHGXCXRstDlg(CComQIPtr<GisqLib::_DGisqFrameworkCOM>	ipFramework,CWnd* pParent /*=NULL*/)
: CBCGPDialog(CLPGHGXCXRstDlg::IDD, pParent)
{
	m_spFramework = ipFramework;
}

CLPGHGXCXRstDlg::~CLPGHGXCXRstDlg()
{
	if (m_pwndFAInfoGrid != NULL)
	{
		if (m_pwndFAInfoGrid->GetSafeHwnd() != NULL)
		{
			delete m_pwndFAInfoGrid;
		}
	}
	if (m_pwndResTabWnd != NULL)
	{
		if (m_pwndResTabWnd->GetSafeHwnd() != NULL)
		{
			delete m_pwndResTabWnd;
		}
	}
}

void CLPGHGXCXRstDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FALIST, m_wndFARstList);
}


BEGIN_MESSAGE_MAP(CLPGHGXCXRstDlg, CBCGPDialog)
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_FALIST, &CLPGHGXCXRstDlg::OnLbnSelchangeFalist)
END_MESSAGE_MAP()


// CLPGHGXCXRstDlg ��Ϣ�������

void CLPGHGXCXRstDlg::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);
 	if (GetSafeHwnd() == NULL)
 	{
 		return;
 	}
 	CRect ListRect;
 	CRect TabRect;
 	GetClientRect(&ListRect);
 	GetClientRect(&TabRect);
 	if (m_wndFARstList.GetSafeHwnd() != NULL)
 	{
 		ListRect.right = ListRect.left + 200;
 		m_wndFARstList.MoveWindow(ListRect);
 	}
 	if (m_pwndResTabWnd != NULL)
 	{
 		if (m_pwndResTabWnd->GetSafeHwnd() != NULL)
 		{
 			TabRect.left= TabRect.left + 200;
 			m_pwndResTabWnd->MoveWindow(TabRect);
 		}
 	}
	// TODO: �ڴ˴������Ϣ����������
}

BOOL CLPGHGXCXRstDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rc;
	GetClientRect(&rc);	
	if (m_pwndResTabWnd == NULL)
	{
		m_pwndResTabWnd = new CBCGPTabWnd;
		m_pwndResTabWnd->Create(CBCGPTabWnd::STYLE_3D , rc, this, IDC_RESGRIDTAB, CBCGPTabWnd::LOCATION_TOP, TRUE);
	}
	if (m_pwndFAInfoGrid == NULL)
	{
		CRect rectTabArea;
		CWnd *pParent = m_pwndResTabWnd;
		m_pwndResTabWnd->GetClientRect(&rectTabArea);
		ClientToScreen(rectTabArea);
		m_pwndFAInfoGrid = new CCXResBCGGridCtrl;
		CWnd *pTabwnd = m_pwndFAInfoGrid;
		pTabwnd->Create(NULL,_T("������Ϣ"),WS_CHILD | WS_VISIBLE | ES_CENTER, rectTabArea, pParent, IDD_GRIDDOCKTABWND,NULL);
		m_pwndFAInfoGrid->EnableMarkSortedColumn (FALSE);//���ñ�����ʼ��Ϊ��
		m_pwndFAInfoGrid->EnableHeader(FALSE);//������ͷ
		m_pwndFAInfoGrid->SetReadOnly(TRUE);//ֻ��
		CBCGPGridColors GridTheme;
		GridTheme = m_pwndFAInfoGrid->GetColorTheme();
		GridTheme.m_clrHorzLine = RGB (0, 0, 0);//����ˮƽ��������ɫ
		GridTheme.m_clrVertLine = RGB (0, 0, 0);//���ô�ֱ��������ɫ
		CBCGPGridColors::ColorData SelColors;
		SelColors.m_clrBackground = RGB (168,205,241);
		SelColors.m_clrBorder = RGB (168,205,241);
		SelColors.m_clrGradient = -1;
		SelColors.m_clrText = RGB (0, 0, 0);
		SelColors.m_nGradientAngle = 0;
		GridTheme.m_SelColors = SelColors;//����ѡ����ɫ
		m_pwndFAInfoGrid->SetColorTheme(GridTheme);
		for (int i = 0;i < 8; i++)
		{
			m_pwndFAInfoGrid->InsertColumn(i,_T("culonm"),130);
			m_pwndFAInfoGrid->SetColumnAlign(i,HDF_CENTER);
		}
		m_pwndResTabWnd->AddTab(m_pwndFAInfoGrid,_T("������Ϣ"));
	}
	InitMaps();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CLPGHGXCXRstDlg::InitMaps()
{
	USES_CONVERSION;

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
	//�������ͽڵ�
	TiXmlElement *FALXDictElement = NULL;
	//����״̬�ڵ�
	TiXmlElement *GXZTDictElement = NULL;
	while(DictElement != NULL)
	{//ѭ�����������ֵ�������ҵ���Ӧ�ı�
		CString DictIDTmp;
		utf8ToUnicode(DictElement->FirstAttribute()->Value(),&DictIDTmp);
		if (DictIDTmp.Compare(_T("GXZT")) == 0) GXZTDictElement = DictElement;
		if (DictIDTmp.Compare(_T("GXFALX")) == 0) FALXDictElement = DictElement;
		if (GXZTDictElement!= NULL && FALXDictElement != NULL) break;
		DictElement = DictElement->NextSiblingElement();
	}
	//����״̬map
	if (GXZTDictElement != NULL)
	{
		TiXmlElement *CodeNamesElement = GXZTDictElement->FirstChildElement();
		TiXmlElement *CodeNameElement = CodeNamesElement->FirstChildElement();
		while(CodeNameElement != NULL)
		{
			CString strCode;
			CString strName;
			utf8ToUnicode(CodeNameElement->Attribute("Code"),&strCode);
			utf8ToUnicode(CodeNameElement->Attribute("Name"),&strName);
			m_strmapGXZT.SetAt(strCode,strName);
			CodeNameElement = CodeNameElement->NextSiblingElement();
		}
	}
	//��������map
	if (FALXDictElement != NULL)
	{
		TiXmlElement *CodeNamesElement = FALXDictElement->FirstChildElement();
		TiXmlElement *CodeNameElement = CodeNamesElement->FirstChildElement();
		while(CodeNameElement != NULL)
		{
			CString strCode;
			CString strName;
			utf8ToUnicode(CodeNameElement->Attribute("Code"),&strCode);
			utf8ToUnicode(CodeNameElement->Attribute("Name"),&strName);
			m_strmapFALX.SetAt(strCode,strName);
			CodeNameElement = CodeNameElement->NextSiblingElement();
		}
	}
}

HRESULT CLPGHGXCXRstDlg::ConnToOracle(_ConnectionPtr &ipADOConn)
{
	CComBSTR bstrConnstr;
	CComBSTR bstrName;
	CComBSTR bstrPasswd;
	getConnString(bstrConnstr,bstrName,bstrPasswd);
	if (ipADOConn == NULL)
	{
		ipADOConn.CreateInstance(__uuidof(Connection));
	}
	HRESULT hr = ipADOConn->Open(_bstr_t(bstrConnstr),_bstr_t(bstrName),_bstr_t(bstrPasswd) , adConnectUnspecified);
	if (hr != S_OK)
	{
		return hr;
	}
	return S_OK;
}

HRESULT CLPGHGXCXRstDlg::getConnString(CComBSTR &bstrConnSet,CComBSTR &bstrName,CComBSTR &bstrPasswd)
{
	TiXmlDocument XmlDoc;
	CString strIP;
	CString strPort;
	CString strUser;
	CString strPassword;
	CString strConnSet;
	USES_CONVERSION;
	//edit by lcc 2016.12.06 ���Ļ�ȡxml·���ķ�ʽ
	TCHAR exeFullPath[MAX_PATH]; 
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	CString strExePath=exeFullPath;
	int index =strExePath.ReverseFind('\\');
	CString strPath = strExePath.Left(index)+_T("\\data\\config.xml");
	//edit by lcc 2016.12.06 end
	if (XmlDoc.LoadFile(W2A(_bstr_t(strPath))) == false)
	{
		//MessageBox(_T("�޷��������ļ�"),_T("����"),MB_OK);
		return E_FAIL;
	}

	TiXmlElement *pSdeSet = XmlDoc.FirstChildElement("connect")->FirstChildElement("gh");
	if (pSdeSet != NULL)
	{
		TiXmlElement *pElement = pSdeSet->FirstChildElement("ip");
		if (pElement != NULL)
		{
			strIP = pElement->FirstAttribute()->Value();
		}
		pElement = NULL;
		pElement = pSdeSet->FirstChildElement("port");
		if (pElement != NULL)
		{
			strPort = pElement->FirstAttribute()->Value();
		}
		pElement = NULL;
		pElement = pSdeSet->FirstChildElement("user");
		if (pElement != NULL)
		{
			strUser = pElement->FirstAttribute()->Value();
		}
		pElement = NULL;
		pElement = pSdeSet->FirstChildElement("passwd");
		if (pElement != NULL)
		{
			strPassword = pElement->FirstAttribute()->Value();
		}
	}
	strConnSet.Format(_T("Provider=OraOLEDB.Oracle.1;User ID=%s;Password=%s;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=%s)(PORT=%s))(CONNECT_DATA=(SERVICE_NAME=%s)));Persist Security Info=False"),
		strUser,strPassword,strIP,_T("1521"),strPort);
	bstrConnSet = (LPCTSTR)strConnSet;
	bstrName = (LPCTSTR)strUser;
	bstrPasswd = (LPCTSTR)strPassword;
	return S_OK;
}

HRESULT CLPGHGXCXRstDlg::SearchFA(CComBSTR strQrueyQueryFilter)
{
	m_wndFARstList.CleanUp();
	m_csFAInfo.ClearInfo();
	ShowFAInfo();
	HRESULT hr;
	_ConnectionPtr ipADOConn;
	_RecordsetPtr pRst;
	VARIANT vt;
	hr = ipADOConn.CreateInstance(__uuidof(Connection));
	hr = pRst.CreateInstance(__uuidof(Recordset));
	hr = ConnToOracle(ipADOConn);
	if (hr != S_OK)
	{
		BCGPMessageBox(_T("���ݿ�����ʧ��"),_T("����"),MB_OK);
		return E_FAIL;
	}
	hr = pRst->Open(_variant_t(strQrueyQueryFilter),_variant_t(ipADOConn.GetInterfacePtr()), adOpenStatic,adLockOptimistic,adCmdText);
	//��ȡ������Ϣ
	long nCount = 0;
	pRst->get_RecordCount(&nCount);
	hr = pRst->GetRows(nCount,vtMissing,vtMissing,&vt);
	if (!SUCCEEDED(hr)) return E_FAIL;
	//��ȡ����ָ�������
	void *pvd = NULL;
	SafeArrayAccessData(vt.parray,&pvd);
	LONG nFieldNum = 0;
	SafeArrayGetUBound(vt.parray,(UINT)1,&nFieldNum);
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		CString *pstrFABSM;
		CString strFAMC;
		for (LONG nField = 0; nField <= nFieldNum; nField++)
		{	
			VARIANT vtTmp = ((VARIANT*)pvd)[nField+(nIndex)*(nFieldNum + 1)];
			switch (nField)
			{
			case 0:
				pstrFABSM = new CString;
				*pstrFABSM = VariantToString(vtTmp);
				break;
			case 1:
				strFAMC = VariantToString(vtTmp);				
				m_wndFARstList.SetItemData(m_wndFARstList.AddString(strFAMC),(DWORD_PTR)pstrFABSM);
				break;
			default:
				break;
			}
		}
	}
	pRst->Close();
	ipADOConn->Close();

	return S_OK;
}

void CLPGHGXCXRstDlg::OnLbnSelchangeFalist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_wndFARstList.GetCurSel();
	if (nIndex != m_nFASelectIndex)
	{
		CString *strSelText = (CString*)m_wndFARstList.GetItemData(nIndex);
		HRESULT hr;
		_ConnectionPtr ipADOConn;
		hr = ConnToOracle(ipADOConn);
		if (hr != S_OK)
		{
			BCGPMessageBox(_T("���ݿ�����ʧ��"),_T("����"),MB_OK);
			return;
		}
		//��ȡ��ǰ������Ϣ
		GetFAInfoByUID(ipADOConn,*strSelText);
		//��ʾ������Ϣ
		ShowFAInfo();
	}
}
HRESULT CLPGHGXCXRstDlg::GetFAInfoByUID(_ConnectionPtr &ipADOConn, CString &strFABSM)
{
	HRESULT hr;
	CComVariant vt;
	_RecordsetPtr pRst;
	pRst.CreateInstance(__uuidof(Recordset));

	CString strQrueyQueryFilter;
	GetSQLstring(strFABSM,strQrueyQueryFilter);

	hr = pRst->Open(_variant_t(strQrueyQueryFilter),_variant_t(ipADOConn.GetInterfacePtr()), adOpenStatic,adLockOptimistic,adCmdText);
	if (hr != S_OK)
	{
		return E_FAIL;
		pRst->Close();
	}
	//��ȡ������Ϣ
	long nCount = 0;
	pRst->get_RecordCount(&nCount);
	hr = pRst->GetRows(nCount,vtMissing,vtMissing,&vt);
	if (!SUCCEEDED(hr)) return E_FAIL;
	//��ȡ����ָ�������
	void *pvd = NULL;
	SafeArrayAccessData(vt.parray,&pvd);
	LONG nFieldNum = 0;
	SafeArrayGetUBound(vt.parray,(UINT)1,&nFieldNum);
	//�����һ��������������Ϣ
	m_csFAInfo.ClearInfo();
	//�ؿ�����������Ӧ�������ϸ�һֱ
	m_csFAInfo.nDKCount = nCount;
	//��Ҫ����ʱ����
	//�������ʹ���
	CString strFALXDM;
	//��������ר�����
	CString strFAGXZTDM;
	//��Ŀ��ʶ��
	CString strXMBSM = _T("");
	//�Ƿ�����Ŀ��
	BOOL bIsNewXMLine = TRUE;
	//���ڶ�ȡһ����Ŀ����ʱ����
	CXMInfoClass *XMInfo;
	//ʵ����һ�����������ӿڣ����ڻ�ȡ�������ֵ����������뷭��Ϊ��������
	CComQIPtr<ILPXZQDataTree> spXZQTree;
	spXZQTree.CoCreateInstance(CLSID_LPXZQDataTree);
	IUnknown *pMapUnknow;
	spXZQTree->GetXZQMap(&pMapUnknow);
	CMapStringToString *pXZQMap = (CMapStringToString *)pMapUnknow;

	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		//Ԥ����Ŀ��ʶ���ж��ǲ����µ���Ŀ
		VARIANT vtXMBSMTmp = ((VARIANT*)pvd)[25+(nIndex)*(nFieldNum + 1)];
		if (strXMBSM.Compare(VariantToString(vtXMBSMTmp)) == 0)
		{
			bIsNewXMLine = FALSE;
		}else
		{
			strXMBSM = vtXMBSMTmp;
			bIsNewXMLine = TRUE;
			XMInfo = NULL;
			XMInfo = new CXMInfoClass;
			m_csFAInfo.m_arrXMInfo.Add(XMInfo);
		}
		//ÿһ�п϶�����һ���µĵؿ�
		CDKInfoClass *pDKInfo = new CDKInfoClass;
		XMInfo->m_arrDKInfo.Add(pDKInfo);
		for (LONG nField = 0; nField <= nFieldNum; nField++)
		{	
			VARIANT vtTmp = ((VARIANT*)pvd)[nField+(nIndex)*(nFieldNum + 1)];
			//��������ֻ��Ҫ��һ�ξ͹���
			if (nIndex == 0)
			{
				switch (nField)
				{
				case 0://������ʶ��
					m_csFAInfo.strFABSM = VariantToString(vtTmp);
					break;
				case 1://��������
					m_csFAInfo.strFAMC = VariantToString(vtTmp);
					break;
				case 2://�����
					m_csFAInfo.strSLH = VariantToString(vtTmp);
					break;
				case 3://��������
					strFALXDM = VariantToString(vtTmp);
					strFALXDM = strFALXDM.Left(strFALXDM.Find(_T(".")));
					m_strmapFALX.Lookup(strFALXDM,m_csFAInfo.strFALX);
					break;
				case 4://�ش���
					m_csFAInfo.strXDM = VariantToString(vtTmp);
					break;
				case 5://������
					m_csFAInfo.strXMC = VariantToString(vtTmp);
					break;
				case 6://�������	
					m_csFAInfo.dbZMJ = (double)(_variant_t)vtTmp;
					break;
				case 7://ũ�õ����
					m_csFAInfo.dbNYDMJ = (double)(_variant_t)vtTmp;
					break;
				case 8://���ؾ����
					m_csFAInfo.dbGDJMJ = (double)(_variant_t)vtTmp;
					break;
				case 9://�����õ����
					m_csFAInfo.dbJSYDMJ = (double)(_variant_t)vtTmp;
					break;
				case 10://δ���õ����
					m_csFAInfo.dbWLYDMJ = (double)(_variant_t)vtTmp;
					break;
				case 11://����ũ�����
					m_csFAInfo.dbJBNTJMJ = (double)(_variant_t)vtTmp;
					break;
				case 12://���������õ�ʡ��ָ��
					m_csFAInfo.dbJSYDZBsheng = (double)(_variant_t)vtTmp;
					break;
				case 13://���������õ��м�ָ��
					m_csFAInfo.dbJSYDZBshi = (double)(_variant_t)vtTmp;
					break;
				case 14://���������õ��ؼ�ָ��
					m_csFAInfo.dbJSYDZBxian = (double)(_variant_t)vtTmp;
					break;
				case 15://���������õ��缶ָ��
					m_csFAInfo.dbJSYDZBxiang = (double)(_variant_t)vtTmp;
					break;
				case 16://����ũ��ʡ��ָ��
					m_csFAInfo.dbJBNTZBsheng = (double)(_variant_t)vtTmp;
					break;
				case 17://����ũ���м�ָ��
					m_csFAInfo.dbJBNTZBshi = (double)(_variant_t)vtTmp;
					break;
				case 18://����ũ���ؼ�ָ��
					m_csFAInfo.dbJBNTZBxian = (double)(_variant_t)vtTmp;
					break;
				case 19://����ũ���缶ָ��
					m_csFAInfo.dbJBNTZBxiang = (double)(_variant_t)vtTmp;
					break;
				case 20://����ʱ��
					m_csFAInfo.strGXSJ = VariantToString(vtTmp);
					break;
				case 21://����״̬
					strFAGXZTDM = VariantToString(vtTmp);
					strFAGXZTDM = strFAGXZTDM.Left(strFAGXZTDM.Find(_T(".")));
					m_strmapGXZT.Lookup(strFAGXZTDM,m_csFAInfo.strGXZT);
					break;
				case 22://��׼����
					m_csFAInfo.strSPJG = VariantToString(vtTmp);
					break;
				case 23://��׼ʱ��
					m_csFAInfo.strSPSJ = VariantToString(vtTmp);
					break;
				case 24://��Ŀ�����
					m_csFAInfo.dbXMZMJ = (double)(_variant_t)vtTmp;
					break;
				default:
					break;
				}
			}
			//������Ŀ�����Ϣ
			//������Ŀ��ʶ�벻ͬʱ������Ŀ��Ϣ
			if (bIsNewXMLine == TRUE)
			{
				switch(nField)
				{
				case 25://��Ŀ��ʶ��
					XMInfo->strXMBSM = VariantToString(vtTmp);
					break;
				case 26://��Ŀ����
					XMInfo->strXMMC = VariantToString(vtTmp);
					break;
				case 27://��Ŀ���ͣ������Ҫ����
					XMInfo->strXMLX = VariantToString(vtTmp);
					break;
				case 28://��Ŀ��������������ĺϼ�Ӧ���뷽�����е���Ŀ�������ͬ
					XMInfo->dbZMJ = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumZMJ += XMInfo->dbZMJ;
					break;
				case 29://ũ�õ����
					XMInfo->dbNYDMJ = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumNYDMJ += XMInfo->dbNYDMJ;
					break;
				case 30://���ؾ����
					XMInfo->dbGDJMJ = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumGDJMJ += XMInfo->dbGDJMJ;
					break;
				case 31://�����õ����
					XMInfo->dbJSYDMJ = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumJSYDMJ += XMInfo->dbJSYDMJ;
					break;
				case 32://δ���õ����
					XMInfo->dbWLYDMJ = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumWLYDMJ += XMInfo->dbWLYDMJ;
					break;
				case 33://����ũ�����
					XMInfo->dbJBNTJMJ = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumJBNTJMJ += XMInfo->dbJBNTJMJ;
					break;
				case 34://�����õ�ʡ��ָ��
					XMInfo->dbJSYDZBsheng = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumXZJSYD1 += XMInfo->dbJSYDZBsheng;
					break;
				case 35://�����õ��м�ָ��
					XMInfo->dbJSYDZBshi = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumXZJSYD2 += XMInfo->dbJSYDZBshi;
					break;
				case 36://�����õ��ؼ�ָ��
					XMInfo->dbJSYDZBxian = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumXZJSYD3 += XMInfo->dbJSYDZBxian;
					break;
				case 37://�����õ��缶ָ��
					XMInfo->dbJSYDZBxiang = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumXZJSYD4 += XMInfo->dbJSYDZBxiang;
					break;
				case 38://����ũ��ʡ��ָ��
					XMInfo->dbJBNTZBsheng = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumJBNT1 += XMInfo->dbJBNTZBsheng;
					break;
				case 39://����ũ���м�ָ��
					XMInfo->dbJBNTZBshi = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumJBNT2 += XMInfo->dbJBNTZBshi;
					break;
				case 40://����ũ���ؼ�ָ��
					XMInfo->dbJBNTZBxian = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumJBNT3 += XMInfo->dbJBNTZBxian;
					break;
				case 41://����ũ���缶ָ��
					XMInfo->dbJBNTZBxiang = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumJBNT4 += XMInfo->dbJBNTZBxiang;
					break;
				}
			}
			//�����ؿ���Ϣ
			switch(nField)
			{
				case 42://�ؿ��ʶ��
					pDKInfo->strDKBSM = VariantToString(vtTmp);
					break;
				case 43:
					pDKInfo->strXZDM = VariantToString(vtTmp);
					pXZQMap->Lookup(pDKInfo->strXZDM,pDKInfo->strXZMC);
					m_csFAInfo.m_mapstrSJXZMC.SetAt(pDKInfo->strXZDM,pDKInfo->strXZMC);
					break;
				case 44:
					pDKInfo->strCUNDM = VariantToString(vtTmp);
					break;
				case 45:
					pDKInfo->strCUNMC = VariantToString(vtTmp);
					break;
				case 46:
					break;
				case 47:
					break;
				case 48:
					break;
				case 49:
					break;
				case 50:
					break;
				case 51:
					break;
				default:
					break;
			}
		}
	}
	pRst->Close();
	//�������ǽ�������������Ŀ����ص����ݲ�д�뷽����
	//��Ŀ��������Ŀ�����С��ͬ
	m_csFAInfo.nXMCount = m_csFAInfo.m_arrXMInfo.GetCount();
	//�漰������������������С��ͬ
	m_csFAInfo.nXZCount = m_csFAInfo.m_mapstrSJXZMC.GetCount();
	//�漰��Ŀ���ƣ�����Ŀ�����һ����Ŀ�����ƺ���Ŀ����ƴ��
	m_csFAInfo.strSJXMMC.Format(_T("%s��%d��������Ŀ"),(m_csFAInfo.m_arrXMInfo.GetAt(0))->strXMMC,m_csFAInfo.nXMCount);
	//�漰�滮���� ������ + �������ƣ���ֹһ������������ĳ����� + ������������滮��2006-2020��
	if (m_csFAInfo.m_mapstrSJXZMC.GetCount() > 1)
	{
		m_csFAInfo.strGHMC.Format(_T("%s���ĳ���������������滮��2006-2020��"),m_csFAInfo.strXMC);
	}else
	{
		m_csFAInfo.strGHMC.Format(_T("%s%s������������滮��2006-2020��"),m_csFAInfo.strXMC,((m_csFAInfo.m_arrXMInfo.GetAt(0))->m_arrDKInfo.GetAt(0))->strXZMC);
	}
	return S_OK;
}

//��Ϊ�������飬��ѯ���Ƚϳ���ר����һ����������֯
void CLPGHGXCXRstDlg::GetSQLstring(CString &strFABSM, CString &strSQL)
{
	CString strSelect = _T("");
	CString strFrom = _T("");
	CString strWhere = _T("");
	CString strOrderby = _T("");

	strSelect = _T("select ");
	strFrom = _T(" from GHGXFA f, GHGXXM x, GHGXDK d ");
	strWhere.Format(_T(" where d.xmbsm = x.bsm and x.fabsm = f.bsm and f.bsm = '%s' "),strFABSM);
	strOrderby = _T(" order by f.famc,x.xmmc,d.xzdm, d.xzqdm, d.ytdkbh");
	//һ��д��������������ʱ��������
	//Ӧ���õؿ�UID��ΨһԼ������
	
	//0-4��������Ϣ:������ʶ�룬�������ƣ�����ţ��������ͣ��ش���
	strSelect += _T(" f.bsm as xmbsm , f.famc, f.slh, f.falx, f.xdm");
	//5-9��������Ϣ:�������ƣ������ũ�õ���������ؾ�����������õ����
	strSelect += _T(", f.xmc, f.mj as famj, f.nydmj as fanydmj, f.gdjmj as fagdjmj, f.jsydmj as fajsydmj");
	//10-14��������Ϣ��δ���õ����������ũ�ﾻ��������������õ�ʡ��ָ�꣬���������õ��м�ָ�꣬���������õ��ؼ�ָ��
	strSelect += _T(", f.wlydmj as fawlydmj, f.jbntjmj as fajbntjmj, f.xzjsyd1 as faxzjsyd1, f.xzjsyd2 as faxzjsyd2, f.xzjsyd3 as faxzjsyd3");
	//15-19��������Ϣ�����������õ��缶ָ�꣬����ũ��ʡ��ָ�꣬����ũ���м�ָ�꣬����ũ���ؼ�ָ�꣬����ũ���缶ָ��
	strSelect += _T(", f.xzjsyd4 as faxzjsyd, f.jbnt1 as fajbnt1, f.jbnt2 as fajbnt2, f.jbnt3 as fajbnt3, f.jbnt4 as fajbnt4");
	//20-24��������Ϣ������ʱ�䣬����״̬,��׼���أ���׼ʱ��,��Ŀ�����
	//strSelect += _T(", f.gxsj, f.gxzt, f.pzjg, f.pzsj, f.xmzmj");
	strSelect += _T(", f.gxsj, f.gxzt, f.pzjg, f.pzsj, f.mj as xmzmj");

	//25-29��Ŀ����Ϣ����Ŀ��ʶ�룬��Ŀ���ƣ���Ŀ���ͣ������,ũ�õ����
	strSelect += _T(", x.bsm as xmbsm, x.xmmc, x.xmlx, x.mj as xmzmj, x.nydmj as xmnydmj");
	//30-34��Ŀ����Ϣ�����ؾ�����������õ������δ���õؾ����������ũ�ﾻ��������������õ�ʡ��ָ��
	strSelect += _T(", x.gdjmj as xmgdjmj, x.jsydmj as xmjsydmj, x.wlydmj as xmwlydmj, x.jbntjmj as xmjbntjmj, x.xzjsyd1 as xmxzjsyd1");
	//35-39��Ŀ����Ϣ�����������õ��м�ָ�꣬���������õ��ؼ�ָ�꣬���������õ��缶ָ�꣬����ũ��ʡ��ָ�꣬����ũ���м�ָ��
	strSelect += _T(", x.xzjsyd2 as xmxzjsyd2, x.xzjsyd3 as xmxzjsyd3, x.xzjsyd4 as xmxzjsyd, x.jbnt1 as xmjbnt1, x.jbnt2 as xmjbnt2");
	//40-41��Ŀ����Ϣ������ũ���ؼ�ָ�꣬����ũ���缶ָ��
	strSelect += _T(", x.jbnt3 as xmjbnt3, x.jbnt4 as xmjbnt4");

	//42-44�ؿ����Ϣ����ʶ�룬������룬������������
	strSelect += _T(", d.bsm as dkbsm, d.xzdm, d.xzqdm as cundm");
	//45-49�ؿ����Ϣ�������������ƣ�ԭ�滮������룬ԭ�滮�������ƣ�ԭ��;�ؿ��ţ�ԭ�滮��;����
	strSelect += _T(", d.xzqmc as cunmc, d.yghdldm, d.yghdlmc, d.yytdkbh, d.yghytdm");
	//50-54


	strSQL = strSelect + strFrom + strWhere + strOrderby;
}

void CLPGHGXCXRstDlg::utf8ToUnicode(LPCSTR src, CString *Result)  
{  
	int wcsLen = MultiByteToWideChar( CP_UTF8, 0, src, -1, NULL, 0 );  
	LPWSTR result = new wchar_t[wcsLen + 1];
	::MultiByteToWideChar( CP_UTF8, 0, src, -1, result, wcsLen); 
	Result->Format(_T("%s"),result);
	delete result;
} 

HRESULT CLPGHGXCXRstDlg::ShowFAInfo()
{
	if (m_pwndFAInfoGrid == NULL) return E_FAIL;
	CString strTmpMJ;
	
	m_pwndFAInfoGrid->RemoveAll();
	CBCGPGridRow *pRow1 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	//��һ��
	pRow1->GetItem(0)->SetValue(_variant_t(_T("�����")),FALSE);	
	pRow1->GetItem(1)->SetValue(_variant_t(m_csFAInfo.strSLH),FALSE);
	pRow1->GetItem(4)->SetValue(_variant_t(_T("��������")),FALSE);
	pRow1->GetItem(5)->SetValue(_variant_t(m_csFAInfo.strFALX),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow1,FALSE);
	//�ϲ���Ԫ��
	CBCGPGridRange Rang11(1,0,3,0);
	m_pwndFAInfoGrid->MergeRange(Rang11,FALSE);
	CBCGPGridRange Rang12(5,0,7,0);
	m_pwndFAInfoGrid->MergeRange(Rang12,FALSE);

	//�ڶ���
	CBCGPGridRow *pRow2 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow2->GetItem(0)->SetValue(_variant_t(_T("��׼����")),FALSE);
	pRow2->GetItem(1)->SetValue(_variant_t(m_csFAInfo.strSPJG),FALSE);
	pRow2->GetItem(4)->SetValue(_variant_t(_T("��׼ʱ��")),FALSE);
	pRow2->GetItem(5)->SetValue(_variant_t(m_csFAInfo.strSPSJ),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow2,FALSE);
	//�ϲ���Ԫ��
	CBCGPGridRange Rang21(1,1,3,1);
	m_pwndFAInfoGrid->MergeRange(Rang21,FALSE);
	CBCGPGridRange Rang22(5,1,7,1);
	m_pwndFAInfoGrid->MergeRange(Rang22,FALSE);

	//������
	CBCGPGridRow *pRow3 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow3->GetItem(0)->SetValue(_variant_t(_T("��������")),FALSE);
	pRow3->GetItem(1)->SetValue(_variant_t(m_csFAInfo.strFAMC),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow3,FALSE);
	//�ϲ���Ԫ��
	CBCGPGridRange Rang31(1,2,7,2);
	m_pwndFAInfoGrid->MergeRange(Rang31,FALSE);

	//������
	CBCGPGridRow *pRow4 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow4->GetItem(0)->SetValue(_variant_t(_T("�漰�滮����")),FALSE);
	pRow4->GetItem(1)->SetValue(_variant_t(m_csFAInfo.strGHMC),FALSE);
	//pRow4->GetItem(1)->SetValue(_variant_t(_T("���������ĳ���������������滮��2006-2020��")),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow4,FALSE);
	//�ϲ���Ԫ��
	CBCGPGridRange Rang41(1,3,7,3);
	m_pwndFAInfoGrid->MergeRange(Rang41,FALSE);

	//������
	CBCGPGridRow *pRow5 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow5->GetItem(0)->SetValue(_variant_t(_T("�漰��Ŀ����")),FALSE);
	pRow5->GetItem(1)->SetValue(_variant_t(m_csFAInfo.strSJXMMC),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow5,FALSE);
	//�ϲ���Ԫ��
	CBCGPGridRange Rang51(1,4,7,4);
	m_pwndFAInfoGrid->MergeRange(Rang51,FALSE);

	//������
	CString strCountTmp;
	CBCGPGridRow *pRow6 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow6->GetItem(0)->SetValue(_variant_t(_T("�漰��Ŀ����")),FALSE);
	strCountTmp.Format(_T("%d"),m_csFAInfo.nXMCount);
	pRow6->GetItem(1)->SetValue(_variant_t(strCountTmp),FALSE);
	pRow6->GetItem(4)->SetValue(_variant_t(_T("��Ŀ�����")),FALSE);
	strTmpMJ.Format(_T("%.4lf"),m_csFAInfo.dbZMJ);
	pRow6->GetItem(5)->SetValue(_variant_t(strTmpMJ),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow6,FALSE);
	//�ϲ���Ԫ��
	CBCGPGridRange Rang61(1,5,3,5);
	m_pwndFAInfoGrid->MergeRange(Rang61,FALSE);
	CBCGPGridRange Rang62(5,5,7,5);
	m_pwndFAInfoGrid->MergeRange(Rang62,FALSE);

	//������
	CString strTmpSJXZMC = _T("");
	POSITION pItem = m_csFAInfo.m_mapstrSJXZMC.GetStartPosition();
	while(pItem != NULL)
	{
		CString strName;
		CString strCode;
		m_csFAInfo.m_mapstrSJXZMC.GetNextAssoc(pItem,strCode,strName);
		strTmpSJXZMC += strName + _T("��");
	}
	//ȥ�����һ������
	strTmpSJXZMC = strTmpSJXZMC.Left(strTmpSJXZMC.GetLength() - 1);
	CBCGPGridRow *pRow7 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow7->GetItem(0)->SetValue(_variant_t(_T("�漰��������")),FALSE);
	pRow7->GetItem(1)->SetValue(_variant_t(strTmpSJXZMC),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow7,FALSE);
	//�ϲ���Ԫ��
	CBCGPGridRange Rang71(1,6,7,6);
	m_pwndFAInfoGrid->MergeRange(Rang71,FALSE);

	//�ڰ���
	CBCGPGridRow *pRow8 = m_pwndFAInfoGrid->CreateMultiLineRow(2);
	pRow8->GetItem(0)->SetValue(_variant_t(_T("�漰�������")),FALSE);
	strCountTmp.Format(_T("%d"),m_csFAInfo.nXZCount);
	pRow8->GetItem(1)->SetValue(_variant_t(strCountTmp),FALSE);
	pRow8->GetItem(4)->SetValue(_variant_t(_T("�滮�޸ģ���ʵ��\r\n�漰�ؿ����")),FALSE);
	strCountTmp.Format(_T("%d"),m_csFAInfo.nDKCount);
	pRow8->GetItem(5)->SetValue(_variant_t(strCountTmp),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow8,FALSE);
	//�ϲ���Ԫ��
	CBCGPGridRange Rang81(1,7,3,7);
	m_pwndFAInfoGrid->MergeRange(Rang81,FALSE);
	CBCGPGridRange Rang82(5,7,7,7);
	m_pwndFAInfoGrid->MergeRange(Rang82,FALSE);

	//������֣���9,10,11��
	//��9��
	CBCGPGridRow *pRow9 = m_pwndFAInfoGrid->CreateMultiLineRow(1);
	pRow9->GetItem(0)->SetValue(_variant_t(_T("�ؿ�滮��;\r\n���޸�/��ʵǰ��")),FALSE);
	pRow9->GetItem(0)->SetMultiline(TRUE);
	pRow9->GetItem(1)->SetValue(_variant_t(_T("�����")),FALSE);
	pRow9->GetItem(2)->SetValue(_variant_t(_T("ũ�õ�")),FALSE);
	pRow9->GetItem(4)->SetValue(_variant_t(_T("�����õ�")),FALSE);
	pRow9->GetItem(5)->SetValue(_variant_t(_T("δ���õ�")),FALSE);
	pRow9->GetItem(6)->SetValue(_variant_t(_T("����ũ��")),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow9,FALSE);
	//��10��
	CBCGPGridRow *pRow10 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow10->GetItem(2)->SetValue(_variant_t(_T("С��")),FALSE);
	pRow10->GetItem(3)->SetValue(_variant_t(_T("���и���")),FALSE);
	pRow10->GetItem(6)->SetValue(_variant_t(_T("С��")),FALSE);
	pRow10->GetItem(7)->SetValue(_variant_t(_T("���и���")),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow10,FALSE);
	//��11��,����double����ֱ�Ӹ�ֵgridctrl���Զ�������λ��������Ҫ��һ��ת��
	CBCGPGridRow *pRow11 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	strTmpMJ.Format(_T("%.4lf"),m_csFAInfo.dbZMJ);
	pRow11->GetItem(1)->SetValue(_variant_t(strTmpMJ),FALSE);
	strTmpMJ.Format(_T("%.4lf"),m_csFAInfo.dbNYDMJ);
	pRow11->GetItem(2)->SetValue(_variant_t(strTmpMJ),FALSE);
	strTmpMJ.Format(_T("%.4lf"),m_csFAInfo.dbGDJMJ);
	pRow11->GetItem(3)->SetValue(_variant_t(strTmpMJ),FALSE);
	strTmpMJ.Format(_T("%.4lf"),m_csFAInfo.dbJSYDMJ);
	pRow11->GetItem(4)->SetValue(_variant_t(strTmpMJ),FALSE);
	strTmpMJ.Format(_T("%.4lf"),m_csFAInfo.dbWLYDMJ);
	pRow11->GetItem(5)->SetValue(_variant_t(strTmpMJ),FALSE);
	strTmpMJ.Format(_T("%.4lf"),m_csFAInfo.dbJBNTJMJ);
	pRow11->GetItem(6)->SetValue(_variant_t(strTmpMJ),FALSE);
	strTmpMJ.Format(_T("%.4lf"),m_csFAInfo.dbJBNTJMJ);
	pRow11->GetItem(7)->SetValue(_variant_t(strTmpMJ),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow11,FALSE);
	//�ϲ���Ԫ��
	//�ؿ�滮��;���޸�/��ʵǰ��
	CBCGPGridRange Rang91(0,8,0,10);
	m_pwndFAInfoGrid->MergeRange(Rang91,FALSE);
	//�����
	CBCGPGridRange Rang92(1,8,1,9);
	m_pwndFAInfoGrid->MergeRange(Rang92,FALSE);
	//ũ�õ�
	CBCGPGridRange Rang93(2,8,3,8);
	m_pwndFAInfoGrid->MergeRange(Rang93,FALSE);
	//�����õ�
	CBCGPGridRange Rang94(4,8,4,9);
	m_pwndFAInfoGrid->MergeRange(Rang94,FALSE);
	//δ���õ�
	CBCGPGridRange Rang95(5,8,5,9);
	m_pwndFAInfoGrid->MergeRange(Rang95,FALSE);
	//����ũ��
	CBCGPGridRange Rang96(6,8,7,8);
	m_pwndFAInfoGrid->MergeRange(Rang96,FALSE);
	//ָ�겿�֣���12,13,14��
	//��12��
	CBCGPGridRow *pRow12 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow12->GetItem(0)->SetValue(_variant_t(_T("ָ��ʹ�����")),FALSE);
	pRow12->GetItem(1)->SetValue(_variant_t(_T("���")),FALSE);
	pRow12->GetItem(2)->SetValue(_variant_t(_T("ʡ��")),FALSE);
	pRow12->GetItem(3)->SetValue(_variant_t(_T("�м�")),FALSE);
	pRow12->GetItem(4)->SetValue(_variant_t(_T("�ؼ�")),FALSE);
	pRow12->GetItem(5)->SetValue(_variant_t(_T("�缶")),FALSE);
	pRow12->GetItem(6)->SetValue(_variant_t(_T("�ϼ�")),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow12,FALSE);
	//����ָ��չʾ����ʱ����
	CString strZBTmp;
	double dbZBSumTmp = 0.0;
	//��13��
	CBCGPGridRow *pRow13 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow13->GetItem(1)->SetValue(_variant_t(_T("���������õ�")),FALSE);
	strZBTmp.Format(_T("%.4lf"),m_csFAInfo.dbJSYDZBsheng);
	dbZBSumTmp += m_csFAInfo.dbJSYDZBsheng;
	pRow13->GetItem(2)->SetValue(_variant_t(strZBTmp),FALSE);
	strZBTmp.Format(_T("%.4lf"),m_csFAInfo.dbJSYDZBshi);
	dbZBSumTmp += m_csFAInfo.dbJSYDZBshi;
	pRow13->GetItem(3)->SetValue(_variant_t(strZBTmp),FALSE);
	strZBTmp.Format(_T("%.4lf"),m_csFAInfo.dbJSYDZBxian);
	dbZBSumTmp += m_csFAInfo.dbJSYDZBxian;
	pRow13->GetItem(4)->SetValue(_variant_t(strZBTmp),FALSE);
	strZBTmp.Format(_T("%.4lf"),m_csFAInfo.dbJSYDZBxiang);
	dbZBSumTmp += m_csFAInfo.dbJSYDZBxiang;
	pRow13->GetItem(5)->SetValue(_variant_t(strZBTmp),FALSE);
	strZBTmp.Format(_T("%.4lf"),dbZBSumTmp);
	pRow13->GetItem(6)->SetValue(_variant_t(strZBTmp),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow13,FALSE);
	//��14��
	dbZBSumTmp = 0.0;
	CBCGPGridRow *pRow14 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow14->GetItem(1)->SetValue(_variant_t(_T("����ũ��")),FALSE);
	strZBTmp.Format(_T("%.4lf"),m_csFAInfo.dbJBNTZBsheng);
	dbZBSumTmp += m_csFAInfo.dbJBNTZBsheng;
	pRow14->GetItem(2)->SetValue(_variant_t(strZBTmp),FALSE);
	strZBTmp.Format(_T("%.4lf"),m_csFAInfo.dbJBNTZBshi);
	dbZBSumTmp += m_csFAInfo.dbJBNTZBshi;
	pRow14->GetItem(3)->SetValue(_variant_t(strZBTmp),FALSE);
	strZBTmp.Format(_T("%.4lf"),m_csFAInfo.dbJBNTZBxian);
	dbZBSumTmp += m_csFAInfo.dbJBNTZBxian;
	pRow14->GetItem(4)->SetValue(_variant_t(strZBTmp),FALSE);
	strZBTmp.Format(_T("%.4lf"),m_csFAInfo.dbJBNTZBxiang);
	dbZBSumTmp += m_csFAInfo.dbJBNTZBxiang;
	pRow14->GetItem(5)->SetValue(_variant_t(strZBTmp),FALSE);
	strZBTmp.Format(_T("%.4lf"),dbZBSumTmp);
	pRow14->GetItem(6)->SetValue(_variant_t(strZBTmp),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow14,FALSE);
	//�ϲ���Ԫ��
	//ָ��ʹ�����
	CBCGPGridRange Rang121(0,11,0,13);
	m_pwndFAInfoGrid->MergeRange(Rang121,FALSE);
	//�ϼ�
	CBCGPGridRange Rang122(6,11,7,11);
	m_pwndFAInfoGrid->MergeRange(Rang122,FALSE);
	//���������õغϼ�
	CBCGPGridRange Rang131(6,12,7,12);
	m_pwndFAInfoGrid->MergeRange(Rang131,FALSE);
	//����ũ��ϼ�
	CBCGPGridRange Rang141(6,13,7,13);
	m_pwndFAInfoGrid->MergeRange(Rang141,FALSE);
	//��15��
	CBCGPGridRow *pRow15 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow15->GetItem(0)->SetValue(_variant_t(_T("����״̬")),FALSE);
	pRow15->GetItem(1)->SetValue(_variant_t(m_csFAInfo.strGXZT),FALSE);
	pRow15->GetItem(4)->SetValue(_variant_t(_T("����ʱ��")),FALSE);
	pRow15->GetItem(5)->SetValue(_variant_t(m_csFAInfo.strGXSJ),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow15,FALSE);
	//�ϲ���Ԫ��
	CBCGPGridRange Rang151(1,14,3,14);
	m_pwndFAInfoGrid->MergeRange(Rang151,FALSE);
	CBCGPGridRange Rang152(5,14,7,14);
	m_pwndFAInfoGrid->MergeRange(Rang152,FALSE);

	//ͳһˢ��
	m_pwndFAInfoGrid->AdjustLayout();
	return S_OK;
}

