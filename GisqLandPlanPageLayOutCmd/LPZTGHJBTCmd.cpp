// LPZTGHJBTCmd.cpp : CLPZTGHJBTCmd ��ʵ��

#include "stdafx.h"
#include "LPZTGHJBTCmd.h"
#include "LPJBTSetDlg.h"

//ʵʱ��ط��������¼�������
class CSink :  
	public CComObjectRoot,  
	public _IGisqLandPlanAnslysisEvents  
{  
	BEGIN_COM_MAP(CSink)  
		COM_INTERFACE_ENTRY(IDispatch)  
		COM_INTERFACE_ENTRY(_IGisqLandPlanAnslysisEvents)  
	END_COM_MAP()  

public:  
	virtual ~CSink(){} 
	STDMETHODIMP SetGisqProgressBar(CComQIPtr<GisqLib::IGSProgressBar> spGSProgressBar)
	{
		HRESULT hr = S_OK;
		if(spGSProgressBar == NULL)return E_FAIL;
		m_spGSProgressBar = spGSProgressBar;
		return S_OK;
	}
	STDMETHODIMP GetTypeInfoCount(UINT *pctinfo) { return E_NOTIMPL; }  
	STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)   { return E_NOTIMPL; }  
	STDMETHODIMP GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)  { return E_NOTIMPL; }  

	//�¼��ص�
	STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)  
	{  
		/*CString strMSG(*((BSTR*)(pDispParams->rgvarg[0].byref)));
		BCGPMessageBox((LPCTSTR)strMSG,_T(""),MB_OK);*/
		if(m_spGSProgressBar != NULL)
		{
			m_spGSProgressBar->SetPos(*((long*)pDispParams->rgvarg[1].byref));
		}

		return S_OK;  
	}  
	CComQIPtr<GisqLib::IGSProgressBar> m_spGSProgressBar;
};  

// CLPZTGHJBTCmd

// STDMETHODIMP CLPZTGHJBTCmd::get_Enabled(VARIANT_BOOL * bEnabled )
// {
// 	//��ȡ"������"��Ϣ
// 	CComQIPtr<ICLPXZQWindowCmd> spCLPXZQWindowCmd;
// 	CComQIPtr<GisqLib::_DGisqFrameworkCOM> spGisqFramework = m_ipFramework;
// 	CComQIPtr<GisqLib::IGSDockableWindowManager> spGSDockableWindowManager = spGisqFramework->GetDockableWindowManager();
// 	short nCount = spGSDockableWindowManager->Count();
// 	for(short i = 0;i < nCount;i++)
// 	{
// 		CComQIPtr<GisqLib::IGSDockableWindow> spGSDockableWindow =	spGSDockableWindowManager->Item(i);
// 		_bstr_t strTabGUID = spGSDockableWindow->GetDockWindowDefGUID();
// 		if (strTabGUID == _bstr_t("GisqLandPlanCmd.CLPXZQWindowCmd.1"))
// 		{
// 			spCLPXZQWindowCmd = spGSDockableWindow->GetDockableWindowTabDef();
// 			break;
// 		}
// 
// 	}	
// 	if(spCLPXZQWindowCmd != NULL)
// 	{
// 		spCLPXZQWindowCmd->GetSelectedRegion(&m_bstrXZQName,&m_bstrXZQCode);
// 	}
// 	*bEnabled = m_bstrXZQCode == NULL ? VARIANT_FALSE:VARIANT_TRUE;
// 	return S_OK;
// }

