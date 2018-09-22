// ExpCoordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GisqExpCoordCmd.h"
#include "ExpCoordDlg.h"


// CExpCoordDlg 对话框

IMPLEMENT_DYNAMIC(CExpCoordDlg, CBCGPDialog)

CExpCoordDlg::CExpCoordDlg(GisqLib::_DGisqFrameworkCOMPtr ipFramework, CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CExpCoordDlg::IDD, pParent)
	, m_sExpPath(_T(""))
	, m_nExpPrecision(4)
{
	m_ipFramework = ipFramework;
	m_eCoordType = nulltype;
	m_spSpatialRederence = (IUnknown*)NULL;
	m_nProvinceFtIndex = 1;
	m_nProvincePointIndex = 1;
}

CExpCoordDlg::~CExpCoordDlg()
{
}

void CExpCoordDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SAVEPATH, m_sExpPath);
	DDX_Text(pDX, IDC_EDIT_NUM, m_nExpPrecision);
	DDX_Control(pDX, IDC_COMBO_LAYERLIST, m_LayerSelect);
	DDX_Control(pDX, IDC_EDIT_SAVEPATH, m_wndSavePathEdit);
	DDX_Control(pDX, IDC_EDIT_NUM, m_wndExpPrecision);
}

BEGIN_MESSAGE_MAP(CExpCoordDlg, CBCGPDialog)
	ON_BN_CLICKED(IDOK, &CExpCoordDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SELECTIONINLAYER, &CExpCoordDlg::OnBnClickedSelectioninlayer)
	ON_BN_CLICKED(IDC_SELECTLAYER, &CExpCoordDlg::OnBnClickedSelectlayer)
	ON_BN_CLICKED(IDC_RDBCOUNTRY, &CExpCoordDlg::OnBnClickedRdbcountry)
	ON_BN_CLICKED(IDC_RDBPROVINCE, &CExpCoordDlg::OnBnClickedRdbprovince)
	ON_BN_CLICKED(IDC_RDBCONSTOM, &CExpCoordDlg::OnBnClickedRdbconstom)
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_RDBCOUNTRY_P, &CExpCoordDlg::OnBnClickedRdbcountryP)
END_MESSAGE_MAP()

BOOL CExpCoordDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	m_wndExpPrecision.SetValidChars(_T("1234567890"));//可用字符列表
	//设置“导出选中图层中选中的要素”单选框为选中
	TCHAR szDesktopFullPath[MAX_PATH];
	DWORD dBufferSize = sizeof(szDesktopFullPath)/sizeof(CHAR);
	SHGetSpecialFolderPath(NULL,szDesktopFullPath,CSIDL_DESKTOPDIRECTORY,NULL);
	CString strDesktopPath(szDesktopFullPath);
	CString strConfigFile = strDesktopPath + _T("\\地块界址点.txt");
	m_wndSavePathEdit.EnableFileBrowseButton(strConfigFile,_T("TXT(*.txt)|*.txt||"));
	m_sExpPath = strConfigFile;
	m_wndSavePathEdit.EnableBrowseButton();

	((CButton *)GetDlgItem(IDC_RDBCOUNTRY))->SetCheck(TRUE);
	m_eCoordType = Country;

	m_ipMapCtrl = m_ipFramework->GetActiveControl();
	CComQIPtr<GisqLib::IGSDataExchangeS> spGSDataExchangeS = m_ipFramework->GetDataExchangeS();
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

	CComQIPtr<IGeometry> spGeoTmp;
	CComQIPtr<IMap> spMap;
	m_ipMapCtrl->get_Map(&spMap);
	CComQIPtr<IUID> spUID;
	spUID.CoCreateInstance(CLSID_UID);
	CComQIPtr<IEnumLayer> spEnumLayer;
	spUID->put_Value(_variant_t("{40A9E885-5533-11d0-98BE-00805F7CED21}"));
	spMap->get_Layers(spUID,VARIANT_TRUE,&spEnumLayer);
	if (spEnumLayer == NULL) return E_FAIL;
	CComQIPtr<ILayer> spLayer;

	//获取县级行政区要素集
	if (m_ipFtWorkspace != NULL)
	{
		m_ipFtWorkspace->OpenFeatureClass(_T("XZQX"),&m_ipXZQXFtClass);
	}

	//获取选中的要素
	IActiveViewPtr ipActiveView;
	IMapPtr ipMap;
	ISelectionPtr ipSelect;
	IFeaturePtr ipFt;
	CComQIPtr<IFeatureClass> ipFtClass;
	IObjectClassPtr ipObjClass;
	m_ipMapCtrl->get_ActiveView(&ipActiveView);
	ipActiveView->get_FocusMap(&ipMap);
	ipMap->get_FeatureSelection(&ipSelect);
	//这里考虑将选集交给一个全局变量，节约下一次需要用到时获取选集的时间
	IEnumFeaturePtr ipEnumFt(ipSelect);
	m_ipEnumFt = ipEnumFt;
	ipEnumFt->Reset();
	//获取第一个选中要素的要素集，用于判断图层
	if (ipEnumFt->Next(&ipFt) == S_OK)
	{
		ipFt->get_Class(&ipObjClass);
		ipFtClass = ipObjClass;
	}

	//将所有图层添加到combobox
	IUIDPtr ipUID(CLSID_UID);
	IEnumLayerPtr ipEnumLayer;
	ILayerPtr ipLayer;
	ipUID->put_Value(_variant_t(_T("{40A9E885-5533-11d0-98BE-00805F7CED21}")));
	ipMap->get_Layers(ipUID,VARIANT_TRUE,&ipEnumLayer);
	if (ipEnumLayer != NULL)
	{
		ipEnumLayer->Next(&ipLayer);
	}
	int i = 0;
	while(ipLayer != NULL)
	{
		IFeatureLayerPtr ipFtLayer;
		ipFtLayer = ipLayer;
		if (ipFtLayer == NULL)
		{
			ipEnumLayer->Next(&ipLayer);
			continue;
		}
		CComQIPtr<IFeatureClass> ipLayerFtClass;
		ipFtLayer->get_FeatureClass(&ipLayerFtClass);
		if (ipLayerFtClass == NULL)
		{
			ipEnumLayer->Next(&ipLayer);
			i++;
			continue;
		}
		esriGeometryType eShapeType;
		ipLayerFtClass->get_ShapeType(&eShapeType);
		if (eShapeType != esriGeometryPolygon)
		{
			ipEnumLayer->Next(&ipLayer);
			continue;
		}
		CComBSTR sLayerName;
		ipFtLayer->get_Name(&sLayerName);
		CString strLayerName(sLayerName);
		if (strLayerName.Find(_T("行政区")) >= 0)
		{
			ipEnumLayer->Next(&ipLayer);
			continue;
		}
		m_LayerSelect.AddString(sLayerName);
		//如果当前图层与选集中的第一个要素所在图层相同，设置选中
		if (ipLayerFtClass == ipFtClass)
		{
			m_LayerSelect.SetCurSel(i);
		}

		ipEnumLayer->Next(&ipLayer);
		i++;
	}
	//如果为空，做另一个初始化
	if (ipFt == NULL)
	{
		m_LayerSelect.SetCurSel(0);
		((CButton *)GetDlgItem(IDC_SELECTLAYER))->SetCheck(TRUE);
		m_eSelect = SelectThisLayer;
	}else
	{
		((CButton *)GetDlgItem(IDC_SELECTIONINLAYER))->SetCheck(TRUE);
		m_eSelect = SelectionInThisLayer;
	}
	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// CExpCoordDlg 消息处理程序


