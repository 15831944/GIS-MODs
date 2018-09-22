// ExpCoordDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GisqExpCoordCmd.h"
#include "ExpCoordDlg.h"


// CExpCoordDlg �Ի���

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

	m_wndExpPrecision.SetValidChars(_T("1234567890"));//�����ַ��б�
	//���á�����ѡ��ͼ����ѡ�е�Ҫ�ء���ѡ��Ϊѡ��
	TCHAR szDesktopFullPath[MAX_PATH];
	DWORD dBufferSize = sizeof(szDesktopFullPath)/sizeof(CHAR);
	SHGetSpecialFolderPath(NULL,szDesktopFullPath,CSIDL_DESKTOPDIRECTORY,NULL);
	CString strDesktopPath(szDesktopFullPath);
	CString strConfigFile = strDesktopPath + _T("\\�ؿ��ַ��.txt");
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
		//TODO��
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

	//��ȡ�ؼ�������Ҫ�ؼ�
	if (m_ipFtWorkspace != NULL)
	{
		m_ipFtWorkspace->OpenFeatureClass(_T("XZQX"),&m_ipXZQXFtClass);
	}

	//��ȡѡ�е�Ҫ��
	IActiveViewPtr ipActiveView;
	IMapPtr ipMap;
	ISelectionPtr ipSelect;
	IFeaturePtr ipFt;
	CComQIPtr<IFeatureClass> ipFtClass;
	IObjectClassPtr ipObjClass;
	m_ipMapCtrl->get_ActiveView(&ipActiveView);
	ipActiveView->get_FocusMap(&ipMap);
	ipMap->get_FeatureSelection(&ipSelect);
	//���￼�ǽ�ѡ������һ��ȫ�ֱ�������Լ��һ����Ҫ�õ�ʱ��ȡѡ����ʱ��
	IEnumFeaturePtr ipEnumFt(ipSelect);
	m_ipEnumFt = ipEnumFt;
	ipEnumFt->Reset();
	//��ȡ��һ��ѡ��Ҫ�ص�Ҫ�ؼ��������ж�ͼ��
	if (ipEnumFt->Next(&ipFt) == S_OK)
	{
		ipFt->get_Class(&ipObjClass);
		ipFtClass = ipObjClass;
	}

	//������ͼ����ӵ�combobox
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
		if (strLayerName.Find(_T("������")) >= 0)
		{
			ipEnumLayer->Next(&ipLayer);
			continue;
		}
		m_LayerSelect.AddString(sLayerName);
		//�����ǰͼ����ѡ���еĵ�һ��Ҫ������ͼ����ͬ������ѡ��
		if (ipLayerFtClass == ipFtClass)
		{
			m_LayerSelect.SetCurSel(i);
		}

		ipEnumLayer->Next(&ipLayer);
		i++;
	}
	//���Ϊ�գ�����һ����ʼ��
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// CExpCoordDlg ��Ϣ�������


void CExpCoordDlg::OnBnClickedSelectioninlayer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_eSelect = SelectionInThisLayer;
}

void CExpCoordDlg::OnBnClickedSelectlayer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_eSelect = SelectThisLayer;
}

void CExpCoordDlg::OnBnClickedRdbcountry()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_eCoordType = Country;
}
void CExpCoordDlg::OnBnClickedRdbcountryP()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_eCoordType = Country_P;
}

void CExpCoordDlg::OnBnClickedRdbprovince()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_eCoordType = Province;
}