STDMETHODIMP CLPZTGHJBTCmd::OnClick()
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComQIPtr<GisqLib::_DGisqFrameworkCOM> spGisqFramework = m_ipFramework;
	BOOL bIsAnalysisVisible = TRUE;
	//���ع滮�����ռ䣨�����õع����������ռ䣩
	CComQIPtr<IWorkspace> spGHWorkspace;
	CComQIPtr<GisqLib::IGSDataExchangeS> spGSDataExchangeS = spGisqFramework->GetDataExchangeS();
	short index = spGSDataExchangeS->FindIndex(_bstr_t(_T("GHWS")));
	if (index >= 0)
	{
		CComQIPtr<GisqLib::IGSDataExchange> spGSDataExchange = spGSDataExchangeS->Item(index);
		CComVariant var = spGSDataExchange->Item(0);
		spGHWorkspace = var.punkVal;
	}
	if (spGHWorkspace == NULL)
	{
		BCGPMessageBox(_T("�滮�����ռ�δ��ʼ����"),_T("��ʾ"),MB_OK);
		return S_OK;
	}
	CComQIPtr<IPropertySet> spPropertySet;
	spGHWorkspace->get_ConnectionProperties(&spPropertySet);
	VARIANT vtGHINSTANCE = vtMissing;
	spPropertySet->GetProperty(_bstr_t(_T("INSTANCE")),&vtGHINSTANCE);
	CString strGHINSTANCE(vtGHINSTANCE.bstrVal);

	//��鱾�����ݼ��Ƿ���ѡ�е���ͼ��
	CComQIPtr<IEnvelope> spEnvelope;//��������ں�����1:10000ʱ�Ƿ񳬳��߿�
	CArray<CComQIPtr<IFeature>,CComQIPtr<IFeature>&> arryFt;//��Ŀ�ؿ�����
	spEnvelope.CoCreateInstance(CLSID_Envelope);
	CComQIPtr<GisqLib::IGSActiveView> ipGSActiveView;
	CComQIPtr<IMap> spMap;
	CComQIPtr<IMapControl2> spMapCtrl;
	for (int i = 0;i < spGisqFramework->GetViewCount();i++)
	{
		ipGSActiveView =spGisqFramework->GetViewItem(i);
		if (ipGSActiveView->GetViewType() == (short)VT_MapControlView)
		{
			spMapCtrl = ipGSActiveView->GetControl();
			spMapCtrl->get_Map(&spMap);
			break;
		}
	}
	if (spMap == NULL) return E_FAIL;

	CComQIPtr<ISelection> spSelection;
	spMap->get_FeatureSelection(&spSelection);
	CComQIPtr<IEnumFeature> spEnumFt = spSelection;
	CComQIPtr<IFeature> spFt;
	spEnumFt->Next(&spFt);
	if (spFt == NULL)
	{
		int nIsContinue;
		nIsContinue = BCGPMessageBox(_T("��ǰδѡ����Ŀ�ؿ飬�Ƿ�����Ե�ǰ��ͼ���ĳ�ͼ"),_T("δѡ�еؿ�"),MB_OKCANCEL);
		if (nIsContinue == IDOK)//����ǣ�ѡ��
		{
			bIsAnalysisVisible = FALSE;			
		}else if (nIsContinue == IDCANCEL)
		{
			return S_OK;
		}
	}else
	{
		//��¼��һ����õ�Ҫ�ؼ���У���Ƿ�ѡ�ж��ѡ��
		CComQIPtr<IObjectClass> spFstObjClass;		
		while(spFt != NULL)
		{
			CComQIPtr<IObjectClass> spObjClass;
			spFt->get_Class(&spObjClass);
			CComQIPtr<IFeatureClass> spFtClass = spObjClass;
			CComQIPtr<IDataset> spDataset = spFtClass;
			if (spDataset != NULL)
			{
				CComQIPtr<IWorkspace> spTempWorkspace;
				spDataset->get_Workspace(&spTempWorkspace);
				//���˵��滮���еĲ�
				esriWorkspaceType enumWorkspaceType;
				spTempWorkspace->get_Type(&enumWorkspaceType);
				if (enumWorkspaceType != esriRemoteDatabaseWorkspace)
				{//����Ǳ�������
					esriGeometryType enumGeometryType;
					spFtClass->get_ShapeType(&enumGeometryType);
					//���˵������
					if (enumGeometryType == esriGeometryPolygon)
					{
						if (spFstObjClass == NULL)
						{
							spFstObjClass = spObjClass;
						}else if (spObjClass != spFstObjClass)
						{
							BCGPMessageBox(_T("��ǰѡ�ж���滮�����ͼ�㣬\n�ֲ�ͼ��ͼ��֧�ֶ���Ŀͼ�㣡"),_T("��Ŀͼ����࣡"),MB_OK);
							return E_FAIL;
						}
						arryFt.Add(spFt);
						CComQIPtr<IGeometry> spGeo;
						CComQIPtr<IEnvelope> spEnv;
						spFt->get_Shape(&spGeo);
						spGeo->get_Envelope(&spEnv);
						spEnvelope->Union(spEnv);
					}
				}else
				{//������Ǳ�������
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
							if (spFstObjClass == NULL)
							{
								spFstObjClass = spObjClass;
							}else if (spObjClass != spFstObjClass)
							{
								BCGPMessageBox(_T("��ǰѡ�ж���滮�����ͼ�㣬\n�ֲ�ͼ��ͼ��֧�ֶ���Ŀͼ�㣡"),_T("��Ŀͼ����࣡"),MB_OK);
								return E_FAIL;
							}
							arryFt.Add(spFt);
							CComQIPtr<IGeometry> spGeo;
							CComQIPtr<IEnvelope> spEnv;
							spFt->get_Shape(&spGeo);
							spGeo->get_Envelope(&spEnv);
							spEnvelope->Union(spEnv);
						}
					}
				}
			}
			spFt.Release();
			spEnumFt->Next(&spFt);
		}
		if (arryFt.GetCount() == 0)
		{
			int nIsContinue;
			nIsContinue = BCGPMessageBox(_T("��ǰδѡ�й滮�������Ŀ�ؿ飬�Ƿ�����Ե�ǰ��ͼ���ĳ�ͼ"),_T("δѡ����Ŀ�ؿ�"),MB_OK);
			if (nIsContinue == IDOK)
			{
				bIsAnalysisVisible = FALSE;
			}else if (nIsContinue == IDCANCEL)
			{
				return S_OK;
			}
		}
	}

	

	//��������ģ��������ļ�����
	TCHAR szAppFullPath[MAX_PATH];
	DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(CHAR);
	GetModuleFileName(NULL,szAppFullPath,dBufferSize);
	CString strAppPath(szAppFullPath);
	strAppPath = strAppPath.Mid(0,strAppPath.ReverseFind('\\'));
	CString strConfigFile = strAppPath + CONFIGPATH;
	CString strSection(_T("ZTGHJBT"));

	//��ȡ����������
	CString strMapTitle =_T("������������滮�ֲ�ͼ��2016-2020�꣩");//�ֲ�ͼ�����޸�

	//�ؼ���ѯ��Ҫ�õ���ͼ��
	CComQIPtr<IFeature> spFtTmp;
	CComQIPtr<IGeometry> spGeoTmp;
	if (arryFt.GetCount() == 0)//���û��ѡ��ͼ��,��ȡ��ǰ��ͼ����
	{
		CComQIPtr<IActiveView> spActiveView;
		spMapCtrl->get_ActiveView(&spActiveView);
		CComQIPtr<IEnvelope> spTmpEnv;
		//spMap->put_MapScale(10000);
		//spMapCtrl->Refresh(esriViewAll);
		spActiveView->get_Extent(&spTmpEnv);
		double dbXMax,dbXMin,dbYMax,dbYMin;
		spTmpEnv->get_XMax(&dbXMax);
		spTmpEnv->get_XMin(&dbXMin);
		spTmpEnv->get_YMax(&dbYMax);
		spTmpEnv->get_YMin(&dbYMin);
		double CenterX,CenterY;
		CenterX = ( dbXMax + dbXMin ) / 2;
		CenterY = ( dbYMax + dbYMin ) / 2;
		CComQIPtr<IPoint> spTmpPoint;
		spTmpPoint.CoCreateInstance(CLSID_Point);
		spTmpPoint->put_X(CenterX);
		spTmpPoint->put_Y(CenterY);
		
		spGeoTmp = spTmpPoint;
		spEnvelope = spTmpEnv;
	}else
	{
		spFtTmp = arryFt.GetAt(0);
		spFtTmp->get_Shape(&spGeoTmp);
	}

	//��ȡ����������ͼ��
	CComQIPtr<IUID> spUID;
	spUID.CoCreateInstance(CLSID_UID);
	CComQIPtr<IEnumLayer> spEnumLayer;
	spUID->put_Value(_variant_t("{40A9E885-5533-11d0-98BE-00805F7CED21}"));
	spMap->get_Layers(spUID,VARIANT_TRUE,&spEnumLayer);
	if (spEnumLayer == NULL) return E_FAIL;
	CComQIPtr<ILayer> spLayer;

	spEnumLayer->Next(&spLayer);
	while(spLayer != NULL)
	{
		BSTR bstrLayerName;
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
		spFtClass->get_AliasName(&bstrLayerName);
		CComQIPtr<IDataset> spDataset;
		spDataset = spFtClass;
		spDataset->get_Name(&bstrLayerName);
		CString strFtName;
		strFtName.Format(_T("%s"),bstrLayerName);
		int nIndex = -1;
		nIndex = strFtName.Find(_T("."));
		if (nIndex >= 0)
		{
			strFtName = strFtName.Right(strFtName.GetLength() - nIndex - 1);
		}
		if (strFtName.Compare(_T("XZQXZ")) == 0)
		{
			//�ռ��ѯ
			BSTR bstrShapeFieldName;
			spFtClass->get_ShapeFieldName(&bstrShapeFieldName);
			CComQIPtr<ISpatialFilter> spSpatailFilter;
			spSpatailFilter.CoCreateInstance(CLSID_SpatialFilter);
			spSpatailFilter->putref_Geometry(spGeoTmp);
			spSpatailFilter->put_SpatialRel(esriSpatialRelIntersects);
			spSpatailFilter->put_GeometryField(bstrShapeFieldName);

			CComQIPtr<IFeatureCursor> spFtCurSor;
			spFtLayer->Search(spSpatailFilter,VARIANT_FALSE,&spFtCurSor);
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
					//�����ȡ��һ����������У�˵���������ˣ��������������
					spXZFt.Release();
					spFtCurSor->NextFeature(&spXZFt);
					if (spXZFt == NULL)//û����һ������
					{
						CString strXZQX(vt.bstrVal);
						strMapTitle = strXZQX + strMapTitle;
					}
				}
			}
		}
		spLayer.Release();
		spEnumLayer->Next(&spLayer);
	}
	//ͬ���Ĳ�����������ͼ������һ��
// 	CComQIPtr<IUID> spUID;
// 	spUID.CoCreateInstance(CLSID_UID);
// 	CComQIPtr<IEnumLayer> spEnumLayer;
// 	spUID->put_Value(_variant_t("{40A9E885-5533-11d0-98BE-00805F7CED21}"));
// 	spMap->get_Layers(spUID,VARIANT_TRUE,&spEnumLayer);
// 	if (spEnumLayer == NULL) return E_FAIL;
	//CComQIPtr<ILayer> spLayer;
	if (spLayer != NULL)
	{
		spLayer.Release();
	}
	
	spEnumLayer->Reset();
	spEnumLayer->Next(&spLayer);
	while(spLayer != NULL)
	{
		BSTR bstrLayerName;
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
		spFtClass->get_AliasName(&bstrLayerName);
		CComQIPtr<IDataset> spDataset;
		spDataset = spFtClass;
		spDataset->get_Name(&bstrLayerName);
		CString strFtName;
		strFtName.Format(_T("%s"),bstrLayerName);
		int nIndex = -1;
		nIndex = strFtName.Find(_T("."));
		if (nIndex >= 0)
		{
			strFtName = strFtName.Right(strFtName.GetLength() - nIndex - 1);
		}
		if (strFtName.Compare(_T("XZQX")) == 0)
		{
			//�ռ��ѯ
			BSTR bstrShapeFieldName;
			spFtClass->get_ShapeFieldName(&bstrShapeFieldName);
			CComQIPtr<ISpatialFilter> spSpatailFilter;
			spSpatailFilter.CoCreateInstance(CLSID_SpatialFilter);
			spSpatailFilter->putref_Geometry(spGeoTmp);
			spSpatailFilter->put_SpatialRel(esriSpatialRelIntersects);
			spSpatailFilter->put_GeometryField(bstrShapeFieldName);

			CComQIPtr<IFeatureCursor> spFtCurSor;
			spFtLayer->Search(spSpatailFilter,VARIANT_FALSE,&spFtCurSor);
			CComQIPtr<IFeature> spXZFt;
			spFtCurSor->NextFeature(&spXZFt);
			if(spXZFt != NULL)
			{
				CComQIPtr<IFields> spFields;
				spXZFt->get_Fields(&spFields);
				long nIndex = -1;
				spFields->FindField(_T("XZQMC"),&nIndex);
				if (nIndex >= 0)
				{
					VARIANT vt;
					spXZFt->get_Value(nIndex,&vt);
					spXZFt.Release();
					spFtCurSor->NextFeature(&spXZFt);
					if (spXZFt != NULL)//������ֱ����ֹ
					{
						BCGPMessageBox(_T("ѡ��ĵؿ鷶Χ���ڶ��������"),_T("����"),MB_OK);
						return S_FALSE;
					}
					CString strXZQX(vt.bstrVal);
					strMapTitle = strXZQX + strMapTitle;
					break;
				}
			}
		}
		spLayer.Release();
		spEnumLayer->Next(&spLayer);
	}

	//�������öԻ��򣬳�ͼ��ΧУ����������ɵ��ȷ��֮�����
	CLPJBTSetDlg dlg;
	dlg.InitViewConfig(strConfigFile,strSection);
	dlg.SetIsAnalysisVisible(bIsAnalysisVisible);
	dlg.m_strMapConfig.SetAt(_T("MAPTITLE"),strMapTitle);
	if (dlg.DoModal() == IDOK)
	{
		//�л���ͼ
		for (int i = 0;i < spGisqFramework->GetViewCount();i++)
		{
			ipGSActiveView =spGisqFramework->GetViewItem(i);
			if (ipGSActiveView->GetViewType() == (short)VT_PageLayoutView)
			{
				GisqLib::IGSActiveView* ipTemp = ipGSActiveView;
				spGisqFramework->SetActiveView(ipTemp);
				break;
			}
		}
		CComQIPtr<IPageLayoutControl> ipPageLayoutControl = ipGSActiveView->GetControl();
		//��ȡ���ã���ģ��
		CString strMDXName;
		if (dlg.m_bIsAnalysis == TRUE)
		{
			
			dlg.m_strMapConfig.Lookup(_T("MODELPATH_ANALYSIS"),strMDXName);
		}else
		{
			dlg.m_strMapConfig.Lookup(_T("MODELPATH_NO_ANALYSIS"),strMDXName);
		}
		if (strMDXName.GetLength() <= 0) return E_FAIL;
		//��ȡMXD�ĵ�����Ŀ¼
		TCHAR szAppFullPath[MAX_PATH];
		DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(CHAR);
		GetModuleFileName(NULL,szAppFullPath,dBufferSize);
		CString strAppFolder(szAppFullPath);
		strAppFolder = strAppFolder.Left(strAppFolder.ReverseFind('\\'));
		CString strMXDFilePath = strAppFolder + _T("\\data\\template\\");
		strMXDFilePath += strMDXName;
		_bstr_t strDocumentPath = strMXDFilePath;
		VARIANT_BOOL bSecceed;
		ipPageLayoutControl->CheckMxFile(strDocumentPath,&bSecceed);
		if(bSecceed == VARIANT_FALSE)return E_FAIL;
		HRESULT hr = ipPageLayoutControl->LoadMxFile(strDocumentPath);
		if (FAILED(hr))return hr;

		//��ȡͼ������,�жϵؿ鷶Χ�Ƿ񳬳���ʾ����
		int nIsLayout = IDOK;
		BOOL bIsOutofView = FALSE;

		CComQIPtr<IGraphicsContainer> ipGraphicsContainer;
		ipPageLayoutControl->get_GraphicsContainer(&ipGraphicsContainer);
		ipGraphicsContainer->Reset();
		CComQIPtr<IElement> ipElement;
		ipGraphicsContainer->Next(&ipElement);
		CComQIPtr<IMap> ipLayoutMap;
		CComQIPtr<IEnvelope> spMapRect;
		CComQIPtr<ISpatialReference> spSpatial;
		//��ȡ��ǰmap�����꣬��ֵ������map
		spMap->get_SpatialReference(&spSpatial);
		CComBSTR bstrSpatialName;
		spSpatial->get_Name(&bstrSpatialName);

		//����һ��mapframe���������������
		CComQIPtr<IMapFrame> spMapFrame;
		while(ipElement != NULL)
		{
			CComQIPtr<IEnvelope> ipEnvelope;
			CComQIPtr<IMapFrame> ipMapFrame = ipElement;
			if (ipMapFrame != NULL)
			{
				spMapFrame = ipMapFrame;
				ipMapFrame->get_Map(&ipLayoutMap);
				ipLayoutMap->putref_SpatialReference(spSpatial);
				CComQIPtr<IActiveView> ipActiveView = ipLayoutMap;
				ipLayoutMap->put_MapScale(10000);
				ipPageLayoutControl->Refresh(esriViewAll);
				ipActiveView->get_Extent(&spMapRect);
				CComQIPtr<IPoint> spPoint;
				double dbDKWidth,dbDKHeight;
				double dbMapWidth,dbMapHeight;
				if (bIsAnalysisVisible == TRUE)//���������ѡ�еؿ�ģ����û��ѡ�еؿ�����Ҫ��֤��Χ
				{
					spEnvelope->get_Height(&dbDKHeight);
					spEnvelope->get_Width(&dbDKWidth);
					spMapRect->get_Height(&dbMapHeight);
					spMapRect->get_Width(&dbMapWidth);
					if (dbMapWidth < dbDKWidth || dbMapHeight < dbDKHeight)
					{		
						bIsOutofView = TRUE;
						nIsLayout = BCGPMessageBox(_T("�ؿ鷶Χ����ͼ��Χ���Ƿ������ͼ"),_T("��Χ���ޣ�"),MB_OKCANCEL);
						break;
					}
				}				
			}
			ipElement.Release();
			ipGraphicsContainer->Next(&ipElement);
		}
		if (nIsLayout == IDCANCEL)//���ѡ���˳������л��ص�ͼ��ͼ
		{
			for (int i = 0;i < spGisqFramework->GetViewCount();i++)
			{
				ipGSActiveView =spGisqFramework->GetViewItem(i);
				if (ipGSActiveView->GetViewType() == (short)VT_MapControlView)
				{
					GisqLib::IGSActiveView* ipTemp = ipGSActiveView;
					spGisqFramework->SetActiveView(ipTemp);
					return S_FALSE;
				}
			}
		}

		if (bIsOutofView == FALSE)
		{
			//�ƶ����ؿ��е�Ϊ����
			double DKXMAX,DKXMIN,DKYMIN,DKYMAX;
			CComQIPtr<IPoint> spPoint;
			spPoint.CoCreateInstance(CLSID_Point);
			spEnvelope->get_XMax(&DKXMAX);
			spEnvelope->get_XMin(&DKXMIN);
			spPoint->put_X(((DKXMAX+DKXMIN)/2));
			spEnvelope->get_YMax(&DKYMAX);
			spEnvelope->get_YMin(&DKYMIN);
			spPoint->put_Y(((DKYMAX+DKYMIN)/2));
			spMapRect->CenterAt(spPoint);
			CComQIPtr<IActiveView> ipActiveView = ipLayoutMap;
			ipActiveView->put_Extent(spMapRect);
		}else
		{
			//�Ե�ǰ��ͼ����Ϊ����
			CComQIPtr<IActiveView> spActiveView;
			spMapCtrl->get_ActiveView(&spActiveView);
			CComQIPtr<IEnvelope> spTmpEnv;
			//spMap->put_MapScale(10000);
			//spMapCtrl->Refresh(esriViewAll);
			spActiveView->get_Extent(&spTmpEnv);
			double dbXMax,dbXMin,dbYMax,dbYMin;
			spTmpEnv->get_XMax(&dbXMax);
			spTmpEnv->get_XMin(&dbXMin);
			spTmpEnv->get_YMax(&dbYMax);
			spTmpEnv->get_YMin(&dbYMin);
			double CenterX,CenterY;
			CenterX = ( dbXMax + dbXMin ) / 2;
			CenterY = ( dbYMax + dbYMin ) / 2;
			CComQIPtr<IPoint> spPoint;
			spPoint.CoCreateInstance(CLSID_Point);
			spPoint->put_X(CenterX);
			spPoint->put_Y(CenterY);

			spMapRect->CenterAt(spPoint);
			CComQIPtr<IActiveView> ipActiveView = ipLayoutMap;
			ipActiveView->put_Extent(spMapRect);
		}
		//����ѡ����ƴ�ӹ���������ֻ��ʾѡ�еؿ�
		CString strSQLWhere = _T("");
		long nDKOIDIndex;
		for (int nFt = 0; nFt < arryFt.GetCount(); nFt++)
		{
			CComQIPtr<IFeature> spDKFt = arryFt.GetAt(nFt);
			if (nFt == 0)
			{
				CComQIPtr<IObjectClass> spObjClass;
				CComQIPtr<IFeatureClass> spFtClass;
				spDKFt->get_Class(&spObjClass);
				spFtClass = spObjClass;
				CComBSTR bstrOIDName;
				spFtClass->get_OIDFieldName(&bstrOIDName);
				CString strOIDName = bstrOIDName;
				spFtClass->FindField(bstrOIDName,&nDKOIDIndex);
				strSQLWhere += strOIDName + _T(" in (");
				CComVariant vtTmp;
				spDKFt->get_Value(nDKOIDIndex,&vtTmp);
				strSQLWhere += VariantToString(vtTmp);
			}else
			{
				CComVariant vtTmp;
				spDKFt->get_Value(nDKOIDIndex,&vtTmp);
				strSQLWhere += _T(",") + VariantToString(vtTmp);
			}
		}
		strSQLWhere += _T(")");
		//��ӵؿ�����ͼ��
		CComQIPtr<IObjectClass> spStaticDKClass;
		for (int nFt = 0; nFt < arryFt.GetCount(); nFt++)
		{
			CComQIPtr<IFeature> spDKFt = arryFt.GetAt(nFt);
			CComQIPtr<IObjectClass> spObjDKClass;
			spDKFt->get_Class(&spObjDKClass);
			if (spStaticDKClass == NULL)
			{
				spStaticDKClass = spObjDKClass;
			}else
			{
				continue;
			}
			CComQIPtr<IFeatureClass> spDKFtClass = spObjDKClass;
			CComQIPtr<ILayer> spDKLayer;
			spDKLayer.CoCreateInstance(CLSID_FeatureLayer);
			CComQIPtr<IFeatureLayer> spDKFtlayer = spDKLayer;
			spDKFtlayer->putref_FeatureClass(spDKFtClass);
			//������Ⱦ��ʽ
			// ����symbol
			IColorPtr ipRgbColor(CLSID_RgbColor);
			ISimpleFillSymbolPtr ipStylePg(CLSID_SimpleFillSymbol);
			ISimpleLineSymbolPtr ipStyLn(CLSID_SimpleLineSymbol);
			//�߽���ʽ
			ipRgbColor->put_RGB(RGB(0,76,155));
			ipStyLn->put_Style(esriSLSSolid);
			ipStyLn->put_Color(ipRgbColor);
			ipStyLn->put_Width(2);
			//�������ʽ
			ipStylePg->put_Color(ipRgbColor);
			ipStylePg->put_Style(esriSFSNull);
			//ipStylePg->put_Style(esriSFSBackwardDiagonal);
			ipStylePg->put_Outline(ipStyLn);
			//���ͼ����Ⱦ��ʽ
			CComQIPtr<ISimpleRenderer> spSimpleRenderer;
			spSimpleRenderer.CoCreateInstance(CLSID_SimpleRenderer);
			CComQIPtr<ISymbol> spSymbol = ipStylePg;
			spSimpleRenderer->putref_Symbol(spSymbol);
			CComQIPtr<IFeatureRenderer> spFtRenderer = spSimpleRenderer;
			CComQIPtr<IGeoFeatureLayer> spGeoFtLayer = spDKLayer;
			spGeoFtLayer->putref_Renderer(spFtRenderer);
			//���ͼ����
			//CString strLayerName;
			//strLayerName.Format(_T("��Ŀ�ؿ��%d"),(nFt+1));
			//spDKLayer->put_Name(_bstr_t(strLayerName));
			spDKLayer->put_Name(_T("��Ŀ�ؿ��"));

			CComQIPtr<IFeatureLayerDefinition> spFtLayerDefinition = spDKLayer;
			CComBSTR bstrDefinition;
			spFtLayerDefinition->get_DefinitionExpression(&bstrDefinition);
			bstrDefinition = strSQLWhere;
			spFtLayerDefinition->put_DefinitionExpression(bstrDefinition);

			ipLayoutMap->AddLayer(spDKLayer);

			//������Ҫ���һ������label
			//��ȡ��ע���ýӿ�
// 			CComQIPtr<IAnnotateLayerPropertiesCollection> spAnnotateLayerPropertiesCollection;
// 			CComQIPtr<IAnnotateLayerProperties> spAnnotateLayerProperties;
// 			CComQIPtr<ILabelEngineLayerProperties> spLabelEngineLayerProperties;
// 			//CComQIPtr<IBasicOverposterLayerProperties> spBasicOverposterLayerProperties;
// 			CComQIPtr<IElementCollection> spElementCollecyion;
// 			CComQIPtr<ITextSymbol> spTextSymbol;
// 
// 
// 			spGeoFtLayer->get_AnnotationProperties(&spAnnotateLayerPropertiesCollection);
// 			spAnnotateLayerPropertiesCollection->QueryItem(0,&spAnnotateLayerProperties,&spElementCollecyion,&spElementCollecyion);
// 			spLabelEngineLayerProperties = spAnnotateLayerProperties;
// 			spLabelEngineLayerProperties->get_Symbol(&spTextSymbol);
// 			//����������Ϣ
// 			CComQIPtr<IFontDisp> spFontDisp;
// 			spTextSymbol->get_Font(&spFontDisp);
// 			//������ɫ
// 			CComQIPtr<IColor> spColor;
// 			spColor.CoCreateInstance(CLSID_RgbColor);
// 			spColor->put_RGB(RGB(255,0,0));
// 			spTextSymbol->put_Color(spColor);
// 			//��������
// 			CComQIPtr<IFont> spFont = spFontDisp;
// //			spFont->put_Name(_T("����"));
// 			//�����С
// 			CY cySize;
// 			spFont->get_Size(&cySize);
// 			cySize.int64 = 20*10000;
// 			spFont->put_Size(cySize);
// 			//������ʽ
// 			spFont->put_Bold(TRUE);//�Ӵ�
// // 			spFont->put_Italic(FALSE);//б��
// // 			spFont->put_Underline(FALSE);//�»���
// // 
// 			spTextSymbol->put_Font(spFontDisp);
// 			
// 			hr = spLabelEngineLayerProperties->put_IsExpressionSimple(VARIANT_TRUE);
// 			CString strLabelField;
// 			CString strLableString;
// 			dlg.m_strMapConfig.Lookup(_T("MAPLABEL_FIELD"),strLabelField);
// 			strLableString.Format(_T("[%s]"),strLabelField);
// 			hr = spLabelEngineLayerProperties->put_Expression(_bstr_t(strLableString));
// 			//hr = spLabelEngineLayerProperties->putref_Symbol(spTextSymbol);
// 			hr = spGeoFtLayer->put_DisplayAnnotation(VARIANT_TRUE);

		}
		//�ٱ���һ�飬���callout
		for (int i = 0; i < arryFt.GetCount(); i++)
		{
			AddCallout(ipPageLayoutControl,spMapFrame,arryFt.GetAt(i));
		}

		//ipPageLayoutControl->Refresh(esriViewAll);
		

		//����ѡַ�����ӿڽ������ݷ���
		if (dlg.m_bIsAnalysis == TRUE)
		{
			//�������������ʱGDB��dataset
			CreateGDBResult();
			//������Ŵ�����Ŀ���Ҫ�ؼ�
			CreateXMDKFeatureClass();

			//��Ҫ�����ĵؿ�д��Ҫ�ؼ���
			//XMDK��DKMC�ֶ�����
			long nIndexDKMC = -1;
			m_spXMDKFeatureClass->FindField(_bstr_t(_T("DKMC")),&nIndexDKMC);
			if (-1 == nIndexDKMC)return E_FAIL;
			//XMDK��DKID�ֶ�����
			long nIndexDKID = -1;
			m_spXMDKFeatureClass->FindField(_bstr_t(_T("DKID")),&nIndexDKID);
			if(-1 == nIndexDKID)return E_FAIL;

			for (int i = 0; i < arryFt.GetCount(); i++)
			{
				CComQIPtr<IFeature> spFeature = arryFt.GetAt(i);
				ASSERT(spFeature != NULL);
				//����ϵ�任
				CComQIPtr<IGeometry> spGeometry;
				CComQIPtr<ISpatialReference> spFeatureSpatialRef;
				spFeature->get_ShapeCopy(&spGeometry);
				spGeometry->get_SpatialReference(&spFeatureSpatialRef);

				CComQIPtr<ISpatialReference> spTargetSpatialRef;
				CComQIPtr<IGeoDataset> spGeoDataset = m_spXMDKFeatureClass;
				spGeoDataset->get_SpatialReference(&spTargetSpatialRef);
				spGeometry->Project(spTargetSpatialRef);

				CComQIPtr<IFeatureBuffer> spFeatureBuffer;
				m_spXMDKFeatureClass->CreateFeatureBuffer(&spFeatureBuffer);
				ASSERT(spFeatureBuffer != NULL);
				//���ò���Ҫ�ؼ�������
				if(spGeometry != NULL)spGeometry.Release();
				spFeature->get_ShapeCopy(&spGeometry);
				spFeatureBuffer->putref_Shape(spGeometry);
				//���ò���Ҫ�ص�DKMC�����ֶ�ֵ
				spFeatureBuffer->put_Value(nIndexDKMC,_variant_t(i+1));
				//���ò���Ҫ��DKID�ֶ�ֵ(���ﰴ�ղ���˳�����ID���ɣ�����ʹ��GUID�ȸ��ӵ�Ψһ��ʶ��)
				spFeatureBuffer->put_Value(nIndexDKID,_variant_t(i+1));
				//����Ҫ��
				CComQIPtr<IFeatureCursor> spFeatureCursor;
				m_spXMDKFeatureClass->Insert(VARIANT_TRUE,&spFeatureCursor);
				_variant_t vtOID;
				spFeatureCursor->InsertFeature(spFeatureBuffer,&vtOID);
			}
			//������
			CComQIPtr<GisqLib::IGSProgressBar> spGSProgressBar = spGisqFramework->GetProgressBar();
			spGSProgressBar->SetRange(0,100);
			spGSProgressBar->Show();
			//����ͳ�Ʒ���ģ��
			CComQIPtr<IGisqLandPlanAnslysis> spGisLandPlanAnalysis;
			spGisLandPlanAnalysis.CoCreateInstance(CLSID_GisqLandPlanAnslysis);
			spGisLandPlanAnalysis->InitWorkspace(spGHWorkspace,m_spGDBWorkspace);
			spGisLandPlanAnalysis->SetCurFeatureDataset(m_spDSCurFeatureDataset);
			spGisLandPlanAnalysis->SetCurSuffix(_bstr_t("_120"));
			spGisLandPlanAnalysis->SetSTBMJMax(10);
			//ʵʱ��ط��������¼��󶨲���
			CComObject<CSink>* pSink = NULL;  
			CComObject<CSink>::CreateInstance(&pSink);
			ASSERT(pSink != NULL);
			pSink->SetGisqProgressBar(spGSProgressBar);
			DWORD cookies = 0;  
			AtlAdvise(spGisLandPlanAnalysis,pSink,__uuidof(_IGisqLandPlanAnslysisEvents), &cookies); 
			//ִ�з���
			spGisLandPlanAnalysis->ExcuteAnalysisPro();
			//����֮�����ؽ�����
			spGSProgressBar->Hide();
		
// 			//ѡַ�����������
// 			int nNYD = 0;
// 			int nNYD_GD = 0;
// 			int nJSYD = 0;
// 			int nWLYD = 0;
// 			int nAll = 0;
// 			CString strGHLYMC = _T("");
// 			CString strDKID = _T("");
// 
// 		CComQIPtr<IFeatureWorkspace> spGDBFtWork = m_spGDBWorkspace;
// 		CComQIPtr<ITable> spResultTable;
// 		spGDBFtWork->OpenTable(_T("RESULT_120"),&spResultTable);
// 		if (spResultTable == NULL)
// 		{
// 			return E_FAIL;
// 		}
// 
// 		//��ȡ�ֶ�����
// 		CComBSTR bstrDKID(_T("DKID"));
// 		CComBSTR bstrDLDM(_T("DLDM"));
// 		CComBSTR bstrGHYTDM(_T("GHYTDM"));
// 		CComBSTR bstrGHYTMC(_T("GHYTMC"));
// 		CComBSTR bstrGZQLXDM(_T("GZQLXDM"));
// 		CComBSTR bstrJQTBMJ(_T("JQTBMJ"));
// 		CComBSTR bstrGHYTMJ(_T("GHYTMJ"));
// 		CComBSTR bstrXZDWMJ(_T("XZDWMJ"));
// 		CComBSTR bstrLXDWMJ(_T("LXDWMJ"));
// 		long iDKID = -1;
// 		long iDLDM = -1;
// 		long iGHYTMC = -1;
// 		long iGHYTDM = -1;
// 		long iJQTBMJ = -1;
// 		long iGHYTMJ = -1;
// 		long iXZDWMJ = -1;
// 		long iLXDWMJ = -1;
// 		CComQIPtr<IFields> spFields;
// 		spResultTable->get_Fields(&spFields);
// 		spFields->FindField(bstrDKID,&iDKID);
// 		spFields->FindField(bstrDLDM,&iDLDM);
// 		spFields->FindField(bstrGHYTDM,&iGHYTDM);
// 		spFields->FindField(bstrGHYTMC,&iGHYTMC);
// 		spFields->FindField(bstrJQTBMJ,&iJQTBMJ);
// 		spFields->FindField(bstrGHYTMJ,&iGHYTMJ);
// 		spFields->FindField(bstrXZDWMJ,&iXZDWMJ);
// 		spFields->FindField(bstrLXDWMJ,&iLXDWMJ);
// 
// 		//���������
// 		CComQIPtr<esriICursor> spCursor;
// 		spResultTable->Search(NULL,VARIANT_TRUE,&spCursor);
// 		CComQIPtr<esriIRow> spRow;
// 		spCursor->NextRow(&spRow);
// 		while(spRow != NULL)
// 		{
// 			CComVariant vtGHLYMC;
// 			CComVariant vtGHYTDM;
// 			CComVariant vtJQTBMJ;
// 			CComVariant vtGHYTMJ;
// 			CComVariant vtXZDWMJ;
// 			CComVariant vtLXDWMJ;
// 			CComVariant vtDLDM;
// 			CComVariant vtDKID;
// 
// 			spRow->get_Value(iGHYTDM,&vtGHYTDM);
// 			spRow->get_Value(iGHYTMC,&vtGHLYMC);
// 			spRow->get_Value(iJQTBMJ,&vtJQTBMJ);
// 			spRow->get_Value(iGHYTMJ,&vtGHYTMJ);
// 			spRow->get_Value(iXZDWMJ,&vtXZDWMJ);
// 			spRow->get_Value(iLXDWMJ,&vtLXDWMJ);
// 			spRow->get_Value(iDLDM,&vtDLDM);
// 			spRow->get_Value(iDKID,&vtDKID);
// 
// 			double dbJQTBMJ = vtJQTBMJ.dblVal;
// 			double dbGHYTMJ = vtGHYTMJ.dblVal;
// 			double dbXZDWMJ = vtXZDWMJ.dblVal;
// 			double dbLXDWMJ = vtLXDWMJ.dblVal;
// 
// 			int dJQTBMJ = dbJQTBMJ;
// 			int dGHYTMJ = dbGHYTMJ;
// 			int dXZDWMJ = dbXZDWMJ;
// 			int dLXDWMJ = dbLXDWMJ;
// 
// 			if(vtGHYTDM != CComVariant(_T("")))
// 			{
// 				//ũҵ�õ�
// 				//����ũ�G11��
// 				if (vtGHYTDM == CComVariant(_T("G111")))
// 				{//ʾ��������ũ��
// 					nNYD += dGHYTMJ;
// 					nNYD_GD += dGHYTMJ;
// 					//������ũ��۳���������������Ҫ�ӵ�G15��
// 					if(vtDLDM == CComVariant(_T("111")) || vtDLDM == CComVariant(_T("113")))
// 					{
// 						nNYD += dXZDWMJ;
// 						nNYD += dLXDWMJ;
// 					} 				
// 				}
// 				if (vtGHYTDM == CComVariant(_T("G112")))
// 				{//һ�����ũ��
// 					nNYD += dGHYTMJ;
// 					nNYD_GD += dGHYTMJ;
// 					//������ũ��۳���������������Ҫ�ӵ�G15��
// 					if(vtDLDM == CComVariant(_T("111")) || vtDLDM == CComVariant(_T("113")))
// 					{
// 						nNYD += dXZDWMJ;
// 						nNYD += dLXDWMJ;
// 					} 				
// 				}
// 				//һ��ũ�N11��
// 				if (vtGHYTDM == CComVariant(_T("N111")))
// 				{//һ��ũ��
// 					if(vtDLDM == CComVariant(_T("111")) || vtDLDM == CComVariant(_T("113")))
// 					{
// 						nNYD_GD += dJQTBMJ;
// 					}
// 					else
// 					{
// 						nNYD_GD += dGHYTMJ;
// 					}
// 					nNYD += dGHYTMJ;
// 				}
// 				if (vtGHYTDM == CComVariant(_T("N112")))
// 				{//����һ��ũ��
// 					nNYD += dGHYTMJ;
// 					nNYD_GD += dGHYTMJ;
// 				}
// 				CString strTemp = VariantToString(vtGHYTDM);
// 				if(strTemp.GetLength() > 2)
// 				{
// 					//����ũ�õأ�1xx��
// 					if (strTemp[1] == _T('1') && strTemp[2] != _T('1'))
// 					{
// 						nNYD += dGHYTMJ;
// 					}
// 					//�����õأ�2xx��
// 					if (strTemp[1] == _T('2'))
// 					{
// 						nJSYD += dGHYTMJ;
// 					}
// 					//δ���õأ�3xx��
// 					if (strTemp[1] == _T('3'))
// 					{
// 						nWLYD += dGHYTMJ;
// 					}
// 				}
// 				
// 			}
// 			nAll += dGHYTMJ;
// 
// 			if (strGHLYMC.GetLength() == 0)
// 			{
// 				strGHLYMC = VariantToString(vtGHLYMC);
// 			}
// 			if (strDKID.GetLength() == 0)
// 			{
// 				strDKID = VariantToString(vtDKID);
// 			}
// 			spRow.Release();
// 			spCursor->NextRow(&spRow);
// 		}
		//ѡַ�����������
		double dbNYD = 0;
		double dbNYD_GD = 0;
		double dbJSYD = 0;
		double dbWLYD = 0;
		double dbAll = 0;
		CString strNYD = _T("");
		CString strNYD_GD = _T("");
		CString strJSYD = _T("");
		CString strWLYD = _T("");
		CString strAll = _T("");
		CString strGHLYMC = _T("");
		CString strDKID = _T("");

		IUnknown *pukTable = NULL;
		CComQIPtr<ITable> spYTResTable;
		spGisLandPlanAnalysis->GetYTResult(&pukTable);
		spYTResTable = pukTable;
		if (spYTResTable == NULL)
		{
// 			for (int i = 0;i < spGisqFramework->GetViewCount();i++)
// 			{
// 				ipGSActiveView =spGisqFramework->GetViewItem(i);
// 				if (ipGSActiveView->GetViewType() == (short)VT_MapControlView)
// 				{
// 					spGisqFramework->SetActiveView(ipGSActiveView);
// 				}
// 			}
			return E_FAIL;
		}
		//ƴ�Ӳ�ѯ�ֶ�
		CComQIPtr<IQueryFilter> spQueryFilter;
		spQueryFilter.CoCreateInstance(CLSID_QueryFilter);
		spQueryFilter->put_WhereClause(_T("DKMC = '' and DKID = ''"));
		CComQIPtr<esriICursor> spCursor;
		spYTResTable->Search(spQueryFilter,VARIANT_FALSE,&spCursor);
		CComQIPtr<esriIRow> spRow;
		spCursor->NextRow(&spRow);
		long nIndex;
		CComVariant vtTmp;
		spYTResTable->FindField(_T("ZJ"),&nIndex);//�ܼ��ֶ�
		spRow->get_Value(nIndex,&vtTmp);
		strAll = VariantToString(vtTmp);
		spYTResTable->FindField(_T("NTotal"),&nIndex);//ũ�õ��ֶ�
		spRow->get_Value(nIndex,&vtTmp);
		strNYD = VariantToString(vtTmp);
		spYTResTable->FindField(_T("QZGD"),&nIndex);//���и����ֶ�
		spRow->get_Value(nIndex,&vtTmp);
		strNYD_GD = VariantToString(vtTmp);
		spYTResTable->FindField(_T("JTotal"),&nIndex);//�����õ��ֶ�
		spRow->get_Value(nIndex,&vtTmp);
		strJSYD = VariantToString(vtTmp);
		spYTResTable->FindField(_T("WTotal"),&nIndex);//δ���õ��ֶ�
		spRow->get_Value(nIndex,&vtTmp);
		strWLYD = VariantToString(vtTmp);

 		//��10000
		dbAll = _wtof(strAll);
		dbAll /= 10000;
		strAll.Format(_T("%.4lf"),dbAll);
		dbNYD = _wtof(strNYD);
		dbNYD /= 10000;
		strNYD.Format(_T("%.4lf"),dbNYD);
		dbNYD_GD = _wtof(strNYD_GD);
		dbNYD_GD /= 10000;
		strNYD_GD.Format(_T("%.4lf"),dbNYD_GD);
		dbJSYD = _wtof(strJSYD);
		dbJSYD /= 10000;
		strJSYD.Format(_T("%.4lf"),dbJSYD);
		dbWLYD = _wtof(strWLYD);
		dbWLYD /= 10000;
		strWLYD.Format(_T("%.4lf"),dbWLYD);
		//�Ƿ���Ϲ滮
		CString strSFFHGH;
		if (dbNYD == 0.0 && dbWLYD == 0.0)
		{
			strSFFHGH = _T("��");
		}else
		{
			strSFFHGH = _T("��");
		}
		//��ȡһ���ؿ�ĵؿ����ƺ͹滮��;����
		CComQIPtr<IFeatureWorkspace> spGDBFtWork = m_spGDBWorkspace;
		CComQIPtr<ITable> spResultTable;
		spGDBFtWork->OpenTable(_T("RESULT_120"),&spResultTable);
		if (spResultTable == NULL)
		{
			return E_FAIL;
		}
		CComQIPtr<esriICursor> spDKCursor;
		spResultTable->Search(NULL,VARIANT_FALSE,&spDKCursor);
		CComQIPtr<esriIRow> spDKRow;
		spDKCursor->NextRow(&spDKRow);
		spResultTable->FindField(_T("DKID"),&nIndex);
		spDKRow->get_Value(nIndex,&vtTmp);
		strDKID = VariantToString(vtTmp);
		spResultTable->FindField(_T("GHYTMC"),&nIndex);
		spDKRow->get_Value(nIndex,&vtTmp);
		strGHLYMC = VariantToString(vtTmp);
		//д�뵽����map��
		//dlg.m_strMapConfig.SetAt(_T("GHYTMC"),strGHLYMC);
		dlg.m_strMapConfig.SetAt(_T("���"),strDKID);
		dlg.m_strMapConfig.SetAt(_T("�ϼ�"),strAll);
		dlg.m_strMapConfig.SetAt(_T("ũ�õ�"),strNYD);
		dlg.m_strMapConfig.SetAt(_T("���У�����"),strNYD_GD);
		dlg.m_strMapConfig.SetAt(_T("�����õ�"),strJSYD);
		dlg.m_strMapConfig.SetAt(_T("δ���õ�"),strWLYD);
		dlg.m_strMapConfig.SetAt(_T("�Ƿ���Ϲ滮"),strSFFHGH);
		}
		CExportThemeMap exportThemeMap((GisqLib::_DGisqFrameworkCOM*)m_ipFramework);
		exportThemeMap.ExportJBTMap(spGHWorkspace,dlg.m_strMapConfig);
	}
	return S_OK;
}

