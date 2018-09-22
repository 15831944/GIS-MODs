// ImpCoordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GisqImpCoordCmd.h"
#include "ImpCoordDlg.h"


// CImpCoordDlg 对话框

IMPLEMENT_DYNAMIC(CImpCoordDlg, CGSBCGDialog)

CImpCoordDlg::CImpCoordDlg(GisqLib::_DGisqFrameworkCOMPtr ipFramework, CWnd* pParent /*=NULL*/)
	: CGSBCGDialog(CImpCoordDlg::IDD, pParent)
	, m_strShowFileType(_T(""))
	, m_nXType(2)
	, m_nYType(1)
	, m_sSymbol(_T(","))
	,m_strOpenFilePath(_T(""))
	, m_strShowCoordExample(_T(""))
	, m_IsSetCustomType(FALSE)
{
	m_eCoordType = nulltype;
	m_spSpatialRederence = (IUnknown*)NULL;
	m_ipFramework = ipFramework;
	m_spWorkspaceSpatial = (IUnknown*)NULL;
	m_nWorkspacesSpatialNum = -1;
}

CImpCoordDlg::~CImpCoordDlg()
{
}

void CImpCoordDlg::DoDataExchange(CDataExchange* pDX)
{
	CGSBCGDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strOpenFilePath);
	//DDX_Text(pDX, IDC_EDITXTYPE, m_nXType);
	//DDX_Text(pDX, IDC_EDITYTYPE, m_nYType);
	DDX_Control(pDX, IDC_EDITXTYPE, m_XEditCtrl);
	DDX_Control(pDX, IDC_EDITYTYPE, m_YEditCtrl);
	DDX_Text(pDX, IDC_EDITSYMBOL, m_sSymbol);
	DDX_Control(pDX, IDC_EDITSYMBOL, m_SymbolEditCtrl);
	DDX_Control(pDX, IDC_EDIT1, m_wndFilePath);
	DDX_Text(pDX, IDC_CHECKTYPE, m_strShowFileType);
	DDX_Text(pDX, IDC_EDIT_SHOWFILE, m_strShowCoordExample);
	DDX_Check(pDX, IDC_CUSTOMTYPE, m_IsSetCustomType);
}