void CExpCoordDlg::OnBnClickedSelectioninlayer()
{
	// TODO: 在此添加控件通知处理程序代码
	m_eSelect = SelectionInThisLayer;
}

void CExpCoordDlg::OnBnClickedSelectlayer()
{
	// TODO: 在此添加控件通知处理程序代码
	m_eSelect = SelectThisLayer;
}

void CExpCoordDlg::OnBnClickedRdbcountry()
{
	// TODO: 在此添加控件通知处理程序代码
	m_eCoordType = Country;
}
void CExpCoordDlg::OnBnClickedRdbcountryP()
{
	// TODO: 在此添加控件通知处理程序代码
	m_eCoordType = Country_P;
}

void CExpCoordDlg::OnBnClickedRdbprovince()
{
	// TODO: 在此添加控件通知处理程序代码
	m_eCoordType = Province;
}

void CExpCoordDlg::OnBnClickedRdbconstom()
{
	// TODO: 在此添加控件通知处理程序代码
	m_eCoordType = Custom;
}



BOOL CExpCoordDlg::ExportingCoord(IFeatureLayer *ipFtLayer)
{
	if (ipFtLayer == NULL)
	{return FALSE;}

	BOOL bRes = CheckData(ipFtLayer);
	if (bRes == FALSE)
	{
		return -2;
	}

	if (m_fSaveFile.Open(m_sExpPath, CFile::modeCreate | CFile::modeReadWrite) == FALSE)
	{
		MessageBox(_T("无法在指定位置创建文件，或选定的文件无法打开"),_T("写入文件失败"),MB_OK);
		return FALSE;
	}

	if (ExportHead() == FALSE)
	{return FALSE;}

	CComQIPtr<IFeatureCursor> ipFtCursor;
	CComQIPtr<IFeature> ipFt;
	int FtNum = 1;//标识要素序号，从1开始
	ipFtLayer->Search(NULL,TRUE,&ipFtCursor);
	ipFtCursor->NextFeature(&ipFt);
	while (ipFt != NULL )
	{
		//把多圈的验证放到这里，可以排除第一个要素是多圈的情况，防止与预处理重复
// 		CComQIPtr<IGeometry> spGeometry;
// 		CComQIPtr<IGeometryCollection> spGeometryCollection;
// 		long nOID;
// 		long nCount;
// 		ipFt->get_Shape(&spGeometry);
// 		spGeometry->QueryInterface(&spGeometryCollection);
// 		spGeometryCollection->get_GeometryCount(&nCount);
// 		if (nCount > 1 && FtNum != 1 && m_eCoordType == Custom)
// 		{
// 			ipFt->get_OID(&nOID);
// 			CString strMSG;
// 			strMSG.Format(_T("常规坐标不支持多图形要素！\n当前要素:ID=%ld包含多个图形，是否跳过？"),nOID);
// 			if (BCGPMessageBox(strMSG,_T("警告"),MB_OKCANCEL) == IDOK)
// 			{
// 				FtNum++;
// 				ipFt.Release();
// 				ipFtCursor->NextFeature(&ipFt);
// 				continue;
// 			}
// 		}
		if (ExportingFeature(ipFt) == FALSE)
		{
			CString sMessage;
			sMessage.Format(_T("导出要素失败: NUM %d"),FtNum);
			MessageBox(sMessage,_T("错误"),MB_OK);
			continue;
		}
		FtNum++;
		ipFt.Release();
		ipFtCursor->NextFeature(&ipFt);
	}
	m_fSaveFile.Close();

	return TRUE;
}

BOOL CExpCoordDlg::ExportingCoord(CArray<CComQIPtr<IFeature>, CComQIPtr<IFeature>> *arFt)
{
	if (arFt == NULL)
	{return FALSE;}

	BOOL bRes = CheckData(arFt);
	if (bRes == FALSE)
	{
		return -2;
	}
	if (m_fSaveFile.Open(m_sExpPath, CFile::modeCreate | CFile::modeReadWrite) == FALSE)
	{
		MessageBox(_T("无法在指定位置创建文件，或选定的文件无法打开"),_T("写入文件失败"),MB_OK);
		return FALSE;
	}

	if (ExportHead() == FALSE)
	{return FALSE;}

	CComQIPtr<IFeature> ipFt;
	int FtNum = 1;//标识要素序号，从1开始
	for (FtNum = 1; FtNum <= arFt->GetCount(); FtNum++)
	{
		ipFt = arFt->GetAt(FtNum - 1);//数组序号从0开始，所以这里减1
		//把多圈的验证放到这里，可以排除第一个要素是多圈的情况，防止与预处理重复
// 		CComQIPtr<IGeometry> spGeometry;
// 		CComQIPtr<IGeometryCollection> spGeometryCollection;
// 		long nOID;
// 		long nCount;
// 		ipFt->get_Shape(&spGeometry);
// 		spGeometry->QueryInterface(&spGeometryCollection);
// 		spGeometryCollection->get_GeometryCount(&nCount);
// 		if (nCount > 1 && FtNum != 1 && m_eCoordType == Custom)
// 		{
// 			ipFt->get_OID(&nOID);
// 			CString strMSG;
// 			strMSG.Format(_T("常规坐标不支持多图形要素！\n当前要素:ID=%ld包含多个图形，是否跳过？"),nOID);
// 			if (BCGPMessageBox(strMSG,_T("警告"),MB_OKCANCEL) == IDOK)
// 			{
// 				continue;
// 			}
// 		}
		if (ExportingFeature(ipFt) == FALSE)
		{
			CString sMessage;
			sMessage.Format(_T("导出要素失败: NUM %d"),FtNum);
			MessageBox(sMessage,_T("导出失败"),MB_OK);
			continue;
		}
	}
	m_fSaveFile.Close();

	return TRUE;
}

