// LPZTGHJBTCmd.cpp : CLPZTGHJBTCmd 的实现

#include "stdafx.h"
#include "LPZTGHJBTCmd.h"
#include "LPJBTSetDlg.h"

//实时监控分析进度事件接收器
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

	//事件回调
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
// 	//获取"行政区"信息
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
	//土地规划工作空间（建设用地管制区工作空间）
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
		BCGPMessageBox(_T("规划工作空间未初始化！"),_T("提示"),MB_OK);
		return S_OK;
	}
	CComQIPtr<IPropertySet> spPropertySet;
	spGHWorkspace->get_ConnectionProperties(&spPropertySet);
	VARIANT vtGHINSTANCE = vtMissing;
	spPropertySet->GetProperty(_bstr_t(_T("INSTANCE")),&vtGHINSTANCE);
	CString strGHINSTANCE(vtGHINSTANCE.bstrVal);

	//检查本地数据集是否有选中的面图层
	CComQIPtr<IEnvelope> spEnvelope;//这个用来在后面检查1:10000时是否超出边框
	CArray<CComQIPtr<IFeature>,CComQIPtr<IFeature>&> arryFt;//项目地块数组
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
		nIsContinue = BCGPMessageBox(_T("当前未选中项目地块，是否继续以当前视图中心出图"),_T("未选中地块"),MB_OKCANCEL);
		if (nIsContinue == IDOK)//如果是，选择
		{
			bIsAnalysisVisible = FALSE;			
		}else if (nIsContinue == IDCANCEL)
		{
			return S_OK;
		}
	}else
	{
		//记录第一个获得的要素集，校验是否选中多个选集
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
				//过滤掉规划库中的层
				esriWorkspaceType enumWorkspaceType;
				spTempWorkspace->get_Type(&enumWorkspaceType);
				if (enumWorkspaceType != esriRemoteDatabaseWorkspace)
				{//如果是本地数据
					esriGeometryType enumGeometryType;
					spFtClass->get_ShapeType(&enumGeometryType);
					//过滤掉非面层
					if (enumGeometryType == esriGeometryPolygon)
					{
						if (spFstObjClass == NULL)
						{
							spFstObjClass = spObjClass;
						}else if (spObjClass != spFstObjClass)
						{
							BCGPMessageBox(_T("当前选中多个规划库外的图层，\n局部图出图不支持多项目图层！"),_T("项目图层过多！"),MB_OK);
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
				{//如果不是本地数据
					CComQIPtr<IPropertySet> spTmpPropertySet;
					spTempWorkspace->get_ConnectionProperties(&spTmpPropertySet);
					VARIANT vtTmpINSTANCE = vtMissing;
					spTmpPropertySet->GetProperty(_bstr_t(_T("INSTANCE")),&vtTmpINSTANCE);
					CString strTmpINSTANCE(vtTmpINSTANCE.bstrVal);
					if (strGHINSTANCE.Compare(strTmpINSTANCE) != 0)
					{
						esriGeometryType enumGeometryType;
						spFtClass->get_ShapeType(&enumGeometryType);
						//过滤掉非面层
						if (enumGeometryType == esriGeometryPolygon)
						{
							if (spFstObjClass == NULL)
							{
								spFstObjClass = spObjClass;
							}else if (spObjClass != spFstObjClass)
							{
								BCGPMessageBox(_T("当前选中多个规划库外的图层，\n局部图出图不支持多项目图层！"),_T("项目图层过多！"),MB_OK);
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
			nIsContinue = BCGPMessageBox(_T("当前未选中规划库外的项目地块，是否继续以当前视图中心出图"),_T("未选中项目地块"),MB_OK);
			if (nIsContinue == IDOK)
			{
				bIsAnalysisVisible = FALSE;
			}else if (nIsContinue == IDCANCEL)
			{
				return S_OK;
			}
		}
	}

	

	//解析导出模板的配置文件内容
	TCHAR szAppFullPath[MAX_PATH];
	DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(CHAR);
	GetModuleFileName(NULL,szAppFullPath,dBufferSize);
	CString strAppPath(szAppFullPath);
	strAppPath = strAppPath.Mid(0,strAppPath.ReverseFind('\\'));
	CString strConfigFile = strAppPath + CONFIGPATH;
	CString strSection(_T("ZTGHJBT"));

	//获取所在行政区
	CString strMapTitle =_T("土地利用总体规划局部图（2016-2020年）");//局部图标题修改

	//控件查询需要用到的图形
	CComQIPtr<IFeature> spFtTmp;
	CComQIPtr<IGeometry> spGeoTmp;
	if (arryFt.GetCount() == 0)//如果没有选中图形,获取当前视图中心
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

	//获取乡镇行政区图层
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
			//空间查询
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
					//这里获取下一个乡镇，如果有，说明跨乡镇了，不添加乡镇名称
					spXZFt.Release();
					spFtCurSor->NextFeature(&spXZFt);
					if (spXZFt == NULL)//没有下一个乡镇
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
	//同样的操作对县区级图层再做一遍
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
			//空间查询
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
					if (spXZFt != NULL)//跨县区直接终止
					{
						BCGPMessageBox(_T("选择的地块范围属于多个县区！"),_T("错误！"),MB_OK);
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

	//弹出设置对话框，出图范围校验在设置完成点击确定之后进行
	CLPJBTSetDlg dlg;
	dlg.InitViewConfig(strConfigFile,strSection);
	dlg.SetIsAnalysisVisible(bIsAnalysisVisible);
	dlg.m_strMapConfig.SetAt(_T("MAPTITLE"),strMapTitle);
	if (dlg.DoModal() == IDOK)
	{
		//切换视图
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
		//获取设置，打开模板
		CString strMDXName;
		if (dlg.m_bIsAnalysis == TRUE)
		{
			
			dlg.m_strMapConfig.Lookup(_T("MODELPATH_ANALYSIS"),strMDXName);
		}else
		{
			dlg.m_strMapConfig.Lookup(_T("MODELPATH_NO_ANALYSIS"),strMDXName);
		}
		if (strMDXName.GetLength() <= 0) return E_FAIL;
		//获取MXD文档所在目录
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

		//获取图框区域,判断地块范围是否超出显示区域
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
		//获取当前map的坐标，赋值给布局map
		spMap->get_SpatialReference(&spSpatial);
		CComBSTR bstrSpatialName;
		spSpatial->get_Name(&bstrSpatialName);

		//保存一个mapframe给后面添加气泡用
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
				if (bIsAnalysisVisible == TRUE)//这表明是有选中地块的，如果没有选中地块则不需要验证范围
				{
					spEnvelope->get_Height(&dbDKHeight);
					spEnvelope->get_Width(&dbDKWidth);
					spMapRect->get_Height(&dbMapHeight);
					spMapRect->get_Width(&dbMapWidth);
					if (dbMapWidth < dbDKWidth || dbMapHeight < dbDKHeight)
					{		
						bIsOutofView = TRUE;
						nIsLayout = BCGPMessageBox(_T("地块范围超出图框范围，是否继续出图"),_T("范围超限！"),MB_OKCANCEL);
						break;
					}
				}				
			}
			ipElement.Release();
			ipGraphicsContainer->Next(&ipElement);
		}
		if (nIsLayout == IDCANCEL)//如果选择退出，则切换回地图视图
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
			//移动到地块中点为中心
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
			//以当前视图中心为中心
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
		//遍历选集，拼接过滤条件，只显示选中地块
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
		//添加地块所在图层
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
			//创建渲染格式
			// 设置symbol
			IColorPtr ipRgbColor(CLSID_RgbColor);
			ISimpleFillSymbolPtr ipStylePg(CLSID_SimpleFillSymbol);
			ISimpleLineSymbolPtr ipStyLn(CLSID_SimpleLineSymbol);
			//边界样式
			ipRgbColor->put_RGB(RGB(0,76,155));
			ipStyLn->put_Style(esriSLSSolid);
			ipStyLn->put_Color(ipRgbColor);
			ipStyLn->put_Width(2);
			//多边形样式
			ipStylePg->put_Color(ipRgbColor);
			ipStylePg->put_Style(esriSFSNull);
			//ipStylePg->put_Style(esriSFSBackwardDiagonal);
			ipStylePg->put_Outline(ipStyLn);
			//添加图层渲染格式
			CComQIPtr<ISimpleRenderer> spSimpleRenderer;
			spSimpleRenderer.CoCreateInstance(CLSID_SimpleRenderer);
			CComQIPtr<ISymbol> spSymbol = ipStylePg;
			spSimpleRenderer->putref_Symbol(spSymbol);
			CComQIPtr<IFeatureRenderer> spFtRenderer = spSimpleRenderer;
			CComQIPtr<IGeoFeatureLayer> spGeoFtLayer = spDKLayer;
			spGeoFtLayer->putref_Renderer(spFtRenderer);
			//添加图层名
			//CString strLayerName;
			//strLayerName.Format(_T("项目地块层%d"),(nFt+1));
			//spDKLayer->put_Name(_bstr_t(strLayerName));
			spDKLayer->put_Name(_T("项目地块层"));

			CComQIPtr<IFeatureLayerDefinition> spFtLayerDefinition = spDKLayer;
			CComBSTR bstrDefinition;
			spFtLayerDefinition->get_DefinitionExpression(&bstrDefinition);
			bstrDefinition = strSQLWhere;
			spFtLayerDefinition->put_DefinitionExpression(bstrDefinition);

			ipLayoutMap->AddLayer(spDKLayer);

			//这里需要添加一步设置label
			//获取标注配置接口
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
// 			//配置字体信息
// 			CComQIPtr<IFontDisp> spFontDisp;
// 			spTextSymbol->get_Font(&spFontDisp);
// 			//设置颜色
// 			CComQIPtr<IColor> spColor;
// 			spColor.CoCreateInstance(CLSID_RgbColor);
// 			spColor->put_RGB(RGB(255,0,0));
// 			spTextSymbol->put_Color(spColor);
// 			//设置字体
// 			CComQIPtr<IFont> spFont = spFontDisp;
// //			spFont->put_Name(_T("楷体"));
// 			//字体大小
// 			CY cySize;
// 			spFont->get_Size(&cySize);
// 			cySize.int64 = 20*10000;
// 			spFont->put_Size(cySize);
// 			//其他样式
// 			spFont->put_Bold(TRUE);//加粗
// // 			spFont->put_Italic(FALSE);//斜体
// // 			spFont->put_Underline(FALSE);//下划线
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
		//再遍历一遍，添加callout
		for (int i = 0; i < arryFt.GetCount(); i++)
		{
			AddCallout(ipPageLayoutControl,spMapFrame,arryFt.GetAt(i));
		}

		//ipPageLayoutControl->Refresh(esriViewAll);
		

		//调用选址分析接口进行数据分析
		if (dlg.m_bIsAnalysis == TRUE)
		{
			//创建分析结果临时GDB和dataset
			CreateGDBResult();
			//创建存放待分析目标的要素集
			CreateXMDKFeatureClass();

			//将要分析的地块写入要素集中
			//XMDK中DKMC字段索引
			long nIndexDKMC = -1;
			m_spXMDKFeatureClass->FindField(_bstr_t(_T("DKMC")),&nIndexDKMC);
			if (-1 == nIndexDKMC)return E_FAIL;
			//XMDK中DKID字段索引
			long nIndexDKID = -1;
			m_spXMDKFeatureClass->FindField(_bstr_t(_T("DKID")),&nIndexDKID);
			if(-1 == nIndexDKID)return E_FAIL;

			for (int i = 0; i < arryFt.GetCount(); i++)
			{
				CComQIPtr<IFeature> spFeature = arryFt.GetAt(i);
				ASSERT(spFeature != NULL);
				//坐标系变换
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
				//设置插入要素几何类型
				if(spGeometry != NULL)spGeometry.Release();
				spFeature->get_ShapeCopy(&spGeometry);
				spFeatureBuffer->putref_Shape(spGeometry);
				//设置插入要素的DKMC属性字段值
				spFeatureBuffer->put_Value(nIndexDKMC,_variant_t(i+1));
				//设置插入要素DKID字段值(这里按照插入顺序给个ID即可，无需使用GUID等复杂的唯一标识符)
				spFeatureBuffer->put_Value(nIndexDKID,_variant_t(i+1));
				//插入要素
				CComQIPtr<IFeatureCursor> spFeatureCursor;
				m_spXMDKFeatureClass->Insert(VARIANT_TRUE,&spFeatureCursor);
				_variant_t vtOID;
				spFeatureCursor->InsertFeature(spFeatureBuffer,&vtOID);
			}
			//进度条
			CComQIPtr<GisqLib::IGSProgressBar> spGSProgressBar = spGisqFramework->GetProgressBar();
			spGSProgressBar->SetRange(0,100);
			spGSProgressBar->Show();
			//调用统计分析模块
			CComQIPtr<IGisqLandPlanAnslysis> spGisLandPlanAnalysis;
			spGisLandPlanAnalysis.CoCreateInstance(CLSID_GisqLandPlanAnslysis);
			spGisLandPlanAnalysis->InitWorkspace(spGHWorkspace,m_spGDBWorkspace);
			spGisLandPlanAnalysis->SetCurFeatureDataset(m_spDSCurFeatureDataset);
			spGisLandPlanAnalysis->SetCurSuffix(_bstr_t("_120"));
			spGisLandPlanAnalysis->SetSTBMJMax(10);
			//实时监控分析进度事件绑定测试
			CComObject<CSink>* pSink = NULL;  
			CComObject<CSink>::CreateInstance(&pSink);
			ASSERT(pSink != NULL);
			pSink->SetGisqProgressBar(spGSProgressBar);
			DWORD cookies = 0;  
			AtlAdvise(spGisLandPlanAnalysis,pSink,__uuidof(_IGisqLandPlanAnslysisEvents), &cookies); 
			//执行分析
			spGisLandPlanAnalysis->ExcuteAnalysisPro();
			//结束之后隐藏进度条
			spGSProgressBar->Hide();
		
// 			//选址分析结果汇总
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
// 		//获取字段索引
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
// 		//遍历结果表
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
// 				//农业用地
// 				//基本农田（G11）
// 				if (vtGHYTDM == CComVariant(_T("G111")))
// 				{//示范区基本农田
// 					nNYD += dGHYTMJ;
// 					nNYD_GD += dGHYTMJ;
// 					//将基本农田扣除的线物和零物面积要加到G15中
// 					if(vtDLDM == CComVariant(_T("111")) || vtDLDM == CComVariant(_T("113")))
// 					{
// 						nNYD += dXZDWMJ;
// 						nNYD += dLXDWMJ;
// 					} 				
// 				}
// 				if (vtGHYTDM == CComVariant(_T("G112")))
// 				{//一般基本农田
// 					nNYD += dGHYTMJ;
// 					nNYD_GD += dGHYTMJ;
// 					//将基本农田扣除的线物和零物面积要加到G15中
// 					if(vtDLDM == CComVariant(_T("111")) || vtDLDM == CComVariant(_T("113")))
// 					{
// 						nNYD += dXZDWMJ;
// 						nNYD += dLXDWMJ;
// 					} 				
// 				}
// 				//一般农田（N11）
// 				if (vtGHYTDM == CComVariant(_T("N111")))
// 				{//一般农田
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
// 				{//新增一般农田
// 					nNYD += dGHYTMJ;
// 					nNYD_GD += dGHYTMJ;
// 				}
// 				CString strTemp = VariantToString(vtGHYTDM);
// 				if(strTemp.GetLength() > 2)
// 				{
// 					//其他农用地（1xx）
// 					if (strTemp[1] == _T('1') && strTemp[2] != _T('1'))
// 					{
// 						nNYD += dGHYTMJ;
// 					}
// 					//建设用地（2xx）
// 					if (strTemp[1] == _T('2'))
// 					{
// 						nJSYD += dGHYTMJ;
// 					}
// 					//未利用地（3xx）
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
		//选址分析结果汇总
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
		//拼接查询字段
		CComQIPtr<IQueryFilter> spQueryFilter;
		spQueryFilter.CoCreateInstance(CLSID_QueryFilter);
		spQueryFilter->put_WhereClause(_T("DKMC = '' and DKID = ''"));
		CComQIPtr<esriICursor> spCursor;
		spYTResTable->Search(spQueryFilter,VARIANT_FALSE,&spCursor);
		CComQIPtr<esriIRow> spRow;
		spCursor->NextRow(&spRow);
		long nIndex;
		CComVariant vtTmp;
		spYTResTable->FindField(_T("ZJ"),&nIndex);//总计字段
		spRow->get_Value(nIndex,&vtTmp);
		strAll = VariantToString(vtTmp);
		spYTResTable->FindField(_T("NTotal"),&nIndex);//农用地字段
		spRow->get_Value(nIndex,&vtTmp);
		strNYD = VariantToString(vtTmp);
		spYTResTable->FindField(_T("QZGD"),&nIndex);//其中耕地字段
		spRow->get_Value(nIndex,&vtTmp);
		strNYD_GD = VariantToString(vtTmp);
		spYTResTable->FindField(_T("JTotal"),&nIndex);//建设用地字段
		spRow->get_Value(nIndex,&vtTmp);
		strJSYD = VariantToString(vtTmp);
		spYTResTable->FindField(_T("WTotal"),&nIndex);//未利用地字段
		spRow->get_Value(nIndex,&vtTmp);
		strWLYD = VariantToString(vtTmp);

 		//除10000
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
		//是否符合规划
		CString strSFFHGH;
		if (dbNYD == 0.0 && dbWLYD == 0.0)
		{
			strSFFHGH = _T("是");
		}else
		{
			strSFFHGH = _T("否");
		}
		//读取一个地块的地块名称和规划用途名称
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
		//写入到配置map中
		//dlg.m_strMapConfig.SetAt(_T("GHYTMC"),strGHLYMC);
		dlg.m_strMapConfig.SetAt(_T("编号"),strDKID);
		dlg.m_strMapConfig.SetAt(_T("合计"),strAll);
		dlg.m_strMapConfig.SetAt(_T("农用地"),strNYD);
		dlg.m_strMapConfig.SetAt(_T("其中：耕地"),strNYD_GD);
		dlg.m_strMapConfig.SetAt(_T("建设用地"),strJSYD);
		dlg.m_strMapConfig.SetAt(_T("未利用地"),strWLYD);
		dlg.m_strMapConfig.SetAt(_T("是否符合规划"),strSFFHGH);
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
	//使用当前系统时间做临时结果文件数据库所在文件夹名称
	SYSTEMTIME st;
	CString strDate,strTime;
	GetLocalTime(&st);
	CString strCurTempFolder;
	//文件夹名称：年月日时分秒
	strCurTempFolder.Format(_T("%4d%02d%02d%02d%02d%02d"),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	strTempResultFolder += _T("\\") +strCurTempFolder;
	//如果临时文件夹存在则重新创建，
	if(!PathIsDirectory((LPCTSTR)strTempResultFolder))
	{
		if (!CreateDirectory((LPCTSTR)strTempResultFolder,NULL))
		{
			::BCGPMessageBox(_T("创建统计分析临时文件夹失败!"),_T("错误"),MB_OK| MB_ICONERROR);
			return E_FAIL;
		}
	}
	else
	{
		if (RemoveDirectory((LPCTSTR)strTempResultFolder))
		{
			if (!CreateDirectory((LPCTSTR)strTempResultFolder,NULL))
			{
				::BCGPMessageBox(_T("创建统计分析临时文件夹失败!"),_T("错误"),MB_OK| MB_ICONERROR);
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
			//创建esriSRProjCS_Xian1980_3_Degree_GK_Zone_40坐标系
			CComQIPtr<ISpatialReferenceFactory> spSpatialReferenceFactory;
			spSpatialReferenceFactory.CoCreateInstance(CLSID_SpatialReferenceEnvironment);
			CComQIPtr<IProjectedCoordinateSystem> spProjectedCoordinateSystem;
			spSpatialReferenceFactory->CreateProjectedCoordinateSystem(esriSRProjCS_Xian1980_3_Degree_GK_Zone_40,&spProjectedCoordinateSystem);
			CComQIPtr<ISpatialReference> spSpatialReference = spProjectedCoordinateSystem;
			if(spSpatialReference == NULL)return E_FAIL;
			//设置坐标系的域，这里必须设置，否则创建FeatureDataset会失败
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
	//创建字段集
	CComQIPtr<IFeatureClassDescription> spFeatureClassDescription;
	spFeatureClassDescription.CoCreateInstance(CLSID_FeatureClassDescription);
	CComQIPtr<IObjectClassDescription> spObjectClassDescription = spFeatureClassDescription;
	
	//必要字段
	CComQIPtr<IFields> spFields;
	spObjectClassDescription->get_RequiredFields(&spFields);
	
	//要素类的几何类型
	CComBSTR bstrShapeFieldName;
	spFeatureClassDescription->get_ShapeFieldName(&bstrShapeFieldName);
	long nShapeFieldName;
	spFields->FindField(bstrShapeFieldName,&nShapeFieldName);
	CComQIPtr<IField> spField;
	spFields->get_Field(nShapeFieldName,&spField);
	CComQIPtr<IGeometryDef> spGeometryDef;
	spField->get_GeometryDef(&spGeometryDef);
	//几何类型(esriGeometryPolygon)
	CComQIPtr<IGeometryDefEdit> spGeometryDefEdit = spGeometryDef;
	spGeometryDefEdit->put_GeometryType(esriGeometryPolygon);

	//添加DKID字段
	CComQIPtr<IFieldsEdit> spFieldsEdit = spFields;
	if (spField != NULL)spField.Release();
	CComQIPtr<IFieldEdit> spFieldEdit;
	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_bstr_t(_T("DKID")));
	spFieldEdit->put_Type(esriFieldTypeInteger);
	spFieldsEdit->AddField(spField);
	//添加DKMC字段
	if(spField != NULL)spField.Release();
	if(spFieldEdit != NULL)spFieldEdit.Release();
	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_bstr_t(_T("DKMC")));
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldsEdit->AddField(spField);

	//传入字段
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
	//创建相关接口
	CComQIPtr<IPageLayout> spPageLayout;
	spPageLayoutControl->get_PageLayout(&spPageLayout);
	CComQIPtr<IGraphicsContainer> spGraphicsContainer = spPageLayout;
	//创建元素
	CComQIPtr<ITextElement> spTextElement;
	spTextElement.CoCreateInstance(CLSID_TextElement);
	CComQIPtr<IBalloonCallout> spBalloonCallout;
	spBalloonCallout.CoCreateInstance(CLSID_BalloonCallout);
	//设置字体
	SetFontType(spTextElement);
	//设置气泡风格
	SetBalloonCalloutSymbol(spBalloonCallout);
	//设置内容
	CComBSTR bstrName;
	GetFeatureName(spFt,bstrName);
	spTextElement->put_Text(bstrName);
	//设置位置
	CComQIPtr<IPoint> spBalloonCalloutAnchorPoint;
	CComQIPtr<IPoint> spTextElementAnchorPoint;
	GetElementAnchorPoint(spPageLayoutControl,spMapFrame,spFt,spBalloonCalloutAnchorPoint,spTextElementAnchorPoint);
	CComQIPtr<IElement> spText = spTextElement;
	spText->put_Geometry(spTextElementAnchorPoint);
	spBalloonCallout->put_AnchorPoint(spBalloonCalloutAnchorPoint);
	//设置背景为气泡
	CComQIPtr<ITextSymbol> spTextSymbol;
	spTextElement->get_Symbol(&spTextSymbol);
	CComQIPtr<IFormattedTextSymbol> spFormarttedTextSymblol = spTextSymbol;
	spFormarttedTextSymblol->putref_Background((CComQIPtr<ITextBackground>)spBalloonCallout);
	spTextElement->put_Symbol(spTextSymbol);
	//保存元素
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
			strOIDName += _T("地块") + strOID;
			bstrName = strOIDName;
			return;
		}
	}

}

void CLPZTGHJBTCmd::SetFontType(CComQIPtr<ITextElement> &spTextElement)
{
	CComQIPtr<ITextSymbol> spTextSymbol;
	spTextElement->get_Symbol(&spTextSymbol);
	//配置字体信息
	CComQIPtr<IFontDisp> spFontDisp;
	spTextSymbol->get_Font(&spFontDisp);
	//设置颜色
	CComQIPtr<IColor> spColor;
	spColor.CoCreateInstance(CLSID_RgbColor);
	spColor->put_RGB(RGB(255,0,0));
	spTextSymbol->put_Color(spColor);
	//设置字体
	CComQIPtr<IFont> spFont = spFontDisp;
	//spFont->put_Name(_T("楷体"));
	//字体大小
	//CY cySize;
	//spFont->get_Size(&cySize);
	//cySize.int64 = 20*10000;
	//spFont->put_Size(cySize);
	//其他样式
	spFont->put_Bold(TRUE);//加粗
	spTextSymbol->put_Font(spFontDisp);
	spTextElement->put_Symbol(spTextSymbol);
}

void CLPZTGHJBTCmd::GetElementAnchorPoint(CComQIPtr<IPageLayoutControl> spPageLayoutControl, CComQIPtr<IMapFrame> spMapFrame, CComQIPtr<IFeature> spFt,
						   CComQIPtr<IPoint> &spBalloonCalloutAnchorPoint,
						   CComQIPtr<IPoint> &spTextElementAnchorPoint)
{

	//第一步是获取图框坐标相对地图坐标的换算	
	//定义获取比例需要的变量
	//图框元素相关
	CComQIPtr<IElement> spMapFrameElement = spMapFrame;
	CComQIPtr<IGeometry> spMapFrameElementGeometry;
	CComQIPtr<IEnvelope> spMapFrameElementEnvelope;
	double dMapFrameElementHeight,dMapFrameElementWidth;
	//地图坐标相关
	CComQIPtr<IMap> spMapFrameMap;
	CComQIPtr<IActiveView> spMapFrameActiveView;
	CComQIPtr<IEnvelope> spMapFrameMapEnvelope;
	double dMapFrameMapHeight,dMapFrameMapWidth;
	//换算比例
	double dScaleMaptToPaper;
	//获取图框纸面长宽
	spMapFrameElement->get_Geometry(&spMapFrameElementGeometry);
	spMapFrameElementGeometry->get_Envelope(&spMapFrameElementEnvelope);
	spMapFrameElementEnvelope->get_Height(&dMapFrameElementHeight);
	spMapFrameElementEnvelope->get_Width(&dMapFrameElementWidth);
	//获取图框地图长宽
	spMapFrame->get_Map(&spMapFrameMap);
	//为了确保换算正确，这里需要将map坐标与地块要素集匹配
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
	//完成坐标匹配后获取地图长宽
	spMapFrameActiveView = spMapFrameMap;
	spMapFrameActiveView->get_Extent(&spMapFrameMapEnvelope);
	spMapFrameMapEnvelope->get_Height(&dMapFrameMapHeight);
	spMapFrameMapEnvelope->get_Width(&dMapFrameMapWidth);
	//计算比例
	//长宽的比例会有细微差距，这里只需要大致位置就行，不区分这个问题
	//所以这里只计算一个比例
	dScaleMaptToPaper = dMapFrameElementHeight / dMapFrameMapHeight;
	
	//第二步是获取图框左下角纸面坐标
	CComQIPtr<IPoint> spMapFrameElementLowerLeftPoint;
	spMapFrameElementEnvelope->get_LowerLeft(&spMapFrameElementLowerLeftPoint);

	//第三步获取地块右上角坐标和图框地图上左下角坐标
	//首先获取外接矩形坐标
	CComQIPtr<IGeometry> spFeatureGeometry;
	CComQIPtr<IEnvelope> spFeatureEnvelpoe;
	CComQIPtr<IPoint> spFeatureEnvelpoeUpperRightPoint;
	spFt->get_Shape(&spFeatureGeometry);
	spFeatureGeometry->get_Envelope(&spFeatureEnvelpoe);
	spFeatureEnvelpoe->get_UpperRight(&spFeatureEnvelpoeUpperRightPoint);
	//获取距离该点最近的点
	CComQIPtr<IPoint> spFeatureUpperRightPoint;
	CComQIPtr<IProximityOperator> spProximitryOperator = spFeatureGeometry;
	spProximitryOperator->ReturnNearestPoint(spFeatureEnvelpoeUpperRightPoint,esriNoExtension,&spFeatureUpperRightPoint);
	//获取图框左下角点
	CComQIPtr<IPoint> spMapFrameMapLowerLeftPoint;
	spMapFrameMapEnvelope->get_LowerLeft(&spMapFrameMapLowerLeftPoint);

	//第四步确定地块右上角与图框左下角的图面坐标xy距离，换算为纸面长度
	//需要用到的变量
	double dMapFrameMapXMin,dMapFrameMapYMin;
	double dFeatureUpperRightX,dFeatureUpperRightY;
	double dAnchorPointMapX,dAnchorPointMapY;
	double dAnchorPointPaperX,dAnchorPointPaperY;
	//计算两点图上距离
	spMapFrameMapLowerLeftPoint->get_X(&dMapFrameMapXMin);
	spMapFrameMapLowerLeftPoint->get_Y(&dMapFrameMapYMin);
	spFeatureUpperRightPoint->get_X(&dFeatureUpperRightX);
	spFeatureUpperRightPoint->get_Y(&dFeatureUpperRightY);
	dAnchorPointMapX = dFeatureUpperRightX - dMapFrameMapXMin;
	dAnchorPointMapY = dFeatureUpperRightY - dMapFrameMapYMin;
	//获取图框左下角纸面坐标
	double dMapFrameElementMinX,dMapFrameElementMinY;
	spMapFrameElementLowerLeftPoint->get_X(&dMapFrameElementMinX);
	spMapFrameElementLowerLeftPoint->get_Y(&dMapFrameElementMinY);
	//转换为纸面距离
	dAnchorPointPaperX = dAnchorPointMapX * dScaleMaptToPaper + dMapFrameElementMinX;
	dAnchorPointPaperY = dAnchorPointMapY * dScaleMaptToPaper + dMapFrameElementMinY;

	//第五步根据获得的长度构造锚点坐标，根据固定值换算文字锚点
	//构造气泡锚点
	if (spBalloonCalloutAnchorPoint != NULL)
	{
		spBalloonCalloutAnchorPoint.Release();
	}
	spBalloonCalloutAnchorPoint.CoCreateInstance(CLSID_Point);
	spBalloonCalloutAnchorPoint->put_X(dAnchorPointPaperX);
	spBalloonCalloutAnchorPoint->put_Y(dAnchorPointPaperY);
	//构造文字锚点
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
	//圆角矩形
	spBallonCallout->put_Style(esriBCSRoundedRectangle);
	CComQIPtr<IFillSymbol> spFillSymbol;
	spFillSymbol.CoCreateInstance(CLSID_SimpleFillSymbol);
	//黄色背景
	CComQIPtr<IColor> spColor;
	spColor.CoCreateInstance(CLSID_RgbColor);
	spColor->put_RGB(RGB(255,255,0));
	spFillSymbol->put_Color(spColor);
	//红色边框
	CComQIPtr<ILineSymbol> spLineSymbol;
	spLineSymbol.CoCreateInstance(CLSID_SimpleLineSymbol);
	spColor->put_RGB(RGB(255,0,0));
	spLineSymbol->put_Color(spColor);
	spLineSymbol->put_Width(2);
	spFillSymbol->put_Outline(spLineSymbol);
	spBallonCallout->putref_Symbol(spFillSymbol);
}