BEGIN_MESSAGE_MAP(CImpCoordDlg, CGSBCGDialog)
	ON_EN_CHANGE(IDC_EDIT1, &CImpCoordDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_CUSTOMTYPE, &CImpCoordDlg::OnBnClickedCustomtype)
	ON_BN_CLICKED(IDOK, &CImpCoordDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
	ON_EN_CHANGE(IDC_EDITSYMBOL, &CImpCoordDlg::OnEnChangeEditsymbol)
	ON_EN_CHANGE(IDC_EDITXTYPE, &CImpCoordDlg::OnEnChangeEditxtype)
	ON_EN_CHANGE(IDC_EDITYTYPE, &CImpCoordDlg::OnEnChangeEditytype)
END_MESSAGE_MAP()


// CImpCoordDlg 消息处理程序

BOOL CImpCoordDlg::OnInitDialog()
{
	CGSBCGDialog::OnInitDialog();

	m_wndFilePath.EnableFileBrowseButton(NULL,_T("TXT(*.txt)|*.txt||"));

	//初始化自定义设置
	m_XEditCtrl.SetWindowText(_T("1"));
	m_YEditCtrl.SetWindowText(_T("2"));
	m_XEditCtrl.EnableWindow(FALSE);
	m_YEditCtrl.EnableWindow(FALSE);
	m_SymbolEditCtrl.EnableWindow(FALSE);
	CComQIPtr<IMapControl2> spMapCtrl;
	m_ipMapCtrl = m_ipFramework->GetActiveControl();
	//获取系统数据库投影
	GetWorkspaceSpatial(m_spWorkspaceSpatial);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CImpCoordDlg::OnBnClickedCustomtype()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (m_IsSetCustomType == FALSE)
	{
		CheckCoordType();
		m_XEditCtrl.EnableWindow(FALSE);
		m_YEditCtrl.EnableWindow(FALSE);
		m_SymbolEditCtrl.EnableWindow(FALSE);
	}else
	{
		m_eCoordType = Custom;
		CheckCustomTypeFile();
		m_XEditCtrl.EnableWindow(TRUE);
		m_YEditCtrl.EnableWindow(TRUE);
		m_SymbolEditCtrl.EnableWindow(TRUE);
	}
}

void CImpCoordDlg::OnEnChangeEdit1()
{
	UpdateData();
	m_nSpatailNum = -1;
	m_spSpatialRederence.Release();

	CString strErr;
	if (CoordFileCheck(m_strOpenFilePath,strErr) == FALSE)
	{
		m_strShowFileType = strErr;
		m_strShowCoordExample.Empty();
		UpdateData(FALSE);
		return;
	}

	CFile fCooordFile;
	fCooordFile.Open(m_strOpenFilePath,CFile::modeReadWrite,NULL);
	ReadCoordFile(fCooordFile,&m_arstrCoord);

 	CheckCoordType();
	
}

FileCodeType  CImpCoordDlg::CheckFile(CFile *pCoordFile)
{

	wchar_t Unicodetest[1];
	pCoordFile->Read(Unicodetest,2);
	if (Unicodetest[0] == 0xFEFF)
	{
		return unicode;
	}
	pCoordFile->SeekToBegin();

	unsigned char Utf8test[3];
	pCoordFile->Read(Utf8test,3);
	if (Utf8test[0] == 0xEF && Utf8test[1] == 0xBB && Utf8test[2] == 0xBF)
	{
		return utf8;
	}

	pCoordFile->SeekToBegin();

	return asni;
}

BOOL CImpCoordDlg::ReadCoordFile(CFile &CoordFile,CArray<CString,CString> *arstrTmp)
{
	USES_CONVERSION;
	FileCodeType fType;
	fType = CheckFile(&CoordFile);
	if (fType == unkown)
	{
		MessageBox(_T("未知文件编码类型"),_T("错误"),MB_OK);
		return FALSE;
	}
	if (arstrTmp->GetCount() != 0)
	{
		arstrTmp->RemoveAll();
	}

	if (fType == unicode)
	{
		//这个用于读取UniCode文件头,跳过这一段读正文文本
		CoordFile.SeekToBegin();
		wchar_t Unicodetest[1];
		CoordFile.Read(Unicodetest,2);

		//这个用于以Unicode形式读取文件
		UINT nlenth;
		nlenth = CoordFile.GetLength();
		wchar_t *pwcharBuf = new wchar_t[nlenth +1];
		ZeroMemory(pwcharBuf,nlenth +1);
		//这个地方读取了全部字符，然后就是要按行处理
		CoordFile.Read(pwcharBuf,nlenth+1);
		CString strBuf;
		strBuf.Format(_T("%s"),pwcharBuf);

		//按行分割为数组
		int nStrIndex;
		int ntest = 0;
		for (UINT i = 0 ; i < nlenth/2-1;)
		{
			nStrIndex = strBuf.Find(_T("\r\n"),i);
			if (nStrIndex != -1)
			{
				arstrTmp->Add(strBuf.Mid(i,nStrIndex - i));
				TRACE(arstrTmp->GetAt(ntest));
				TRACE(_T("\n"));
				int nfindn;
				nfindn = arstrTmp->GetAt(ntest).Find(_T("\n"));
				if (nfindn != -1)
				{
					i++;//如果是以“\n\r\n”结尾，那么这里必须多加1并且删除这个地方的“\n”
					nfindn = arstrTmp->GetAt(ntest).Delete(nfindn,1);
				}
				ntest++;
				i = nStrIndex + 2;
			}else
			{
				arstrTmp->Add(strBuf.Mid(i));
				TRACE(arstrTmp->GetAt(ntest));
				TRACE(_T("\n"));
				int nfindn;
				nfindn = arstrTmp->GetAt(ntest).Find(_T("\n"));
				if (nfindn != -1)
				{
					//如果是以“\n\r\n”结尾，那么这里必须多加1并且删除这个地方的“\n”
					nfindn = arstrTmp->GetAt(ntest).Delete(nfindn,1);
					i++;
				}
				break;//如果没找到回车说明读到文件结尾了
			}
		}

		delete pwcharBuf;
		for (int i = 0; i < arstrTmp->GetCount(); i++)
		{
			CString strTmp = arstrTmp->GetAt(i);
			if (arstrTmp->GetAt(i).GetLength() == 0)
			{
				arstrTmp->RemoveAt(i,1);
				i--;
			}
		}
		return TRUE;
	}else if (fType == asni)
	{
		UINT nlenth;
		nlenth = CoordFile.GetLength();
		char *pcharBuf = new char[nlenth +1];
		ZeroMemory(pcharBuf,nlenth +1);

		CoordFile.Read(pcharBuf,nlenth+1);

		LPWSTR pwcharBuf = A2W(pcharBuf);

		CString strBuf;
		strBuf.Format(_T("%s"),pwcharBuf);
		//按行分割为数组
		int nStrIndex;
		int ntest = 0;
		for (UINT i = 0 ; i < nlenth-1;)
		{
			nStrIndex = strBuf.Find(_T("\r\n"),i);
			if (nStrIndex != -1)
			{
				arstrTmp->Add(strBuf.Mid(i,nStrIndex - i));
				TRACE(arstrTmp->GetAt(ntest));
				TRACE(_T("\n"));
				int nfindn;
				nfindn = arstrTmp->GetAt(ntest).Find(_T("\n"));
				if (nfindn != -1)
				{
					//如果是以“\n\r\n”结尾，那么这里必须多加1并且删除这个地方的“\n”
					nfindn = arstrTmp->GetAt(ntest).Delete(nfindn,1);
					i++;
				}
				ntest++;
				i = nStrIndex + 2;
			}else
			{
				arstrTmp->Add(strBuf.Mid(i));
				TRACE(arstrTmp->GetAt(ntest));
				TRACE(_T("\n"));
				int nfindn;
				nfindn = arstrTmp->GetAt(ntest).Find(_T("\n"));
				if (nfindn != -1)
				{
					//如果是以“\n\r\n”结尾，那么这里必须多加1并且删除这个地方的“\n”
					nfindn = arstrTmp->GetAt(ntest).Delete(nfindn,1);
					i++;
				}
				break;//如果没找到回车说明读到文件结尾了
			}
		}

		delete pcharBuf;
		for (int i = 0; i < arstrTmp->GetCount(); i++)
		{
			CString strTmp = arstrTmp->GetAt(i);
			if (arstrTmp->GetAt(i).GetLength() == 0)
			{
				arstrTmp->RemoveAt(i,1);
				i--;
			}
		}
		return TRUE;
	}else if (fType == utf8)
	{
		UINT nlenth;
		nlenth = CoordFile.GetLength();
		char *pcharBuf = new char[nlenth +1];
		ZeroMemory(pcharBuf,nlenth +1);

		CoordFile.Read(pcharBuf,nlenth+1);

		int wcsLen = MultiByteToWideChar( CP_UTF8, 0, pcharBuf, -1, NULL, 0 );  
		LPWSTR result = new wchar_t[wcsLen + 1];
		::MultiByteToWideChar( CP_UTF8, 0, pcharBuf, -1, result, wcsLen); 

		CString strBuf;
		strBuf.Format(_T("%s"),result);
		//按行分割为数组
		int nStrIndex;
		int ntest = 0;
		for (UINT i = 0 ; i < nlenth-1;)
		{
			nStrIndex = strBuf.Find(_T("\r\n"),i);
			if (nStrIndex != -1)
			{
				arstrTmp->Add(strBuf.Mid(i,nStrIndex - i));
				TRACE(arstrTmp->GetAt(ntest));
				TRACE(_T("\n"));
				int nfindn;
				nfindn = arstrTmp->GetAt(ntest).Find(_T("\n"));
				if (nfindn != -1)
				{
					//如果是以“\n\r\n”结尾，那么这里必须多加1并且删除这个地方的“\n”
					nfindn = arstrTmp->GetAt(ntest).Delete(nfindn,1);
					i++;
				}
				ntest++;
				i = nStrIndex + 2;
			}else
			{
				arstrTmp->Add(strBuf.Mid(i));
				TRACE(arstrTmp->GetAt(ntest));
				TRACE(_T("\n"));
				int nfindn;
				nfindn = arstrTmp->GetAt(ntest).Find(_T("\n"));
				if (nfindn != -1)
				{
					//如果是以“\n\r\n”结尾，那么这里必须多加1并且删除这个地方的“\n”
					nfindn = arstrTmp->GetAt(ntest).Delete(nfindn,1);
					i++;
				}
				break;//如果没找到回车说明读到文件结尾了
			}
		}
		delete result;
		delete pcharBuf;
		for (int i = 0; i < arstrTmp->GetCount(); i++)
		{
			CString strTmp = arstrTmp->GetAt(i);
			if (arstrTmp->GetAt(i).GetLength() == 0)
			{
				arstrTmp->RemoveAt(i,1);
				i--;
			}
		}
		return TRUE;
	}
	//做一遍遍历，检查并删除空行

	return TRUE;
}

BOOL CImpCoordDlg::FindCustomXY(CString cs,double *x,double *y)
{
	UpdateData();
	int i = 1;
	for (int nIndex = 0; nIndex < cs.GetLength(); )
	{
		if (i == m_nXType)
		{
			CString TmpStr;
			TmpStr = cs.Mid(nIndex,cs.GetLength());
			LPCWSTR ptmp;
			ptmp = TmpStr;
			char *pcahrtmp = new char[100];
			wcstombs_s(NULL,pcahrtmp,100,ptmp,100);
			sscanf_s(pcahrtmp,"%lf",x);
			delete pcahrtmp;
		}
		if (i == m_nYType)
		{
			CString TmpStr;
			TmpStr = cs.Mid(nIndex,cs.GetLength());
			LPCWSTR ptmp;
			ptmp = TmpStr;
			char *pcahrtmp = new char[100];
			wcstombs_s(NULL,pcahrtmp,100,ptmp,100);
			sscanf_s(pcahrtmp,"%lf",y);
			delete pcahrtmp;
		}
		if (*x != 0 && *y != 0)
		{
			break;
		}
		int n = 0;
		n = cs.Find(m_sSymbol,nIndex);
		if (n > 0)
		{
			i++;
			nIndex = n + 1;
		}else
		{//找不到分隔符，这行没用
			*x = 0;
			*y = 0;
			break;
		}
	}
	return TRUE;
}

BOOL CImpCoordDlg::FindCountryXY(CString cs, int *nPoint, int *nCircle, double *x,double *y)
{
	swscanf_s(cs, _T("J%d,%d,%lf,%lf"), nPoint, nCircle, y, x);
	if (x==0||y==0)
	{
		swscanf_s(cs, _T("%d,%d,%lf,%lf"), nPoint, nCircle, y, x);
	}
	return TRUE;
}
BOOL CImpCoordDlg::FindProvinceXY(CString cs, int *nPoint, int *nCircle, int *nDKH, int *nJZD, double *x,double *y)
{
	swscanf_s(cs,_T("%d,%d,%d,J%d,%lf,%lf"), nPoint, nCircle, nDKH, nJZD, y, x);
	if (x==0&&y==0)
	{
		swscanf_s(cs,_T("%d,%d,%d,%d,%lf,%lf"), nPoint, nCircle, nDKH, nJZD, y, x);
	}
	return TRUE;
}


BOOL CImpCoordDlg::FindGisqCustomXY(CString cs,double *x,double *y)
{
	swscanf_s(cs, _T("%lf,%lf"), y, x);
	return TRUE;
}


int CImpCoordDlg::GetSpatialNum(CString strCoord)
{
	int Unused1 = 0;
	int Unused2 = 0;
	int Unused3 = 0;
	int Unused4 = 0;
	double dbX = 0;
	double dbY = 0;
	switch(m_eCoordType)
	{
	case Country:
		FindCountryXY(strCoord,&Unused1,&Unused2,&dbX,&dbY);
		break;
	case Province:
		FindProvinceXY(strCoord,&Unused1,&Unused2,&Unused3,&Unused4,&dbX,&dbY);
		break;
	case Custom:
		FindCustomXY(strCoord,&dbX,&dbY);
		break;
	case Country_P:
		FindCountryXY(strCoord,&Unused1,&Unused2,&dbX,&dbY);
		break;
	case GisqCustom:
		FindGisqCustomXY(strCoord,&dbX,&dbY);
		break;
	default:
		break;
	}
	if (dbX < 1000000 && dbX > 100000)//x6位，不带大数,强投到系统坐标
	{
		if (m_nWorkspacesSpatialNum < 50)//系统坐标带大数，转为不带大数
		{
			return m_nWorkspacesSpatialNum * 3;
		}else if (m_nWorkspacesSpatialNum == 80)//系统坐标为经纬度，强投120
		{
			return 120;
		}else
		{
			return m_nWorkspacesSpatialNum;
		}
	}else if (dbX < 200 && dbX > 100)//x3位，经纬度
	{
		return 80;
	}else if (dbX > 10000000)//x8位，带大数
	{
		return dbX/1000000;
	}
	return -1;//其他是错误情况
}

void CImpCoordDlg::ShowFileLine()
{
	m_strShowCoordExample.Empty();
	if (m_arstrCoord.GetCount() == 0)
	{
		m_strShowCoordExample = _T("当前文件为空");
	}else if (m_eCoordType == Country)
	{
		for (int i = 12; i < m_arstrCoord.GetCount(); i++)
		{
			m_strShowCoordExample += m_arstrCoord.GetAt(i);
			m_strShowCoordExample += _T("\r\n");
			if (i >= 15)
			{
				break;
			}
		}
		m_IsSetCustomType = FALSE;
		m_XEditCtrl.EnableWindow(FALSE);
		m_YEditCtrl.EnableWindow(FALSE);
		m_SymbolEditCtrl.EnableWindow(FALSE);
	}else if (m_eCoordType == Country_P)
	{
		for (int i = 12; i < m_arstrCoord.GetCount(); i++)
		{
			m_strShowCoordExample += m_arstrCoord.GetAt(i);
			m_strShowCoordExample += _T("\r\n");
			if (i >= 15)
			{
				break;
			}
		}
		m_IsSetCustomType = FALSE;
		m_XEditCtrl.EnableWindow(FALSE);
		m_YEditCtrl.EnableWindow(FALSE);
		m_SymbolEditCtrl.EnableWindow(FALSE);
	}else if (m_eCoordType == Province)
	{
		for (int i = 0; i < m_arstrCoord.GetCount(); i++)
		{
			m_strShowCoordExample += m_arstrCoord.GetAt(i);
			m_strShowCoordExample += _T("\r\n");
			if (i >= 5)
			{
				break;
			}
		}
		m_IsSetCustomType = FALSE;
		m_XEditCtrl.EnableWindow(FALSE);
		m_YEditCtrl.EnableWindow(FALSE);
		m_SymbolEditCtrl.EnableWindow(FALSE);
	}else
	{
		for (int i = 0; i < m_arstrCoord.GetCount(); i++)
		{
			m_strShowCoordExample += m_arstrCoord.GetAt(i);
			m_strShowCoordExample += _T("\r\n");
			if (i >= 5)
			{
				break;
			}
		}
	}
	UpdateData(FALSE);
}


void CImpCoordDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//导入前最后一次检查文件是否正确
	CString strErr;
	if (CoordFileCheck(m_strOpenFilePath,strErr) == FALSE)
	{
		BCGPMessageBox(strErr,_T("错误"),MB_OK);
		return;
	}	
	if (m_arstrCoord.GetCount() == 0)
	{
		BCGPMessageBox(_T("所选文件无法读取到界址点"),_T("文件错误"),MB_OK);
		return;
	}
	if (m_eCoordType == Custom && m_IsSetCustomType == FALSE)
	{
		BCGPMessageBox(_T("请勾选并设置非标准界址点文件格式"),_T("格式错误"),MB_OK);
		return;
	}
	//创建带号
	if (m_nSpatailNum > 0)
	{
		CreateSpatial(m_nSpatailNum);
	}else
	{
		CreateSpatial(40);
	}
	//找到辅助面层，调用解析函数
	CComQIPtr<IMapControl2> spMapCtrl = (CComQIPtr<IMapControl2>)m_ipFramework->GetActiveControl();
	CComQIPtr<IMap> spMap;
	if (spMapCtrl != NULL)
	{
		spMapCtrl->get_Map(&spMap);
	}
	CComQIPtr<IUID> spUID;
	spUID.CoCreateInstance(CLSID_UID);
	CComQIPtr<IEnumLayer> spEnumLayer;
	spUID->put_Value(_variant_t("{40A9E885-5533-11d0-98BE-00805F7CED21}"));
	spMap->get_Layers(spUID,VARIANT_TRUE,&spEnumLayer);
	if (spEnumLayer == NULL) return ;
	CComQIPtr<ILayer> spLayer;

	spEnumLayer->Next(&spLayer);
	while(spLayer != NULL)
	{
		CComQIPtr<IFeatureLayer> spFtLayer = spLayer;
		if (m_spFeatureClass != NULL) m_spFeatureClass.Release();
		spFtLayer->get_FeatureClass(&m_spFeatureClass);
		if (m_spFeatureClass == NULL)
		{
			spLayer.Release();
			spEnumLayer->Next(&spLayer);
			continue;
		}
		CComBSTR bstrName;
		spLayer->get_Name(&bstrName);
		if (bstrName == _T("辅助面"))
		{
			m_spFZLayer = spLayer;
			break;
		}

		spLayer.Release();
		spEnumLayer->Next(&spLayer);
	}
	if (m_spFeatureClass == NULL)
	{
		MessageBox(_T("请匹配辅助面数据源"),_T("错误"),MB_OK);
		return;
	}
	if (ReadCoord(m_spFeatureClass) == FALSE)
	{
		MessageBox(_T("导入界址点失败，请检查界址点文件格式是否合法"),_T("错误"),MB_OK);
	}else
	{
		MessageBox(_T("界址点已导入辅助面"),_T("导入界址点成功"),MB_OK);
		//刷新辅助面属性表（如果打开）
		CComQIPtr<GisqLib::IGSDockableWindowManager> spGSDockableWindowManager = m_ipFramework->GetDockableWindowManager();
		short nCount = 0;
		nCount = spGSDockableWindowManager->Count();
		for (short i = 0; i < nCount; i++)
		{
			CComQIPtr<GisqLib::IGSDockableWindow> spDockwin = spGSDockableWindowManager->Item(i);
			_bstr_t bstrGUID = spDockwin->GetDockWindowDefGUID();
			if (spDockwin != NULL)
			{
				CComQIPtr<IGSAttributesTableView> spGSAttributesTableView = (CComQIPtr<IGSAttributesTableView>)spDockwin->GetDockableWindowTabDef();
				if (spGSAttributesTableView != NULL)
				{
					CString strName = _T("属性表：辅助面");
					_bstr_t bstrTempName = spDockwin->GetCaption();
					if (strName.CompareNoCase(bstrTempName)==0)
					{
						//重新加载
						CString strWhereClause(_T("2=2"));
						IQueryFilterPtr pQueryFilter(CLSID_QueryFilter);
						pQueryFilter->put_WhereClause(_bstr_t(strWhereClause));
						spGSAttributesTableView->SearchRecords(m_ipFramework,(CComQIPtr<ITable>)m_spFZLayer,pQueryFilter);
						spDockwin->ShowWindow(VARIANT_TRUE);
					}
				}
			}
		}
	}
	spMapCtrl->Refresh(esriViewAll);
}