BOOL CExpCoordDlg::ExportHead()
{

	//Unicode文件头
	const unsigned char LeadBytes[]  = {0xff, 0xfe};
	m_fSaveFile.Write(LeadBytes, sizeof(LeadBytes));
	//对于非标准格式，给一个系统自定义标准文件头作为标识
	if (m_eCoordType == Custom)
	{
		CTime tSystime =  CTime::GetTickCount();
		CString stime;
		stime.Format(_T("GISQLANDPLAN$$%d-%02d-%02d\r\n"),tSystime.GetYear(),tSystime.GetMonth(),tSystime.GetDay());
		m_fSaveFile.Write(stime,stime.GetLength()*sizeof(wchar_t));
	}

	//只有国家标准有额外头部
	if (m_eCoordType != Country && m_eCoordType != Country_P)
	{return TRUE;}

	CTime tSystime =  CTime::GetTickCount();
	CString stime;
	stime.Format(_T("数据产生日期=%d-%02d-%02d\r\n"),tSystime.GetYear(),tSystime.GetMonth(),tSystime.GetDay());

	double nExp = 1.0;
	for (int i = 0; i < m_nExpPrecision; i++)
	{
		nExp /= 10.0;
	}
	CString sExp;
	CString sExpType;
	sExpType.Format(_T("精度=%%.%dlf\r\n"),m_nExpPrecision);
	sExp.Format(sExpType,nExp);

	CString strHead;
	strHead	+= _T("[属性描述]\r\n"); 
	strHead	+= _T("格式版本号=\r\n");
	strHead += _T("数据产生单位=国土资源部\r\n");
	strHead	+= stime;
	strHead += _T("坐标系=80国家大地坐标系\r\n");
	strHead += _T("几度分带=3\r\n");
	strHead += _T("投影类型=高斯克吕格\r\n");
	strHead += _T("计量单位=米\r\n");
	strHead += _T("带号=40\r\n");
	strHead += sExp;
	strHead += _T("转换参数=0,0,0,0,0,0,0\r\n");
	strHead += _T("[地块坐标]\r\n");

	m_fSaveFile.Write(strHead,strHead.GetLength()*sizeof(wchar_t));
	return TRUE;
}

BOOL CExpCoordDlg::ExportingFeature(CComQIPtr<IFeature> ipFt)
{
	if (ipFt == NULL)
	{return FALSE;}

	switch (m_eCoordType)
	{
	case Country:
		if (CountryTypeExporting(ipFt) == FALSE)
		{return FALSE;}		
		break;
	case Province:
		if (ProvinceTypeExporting(ipFt) == FALSE)
		{return FALSE;}
		break;
	case Custom:
		if (CustomTypeExporting(ipFt) == FALSE)
		{return FALSE;}
		break;
	case Country_P:
		if (Country_pTypeExproting(ipFt) == FALSE)
		{return FALSE;}
		break;
	default:
		break;
	}

	return TRUE;
}

BOOL CExpCoordDlg::CountryTypeExporting(CComQIPtr<IFeature> ipFt)
{	
	if (ipFt == NULL)
	{return FALSE;}

	//创建格式化字符串
	CString sPoint;
	CString sPointType;
	//单个点坐标格式化模板
	sPointType.Format(_T("J%%02d,%%d,%%.%dlf,%%.%dlf\r\n"),m_nExpPrecision,m_nExpPrecision);
	CString sFeatureProperty;
	CString sFeaturePropertyType;
	CString sFeaturePropertyOtherType;
	//多边形要素属性模板（界址点数，地块面积，地块编号，地块名称，地块类型）
	sFeaturePropertyType.Format(_T("%%d,%%s,%%s,%%s,面,,,@\r\n"));
	//其他要素属性模板
	sFeaturePropertyOtherType.Format(_T("%%d,,%%s,%%s,线,,,@\r\n"));

	IGeometryPtr ipGeometry;
	IGeometryCollectionPtr ipGeometryCollection;
	ipFt->get_ShapeCopy(&ipGeometry);
	if (ipGeometry == NULL)
	{
		return FALSE;
	}
	CheckSpatial(ipGeometry);
	ExportFeatureFields(ipFt);
/*	ipFt->get_Shape(&ipGeometry);*/

// 	//这里需要对每个要素输出一个属性头
 	esriGeometryType eType;
 	ipGeometry->get_GeometryType(&eType);
// 	long nPointCount;
// 	IPointCollectionPtr ipPCollection(ipGeometry);
// 	ipPCollection->get_PointCount(&nPointCount);
// 
// 	if (eType == esriGeometryPolygon)
// 	{
// 		CString strDKBH;
// 		CString strMJ;
// 		IFieldsPtr ipFields;
// 		IFieldPtr ipField;
// 		ipFt->get_Fields(&ipFields);
// 		long nFieldCount;
// 		ipFields->get_FieldCount(&nFieldCount);
// 		for(int i = 0; i < nFieldCount - 1; i++)
// 		{
// 			ipFields->get_Field(i,&ipField);
// 			CString strFieldName;
// 			BSTR bstrName;
// 			ipField->get_Name(&bstrName);
// 			strFieldName = bstrName;
// 			if (strFieldName.Find(_T("DKBH")) >= 0)
// 			{
// 				VARIANT vt;
// 				ipFt->get_Value(i,&vt);
// 				strDKBH = vt.bstrVal;
// // 			}else if (strFieldName.Find(_T("MJ")) >= 0 && strMJ.IsEmpty())
// // 			{
// // 				VARIANT vt;
// // 				ipFt->get_Value(i,&vt);
// // 				strMJ.Format(_T("%lf"),vt.dblVal);
// 			}
// 		}
// 		if (strMJ.IsEmpty())
// 		{
// 			double nArea;
// 			IAreaPtr ipArea;
// 			ipArea = ipGeometry;
// 			ipArea->get_Area(&nArea);
// 			nArea = nArea/*/10000.0*/;
// 			strMJ.Format(_T("%lf"),nArea);
// 		}
// 		//（界址点数，地块面积，地块编号，地块名称，地块类型）
// 		sFeatureProperty.Format(sFeaturePropertyType,nPointCount,strMJ,strDKBH,_T(""));
// 		m_fSaveFile.Write(sFeatureProperty,sFeatureProperty.GetLength()*sizeof(wchar_t));
// 	}else
// 	{
// 		sFeatureProperty.Format(sFeaturePropertyOtherType,nPointCount);
// 		m_fSaveFile.Write(sFeatureProperty,sFeatureProperty.GetLength()*sizeof(wchar_t));
//	}

	//一个要素有多个圈时需要区分圈号，所以用collection转一次
	ipGeometry->QueryInterface(&ipGeometryCollection);
	long nCount = 0;
	ipGeometryCollection->get_GeometryCount(&nCount);
	long nPIndex = 1;//界址点号
	long nPStartIndex = 1;//多边形起始界址点号位置
	for (int i = 0; i < nCount; i++)
	{
		IGeometryPtr ipGeometryTmp;
		ipGeometryCollection->get_Geometry(i,&ipGeometryTmp);
		IPointCollectionPtr ipPointCollection(ipGeometryTmp);
		IPointPtr ipPoint;//用于二级遍历
		double dx;
		double dy;
		IPointPtr ipStartPoint;//多边形要素起始点
		double dsx;
		double dsy;
		ipPointCollection->get_Point(0,&ipStartPoint);
		ipStartPoint->get_X(&dsx);
		ipStartPoint->get_Y(&dsy);
		nPStartIndex = nPIndex;
		long nPointCount = 0;
		ipPointCollection->get_PointCount(&nPointCount);
		for (int j = 0; j < nPointCount; j++)
		{
			ipPointCollection->get_Point(j,&ipPoint);
			ipPoint->get_X(&dx);
			ipPoint->get_Y(&dy);
			switch (eType)
			{
			case esriGeometryPoint:
				sPoint.Format(sPointType,nPIndex,1,dy,dx);
				m_fSaveFile.Write(sPoint,sPoint.GetLength()*sizeof(wchar_t));
				nPIndex++;
				break;
			case esriGeometryMultipoint:
				sPoint.Format(sPointType,nPIndex,j+1,dy,dx);
				m_fSaveFile.Write(sPoint,sPoint.GetLength()*sizeof(wchar_t));
				nPIndex++;
				break;
			case esriGeometryLine:
			case esriGeometryPolyline:
				sPoint.Format(sPointType,nPIndex,1,dy,dx);
				m_fSaveFile.Write(sPoint,sPoint.GetLength()*sizeof(wchar_t));
				nPIndex++;
				break;
			case esriGeometryPolygon:
			case esriGeometryEnvelope:
				if (dx == dsx && j != 0)//如果到了最后一个点，应该与起始点位置一样，用起始点界址点号，界址点号不累加
				{
					sPoint.Format(sPointType,nPStartIndex,i+1,dy,dx);
					m_fSaveFile.Write(sPoint,sPoint.GetLength()*sizeof(wchar_t));
				}else
				{
					sPoint.Format(sPointType,nPIndex,i+1,dy,dx);
					m_fSaveFile.Write(sPoint,sPoint.GetLength()*sizeof(wchar_t));
					nPIndex++;
				}
				break;
			default:
				MessageBox(_T("选集中存在不支持的要素类型"),_T("错误"),MB_OK);
				return FALSE;
				break;
			}
		}
	}
	return TRUE;
}