void CExpCoordDlg::OnBnClickedRdbconstom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox(_T("�޷���ָ��λ�ô����ļ�����ѡ�����ļ��޷���"),_T("д���ļ�ʧ��"),MB_OK);
		return FALSE;
	}

	if (ExportHead() == FALSE)
	{return FALSE;}

	CComQIPtr<IFeatureCursor> ipFtCursor;
	CComQIPtr<IFeature> ipFt;
	int FtNum = 1;//��ʶҪ����ţ���1��ʼ
	ipFtLayer->Search(NULL,TRUE,&ipFtCursor);
	ipFtCursor->NextFeature(&ipFt);
	while (ipFt != NULL )
	{
		//�Ѷ�Ȧ����֤�ŵ���������ų���һ��Ҫ���Ƕ�Ȧ���������ֹ��Ԥ�����ظ�
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
// 			strMSG.Format(_T("�������겻֧�ֶ�ͼ��Ҫ�أ�\n��ǰҪ��:ID=%ld�������ͼ�Σ��Ƿ�������"),nOID);
// 			if (BCGPMessageBox(strMSG,_T("����"),MB_OKCANCEL) == IDOK)
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
			sMessage.Format(_T("����Ҫ��ʧ��: NUM %d"),FtNum);
			MessageBox(sMessage,_T("����"),MB_OK);
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
		MessageBox(_T("�޷���ָ��λ�ô����ļ�����ѡ�����ļ��޷���"),_T("д���ļ�ʧ��"),MB_OK);
		return FALSE;
	}

	if (ExportHead() == FALSE)
	{return FALSE;}

	CComQIPtr<IFeature> ipFt;
	int FtNum = 1;//��ʶҪ����ţ���1��ʼ
	for (FtNum = 1; FtNum <= arFt->GetCount(); FtNum++)
	{
		ipFt = arFt->GetAt(FtNum - 1);//������Ŵ�0��ʼ�����������1
		//�Ѷ�Ȧ����֤�ŵ���������ų���һ��Ҫ���Ƕ�Ȧ���������ֹ��Ԥ�����ظ�
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
// 			strMSG.Format(_T("�������겻֧�ֶ�ͼ��Ҫ�أ�\n��ǰҪ��:ID=%ld�������ͼ�Σ��Ƿ�������"),nOID);
// 			if (BCGPMessageBox(strMSG,_T("����"),MB_OKCANCEL) == IDOK)
// 			{
// 				continue;
// 			}
// 		}
		if (ExportingFeature(ipFt) == FALSE)
		{
			CString sMessage;
			sMessage.Format(_T("����Ҫ��ʧ��: NUM %d"),FtNum);
			MessageBox(sMessage,_T("����ʧ��"),MB_OK);
			continue;
		}
	}
	m_fSaveFile.Close();

	return TRUE;
}