void CImpCoordDlg::CreateSpatial(int nProjCS)
{
	CComQIPtr<ISpatialReferenceFactory> spSpatialReFactory;
	spSpatialReFactory.CoCreateInstance(CLSID_SpatialReferenceEnvironment);
	CComQIPtr<IProjectedCoordinateSystem> spProjCorrdSys;
	switch(nProjCS)
	{
	case 39:
		spSpatialReFactory->CreateProjectedCoordinateSystem(esriSRProjCS_Xian1980_3_Degree_GK_Zone_39, &spProjCorrdSys);
		break;
	case 40:
		spSpatialReFactory->CreateProjectedCoordinateSystem(esriSRProjCS_Xian1980_3_Degree_GK_Zone_40, &spProjCorrdSys);
		break;
	case 41:
		spSpatialReFactory->CreateProjectedCoordinateSystem(esriSRProjCS_Xian1980_3_Degree_GK_Zone_41, &spProjCorrdSys);
		break;
	case 117:
		spSpatialReFactory->CreateProjectedCoordinateSystem(esriSRProjCS_Xian1980_3_Degree_GK_CM_117E, &spProjCorrdSys);
		break;
	case 120:
		spSpatialReFactory->CreateProjectedCoordinateSystem(esriSRProjCS_Xian1980_3_Degree_GK_CM_120E, &spProjCorrdSys);
		break;
	case 123:
		spSpatialReFactory->CreateProjectedCoordinateSystem(esriSRProjCS_Xian1980_3_Degree_GK_CM_123E, &spProjCorrdSys);
	default:
		return;
	}	
	if (m_spSpatialRederence != NULL)
	{
		m_spSpatialRederence.Release();
	}
	m_spSpatialRederence = spProjCorrdSys;
	CGeometryOperation::CreateSpatialReference(m_spSpatialRederence);
}

BOOL CImpCoordDlg::ReadCoord(IFeatureClass *ipFtClass)
{
	BOOL bIsCustomSetCorrect = FALSE;
	switch (m_eCoordType)
	{
	case Country:
		if (ReadCountryCoord(ipFtClass) == FALSE)
		{return FALSE;}		
		break;
	case Province:
		if (ReadProvinceCoord(ipFtClass) == FALSE)
		{return FALSE;}
		break;
	case Custom:
		for (int i = 0; i < m_arstrCoord.GetCount();i++)
		{
			int nSpatail = GetSpatialNum(m_arstrCoord.GetAt(i));
			if (nSpatail > 0)
			{
				CreateSpatial(nSpatail);
				bIsCustomSetCorrect = TRUE;
				break;
			}
		}
		if (bIsCustomSetCorrect == FALSE)
		{
			//edit by lcc 20170301 格式错误这里不进行提示，在返回后在点击响应函数中提示
			//BCGPMessageBox(_T("非标准坐标格式设置错误，不能成功读取界址点"),_T("错误"),MB_OK);
			return FALSE;
		}
		if (ReadCustomCoord(ipFtClass) == FALSE)
		{return FALSE;}
		break;
	case Country_P:
		if (ReadCountry_PCoord(ipFtClass) == FALSE)
		{return FALSE;}		
		break;
	case GisqCustom:
		if (ReadGisqCustomCoord(ipFtClass) == FALSE)
		{return FALSE;}
		break;
	default:
		break;
	}
	return TRUE;
}


BOOL CImpCoordDlg::ReadCountryCoord(CComQIPtr<IFeatureClass> spFtClass)
{
	HRESULT hr;
	CComQIPtr<IFeature> spFt;
	CComQIPtr<IPolygon> spPolyGon;
	CComQIPtr<IGeometry> spGeometry;
	CComQIPtr<IPointCollection> spPointCollection;
	CComQIPtr<IGeometryCollection> spGeometryCollection;
	//下面两个用于闪烁和缩放
	CComQIPtr<IGeometry> spAllGeometry;
	CComQIPtr<IGeometryCollection> spAllGeometryCollection;
	spAllGeometry.CoCreateInstance(CLSID_GeometryBag);
	spAllGeometry->putref_SpatialReference(m_spSpatialRederence);
	spAllGeometry->QueryInterface(&spAllGeometryCollection);

	//转化为图形
	int nStopRingID = 0;
	int nStartPointID = 1;
	//部坐标的前12行没有用，这里从第13行开始读
	for (int i = 12; i < m_arstrCoord.GetCount(); i++)
	{
		LPCWSTR readtmp;
		readtmp = m_arstrCoord.GetAt(i).GetString();
		TRACE(readtmp);
		TRACE(_T("\n"));
		if (m_arstrCoord.GetAt(i).Find(_T("@")) > 0)//一个要素开始
		{
			if (spPointCollection != NULL)//说明之前有一个要素或圈需要保存
			{
				if (nStopRingID == 1)//之前的要素只有一个圈,保存这个要素
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

					SaveFeature(spGeometry,spFt);
					spAllGeometryCollection->AddGeometry(spGeometry);

				}else if (nStopRingID > 1)//这是上个要素的一个内切割多边形,先切割,再保存
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

					VARIANT_BOOL bIsEmpty;
					spGeometry->get_IsEmpty(&bIsEmpty);

					hr = PolygonCutHollow(spPointCollection,spGeometry);
					if (hr != S_OK) return hr;
					SaveFeature(spGeometry,spFt);
					spAllGeometryCollection->AddGeometry(spGeometry);
				}				
			}

			//创建要素，写入属性行信息	
			if (spFt != NULL) spFt.Release();
			spFtClass->CreateFeature(&spFt);
			AddFieldsToFeature(m_arstrCoord.GetAt(i),spFt);
			//初始化其他
			if (spPolyGon != NULL) spPolyGon.Release();
			if (spGeometry != NULL) spGeometry.Release();
			if (spPointCollection != NULL) spPointCollection.Release();
			if (spGeometryCollection != NULL) spGeometryCollection.Release();

			spPolyGon.CoCreateInstance(CLSID_Polygon);
			spGeometry = spPolyGon;
			spGeometry->QueryInterface(&spGeometryCollection);
			spPointCollection.CoCreateInstance(CLSID_Multipoint);
			nStopRingID = 1;
			nStartPointID = 1;
		}else//不是起始行，那这里处理数据，写入点集		
		{
			LPCWSTR ptmp;
			ptmp = m_arstrCoord.GetAt(i).GetString();
			//TRACE(ptmp);
			int nPointID = 0;
			int nRingID = 0;
			double x = 0;
			double y = 0;
			//这里格式化了一行的数据
			//swscanf_s(ptmp,_T("J%d,%d,%lf,%lf"),&nPointID,&nRingID,&y,&x);
			FindCountryXY(m_arstrCoord.GetAt(i),&nPointID,&nRingID,&x,&y);
			if (nPointID == nStartPointID && i > m_arstrCoord.GetCount() -3)//到底了
			{
				if (nRingID == 1)//如果结束的是第一个圈,那么成环保存就可以了
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

				}else//如果不是第一个圈,切割
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

					VARIANT_BOOL bIsEmpty;
					spGeometry->get_IsEmpty(&bIsEmpty);

				}

				SaveFeature(spGeometry,spFt);
				spAllGeometryCollection->AddGeometry(spGeometry);
				break;

			}else if (nRingID > nStopRingID)//一个要素的下一个圈开始
			{
				if (nStopRingID == 1)//如果结束的是第一个圈,那么成环保存就可以了
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

				}else//如果不是第一个圈,切割
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

					VARIANT_BOOL bIsEmpty;
					spGeometry->get_IsEmpty(&bIsEmpty);

					hr = PolygonCutHollow(spPointCollection,spGeometry);
					if (hr != S_OK) return hr;
				}
				//不管哪种情况,更新圈号,重置点集
				nStopRingID = nRingID;
				nStartPointID = nPointID;
				spPointCollection.Release();
				spPointCollection.CoCreateInstance(CLSID_Multipoint);

				CComQIPtr<IPoint> spPoint;
				spPoint.CoCreateInstance(CLSID_Point);
				spPoint->PutCoords(x,y);
				spPointCollection->AddPoint(spPoint);
			}else//就是一个普通的点
			{
				CComQIPtr<IPoint> spPoint;
				spPoint.CoCreateInstance(CLSID_Point);
				spPoint->PutCoords(x,y);
				spPointCollection->AddPoint(spPoint);
			}			
		}
	}
	//添加判断，结束时最后一个要素的对象肯定存在，检查信息是否正确
	CComQIPtr<IGeometry> spGeo;
	spFt->get_Shape(&spGeo);
	if (spGeo == NULL)
	{
		BCGPMessageBox(_T("结束点缺失！\r\n当前界址点直到文件结尾仍不能形成图形！"),_T("错误"),MB_OK);
		return FALSE;
	}
	if (m_ipMapCtrl != NULL)
	{
		//缩放
		CComQIPtr<IActiveView> spActiveview;
		CComQIPtr<IMap> spMap;
		m_ipMapCtrl->get_Map(&spMap);
		spActiveview = spMap;
		CComQIPtr<IEnvelope> spEnvelope;
		spAllGeometry->get_Envelope(&spEnvelope);
		spActiveview->put_Extent(spEnvelope);
		m_ipMapCtrl->Refresh(esriViewAll);
		//下面是闪烁
		CComQIPtr<ISpatialReference> spSpatial;
		CComBSTR bstrName;
		spAllGeometry->get_SpatialReference(&spSpatial);
		spSpatial->get_Name(&bstrName);
		ISpatialReferencePtr ipSpat;
		CComQIPtr<IGeoDataset> ipGeoDataset;
		ipGeoDataset = m_spFeatureClass;
		ipGeoDataset->get_SpatialReference(&ipSpat);
		spAllGeometry->Project(ipSpat);
		FlashShapes(m_ipMapCtrl,spAllGeometry);
// 		CComQIPtr<ISpatialReference> spSpatial;
// 		m_ipMapCtrl->get_SpatialReference(&spSpatial);
// 		spAllGeometry->Project(spSpatial);
// 		CComQIPtr<IColor> spColor;
// 		spColor.CoCreateInstance(CLSID_RgbColor);
// 		spColor->put_RGB(RGB(255,0,0));
// 		CComQIPtr<ISimpleFillSymbol> spSimpleFillSymbol;
// 		spSimpleFillSymbol.CoCreateInstance(CLSID_SimpleFillSymbol);
// 		spSimpleFillSymbol->put_Color(spColor);
// 		CComQIPtr<ISimpleLineSymbol> spSimpleLineSymbol;
// 		spSimpleLineSymbol.CoCreateInstance(CLSID_SimpleLineSymbol);
// 		spSimpleLineSymbol->put_Style(esriSLSSolid);
// 		spSimpleLineSymbol->put_Color(spColor);
// 		spSimpleLineSymbol->put_Width(4);
// 		spSimpleFillSymbol->put_Outline(spSimpleLineSymbol);
// 		CComQIPtr<ISymbol> spSymbol = spSimpleFillSymbol;
// 		hr = spSymbol->put_ROP2(esriROPMergeNotPen);
// 		m_ipMapCtrl->FlashShape(spAllGeometry,2,200,CComVariant(spSimpleFillSymbol));
	}
	return TRUE;
}