BOOL CExpCoordDlg::ProvinceTypeExporting(CComQIPtr<IFeature> ipFt)
{
	if (ipFt == NULL)
	{return FALSE;}
	//创建格式化字符串，写入保留小数的位数
	CString sPoint;
	CString sPointType;
	sPointType.Format(_T("%%d,%%d,%%d,J%%d,%%.%dlf,%%.%dlf\r\n"),m_nExpPrecision,m_nExpPrecision);
	//一个要素有多个圈时需要区分圈号，所以用collection转一次
	IGeometryPtr ipGeometry;
	IGeometryCollectionPtr ipGeometryCollection;
	/*ipFt->get_Shape(&ipGeometry);*/
	ipFt->get_ShapeCopy(&ipGeometry);
	CheckSpatial(ipGeometry);
	ipGeometry->QueryInterface(&ipGeometryCollection);
	long nCount = 0;
	ipGeometryCollection->get_GeometryCount(&nCount);
	esriGeometryType eType;
	ipGeometry->get_GeometryType(&eType);
	long nPIndex = 1;//界址点号
	long nPStartIndex = 1;//多边形起始界址点号位置
	BOOL bIsStartPoint = TRUE;//当前点是否是一个圈的起始点
	for (int i = 0; i < nCount; i++)
	{
		IGeometryPtr ipGeometryTmp;
		ipGeometryCollection->get_Geometry(i,&ipGeometryTmp);
		IPointCollectionPtr ipPointCollection(ipGeometryTmp);
		IPointPtr ipPoint;//用于二级遍历
		double dx;
		double dy;
		IPointPtr ipStartPoint;//多边形要素起始点
		double dsx;
		double dsy;
		ipPointCollection->get_Point(0,&ipStartPoint);
		ipStartPoint->get_X(&dsx);
		ipStartPoint->get_Y(&dsy);
		nPStartIndex = nPIndex;
		long nPointCount = 0;
		ipPointCollection->get_PointCount(&nPointCount);
		for (int j = 0; j < nPointCount; j++)
		{
			ipPointCollection->get_Point(j,&ipPoint);
			ipPoint->get_X(&dx);
			ipPoint->get_Y(&dy);
			switch (eType)
			{
			case esriGeometryPoint:
				sPoint.Format(sPointType,m_nProvincePointIndex,m_nProvinceFtIndex,1,nPIndex,dy,dx);
				m_fSaveFile.Write(sPoint,sPoint.GetLength()*sizeof(wchar_t));
				nPIndex++;
				break;
			case esriGeometryMultipoint:
				sPoint.Format(sPointType,m_nProvincePointIndex,m_nProvinceFtIndex,i,nPIndex,dy,dx);
				m_fSaveFile.Write(sPoint,sPoint.GetLength()*sizeof(wchar_t));
				nPIndex++;
				break;
			case esriGeometryLine:
			case esriGeometryPolyline:
				sPoint.Format(sPointType,m_nProvincePointIndex,m_nProvinceFtIndex,1,nPIndex,dy,dx);
				m_fSaveFile.Write(sPoint,sPoint.GetLength()*sizeof(wchar_t));
				nPIndex++;
				break;
			case esriGeometryPolygon:
			case esriGeometryEnvelope:
				if (dx == dsx && j == nPointCount - 1)//如果到了最后一个点，应该与起始点位置一样，用起始点界址点号，界址点号不累加
				{
					sPoint.Format(sPointType,m_nProvincePointIndex,m_nProvinceFtIndex,i+1,nPStartIndex,dy,dx);
					m_fSaveFile.Write(sPoint,sPoint.GetLength()*sizeof(wchar_t));
				}else
				{
					sPoint.Format(sPointType,m_nProvincePointIndex,m_nProvinceFtIndex,i+1,nPIndex,dy,dx);
					m_fSaveFile.Write(sPoint,sPoint.GetLength()*sizeof(wchar_t));
					nPIndex++;
				}
				break;
			default:
				MessageBox(_T("选集中存在不支持的要素类型"),_T("错误"),MB_OK);
				return FALSE;
				break;
			}
			m_nProvincePointIndex++;
		}
	}
	m_nProvinceFtIndex++;
	//每一个要素结束后添加一个空行标识结束
	m_fSaveFile.Write(_T("\r\n"),2*sizeof(wchar_t));
	return TRUE;
}

BOOL CExpCoordDlg::CustomTypeExporting(CComQIPtr<IFeature> ipFt)
{
	if (ipFt == NULL)
	{return FALSE;}
	//常规坐标导出时不需要判断要素类型和圈号
	IGeometryPtr ipGeometry;
	/*ipFt->get_Shape(&ipGeometry);*/
	ipFt->get_ShapeCopy(&ipGeometry);
	CheckSpatial(ipGeometry);
	IPointCollectionPtr ipPointCollection(ipGeometry);
	long nCount;
	ipPointCollection->get_PointCount(&nCount);
	IPointPtr ipPoint;
	double dx = 0;
	double dy = 0;
	CString sPoint;
	//创建格式化字符串，写入保留小数的位数
	CString sPointType;
	sPointType.Format(_T("%%.%dlf,%%.%dlf\r\n"),m_nExpPrecision,m_nExpPrecision);
	for (int i = 0; i < nCount; i++)
	{
		ipPointCollection->get_Point(i,&ipPoint);
		ipPoint->get_X(&dx);
		ipPoint->get_Y(&dy);
		//edit by lcc 20170331 更换X,Y坐标位置
		//sPoint.Format(sPointType, dx, dy);
		sPoint.Format(sPointType, dy, dx);
		m_fSaveFile.Write(sPoint,(sPoint.GetLength())*sizeof(wchar_t));
	}
	//每一个要素结束后添加一个空行标识结束
	m_fSaveFile.Write(_T("\r\n"),2*sizeof(wchar_t));
	return TRUE;
}
void CExpCoordDlg::OnBnClickedOk()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString strTmp;
	m_wndExpPrecision.GetWindowText(strTmp);
	if (strTmp.IsEmpty() == TRUE)
	{
		m_wndExpPrecision.SetWindowText(_T("0"));
	}
	UpdateData();
	if (m_eCoordType == nulltype)
	{
		MessageBox(_T("未选择坐标格式"),_T("错误"),MB_OK);
		return;
	}