BOOL CExpCoordDlg::ExportHead()
{

	//Unicode�ļ�ͷ
	const unsigned char LeadBytes[]  = {0xff, 0xfe};
	m_fSaveFile.Write(LeadBytes, sizeof(LeadBytes));
	//���ڷǱ�׼��ʽ����һ��ϵͳ�Զ����׼�ļ�ͷ��Ϊ��ʶ
	if (m_eCoordType == Custom)
	{
		CTime tSystime =  CTime::GetTickCount();
		CString stime;
		stime.Format(_T("GISQLANDPLAN$$%d-%02d-%02d\r\n"),tSystime.GetYear(),tSystime.GetMonth(),tSystime.GetDay());
		m_fSaveFile.Write(stime,stime.GetLength()*sizeof(wchar_t));
	}

	//ֻ�й��ұ�׼�ж���ͷ��
	if (m_eCoordType != Country && m_eCoordType != Country_P)
	{return TRUE;}

	CTime tSystime =  CTime::GetTickCount();
	CString stime;
	stime.Format(_T("���ݲ�������=%d-%02d-%02d\r\n"),tSystime.GetYear(),tSystime.GetMonth(),tSystime.GetDay());

	double nExp = 1.0;
	for (int i = 0; i < m_nExpPrecision; i++)
	{
		nExp /= 10.0;
	}
	CString sExp;
	CString sExpType;
	sExpType.Format(_T("����=%%.%dlf\r\n"),m_nExpPrecision);
	sExp.Format(sExpType,nExp);

	CString strHead;
	strHead	+= _T("[��������]\r\n"); 
	strHead	+= _T("��ʽ�汾��=\r\n");
	strHead += _T("���ݲ�����λ=������Դ��\r\n");
	strHead	+= stime;
	strHead += _T("����ϵ=80���Ҵ������ϵ\r\n");
	strHead += _T("���ȷִ�=3\r\n");
	strHead += _T("ͶӰ����=��˹������\r\n");
	strHead += _T("������λ=��\r\n");
	strHead += _T("����=40\r\n");
	strHead += sExp;
	strHead += _T("ת������=0,0,0,0,0,0,0\r\n");
	strHead += _T("[�ؿ�����]\r\n");

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

	//������ʽ���ַ���
	CString sPoint;
	CString sPointType;
	//�����������ʽ��ģ��
	sPointType.Format(_T("J%%02d,%%d,%%.%dlf,%%.%dlf\r\n"),m_nExpPrecision,m_nExpPrecision);
	CString sFeatureProperty;
	CString sFeaturePropertyType;
	CString sFeaturePropertyOtherType;
	//�����Ҫ������ģ�壨��ַ�������ؿ�������ؿ��ţ��ؿ����ƣ��ؿ����ͣ�
	sFeaturePropertyType.Format(_T("%%d,%%s,%%s,%%s,��,,,@\r\n"));
	//����Ҫ������ģ��
	sFeaturePropertyOtherType.Format(_T("%%d,,%%s,%%s,��,,,@\r\n"));

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

// 	//������Ҫ��ÿ��Ҫ�����һ������ͷ
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
// 		//����ַ�������ؿ�������ؿ��ţ��ؿ����ƣ��ؿ����ͣ�
// 		sFeatureProperty.Format(sFeaturePropertyType,nPointCount,strMJ,strDKBH,_T(""));
// 		m_fSaveFile.Write(sFeatureProperty,sFeatureProperty.GetLength()*sizeof(wchar_t));
// 	}else
// 	{
// 		sFeatureProperty.Format(sFeaturePropertyOtherType,nPointCount);
// 		m_fSaveFile.Write(sFeatureProperty,sFeatureProperty.GetLength()*sizeof(wchar_t));
//	}

	//һ��Ҫ���ж��Ȧʱ��Ҫ����Ȧ�ţ�������collectionתһ��
	ipGeometry->QueryInterface(&ipGeometryCollection);
	long nCount = 0;
	ipGeometryCollection->get_GeometryCount(&nCount);
	long nPIndex = 1;//��ַ���
	long nPStartIndex = 1;//�������ʼ��ַ���λ��
	for (int i = 0; i < nCount; i++)
	{
		IGeometryPtr ipGeometryTmp;
		ipGeometryCollection->get_Geometry(i,&ipGeometryTmp);
		IPointCollectionPtr ipPointCollection(ipGeometryTmp);
		IPointPtr ipPoint;//���ڶ�������
		double dx;
		double dy;
		IPointPtr ipStartPoint;//�����Ҫ����ʼ��
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
				if (dx == dsx && j != 0)//����������һ���㣬Ӧ������ʼ��λ��һ��������ʼ���ַ��ţ���ַ��Ų��ۼ�
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
				MessageBox(_T("ѡ���д��ڲ�֧�ֵ�Ҫ������"),_T("����"),MB_OK);
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
	//������ʽ���ַ�����д�뱣��С����λ��
	CString sPoint;
	CString sPointType;
	sPointType.Format(_T("%%d,%%d,%%d,J%%d,%%.%dlf,%%.%dlf\r\n"),m_nExpPrecision,m_nExpPrecision);
	//һ��Ҫ���ж��Ȧʱ��Ҫ����Ȧ�ţ�������collectionתһ��
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
	long nPIndex = 1;//��ַ���
	long nPStartIndex = 1;//�������ʼ��ַ���λ��
	BOOL bIsStartPoint = TRUE;//��ǰ���Ƿ���һ��Ȧ����ʼ��
	for (int i = 0; i < nCount; i++)
	{
		IGeometryPtr ipGeometryTmp;
		ipGeometryCollection->get_Geometry(i,&ipGeometryTmp);
		IPointCollectionPtr ipPointCollection(ipGeometryTmp);
		IPointPtr ipPoint;//���ڶ�������
		double dx;
		double dy;
		IPointPtr ipStartPoint;//�����Ҫ����ʼ��
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
				if (dx == dsx && j == nPointCount - 1)//����������һ���㣬Ӧ������ʼ��λ��һ��������ʼ���ַ��ţ���ַ��Ų��ۼ�
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
				MessageBox(_T("ѡ���д��ڲ�֧�ֵ�Ҫ������"),_T("����"),MB_OK);
				return FALSE;
				break;
			}
			m_nProvincePointIndex++;
		}
	}
	m_nProvinceFtIndex++;
	//ÿһ��Ҫ�ؽ��������һ�����б�ʶ����
	m_fSaveFile.Write(_T("\r\n"),2*sizeof(wchar_t));
	return TRUE;
}