BOOL CImpCoordDlg::ReadCountry_PCoord(CComQIPtr<IFeatureClass> spFtClass)
{
	HRESULT hr;
	CComQIPtr<IFeature> spFt;
	CComQIPtr<IPolygon> spPolyGon;
	CComQIPtr<IGeometry> spGeometry;
	CComQIPtr<IPointCollection> spPointCollection;
	CComQIPtr<IGeometryCollection> spGeometryCollection;
	//下面两个用于闪烁和缩放
	CComQIPtr<IGeometry> spAllGeometry;
	CComQIPtr<IGeometryCollection> spAllGeometryCollection;
	spAllGeometry.CoCreateInstance(CLSID_GeometryBag);
	spAllGeometry->putref_SpatialReference(m_spSpatialRederence);
	spAllGeometry->QueryInterface(&spAllGeometryCollection);

	//转化为图形
	int nStopRingID = 0;
	int nStartPointID = 1;
	//部坐标的前12行没有用，这里从第13行开始读
	for (int i = 12; i < m_arstrCoord.GetCount(); i++)
	{
		LPCWSTR readtmp;
		readtmp = m_arstrCoord.GetAt(i).GetString();
		TRACE(readtmp);
		TRACE(_T("\n"));
		if (m_arstrCoord.GetAt(i).Find(_T("@")) > 0)//一个要素开始
		{
			if (spPointCollection != NULL)//说明之前有一个要素或圈需要保存
			{
				if (nStopRingID == 1)//之前的要素只有一个圈,保存这个要素
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

					SaveFeature(spGeometry,spFt);
					spAllGeometryCollection->AddGeometry(spGeometry);

				}else if (nStopRingID > 1)//这是上个要素的一个内切割多边形,先切割,再保存
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

					VARIANT_BOOL bIsEmpty;
					spGeometry->get_IsEmpty(&bIsEmpty);

					hr = PolygonCutHollow(spPointCollection,spGeometry);
					if (hr != S_OK) return hr;
					SaveFeature(spGeometry,spFt);
					spAllGeometryCollection->AddGeometry(spGeometry);
				}				
			}

			//创建要素，写入属性行信息	
			if (spFt != NULL) spFt.Release();
			spFtClass->CreateFeature(&spFt);
			AddFieldsToFeature(m_arstrCoord.GetAt(i),spFt);
			//初始化其他
			if (spPolyGon != NULL) spPolyGon.Release();
			if (spGeometry != NULL) spGeometry.Release();
			if (spPointCollection != NULL) spPointCollection.Release();
			if (spGeometryCollection != NULL) spGeometryCollection.Release();

			spPolyGon.CoCreateInstance(CLSID_Polygon);
			spGeometry = spPolyGon;
			spGeometry->QueryInterface(&spGeometryCollection);
			spPointCollection.CoCreateInstance(CLSID_Multipoint);
			nStopRingID = 1;
			nStartPointID = 1;
		}else//不是起始行，那这里处理数据，写入点集		
		{
			LPCWSTR ptmp;
			ptmp = m_arstrCoord.GetAt(i).GetString();
			//TRACE(ptmp);
			int nPointID = 0;
			int nRingID = 0;
			double x = 0;
			double y = 0;
			//这里格式化了一行的数据
			//swscanf_s(ptmp,_T("J%d,%d,%lf,%lf"),&nPointID,&nRingID,&y,&x);
			FindCountryXY(m_arstrCoord.GetAt(i),&nPointID,&nRingID,&x,&y);
			if (nPointID == nStartPointID && i > m_arstrCoord.GetCount() -3)//到底了
			{
				if (nRingID == 1)//如果结束的是第一个圈,那么成环保存就可以了
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

				}else//如果不是第一个圈,切割
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

					VARIANT_BOOL bIsEmpty;
					spGeometry->get_IsEmpty(&bIsEmpty);

				}

				SaveFeature(spGeometry,spFt);
				spAllGeometryCollection->AddGeometry(spGeometry);
				break;

			}else if (nRingID > nStopRingID)//一个要素的下一个圈开始
			{
				if (nStopRingID == 1)//如果结束的是第一个圈,那么成环保存就可以了
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

				}else//如果不是第一个圈,切割
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

					VARIANT_BOOL bIsEmpty;
					spGeometry->get_IsEmpty(&bIsEmpty);

					hr = PolygonCutHollow(spPointCollection,spGeometry);
					if (hr != S_OK) return hr;
				}
				//不管哪种情况,更新圈号,重置点集
				nStopRingID = nRingID;
				nStartPointID = nPointID;
				spPointCollection.Release();
				spPointCollection.CoCreateInstance(CLSID_Multipoint);

				CComQIPtr<IPoint> spPoint;
				spPoint.CoCreateInstance(CLSID_Point);
				spPoint->PutCoords(x,y);
				spPointCollection->AddPoint(spPoint);
			}else//就是一个普通的点
			{
				CComQIPtr<IPoint> spPoint;
				spPoint.CoCreateInstance(CLSID_Point);
				spPoint->PutCoords(x,y);
				spPointCollection->AddPoint(spPoint);
			}			
		}
	}
	//添加判断，结束时最后一个要素的对象肯定存在，检查信息是否正确
	CComQIPtr<IGeometry> spGeo;
	spFt->get_Shape(&spGeo);
	if (spGeo == NULL)
	{
		BCGPMessageBox(_T("结束点缺失！\r\n当前界址点直到文件结尾仍不能形成图形！"),_T("错误"),MB_OK);
		return FALSE;
	}
	if (m_ipMapCtrl != NULL)
	{
		//缩放
		CComQIPtr<IActiveView> spActiveview;
		CComQIPtr<IMap> spMap;
		m_ipMapCtrl->get_Map(&spMap);
		spActiveview = spMap;
		CComQIPtr<IEnvelope> spEnvelope;
		spAllGeometry->get_Envelope(&spEnvelope);
		spActiveview->put_Extent(spEnvelope);
		m_ipMapCtrl->Refresh(esriViewAll);
		//下面是闪烁
		CComQIPtr<ISpatialReference> spSpatial;
		CComBSTR bstrName;
		spAllGeometry->get_SpatialReference(&spSpatial);
		spSpatial->get_Name(&bstrName);
		ISpatialReferencePtr ipSpat;
		CComQIPtr<IGeoDataset> ipGeoDataset;
		ipGeoDataset = m_spFeatureClass;
		ipGeoDataset->get_SpatialReference(&ipSpat);
		spAllGeometry->Project(ipSpat);
		FlashShapes(m_ipMapCtrl,spAllGeometry);
	}
	return TRUE;
}

BOOL CImpCoordDlg::ReadProvinceCoord(CComQIPtr<IFeatureClass> spFtClass)
{
	USES_CONVERSION;
	//构造用于记录的各个对象
	HRESULT hr;
	CComQIPtr<IFeature> spFt;
	CComQIPtr<IPolygon> spPolyGon;
	CComQIPtr<IGeometry> spGeometry;
	CComQIPtr<IPointCollection> spPointCollection;
	CComQIPtr<IGeometryCollection> spGeometryCollection;
	//下面两个用于闪烁和缩放
	CComQIPtr<IGeometry> spAllGeometry;
	CComQIPtr<IGeometryCollection> spAllGeometryCollection;
	spAllGeometry.CoCreateInstance(CLSID_GeometryBag);
	spAllGeometry->putref_SpatialReference(m_spSpatialRederence);
	spAllGeometry->QueryInterface(&spAllGeometryCollection);

	//转化为图形
	int nTmpDKH = 0;//当前地块号
	int nStopRingID = 0;//当前圈号
	int nStartPointID = 1;//当前圈起始点号
	for (int i = 0; i < m_arstrCoord.GetCount(); i++)
	{
		LPCWSTR ptmp;
		ptmp = m_arstrCoord.GetAt(i).GetString();

		double x = 0;
		double y = 0;
		int nPointID;
		int nDKH;
		int nRingID;
		int nRPointID;
		//这里格式化了一行的数据
		//swscanf_s(ptmp,_T("%d,%d,%d,J%d,%lf,%lf"),&nPointID,&nDKH,&nRingID,&nRPointID,&y,&x);
		FindProvinceXY(m_arstrCoord.GetAt(i),&nPointID,&nDKH,&nRingID,&nRPointID,&x,&y);
		if (nDKH != nTmpDKH)//一个要素开始
		{
			if (spPointCollection != NULL)//说明之前有一个要素或圈需要保存
			{
				if (nStopRingID == 1)//之前的要素只有一个圈,保存这个要素
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

					SaveFeature(spGeometry,spFt);
					spAllGeometryCollection->AddGeometry(spGeometry);

				}else if (nStopRingID > 1)//这是上个要素的一个内切割多边形,先切割,再保存
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

					VARIANT_BOOL bIsEmpty;
					spGeometry->get_IsEmpty(&bIsEmpty);

					hr = PolygonCutHollow(spPointCollection,spGeometry);
					if (hr != S_OK) return hr;
					SaveFeature(spGeometry,spFt);
					spAllGeometryCollection->AddGeometry(spGeometry);
				}				
			}

			//创建要素，写入属性行信息	
			if (spFt != NULL) spFt.Release();
			spFtClass->CreateFeature(&spFt);
			//AddFieldsToFeature(arstrTmp.GetAt(i),spFt);
			//初始化其他
			if (spPolyGon != NULL) spPolyGon.Release();
			if (spGeometry != NULL) spGeometry.Release();
			if (spPointCollection != NULL) spPointCollection.Release();
			if (spGeometryCollection != NULL) spGeometryCollection.Release();

			spPolyGon.CoCreateInstance(CLSID_Polygon);
			spGeometry = spPolyGon;
			spGeometry->QueryInterface(&spGeometryCollection);
			spPointCollection.CoCreateInstance(CLSID_Multipoint);
			nStopRingID = 1;
			nStartPointID = 1;
			nTmpDKH = nDKH;

			CComQIPtr<IPoint> spPoint;
			spPoint.CoCreateInstance(CLSID_Point);
			spPoint->PutCoords(x,y);
			spPointCollection->AddPoint(spPoint);
		}else//不是起始行，写入点集		
		{
			if (nRPointID == nStartPointID && i > m_arstrCoord.GetCount() - 3)//到底了
			{
				if (nRingID == 1)//如果结束的是第一个圈,那么成环保存就可以了
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

				}else//如果不是第一个圈,切割
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

					VARIANT_BOOL bIsEmpty;
					spGeometry->get_IsEmpty(&bIsEmpty);

					hr = PolygonCutHollow(spPointCollection,spGeometry);
					if (hr != S_OK) return hr;
				}

				SaveFeature(spGeometry,spFt);
				spAllGeometryCollection->AddGeometry(spGeometry);
				break;

			}else if (nRingID > nStopRingID)//一个要素的下一个圈开始
			{
				if (nStopRingID == 1)//如果结束的是第一个圈,那么成环保存就可以了
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

				}else//如果不是第一个圈,切割
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

					VARIANT_BOOL bIsEmpty;
					spGeometry->get_IsEmpty(&bIsEmpty);

					hr = PolygonCutHollow(spPointCollection,spGeometry);
					if (hr != S_OK) return hr;
				}
				//不管哪种情况,更新圈号,重置点集
				nStopRingID = nRingID;
				nStartPointID = nRPointID;
				spPointCollection.Release();
				spPointCollection.CoCreateInstance(CLSID_Multipoint);

				CComQIPtr<IPoint> spPoint;
				spPoint.CoCreateInstance(CLSID_Point);
				spPoint->PutCoords(x,y);
				spPointCollection->AddPoint(spPoint);
			}else//就是一个普通的点
			{
				CComQIPtr<IPoint> spPoint;
				spPoint.CoCreateInstance(CLSID_Point);
				spPoint->PutCoords(x,y);
				spPointCollection->AddPoint(spPoint);
			}			
		}	
	}
	//添加判断，结束时最后一个要素的对象肯定存在，检查信息是否正确
	CComQIPtr<IGeometry> spGeo;
	spFt->get_Shape(&spGeo);
	if (spGeo == NULL)
	{
		BCGPMessageBox(_T("结束点缺失！\r\n当前界址点直到文件结尾仍不能形成图形！"),_T("错误"),MB_OK);
		return FALSE;
	}
	if (m_ipMapCtrl != NULL)
	{
		//缩放
		CComQIPtr<IActiveView> spActiveview;
		CComQIPtr<IMap> spMap;
		m_ipMapCtrl->get_Map(&spMap);
		spActiveview = spMap;
		CComQIPtr<IEnvelope> spEnvelope;
		spAllGeometry->get_Envelope(&spEnvelope);
		spActiveview->put_Extent(spEnvelope);
		m_ipMapCtrl->Refresh(esriViewAll);
		//下面是闪烁
		CComQIPtr<ISpatialReference> spSpatial;
		CComBSTR bstrName;
		spAllGeometry->get_SpatialReference(&spSpatial);
		spSpatial->get_Name(&bstrName);
		ISpatialReferencePtr ipSpat;
		CComQIPtr<IGeoDataset> ipGeoDataset;
		ipGeoDataset = m_spFeatureClass;
		ipGeoDataset->get_SpatialReference(&ipSpat);
		spAllGeometry->Project(ipSpat);
		FlashShapes(m_ipMapCtrl,spAllGeometry);
	}
	return TRUE;
}