// 	//校验文件路径是否为空
// 	if (m_sExpPath.GetLength() == 0)
// 	{
// 		MessageBox(_T("请选择导出路径"),_T("文件名为空"),MB_OK);
// 		return;
// 	}
// 	//先做文件路径打开
// 	//------2017/02/25 lcc 将文本框设置为只读，不在确定时再验证文件是否存在
// 	//-------2017/02/23 end
// 	CFileFind filefind;
// 	if (filefind.FindFile(m_sExpPath) == TRUE)
// 	{
// 		if (BCGPMessageBox(_T("文件已存在是否覆盖原文件"),_T("警告"),MB_OKCANCEL) == IDCANCEL)
// 		{
// 			return;
// 		}		
// 	}
// 	if (m_fSaveFile.Open(m_sExpPath, CFile::modeCreate | CFile::modeReadWrite) == FALSE)
// 	{
// 		MessageBox(_T("无法在指定位置创建文件，或选定的文件无法打开"),_T("写入文件失败"),MB_OK);
// 		return;
// 	}
	//检查是否选中图层
	int nindex = m_LayerSelect.GetCurSel();
	if (nindex == CB_ERR)
	{
		MessageBox(_T("未选中图层"),_T("选集为空"),MB_OK);
		return;
	}
	//获取活动地图
	CComQIPtr<IMap> spMap;
	m_ipMapCtrl->get_Map(&spMap);
	//获取选中的图层及其要素集
	CString strLayerName;
	m_LayerSelect.GetLBText(nindex,strLayerName);

	CComQIPtr<IUID> spUID;
	spUID.CoCreateInstance(CLSID_UID);
	spUID->put_Value(_variant_t("{40A9E885-5533-11d0-98BE-00805F7CED21}"));
	CComQIPtr<IEnumLayer> spEnumLayer;
	spMap->get_Layers(spUID,VARIANT_TRUE,&spEnumLayer);
	if (spEnumLayer == NULL) return ;

	CComQIPtr<ILayer> ipLayer;
	CComQIPtr<IFeatureLayer> ipFtLayer;//选中的图层
	CComQIPtr<IFeatureClass> ipFtLayerClass;//选中图层的要素集
	spEnumLayer->Next(&ipLayer);
	while(ipLayer != NULL)
	{
		ipFtLayer = ipLayer;
		CComBSTR bstrLayerName;
		ipLayer->get_Name(&bstrLayerName);
		if (strLayerName.Compare(bstrLayerName) == 0)
		{
			ipFtLayer->get_FeatureClass(&ipFtLayerClass);
			break;
		}
		ipFtLayer.Release();
		ipLayer.Release();
		spEnumLayer->Next(&ipLayer);
	}

	CComQIPtr<IFeature> ipFt;
	CComQIPtr<IFeatureClass> ipFtClass;
	CComQIPtr<IObjectClass> ipObjClass;
	CArray<CComQIPtr<IFeature>, CComQIPtr<IFeature>> arFt;

	//考虑到初始化窗口后可能还会进行选择操作，这里再获取一次选中要素
	CComQIPtr<ISelection> spFtSelect;
	spMap->get_FeatureSelection(&spFtSelect);
	CComQIPtr<IEnumFeature> ipEnumFt(spFtSelect);
	m_ipEnumFt = ipEnumFt;

	//重置省标的输出标记
	m_nProvinceFtIndex = 1;
	m_nProvincePointIndex = 1;

	BOOL bRES = FALSE;

	switch (m_eSelect)
	{
	case SelectThisLayer:
		//调用导出函数，如果失败，给出警告
		bRES = ExportingCoord(ipFtLayer);
		if (bRES == FALSE)
		{
			MessageBox(_T("导出失败"),_T("导出失败"),MB_OK);
		}else if (bRES == TRUE)
		{
			MessageBox(_T("导出成功"),_T("导出成功"),MB_OK);
		}
		break;
// 	case SelectionInAllLayer:		
// 		m_ipEnumFt->Reset();
// 		m_ipEnumFt->Next(&ipFt);
// 		if (ipFt == NULL)
// 		{
// 			MessageBox(_T("没有选定要导出的要素"),_T("选集为空"),MB_OK);
// 			break;
// 		}
// 		//调用导出函数，如果失败，给出警告
// 		if (ExportingCoord(m_ipEnumFt) == FALSE)
// 		{
// 			MessageBox(_T("导出失败"),_T("导出失败"),MB_OK);
// 		}else
// 		{
// 			MessageBox(_T("导出成功"),_T("导出成功"),MB_OK);
// 		}
// 		break;
	case SelectionInThisLayer:

		m_ipEnumFt->Reset();
		m_ipEnumFt->Next(&ipFt);
		while (ipFt != NULL)
		{
			if (ipObjClass != NULL)
			{
				ipObjClass.Release();
			}
			if (ipFtClass != NULL)
			{
				ipFtClass.Release();
			}
			ipFt->get_Class(&ipObjClass);
			ipFtClass = ipObjClass;
			if (ipFtClass == ipFtLayerClass)
			{
				//ipFt->AddRef();
				arFt.Add(ipFt);
			}
			ipFt.Release();
			m_ipEnumFt->Next(&ipFt);
		}
		
		//调用导出函数，如果失败，给出警告
		bRES = ExportingCoord(&arFt);
		if (bRES == FALSE)
		{
			MessageBox(_T("导出失败"),_T("导出失败"),MB_OK);
		}else if (bRES == TRUE)
		{
			MessageBox(_T("导出成功"),_T("导出成功"),MB_OK);
		}
		break;
	default:
		break;
	}

	//m_fSaveFile.Close();
	//CDialog::OnOK();
}