BOOL CExpCoordDlg::CustomTypeExporting(CComQIPtr<IFeature> ipFt)
{
	if (ipFt == NULL)
	{return FALSE;}
	//�������굼��ʱ����Ҫ�ж�Ҫ�����ͺ�Ȧ��
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
	//������ʽ���ַ�����д�뱣��С����λ��
	CString sPointType;
	sPointType.Format(_T("%%.%dlf,%%.%dlf\r\n"),m_nExpPrecision,m_nExpPrecision);
	for (int i = 0; i < nCount; i++)
	{
		ipPointCollection->get_Point(i,&ipPoint);
		ipPoint->get_X(&dx);
		ipPoint->get_Y(&dy);
		//edit by lcc 20170331 ����X,Y����λ��
		//sPoint.Format(sPointType, dx, dy);
		sPoint.Format(sPointType, dy, dx);
		m_fSaveFile.Write(sPoint,(sPoint.GetLength())*sizeof(wchar_t));
	}
	//ÿһ��Ҫ�ؽ��������һ�����б�ʶ����
	m_fSaveFile.Write(_T("\r\n"),2*sizeof(wchar_t));
	return TRUE;
}
void CExpCoordDlg::OnBnClickedOk()
{
	// TODO: �ڴ����ר�ô����/����û���
	CString strTmp;
	m_wndExpPrecision.GetWindowText(strTmp);
	if (strTmp.IsEmpty() == TRUE)
	{
		m_wndExpPrecision.SetWindowText(_T("0"));
	}
	UpdateData();
	if (m_eCoordType == nulltype)
	{
		MessageBox(_T("δѡ�������ʽ"),_T("����"),MB_OK);
		return;
	}
// 	//У���ļ�·���Ƿ�Ϊ��
// 	if (m_sExpPath.GetLength() == 0)
// 	{
// 		MessageBox(_T("��ѡ�񵼳�·��"),_T("�ļ���Ϊ��"),MB_OK);
// 		return;
// 	}
// 	//�����ļ�·����
// 	//------2017/02/25 lcc ���ı�������Ϊֻ��������ȷ��ʱ����֤�ļ��Ƿ����
// 	//-------2017/02/23 end
// 	CFileFind filefind;
// 	if (filefind.FindFile(m_sExpPath) == TRUE)
// 	{
// 		if (BCGPMessageBox(_T("�ļ��Ѵ����Ƿ񸲸�ԭ�ļ�"),_T("����"),MB_OKCANCEL) == IDCANCEL)
// 		{
// 			return;
// 		}		
// 	}
// 	if (m_fSaveFile.Open(m_sExpPath, CFile::modeCreate | CFile::modeReadWrite) == FALSE)
// 	{
// 		MessageBox(_T("�޷���ָ��λ�ô����ļ�����ѡ�����ļ��޷���"),_T("д���ļ�ʧ��"),MB_OK);
// 		return;
// 	}
	//����Ƿ�ѡ��ͼ��
	int nindex = m_LayerSelect.GetCurSel();
	if (nindex == CB_ERR)
	{
		MessageBox(_T("δѡ��ͼ��"),_T("ѡ��Ϊ��"),MB_OK);
		return;
	}
	//��ȡ���ͼ
	CComQIPtr<IMap> spMap;
	m_ipMapCtrl->get_Map(&spMap);
	//��ȡѡ�е�ͼ�㼰��Ҫ�ؼ�
	CString strLayerName;
	m_LayerSelect.GetLBText(nindex,strLayerName);

	CComQIPtr<IUID> spUID;
	spUID.CoCreateInstance(CLSID_UID);
	spUID->put_Value(_variant_t("{40A9E885-5533-11d0-98BE-00805F7CED21}"));
	CComQIPtr<IEnumLayer> spEnumLayer;
	spMap->get_Layers(spUID,VARIANT_TRUE,&spEnumLayer);
	if (spEnumLayer == NULL) return ;

	CComQIPtr<ILayer> ipLayer;
	CComQIPtr<IFeatureLayer> ipFtLayer;//ѡ�е�ͼ��
	CComQIPtr<IFeatureClass> ipFtLayerClass;//ѡ��ͼ���Ҫ�ؼ�
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

	//���ǵ���ʼ�����ں���ܻ������ѡ������������ٻ�ȡһ��ѡ��Ҫ��
	CComQIPtr<ISelection> spFtSelect;
	spMap->get_FeatureSelection(&spFtSelect);
	CComQIPtr<IEnumFeature> ipEnumFt(spFtSelect);
	m_ipEnumFt = ipEnumFt;

	//����ʡ���������
	m_nProvinceFtIndex = 1;
	m_nProvincePointIndex = 1;

	BOOL bRES = FALSE;

	switch (m_eSelect)
	{
	case SelectThisLayer:
		//���õ������������ʧ�ܣ���������
		bRES = ExportingCoord(ipFtLayer);
		if (bRES == FALSE)
		{
			MessageBox(_T("����ʧ��"),_T("����ʧ��"),MB_OK);
		}else if (bRES == TRUE)
		{
			MessageBox(_T("�����ɹ�"),_T("�����ɹ�"),MB_OK);
		}
		break;
// 	case SelectionInAllLayer:		
// 		m_ipEnumFt->Reset();
// 		m_ipEnumFt->Next(&ipFt);
// 		if (ipFt == NULL)
// 		{
// 			MessageBox(_T("û��ѡ��Ҫ������Ҫ��"),_T("ѡ��Ϊ��"),MB_OK);
// 			break;
// 		}
// 		//���õ������������ʧ�ܣ���������
// 		if (ExportingCoord(m_ipEnumFt) == FALSE)
// 		{
// 			MessageBox(_T("����ʧ��"),_T("����ʧ��"),MB_OK);
// 		}else
// 		{
// 			MessageBox(_T("�����ɹ�"),_T("�����ɹ�"),MB_OK);
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
		
		//���õ������������ʧ�ܣ���������
		bRES = ExportingCoord(&arFt);
		if (bRES == FALSE)
		{
			MessageBox(_T("����ʧ��"),_T("����ʧ��"),MB_OK);
		}else if (bRES == TRUE)
		{
			MessageBox(_T("�����ɹ�"),_T("�����ɹ�"),MB_OK);
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
	//�ռ��ѯ
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
	// TODO: �ڴ����ר�ô����/����û���
	CBCGPDialog::OnCancel();
	CBCGPDialog::DestroyWindow();
	//delete this;
}

void CExpCoordDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CBCGPDialog::OnClose();
	CBCGPDialog::DestroyWindow();
	//delete this;
}

void CExpCoordDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CBCGPDialog::OnActivate(nState, pWndOther, bMinimized);
	((CWnd*)GetDlgItem(IDC_EDIT_NUM))->SetFocus();

	// TODO: �ڴ˴������Ϣ����������
}

void CExpCoordDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	CBCGPDialog::PostNcDestroy();
	delete this;
}

BOOL CExpCoordDlg::CheckData(CComQIPtr<IFeatureLayer> spFtLayer)
{
	//���ͼ��Ϊ�յ����
	long nCount;
	CComQIPtr<IFeatureClass> spFtClass;
	spFtLayer->get_FeatureClass(&spFtClass);
	spFtClass->FeatureCount(NULL,&nCount);
	if (nCount == 0)
	{
		BCGPMessageBox(_T("��ѡͼ��û��ͼ������"),_T("ͼ��Ϊ��"),MB_OK);
		return FALSE;
	}
	//���ͼ��ֻ��һ��Ԫ����Ԫ��Ϊ��Ȧ�����,ֻ�е�ѡ��ĵ����Ǳ�׼��������
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
// 			if (BCGPMessageBox(_T("ѡ��ͼ�����һ��Ҫ����Ϊ��ȦҪ��, �Ƿ񵼳�"),_T("����"),MB_OK) == IDCANCEL)
// 			{
// 				return FALSE;
// 			}
// 		}
// 	}
	//���·���Ƿ�Ϊ��
	if (m_sExpPath.GetLength() == 0)
	{
		BCGPMessageBox(_T("��ѡ�񵼳�·��"),_T("·��Ϊ��"),MB_OK);
		return FALSE;
	}
	//����Ƿ����ظ��ļ�
	CFileFind filefind;
	if (filefind.FindFile(m_sExpPath) == TRUE)
	{
		if (BCGPMessageBox(_T("�ļ��Ѵ����Ƿ񸲸�ԭ�ļ�"),_T("����"),MB_OKCANCEL) == IDCANCEL)
		{
			return FALSE;
		}		
	}
	return TRUE;
}