HRESULT CLPZTGHJBTCmd::CreateGDBResult()
{
	HRESULT hr = S_OK;
	TCHAR szAppFullPath[MAX_PATH];
	DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(TCHAR);
	GetModuleFileName(NULL,szAppFullPath,dBufferSize);
	CString strAppPath(szAppFullPath);
	strAppPath = strAppPath.Mid(0,strAppPath.ReverseFind(_T('\\')));
	CString strTempResultFolder = strAppPath + _T("\\data\\TempResult");
	//ʹ�õ�ǰϵͳʱ������ʱ����ļ����ݿ������ļ�������
	SYSTEMTIME st;
	CString strDate,strTime;
	GetLocalTime(&st);
	CString strCurTempFolder;
	//�ļ������ƣ�������ʱ����
	strCurTempFolder.Format(_T("%4d%02d%02d%02d%02d%02d"),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	strTempResultFolder += _T("\\") +strCurTempFolder;
	//�����ʱ�ļ��д��������´�����
	if(!PathIsDirectory((LPCTSTR)strTempResultFolder))
	{
		if (!CreateDirectory((LPCTSTR)strTempResultFolder,NULL))
		{
			::BCGPMessageBox(_T("����ͳ�Ʒ�����ʱ�ļ���ʧ��!"),_T("����"),MB_OK| MB_ICONERROR);
			return E_FAIL;
		}
	}
	else
	{
		if (RemoveDirectory((LPCTSTR)strTempResultFolder))
		{
			if (!CreateDirectory((LPCTSTR)strTempResultFolder,NULL))
			{
				::BCGPMessageBox(_T("����ͳ�Ʒ�����ʱ�ļ���ʧ��!"),_T("����"),MB_OK| MB_ICONERROR);
				return E_FAIL;
			}
		}
		else
		{
			return E_FAIL;
		}
	}


	CComQIPtr<IWorkspaceName> spWorkspaceName;
	hr = CGeometryOperation::CreateWorkspace(enmWSTGdb,_bstr_t((LPCTSTR)strTempResultFolder),_T("TempResult"),spWorkspaceName);
	if (SUCCEEDED(hr))
	{
		CComBSTR bstrPathName;
		spWorkspaceName->get_PathName(&bstrPathName);
		hr = CGeometryOperation::OpenWorkspaceFromFile(enmWSTGdb,_bstr_t(bstrPathName),m_spGDBWorkspace);
		if(m_spGDBWorkspace != NULL)
		{
			//����esriSRProjCS_Xian1980_3_Degree_GK_Zone_40����ϵ
			CComQIPtr<ISpatialReferenceFactory> spSpatialReferenceFactory;
			spSpatialReferenceFactory.CoCreateInstance(CLSID_SpatialReferenceEnvironment);
			CComQIPtr<IProjectedCoordinateSystem> spProjectedCoordinateSystem;
			spSpatialReferenceFactory->CreateProjectedCoordinateSystem(esriSRProjCS_Xian1980_3_Degree_GK_Zone_40,&spProjectedCoordinateSystem);
			CComQIPtr<ISpatialReference> spSpatialReference = spProjectedCoordinateSystem;
			if(spSpatialReference == NULL)return E_FAIL;
			//��������ϵ��������������ã����򴴽�FeatureDataset��ʧ��
			hr = CGeometryOperation::CreateSpatialReference(spSpatialReference);
			if(SUCCEEDED(hr))
			{
				CComQIPtr<IFeatureWorkspace> spGDBFeatureWorkspace = m_spGDBWorkspace;
				if(m_spDSCurFeatureDataset != NULL)m_spDSCurFeatureDataset.Release();
				hr = CGeometryOperation::CreateFeatureDataset(spGDBFeatureWorkspace,spSpatialReference,_bstr_t("DS120"),m_spDSCurFeatureDataset);
				if(m_spDSCurFeatureDataset == NULL)return E_FAIL;
			}		
		}
	}
	return hr;
}

HRESULT CLPZTGHJBTCmd::CreateXMDKFeatureClass()
{
	HRESULT hr = S_OK;
	//if (!(vecSPDKFeatures.Count() > 0))return hr;
	ASSERT(m_spGDBWorkspace != NULL);
	CComQIPtr<IFeatureWorkspace> spGDBFeatureWorkspace = m_spGDBWorkspace;	
	//�����ֶμ�
	CComQIPtr<IFeatureClassDescription> spFeatureClassDescription;
	spFeatureClassDescription.CoCreateInstance(CLSID_FeatureClassDescription);
	CComQIPtr<IObjectClassDescription> spObjectClassDescription = spFeatureClassDescription;
	
	//��Ҫ�ֶ�
	CComQIPtr<IFields> spFields;
	spObjectClassDescription->get_RequiredFields(&spFields);
	
	//Ҫ����ļ�������
	CComBSTR bstrShapeFieldName;
	spFeatureClassDescription->get_ShapeFieldName(&bstrShapeFieldName);
	long nShapeFieldName;
	spFields->FindField(bstrShapeFieldName,&nShapeFieldName);
	CComQIPtr<IField> spField;
	spFields->get_Field(nShapeFieldName,&spField);
	CComQIPtr<IGeometryDef> spGeometryDef;
	spField->get_GeometryDef(&spGeometryDef);
	//��������(esriGeometryPolygon)
	CComQIPtr<IGeometryDefEdit> spGeometryDefEdit = spGeometryDef;
	spGeometryDefEdit->put_GeometryType(esriGeometryPolygon);

	//���DKID�ֶ�
	CComQIPtr<IFieldsEdit> spFieldsEdit = spFields;
	if (spField != NULL)spField.Release();
	CComQIPtr<IFieldEdit> spFieldEdit;
	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_bstr_t(_T("DKID")));
	spFieldEdit->put_Type(esriFieldTypeInteger);
	spFieldsEdit->AddField(spField);
	//���DKMC�ֶ�
	if(spField != NULL)spField.Release();
	if(spFieldEdit != NULL)spFieldEdit.Release();
	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_bstr_t(_T("DKMC")));
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldsEdit->AddField(spField);

	//�����ֶ�
	CComQIPtr<IFieldChecker> spFieldChecker;
	spFieldChecker.CoCreateInstance(CLSID_FieldChecker);
	CComQIPtr<IEnumFieldError> spEnumFieldError;
	CComQIPtr<IFields> spValidFields;
	CComQIPtr<IWorkspace> spGDBWorkspace = spGDBFeatureWorkspace;
	spFieldChecker->putref_ValidateWorkspace(spGDBWorkspace);
	spFieldChecker->Validate(spFields,&spEnumFieldError,&spValidFields);
	
	CComQIPtr<IUID> spInstanceUID;
	CComQIPtr<IUID> spExtensionUID;
	spObjectClassDescription->get_InstanceCLSID(&spInstanceUID);
	spObjectClassDescription->get_ClassExtensionCLSID(&spExtensionUID);
	if(m_spDSCurFeatureDataset == NULL)return E_FAIL;
	CComBSTR bstrFeatureClasName(_T("XMDK_120"));
	m_spDSCurFeatureDataset->CreateFeatureClass(bstrFeatureClasName,spValidFields,spInstanceUID,spExtensionUID,esriFTSimple,_bstr_t(_T("Shape")),NULL,&m_spXMDKFeatureClass);
	return hr;
}

