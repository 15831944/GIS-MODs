// LPGHGXCXRstDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LPGHGXCXRstDlg.h"


// CLPGHGXCXRstDlg 对话框

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


// CLPGHGXCXRstDlg 消息处理程序

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
	// TODO: 在此处添加消息处理程序代码
}

BOOL CLPGHGXCXRstDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
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
		pTabwnd->Create(NULL,_T("方案信息"),WS_CHILD | WS_VISIBLE | ES_CENTER, rectTabArea, pParent, IDD_GRIDDOCKTABWND,NULL);
		m_pwndFAInfoGrid->EnableMarkSortedColumn (FALSE);//设置表背景初始化为无
		m_pwndFAInfoGrid->EnableHeader(FALSE);//隐藏列头
		m_pwndFAInfoGrid->SetReadOnly(TRUE);//只读
		CBCGPGridColors GridTheme;
		GridTheme = m_pwndFAInfoGrid->GetColorTheme();
		GridTheme.m_clrHorzLine = RGB (0, 0, 0);//设置水平网格线颜色
		GridTheme.m_clrVertLine = RGB (0, 0, 0);//设置垂直网格线颜色
		CBCGPGridColors::ColorData SelColors;
		SelColors.m_clrBackground = RGB (168,205,241);
		SelColors.m_clrBorder = RGB (168,205,241);
		SelColors.m_clrGradient = -1;
		SelColors.m_clrText = RGB (0, 0, 0);
		SelColors.m_nGradientAngle = 0;
		GridTheme.m_SelColors = SelColors;//设置选中颜色
		m_pwndFAInfoGrid->SetColorTheme(GridTheme);
		for (int i = 0;i < 8; i++)
		{
			m_pwndFAInfoGrid->InsertColumn(i,_T("culonm"),130);
			m_pwndFAInfoGrid->SetColumnAlign(i,HDF_CENTER);
		}
		m_pwndResTabWnd->AddTab(m_pwndFAInfoGrid,_T("方案信息"));
	}
	InitMaps();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
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
	//方案类型节点
	TiXmlElement *FALXDictElement = NULL;
	//更新状态节点
	TiXmlElement *GXZTDictElement = NULL;
	while(DictElement != NULL)
	{//循环遍历所有字典表名，找到对应的表
		CString DictIDTmp;
		utf8ToUnicode(DictElement->FirstAttribute()->Value(),&DictIDTmp);
		if (DictIDTmp.Compare(_T("GXZT")) == 0) GXZTDictElement = DictElement;
		if (DictIDTmp.Compare(_T("GXFALX")) == 0) FALXDictElement = DictElement;
		if (GXZTDictElement!= NULL && FALXDictElement != NULL) break;
		DictElement = DictElement->NextSiblingElement();
	}
	//更新状态map
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
	//方案类型map
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
	//edit by lcc 2016.12.06 更改获取xml路径的方式
	TCHAR exeFullPath[MAX_PATH]; 
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	CString strExePath=exeFullPath;
	int index =strExePath.ReverseFind('\\');
	CString strPath = strExePath.Left(index)+_T("\\data\\config.xml");
	//edit by lcc 2016.12.06 end
	if (XmlDoc.LoadFile(W2A(_bstr_t(strPath))) == false)
	{
		//MessageBox(_T("无法打开配置文件"),_T("错误"),MB_OK);
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
		BCGPMessageBox(_T("数据库连接失败"),_T("错误"),MB_OK);
		return E_FAIL;
	}
	hr = pRst->Open(_variant_t(strQrueyQueryFilter),_variant_t(ipADOConn.GetInterfacePtr()), adOpenStatic,adLockOptimistic,adCmdText);
	//获取所有信息
	long nCount = 0;
	pRst->get_RecordCount(&nCount);
	hr = pRst->GetRows(nCount,vtMissing,vtMissing,&vt);
	if (!SUCCEEDED(hr)) return E_FAIL;
	//获取数据指针和列数
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
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_wndFARstList.GetCurSel();
	if (nIndex != m_nFASelectIndex)
	{
		CString *strSelText = (CString*)m_wndFARstList.GetItemData(nIndex);
		HRESULT hr;
		_ConnectionPtr ipADOConn;
		hr = ConnToOracle(ipADOConn);
		if (hr != S_OK)
		{
			BCGPMessageBox(_T("数据库连接失败"),_T("错误"),MB_OK);
			return;
		}
		//获取当前方案信息
		GetFAInfoByUID(ipADOConn,*strSelText);
		//显示方案信息
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
	//获取所有信息
	long nCount = 0;
	pRst->get_RecordCount(&nCount);
	hr = pRst->GetRows(nCount,vtMissing,vtMissing,&vt);
	if (!SUCCEEDED(hr)) return E_FAIL;
	//获取数据指针和列数
	void *pvd = NULL;
	SafeArrayAccessData(vt.parray,&pvd);
	LONG nFieldNum = 0;
	SafeArrayGetUBound(vt.parray,(UINT)1,&nFieldNum);
	//清空上一个方案遗留的信息
	m_csFAInfo.ClearInfo();
	//地块数与结果条数应当保持严格一直
	m_csFAInfo.nDKCount = nCount;
	//需要的临时变量
	//方案类型代码
	CString strFALXDM;
	//方案更新专题代码
	CString strFAGXZTDM;
	//项目标识码
	CString strXMBSM = _T("");
	//是否新项目行
	BOOL bIsNewXMLine = TRUE;
	//用于读取一行项目的临时变量
	CXMInfoClass *XMInfo;
	//实例化一个行政区树接口，用于获取行政区字典表，将乡镇代码翻译为乡镇名称
	CComQIPtr<ILPXZQDataTree> spXZQTree;
	spXZQTree.CoCreateInstance(CLSID_LPXZQDataTree);
	IUnknown *pMapUnknow;
	spXZQTree->GetXZQMap(&pMapUnknow);
	CMapStringToString *pXZQMap = (CMapStringToString *)pMapUnknow;

	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		//预读项目标识码判断是不是新的项目
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
		//每一行肯定都是一个新的地块
		CDKInfoClass *pDKInfo = new CDKInfoClass;
		XMInfo->m_arrDKInfo.Add(pDKInfo);
		for (LONG nField = 0; nField <= nFieldNum; nField++)
		{	
			VARIANT vtTmp = ((VARIANT*)pvd)[nField+(nIndex)*(nFieldNum + 1)];
			//方案属性只需要读一次就够了
			if (nIndex == 0)
			{
				switch (nField)
				{
				case 0://方案标识码
					m_csFAInfo.strFABSM = VariantToString(vtTmp);
					break;
				case 1://方案名称
					m_csFAInfo.strFAMC = VariantToString(vtTmp);
					break;
				case 2://受理号
					m_csFAInfo.strSLH = VariantToString(vtTmp);
					break;
				case 3://方案类型
					strFALXDM = VariantToString(vtTmp);
					strFALXDM = strFALXDM.Left(strFALXDM.Find(_T(".")));
					m_strmapFALX.Lookup(strFALXDM,m_csFAInfo.strFALX);
					break;
				case 4://县代码
					m_csFAInfo.strXDM = VariantToString(vtTmp);
					break;
				case 5://县名称
					m_csFAInfo.strXMC = VariantToString(vtTmp);
					break;
				case 6://方案面积	
					m_csFAInfo.dbZMJ = (double)(_variant_t)vtTmp;
					break;
				case 7://农用地面积
					m_csFAInfo.dbNYDMJ = (double)(_variant_t)vtTmp;
					break;
				case 8://耕地净面积
					m_csFAInfo.dbGDJMJ = (double)(_variant_t)vtTmp;
					break;
				case 9://建设用地面积
					m_csFAInfo.dbJSYDMJ = (double)(_variant_t)vtTmp;
					break;
				case 10://未利用地面积
					m_csFAInfo.dbWLYDMJ = (double)(_variant_t)vtTmp;
					break;
				case 11://基本农田面积
					m_csFAInfo.dbJBNTJMJ = (double)(_variant_t)vtTmp;
					break;
				case 12://新增建设用地省级指标
					m_csFAInfo.dbJSYDZBsheng = (double)(_variant_t)vtTmp;
					break;
				case 13://新增建设用地市级指标
					m_csFAInfo.dbJSYDZBshi = (double)(_variant_t)vtTmp;
					break;
				case 14://新增建设用地县级指标
					m_csFAInfo.dbJSYDZBxian = (double)(_variant_t)vtTmp;
					break;
				case 15://新增建设用地乡级指标
					m_csFAInfo.dbJSYDZBxiang = (double)(_variant_t)vtTmp;
					break;
				case 16://基本农田省级指标
					m_csFAInfo.dbJBNTZBsheng = (double)(_variant_t)vtTmp;
					break;
				case 17://基本农田市级指标
					m_csFAInfo.dbJBNTZBshi = (double)(_variant_t)vtTmp;
					break;
				case 18://基本农田县级指标
					m_csFAInfo.dbJBNTZBxian = (double)(_variant_t)vtTmp;
					break;
				case 19://基本农田乡级指标
					m_csFAInfo.dbJBNTZBxiang = (double)(_variant_t)vtTmp;
					break;
				case 20://更新时间
					m_csFAInfo.strGXSJ = VariantToString(vtTmp);
					break;
				case 21://更新状态
					strFAGXZTDM = VariantToString(vtTmp);
					strFAGXZTDM = strFAGXZTDM.Left(strFAGXZTDM.Find(_T(".")));
					m_strmapGXZT.Lookup(strFAGXZTDM,m_csFAInfo.strGXZT);
					break;
				case 22://批准机关
					m_csFAInfo.strSPJG = VariantToString(vtTmp);
					break;
				case 23://批准时间
					m_csFAInfo.strSPSJ = VariantToString(vtTmp);
					break;
				case 24://项目总面积
					m_csFAInfo.dbXMZMJ = (double)(_variant_t)vtTmp;
					break;
				default:
					break;
				}
			}
			//解析项目相关信息
			//读到项目标识码不同时解析项目信息
			if (bIsNewXMLine == TRUE)
			{
				switch(nField)
				{
				case 25://项目标识码
					XMInfo->strXMBSM = VariantToString(vtTmp);
					break;
				case 26://项目名称
					XMInfo->strXMMC = VariantToString(vtTmp);
					break;
				case 27://项目类型，这个需要翻译
					XMInfo->strXMLX = VariantToString(vtTmp);
					break;
				case 28://项目总面积，这个面积的合计应当与方案表中的项目总面积相同
					XMInfo->dbZMJ = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumZMJ += XMInfo->dbZMJ;
					break;
				case 29://农用地面积
					XMInfo->dbNYDMJ = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumNYDMJ += XMInfo->dbNYDMJ;
					break;
				case 30://耕地净面积
					XMInfo->dbGDJMJ = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumGDJMJ += XMInfo->dbGDJMJ;
					break;
				case 31://建设用地面积
					XMInfo->dbJSYDMJ = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumJSYDMJ += XMInfo->dbJSYDMJ;
					break;
				case 32://未利用地面积
					XMInfo->dbWLYDMJ = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumWLYDMJ += XMInfo->dbWLYDMJ;
					break;
				case 33://基本农田面积
					XMInfo->dbJBNTJMJ = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumJBNTJMJ += XMInfo->dbJBNTJMJ;
					break;
				case 34://建设用地省级指标
					XMInfo->dbJSYDZBsheng = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumXZJSYD1 += XMInfo->dbJSYDZBsheng;
					break;
				case 35://建设用地市级指标
					XMInfo->dbJSYDZBshi = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumXZJSYD2 += XMInfo->dbJSYDZBshi;
					break;
				case 36://建设用地县级指标
					XMInfo->dbJSYDZBxian = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumXZJSYD3 += XMInfo->dbJSYDZBxian;
					break;
				case 37://建设用地乡级指标
					XMInfo->dbJSYDZBxiang = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumXZJSYD4 += XMInfo->dbJSYDZBxiang;
					break;
				case 38://基本农田省级指标
					XMInfo->dbJBNTZBsheng = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumJBNT1 += XMInfo->dbJBNTZBsheng;
					break;
				case 39://基本农田市级指标
					XMInfo->dbJBNTZBshi = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumJBNT2 += XMInfo->dbJBNTZBshi;
					break;
				case 40://基本农田县级指标
					XMInfo->dbJBNTZBxian = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumJBNT3 += XMInfo->dbJBNTZBxian;
					break;
				case 41://基本农田乡级指标
					XMInfo->dbJBNTZBxiang = (double)(_variant_t)vtTmp;
					m_csFAInfo.m_XMSumInfo.dbSumJBNT4 += XMInfo->dbJBNTZBxiang;
					break;
				}
			}
			//解析地块信息
			switch(nField)
			{
				case 42://地块标识码
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
	//接下来是解析方案中与项目表相关的内容并写入方案类
	//项目个数与项目数组大小相同
	m_csFAInfo.nXMCount = m_csFAInfo.m_arrXMInfo.GetCount();
	//涉及乡镇个数与乡镇数组大小相同
	m_csFAInfo.nXZCount = m_csFAInfo.m_mapstrSJXZMC.GetCount();
	//涉及项目名称，由项目数组第一个项目的名称和项目个数拼接
	m_csFAInfo.strSJXMMC.Format(_T("%s等%d个建设项目"),(m_csFAInfo.m_arrXMInfo.GetAt(0))->strXMMC,m_csFAInfo.nXMCount);
	//涉及规划名称 县名称 + 乡镇名称（不止一个乡镇就是中心城区） + 土地利用总体规划（2006-2020）
	if (m_csFAInfo.m_mapstrSJXZMC.GetCount() > 1)
	{
		m_csFAInfo.strGHMC.Format(_T("%s中心城区土地利用总体规划（2006-2020）"),m_csFAInfo.strXMC);
	}else
	{
		m_csFAInfo.strGHMC.Format(_T("%s%s土地利用总体规划（2006-2020）"),m_csFAInfo.strXMC,((m_csFAInfo.m_arrXMInfo.GetAt(0))->m_arrDKInfo.GetAt(0))->strXZMC);
	}
	return S_OK;
}

//因为三表连查，查询语句比较长，专门用一个函数来组织
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
	//一行写五个，方便解析的时候做对照
	//应当用地块UID作唯一约束条件
	
	//0-4方案表信息:方案标识码，方案名称，受理号，方案类型，县代码
	strSelect += _T(" f.bsm as xmbsm , f.famc, f.slh, f.falx, f.xdm");
	//5-9方案表信息:，县名称，面积，农用地面积，耕地净面积，建设用地面积
	strSelect += _T(", f.xmc, f.mj as famj, f.nydmj as fanydmj, f.gdjmj as fagdjmj, f.jsydmj as fajsydmj");
	//10-14方案表信息：未利用地面积，基本农田净面积，新增建设用地省级指标，新增建设用地市级指标，新增建设用地县级指标
	strSelect += _T(", f.wlydmj as fawlydmj, f.jbntjmj as fajbntjmj, f.xzjsyd1 as faxzjsyd1, f.xzjsyd2 as faxzjsyd2, f.xzjsyd3 as faxzjsyd3");
	//15-19方案表信息：新增建设用地乡级指标，基本农田省级指标，基本农田市级指标，基本农田县级指标，基本农田乡级指标
	strSelect += _T(", f.xzjsyd4 as faxzjsyd, f.jbnt1 as fajbnt1, f.jbnt2 as fajbnt2, f.jbnt3 as fajbnt3, f.jbnt4 as fajbnt4");
	//20-24方案表信息：更新时间，更新状态,批准机关，批准时间,项目总面积
	//strSelect += _T(", f.gxsj, f.gxzt, f.pzjg, f.pzsj, f.xmzmj");
	strSelect += _T(", f.gxsj, f.gxzt, f.pzjg, f.pzsj, f.mj as xmzmj");

	//25-29项目表信息：项目标识码，项目名称，项目类型，总面积,农用地面积
	strSelect += _T(", x.bsm as xmbsm, x.xmmc, x.xmlx, x.mj as xmzmj, x.nydmj as xmnydmj");
	//30-34项目表信息：耕地净面积，建设用地面积，未利用地净面积，基本农田净面积，新增建设用地省级指标
	strSelect += _T(", x.gdjmj as xmgdjmj, x.jsydmj as xmjsydmj, x.wlydmj as xmwlydmj, x.jbntjmj as xmjbntjmj, x.xzjsyd1 as xmxzjsyd1");
	//35-39项目表信息：新增建设用地市级指标，新增建设用地县级指标，新增建设用地乡级指标，基本农田省级指标，基本农田市级指标
	strSelect += _T(", x.xzjsyd2 as xmxzjsyd2, x.xzjsyd3 as xmxzjsyd3, x.xzjsyd4 as xmxzjsyd, x.jbnt1 as xmjbnt1, x.jbnt2 as xmjbnt2");
	//40-41项目表信息：基本农田县级指标，基本农田乡级指标
	strSelect += _T(", x.jbnt3 as xmjbnt3, x.jbnt4 as xmjbnt4");

	//42-44地块表信息：标识码，乡镇代码，行政区划代码
	strSelect += _T(", d.bsm as dkbsm, d.xzdm, d.xzqdm as cundm");
	//45-49地块表信息：行政区划名称，原规划地类代码，原规划地类名称，原用途地块编号，原规划用途代码
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
	//第一行
	pRow1->GetItem(0)->SetValue(_variant_t(_T("受理号")),FALSE);	
	pRow1->GetItem(1)->SetValue(_variant_t(m_csFAInfo.strSLH),FALSE);
	pRow1->GetItem(4)->SetValue(_variant_t(_T("方案类型")),FALSE);
	pRow1->GetItem(5)->SetValue(_variant_t(m_csFAInfo.strFALX),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow1,FALSE);
	//合并单元格
	CBCGPGridRange Rang11(1,0,3,0);
	m_pwndFAInfoGrid->MergeRange(Rang11,FALSE);
	CBCGPGridRange Rang12(5,0,7,0);
	m_pwndFAInfoGrid->MergeRange(Rang12,FALSE);

	//第二行
	CBCGPGridRow *pRow2 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow2->GetItem(0)->SetValue(_variant_t(_T("批准机关")),FALSE);
	pRow2->GetItem(1)->SetValue(_variant_t(m_csFAInfo.strSPJG),FALSE);
	pRow2->GetItem(4)->SetValue(_variant_t(_T("批准时间")),FALSE);
	pRow2->GetItem(5)->SetValue(_variant_t(m_csFAInfo.strSPSJ),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow2,FALSE);
	//合并单元格
	CBCGPGridRange Rang21(1,1,3,1);
	m_pwndFAInfoGrid->MergeRange(Rang21,FALSE);
	CBCGPGridRange Rang22(5,1,7,1);
	m_pwndFAInfoGrid->MergeRange(Rang22,FALSE);

	//第三行
	CBCGPGridRow *pRow3 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow3->GetItem(0)->SetValue(_variant_t(_T("方案名称")),FALSE);
	pRow3->GetItem(1)->SetValue(_variant_t(m_csFAInfo.strFAMC),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow3,FALSE);
	//合并单元格
	CBCGPGridRange Rang31(1,2,7,2);
	m_pwndFAInfoGrid->MergeRange(Rang31,FALSE);

	//第四行
	CBCGPGridRow *pRow4 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow4->GetItem(0)->SetValue(_variant_t(_T("涉及规划名称")),FALSE);
	pRow4->GetItem(1)->SetValue(_variant_t(m_csFAInfo.strGHMC),FALSE);
	//pRow4->GetItem(1)->SetValue(_variant_t(_T("武义县中心城区土地利用总体规划（2006-2020）")),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow4,FALSE);
	//合并单元格
	CBCGPGridRange Rang41(1,3,7,3);
	m_pwndFAInfoGrid->MergeRange(Rang41,FALSE);

	//第五行
	CBCGPGridRow *pRow5 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow5->GetItem(0)->SetValue(_variant_t(_T("涉及项目名称")),FALSE);
	pRow5->GetItem(1)->SetValue(_variant_t(m_csFAInfo.strSJXMMC),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow5,FALSE);
	//合并单元格
	CBCGPGridRange Rang51(1,4,7,4);
	m_pwndFAInfoGrid->MergeRange(Rang51,FALSE);

	//第六行
	CString strCountTmp;
	CBCGPGridRow *pRow6 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow6->GetItem(0)->SetValue(_variant_t(_T("涉及项目个数")),FALSE);
	strCountTmp.Format(_T("%d"),m_csFAInfo.nXMCount);
	pRow6->GetItem(1)->SetValue(_variant_t(strCountTmp),FALSE);
	pRow6->GetItem(4)->SetValue(_variant_t(_T("项目总面积")),FALSE);
	strTmpMJ.Format(_T("%.4lf"),m_csFAInfo.dbZMJ);
	pRow6->GetItem(5)->SetValue(_variant_t(strTmpMJ),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow6,FALSE);
	//合并单元格
	CBCGPGridRange Rang61(1,5,3,5);
	m_pwndFAInfoGrid->MergeRange(Rang61,FALSE);
	CBCGPGridRange Rang62(5,5,7,5);
	m_pwndFAInfoGrid->MergeRange(Rang62,FALSE);

	//第七行
	CString strTmpSJXZMC = _T("");
	POSITION pItem = m_csFAInfo.m_mapstrSJXZMC.GetStartPosition();
	while(pItem != NULL)
	{
		CString strName;
		CString strCode;
		m_csFAInfo.m_mapstrSJXZMC.GetNextAssoc(pItem,strCode,strName);
		strTmpSJXZMC += strName + _T("，");
	}
	//去掉最后一个逗号
	strTmpSJXZMC = strTmpSJXZMC.Left(strTmpSJXZMC.GetLength() - 1);
	CBCGPGridRow *pRow7 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow7->GetItem(0)->SetValue(_variant_t(_T("涉及乡镇名称")),FALSE);
	pRow7->GetItem(1)->SetValue(_variant_t(strTmpSJXZMC),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow7,FALSE);
	//合并单元格
	CBCGPGridRange Rang71(1,6,7,6);
	m_pwndFAInfoGrid->MergeRange(Rang71,FALSE);

	//第八行
	CBCGPGridRow *pRow8 = m_pwndFAInfoGrid->CreateMultiLineRow(2);
	pRow8->GetItem(0)->SetValue(_variant_t(_T("涉及乡镇个数")),FALSE);
	strCountTmp.Format(_T("%d"),m_csFAInfo.nXZCount);
	pRow8->GetItem(1)->SetValue(_variant_t(strCountTmp),FALSE);
	pRow8->GetItem(4)->SetValue(_variant_t(_T("规划修改（落实）\r\n涉及地块个数")),FALSE);
	strCountTmp.Format(_T("%d"),m_csFAInfo.nDKCount);
	pRow8->GetItem(5)->SetValue(_variant_t(strCountTmp),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow8,FALSE);
	//合并单元格
	CBCGPGridRange Rang81(1,7,3,7);
	m_pwndFAInfoGrid->MergeRange(Rang81,FALSE);
	CBCGPGridRange Rang82(5,7,7,7);
	m_pwndFAInfoGrid->MergeRange(Rang82,FALSE);

	//面积部分，第9,10,11行
	//第9行
	CBCGPGridRow *pRow9 = m_pwndFAInfoGrid->CreateMultiLineRow(1);
	pRow9->GetItem(0)->SetValue(_variant_t(_T("地块规划用途\r\n（修改/落实前）")),FALSE);
	pRow9->GetItem(0)->SetMultiline(TRUE);
	pRow9->GetItem(1)->SetValue(_variant_t(_T("总面积")),FALSE);
	pRow9->GetItem(2)->SetValue(_variant_t(_T("农用地")),FALSE);
	pRow9->GetItem(4)->SetValue(_variant_t(_T("建设用地")),FALSE);
	pRow9->GetItem(5)->SetValue(_variant_t(_T("未利用地")),FALSE);
	pRow9->GetItem(6)->SetValue(_variant_t(_T("基本农田")),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow9,FALSE);
	//第10行
	CBCGPGridRow *pRow10 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow10->GetItem(2)->SetValue(_variant_t(_T("小计")),FALSE);
	pRow10->GetItem(3)->SetValue(_variant_t(_T("其中耕地")),FALSE);
	pRow10->GetItem(6)->SetValue(_variant_t(_T("小计")),FALSE);
	pRow10->GetItem(7)->SetValue(_variant_t(_T("其中耕地")),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow10,FALSE);
	//第11行,由于double类型直接赋值gridctrl会自动保留两位，这里需要做一个转换
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
	//合并单元格
	//地块规划用途（修改/落实前）
	CBCGPGridRange Rang91(0,8,0,10);
	m_pwndFAInfoGrid->MergeRange(Rang91,FALSE);
	//总面积
	CBCGPGridRange Rang92(1,8,1,9);
	m_pwndFAInfoGrid->MergeRange(Rang92,FALSE);
	//农用地
	CBCGPGridRange Rang93(2,8,3,8);
	m_pwndFAInfoGrid->MergeRange(Rang93,FALSE);
	//建设用地
	CBCGPGridRange Rang94(4,8,4,9);
	m_pwndFAInfoGrid->MergeRange(Rang94,FALSE);
	//未利用地
	CBCGPGridRange Rang95(5,8,5,9);
	m_pwndFAInfoGrid->MergeRange(Rang95,FALSE);
	//基本农田
	CBCGPGridRange Rang96(6,8,7,8);
	m_pwndFAInfoGrid->MergeRange(Rang96,FALSE);
	//指标部分，第12,13,14行
	//第12行
	CBCGPGridRow *pRow12 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow12->GetItem(0)->SetValue(_variant_t(_T("指标使用情况")),FALSE);
	pRow12->GetItem(1)->SetValue(_variant_t(_T("类别")),FALSE);
	pRow12->GetItem(2)->SetValue(_variant_t(_T("省级")),FALSE);
	pRow12->GetItem(3)->SetValue(_variant_t(_T("市级")),FALSE);
	pRow12->GetItem(4)->SetValue(_variant_t(_T("县级")),FALSE);
	pRow12->GetItem(5)->SetValue(_variant_t(_T("乡级")),FALSE);
	pRow12->GetItem(6)->SetValue(_variant_t(_T("合计")),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow12,FALSE);
	//用于指标展示的临时变量
	CString strZBTmp;
	double dbZBSumTmp = 0.0;
	//第13行
	CBCGPGridRow *pRow13 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow13->GetItem(1)->SetValue(_variant_t(_T("新增建设用地")),FALSE);
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
	//第14行
	dbZBSumTmp = 0.0;
	CBCGPGridRow *pRow14 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow14->GetItem(1)->SetValue(_variant_t(_T("基本农田")),FALSE);
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
	//合并单元格
	//指标使用情况
	CBCGPGridRange Rang121(0,11,0,13);
	m_pwndFAInfoGrid->MergeRange(Rang121,FALSE);
	//合计
	CBCGPGridRange Rang122(6,11,7,11);
	m_pwndFAInfoGrid->MergeRange(Rang122,FALSE);
	//新增建设用地合计
	CBCGPGridRange Rang131(6,12,7,12);
	m_pwndFAInfoGrid->MergeRange(Rang131,FALSE);
	//基本农田合计
	CBCGPGridRange Rang141(6,13,7,13);
	m_pwndFAInfoGrid->MergeRange(Rang141,FALSE);
	//第15行
	CBCGPGridRow *pRow15 = m_pwndFAInfoGrid->CreateRow(m_pwndFAInfoGrid->GetColumnCount());
	pRow15->GetItem(0)->SetValue(_variant_t(_T("更新状态")),FALSE);
	pRow15->GetItem(1)->SetValue(_variant_t(m_csFAInfo.strGXZT),FALSE);
	pRow15->GetItem(4)->SetValue(_variant_t(_T("更新时间")),FALSE);
	pRow15->GetItem(5)->SetValue(_variant_t(m_csFAInfo.strGXSJ),FALSE);
	m_pwndFAInfoGrid->AddRow(pRow15,FALSE);
	//合并单元格
	CBCGPGridRange Rang151(1,14,3,14);
	m_pwndFAInfoGrid->MergeRange(Rang151,FALSE);
	CBCGPGridRange Rang152(5,14,7,14);
	m_pwndFAInfoGrid->MergeRange(Rang152,FALSE);

	//统一刷新
	m_pwndFAInfoGrid->AdjustLayout();
	return S_OK;
}