BOOL CExpCoordDlg::CheckSpatial(IGeometry* spGeo)
{
	if (spGeo == NULL) return FALSE;
	CComQIPtr<ISpatialReference> spGeoSpatail;
	spGeo->get_SpatialReference(&spGeoSpatail);
	CComBSTR bstrGeoSpatailName;
	CComBSTR bstrXZQXSpatailName;
	spGeoSpatail->get_Name(&bstrGeoSpatailName);
	GetGeoXZQSpatial(spGeo);
	if (m_spSpatialRederence == NULL)
	{return FALSE;
	}
	m_spSpatialRederence->get_Name(&bstrXZQXSpatailName);
	CString strGeoSpatailName(bstrGeoSpatailName);
	if (strGeoSpatailName.Compare(bstrXZQXSpatailName) == 0)
	{
		return TRUE;
	}else
	{
		spGeo->Project(m_spSpatialRederence);
	}
	return TRUE;
}

BOOL CExpCoordDlg::GetGeoXZQSpatial(CComQIPtr<IGeometry> spGeo)
{
	if (m_spSpatialRederence != NULL)
	{
		return TRUE;
	}
	if (spGeo == NULL) return FALSE;
	if (m_ipXZQXFtClass == NULL ) return FALSE;
	//空间查询
	BSTR bstrShapeFieldName;
	m_ipXZQXFtClass->get_ShapeFieldName(&bstrShapeFieldName);
	CComQIPtr<ISpatialFilter> spSpatailFilter;
	spSpatailFilter.CoCreateInstance(CLSID_SpatialFilter);
	spSpatailFilter->putref_Geometry(spGeo);
	spSpatailFilter->put_SpatialRel(esriSpatialRelIntersects);
	spSpatailFilter->put_GeometryField(bstrShapeFieldName);

	CComQIPtr<IFeatureCursor> spFtCurSor;
	m_ipXZQXFtClass->Search(spSpatailFilter,VARIANT_FALSE,&spFtCurSor);
	CComQIPtr<IFeature> spXZFt;
	spFtCurSor->NextFeature(&spXZFt);
	if (spXZFt != NULL)
	{
		CComQIPtr<IFields> spFields;
		spXZFt->get_Fields(&spFields);
		long nIndex = -1;
		spFields->FindField(_T("XZQMC"),&nIndex);
		if (nIndex >= 0)
		{
			VARIANT vt;
			spXZFt->get_Value(nIndex,&vt);
		}
	}
	CreateSpatial(40);
	return TRUE;
}

void CExpCoordDlg::CreateSpatial(int nProjCS)
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
	default:
		return;
	}	
	m_spSpatialRederence = spProjCorrdSys;
	CGeometryOperation::CreateSpatialReference(m_spSpatialRederence);
}
void CExpCoordDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CBCGPDialog::OnCancel();
	CBCGPDialog::DestroyWindow();
	//delete this;
}

void CExpCoordDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CBCGPDialog::OnClose();
	CBCGPDialog::DestroyWindow();
	//delete this;
}

void CExpCoordDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CBCGPDialog::OnActivate(nState, pWndOther, bMinimized);
	((CWnd*)GetDlgItem(IDC_EDIT_NUM))->SetFocus();

	// TODO: 在此处添加消息处理程序代码
}

void CExpCoordDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	CBCGPDialog::PostNcDestroy();
	delete this;
}

BOOL CExpCoordDlg::CheckData(CComQIPtr<IFeatureLayer> spFtLayer)
{
	//检查图层为空的情况
	long nCount;
	CComQIPtr<IFeatureClass> spFtClass;
	spFtLayer->get_FeatureClass(&spFtClass);
	spFtClass->FeatureCount(NULL,&nCount);
	if (nCount == 0)
	{
		BCGPMessageBox(_T("所选图层没有图形数据"),_T("图层为空"),MB_OK);
		return FALSE;
	}
	//检查图层只有一个元素且元素为多圈的情况,只有当选择的导出非标准坐标的情况
// 	CComQIPtr<IFeature> spFt;
// 	CComQIPtr<IFeatureCursor> spFtCursor;
// 	spFtClass->Search(NULL,VARIANT_FALSE,&spFtCursor);
// 	spFtCursor->NextFeature(&spFt);
// 	if (spFt != NULL && nCount == 1 && m_eCoordType == Custom)
// 	{
// 		CComQIPtr<IGeometry> spGeometry;
// 		CComQIPtr<IGeometryCollection> spGeoCollection;
// 		spFt->get_Shape(&spGeometry);
// 		spGeoCollection = spGeometry;
// 
// 		long nGeoCount;
// 		spGeoCollection->get_GeometryCount(&nGeoCount);
// 		if (nGeoCount > 1)
// 		{
// 			if (BCGPMessageBox(_T("选择图层仅有一个要素且为多圈要素, 是否导出"),_T("警告"),MB_OK) == IDCANCEL)
// 			{
// 				return FALSE;
// 			}
// 		}
// 	}
	//检查路径是否为空
	if (m_sExpPath.GetLength() == 0)
	{
		BCGPMessageBox(_T("请选择导出路径"),_T("路径为空"),MB_OK);
		return FALSE;
	}
	//检查是否有重复文件
	CFileFind filefind;
	if (filefind.FindFile(m_sExpPath) == TRUE)
	{
		if (BCGPMessageBox(_T("文件已存在是否覆盖原文件"),_T("警告"),MB_OKCANCEL) == IDCANCEL)
		{
			return FALSE;
		}		
	}
	return TRUE;
}

BOOL CExpCoordDlg::CheckData(CArray<CComQIPtr<IFeature>, CComQIPtr<IFeature>> *arFt)
{
	//校验选集是否为空
	if (arFt->IsEmpty() == TRUE)
	{
		MessageBox(_T("选定图层中不存在选定要素"),_T("选集为空"),MB_OK);
		return FALSE;
	}
	//检查图层只有一个元素且元素为多圈的情况,只有当选择的导出非标准坐标的情况
// 	CComQIPtr<IFeature> spFt = arFt->GetAt(0);
// 	if (spFt != NULL && arFt->GetCount() == 1 && m_eCoordType == Custom)
// 	{
// 		CComQIPtr<IGeometry> spGeometry;
// 		CComQIPtr<IGeometryCollection> spGeoCollection;
// 		spFt->get_Shape(&spGeometry);
// 		spGeoCollection = spGeometry;
// 
// 		long nGeoCount;
// 		spGeoCollection->get_GeometryCount(&nGeoCount);
// 		if (nGeoCount > 1)
// 		{
// 			if (BCGPMessageBox(_T("当前选择要素含有环形地块或含有多个地块, 是否导出"),_T("警告"),MB_OKCANCEL) == IDCANCEL)
// 			{
// 				return FALSE;
// 			}
// 		}
// 	}
	//校验文件路径是否为空
	if (m_sExpPath.GetLength() == 0)
	{
		MessageBox(_T("请选择导出路径"),_T("文件名为空"),MB_OK);
		return FALSE;
	}
	//校验文件是否存在
	CFileFind filefind;
	if (filefind.FindFile(m_sExpPath) == TRUE)
	{
		if (BCGPMessageBox(_T("文件已存在是否覆盖原文件"),_T("警告"),MB_OKCANCEL) == IDCANCEL)
		{
			return FALSE;
		}		
	}
	return TRUE;
}