HRESULT CLPZTGHJBTCmd::AddCallout(CComQIPtr<IPageLayoutControl> &spPageLayoutControl, CComQIPtr<IMapFrame> spMapFrame, CComQIPtr<IFeature> spFt)
{
	HRESULT hr;
	//������ؽӿ�
	CComQIPtr<IPageLayout> spPageLayout;
	spPageLayoutControl->get_PageLayout(&spPageLayout);
	CComQIPtr<IGraphicsContainer> spGraphicsContainer = spPageLayout;
	//����Ԫ��
	CComQIPtr<ITextElement> spTextElement;
	spTextElement.CoCreateInstance(CLSID_TextElement);
	CComQIPtr<IBalloonCallout> spBalloonCallout;
	spBalloonCallout.CoCreateInstance(CLSID_BalloonCallout);
	//��������
	SetFontType(spTextElement);
	//�������ݷ��
	SetBalloonCalloutSymbol(spBalloonCallout);
	//��������
	CComBSTR bstrName;
	GetFeatureName(spFt,bstrName);
	spTextElement->put_Text(bstrName);
	//����λ��
	CComQIPtr<IPoint> spBalloonCalloutAnchorPoint;
	CComQIPtr<IPoint> spTextElementAnchorPoint;
	GetElementAnchorPoint(spPageLayoutControl,spMapFrame,spFt,spBalloonCalloutAnchorPoint,spTextElementAnchorPoint);
	CComQIPtr<IElement> spText = spTextElement;
	spText->put_Geometry(spTextElementAnchorPoint);
	spBalloonCallout->put_AnchorPoint(spBalloonCalloutAnchorPoint);
	//���ñ���Ϊ����
	CComQIPtr<ITextSymbol> spTextSymbol;
	spTextElement->get_Symbol(&spTextSymbol);
	CComQIPtr<IFormattedTextSymbol> spFormarttedTextSymblol = spTextSymbol;
	spFormarttedTextSymblol->putref_Background((CComQIPtr<ITextBackground>)spBalloonCallout);
	spTextElement->put_Symbol(spTextSymbol);
	//����Ԫ��
	spGraphicsContainer->AddElement((CComQIPtr<IElement>)spTextElement,0);
	return S_OK;
}