BOOL CExpCoordDlg::CheckData(CArray<CComQIPtr<IFeature>, CComQIPtr<IFeature>> *arFt)
{
	//У��ѡ���Ƿ�Ϊ��
	if (arFt->IsEmpty() == TRUE)
	{
		MessageBox(_T("ѡ��ͼ���в�����ѡ��Ҫ��"),_T("ѡ��Ϊ��"),MB_OK);
		return FALSE;
	}
	//���ͼ��ֻ��һ��Ԫ����Ԫ��Ϊ��Ȧ�����,ֻ�е�ѡ��ĵ����Ǳ�׼��������
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
// 			if (BCGPMessageBox(_T("��ǰѡ��Ҫ�غ��л��εؿ���ж���ؿ�, �Ƿ񵼳�"),_T("����"),MB_OKCANCEL) == IDCANCEL)
// 			{
// 				return FALSE;
// 			}
// 		}
// 	}
	//У���ļ�·���Ƿ�Ϊ��
	if (m_sExpPath.GetLength() == 0)
	{
		MessageBox(_T("��ѡ�񵼳�·��"),_T("�ļ���Ϊ��"),MB_OK);
		return FALSE;
	}
	//У���ļ��Ƿ����
	CFileFind filefind;
	if (filefind.FindFile(m_sExpPath) == TRUE)
	{
		if (BCGPMessageBox(_T("�ļ��Ѵ����Ƿ񸲸�ԭ�ļ�"),_T("����"),MB_OKCANCEL) == IDCANCEL)
		{
			return FALSE;
		}		
	}
	return TRUE;
}

BOOL CExpCoordDlg::ExportFeatureFields(CComQIPtr<IFeature> spFt)
{
	//��������ͷ
	CString strFeatureFields;
	//��ַ����,�ؿ����,�ؿ���,�ؿ�����,��¼ͼ������(�㡢�ߡ���),ͼ����,�ؿ���;,�������,@

	CComQIPtr<IGeometry> spGeometry;
	spFt->get_Shape(&spGeometry);
	//��ַ����
	CString strPointCount;
	CComQIPtr<IPointCollection> spPointCollection;
	spPointCollection = spGeometry;
	long nPointCount;
	spPointCollection->get_PointCount(&nPointCount);
	strPointCount.Format(_T("%d"),nPointCount);
	strFeatureFields += strPointCount + _T(",");
	//��ȡҪ�ؼ�
	CComQIPtr<IFeatureClass> spFtClass;
	CComQIPtr<IObjectClass> spObjClass;
	spFt->get_Class(&spObjClass);
	spFtClass = spObjClass;
	long nIndex = -1;
	CComVariant vtTmp;
	//��ȡ����Դ����
	CComQIPtr<ISpatialReference> spSpatial;
	CComQIPtr<IGeoDataset> spGeoDataset = spFtClass;
	spGeoDataset->get_SpatialReference(&spSpatial);
	spGeometry->Project(spSpatial);
	//�ؿ����
	CString strMJ;
	double nArea;
	CComQIPtr<IArea> spArea;
	spArea = spGeometry;
	spArea->get_Area(&nArea);
	strMJ.Format(_T("%.4lf"),nArea);
	strFeatureFields += strMJ + _T(",");
	//�ؿ���
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
	//�ؿ�����
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
			strFeatureFields += _T("�ؿ�");
		}
	}
	strFeatureFields += _T(",");
	//��,ͼ����
	strFeatureFields += _T("��,");

	spFtClass->FindField(_T("TFH"),&nIndex);
	if (nIndex >= 0)
	{
		spFt->get_Value(nIndex,&vtTmp);
		strFeatureFields += VariantToString(vtTmp);
	}
	strFeatureFields += _T(",");
	//�ؿ���;
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
	//�������
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

	//������ʽ���ַ���
	CString sPoint;
	CString sPointType;
	//�����������ʽ��ģ��
	sPointType.Format(_T("J%%d,%%d,%%.%dlf,%%.%dlf\r\n"),m_nExpPrecision,m_nExpPrecision);
	CString sFeatureProperty;
	CString sFeaturePropertyType;
	CString sFeaturePropertyOtherType;
	//�����Ҫ������ģ�壨��ַ�������ؿ�������ؿ��ţ��ؿ����ƣ��ؿ����ͣ�
	sFeaturePropertyType.Format(_T("%%d,%%s,%%s,%%s,��,,,@\r\n"));
	//����Ҫ������ģ��
	sFeaturePropertyOtherType.Format(_T("%%d,,%%s,%%s,��,,,@\r\n"));

	IGeometryPtr ipGeometry;
	IGeometryCollectionPtr ipGeometryCollection;
	ipFt->get_ShapeCopy(&ipGeometry);
	if (ipGeometry == NULL)
	{
		return FALSE;
	}
	CheckSpatial(ipGeometry);
	ExportFeatureFieldsForCountry_P(ipFt);

	// 	//������Ҫ��ÿ��Ҫ�����һ������ͷ
	esriGeometryType eType;
	ipGeometry->get_GeometryType(&eType);
	
	//һ��Ҫ���ж��Ȧʱ��Ҫ����Ȧ�ţ�������collectionתһ��
	ipGeometry->QueryInterface(&ipGeometryCollection);
	long nCount = 0;
	ipGeometryCollection->get_GeometryCount(&nCount);
	long nPIndex = 1;//��ַ���
	long nPStartIndex = 1;//�������ʼ��ַ���λ��
	for (int i = 0; i < nCount; i++)
	{
		IGeometryPtr ipGeometryTmp;
		ipGeometryCollection->get_Geometry(i,&ipGeometryTmp);
		IPointCollectionPtr ipPointCollection(ipGeometryTmp);
		IPointPtr ipPoint;//���ڶ�������
		double dx;
		double dy;
		IPointPtr ipStartPoint;//�����Ҫ����ʼ��
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
				if (dx == dsx && j != 0)//����������һ���㣬Ӧ������ʼ��λ��һ��������ʼ���ַ��ţ���ַ��Ų��ۼ�
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
				MessageBox(_T("ѡ���д��ڲ�֧�ֵ�Ҫ������"),_T("����"),MB_OK);
				return FALSE;
				break;
			}
		}
	}
	return TRUE;}