BOOL CImpCoordDlg::ReadCustomCoord(IFeatureClass *ipFtClass)
{
	//hr = ipFtClass->CreateFeature(&ipFt);hr = 0x80041354The requested operation is invalid on a closed state
	//构造用于记录的各个对象
	IPointCollectionPtr ipPointCollection = NULL;
	IFeaturePtr ipFt = NULL;
	HRESULT hr;
	IPolygonPtr ipPolyGon = NULL;
	IGeometryPtr ipGeometry = NULL;
	IGeometryCollectionPtr ipGeometryCollection = NULL;
	//下面两个用于闪烁和缩放
	CComQIPtr<IGeometry> spAllGeometry;
	CComQIPtr<IGeometryCollection> spAllGeometryCollection;
	spAllGeometry.CoCreateInstance(CLSID_GeometryBag);
	spAllGeometry->putref_SpatialReference(m_spSpatialRederence);
	spAllGeometry->QueryInterface(&spAllGeometryCollection);

	//转化为图形
	BOOL bIsGeoStart = TRUE;
	double nStartX = 0;
	double nStartY = 0;
	for (int i = 0; i < m_arstrCoord.GetCount(); i++)
	{
		LPCWSTR readtmp;
		readtmp = m_arstrCoord.GetAt(i).GetString();
		TRACE(readtmp);
		TRACE(_T("\n"));

		double x = 0;
		double y = 0;
		FindCustomXY(m_arstrCoord.GetAt(i),&x,&y);
		if (x == 0 || y == 0)
		{
			continue;//这行数据无效
		}
		if (nStartX == 0 && nStartY == 0)
		{//说明这行是当前要素的开始
			nStartX = x;
			nStartY = y;
		}else if (fabs(nStartX - x) < 0.00000001 && fabs(nStartY - y) < 0.00000001)
		{//如果startx，straty都不为零且与当前点相等，说明这行是当前要素的结束，保存


			if (ipPointCollection != NULL)
			{//写进最后一个点，并写入图形中
				IPointPtr ipPoint(CLSID_Point);
				ipPoint->PutCoords(x,y);
				ipPointCollection->AddPoint(ipPoint);

				CComQIPtr<IPointCollection> spPointCollection = ipPointCollection;
				CComQIPtr<IRing> spRing;
				hr = GetRing(spPointCollection,spRing);
				if (hr != S_OK) return hr;
				ipGeometryCollection->AddGeometry(spRing);
				ipPolyGon->Close();
			}				
			//设置拓扑和坐标规则
			ITopologicalOperatorPtr ipTop;
			ipTop = ipPolyGon;
			hr = ipTop->Simplify();

			ISpatialReferencePtr ipSpat;
			IGeoDatasetPtr ipGeoDataset;
			ipGeoDataset = ipFtClass;
			ipGeoDataset->get_SpatialReference(&ipSpat);
			//CreateSpatial(40);
			hr = ipPolyGon->putref_SpatialReference(m_spSpatialRederence);
			ipPolyGon->Project(ipSpat);

			VARIANT_BOOL bIsEmpty;
			ipPolyGon->get_IsEmpty(&bIsEmpty);
			if (bIsEmpty == VARIANT_FALSE)
			{
				ipFtClass->CreateFeature(&ipFt);
				hr = ipFt->putref_Shape(ipPolyGon);
				CComQIPtr<IFeature> spFt = ipFt;
				hr = AddFields(spFt);
				hr = ipFt->Store();	
				spAllGeometryCollection->AddGeometry(ipPolyGon);

				//如果没到结尾，创建下一个图形
				if (m_arstrCoord.GetCount() - 1 - i > 2)
				{

					//重置
					ipFt = NULL;
					ipPolyGon = NULL;
					ipGeometry = NULL;
					ipGeometryCollection = NULL;

					ipPolyGon.CreateInstance(CLSID_Polygon);
					ipGeometry = ipPolyGon;
					hr = ipGeometry->QueryInterface(&ipGeometryCollection);
				}
			}
			//重置
			ipPointCollection = NULL;
			bIsGeoStart = TRUE;
			nStartX = 0;
			nStartY = 0;
			continue;//这行数据以及写入过了，不需要做后面的操作
		}
		if (bIsGeoStart == TRUE)//如果是开始一个图形
		{
			if (ipPointCollection != NULL)//如果不是第一个图形，保存上一个
			{
				CComQIPtr<IPointCollection> spPointCollection = ipPointCollection;
				CComQIPtr<IRing> spRing;
				hr = GetRing(spPointCollection,spRing);
				if (hr != S_OK) return hr;
				bIsGeoStart = FALSE;
				nStartX = 0;
				nStartY = 0;
				ipPointCollection = NULL;
				ipPointCollection.CreateInstance(CLSID_Multipoint);
			}else//或者刚开始
			{
				ipPointCollection.CreateInstance(CLSID_Multipoint);
				bIsGeoStart = FALSE;
				//创建图形
				ipPolyGon.CreateInstance(CLSID_Polygon);
				ipGeometry = ipPolyGon;
				hr = ipGeometry->QueryInterface(&ipGeometryCollection);
			}
		}
		//写入当前读取的点
		IPointPtr ipPoint(CLSID_Point);
		ipPoint->PutCoords(x,y);
		ipPointCollection->AddPoint(ipPoint);


	}
	//添加判断，结束时最后一个要素的对象肯定存在，检查信息是否正确
	if (ipFt == NULL)
	{
		BCGPMessageBox(_T("结束点缺失！\r\n当前界址点直到文件结尾仍不能形成图形！"),_T("错误"),MB_OK);
		return FALSE;
	}
	if (m_ipMapCtrl != NULL)
	{
		//缩放
		CComQIPtr<IActiveView> spActiveview;
		CComQIPtr<IMap> spMap;
		m_ipMapCtrl->get_Map(&spMap);
		spActiveview = spMap;
		CComQIPtr<IEnvelope> spEnvelope;
		spAllGeometry->get_Envelope(&spEnvelope);
		spActiveview->put_Extent(spEnvelope);
		m_ipMapCtrl->Refresh(esriViewAll);
		//下面是闪烁
		CComQIPtr<ISpatialReference> spSpatial;
		CComBSTR bstrName;
		spAllGeometry->get_SpatialReference(&spSpatial);
		spSpatial->get_Name(&bstrName);
		ISpatialReferencePtr ipSpat;
		CComQIPtr<IGeoDataset> ipGeoDataset;
		ipGeoDataset = m_spFeatureClass;
		ipGeoDataset->get_SpatialReference(&ipSpat);
		spAllGeometry->Project(ipSpat);
		FlashShapes(m_ipMapCtrl,spAllGeometry);
	}
	return TRUE;
}