void CLPZTGHJBTCmd::GetFeatureName(CComQIPtr<IFeature> spFt, CComBSTR &bstrName)
{
	long nDKOIDIndex;
	CComQIPtr<IObjectClass> spObjClass;
	CComQIPtr<IFeatureClass> spFtClass;
	spFt->get_Class(&spObjClass);
	spFtClass = spObjClass;
	
	long nDKMCIndex;
	spFtClass->FindField(_T("DK_MC"),&nDKMCIndex);
	if (nDKMCIndex >= 0)
	{
		CComVariant vtTmp;
		spFt->get_Value(nDKMCIndex,&vtTmp);
		bstrName = vtTmp.bstrVal;
		return;
	}else
	{
		spFtClass->FindField(_T("DKMC"),&nDKMCIndex);
		if(nDKMCIndex >= 0)
		{
			CComVariant vtTmp;
			spFt->get_Value(nDKMCIndex,&vtTmp);
			bstrName = vtTmp.bstrVal;
			return;
		}else
		{
			CComVariant vtTmp;
			CComBSTR bstrOIDName;
			spFtClass->get_OIDFieldName(&bstrOIDName);
			spFtClass->FindField(bstrOIDName,&nDKOIDIndex);
			spFt->get_Value(nDKOIDIndex,&vtTmp);
			CString strOID = VariantToString(vtTmp);
			CString strOIDName;
			strOIDName += _T("�ؿ�") + strOID;
			bstrName = strOIDName;
			return;
		}
	}

}