BOOL CExpCoordDlg::ExportFeatureFieldsForCountry_P(CComQIPtr<IFeature> spFt)
{
	//��������ͷ
	CString strFeatureFields;
	//��ַ����,�ؿ����,������룬����������,�ؿ���,�ؿ�����,��¼ͼ������(�㡢�ߡ���),ͼ����,�ؿ���;,�������,@

	CComQIPtr<IGeometry> spGeometry;
	spFt->get_Shape(&spGeometry);
	//��ַ����
	CString strPointCount;
	CComQIPtr<IPointCollection> spPointCollection;
	spPointCollection = spGeometry;
	long nPointCount;
	spPointCollection->get_PointCount(&nPointCount);
	strPointCount.Format(_T("%d"),nPointCount);
	strFeatureFields += strPointCount + _T(",");
	//��ȡҪ�ؼ�
	CComQIPtr<IFeatureClass> spFtClass;
	CComQIPtr<IObjectClass> spObjClass;
	spFt->get_Class(&spObjClass);
	spFtClass = spObjClass;
	long nIndex = -1;
	CComVariant vtTmp;
	//��ȡ����Դ����
	CComQIPtr<ISpatialReference> spSpatial;
	CComQIPtr<IGeoDataset> spGeoDataset = spFtClass;
	spGeoDataset->get_SpatialReference(&spSpatial);
	spGeometry->Project(spSpatial);
	//�ؿ����
	CString strMJ;
	double nArea;
	CComQIPtr<IArea> spArea;
	spArea = spGeometry;
	spArea->get_Area(&nArea);
	strMJ.Format(_T("%.4lf"),nArea);
	strFeatureFields += strMJ + _T(",");
	//�������
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
	//����������
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
	//�ؿ���
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
	//�ؿ�����
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
			strFeatureFields += _T("�ؿ�");
		}
	}
	strFeatureFields += _T(",");
	//��,ͼ����
	strFeatureFields += _T("��,");

	spFtClass->FindField(_T("TFH"),&nIndex);
	if (nIndex >= 0)
	{
		spFt->get_Value(nIndex,&vtTmp);
		strFeatureFields += VariantToString(vtTmp);
	}
	strFeatureFields += _T(",");
	//�ؿ���;
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
	//�������
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