BOOL CExpCoordDlg::ExportFeatureFields(CComQIPtr<IFeature> spFt)
{
	//国标属性头
	CString strFeatureFields;
	//界址点数,地块面积,地块编号,地块名称,记录图形属性(点、线、面),图幅号,地块用途,地类编码,@

	CComQIPtr<IGeometry> spGeometry;
	spFt->get_Shape(&spGeometry);
	//界址点数
	CString strPointCount;
	CComQIPtr<IPointCollection> spPointCollection;
	spPointCollection = spGeometry;
	long nPointCount;
	spPointCollection->get_PointCount(&nPointCount);
	strPointCount.Format(_T("%d"),nPointCount);
	strFeatureFields += strPointCount + _T(",");
	//获取要素集
	CComQIPtr<IFeatureClass> spFtClass;
	CComQIPtr<IObjectClass> spObjClass;
	spFt->get_Class(&spObjClass);
	spFtClass = spObjClass;
	long nIndex = -1;
	CComVariant vtTmp;
	//获取数据源坐标
	CComQIPtr<ISpatialReference> spSpatial;
	CComQIPtr<IGeoDataset> spGeoDataset = spFtClass;
	spGeoDataset->get_SpatialReference(&spSpatial);
	spGeometry->Project(spSpatial);
	//地块面积
	CString strMJ;
	double nArea;
	CComQIPtr<IArea> spArea;
	spArea = spGeometry;
	spArea->get_Area(&nArea);
	strMJ.Format(_T("%.4lf"),nArea);
	strFeatureFields += strMJ + _T(",");
	//地块编号
	spFtClass->FindField(_T("DK_BH"),&nIndex);
	if (nIndex >= 0)
	{
		spFt->get_Value(nIndex,&vtTmp);
		strFeatureFields += VariantToString(vtTmp);
	}else
	{
		spFtClass->FindField(_T("YTDKBH"),&nIndex);
		if (nIndex >= 0)
		{
			spFt->get_Value(nIndex,&vtTmp);
			strFeatureFields += VariantToString(vtTmp);
		}
	}
	strFeatureFields += _T(",");
	//地块名称
	spFtClass->FindField(_T("DK_MC"),&nIndex);
	if (nIndex >= 0)
	{
		spFt->get_Value(nIndex,&vtTmp);
		strFeatureFields += VariantToString(vtTmp);
	}else
	{
		spFtClass->FindField(_T("DKMC"),&nIndex);
		if (nIndex >= 0)
		{
			spFt->get_Value(nIndex,&vtTmp);
			strFeatureFields += VariantToString(vtTmp);
		}else
		{
			strFeatureFields += _T("地块");
		}
	}
	strFeatureFields += _T(",");
	//面,图幅号
	strFeatureFields += _T("面,");

	spFtClass->FindField(_T("TFH"),&nIndex);
	if (nIndex >= 0)
	{
		spFt->get_Value(nIndex,&vtTmp);
		strFeatureFields += VariantToString(vtTmp);
	}
	strFeatureFields += _T(",");
	//地块用途
	spFtClass->FindField(_T("DK_YT"),&nIndex);
	if (nIndex >= 0)
	{
		spFt->get_Value(nIndex,&vtTmp);
		strFeatureFields += VariantToString(vtTmp);
	}/*else
	{
		spFtClass->FindField(_T("GHYTDM"),&nIndex);
		if (nIndex >= 0)
		{
			spFt->get_Value(nIndex,&vtTmp);
			strFeatureFields += VariantToString(vtTmp);
		}
	}*/
	strFeatureFields += _T(",");
	//地类编码
	spFtClass->FindField(_T("DL_BM"),&nIndex);
	if (nIndex >= 0)
	{
		spFt->get_Value(nIndex,&vtTmp);
		strFeatureFields += VariantToString(vtTmp);
	}/*else
	{
		spFtClass->FindField(_T("GHDLDM"),&nIndex);
		if (nIndex >= 0)
		{
			spFt->get_Value(nIndex,&vtTmp);
			strFeatureFields += VariantToString(vtTmp);
		}
	}*/
	strFeatureFields  += _T(",@\r\n");
	m_fSaveFile.Write(strFeatureFields,strFeatureFields.GetLength()*sizeof(wchar_t));
	return TRUE;
}

BOOL CExpCoordDlg::Country_pTypeExproting(CComQIPtr<IFeature> ipFt)
{
	if (ipFt == NULL)
	{return FALSE;}

	//创建格式化字符串
	CString sPoint;
	CString sPointType;
	//单个点坐标格式化模板
	sPointType.Format(_T("J%%d,%%d,%%.%dlf,%%.%dlf\r\n"),m_nExpPrecision,m_nExpPrecision);
	CString sFeatureProperty;
	CString sFeaturePropertyType;
	CString sFeaturePropertyOtherType;
	//多边形要素属性模板（界址点数，地块面积，地块编号，地块名称，地块类型）
	sFeaturePropertyType.Format(_T("%%d,%%s,%%s,%%s,面,,,@\r\n"));
	//其他要素属性模板
	sFeaturePropertyOtherType.Format(_T("%%d,,%%s,%%s,线,,,@\r\n"));

	IGeometryPtr ipGeometry;
	IGeometryCollectionPtr ipGeometryCollection;
	ipFt->get_ShapeCopy(&ipGeometry);
	if (ipGeometry == NULL)
	{
		return FALSE;
	}
	CheckSpatial(ipGeometry);
	ExportFeatureFieldsForCountry_P(ipFt);

	// 	//这里需要对每个要素输出一个属性头
	esriGeometryType eType;
	ipGeometry->get_GeometryType(&eType);
	
	//一个要素有多个圈时需要区分圈号，所以用collection转一次
	ipGeometry->QueryInterface(&ipGeometryCollection);
	long nCount = 0;
	ipGeometryCollection->get_GeometryCount(&nCount);
	long nPIndex = 1;//界址点号
	long nPStartIndex = 1;//多边形起始界址点号位置
	for (int i = 0; i < nCount; i++)
	{
		IGeometryPtr ipGeometryTmp;
		ipGeometryCollection->get_Geometry(i,&ipGeometryTmp);
		IPointCollectionPtr ipPointCollection(ipGeometryTmp);
		IPointPtr ipPoint;//用于二级遍历
		double dx;
		double dy;
		IPointPtr ipStartPoint;//多边形要素起始点
		double dsx;
		double dsy;
		ipPointCollection->get_Point(0,&ipStartPoint);
		ipStartPoint->get_X(&dsx);
		ipStartPoint->get_Y(&dsy);
		nPStartIndex = nPIndex;
		long nPointCount = 0;
		ipPointCollection->get_PointCount(&nPointCount);
		for (int j = 0; j < nPointCount; j++)
		{
			ipPointCollection->get_Point(j,&ipPoint);
			ipPoint->get_X(&dx);
			ipPoint->get_Y(&dy);
			switch (eType)
			{
			case esriGeometryPoint:
				sPoint.Format(sPointType,nPIndex,1,dy,dx);
				m_fSaveFile.Write(sPoint,sPoint.GetLength()*sizeof(wchar_t));
				nPIndex++;
				break;
			case esriGeometryMultipoint:
				sPoint.Format(sPointType,nPIndex,j+1,dy,dx);
				m_fSaveFile.Write(sPoint,sPoint.GetLength()*sizeof(wchar_t));
				nPIndex++;
				break;
			case esriGeometryLine:
			case esriGeometryPolyline:
				sPoint.Format(sPointType,nPIndex,1,dy,dx);
				m_fSaveFile.Write(sPoint,sPoint.GetLength()*sizeof(wchar_t));
				nPIndex++;
				break;
			case esriGeometryPolygon:
			case esriGeometryEnvelope:
				if (dx == dsx && j != 0)//如果到了最后一个点，应该与起始点位置一样，用起始点界址点号，界址点号不累加
				{
					sPoint.Format(sPointType,nPStartIndex,i+1,dy,dx);
					m_fSaveFile.Write(sPoint,sPoint.GetLength()*sizeof(wchar_t));
				}else
				{
					sPoint.Format(sPointType,nPIndex,i+1,dy,dx);
					m_fSaveFile.Write(sPoint,sPoint.GetLength()*sizeof(wchar_t));
					nPIndex++;
				}
				break;
			default:
				MessageBox(_T("选集中存在不支持的要素类型"),_T("错误"),MB_OK);
				return FALSE;
				break;
			}
		}
	}
	return TRUE;}