void CLPZTGHJBTCmd::SetFontType(CComQIPtr<ITextElement> &spTextElement)
{
	CComQIPtr<ITextSymbol> spTextSymbol;
	spTextElement->get_Symbol(&spTextSymbol);
	//����������Ϣ
	CComQIPtr<IFontDisp> spFontDisp;
	spTextSymbol->get_Font(&spFontDisp);
	//������ɫ
	CComQIPtr<IColor> spColor;
	spColor.CoCreateInstance(CLSID_RgbColor);
	spColor->put_RGB(RGB(255,0,0));
	spTextSymbol->put_Color(spColor);
	//��������
	CComQIPtr<IFont> spFont = spFontDisp;
	//spFont->put_Name(_T("����"));
	//�����С
	//CY cySize;
	//spFont->get_Size(&cySize);
	//cySize.int64 = 20*10000;
	//spFont->put_Size(cySize);
	//������ʽ
	spFont->put_Bold(TRUE);//�Ӵ�
	spTextSymbol->put_Font(spFontDisp);
	spTextElement->put_Symbol(spTextSymbol);
}

void CLPZTGHJBTCmd::GetElementAnchorPoint(CComQIPtr<IPageLayoutControl> spPageLayoutControl, CComQIPtr<IMapFrame> spMapFrame, CComQIPtr<IFeature> spFt,
						   CComQIPtr<IPoint> &spBalloonCalloutAnchorPoint,
						   CComQIPtr<IPoint> &spTextElementAnchorPoint)
{

	//��һ���ǻ�ȡͼ��������Ե�ͼ����Ļ���	
	//�����ȡ������Ҫ�ı���
	//ͼ��Ԫ�����
	CComQIPtr<IElement> spMapFrameElement = spMapFrame;
	CComQIPtr<IGeometry> spMapFrameElementGeometry;
	CComQIPtr<IEnvelope> spMapFrameElementEnvelope;
	double dMapFrameElementHeight,dMapFrameElementWidth;
	//��ͼ�������
	CComQIPtr<IMap> spMapFrameMap;
	CComQIPtr<IActiveView> spMapFrameActiveView;
	CComQIPtr<IEnvelope> spMapFrameMapEnvelope;
	double dMapFrameMapHeight,dMapFrameMapWidth;
	//�������
	double dScaleMaptToPaper;
	//��ȡͼ��ֽ�泤��
	spMapFrameElement->get_Geometry(&spMapFrameElementGeometry);
	spMapFrameElementGeometry->get_Envelope(&spMapFrameElementEnvelope);
	spMapFrameElementEnvelope->get_Height(&dMapFrameElementHeight);
	spMapFrameElementEnvelope->get_Width(&dMapFrameElementWidth);
	//��ȡͼ���ͼ����
	spMapFrame->get_Map(&spMapFrameMap);
	//Ϊ��ȷ��������ȷ��������Ҫ��map������ؿ�Ҫ�ؼ�ƥ��
// 	CComQIPtr<IObjectClass> spObjClass;
// 	CComQIPtr<IFeatureClass> spFtClass;
// 	CComQIPtr<IGeoDataset> spGeoDataset;
// 	CComQIPtr<ISpatialReference> spSpatial;
// 	spFt->get_Class(&spObjClass);
// 	spFtClass = spObjClass;
// 	spGeoDataset = spFtClass;
// 	spGeoDataset->get_SpatialReference(&spSpatial);
// 	if (spSpatial != NULL)
// 	{
// 		CComBSTR bstrName;
// 		spSpatial->get_Name(&bstrName);
// 		spMapFrameMap->putref_SpatialReference(spSpatial);
// 	}
	//�������ƥ����ȡ��ͼ����
	spMapFrameActiveView = spMapFrameMap;
	spMapFrameActiveView->get_Extent(&spMapFrameMapEnvelope);
	spMapFrameMapEnvelope->get_Height(&dMapFrameMapHeight);
	spMapFrameMapEnvelope->get_Width(&dMapFrameMapWidth);
	//�������
	//����ı�������ϸ΢��࣬����ֻ��Ҫ����λ�þ��У��������������
	//��������ֻ����һ������
	dScaleMaptToPaper = dMapFrameElementHeight / dMapFrameMapHeight;
	
	//�ڶ����ǻ�ȡͼ�����½�ֽ������
	CComQIPtr<IPoint> spMapFrameElementLowerLeftPoint;
	spMapFrameElementEnvelope->get_LowerLeft(&spMapFrameElementLowerLeftPoint);

	//��������ȡ�ؿ����Ͻ������ͼ���ͼ�����½�����
	//���Ȼ�ȡ��Ӿ�������
	CComQIPtr<IGeometry> spFeatureGeometry;
	CComQIPtr<IEnvelope> spFeatureEnvelpoe;
	CComQIPtr<IPoint> spFeatureEnvelpoeUpperRightPoint;
	spFt->get_Shape(&spFeatureGeometry);
	spFeatureGeometry->get_Envelope(&spFeatureEnvelpoe);
	spFeatureEnvelpoe->get_UpperRight(&spFeatureEnvelpoeUpperRightPoint);
	//��ȡ����õ�����ĵ�
	CComQIPtr<IPoint> spFeatureUpperRightPoint;
	CComQIPtr<IProximityOperator> spProximitryOperator = spFeatureGeometry;
	spProximitryOperator->ReturnNearestPoint(spFeatureEnvelpoeUpperRightPoint,esriNoExtension,&spFeatureUpperRightPoint);
	//��ȡͼ�����½ǵ�
	CComQIPtr<IPoint> spMapFrameMapLowerLeftPoint;
	spMapFrameMapEnvelope->get_LowerLeft(&spMapFrameMapLowerLeftPoint);

	//���Ĳ�ȷ���ؿ����Ͻ���ͼ�����½ǵ�ͼ������xy���룬����Ϊֽ�泤��
	//��Ҫ�õ��ı���
	double dMapFrameMapXMin,dMapFrameMapYMin;
	double dFeatureUpperRightX,dFeatureUpperRightY;
	double dAnchorPointMapX,dAnchorPointMapY;
	double dAnchorPointPaperX,dAnchorPointPaperY;
	//��������ͼ�Ͼ���
	spMapFrameMapLowerLeftPoint->get_X(&dMapFrameMapXMin);
	spMapFrameMapLowerLeftPoint->get_Y(&dMapFrameMapYMin);
	spFeatureUpperRightPoint->get_X(&dFeatureUpperRightX);
	spFeatureUpperRightPoint->get_Y(&dFeatureUpperRightY);
	dAnchorPointMapX = dFeatureUpperRightX - dMapFrameMapXMin;
	dAnchorPointMapY = dFeatureUpperRightY - dMapFrameMapYMin;
	//��ȡͼ�����½�ֽ������
	double dMapFrameElementMinX,dMapFrameElementMinY;
	spMapFrameElementLowerLeftPoint->get_X(&dMapFrameElementMinX);
	spMapFrameElementLowerLeftPoint->get_Y(&dMapFrameElementMinY);
	//ת��Ϊֽ�����
	dAnchorPointPaperX = dAnchorPointMapX * dScaleMaptToPaper + dMapFrameElementMinX;
	dAnchorPointPaperY = dAnchorPointMapY * dScaleMaptToPaper + dMapFrameElementMinY;

	//���岽���ݻ�õĳ��ȹ���ê�����꣬���ݹ̶�ֵ��������ê��
	//��������ê��
	if (spBalloonCalloutAnchorPoint != NULL)
	{
		spBalloonCalloutAnchorPoint.Release();
	}
	spBalloonCalloutAnchorPoint.CoCreateInstance(CLSID_Point);
	spBalloonCalloutAnchorPoint->put_X(dAnchorPointPaperX);
	spBalloonCalloutAnchorPoint->put_Y(dAnchorPointPaperY);
	//��������ê��
	if (spTextElementAnchorPoint != NULL)
	{
		spTextElementAnchorPoint.Release();
	}
	spTextElementAnchorPoint.CoCreateInstance(CLSID_Point);
	spTextElementAnchorPoint->put_X(dAnchorPointPaperX + 0.7);
	spTextElementAnchorPoint->put_Y(dAnchorPointPaperY + 1);
}

void CLPZTGHJBTCmd::SetBalloonCalloutSymbol(CComQIPtr<IBalloonCallout> &spBallonCallout)
{
	spBallonCallout->put_LeaderTolerance(10);
	//Բ�Ǿ���
	spBallonCallout->put_Style(esriBCSRoundedRectangle);
	CComQIPtr<IFillSymbol> spFillSymbol;
	spFillSymbol.CoCreateInstance(CLSID_SimpleFillSymbol);
	//��ɫ����
	CComQIPtr<IColor> spColor;
	spColor.CoCreateInstance(CLSID_RgbColor);
	spColor->put_RGB(RGB(255,255,0));
	spFillSymbol->put_Color(spColor);
	//��ɫ�߿�
	CComQIPtr<ILineSymbol> spLineSymbol;
	spLineSymbol.CoCreateInstance(CLSID_SimpleLineSymbol);
	spColor->put_RGB(RGB(255,0,0));
	spLineSymbol->put_Color(spColor);
	spLineSymbol->put_Width(2);
	spFillSymbol->put_Outline(spLineSymbol);
	spBallonCallout->putref_Symbol(spFillSymbol);
}