BOOL CImpCoordDlg::ReadGisqCustomCoord(IFeatureClass *ipFtClass)
{
	//hr = ipFtClass->CreateFeature(&ipFt);hr = 0x80041354The requested operation is invalid on a closed state
	//构造用于记录的各个对象
	IPointCollectionPtr ipPointCollection = NULL;
	IFeaturePtr ipFt = NULL;
	HRESULT hr;
	IPolygonPtr ipPolyGon = NULL;
	IGeometryPtr ipGeometry = NULL;
	IGeometryCollectionPtr ipGeometryCollection = NULL;
	//下面两个用于闪烁和缩放
	CComQIPtr<IGeometry> spAllGeometry;
	CComQIPtr<IGeometryCollection> spAllGeometryCollection;
	spAllGeometry.CoCreateInstance(CLSID_GeometryBag);
	spAllGeometry->putref_SpatialReference(m_spSpatialRederence);
	spAllGeometry->QueryInterface(&spAllGeometryCollection);

	//转化为图形
	BOOL bIsGeoStart = TRUE;
	double nStartX = 0;
	double nStartY = 0;
	for (int i = 0; i < m_arstrCoord.GetCount(); i++)
	{
		LPCWSTR readtmp;
		readtmp = m_arstrCoord.GetAt(i).GetString();
		TRACE(readtmp);
		TRACE(_T("\n"));

		double x = 0;
		double y = 0;
		FindCustomXY(m_arstrCoord.GetAt(i),&x,&y);
		if (x == 0 || y == 0)
		{
			continue;//这行数据无效
		}
		if (nStartX == 0 && nStartY == 0)
		{//说明这行是当前要素的开始
			nStartX = x;
			nStartY = y;
		}else if (fabs(nStartX - x) < 0.00000001 && fabs(nStartY - y) < 0.00000001)
		{//如果startx，straty都不为零且与当前点相等，说明这行是当前要素的结束，保存


			if (ipPointCollection != NULL)
			{//写进最后一个点，并写入图形中
				IPointPtr ipPoint(CLSID_Point);
				ipPoint->PutCoords(x,y);
				ipPointCollection->AddPoint(ipPoint);

				CComQIPtr<IPointCollection> spPointCollection = ipPointCollection;
				CComQIPtr<IRing> spRing;
				hr = GetRing(spPointCollection,spRing);
				if (hr != S_OK) return hr;
				ipGeometryCollection->AddGeometry(spRing);
				ipPolyGon->Close();
			}				
			//设置拓扑和坐标规则
			ITopologicalOperatorPtr ipTop;
			ipTop = ipPolyGon;
			hr = ipTop->Simplify();

			ISpatialReferencePtr ipSpat;
			IGeoDatasetPtr ipGeoDataset;
			ipGeoDataset = ipFtClass;
			ipGeoDataset->get_SpatialReference(&ipSpat);
			//CreateSpatial(40);
			hr = ipPolyGon->putref_SpatialReference(m_spSpatialRederence);
			ipPolyGon->Project(ipSpat);

			VARIANT_BOOL bIsEmpty;
			ipPolyGon->get_IsEmpty(&bIsEmpty);
			if (bIsEmpty == VARIANT_FALSE)
			{
				ipFtClass->CreateFeature(&ipFt);
				hr = ipFt->putref_Shape(ipPolyGon);
				CComQIPtr<IFeature> spFt = ipFt;
				hr = AddFields(spFt);
				hr = ipFt->Store();	
				spAllGeometryCollection->AddGeometry(ipPolyGon);

				//如果没到结尾，创建下一个图形
				if (m_arstrCoord.GetCount() - 1 - i > 2)
				{

					//重置
					ipFt = NULL;
					ipPolyGon = NULL;
					ipGeometry = NULL;
					ipGeometryCollection = NULL;

					ipPolyGon.CreateInstance(CLSID_Polygon);
					ipGeometry = ipPolyGon;
					hr = ipGeometry->QueryInterface(&ipGeometryCollection);
				}
			}
			//重置
			ipPointCollection = NULL;
			bIsGeoStart = TRUE;
			nStartX = 0;
			nStartY = 0;
			continue;//这行数据以及写入过了，不需要做后面的操作
		}else if (m_arstrCoord.GetAt(i).IsEmpty() == TRUE)//空行，是一个要素的结束
		{
			if (ipPointCollection != NULL)
			{//写进最后一个点，并写入图形中
				CComQIPtr<IPointCollection> spPointCollection = ipPointCollection;
				CComQIPtr<IRing> spRing;
				hr = GetRing(spPointCollection,spRing);
				if (hr != S_OK) return hr;
				ipGeometryCollection->AddGeometry(spRing);
				ipPolyGon->Close();
			}				
			//设置拓扑和坐标规则
			ITopologicalOperatorPtr ipTop;
			ipTop = ipPolyGon;
			hr = ipTop->Simplify();

			ISpatialReferencePtr ipSpat;
			IGeoDatasetPtr ipGeoDataset;
			ipGeoDataset = ipFtClass;
			ipGeoDataset->get_SpatialReference(&ipSpat);
			//CreateSpatial(40);
			hr = ipPolyGon->putref_SpatialReference(m_spSpatialRederence);
			ipPolyGon->Project(ipSpat);

			VARIANT_BOOL bIsEmpty;
			ipPolyGon->get_IsEmpty(&bIsEmpty);
			if (bIsEmpty == VARIANT_FALSE)
			{
				ipFtClass->CreateFeature(&ipFt);
				hr = ipFt->putref_Shape(ipPolyGon);
				CComQIPtr<IFeature> spFt = ipFt;
				hr = AddFields(spFt);
				hr = ipFt->Store();	
				spAllGeometryCollection->AddGeometry(ipPolyGon);

				//如果没到结尾，创建下一个图形
				if (m_arstrCoord.GetCount() - 1 - i > 2)
				{

					//重置
					ipFt = NULL;
					ipPolyGon = NULL;
					ipGeometry = NULL;
					ipGeometryCollection = NULL;

					ipPolyGon.CreateInstance(CLSID_Polygon);
					ipGeometry = ipPolyGon;
					hr = ipGeometry->QueryInterface(&ipGeometryCollection);
				}
			}
			//重置
			ipPointCollection = NULL;
			bIsGeoStart = TRUE;
			nStartX = 0;
			nStartY = 0;
			continue;//这行数据以及写入过了，不需要做后面的操作
		}
		if (bIsGeoStart == TRUE)//如果是开始一个图形
		{
			if (ipPointCollection != NULL)//如果不是第一个图形，保存上一个
			{
				CComQIPtr<IPointCollection> spPointCollection = ipPointCollection;
				CComQIPtr<IRing> spRing;
				hr = GetRing(spPointCollection,spRing);
				if (hr != S_OK) return hr;
				bIsGeoStart = FALSE;
				nStartX = 0;
				nStartY = 0;
				ipPointCollection = NULL;
				ipPointCollection.CreateInstance(CLSID_Multipoint);
			}else//或者刚开始
			{
				ipPointCollection.CreateInstance(CLSID_Multipoint);
				bIsGeoStart = FALSE;
				//创建图形
				ipPolyGon.CreateInstance(CLSID_Polygon);
				ipGeometry = ipPolyGon;
				hr = ipGeometry->QueryInterface(&ipGeometryCollection);
			}
		}
		//写入当前读取的点
		IPointPtr ipPoint(CLSID_Point);
		ipPoint->PutCoords(x,y);
		ipPointCollection->AddPoint(ipPoint);


	}
	//添加判断，结束时最后一个要素的对象肯定存在，检查信息是否正确
	if (ipFt == NULL)
	{
		BCGPMessageBox(_T("结束点缺失！\r\n当前界址点直到文件结尾仍不能形成图形！"),_T("错误"),MB_OK);
		return FALSE;
	}
	if (m_ipMapCtrl != NULL)
	{
		//缩放
		CComQIPtr<IActiveView> spActiveview;
		CComQIPtr<IMap> spMap;
		m_ipMapCtrl->get_Map(&spMap);
		spActiveview = spMap;
		CComQIPtr<IEnvelope> spEnvelope;
		spAllGeometry->get_Envelope(&spEnvelope);
		spActiveview->put_Extent(spEnvelope);
		m_ipMapCtrl->Refresh(esriViewAll);
		//下面是闪烁
		CComQIPtr<ISpatialReference> spSpatial;
		CComBSTR bstrName;
		spAllGeometry->get_SpatialReference(&spSpatial);
		spSpatial->get_Name(&bstrName);
		ISpatialReferencePtr ipSpat;
		CComQIPtr<IGeoDataset> ipGeoDataset;
		ipGeoDataset = m_spFeatureClass;
		ipGeoDataset->get_SpatialReference(&ipSpat);
		spAllGeometry->Project(ipSpat);
		FlashShapes(m_ipMapCtrl,spAllGeometry);
	}
	return TRUE;
}

HRESULT CImpCoordDlg::PolygonCutHollow(CComQIPtr<IPointCollection> spPointCollection, CComQIPtr<IGeometry> spGeometry)
{
	HRESULT hr;
	CComQIPtr<IPolygon> spPolyGon;
	CComQIPtr<IGeometry> spCutGeometry;
	CComQIPtr<IGeometryCollection> spGeometryCollection;

	spPolyGon.CoCreateInstance(CLSID_Polygon);
	spCutGeometry = spPolyGon;
	spCutGeometry->QueryInterface(&spGeometryCollection);

	//先成环,然后切割
	CComQIPtr<IRing> spRing;
	hr = GetRing(spPointCollection,spRing);
	if (hr != S_OK) return hr;
	hr = spGeometryCollection->AddGeometry(spRing);
	spPolyGon->Close();

	VARIANT_BOOL bIsEmpty;
	spPolyGon->get_IsEmpty(&bIsEmpty);

	CComQIPtr<ITopologicalOperator> spTopOper;
	spTopOper = spGeometry;
	CComQIPtr<IGeometry> ipResultGeo;
	hr = spTopOper->Difference(spPolyGon, &ipResultGeo);

	ipResultGeo->get_IsEmpty(&bIsEmpty);
	if (bIsEmpty == VARIANT_FALSE)
	{
		spGeometry = ipResultGeo;
	}

	return S_OK;
}

HRESULT CImpCoordDlg::SaveFeature(CComQIPtr<IGeometry> spGeometry, CComQIPtr<IFeature> &spFt)
{
	HRESULT hr;
	//设置拓扑和坐标规则
	CComQIPtr<ITopologicalOperator> spTop;
	spTop = spGeometry;
	hr = spTop->Simplify();

	ISpatialReferencePtr ipSpat;
	CComQIPtr<IGeoDataset> ipGeoDataset;
	ipGeoDataset = m_spFeatureClass;
	ipGeoDataset->get_SpatialReference(&ipSpat);
	hr = spGeometry->putref_SpatialReference(m_spSpatialRederence);
	hr = spGeometry->Project(ipSpat);

	VARIANT_BOOL bIsEmpty;
	spGeometry->get_IsEmpty(&bIsEmpty);
	if (bIsEmpty == VARIANT_TRUE)
	{
		return E_FAIL;
	}

	//只有当图形不为空时，赋值						
	hr = spFt->putref_Shape(spGeometry);
	hr = AddFields(spFt);
	hr = spFt->Store();	

	return S_OK;
}

HRESULT CImpCoordDlg::GetRing(CComQIPtr<IPointCollection> spPointCollection,CComQIPtr<IRing> &spRing)
{
	HRESULT hr;
	if (spRing == NULL)
	{
		spRing.CoCreateInstance(CLSID_Ring);
	}else
	{
		spRing.Release();
		spRing.CoCreateInstance(CLSID_Ring);
	}
	CComQIPtr<IPointCollection> spPointCol(spRing);
	spPointCol->AddPointCollection(spPointCollection);
	hr = spRing->Close();
	return hr;
}

HRESULT CImpCoordDlg::AddFields(CComQIPtr<IFeature> &spFt, CMapStringToString *mapFieldAndValue)
{
	if (m_eCoordType == Country || m_eCoordType == Country_P)
	{
		return S_OK;
	}
	//截断文件路径，仅保留文件名

	CString strFileName;
	strFileName = m_strOpenFilePath.Right(m_strOpenFilePath.GetLength() - m_strOpenFilePath.ReverseFind('\\') - 1);
	long nIndex = -1;
	m_spFeatureClass->FindField(_T("FILENAME"),&nIndex);
	if (nIndex > 0)
	{
		spFt->put_Value(nIndex,_variant_t(_bstr_t(strFileName)));
	}

	//将OID赋值给DK_MC
	CComBSTR bstrOIDFieldName;
	CComVariant vtOID;
	m_spFeatureClass->get_OIDFieldName(&bstrOIDFieldName);
	m_spFeatureClass->FindField(bstrOIDFieldName,&nIndex);
	if (nIndex >= 0)
	{
		spFt->get_Value(nIndex,&vtOID);
	}
	m_spFeatureClass->FindField(_T("DK_MC"),&nIndex);
	if (nIndex > 0)
	{
		CString strMC;
		strMC.Format(_T("地块%s"),VariantToString(vtOID));
		spFt->put_Value(nIndex,_variant_t(_bstr_t(strMC)));
	}
	return S_OK;
}

void CImpCoordDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CGSBCGDialog::OnCancel();
	CGSBCGDialog::DestroyWindow();
	//delete this;
}

void CImpCoordDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CGSBCGDialog::OnClose();
	CGSBCGDialog::DestroyWindow();
	//delete this;
}