BOOL CExpCoordDlg::ExportFeatureFieldsForCountry_P(CComQIPtr<IFeature> spFt)
{
	//国标属性头
	CString strFeatureFields;
	//界址点数,地块面积,乡镇代码，行政区名称,地块编号,地块名称,记录图形属性(点、线、面),图幅号,地块用途,地类编码,@

	CComQIPtr<IGeometry> spGeometry;
	spFt->get_Shape(&spGeometry);
	//界址点数
	CString strPointCount;
	CComQIPtr<IPointCollection> spPointCollection;
	spPointCollection = spGeometry;
	long nPointCount;
	spPointCollection->get_PointCount(&nPointCount);
	strPointCount.Format(_T("%d"),nPointCount);
	strFeatureFields += strPointCount + _T(",");
	//获取要素集
	CComQIPtr<IFeatureClass> spFtClass;
	CComQIPtr<IObjectClass> spObjClass;
	spFt->get_Class(&spObjClass);
	spFtClass = spObjClass;
	long nIndex = -1;
	CComVariant vtTmp;
	//获取数据源坐标
	CComQIPtr<ISpatialReference> spSpatial;
	CComQIPtr<IGeoDataset> spGeoDataset = spFtClass;
	spGeoDataset->get_SpatialReference(&spSpatial);
	spGeometry->Project(spSpatial);
	//地块面积
	CString strMJ;
	double nArea;
	CComQIPtr<IArea> spArea;
	spArea = spGeometry;
	spArea->get_Area(&nArea);
	strMJ.Format(_T("%.4lf"),nArea);
	strFeatureFields += strMJ + _T(",");
	//乡镇代码
	spFtClass->FindField(_T("XZ_DM"),&nIndex);
	if (nIndex >= 0)
	{
		spFt->get_Value(nIndex,&vtTmp);
		strFeatureFields += VariantToString(vtTmp);
	}else
	{
		spFtClass->FindField(_T("XZDM"),&nIndex);
		if (nIndex >= 0)
		{
			spFt->get_Value(nIndex,&vtTmp);
			strFeatureFields += VariantToString(vtTmp);
		}else
		{
			spFtClass->FindField(_T("XZQDM"),&nIndex);
			if (nIndex >= 0)
			{
				spFt->get_Value(nIndex,&vtTmp);
				strFeatureFields += (VariantToString(vtTmp).Left(9));
			}
		}
	}
	strFeatureFields += _T(",");
	//行政区名称
	spFtClass->FindField(_T("XZQ_MC"),&nIndex);
	if (nIndex >= 0)
	{
		spFt->get_Value(nIndex,&vtTmp);
		strFeatureFields += VariantToString(vtTmp);
	}else
	{
		spFtClass->FindField(_T("XZQMC"),&nIndex);
		if (nIndex >= 0)
		{
			spFt->get_Value(nIndex,&vtTmp);
			strFeatureFields += VariantToString(vtTmp);
		}
	}
	strFeatureFields += _T(",");
	//地块编号
	spFtClass->FindField(_T("DK_BH"),&nIndex);
	if (nIndex >= 0)
	{
		spFt->get_Value(nIndex,&vtTmp);
		strFeatureFields += VariantToString(vtTmp);
	}else
	{
		spFtClass->FindField(_T("YTDKBH"),&nIndex);
		if (nIndex >= 0)
		{
			spFt->get_Value(nIndex,&vtTmp);
			strFeatureFields += VariantToString(vtTmp);
		}
	}
	strFeatureFields += _T(",");
	//地块名称
	spFtClass->FindField(_T("DK_MC"),&nIndex);
	if (nIndex >= 0)
	{
		spFt->get_Value(nIndex,&vtTmp);
		strFeatureFields += VariantToString(vtTmp);
	}else
	{
		spFtClass->FindField(_T("DKMC"),&nIndex);
		if (nIndex >= 0)
		{
			spFt->get_Value(nIndex,&vtTmp);
			strFeatureFields += VariantToString(vtTmp);
		}else
		{
			strFeatureFields += _T("地块");
		}
	}
	strFeatureFields += _T(",");
	//面,图幅号
	strFeatureFields += _T("面,");

	spFtClass->FindField(_T("TFH"),&nIndex);
	if (nIndex >= 0)
	{
		spFt->get_Value(nIndex,&vtTmp);
		strFeatureFields += VariantToString(vtTmp);
	}
	strFeatureFields += _T(",");
	//地块用途
	spFtClass->FindField(_T("DK_YT"),&nIndex);
	if (nIndex >= 0)
	{
		spFt->get_Value(nIndex,&vtTmp);
		strFeatureFields += VariantToString(vtTmp);
	}/*else
	{
		spFtClass->FindField(_T("GHYTDM"),&nIndex);
		if (nIndex >= 0)
		{
			spFt->get_Value(nIndex,&vtTmp);
			strFeatureFields += VariantToString(vtTmp);
		}
	}*/
	strFeatureFields += _T(",");
	//地类编码
	spFtClass->FindField(_T("DL_BM"),&nIndex);
	if (nIndex >= 0)
	{
		spFt->get_Value(nIndex,&vtTmp);
		strFeatureFields += VariantToString(vtTmp);
	}/*else
	{
		spFtClass->FindField(_T("GHDLDM"),&nIndex);
		if (nIndex >= 0)
		{
			spFt->get_Value(nIndex,&vtTmp);
			strFeatureFields += VariantToString(vtTmp);
		}
	}*/
	strFeatureFields  += _T(",@\r\n");
	m_fSaveFile.Write(strFeatureFields,strFeatureFields.GetLength()*sizeof(wchar_t));
	return TRUE;
}