void CImpCoordDlg::FlashShapes(CComQIPtr<IMapControl2> ipMapControl, CComQIPtr<IGeometry> ipGeo )
{
	if (ipGeo == NULL)	return;

	IActiveViewPtr ipActiveView;
	ipMapControl->get_ActiveView(&ipActiveView);
	ipActiveView->Refresh();
	IMapPtr ipMap;
	ipActiveView->get_FocusMap(&ipMap);
	IScreenDisplayPtr ipScreenDisp;
	ipActiveView->get_ScreenDisplay(&ipScreenDisp);
	OLE_HANDLE hdc;
	ipScreenDisp->get_hDC(&hdc);

	// 为了提高绘制效率, 先在CacheMemDC里绘, 然后再绘制到ScreenDisplay上
	short nMemCacheID;
	ipScreenDisp->AddCache(&nMemCacheID);
	OLE_HANDLE hMemDC;
	ipScreenDisp->get_CacheMemDC(nMemCacheID, &hMemDC);

	// 设置symbol
	IColorPtr ipRgbColor(CLSID_RgbColor);
	ISimpleFillSymbolPtr ipStylePg(CLSID_SimpleFillSymbol);
	ISimpleLineSymbolPtr ipStyLn(CLSID_SimpleLineSymbol);
	//边界样式
	ipRgbColor->put_RGB(RGB(255, 123, 128));
	ipStyLn->put_Style(esriSLSSolid);
	ipStyLn->put_Color(ipRgbColor);
	ipStyLn->put_Width(4);
	//多边形样式
	ipStylePg->put_Color(ipRgbColor);
	ipStylePg->put_Style(esriSFSBackwardDiagonal);
	ipStylePg->put_Outline(ipStyLn);

	ISymbolPtr ipSymbol = ipStylePg;

	// 绘图形
	double dPointBufferDis = 6;
	double dLineBufferDis = 2;

	IDisplayTransformationPtr ipDisplayTransformation;
	ipScreenDisp->get_DisplayTransformation(&ipDisplayTransformation);
	ipSymbol->SetupDC(hMemDC, ipDisplayTransformation);

	//先更新屏幕到缩放后的位置
	ipScreenDisp->UpdateWindow();
	//edit by lcc 2016.12.14 修改行政区缩放逻辑
	esriGeometryType eShapeType;
	ipGeo->get_GeometryType(&eShapeType);

	CComQIPtr<IGeometryCollection> spGeoCollection;
	ipGeo->QueryInterface(&spGeoCollection);
	long nCount;
	spGeoCollection->get_GeometryCount(&nCount);
	for (long i = 0;i < nCount; i++)
	{
		CComQIPtr<IGeometry> ipShape, ipBufferShape;
		spGeoCollection->get_Geometry(i, &ipShape);
		ipBufferShape = ipShape;
		esriGeometryType eShapeType;
		ipShape->get_GeometryType(&eShapeType);
		if (eShapeType != esriGeometryPolygon)
		{
			if (eShapeType == esriGeometryRing)
			{
				ConvertLow2High(ipShape,ipBufferShape);
			}else
			{
				double dBufferDis = 0;
				if (eShapeType == esriGeometryPoint)
					dBufferDis = dPointBufferDis;
				else if (eShapeType == esriGeometryPolyline)
					dBufferDis = dLineBufferDis;

				CComQIPtr<ITopologicalOperator> ipTopOper = ipShape;
				ipTopOper->Buffer(dBufferDis, &ipBufferShape);
			}
		}
		ipSymbol->Draw(ipBufferShape);
	}

	//遍历集合
	ipSymbol->ResetDC();
	ipScreenDisp->DrawCache(hdc, nMemCacheID, NULL, NULL);
	ipScreenDisp->RemoveCache(nMemCacheID);
	// 延时100毫秒擦掉绘制
	::Sleep(300);
	ipActiveView->PartialRefresh(esriViewAll, NULL, NULL);

	//edit by lcc 2016.12.14 end

}
void CImpCoordDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CGSBCGDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
	((CWnd*)GetDlgItem(IDC_CHECKTYPE))->SetFocus();
}

HRESULT CImpCoordDlg::ConvertLow2High(CComQIPtr<IGeometry> spLowGeometry,CComQIPtr<IGeometry> spHighGeometry)
{
	if (spLowGeometry == NULL) return E_FAIL;
	CComQIPtr<IGeometryCollection> spGeomColNew;
	CComQIPtr<ISegmentCollection> spSegCln;
	CComQIPtr<ISegmentCollection> spgClnNew; 
	CComQIPtr<IClone> spClone;
	CComQIPtr<IClone> spNewClone;
	CComQIPtr<IPolygon> spPolygon;
	CComQIPtr<IRing2> spRing;
	esriGeometryType eType;
	spLowGeometry->get_GeometryType(&eType);
	switch (eType)
	{
	case esriGeometryPoint:
	case esriGeometryPolyline:
	case esriGeometryPolygon:
		 spClone = spLowGeometry;
		 spClone->Clone(&spNewClone);
		 spHighGeometry = spNewClone;
		 return S_OK;
		 break;
// 	case esriGeometryEnvelope:
// 		segCln = new Polygon() as ISegmentCollection;
// 		segCln.SetRectangle(geometry as IEnvelope);
// 		IPolygon polygon = segCln as IPolygon;
// 		return polygon;
// 	case esriGeometryPath:
// 		geomColNew = new Polyline() as IGeometryCollection;
// 		IPath path = geometry as IPath;
// 		segCln = path as ISegmentCollection;
// 		segClnNew = new Path() as ISegmentCollection;
// 		break;
	case esriGeometryRing:
		spPolygon.CoCreateInstance(CLSID_Polygon);
		spGeomColNew = spPolygon;
		spRing = spLowGeometry;
		spSegCln = spRing;
		spgClnNew.CoCreateInstance(CLSID_Ring);
		break;
// 	case esriGeometryLine:
// 		ILine line = geometry as ILine;
// 		IPolyline polyline = new Polyline() as IPolyline;
// 		polyline.FromPoint = line.FromPoint;
// 		polyline.ToPoint = line.ToPoint;
// 		polyline.Project(geometry.SpatialReference);
// 		return polyline;
	default:
		return E_FAIL;
		break;
	}
	if (spSegCln != NULL)
	{
		long nCount;
		spSegCln->get_SegmentCount(&nCount);
		for (int k = 0; k < nCount; k++)
		{
			CComQIPtr<esriISegment> spSeg;
			spSegCln->get_Segment(k,&spSeg);
			spgClnNew->AddSegment(spSeg);
		}
	}
	CComQIPtr<IGeometry> spGeoTmp = spgClnNew;
	if (spGeomColNew != NULL) spGeomColNew->AddGeometry(spGeoTmp);
	spHighGeometry.CoCreateInstance(CLSID_Polygon);
	spHighGeometry = spGeomColNew;
	CComQIPtr<ISpatialReference> spSpatial; 
	spLowGeometry->get_SpatialReference(&spSpatial);
	CComBSTR bstrName;
	spSpatial->get_Name(&bstrName);
	spHighGeometry->putref_SpatialReference(spSpatial);
	return S_OK;
}
void CImpCoordDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	CGSBCGDialog::PostNcDestroy();
	delete this;
}

BOOL CImpCoordDlg::CheckIsCountryTypeFile()
{
	if (m_arstrCoord.GetCount() == 0) return FALSE;
	BOOL bFindSXMS = FALSE;
	BOOL bFindDKZB = FALSE;
	BOOL bFindComma = FALSE;
	CString strWhere(_T(","));
	for (int i = 0; i < m_arstrCoord.GetCount(); i++)
	{
		CString strPoint = m_arstrCoord.GetAt(i);
		if (strPoint.Compare(_T("[属性描述]")) == 0) bFindSXMS = TRUE;
		if (strPoint.Compare(_T("[地块坐标]")) == 0) bFindDKZB = TRUE;
		if (FindCharNum(strPoint,strWhere) == 8) bFindComma = TRUE;
		if (bFindSXMS == TRUE && bFindDKZB == TRUE && bFindComma == TRUE)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CImpCoordDlg::CheckIsCountry_PTypeFile()
{
	if (m_arstrCoord.GetCount() == 0) return FALSE;
	BOOL bFindSXMS = FALSE;
	BOOL bFindDKZB = FALSE;
	BOOL bFindComma = FALSE;
	CString strWhere(_T(","));
	for (int i = 0; i < m_arstrCoord.GetCount(); i++)
	{
		CString strPoint = m_arstrCoord.GetAt(i);
		if (strPoint.Compare(_T("[属性描述]")) == 0) bFindSXMS = TRUE;
		if (strPoint.Compare(_T("[地块坐标]")) == 0) bFindDKZB = TRUE;
		if (FindCharNum(strPoint,strWhere) == 10) bFindComma = TRUE;
		if (bFindSXMS == TRUE && bFindDKZB == TRUE && bFindComma == TRUE)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CImpCoordDlg::CheckIsProvinceTypeFile()
{
	if (m_arstrCoord.GetCount() == 0) return FALSE;
	for (int i = 0; i < m_arstrCoord.GetCount(); i++)
	{
		CString strPoint = m_arstrCoord.GetAt(i);
		int nIndex = strPoint.Find(_T("1,1,1,"));
		double dx,dy;
		int Unused1 = 0;
		int Unused2 = 0;
		int Unused3 = 0;
		int Unused4 = 0;
		FindProvinceXY(strPoint,&Unused1,&Unused2,&Unused3,&Unused4,&dx,&dy);
		if (nIndex >= 0 && dx != 0 && dy != 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CImpCoordDlg::CheckIsGisqCustomTypeFile()
{
	if (m_arstrCoord.GetCount() == 0) return FALSE;
	//第一行能够匹配特征字符串，则确认为系统导出
	if (m_arstrCoord.GetAt(0).Find(GisqCustomText) >= 0) return TRUE;
	return FALSE;
}


BOOL CImpCoordDlg::CheckCustomTypeFile()
{
	if (m_arstrCoord.GetCount() == 0) return FALSE;
	for(int i = 0;i < m_arstrCoord.GetCount(); i++)
	{
		m_nSpatailNum = GetSpatialNum(m_arstrCoord.GetAt(i));
		if (m_nSpatailNum > 0) break;
	}
	if (m_nSpatailNum > 0)
	{
		m_strShowFileType.Format(_T("文件格式：非标准坐标,带号%d"),m_nSpatailNum);
	}else
	{
		m_strShowFileType = _T("文件格式：非标准坐标，但无法正确获取带号！");
	}
	ShowFileLine();
	return TRUE;
}

void CImpCoordDlg::CheckCoordType()
{
	if (CheckIsCountryTypeFile())
	{
		m_eCoordType = Country;
		for(int i = 13;i < m_arstrCoord.GetCount(); i++)
		{
			m_nSpatailNum = GetSpatialNum(m_arstrCoord.GetAt(i));
			if (m_nSpatailNum > 0) break;
		}
		if (m_nSpatailNum > 0)
		{
			m_strShowFileType.Format(_T("文件格式：部标,带号%d"),m_nSpatailNum);
		}else
		{
			m_strShowFileType = _T("文件格式：部标，但无法正确获取带号！");
		}
		ShowFileLine();
		return;
	}
	if (CheckIsCountry_PTypeFile())
	{
		m_eCoordType = Country_P;
		for(int i = 13;i < m_arstrCoord.GetCount(); i++)
		{
			m_nSpatailNum = GetSpatialNum(m_arstrCoord.GetAt(i));
			if (m_nSpatailNum > 0) break;
		}
		if (m_nSpatailNum > 0)
		{
			m_strShowFileType.Format(_T("文件格式：报省部标,带号%d"),m_nSpatailNum);
		}else
		{
			m_strShowFileType = _T("文件格式：报省部标，但无法正确获取带号！");
		}
		ShowFileLine();
		return;
	}
	if (CheckIsProvinceTypeFile())
	{
		m_eCoordType = Province;
		for (int i = 0; i < m_arstrCoord.GetCount(); i++)
		{
			m_nSpatailNum = GetSpatialNum(m_arstrCoord.GetAt(i));
			if (m_nSpatailNum > 0) break;
		}
		if (m_nSpatailNum > 0)
		{
			m_strShowFileType.Format(_T("文件格式：省厅标准,带号%d"),m_nSpatailNum);
		}else
		{
			m_strShowFileType = _T("文件格式：省厅标准，但无法正确获取带号！");
		}
		ShowFileLine();
		return;
	}
	if (CheckIsGisqCustomTypeFile())
	{
		m_eCoordType = GisqCustom;
		for (int i = 0; i < m_arstrCoord.GetCount(); i++)
		{
			m_nSpatailNum = GetSpatialNum(m_arstrCoord.GetAt(i));
			if (m_nSpatailNum > 0) break;
		}
		if (m_nSpatailNum > 0)
		{
			m_strShowFileType.Format(_T("文件格式：杰思科简单坐标,带号%d"),m_nSpatailNum);
		}else
		{
			m_strShowFileType = _T("文件格式：杰思科简单坐标，但无法正确获取带号！");
		}
		ShowFileLine();
		return;

	}
	//如果能到这里说明不是标准格式
	m_eCoordType = Custom;
	m_strShowFileType = _T("文件格式：未知格式，请手动设置");
	ShowFileLine();
	return;
}

int CImpCoordDlg::FindCharNum(CString &strTargetString, CString &strWhere)
{
	int nCount = 0;
	int nIndex = 0;
	while (nIndex >= 0)
	{
		nIndex = strTargetString.Find(strWhere,nIndex);
		if (nIndex >= 0)
		{
			nIndex++;
			nCount++;
		}
	} 
	return nCount;
}

HRESULT CImpCoordDlg::AddFieldsToFeature(CString &strInfo, CComQIPtr<IFeature> spFt)
{
	if (spFt == NULL) return E_FAIL;
	CComQIPtr<IObjectClass> spObjClass;
	CComQIPtr<IFeatureClass> spFtClass;
	spFt->get_Class(&spObjClass);
	spFtClass = spObjClass;
	if (spFtClass == NULL) return E_FAIL;

	if (strInfo.IsEmpty() == true) return E_FAIL;
	CString strFields(strInfo);

	CArray<CString,CString> strarrFields;
	int nIndex = 0;
	nIndex = strFields.Find(_T(","));
	while(nIndex>=0)
	{
		CString strTmp = strFields.Left(nIndex);
		strarrFields.Add(strTmp);
		strFields = strFields.Right(strFields.GetLength() - nIndex - 1);
		nIndex = strFields.Find(_T(","));
	}
	//去掉@，加入最后一个
	strFields = strFields.Left(strFields.GetLength() - 2);
	strarrFields.Add(strFields);
	long nFieldIndex = -1;
	if (m_eCoordType == Country)
	{
		//8个属性
		strarrFields.GetAt(0);//界址点数

		//面积
		spFtClass->FindField(_T("DK_MJ"),&nFieldIndex);
		if (nFieldIndex >= 0)
		{
			spFt->put_Value(nFieldIndex,_variant_t(strarrFields.GetAt(1)));
		}
		//地块编号
		spFtClass->FindField(_T("DK_BH"),&nFieldIndex);
		if (nFieldIndex >= 0)
		{
			spFt->put_Value(nFieldIndex,_variant_t(strarrFields.GetAt(2)));
		}
		//地块名称
		spFtClass->FindField(_T("DK_MC"),&nFieldIndex);
		if (nFieldIndex >= 0)
		{
			if (strarrFields.GetAt(3).IsEmpty() == true)
			{
				long nOIDIndex;
				CComBSTR bstrOIDFieldName;
				CComVariant vtOID;
				m_spFeatureClass->get_OIDFieldName(&bstrOIDFieldName);
				m_spFeatureClass->FindField(bstrOIDFieldName,&nOIDIndex);
				if (nOIDIndex >= 0)
				{
					spFt->get_Value(nOIDIndex,&vtOID);
					CString strMC;
					strMC.Format(_T("地块%s"),VariantToString(vtOID));
					spFt->put_Value(nFieldIndex,_variant_t(_bstr_t(strMC)));
				}
			}else
			{
				spFt->put_Value(nFieldIndex,_variant_t(strarrFields.GetAt(3)));
			}
		}

		strarrFields.GetAt(4);//面

		//图幅号
		spFtClass->FindField(_T("TFH"),&nFieldIndex);
		if (nFieldIndex >= 0)
		{
			spFt->put_Value(nFieldIndex,_variant_t(strarrFields.GetAt(5)));
		}
		//地块用途
		spFtClass->FindField(_T("DK_YT"),&nFieldIndex);
		if (nFieldIndex >= 0)
		{
			spFt->put_Value(nFieldIndex,_variant_t(strarrFields.GetAt(6)));
		}
		//地类编码
		spFtClass->FindField(_T("DL_BM"),&nFieldIndex);
		if (nFieldIndex >= 0)
		{
			spFt->put_Value(nFieldIndex,_variant_t(strarrFields.GetAt(7)));
		}
	}else if (m_eCoordType == Country_P)
	{
		//10个属性
		strarrFields.GetAt(0);//界址点数

		//面积
		spFtClass->FindField(_T("DK_MJ"),&nFieldIndex);
		if (nFieldIndex >= 0)
		{
			spFt->put_Value(nFieldIndex,_variant_t(strarrFields.GetAt(1)));
		}
		//乡镇代码
		spFtClass->FindField(_T("XZDM"),&nFieldIndex);
		if (nFieldIndex >= 0)
		{
			spFt->put_Value(nFieldIndex,_variant_t(strarrFields.GetAt(2)));
		}
		//行政区名称
		spFtClass->FindField(_T("XZQMC"),&nFieldIndex);
		if (nFieldIndex >= 0)
		{
			spFt->put_Value(nFieldIndex,_variant_t(strarrFields.GetAt(3)));
		}
		//地块编号
		spFtClass->FindField(_T("DK_BH"),&nFieldIndex);
		if (nFieldIndex >= 0)
		{
			spFt->put_Value(nFieldIndex,_variant_t(strarrFields.GetAt(4)));
		}
		//地块名称
		spFtClass->FindField(_T("DK_MC"),&nFieldIndex);
		if (nFieldIndex >= 0)
		{
			if (strarrFields.GetAt(5).IsEmpty() == true)
			{
				long nOIDIndex;
				CComBSTR bstrOIDFieldName;
				CComVariant vtOID;
				m_spFeatureClass->get_OIDFieldName(&bstrOIDFieldName);
				m_spFeatureClass->FindField(bstrOIDFieldName,&nOIDIndex);
				if (nOIDIndex >= 0)
				{
					spFt->get_Value(nOIDIndex,&vtOID);
					CString strMC;
					strMC.Format(_T("地块%s"),VariantToString(vtOID));
					spFt->put_Value(nFieldIndex,_variant_t(_bstr_t(strMC)));
				}
			}else
			{
				spFt->put_Value(nFieldIndex,_variant_t(strarrFields.GetAt(5)));
			}
		}
		
		strarrFields.GetAt(6);//面

		//图幅号
		spFtClass->FindField(_T("TFH"),&nFieldIndex);
		if (nFieldIndex >= 0)
		{
			spFt->put_Value(nFieldIndex,_variant_t(strarrFields.GetAt(7)));
		}
		//地块用途
		spFtClass->FindField(_T("DK_YT"),&nFieldIndex);
		if (nFieldIndex >= 0)
		{
			spFt->put_Value(nFieldIndex,_variant_t(strarrFields.GetAt(8)));
		}
		//地类编码
		spFtClass->FindField(_T("DL_BM"),&nFieldIndex);
		if (nFieldIndex >= 0)
		{
			spFt->put_Value(nFieldIndex,_variant_t(strarrFields.GetAt(9)));
		}
	}

	CString strFileName;
	strFileName = m_strOpenFilePath.Right(m_strOpenFilePath.GetLength() - m_strOpenFilePath.ReverseFind('\\') - 1);
	m_spFeatureClass->FindField(_T("FILENAME"),&nFieldIndex);
	if (nFieldIndex > 0)
	{
		spFt->put_Value(nFieldIndex,_variant_t(_bstr_t(strFileName)));
	}
	return S_OK;
}

int CImpCoordDlg::GetWorkspaceSpatial(CComQIPtr<ISpatialReference> &spSpatial)
{
	if (spSpatial != NULL)
	{
		spSpatial.Release();
	}
	if (m_spFeatureClass == NULL)
	{
		CComQIPtr<IMapControl2> spMapCtrl = (CComQIPtr<IMapControl2>)m_ipFramework->GetActiveControl();
		CComQIPtr<IMap> spMap;
		if (spMapCtrl != NULL)
		{
			spMapCtrl->get_Map(&spMap);
		}
		CComQIPtr<IUID> spUID;
		spUID.CoCreateInstance(CLSID_UID);
		CComQIPtr<IEnumLayer> spEnumLayer;
		spUID->put_Value(_variant_t("{40A9E885-5533-11d0-98BE-00805F7CED21}"));
		spMap->get_Layers(spUID,VARIANT_TRUE,&spEnumLayer);
		if (spEnumLayer == NULL) return 0;
		CComQIPtr<ILayer> spLayer;

		spEnumLayer->Next(&spLayer);
		while(spLayer != NULL)
		{
			CComQIPtr<IFeatureLayer> spFtLayer = spLayer;
			if (m_spFeatureClass != NULL) m_spFeatureClass.Release();
			spFtLayer->get_FeatureClass(&m_spFeatureClass);
			if (m_spFeatureClass == NULL)
			{
				spLayer.Release();
				spEnumLayer->Next(&spLayer);
				continue;
			}
			CComBSTR bstrName;
			spLayer->get_Name(&bstrName);
			if (bstrName == _T("辅助面"))
			{
				break;
			}

			spLayer.Release();
			spEnumLayer->Next(&spLayer);
		}
		if (m_spFeatureClass == NULL)
		{
			MessageBox(_T("请匹配辅助面数据源"),_T("错误"),MB_OK);
			return 0;
		}
	}

	CComQIPtr<IGeoDataset> spGeoDataset = m_spFeatureClass;
	spGeoDataset->get_SpatialReference(&m_spWorkspaceSpatial);
	m_nWorkspacesSpatialNum = SpatialReferenceToInt(m_spWorkspaceSpatial);
	return 0;
}

int CImpCoordDlg::SpatialReferenceToInt(CComQIPtr<ISpatialReference> spSpatial)
{
	CComBSTR bstrName;
	spSpatial->get_Name(&bstrName);
	if (bstrName == _T("esriSRProjCS_Xian1980_3_Degree_GK_Zone_39"))
	{
		return 39;
	}
	if (bstrName == _T("esriSRProjCS_Xian1980_3_Degree_GK_Zone_40"))
	{
		return 40;
	}
	if (bstrName == _T("esriSRProjCS_Xian1980_3_Degree_GK_Zone_41"))
	{
		return 41;
	} 
	if (bstrName == _T("esriSRProjCS_Xian1980_3_Degree_GK_CM_120E"))
	{
		return 120;
	}
	if (bstrName == _T("esriSRProjCS_Xian1980_3_Degree_GK_CM_117E"))
	{
		return 117;
	}
	if (bstrName == _T("esriSRProjCS_Xian1980_3_Degree_GK_CM_123E"))
	{
		return 123;
	}
	if (bstrName == _T("esriSRProjCS_Xian1980"))
	{
		return 80;
	}
	return -1;
}

int CImpCoordDlg::BigToSmall(int nBig)
{
	return nBig / 3;
}

int CImpCoordDlg::SmallToBig(int nSmall)
{
	return nSmall * 3;
}


void CImpCoordDlg::OnEnChangeEditsymbol()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CGSBCGDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CheckCustomTypeFile();
}

void CImpCoordDlg::OnEnChangeEditxtype()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CGSBCGDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strX;
	m_XEditCtrl.GetWindowText(strX);
	if (strX.IsEmpty() == true)
	{
		m_nXType = -1;
	}else
	{
		m_nXType = _wtof(strX);
	}
	CheckCustomTypeFile();
}

void CImpCoordDlg::OnEnChangeEditytype()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CGSBCGDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strY;
	m_YEditCtrl.GetWindowText(strY);
	if (strY.IsEmpty() == true)
	{
		m_nYType = -1;
	}else
	{
		m_nYType = _wtof(strY);
	}
	CheckCustomTypeFile();
}
