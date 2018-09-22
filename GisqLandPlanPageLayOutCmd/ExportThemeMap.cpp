#include "StdAfx.h"
#include "ExportThemeMap.h"

CExportThemeMap::CExportThemeMap(CComQIPtr<GisqLib::_DGisqFrameworkCOM> ipGisqFramework)
{
	//m_pszArrDatasetName = NULL;
	ASSERT(ipGisqFramework != NULL);
	m_ipGisqFramework = ipGisqFramework;
}

CExportThemeMap::~CExportThemeMap()
{
}

//void CExportThemeMap::SetDatasetName(CStringArray* pszArrDatasetName)
//{
//	ASSERT(pszArrDatasetName != NULL && !pszArrDatasetName->IsEmpty());
//	m_pszArrDatasetName = pszArrDatasetName;
//}

void CExportThemeMap::ExportThemeMap(
			CComQIPtr<IWorkspace> ipXZWorkspace,
			CComQIPtr<IWorkspace> ipGHWorkspace,
			CString strXZQDM,
			CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig,
			CStringArray& strArrDatasetName)
{
	double doScale = 1;
	BOOL bPortrait = FALSE;
	int iSideEdge = 0;
	int iTopEdge = 0;
	if(strXZQDM.IsEmpty() || strMapConfig.IsEmpty() || strArrDatasetName.IsEmpty())return;
	//���ò�����ͼΪ��ǰ���ͼ
	CComQIPtr<GisqLib::IGSActiveView> ipGSActiveView;
	for (int i = 0;i < m_ipGisqFramework->GetViewCount();i++)
	{
		ipGSActiveView =m_ipGisqFramework->GetViewItem(i);
		if (ipGSActiveView->GetViewType() == (short)VT_PageLayoutView)
		{
			GisqLib::IGSActiveView* ipTemp = ipGSActiveView;
			m_ipGisqFramework->SetActiveView(ipTemp);
			break;
		}
	}
	ASSERT(ipGSActiveView != NULL);
	//��ȡ������ͼ�ؼ�
	CComQIPtr<IPageLayoutControl> ipPageLayoutControl = ipGSActiveView->GetControl();
	CString strValue = _T("");
	//���ò�����ͼ����(���ڡ�����...)
	if(strMapConfig.Lookup(_T("MODELPATH"),strValue))
	{
		//CComBSTR strDocumentPath = strValue.AllocSysString();
		_bstr_t strDocumentPath = strValue;
		VARIANT_BOOL bSecceed = m_ipGisqFramework->OpenMapDocument(strDocumentPath);
		if (bSecceed == VARIANT_FALSE)return;
		strValue = _T("");
	}

	if (strMapConfig.Lookup(_T("MAKEWHERE"),strValue))
	{
		CComPtr<IElement> ipElement = NULL;
		CComBSTR strTagName(_T("MAPPUBLIC"));
		//ipPageLayoutControl->FindElementByName(strTemp,1,&ipElement);
		ipPageLayoutControl->FindElementByName(strTagName,1,&ipElement);
		if (ipElement != NULL)
		{
			strValue += _T("	");
			CComBSTR strText;
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			ipTextElement->get_Text(&strText);
			strText = strValue + strText;
			ipTextElement->put_Text(strText);
		}
		strValue = _T("");
	}
	if(strMapConfig.Lookup(_T("MAKEDATE"),strValue))
	{
		CComPtr<IElement> ipElement = NULL;
		CComBSTR strTagName(_T("MAPRQ"));
		ipPageLayoutControl->FindElementByName(strTagName,1,&ipElement);
		if (ipElement != NULL)
		{	
			//����ȡ�õ�ʱ���ַ����ĸ�ʽ�ֹ�������ʱ����Ϣ,���Ҫ�������ַ����ĸ�ʽҪȷ��������д��ȷ�������Ȳ�����
			CComBSTR strText;
			CComBSTR strTemp(_T("���ڣ�"));
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			ipTextElement->get_Text(&strText);
			strTemp += strText;
			ipTextElement->put_Text(strTemp);
		}
		strValue = _T("");
	}
	if (strMapConfig.Lookup(_T("MAPZF"),strValue))
	{
		CComPtr<IElement> ipElement = NULL;
		BSTR strTagName(_T("ZHEN"));
		ipPageLayoutControl->FindElementByName(strTagName,1,&ipElement);
		SysFreeString(strTagName);
		if (ipElement != NULL)
		{
			CComBSTR strText;
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			ipTextElement->get_Text(&strText);
			strText = strValue + strText;
			ipTextElement->put_Text(strText);
		}
		strValue = _T("");
	}
	if (strMapConfig.Lookup(_T("MAPPUBLIC"),strValue))
	{
		CComPtr<IElement> ipElement = NULL;
		BSTR strTagName(_T("XIAN"));
		ipPageLayoutControl->FindElementByName(strTagName,1,&ipElement);
		SysFreeString(strTagName);
		if (ipElement != NULL)
		{
			CComBSTR strText;
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			ipTextElement->get_Text(&strText);
			strText = strValue + strText;
			ipTextElement->put_Text(strText);
		}
		strValue = _T("");
	}
	if (strMapConfig.Lookup(_T("MAPNAME"),strValue))
	{
		CComPtr<IElement> ipElement = NULL;
		CComBSTR strTagName(_T("MAPTITLE"));
		ipPageLayoutControl->FindElementByName(strTagName,1,&ipElement);
		if (ipElement != NULL)
		{
			CComBSTR strText;
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			strText = strValue;
			ipTextElement->put_Text(strText);
		}
		strValue = _T("");
	}
	//��ȡ������ͼ��������Ϣ
	if (strMapConfig.Lookup(_T("SCALE"),strValue))
	{
		doScale = _wtof(strValue);
		strValue = _T("");
	}
	if (strMapConfig.Lookup(_T("DIRECTION"),strValue))
	{
		//bPortrait = (BOOL)_wtoi(strValue);
		bPortrait = strValue == _T("True") ? TRUE:FALSE;
		CComPtr<IPage> ipPage;
		ipPageLayoutControl->get_Page(&ipPage);
		if (bPortrait)
		{
			ipPage->put_Orientation((short)1);
		}
		else
		{
			ipPage->put_Orientation((short)2);
		}
		strValue = _T("");
	}
	if (strMapConfig.Lookup(_T("SIDEEDGE"),strValue))
	{
		iSideEdge = _wtoi(strValue);
		strValue = _T("");
	}
	if (strMapConfig.Lookup(_T("TOPEDGE"),strValue))
	{
		iTopEdge = _wtoi(strValue);
		strValue = _T("");
	}
	
	CComQIPtr<IFeatureWorkspace> ipGHFeatureWorkspace = ipGHWorkspace;
	CComQIPtr<IGraphicsContainer> ipGraphicsContainer;
	ipPageLayoutControl->get_GraphicsContainer(&ipGraphicsContainer);
	ipGraphicsContainer->Reset();
	CComQIPtr<IElement> ipElement;
	ipGraphicsContainer->Next(&ipElement);
	//����Element
	while(ipElement != NULL)
	{
		CComQIPtr<IEnvelope> ipEnvelope;
		CComQIPtr<IMapFrame> ipMapFrame = ipElement;
		if (ipMapFrame != NULL)
		{
			CComQIPtr<IMap> ipMap;
			ipMapFrame->get_Map(&ipMap);
			CComQIPtr<IEnumLayer> ipEnumLayer;

			CComQIPtr<IUID> uid;
			uid.CoCreateInstance(CLSID_UID);
			uid->put_Value(_variant_t(_T("{40A9E885-5533-11d0-98BE-00805F7CED21}")));
			ipMap->get_Layers(uid,TRUE,&ipEnumLayer);
			ipEnumLayer->Reset();
			CComQIPtr<ILayer> ipLayer;
			ipEnumLayer->Next(&ipLayer);
			//����ͼ��
			while (ipLayer != NULL)
			{
				CComQIPtr<IFeatureClass> ipFeatureClass;
				CComQIPtr<IFeatureLayer> ipFeatureLayer = ipLayer;
				CComQIPtr<IDataLayer> ipDataLayer = ipFeatureLayer;
				CComQIPtr<IName> ipName;
				ipDataLayer->get_DataSourceName(&ipName);
				CComQIPtr<IDatasetName> ipDatasetName = ipName;
				CComBSTR strName;
				ipDatasetName->get_Name(&strName);
				CString strTemp(strName);
				strTemp.Replace(_T("SDE"),_T(""));
				int nCount = strArrDatasetName.GetCount();
				for(int i = 0;i < nCount;i++)
				{
					if (strTemp == strArrDatasetName.GetAt(i))
					{
						CComBSTR sName = strTemp.AllocSysString();
						ipGHFeatureWorkspace->OpenFeatureClass(sName,&ipFeatureClass);
						//ע��ipFeatureClassһ�㲻ΪNULL�����ΪNULL������Ҫ���SDE���Ƿ�������
						if(ipFeatureClass == NULL)continue;
						if(strTemp == _T("XJXZQ"))
						{
							CComBSTR sMapName;
							ipMap->get_Name(&sMapName);
							CString strMapName(sMapName);
							if (strMapName.Trim() == _T("ʾ��ͼ"))
							{
								CComQIPtr<IGeoDataset> ipGeoDataset = ipFeatureClass;
								/*CComQIPtr<IEnvelope> ipEnvelope1;
								ipGeoDataset->get_Extent(&ipEnvelope1);
								ipEnvelope = ipEnvelope1;*/
								ipEnvelope.Release();
								ipGeoDataset->get_Extent(&ipEnvelope);
							}
							else
							{
								if (strXZQDM.GetLength() < 9)
								{
									CComQIPtr<IGeoDataset> ipGeoDataset = ipFeatureClass;
									ipEnvelope.Release();
									ipGeoDataset->get_Extent(&ipEnvelope);
								}
								else
								{
									CComQIPtr<IQueryFilter> ipQueryFilter;
									ipQueryFilter.CoCreateInstance(CLSID_QueryFilter);
									CString strTemp1 = _T("xzqdm = ") + strXZQDM;
									CComBSTR sCondition = strTemp1.AllocSysString();
									ipQueryFilter->put_WhereClause(sCondition);
									CComQIPtr<IFeatureCursor> ipFeatureCursor;
									ipFeatureClass->Search(ipQueryFilter,FALSE,&ipFeatureCursor);
									CComQIPtr<IFeature> ipFeature;
									ipFeatureCursor->NextFeature(&ipFeature);
									if (ipFeature != NULL)
									{
										ipEnvelope.Release();
										ipFeature->get_Extent(&ipEnvelope);
									}
								}
							}
						}
						break;
					}
				}

				if (ipFeatureClass != NULL)
				{
					CComQIPtr<IDataset> ipDataset = ipFeatureClass;
					CComBSTR sName;
					ipDataset->get_BrowseName(&sName);
					ipDatasetName->put_Name(sName);
					CComQIPtr<IName> ipName1;
					ipDataset->get_FullName(&ipName1);
					ipDatasetName = ipName1;
					VARIANT_BOOL bSucceed;//0 == FALSE, -1 == TRUE
					ipDataLayer->Connect((CComQIPtr<IName>)ipDatasetName,&bSucceed);

					ipFeatureLayer->putref_FeatureClass(ipFeatureClass);
					ipFeatureLayer->put_Visible(VARIANT_TRUE);
					CComQIPtr<IFeatureLayerDefinition> ipFeatureLayerDefinition = ipFeatureLayer;
					CComBSTR sSQL;
					ipFeatureLayerDefinition->get_DefinitionExpression(&sSQL);
					CString strSQL(sSQL);
					if (!strSQL.IsEmpty())
					{
						strSQL.Replace(_T("left"),_T("substr"));
						strSQL.Replace(_T("["),_T(""));
						strSQL.Replace(_T("]"),_T(""));
						if (strSQL.Find(_T(",1,")) == -1)
						{
							strSQL.Replace(_T(","),_T(",1,"));
						}
						if (strXZQDM.GetLength() < 9)
						{
							strSQL.Replace(_T(",9"),_T(",6"));
						}
						else
						{
							strSQL.Replace(_T(",6"),_T(",9"));
						}
						sSQL = strSQL.AllocSysString();
						ipFeatureLayerDefinition->put_DefinitionExpression(sSQL);
					}
				}
				ipLayer.Release();
				ipEnumLayer->Next(&ipLayer);
			}

			CComQIPtr<IActiveView> ipActiveView = ipMap;
			ipActiveView->put_Extent(ipEnvelope);
			CComQIPtr<IArea> ipArea = ipEnvelope;
			CComQIPtr<IPoint> ipPoint;
			ipArea->get_Centroid(&ipPoint);
			CComQIPtr<IEnvelope> ipEnvelope1;
			ipActiveView->get_Extent(&ipEnvelope1);
			ipEnvelope1->CenterAt(ipPoint);
			ipPageLayoutControl->ZoomToWholePage();
			//���������ˢ�µĻ�����ÿ��Map����ˢ��һ�Σ����Իῴ����˸��
			//��˽�ˢ�·ŵ�����Map�������֮��������ֻ��ˢ��һ����ͼ
			/*CComQIPtr<IScreenDisplay> ipScreenDisplay;
			ipActiveView->get_ScreenDisplay(&ipScreenDisplay);
			ipScreenDisplay->UpdateWindow();
			ipActiveView->Refresh();*/			
		}

		/*CComQIPtr<IElement> ipElement1;
		ipGraphicsContainer->Next(&ipElement1);
		ipElement = ipElement1;	*/	
		ipElement.Release();
		ipGraphicsContainer->Next(&ipElement);
	}
	//�����е���ͼˢ�¶���������һ��ˢ�£�������˸
	ipPageLayoutControl->Refresh(esriViewAll);
}



void CExportThemeMap::ExportThemeMap2(CComQIPtr<IWorkspace> ipXZWorkspace,
					 CComQIPtr<IWorkspace> ipGHWorkspace,
					 CString strXZQDM,
					 CString strConfigFile,
					 CString strRootName,
					 CStringArray& strArrDatasetName)
{
	//��ʼ��������ͼ������Ϣ
	CMap<CString,LPCTSTR,CString,LPCTSTR> strMapConfig;
	if(!InitViewConfig(strConfigFile,strRootName,strMapConfig))
	{
		return;
	}

	double doScale = 1;
	BOOL bPortrait = FALSE;
	int iSideEdge = 0;
	int iTopEdge = 0;
	if(strXZQDM.IsEmpty() || strMapConfig.IsEmpty() || strArrDatasetName.IsEmpty())return;
	//���ò�����ͼΪ��ǰ���ͼ
	CComQIPtr<GisqLib::IGSActiveView> ipGSActiveView;
	for (int i = 0;i < m_ipGisqFramework->GetViewCount();i++)
	{
		ipGSActiveView =m_ipGisqFramework->GetViewItem(i);
		if (ipGSActiveView->GetViewType() == (short)VT_PageLayoutView)
		{
			GisqLib::IGSActiveView* ipTemp = ipGSActiveView;
			m_ipGisqFramework->SetActiveView(ipTemp);
			break;
		}
	}
	ASSERT(ipGSActiveView != NULL);
	//��ȡ������ͼ�ؼ�
	CComQIPtr<IPageLayoutControl> ipPageLayoutControl = ipGSActiveView->GetControl();
	CString strValue = _T("");
	//���ò�����ͼ����(���ڡ�����...)
	if(strMapConfig.Lookup(_T("MODELPATH"),strValue))
	{
		//��ȡMXD�ĵ�����Ŀ¼
		TCHAR szAppFullPath[MAX_PATH];
		DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(CHAR);
		GetModuleFileName(NULL,szAppFullPath,dBufferSize);
		CString strAppFolder(szAppFullPath);
		strAppFolder = strAppFolder.Mid(0,strAppFolder.ReverseFind('\\'));
		CString strMXDFilePath = strAppFolder + _T("\\data\\template\\");
		strMXDFilePath += strValue;
		//_bstr_t strDocumentPath = strValue;
		_bstr_t strDocumentPath = strMXDFilePath;
		//edit by lh in 2016.09.21 begin
		//ʹ��arcgis�ӿڼ����ĵ�������ʹ��AE��ܣ�
		//��Ҫ����Ϊʹ��AE����ظ�����MXD�ĵ��ᱻ�Ż������ظ�����ʱ���ɹ�
		/*VARIANT_BOOL bSecceed = m_ipGisqFramework->OpenMapDocument(strDocumentPath);
		if (bSecceed == VARIANT_FALSE)return;*/
		VARIANT_BOOL bSecceed;
		ipPageLayoutControl->CheckMxFile(strDocumentPath,&bSecceed);
		if(bSecceed == VARIANT_FALSE)return;
		HRESULT hr = ipPageLayoutControl->LoadMxFile(strDocumentPath);
		if (FAILED(hr))return;
		//edit by lh end
		strValue = _T("");
	}

	if (strMapConfig.Lookup(_T("MAKEWHERE"),strValue))
	{
		CComPtr<IElement> ipElement = NULL;
		CComBSTR strTagName(_T("MAPPUBLIC"));
		//ipPageLayoutControl->FindElementByName(strTemp,1,&ipElement);
		ipPageLayoutControl->FindElementByName(strTagName,1,&ipElement);
		if (ipElement != NULL)
		{
			strValue += _T("	");
			CComBSTR strText;
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			ipTextElement->get_Text(&strText);
			strText = strValue + strText;
			ipTextElement->put_Text(strText);
		}
		strValue = _T("");
	}
	if(strMapConfig.Lookup(_T("MAKEDATE"),strValue))
	{
		CComPtr<IElement> ipElement = NULL;
		CComBSTR strTagName(_T("MAPRQ"));
		ipPageLayoutControl->FindElementByName(strTagName,1,&ipElement);
		if (ipElement != NULL)
		{	
			//����ȡ�õ�ʱ���ַ����ĸ�ʽ�ֹ�������ʱ����Ϣ,���Ҫ�������ַ����ĸ�ʽҪȷ��������д��ȷ�������Ȳ�����
			CComBSTR strText;
			CComBSTR strTemp(_T("���ڣ�"));
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			ipTextElement->get_Text(&strText);
			strTemp += strText;
			ipTextElement->put_Text(strTemp);
		}
		strValue = _T("");
	}
	if (strMapConfig.Lookup(_T("MAPZF"),strValue))
	{
		CComPtr<IElement> ipElement = NULL;
		BSTR strTagName(_T("ZHEN"));
		ipPageLayoutControl->FindElementByName(strTagName,1,&ipElement);
		SysFreeString(strTagName);
		if (ipElement != NULL)
		{
			CComBSTR strText;
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			ipTextElement->get_Text(&strText);
			strText = strValue + strText;
			ipTextElement->put_Text(strText);
		}
		strValue = _T("");
	}
	if (strMapConfig.Lookup(_T("MAPPUBLIC"),strValue))
	{
		CComPtr<IElement> ipElement = NULL;
		BSTR strTagName(_T("XIAN"));
		ipPageLayoutControl->FindElementByName(strTagName,1,&ipElement);
		SysFreeString(strTagName);
		if (ipElement != NULL)
		{
			CComBSTR strText;
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			ipTextElement->get_Text(&strText);
			strText = strValue + strText;
			ipTextElement->put_Text(strText);
		}
		strValue = _T("");
	}
	if (strMapConfig.Lookup(_T("MAPNAME"),strValue))
	{
		CComPtr<IElement> ipElement = NULL;
		CComBSTR strTagName(_T("MAPTITLE"));
		ipPageLayoutControl->FindElementByName(strTagName,1,&ipElement);
		if (ipElement != NULL)
		{
			CComBSTR strText;
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			strText = strValue;
			ipTextElement->put_Text(strText);
		}
		strValue = _T("");
	}
	//��ȡ������ͼ��������Ϣ
	if (strMapConfig.Lookup(_T("SCALE"),strValue))
	{
		doScale = _wtof(strValue);
		strValue = _T("");
	}
	if (strMapConfig.Lookup(_T("DIRECTION"),strValue))
	{
		//bPortrait = (BOOL)_wtoi(strValue);
		bPortrait = strValue == _T("True") ? TRUE:FALSE;
		CComPtr<IPage> ipPage;
		ipPageLayoutControl->get_Page(&ipPage);
		if (bPortrait)
		{
			ipPage->put_Orientation((short)1);
		}
		else
		{
			ipPage->put_Orientation((short)2);
		}
		strValue = _T("");
	}
	if (strMapConfig.Lookup(_T("SIDEEDGE"),strValue))
	{
		iSideEdge = _wtoi(strValue);
		strValue = _T("");
	}
	if (strMapConfig.Lookup(_T("TOPEDGE"),strValue))
	{
		iTopEdge = _wtoi(strValue);
		strValue = _T("");
	}
	/*if (strMapConfig.Lookup(_T("PAGESIZE"),strValue) && !strValue.IsEmpty())
	{
		CComQIPtr<IPage> ipPage;
		ipPageLayoutControl->get_Page(&ipPage);
		if (ipPage != NULL)
		{
			if (_T("�Զ���") == strValue)
			{
				CString strWidth,strHeight;
				if (strMapConfig.Lookup(_T("PAGEWIDTH"),strWidth) && strMapConfig.Lookup(_T("PAGEHEIGHT"),strHeight))
				{
					double dbWidth = _wtof(strWidth);
					double dbHeight = _wtof(strHeight);
					if (dbWidth > 0 && dbHeight > 0)
					{
						ipPage->PutCustomSize(dbWidth,dbHeight);
					}
				}
			}
			else if ( _T("A0") == strValue)
			{
				ipPage->put_FormID(esriPageFormA0);
			}
			else if(_T("A1") == strValue)
			{
				ipPage->put_FormID(esriPageFormA1);
			}
			else if (_T("A2") == strValue)
			{
				ipPage->put_FormID(esriPageFormA2);
			}
			else if (_T("A3") == strValue)
			{
				ipPage->put_FormID(esriPageFormA3);
			}
			else if(_T("A4") == strValue)
			{
				ipPage->put_FormID(esriPageFormA4);
			}
			else if (_T("A5") == strValue)
			{
				ipPage->put_FormID(esriPageFormA5);
			}
		}		
		strValue = _T("");
	}*/
	
	CComQIPtr<IFeatureWorkspace> ipGHFeatureWorkspace = ipGHWorkspace;
	CComQIPtr<IGraphicsContainer> ipGraphicsContainer;
	ipPageLayoutControl->get_GraphicsContainer(&ipGraphicsContainer);
	ipGraphicsContainer->Reset();
	CComQIPtr<IElement> ipElement;
	ipGraphicsContainer->Next(&ipElement);
	//����Element
	while(ipElement != NULL)
	{
		CComQIPtr<IEnvelope> ipEnvelope;
		CComQIPtr<IMapFrame> ipMapFrame = ipElement;
		if (ipMapFrame != NULL)
		{
			CComQIPtr<IMap> ipMap;
			ipMapFrame->get_Map(&ipMap);
			CComQIPtr<IEnumLayer> ipEnumLayer;

			CComQIPtr<IUID> uid;
			uid.CoCreateInstance(CLSID_UID);
			uid->put_Value(_variant_t(_T("{40A9E885-5533-11d0-98BE-00805F7CED21}")));
			ipMap->get_Layers(uid,TRUE,&ipEnumLayer);
			ipEnumLayer->Reset();
			CComQIPtr<ILayer> ipLayer;
			ipEnumLayer->Next(&ipLayer);
			//����ͼ��
			while (ipLayer != NULL)
			{
				CComQIPtr<IFeatureClass> ipFeatureClass;
				CComQIPtr<IFeatureLayer> ipFeatureLayer = ipLayer;
				CComQIPtr<IDataLayer> ipDataLayer = ipFeatureLayer;
				CComQIPtr<IName> ipName;
				ipDataLayer->get_DataSourceName(&ipName);
				CComQIPtr<IDatasetName> ipDatasetName = ipName;
				CComBSTR strName;
				ipDatasetName->get_Name(&strName);
				CString strTemp(strName);
				strTemp.Replace(_T("SDE"),_T(""));
				int nCount = strArrDatasetName.GetCount();
				for(int i = 0;i < nCount;i++)
				{
					if (strTemp == strArrDatasetName.GetAt(i))
					{
						CComBSTR sName = strTemp.AllocSysString();
						ipGHFeatureWorkspace->OpenFeatureClass(sName,&ipFeatureClass);
						//ע��ipFeatureClassһ�㲻ΪNULL�����ΪNULL������Ҫ���SDE���Ƿ�������
						if(ipFeatureClass == NULL)continue;
						if(strTemp == _T("XJXZQ"))
						{
							CComBSTR sMapName;
							ipMap->get_Name(&sMapName);
							CString strMapName(sMapName);
							if (strMapName.Trim() == _T("ʾ��ͼ"))
							{
								CComQIPtr<IGeoDataset> ipGeoDataset = ipFeatureClass;
								/*CComQIPtr<IEnvelope> ipEnvelope1;
								ipGeoDataset->get_Extent(&ipEnvelope1);
								ipEnvelope = ipEnvelope1;*/
								ipEnvelope.Release();
								ipGeoDataset->get_Extent(&ipEnvelope);
							}
							else
							{
								if (strXZQDM.GetLength() < 9)
								{
									CComQIPtr<IGeoDataset> ipGeoDataset = ipFeatureClass;
									ipEnvelope.Release();
									ipGeoDataset->get_Extent(&ipEnvelope);
								}
								else
								{
									CComQIPtr<IQueryFilter> ipQueryFilter;
									ipQueryFilter.CoCreateInstance(CLSID_QueryFilter);
									CString strTemp1 = _T("xzqdm = ") + strXZQDM;
									CComBSTR sCondition = strTemp1.AllocSysString();
									ipQueryFilter->put_WhereClause(sCondition);
									CComQIPtr<IFeatureCursor> ipFeatureCursor;
									ipFeatureClass->Search(ipQueryFilter,FALSE,&ipFeatureCursor);
									CComQIPtr<IFeature> ipFeature;
									ipFeatureCursor->NextFeature(&ipFeature);
									if (ipFeature != NULL)
									{
										ipEnvelope.Release();
										ipFeature->get_Extent(&ipEnvelope);
									}
								}
							}
						}
						break;
					}
				}

				if (ipFeatureClass != NULL)
				{
					CComQIPtr<IDataset> ipDataset = ipFeatureClass;
					CComBSTR sName;
					ipDataset->get_BrowseName(&sName);
					ipDatasetName->put_Name(sName);
					CComQIPtr<IName> ipName1;
					ipDataset->get_FullName(&ipName1);
					ipDatasetName = ipName1;
					VARIANT_BOOL bSucceed;//0 == FALSE, -1 == TRUE
					ipDataLayer->Connect((CComQIPtr<IName>)ipDatasetName,&bSucceed);

					ipFeatureLayer->putref_FeatureClass(ipFeatureClass);
					ipFeatureLayer->put_Visible(VARIANT_TRUE);
					CComQIPtr<IFeatureLayerDefinition> ipFeatureLayerDefinition = ipFeatureLayer;
					CComBSTR sSQL;
					ipFeatureLayerDefinition->get_DefinitionExpression(&sSQL);
					CString strSQL(sSQL);
					if (!strSQL.IsEmpty())
					{
						strSQL.Replace(_T("left"),_T("substr"));
						strSQL.Replace(_T("["),_T(""));
						strSQL.Replace(_T("]"),_T(""));
						if (strSQL.Find(_T(",1,")) == -1)
						{
							strSQL.Replace(_T(","),_T(",1,"));
						}
						if (strXZQDM.GetLength() < 9)
						{
							strSQL.Replace(_T(",9"),_T(",6"));
						}
						else
						{
							strSQL.Replace(_T(",6"),_T(",9"));
						}
						//add by lcc 2016-11-14 �޸���ͼ���ݹ���BUG
						int nDMStart =  strSQL.Find(_T("'"));
						int nDMEnd = strSQL.Find(_T("'"),nDMStart + 1);
						CString DM = strSQL.Mid(nDMStart,nDMEnd - nDMStart + 1);
						strSQL.Replace(DM,strXZQDM);
						//end 2016-11-14
						sSQL = strSQL.AllocSysString();
						ipFeatureLayerDefinition->put_DefinitionExpression(sSQL);
					}
				}
				ipLayer.Release();
				ipEnumLayer->Next(&ipLayer);
			}

			CComQIPtr<IActiveView> ipActiveView = ipMap;
			if (ipEnvelope != NULL)
			{
				ipActiveView->put_Extent(ipEnvelope);
				CComQIPtr<IArea> ipArea = ipEnvelope;
				CComQIPtr<IPoint> ipPoint;
				ipArea->get_Centroid(&ipPoint);
				CComQIPtr<IEnvelope> ipEnvelope1;
				ipActiveView->get_Extent(&ipEnvelope1);
				ipEnvelope1->CenterAt(ipPoint);
			}
 			ipPageLayoutControl->ZoomToWholePage();
			//���������ˢ�µĻ�����ÿ��Map����ˢ��һ�Σ����Իῴ����˸��
			//��˽�ˢ�·ŵ�����Map�������֮��������ֻ��ˢ��һ����ͼ
			/*CComQIPtr<IScreenDisplay> ipScreenDisplay;
			ipActiveView->get_ScreenDisplay(&ipScreenDisplay);
			ipScreenDisplay->UpdateWindow();
			ipActiveView->Refresh();*/			
		}

		/*CComQIPtr<IElement> ipElement1;
		ipGraphicsContainer->Next(&ipElement1);
		ipElement = ipElement1;	*/	
		ipElement.Release();
		ipGraphicsContainer->Next(&ipElement);
	}
	//�����е���ͼˢ�¶���������һ��ˢ�£�������˸
	ipPageLayoutControl->Refresh(esriViewAll);

}

BOOL CExportThemeMap::InitViewConfig(CString strConfigFile,
					CString strRootName,
					CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig)
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
	//CString str1 =_T("123");  
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
		TiXmlHandle xmlHandle(&xmlDoc);
		TiXmlElement* pElement = xmlHandle.FirstChild(pszRootName).FirstChild().ToElement();
		while (pElement != NULL)
		{
			const char* pszName = pElement->Value();
			const char* pszValue = pElement->FirstAttribute()->Value();
			CString strName(pszName);
			CString strValue(pszValue);
			strMapConfig.SetAt(strName.GetBuffer(),strValue.GetBuffer());
			if (pElement->NextSibling() == NULL)break;
			pElement = pElement->NextSibling()->ToElement();
		}

		/*TiXmlElement* pEleModelPath		= xmlHandle.FirstChild(pszRootName).FirstChild("MODELPATH").ToElement();
		TiXmlElement* pEleMapName		= xmlHandle.FirstChild(pszRootName).FirstChild("MAPNAME").ToElement();
		TiXmlElement* pEleMakeWhere		= xmlHandle.FirstChild(pszRootName).FirstChild("MAKEWHERE").ToElement();
		TiXmlElement* pEleProducer		= xmlHandle.FirstChild(pszRootName).FirstChild("PRODUCER").ToElement();
		TiXmlElement* pEleMakeDate		= xmlHandle.FirstChild(pszRootName).FirstChild("MAKEDATE").ToElement();
		TiXmlElement* pEleChecker		= xmlHandle.FirstChild(pszRootName).FirstChild("CHECKER").ToElement();
		TiXmlElement* pEleAuditor		= xmlHandle.FirstChild(pszRootName).FirstChild("AUDITOR").ToElement();
		TiXmlElement* pEleScale			= xmlHandle.FirstChild(pszRootName).FirstChild("SCALE").ToElement();
		TiXmlElement* pElePageSize		= xmlHandle.FirstChild(pszRootName).FirstChild("PAGESIZE").ToElement();
		TiXmlElement* pElePageWidth		= xmlHandle.FirstChild(pszRootName).FirstChild("PAGEWIDTH").ToElement();
		TiXmlElement* pElepPageHeight	= xmlHandle.FirstChild(pszRootName).FirstChild("PAGEHEIGHT").ToElement();
		TiXmlElement* pEleSideEdge		= xmlHandle.FirstChild(pszRootName).FirstChild("SIDEEDGE").ToElement();
		TiXmlElement* pEleTopEdge		= xmlHandle.FirstChild(pszRootName).FirstChild("TOPEDGE").ToElement();
		TiXmlElement* pEleAuto			= xmlHandle.FirstChild(pszRootName).FirstChild("AUTO").ToElement();
		TiXmlElement* pEleImage			= xmlHandle.FirstChild(pszRootName).FirstChild("IMAGE").ToElement();
		TiXmlElement* pEleDirection		= xmlHandle.FirstChild(pszRootName).FirstChild("DIRECTION").ToElement();
		

		if (pEleModelPath != NULL)
		{
			char* pValue = pEleModelPath->Attribute("value");
			CString strValue(pValue);
			strMapConfig.SetAt(_T("MODELPATH"),strValue.GetBuffer());
		}
		if (pEleMapName != NULL)
		{
			char* pValue = pEleMapName->Attribute("value");
			CString strValue(pValue);
			strMapConfig.SetAt(_T("MAPNAME"),strValue.GetBuffer());
		}
		if (pEleMakeWhere != NULL)
		{
			char* pValue = pEleMakeWhere->Attribute("value");
			CString strValue(pValue);
			strMapConfig.SetAt(_T("MAKEWHERE"),strValue.GetBuffer());
		}
		if (pEleProducer != NULL)
		{
			char* pValue = pEleProducer->Attribute("value");
			CString strValue(pValue);
			strMapConfig.SetAt(_T("PRODUCER"),strValue.GetBuffer());
		}
		if (pEleMakeDate != NULL)
		{
			char* pValue = pEleMakeDate->Attribute("value");
			CString strValue(pValue);
			strMapConfig.SetAt(_T("MAKEDATE"),strValue.GetBuffer());
		}
		if (pEleChecker != NULL)
		{
			char* pValue = pEleChecker->Attribute("value");
			CString strValue(pValue);
			strMapConfig.SetAt(_T("CHECKER"),strValue.GetBuffer());
		}
		if (pEleAuditor != NULL)
		{
			char* pValue = pEleAuditor->Attribute("value");
			CString strValue(pValue);
			strMapConfig.SetAt(_T("AUDITOR"),strValue.GetBuffer());
		}
		if (pEleScale != NULL)
		{
			char* pValue = pEleScale->Attribute("value");
			CString strValue(pValue);
			strMapConfig.SetAt(_T("SCALE"),strValue.GetBuffer());
		}
		if (pElePageSize != NULL)
		{
			char* pValue = pElePageSize->Attribute("value");
			CString strValue(pValue);
			strMapConfig.SetAt(_T("PAGESIZE"),strValue.GetBuffer());
		}
		if (pElePageWidth != NULL)
		{
			char* pValue = pElePageWidth->Attribute("value");
			CString strValue(pValue);
			strMapConfig.SetAt(_T("PAGEWIDTH"),strValue.GetBuffer());
		}
		if (pElepPageHeight != NULL)
		{
			char* pValue = pElepPageHeight->Attribute("value");
			CString strValue(pValue);
			strMapConfig.SetAt(_T("PAGEHEIGHT"),strValue.GetBuffer());
		}
		if (pEleSideEdge != NULL)
		{
			char* pValue = pEleSideEdge->Attribute("value");
			CString strValue(pValue);
			strMapConfig.SetAt(_T("SIDEEDGE"),strValue.GetBuffer());
		}
		if (pEleTopEdge != NULL)
		{
			char* pValue = pEleTopEdge->Attribute("value");
			CString strValue(pValue);
			strMapConfig.SetAt(_T("TOPEDGE"),strValue.GetBuffer());
		}
		if (pEleAuto != NULL)
		{
			char* pValue = pEleAuto->Attribute("value");
			CString strValue(pValue);
			strMapConfig.SetAt(_T("AUTO"),strValue.GetBuffer());
		}
		if (pEleImage != NULL)
		{
			char* pValue = pEleImage->Attribute("value");
			CString strValue(pValue);
			strMapConfig.SetAt(_T("IMAGE"),strValue.GetBuffer());
		}
		if (pEleDirection != NULL)
		{
			char* pValue = pEleDirection->Attribute("value");
			CString strValue(pValue);
			strMapConfig.SetAt(_T("DIRECTION"),strValue.GetBuffer());
		}*/
	}
	else
	{
		delete[] pszRootName;
		delete[] pszConfigFile;
		return FALSE;
	}
	delete[] pszRootName;
	delete[] pszConfigFile;
	return TRUE;
}

void CExportThemeMap::ExportThemeMap3(CComQIPtr<IWorkspace> ipXZWorkspace,
					 CComQIPtr<IWorkspace> ipGHWorkspace,
					 CString strXZQDM,
					 CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig,
					 CStringArray& strArrDatasetName)
{
	//��ʼ��������ͼ������Ϣ
	if(strMapConfig.GetCount() == 0)
	{
		return;
	}

	double doScale = 1;
	BOOL bPortrait = FALSE;
	int iSideEdge = 0;
	int iTopEdge = 0;
	if(strXZQDM.IsEmpty() || strMapConfig.IsEmpty() || strArrDatasetName.IsEmpty())return;
	//���ò�����ͼΪ��ǰ���ͼ
	CComQIPtr<GisqLib::IGSActiveView> ipGSActiveView;
	for (int i = 0;i < m_ipGisqFramework->GetViewCount();i++)
	{
		ipGSActiveView =m_ipGisqFramework->GetViewItem(i);
		if (ipGSActiveView->GetViewType() == (short)VT_PageLayoutView)
		{
			GisqLib::IGSActiveView* ipTemp = ipGSActiveView;
			m_ipGisqFramework->SetActiveView(ipTemp);
			break;
		}
	}
	ASSERT(ipGSActiveView != NULL);
	//��ȡ������ͼ�ؼ�
	CComQIPtr<IPageLayoutControl> ipPageLayoutControl = ipGSActiveView->GetControl();
	CString strValue = _T("");
	//���ò�����ͼ����(���ڡ�����...)
	if(strMapConfig.Lookup(_T("MODELPATH"),strValue))
	{
		//��ȡMXD�ĵ�����Ŀ¼
		TCHAR szAppFullPath[MAX_PATH];
		DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(CHAR);
		GetModuleFileName(NULL,szAppFullPath,dBufferSize);
		CString strAppFolder(szAppFullPath);
		strAppFolder = strAppFolder.Mid(0,strAppFolder.ReverseFind('\\'));
		CString strMXDFilePath = strAppFolder + _T("\\data\\template\\");
		strMXDFilePath += strValue;
		//_bstr_t strDocumentPath = strValue;
		_bstr_t strDocumentPath = strMXDFilePath;
		//edit by lh in 2016.09.21 begin
		//ʹ��arcgis�ӿڼ����ĵ�������ʹ��AE��ܣ�
		//��Ҫ����Ϊʹ��AE����ظ�����MXD�ĵ��ᱻ�Ż������ظ�����ʱ���ɹ�
		/*VARIANT_BOOL bSecceed = m_ipGisqFramework->OpenMapDocument(strDocumentPath);
		if (bSecceed == VARIANT_FALSE)return;*/
		VARIANT_BOOL bSecceed;
		ipPageLayoutControl->CheckMxFile(strDocumentPath,&bSecceed);
		if(bSecceed == VARIANT_FALSE)return;
		HRESULT hr = ipPageLayoutControl->LoadMxFile(strDocumentPath);
		if (FAILED(hr))return;
		//edit by lh end
		strValue = _T("");
	}

	if (strMapConfig.Lookup(_T("MAKEWHERE"),strValue))
	{
		CComPtr<IElement> ipElement = NULL;
		CComBSTR strTagName(_T("MAPPUBLIC"));
		//ipPageLayoutControl->FindElementByName(strTemp,1,&ipElement);
		ipPageLayoutControl->FindElementByName(strTagName,1,&ipElement);
		if (ipElement != NULL)
		{
			strValue += _T("	");
			CComBSTR strText;
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			ipTextElement->get_Text(&strText);
			strText = strValue + strText;
			ipTextElement->put_Text(strText);
		}
		strValue = _T("");
	}
	if(strMapConfig.Lookup(_T("MAKEDATE"),strValue))
	{
		CComPtr<IElement> ipElement = NULL;
		CComBSTR strTagName(_T("MAPRQ"));
		ipPageLayoutControl->FindElementByName(strTagName,1,&ipElement);
		if (ipElement != NULL)
		{	
			//����ȡ�õ�ʱ���ַ����ĸ�ʽ�ֹ�������ʱ����Ϣ,���Ҫ�������ַ����ĸ�ʽҪȷ��������д��ȷ�������Ȳ�����
			CComBSTR strText;
			CComBSTR strTemp(_T("���ڣ�"));
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			ipTextElement->get_Text(&strText);
			strTemp += strText;
			ipTextElement->put_Text(strTemp);
		}
		strValue = _T("");
	}
	if (strMapConfig.Lookup(_T("MAPZF"),strValue))
	{
		CComPtr<IElement> ipElement = NULL;
		BSTR strTagName(_T("ZHEN"));
		ipPageLayoutControl->FindElementByName(strTagName,1,&ipElement);
		SysFreeString(strTagName);
		if (ipElement != NULL)
		{
			CComBSTR strText;
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			ipTextElement->get_Text(&strText);
			strText = strValue + strText;
			ipTextElement->put_Text(strText);
		}
		strValue = _T("");
	}
	if (strMapConfig.Lookup(_T("MAPPUBLIC"),strValue))
	{
		CComPtr<IElement> ipElement = NULL;
		BSTR strTagName(_T("XIAN"));
		ipPageLayoutControl->FindElementByName(strTagName,1,&ipElement);
		SysFreeString(strTagName);
		if (ipElement != NULL)
		{
			CComBSTR strText;
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			ipTextElement->get_Text(&strText);
			strText = strValue + strText;
			ipTextElement->put_Text(strText);
		}
		strValue = _T("");
	}
	if (strMapConfig.Lookup(_T("MAPNAME"),strValue))
	{
		CComPtr<IElement> ipElement = NULL;
		CComBSTR strTagName(_T("MAPTITLE"));
		ipPageLayoutControl->FindElementByName(strTagName,1,&ipElement);
		if (ipElement != NULL)
		{
			CComBSTR strText;
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			strText = strValue;
			ipTextElement->put_Text(strText);
		}
		strValue = _T("");
	}
	//��ȡ������ͼ��������Ϣ
	if (strMapConfig.Lookup(_T("SCALE"),strValue))
	{
		doScale = _wtof(strValue);
		strValue = _T("");
	}
	if (strMapConfig.Lookup(_T("DIRECTION"),strValue))
	{
		//bPortrait = (BOOL)_wtoi(strValue);
		bPortrait = strValue == _T("True") ? TRUE:FALSE;
		CComPtr<IPage> ipPage;
		ipPageLayoutControl->get_Page(&ipPage);
		if (bPortrait)
		{
			ipPage->put_Orientation((short)1);
		}
		else
		{
			ipPage->put_Orientation((short)2);
		}
		strValue = _T("");
	}
	if (strMapConfig.Lookup(_T("SIDEEDGE"),strValue))
	{
		iSideEdge = _wtoi(strValue);
		strValue = _T("");
	}
	if (strMapConfig.Lookup(_T("TOPEDGE"),strValue))
	{
		iTopEdge = _wtoi(strValue);
		strValue = _T("");
	}
	/*if (strMapConfig.Lookup(_T("PAGESIZE"),strValue) && !strValue.IsEmpty())
	{
		CComQIPtr<IPage> ipPage;
		ipPageLayoutControl->get_Page(&ipPage);
		if (ipPage != NULL)
		{
			if (_T("�Զ���") == strValue)
			{
				CString strWidth,strHeight;
				if (strMapConfig.Lookup(_T("PAGEWIDTH"),strWidth) && strMapConfig.Lookup(_T("PAGEHEIGHT"),strHeight))
				{
					double dbWidth = _wtof(strWidth);
					double dbHeight = _wtof(strHeight);
					if (dbWidth > 0 && dbHeight > 0)
					{
						ipPage->PutCustomSize(dbWidth,dbHeight);
					}
				}
			}
			else if ( _T("A0") == strValue)
			{
				ipPage->put_FormID(esriPageFormA0);
			}
			else if(_T("A1") == strValue)
			{
				ipPage->put_FormID(esriPageFormA1);
			}
			else if (_T("A2") == strValue)
			{
				ipPage->put_FormID(esriPageFormA2);
			}
			else if (_T("A3") == strValue)
			{
				ipPage->put_FormID(esriPageFormA3);
			}
			else if(_T("A4") == strValue)
			{
				ipPage->put_FormID(esriPageFormA4);
			}
			else if (_T("A5") == strValue)
			{
				ipPage->put_FormID(esriPageFormA5);
			}
		}		
		strValue = _T("");
	}*/
	
	CComQIPtr<IFeatureWorkspace> ipGHFeatureWorkspace = ipGHWorkspace;
	CComQIPtr<IGraphicsContainer> ipGraphicsContainer;
	ipPageLayoutControl->get_GraphicsContainer(&ipGraphicsContainer);
	ipGraphicsContainer->Reset();
	CComQIPtr<IElement> ipElement;
	ipGraphicsContainer->Next(&ipElement);
	//����Element
	while(ipElement != NULL)
	{
		CComQIPtr<IEnvelope> ipEnvelope;
		CComQIPtr<IMapFrame> ipMapFrame = ipElement;
		if (ipMapFrame != NULL)
		{
			CComQIPtr<IMap> ipMap;
			ipMapFrame->get_Map(&ipMap);
			CComQIPtr<IEnumLayer> ipEnumLayer;

			CComQIPtr<IUID> uid;
			uid.CoCreateInstance(CLSID_UID);
			uid->put_Value(_variant_t(_T("{40A9E885-5533-11d0-98BE-00805F7CED21}")));
			ipMap->get_Layers(uid,TRUE,&ipEnumLayer);
			ipEnumLayer->Reset();
			CComQIPtr<ILayer> ipLayer;
			ipEnumLayer->Next(&ipLayer);
			//����ͼ��
			while (ipLayer != NULL)
			{
				CComQIPtr<IFeatureClass> ipFeatureClass;
				CComQIPtr<IFeatureLayer> ipFeatureLayer = ipLayer;
				CComQIPtr<IDataLayer> ipDataLayer = ipFeatureLayer;
				CComQIPtr<IName> ipName;
				ipDataLayer->get_DataSourceName(&ipName);
				CComQIPtr<IDatasetName> ipDatasetName = ipName;
				CComBSTR strName;
				ipDatasetName->get_Name(&strName);
				CString strTemp(strName);
				strTemp.Replace(_T("SDE"),_T(""));
				int nCount = strArrDatasetName.GetCount();
				for(int i = 0;i < nCount;i++)
				{
					if (strTemp == strArrDatasetName.GetAt(i))
					{
						CComBSTR sName = strTemp.AllocSysString();
						ipGHFeatureWorkspace->OpenFeatureClass(sName,&ipFeatureClass);
						//ע��ipFeatureClassһ�㲻ΪNULL�����ΪNULL������Ҫ���SDE���Ƿ�������
						if(ipFeatureClass == NULL)continue;
						if(strTemp == _T("XJXZQ"))
						{
							CComBSTR sMapName;
							ipMap->get_Name(&sMapName);
							CString strMapName(sMapName);
							if (strMapName.Trim() == _T("ʾ��ͼ"))
							{
								CComQIPtr<IGeoDataset> ipGeoDataset = ipFeatureClass;
								/*CComQIPtr<IEnvelope> ipEnvelope1;
								ipGeoDataset->get_Extent(&ipEnvelope1);
								ipEnvelope = ipEnvelope1;*/
								ipEnvelope.Release();
								ipGeoDataset->get_Extent(&ipEnvelope);
							}
							else
							{
								if (strXZQDM.GetLength() < 9)
								{
									CComQIPtr<IGeoDataset> ipGeoDataset = ipFeatureClass;
									ipEnvelope.Release();
									ipGeoDataset->get_Extent(&ipEnvelope);
								}
								else
								{
									CComQIPtr<IQueryFilter> ipQueryFilter;
									ipQueryFilter.CoCreateInstance(CLSID_QueryFilter);
									CString strTemp1 = _T("xzqdm = ") + strXZQDM;
									CComBSTR sCondition = strTemp1.AllocSysString();
									ipQueryFilter->put_WhereClause(sCondition);
									CComQIPtr<IFeatureCursor> ipFeatureCursor;
									ipFeatureClass->Search(ipQueryFilter,FALSE,&ipFeatureCursor);
									CComQIPtr<IFeature> ipFeature;
									ipFeatureCursor->NextFeature(&ipFeature);
									if (ipFeature != NULL)
									{
										ipEnvelope.Release();
										ipFeature->get_Extent(&ipEnvelope);
									}
								}
							}
						}
						break;
					}
				}

				if (ipFeatureClass != NULL)
				{
					CComQIPtr<IDataset> ipDataset = ipFeatureClass;
					CComBSTR sName;
					ipDataset->get_BrowseName(&sName);
					ipDatasetName->put_Name(sName);
					CComQIPtr<IName> ipName1;
					ipDataset->get_FullName(&ipName1);
					ipDatasetName = ipName1;
					VARIANT_BOOL bSucceed;//0 == FALSE, -1 == TRUE
					ipDataLayer->Connect((CComQIPtr<IName>)ipDatasetName,&bSucceed);

					ipFeatureLayer->putref_FeatureClass(ipFeatureClass);
					ipFeatureLayer->put_Visible(VARIANT_TRUE);
					CComQIPtr<IFeatureLayerDefinition> ipFeatureLayerDefinition = ipFeatureLayer;
					CComBSTR sSQL;
					ipFeatureLayerDefinition->get_DefinitionExpression(&sSQL);
					CString strSQL(sSQL);
					if (!strSQL.IsEmpty())
					{
						strSQL.Replace(_T("left"),_T("substr"));
						strSQL.Replace(_T("["),_T(""));
						strSQL.Replace(_T("]"),_T(""));
						if (strSQL.Find(_T(",1,")) == -1)
						{
							strSQL.Replace(_T(","),_T(",1,"));
						}
						if (strXZQDM.GetLength() < 9)
						{
							strSQL.Replace(_T(",9"),_T(",6"));
						}
						else
						{
							strSQL.Replace(_T(",6"),_T(",9"));
						}
						//add by lcc 2016-11-14 �޸���ͼ���ݹ���BUG
						int nDMStart =  strSQL.Find(_T("'"));
						int nDMEnd = strSQL.Find(_T("'"),nDMStart + 1);
						CString DM = strSQL.Mid(nDMStart,nDMEnd - nDMStart + 1);
						strSQL.Replace(DM,strXZQDM);
						//end 2016-11-14
						sSQL = strSQL.AllocSysString();
						ipFeatureLayerDefinition->put_DefinitionExpression(sSQL);
					}
				}
				ipLayer.Release();
				ipEnumLayer->Next(&ipLayer);
			}

			CComQIPtr<IActiveView> ipActiveView = ipMap;
			if (ipEnvelope != NULL)
			{
				ipActiveView->put_Extent(ipEnvelope);
				CComQIPtr<IArea> ipArea = ipEnvelope;
				CComQIPtr<IPoint> ipPoint;
				ipArea->get_Centroid(&ipPoint);
				CComQIPtr<IEnvelope> ipEnvelope1;
				ipActiveView->get_Extent(&ipEnvelope1);
				ipEnvelope1->CenterAt(ipPoint);
			}
 			ipPageLayoutControl->ZoomToWholePage();
			//���������ˢ�µĻ�����ÿ��Map����ˢ��һ�Σ����Իῴ����˸��
			//��˽�ˢ�·ŵ�����Map�������֮��������ֻ��ˢ��һ����ͼ
			/*CComQIPtr<IScreenDisplay> ipScreenDisplay;
			ipActiveView->get_ScreenDisplay(&ipScreenDisplay);
			ipScreenDisplay->UpdateWindow();
			ipActiveView->Refresh();*/			
		}

		/*CComQIPtr<IElement> ipElement1;
		ipGraphicsContainer->Next(&ipElement1);
		ipElement = ipElement1;	*/	
		ipElement.Release();
		ipGraphicsContainer->Next(&ipElement);
	}
	//�����е���ͼˢ�¶���������һ��ˢ�£�������˸
	ipPageLayoutControl->Refresh(esriViewAll);

}

void CExportThemeMap::ExportThemeMap4(CComQIPtr<IWorkspace> ipGHWorkspace,
					 CString strXZQDM,
					 CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig)
{
	//��ʼ��������ͼ������Ϣ

	double doScale = 1;
	BOOL bPortrait = FALSE;
	int iSideEdge = 0;
	int iTopEdge = 0;
	if(strXZQDM.IsEmpty() || strMapConfig.IsEmpty())return;
	//���ò�����ͼΪ��ǰ���ͼ
	CComQIPtr<GisqLib::IGSActiveView> ipGSActiveView;
	for (int i = 0;i < m_ipGisqFramework->GetViewCount();i++)
	{
		ipGSActiveView =m_ipGisqFramework->GetViewItem(i);
		if (ipGSActiveView->GetViewType() == (short)VT_PageLayoutView)
		{
			GisqLib::IGSActiveView* ipTemp = ipGSActiveView;
			m_ipGisqFramework->SetActiveView(ipTemp);
			break;
		}
	}
	ASSERT(ipGSActiveView != NULL);
	//��ȡ������ͼ�ؼ�
	CComQIPtr<IPageLayoutControl> ipPageLayoutControl = ipGSActiveView->GetControl();
	CString strValue = _T("");
	//���ò�����ͼ����(���ڡ�����...)
	if(strMapConfig.Lookup(_T("MODELPATH"),strValue))
	{
		//��ȡMXD�ĵ�����Ŀ¼
		TCHAR szAppFullPath[MAX_PATH];
		DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(CHAR);
		GetModuleFileName(NULL,szAppFullPath,dBufferSize);
		CString strAppFolder(szAppFullPath);
		strAppFolder = strAppFolder.Mid(0,strAppFolder.ReverseFind('\\'));
		CString strMXDFilePath = strAppFolder + _T("\\data\\template\\");
		strMXDFilePath += strValue;
		//_bstr_t strDocumentPath = strValue;
		_bstr_t strDocumentPath = strMXDFilePath;
		//edit by lh in 2016.09.21 begin
		//ʹ��arcgis�ӿڼ����ĵ�������ʹ��AE��ܣ�
		//��Ҫ����Ϊʹ��AE����ظ�����MXD�ĵ��ᱻ�Ż������ظ�����ʱ���ɹ�
		/*VARIANT_BOOL bSecceed = m_ipGisqFramework->OpenMapDocument(strDocumentPath);
		if (bSecceed == VARIANT_FALSE)return;*/
		VARIANT_BOOL bSecceed;
		ipPageLayoutControl->CheckMxFile(strDocumentPath,&bSecceed);
		if(bSecceed == VARIANT_FALSE)return;
		HRESULT hr = ipPageLayoutControl->LoadMxFile(strDocumentPath);
		if (FAILED(hr))return;
		//edit by lh end
		strValue = _T("");
	}
	POSITION pItem = strMapConfig.GetStartPosition();
	while(pItem != NULL)
	{
		CString strItemName;
		CString strItemValue;
		CComPtr<IElement> ipElement = NULL;
		strMapConfig.GetNextAssoc(pItem,strItemName,strItemValue);
		ipPageLayoutControl->FindElementByName(strItemName.AllocSysString(),1,&ipElement);
		if (ipElement != NULL)
		{
			CComBSTR strText;
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			strText = strItemValue;
			ipTextElement->put_Text(strText);
		}
	}
	//��ȡ������ͼ��������Ϣ
	
	CComQIPtr<IFeatureWorkspace> ipGHFeatureWorkspace = ipGHWorkspace;
	CComQIPtr<IGraphicsContainer> ipGraphicsContainer;
	ipPageLayoutControl->get_GraphicsContainer(&ipGraphicsContainer);
	ipGraphicsContainer->Reset();
	CComQIPtr<IElement> ipElement;
	ipGraphicsContainer->Next(&ipElement);
	//����Element
	while(ipElement != NULL)
	{
		CComQIPtr<IEnvelope> ipEnvelope;
		CComQIPtr<IMapFrame> ipMapFrame = ipElement;
		if (ipMapFrame != NULL)
		{
			CComQIPtr<IMap> ipMap;
			ipMapFrame->get_Map(&ipMap);
			CComQIPtr<IEnumLayer> ipEnumLayer;
			CComQIPtr<IUID> uid;
			uid.CoCreateInstance(CLSID_UID);
			uid->put_Value(_variant_t(_T("{40A9E885-5533-11d0-98BE-00805F7CED21}")));
			ipMap->get_Layers(uid,TRUE,&ipEnumLayer);
			ipEnumLayer->Reset();
			CComQIPtr<ILayer> ipLayer;
			ipEnumLayer->Next(&ipLayer);
			//����ͼ��
			while (ipLayer != NULL)
			{
				CComQIPtr<IFeatureClass> ipFeatureClass;
				CComQIPtr<IFeatureLayer> ipFeatureLayer = ipLayer;
				CComQIPtr<IDataLayer> ipDataLayer = ipFeatureLayer;
				CComQIPtr<IName> ipName;
				ipDataLayer->get_DataSourceName(&ipName);
				CComQIPtr<IDatasetName> ipDatasetName = ipName;
				CComBSTR bstrName;
				ipDatasetName->get_Name(&bstrName);
				CString strName(bstrName);
				if (strName.Find(_T(".")) >= 0)
				{
					strName = strName.Right(strName.GetLength() - strName.Find(_T(".")) - 1);
				}
				ipGHFeatureWorkspace->OpenFeatureClass(_bstr_t(strName),&ipFeatureClass);
				//ע��ipFeatureClassһ�㲻ΪNULL�����ΪNULL������Ҫ���SDE���Ƿ�������
				if(ipFeatureClass == NULL)
				{
					ipLayer.Release();
					ipEnumLayer->Next(&ipLayer);
					continue;
				}
				if (strName == _T("XZQX"))
				{
					CComBSTR sMapName;
					ipMap->get_Name(&sMapName);
					CString strMapName(sMapName);
					if (strMapName.Trim() == _T("ʾ��ͼ"))//���������ŵ�������
					{
						CComQIPtr<IQueryFilter> ipQueryFilter;
						ipQueryFilter.CoCreateInstance(CLSID_QueryFilter);
						CString strTemp1 = _T("xzqdm in ('") + strXZQDM.Left(6) + _T("')");
						CComBSTR sCondition = strTemp1.AllocSysString();
						ipQueryFilter->put_WhereClause(sCondition);
						CComQIPtr<IFeatureCursor> ipFeatureCursor;
						ipFeatureClass->Search(ipQueryFilter,FALSE,&ipFeatureCursor);
						CComQIPtr<IFeature> ipFeature;
						ipFeatureCursor->NextFeature(&ipFeature);
						ipEnvelope.Release();
						ipEnvelope.CoCreateInstance(CLSID_Envelope);
						while(ipFeature != NULL)
						{
							CComQIPtr<IEnvelope> spEnv;
							CComQIPtr<IGeometry> ipGeo;
							ipFeature->get_Shape(&ipGeo);
							esriGeometryType Geotype;
							ipGeo->get_GeometryType(&Geotype);
							ipGeo->get_Envelope(&spEnv);
							ipEnvelope->Union(spEnv);
							ipFeature.Release();
							ipFeatureCursor->NextFeature(&ipFeature);
						}
					}
				}else if(strName == _T("XZQXZ"))
				{
					CComBSTR sMapName;
					ipMap->get_Name(&sMapName);
					CString strMapName(sMapName);
					if (strMapName.Trim() == _T("��ʾͼ"))//���������ŵ�������
					{
						CComQIPtr<IQueryFilter> ipQueryFilter;
						ipQueryFilter.CoCreateInstance(CLSID_QueryFilter);
						CString strTemp1 = _T("xzqdm in ('") + strXZQDM + _T("')");
						CComBSTR sCondition = strTemp1.AllocSysString();
						ipQueryFilter->put_WhereClause(sCondition);
						CComQIPtr<IFeatureCursor> ipFeatureCursor;
						ipFeatureClass->Search(ipQueryFilter,FALSE,&ipFeatureCursor);
						CComQIPtr<IFeature> ipFeature;
						ipFeatureCursor->NextFeature(&ipFeature);
						ipEnvelope.Release();
						ipEnvelope.CoCreateInstance(CLSID_Envelope);
						while (ipFeature != NULL)
						{
							CComQIPtr<IEnvelope> spEnv;
							CComQIPtr<IGeometry> ipGeo;
							ipFeature->get_Shape(&ipGeo);
							esriGeometryType Geotype;
							ipGeo->get_GeometryType(&Geotype);
							ipGeo->get_Envelope(&spEnv);
							ipEnvelope->Union(spEnv);
							ipFeature.Release();
							ipFeatureCursor->NextFeature(&ipFeature);
						}
					}
				}
				if (ipFeatureClass != NULL)
				{
					CComQIPtr<IDataset> ipDataset = ipFeatureClass;
					CComBSTR sName;
					ipDataset->get_BrowseName(&sName);
					ipDatasetName->put_Name(sName);
					CComQIPtr<IName> ipName1;
					ipDataset->get_FullName(&ipName1);
					ipDatasetName = ipName1;
					VARIANT_BOOL bSucceed;//0 == FALSE, -1 == TRUE
					ipDataLayer->Connect((CComQIPtr<IName>)ipDatasetName,&bSucceed);

					ipFeatureLayer->putref_FeatureClass(ipFeatureClass);
					ipFeatureLayer->put_Visible(VARIANT_TRUE);
					CComQIPtr<IFeatureLayerDefinition> ipFeatureLayerDefinition = ipFeatureLayer;
					CComBSTR sSQL;
					ipFeatureLayerDefinition->get_DefinitionExpression(&sSQL);
					CString strSQL(sSQL);
					if (!strSQL.IsEmpty())
					{
						strSQL.Replace(_T("left"),_T("substr"));
						strSQL.Replace(_T("["),_T(""));
						strSQL.Replace(_T("]"),_T(""));
// 						if (strSQL.Find(_T(",1,")) == -1)
// 						{
// 							strSQL.Replace(_T(","),_T(",1,"));
// 						}
// 						if (strXZQDM.GetLength() < 9)
// 						{
// 							strSQL.Replace(_T(",9"),_T(",6"));
// 						}
// 						else
// 						{
// 							strSQL.Replace(_T(",6"),_T(",9"));
// 						}
						//add by lcc 2016-11-14 �޸���ͼ���ݹ���BUG
						if (strSQL.Find(_T("XZDM")) >= 0 || strSQL.Find(_T("XZQDM")) >= 0 || strSQL.Find(_T("xzdm")) >= 0 || strSQL.Find(_T("xzqdm")) >= 0)
						{
							int nDMStart =  strSQL.Find(_T("'"));
							int nDMEnd = strSQL.ReverseFind('\'');
							CString DM = strSQL.Mid(nDMStart + 1,nDMEnd - nDMStart -1);
							if (DM.GetLength() < 9)
							{
								strSQL.Replace(DM,strXZQDM.Left(6));
							}else
							{
								strSQL.Replace(DM,strXZQDM);
							}
						}
						//end 2016-11-14
						sSQL = strSQL.AllocSysString();
						ipFeatureLayerDefinition->put_DefinitionExpression(sSQL);
					}
				}
				ipLayer.Release();
				ipEnumLayer->Next(&ipLayer);
			}

			CComQIPtr<IActiveView> ipActiveView = ipMap;
			if (ipEnvelope != NULL)
			{
				ipActiveView->put_Extent(ipEnvelope);
				

				CComQIPtr<IArea> ipArea = ipEnvelope;
				CComQIPtr<IPoint> ipPoint;
				ipArea->get_Centroid(&ipPoint);
				CComQIPtr<IEnvelope> ipEnvelope1;
				ipActiveView->get_Extent(&ipEnvelope1);
				ipEnvelope1->CenterAt(ipPoint);
				//ipActiveView->put_FullExtent(ipEnvelope1);
			}
 			ipPageLayoutControl->ZoomToWholePage();
			//���������ˢ�µĻ�����ÿ��Map����ˢ��һ�Σ����Իῴ����˸��
			//��˽�ˢ�·ŵ�����Map�������֮��������ֻ��ˢ��һ����ͼ
			/*CComQIPtr<IScreenDisplay> ipScreenDisplay;
			ipActiveView->get_ScreenDisplay(&ipScreenDisplay);
			ipScreenDisplay->UpdateWindow();
			ipActiveView->Refresh();*/			
		}

		/*CComQIPtr<IElement> ipElement1;
		ipGraphicsContainer->Next(&ipElement1);
		ipElement = ipElement1;	*/	
		ipElement.Release();
		ipGraphicsContainer->Next(&ipElement);
	}
	//�����е���ͼˢ�¶���������һ��ˢ�£�������˸
	ipPageLayoutControl->Refresh(esriViewAll);

}

void CExportThemeMap::ExportJBTMap(CComQIPtr<IWorkspace> ipGHWorkspace,
					 CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig)
{
	//��ʼ��������ͼ������Ϣ

	double doScale = 1;
	BOOL bPortrait = FALSE;
	int iSideEdge = 0;
	int iTopEdge = 0;
	if(strMapConfig.IsEmpty())return;
	//��ȡ������ͼ
	CComQIPtr<GisqLib::IGSActiveView> ipGSActiveView;
	for (int i = 0;i < m_ipGisqFramework->GetViewCount();i++)
	{
		ipGSActiveView =m_ipGisqFramework->GetViewItem(i);
		if (ipGSActiveView->GetViewType() == (short)VT_PageLayoutView)
		{
			break;
		}
	}
	ASSERT(ipGSActiveView != NULL);
	//��ȡ������ͼ�ؼ�
	CComQIPtr<IPageLayoutControl> ipPageLayoutControl = ipGSActiveView->GetControl();
	CString strValue = _T("");
	//���ò�����ͼ����(���ڡ�����...)
	POSITION pItem = strMapConfig.GetStartPosition();
	while(pItem != NULL)
	{
		CString strItemName;
		CString strItemValue;
		CComPtr<IElement> ipElement = NULL;
		strMapConfig.GetNextAssoc(pItem,strItemName,strItemValue);
		ipPageLayoutControl->FindElementByName(strItemName.AllocSysString(),1,&ipElement);
		if (ipElement != NULL)
		{
			CComBSTR strText;
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			strText = strItemValue;
			ipTextElement->put_Text(strText);
		}
	}
	//��ȡ������ͼ��������Ϣ
	
	CComQIPtr<IFeatureWorkspace> ipGHFeatureWorkspace = ipGHWorkspace;
	CComQIPtr<IGraphicsContainer> ipGraphicsContainer;
	ipPageLayoutControl->get_GraphicsContainer(&ipGraphicsContainer);
	ipGraphicsContainer->Reset();
	CComQIPtr<IElement> ipElement;
	ipGraphicsContainer->Next(&ipElement);
	//����Element
	while(ipElement != NULL)
	{
		CComQIPtr<IEnvelope> ipEnvelope;
		CComQIPtr<IMapFrame> ipMapFrame = ipElement;
		if (ipMapFrame != NULL)
		{
			CComQIPtr<IMap> ipMap;
			ipMapFrame->get_Map(&ipMap);
			CComQIPtr<IEnumLayer> ipEnumLayer;
			CComQIPtr<IUID> uid;
			uid.CoCreateInstance(CLSID_UID);
			uid->put_Value(_variant_t(_T("{40A9E885-5533-11d0-98BE-00805F7CED21}")));
			ipMap->get_Layers(uid,TRUE,&ipEnumLayer);
			ipEnumLayer->Reset();
			CComQIPtr<ILayer> ipLayer;
			ipEnumLayer->Next(&ipLayer);
			//����ͼ��
			while (ipLayer != NULL)
			{
				CComQIPtr<IFeatureClass> ipFeatureClass;
				CComQIPtr<IFeatureLayer> ipFeatureLayer = ipLayer;
				CComQIPtr<IDataLayer> ipDataLayer = ipFeatureLayer;
				CComQIPtr<IName> ipName;
				ipDataLayer->get_DataSourceName(&ipName);
				CComQIPtr<IDatasetName> ipDatasetName = ipName;
				CComBSTR bstrName;
				ipDatasetName->get_Name(&bstrName);
				CString strName(bstrName);
				if (strName.Find(_T(".")) >= 0)
				{
					strName = strName.Right(strName.GetLength() - strName.Find(_T(".")) - 1);
				}
				ipGHFeatureWorkspace->OpenFeatureClass(_bstr_t(strName),&ipFeatureClass);
				//ע��ipFeatureClassһ�㲻ΪNULL�����ΪNULL������Ҫ���SDE���Ƿ�������
				if(ipFeatureClass == NULL)
				{
					ipLayer.Release();
					ipEnumLayer->Next(&ipLayer);
					continue;
				}
				if (ipFeatureClass != NULL)
				{
					CComQIPtr<IDataset> ipDataset = ipFeatureClass;
					CComBSTR sName;
					ipDataset->get_BrowseName(&sName);
					ipDatasetName->put_Name(sName);
					CComQIPtr<IName> ipName1;
					ipDataset->get_FullName(&ipName1);
					ipDatasetName = ipName1;
					VARIANT_BOOL bSucceed;//0 == FALSE, -1 == TRUE
					ipDataLayer->Connect((CComQIPtr<IName>)ipDatasetName,&bSucceed);

					ipFeatureLayer->putref_FeatureClass(ipFeatureClass);
					//ipFeatureLayer->put_Visible(VARIANT_TRUE);
				}
				ipLayer.Release();
				ipEnumLayer->Next(&ipLayer);
			}
		}
		ipElement.Release();
		ipGraphicsContainer->Next(&ipElement);
	}
	//�����е���ͼˢ�¶���������һ��ˢ�£�������˸
	ipPageLayoutControl->ZoomToWholePage();
	SetFullExtent((CComQIPtr<IEnvelope>)(IUnknown*)NULL,TRUE);
	ipPageLayoutControl->Refresh(esriViewAll);

}

void CExportThemeMap::ExportThemeMapTDZZ(CComQIPtr<IWorkspace> ipGHWorkspace,
					 CString strXZQDM,
					 CComQIPtr<ILayer> spZDQYLayer,
					 CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig)
{
	//��ʼ��������ͼ������Ϣ

	double doScale = 1;
	BOOL bPortrait = FALSE;
	int iSideEdge = 0;
	int iTopEdge = 0;
	if(strXZQDM.IsEmpty() || strMapConfig.IsEmpty())return;
	//���ò�����ͼΪ��ǰ���ͼ
	CComQIPtr<GisqLib::IGSActiveView> ipGSActiveView;
	for (int i = 0;i < m_ipGisqFramework->GetViewCount();i++)
	{
		ipGSActiveView =m_ipGisqFramework->GetViewItem(i);
		if (ipGSActiveView->GetViewType() == (short)VT_PageLayoutView)
		{
			GisqLib::IGSActiveView* ipTemp = ipGSActiveView;
			m_ipGisqFramework->SetActiveView(ipTemp);
			break;
		}
	}
	ASSERT(ipGSActiveView != NULL);
	//��ȡ������ͼ�ؼ�
	CComQIPtr<IPageLayoutControl> ipPageLayoutControl = ipGSActiveView->GetControl();
	CString strValue = _T("");
	//���ò�����ͼ����(���ڡ�����...)
	if(strMapConfig.Lookup(_T("MODELPATH"),strValue))
	{
		//��ȡMXD�ĵ�����Ŀ¼
		TCHAR szAppFullPath[MAX_PATH];
		DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(CHAR);
		GetModuleFileName(NULL,szAppFullPath,dBufferSize);
		CString strAppFolder(szAppFullPath);
		strAppFolder = strAppFolder.Mid(0,strAppFolder.ReverseFind('\\'));
		CString strMXDFilePath = strAppFolder + _T("\\data\\template\\");
		strMXDFilePath += strValue;
		//_bstr_t strDocumentPath = strValue;
		_bstr_t strDocumentPath = strMXDFilePath;
		//edit by lh in 2016.09.21 begin
		//ʹ��arcgis�ӿڼ����ĵ�������ʹ��AE��ܣ�
		//��Ҫ����Ϊʹ��AE����ظ�����MXD�ĵ��ᱻ�Ż������ظ�����ʱ���ɹ�
		/*VARIANT_BOOL bSecceed = m_ipGisqFramework->OpenMapDocument(strDocumentPath);
		if (bSecceed == VARIANT_FALSE)return;*/
		VARIANT_BOOL bSecceed;
		ipPageLayoutControl->CheckMxFile(strDocumentPath,&bSecceed);
		if(bSecceed == VARIANT_FALSE)return;
		HRESULT hr = ipPageLayoutControl->LoadMxFile(strDocumentPath);
		if (FAILED(hr))return;
		//edit by lh end
		strValue = _T("");
	}
	POSITION pItem = strMapConfig.GetStartPosition();
	while(pItem != NULL)
	{
		CString strItemName;
		CString strItemValue;
		CComPtr<IElement> ipElement = NULL;
		strMapConfig.GetNextAssoc(pItem,strItemName,strItemValue);
		ipPageLayoutControl->FindElementByName(strItemName.AllocSysString(),1,&ipElement);
		if (ipElement != NULL)
		{
			CComBSTR strText;
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			strText = strItemValue;
			ipTextElement->put_Text(strText);
		}
	}
	//��ȡ������ͼ��������Ϣ
	
	CComQIPtr<IFeatureWorkspace> ipGHFeatureWorkspace = ipGHWorkspace;
	CComQIPtr<IGraphicsContainer> ipGraphicsContainer;
	ipPageLayoutControl->get_GraphicsContainer(&ipGraphicsContainer);
	ipGraphicsContainer->Reset();
	CComQIPtr<IElement> ipElement;
	ipGraphicsContainer->Next(&ipElement);
	//����Element
	while(ipElement != NULL)
	{
		CComQIPtr<IEnvelope> ipEnvelope;
		CComQIPtr<IMapFrame> ipMapFrame = ipElement;
		if (ipMapFrame != NULL)
		{
			CComQIPtr<IMap> ipMap;
			ipMapFrame->get_Map(&ipMap);
			CComQIPtr<IEnumLayer> ipEnumLayer;
			CComQIPtr<IUID> uid;
			uid.CoCreateInstance(CLSID_UID);
			uid->put_Value(_variant_t(_T("{40A9E885-5533-11d0-98BE-00805F7CED21}")));
			ipMap->get_Layers(uid,TRUE,&ipEnumLayer);
			ipEnumLayer->Reset();
			CComQIPtr<ILayer> ipLayer;
			ipEnumLayer->Next(&ipLayer);
			//����ͼ��
			while (ipLayer != NULL)
			{
				CComQIPtr<IFeatureClass> ipFeatureClass;
				CComQIPtr<IFeatureLayer> ipFeatureLayer = ipLayer;
				CComQIPtr<IDataLayer> ipDataLayer = ipFeatureLayer;
				CComQIPtr<IName> ipName;
				ipDataLayer->get_DataSourceName(&ipName);
				CComQIPtr<IDatasetName> ipDatasetName = ipName;
				CComBSTR bstrName;
				ipDatasetName->get_Name(&bstrName);
				CString strName(bstrName);
				if (strName.Find(_T(".")) >= 0)
				{
					strName = strName.Right(strName.GetLength() - strName.Find(_T(".")) - 1);
				}
				//�����ص�����ͼ��
				if (strName.Find(_T("TDZZZDQY")) >= 0)
				{
					CComQIPtr<IFeatureLayer> spFtZDQYLayer = spZDQYLayer;
					spFtZDQYLayer->get_FeatureClass(&ipFeatureClass);
					ipFeatureLayer->putref_FeatureClass(ipFeatureClass);
				}else
				{
					ipGHFeatureWorkspace->OpenFeatureClass(_bstr_t(strName),&ipFeatureClass);
				}
				//ע��ipFeatureClassһ�㲻ΪNULL�����ΪNULL������Ҫ���SDE���Ƿ�������
				if(ipFeatureClass == NULL)
				{
					ipLayer.Release();
					ipEnumLayer->Next(&ipLayer);
					continue;
				}
				if (strName == _T("XZQX"))
				{
					CComBSTR sMapName;
					ipMap->get_Name(&sMapName);
					CString strMapName(sMapName);
					if (strMapName.Trim() == _T("ʾ��ͼ"))//���������ŵ�������
					{
						CComQIPtr<IQueryFilter> ipQueryFilter;
						ipQueryFilter.CoCreateInstance(CLSID_QueryFilter);
						CString strTemp1 = _T("xzqdm in ('") + strXZQDM.Left(6) + _T("')");
						CComBSTR sCondition = strTemp1.AllocSysString();
						ipQueryFilter->put_WhereClause(sCondition);
						CComQIPtr<IFeatureCursor> ipFeatureCursor;
						ipFeatureClass->Search(ipQueryFilter,FALSE,&ipFeatureCursor);
						CComQIPtr<IFeature> ipFeature;
						ipFeatureCursor->NextFeature(&ipFeature);
						ipEnvelope.Release();
						ipEnvelope.CoCreateInstance(CLSID_Envelope);
						while(ipFeature != NULL)
						{
							CComQIPtr<IEnvelope> spEnv;
							CComQIPtr<IGeometry> ipGeo;
							ipFeature->get_Shape(&ipGeo);
							esriGeometryType Geotype;
							ipGeo->get_GeometryType(&Geotype);
							ipGeo->get_Envelope(&spEnv);
							ipEnvelope->Union(spEnv);
							ipFeature.Release();
							ipFeatureCursor->NextFeature(&ipFeature);
						}
					}
				}else if(strName == _T("XZQXZ"))
				{
					CComBSTR sMapName;
					ipMap->get_Name(&sMapName);
					CString strMapName(sMapName);
					if (strMapName.Trim() == _T("��ʾͼ"))//���������ŵ�������
					{
						CComQIPtr<IQueryFilter> ipQueryFilter;
						ipQueryFilter.CoCreateInstance(CLSID_QueryFilter);
						CString strTemp1 = _T("xzqdm in ('") + strXZQDM + _T("')");
						CComBSTR sCondition = strTemp1.AllocSysString();
						ipQueryFilter->put_WhereClause(sCondition);
						CComQIPtr<IFeatureCursor> ipFeatureCursor;
						ipFeatureClass->Search(ipQueryFilter,FALSE,&ipFeatureCursor);
						CComQIPtr<IFeature> ipFeature;
						ipFeatureCursor->NextFeature(&ipFeature);
						ipEnvelope.Release();
						ipEnvelope.CoCreateInstance(CLSID_Envelope);
						while (ipFeature != NULL)
						{
							CComQIPtr<IEnvelope> spEnv;
							CComQIPtr<IGeometry> ipGeo;
							ipFeature->get_Shape(&ipGeo);
							esriGeometryType Geotype;
							ipGeo->get_GeometryType(&Geotype);
							ipGeo->get_Envelope(&spEnv);
							ipEnvelope->Union(spEnv);
							ipFeature.Release();
							ipFeatureCursor->NextFeature(&ipFeature);
						}
					}
				}
				if (ipFeatureClass != NULL)
				{
					esriGeometryType LayerType;
					ipFeatureClass->get_ShapeType(&LayerType);
					long nIndexXZQDM = -1;
					long nIndexXZDM = -1;
					ipFeatureClass->FindField(_T("XZQDM"),&nIndexXZQDM);
					ipFeatureClass->FindField(_T("XZDM"),&nIndexXZDM);
					if ((nIndexXZQDM < 0 /*|| nIndexXZDM < 0*/) && LayerType == esriGeometryPolygon)
					{
						CComQIPtr<IFeatureLayerDefinition> ipFeatureLayerDefinition = ipFeatureLayer;
						ipFeatureLayerDefinition->put_DefinitionExpression(_T(""));
						ipLayer.Release();
						ipEnumLayer->Next(&ipLayer);						
						continue;
					}
					CComQIPtr<IDataset> ipDataset = ipFeatureClass;
					CComBSTR sName;
					ipDataset->get_BrowseName(&sName);
					ipDatasetName->put_Name(sName);
					CComQIPtr<IName> ipName1;
					ipDataset->get_FullName(&ipName1);
					ipDatasetName = ipName1;
					VARIANT_BOOL bSucceed;//0 == FALSE, -1 == TRUE
					ipDataLayer->Connect((CComQIPtr<IName>)ipDatasetName,&bSucceed);

					ipFeatureLayer->putref_FeatureClass(ipFeatureClass);
					ipFeatureLayer->put_Visible(VARIANT_TRUE);
					CComQIPtr<IFeatureLayerDefinition> ipFeatureLayerDefinition = ipFeatureLayer;
					CComBSTR sSQL;
					ipFeatureLayerDefinition->get_DefinitionExpression(&sSQL);
					CString strSQL(sSQL);
					if (!strSQL.IsEmpty())
					{
						strSQL.Replace(_T("left"),_T("substr"));
						strSQL.Replace(_T("["),_T(""));
						strSQL.Replace(_T("]"),_T(""));
						//add by lcc 2016-11-14 �޸���ͼ���ݹ���BUG
						if (strSQL.Find(_T("XZDM")) >= 0 || strSQL.Find(_T("XZQDM")) >= 0 || strSQL.Find(_T("xzdm")) >= 0 || strSQL.Find(_T("xzqdm")) >= 0)
						{
							int nDMStart =  strSQL.Find(_T("'"));
							int nDMEnd = strSQL.ReverseFind('\'');
							CString DM = strSQL.Mid(nDMStart + 1,nDMEnd - nDMStart -1);
							if (DM.GetLength() < 9)
							{
								strSQL.Replace(DM,strXZQDM.Left(6));
							}else
							{
								strSQL.Replace(DM,strXZQDM);
							}
						}
						//end 2016-11-14
						sSQL = strSQL.AllocSysString();
						ipFeatureLayerDefinition->put_DefinitionExpression(sSQL);
					}
				}
				ipLayer.Release();
				ipEnumLayer->Next(&ipLayer);
			}

			CComQIPtr<IActiveView> ipActiveView = ipMap;
			if (ipEnvelope != NULL)
			{
				ipActiveView->put_Extent(ipEnvelope);

				CComQIPtr<IArea> ipArea = ipEnvelope;
				CComQIPtr<IPoint> ipPoint;
				ipArea->get_Centroid(&ipPoint);
				CComQIPtr<IEnvelope> ipEnvelope1;
				ipActiveView->get_Extent(&ipEnvelope1);
				ipEnvelope1->CenterAt(ipPoint);
			}
 			ipPageLayoutControl->ZoomToWholePage();
			//���������ˢ�µĻ�����ÿ��Map����ˢ��һ�Σ����Իῴ����˸��
			//��˽�ˢ�·ŵ�����Map�������֮��������ֻ��ˢ��һ����ͼ
			/*CComQIPtr<IScreenDisplay> ipScreenDisplay;
			ipActiveView->get_ScreenDisplay(&ipScreenDisplay);
			ipScreenDisplay->UpdateWindow();
			ipActiveView->Refresh();*/			
		}

		/*CComQIPtr<IElement> ipElement1;
		ipGraphicsContainer->Next(&ipElement1);
		ipElement = ipElement1;	*/	
		ipElement.Release();
		ipGraphicsContainer->Next(&ipElement);
	}
	//�����е���ͼˢ�¶���������һ��ˢ�£�������˸
	ipPageLayoutControl->Refresh(esriViewAll);

}

void CExportThemeMap::ExportThemeMap5(CComQIPtr<IWorkspace> ipGHWorkspace,
					 CString strXZQDM,
					 CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig,
					 BOOL bIsZXCQ,
					 BOOL bIsLockScale)
{
	double    nScale = 0;
	CComQIPtr<ISpatialReference> spSpatial = (IUnknown*)NULL;
	HRESULT hr;
	
	if(strXZQDM.IsEmpty() || strMapConfig.IsEmpty())return;
	//���ò�����ͼΪ��ǰ���ͼ
	CComQIPtr<GisqLib::IGSActiveView> ipGSActiveView;
	for (int i = 0;i < m_ipGisqFramework->GetViewCount();i++)
	{
		ipGSActiveView =m_ipGisqFramework->GetViewItem(i);
		if (ipGSActiveView->GetViewType() == (short)VT_PageLayoutView)
		{
			GisqLib::IGSActiveView* ipTemp = ipGSActiveView;
			m_ipGisqFramework->SetActiveView(ipTemp);
			break;
		}
	}
	ASSERT(ipGSActiveView != NULL);
	//��ȡ������ͼ�ؼ�����ȡֽ�Ŷ���
	CComQIPtr<IPageLayoutControl> ipPageLayoutControl = ipGSActiveView->GetControl();	
	CString strValue = _T("");
	//���ò�����ͼ����(���ڡ�����...)
	if(strMapConfig.Lookup(_T("MODELPATH"),strValue))
	{
		//��ȡMXD�ĵ�����Ŀ¼
		TCHAR szAppFullPath[MAX_PATH];
		DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(CHAR);
		GetModuleFileName(NULL,szAppFullPath,dBufferSize);
		CString strAppFolder(szAppFullPath);
		strAppFolder = strAppFolder.Mid(0,strAppFolder.ReverseFind('\\'));
		CString strMXDFilePath = strAppFolder + _T("\\data\\template\\");
		strMXDFilePath += strValue;
		_bstr_t strDocumentPath = strMXDFilePath;
		//ʹ��arcgis�ӿڼ����ĵ�������ʹ��AE��ܣ�
		//��Ҫ����Ϊʹ��AE����ظ�����MXD�ĵ��ᱻ�Ż������ظ�����ʱ���ɹ�
		VARIANT_BOOL bSecceed;
		ipPageLayoutControl->CheckMxFile(strDocumentPath,&bSecceed);
		if(bSecceed == VARIANT_FALSE)return;
		HRESULT hr = ipPageLayoutControl->LoadMxFile(strDocumentPath);
		if (FAILED(hr))return;
		strValue = _T("");
	}
	POSITION pItem = strMapConfig.GetStartPosition();
	while(pItem != NULL)
	{
		CString strItemName;
		CString strItemValue;
		CComPtr<IElement> ipElement = NULL;
		strMapConfig.GetNextAssoc(pItem,strItemName,strItemValue);
		ipPageLayoutControl->FindElementByName(strItemName.AllocSysString(),1,&ipElement);
		if (ipElement != NULL)
		{
			CComBSTR strText;
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			strText = strItemValue;
			ipTextElement->put_Text(strText);
		}
	}

	CComQIPtr<IFeatureWorkspace> ipGHFeatureWorkspace = ipGHWorkspace;
	CComQIPtr<IGraphicsContainer> ipGraphicsContainer;
	ipPageLayoutControl->get_GraphicsContainer(&ipGraphicsContainer);
	ipGraphicsContainer->Reset();
	CComQIPtr<IElement> ipElement;
	ipGraphicsContainer->Next(&ipElement);
	//����Element

	//����һ�����ڱ���������������Ķ���
	CComQIPtr<IFeatureLayer> spXZQXZFtLayer;
	while(ipElement != NULL)
	{
		CComQIPtr<IEnvelope> ipEnvelope;
		CComQIPtr<IMapFrame> ipMapFrame = ipElement;
		if (ipMapFrame != NULL)
		{
			CComQIPtr<IMap> ipMap;
			ipMapFrame->get_Map(&ipMap);
			CComQIPtr<IEnumLayer> ipEnumLayer;
			CComQIPtr<IUID> uid;
			uid.CoCreateInstance(CLSID_UID);
			uid->put_Value(_variant_t(_T("{40A9E885-5533-11d0-98BE-00805F7CED21}")));
			ipMap->get_Layers(uid,TRUE,&ipEnumLayer);
			ipEnumLayer->Reset();
			CComQIPtr<ILayer> ipLayer;
			ipEnumLayer->Next(&ipLayer);
			//����ͼ��
			while (ipLayer != NULL)
			{
				CComQIPtr<IFeatureClass> ipFeatureClass;
				CComQIPtr<IFeatureLayer> ipFeatureLayer = ipLayer;
				CComQIPtr<IDataLayer> ipDataLayer = ipFeatureLayer;
				CComQIPtr<IName> ipName;
				ipDataLayer->get_DataSourceName(&ipName);
				CComQIPtr<IDatasetName> ipDatasetName = ipName;
				CComBSTR bstrName;
				ipDatasetName->get_Name(&bstrName);
				CString strName(bstrName);
				if (strName.Find(_T(".")) >= 0)
				{
					strName = strName.Right(strName.GetLength() - strName.Find(_T(".")) - 1);
				}
				ipGHFeatureWorkspace->OpenFeatureClass(_bstr_t(strName),&ipFeatureClass);
				//ע��ipFeatureClassһ�㲻ΪNULL�����ΪNULL������Ҫ���SDE���Ƿ�������
				if(ipFeatureClass == NULL)
				{
					ipLayer.Release();
					ipEnumLayer->Next(&ipLayer);
					continue;
				}				
				if (strName == _T("XZQX"))
				{
					CComBSTR sMapName;
					ipMap->get_Name(&sMapName);
					CString strMapName(sMapName);
					if (strMapName.Trim() == _T("ʾ��ͼ"))//���������ŵ�������
					{
						CComQIPtr<IQueryFilter> ipQueryFilter;
						ipQueryFilter.CoCreateInstance(CLSID_QueryFilter);
						CString strTemp1 = _T("xzqdm in ('") + strXZQDM.Left(6) + _T("')");
						CComBSTR sCondition = strTemp1.AllocSysString();
						ipQueryFilter->put_WhereClause(sCondition);
						CComQIPtr<IFeatureCursor> ipFeatureCursor;
						ipFeatureClass->Search(ipQueryFilter,FALSE,&ipFeatureCursor);
						CComQIPtr<IFeature> ipFeature;
						ipFeatureCursor->NextFeature(&ipFeature);
						ipEnvelope.Release();
						ipEnvelope.CoCreateInstance(CLSID_Envelope);
						while(ipFeature != NULL)
						{
							CComQIPtr<IEnvelope> spEnv;
							CComQIPtr<IGeometry> ipGeo;
							ipFeature->get_Shape(&ipGeo);
							esriGeometryType Geotype;
							ipGeo->get_GeometryType(&Geotype);
							ipGeo->get_Envelope(&spEnv);
							ipEnvelope->Union(spEnv);
							ipFeature.Release();
							ipFeatureCursor->NextFeature(&ipFeature);
						}
					}
					CComQIPtr<IActiveView> spActiveView = ipMap;
					spActiveView->put_Extent(ipEnvelope);
					//��ȡʾ��ͼͼ���С
					//spSYTElement = ipElement;
 				}else if(strName == _T("XZQXZ"))
 				{
					CComBSTR sMapName;
					ipMap->get_Name(&sMapName);
					CString strMapName(sMapName);
					if (strMapName.Trim() == _T("��ʾͼ"))//���������ŵ�������
					{
						spXZQXZFtLayer = ipFeatureLayer;
					}
				}
				if (ipFeatureClass != NULL)
				{
					if (spSpatial == NULL)
					{
						CComQIPtr<IGeoDataset> spGeoDataset = ipFeatureClass;
						spGeoDataset->get_SpatialReference(&spSpatial);
					}
					CComQIPtr<IDataset> ipDataset = ipFeatureClass;
					CComBSTR sName;
					ipDataset->get_BrowseName(&sName);
					ipDatasetName->put_Name(sName);
					CComQIPtr<IName> ipName1;
					ipDataset->get_FullName(&ipName1);
					ipDatasetName = ipName1;
					VARIANT_BOOL bSucceed;//0 == FALSE, -1 == TRUE
					ipDataLayer->Connect((CComQIPtr<IName>)ipDatasetName,&bSucceed);

					ipFeatureLayer->putref_FeatureClass(ipFeatureClass);
					//ipFeatureLayer->put_Visible(VARIANT_TRUE);
					CComQIPtr<IFeatureLayerDefinition> ipFeatureLayerDefinition = ipFeatureLayer;
					CComBSTR sSQL;
					ipFeatureLayerDefinition->get_DefinitionExpression(&sSQL);
					CString strSQL(sSQL);
					if (!strSQL.IsEmpty())
					{
						strSQL.Replace(_T("left"),_T("substr"));
						strSQL.Replace(_T("["),_T(""));
						strSQL.Replace(_T("]"),_T(""));
						//add by lcc 2016-11-14 �޸���ͼ���ݹ���BUG
						if (strSQL.Find(_T("XZDM")) >= 0 || strSQL.Find(_T("XZQDM")) >= 0 || strSQL.Find(_T("xzdm")) >= 0 || strSQL.Find(_T("xzqdm")) >= 0)
						{
							int nDMStart =  strSQL.Find(_T("'"));
							int nDMEnd = strSQL.ReverseFind('\'');
							CString DM = strSQL.Mid(nDMStart + 1,nDMEnd - nDMStart -1);
							if (DM.GetLength() < 9)
							{
								strSQL.Replace(DM,strXZQDM.Left(6));
							}else
							{
								strSQL.Replace(DM,strXZQDM);
							}
						}
						//end 2016-11-14
						sSQL = strSQL.AllocSysString();
						ipFeatureLayerDefinition->put_DefinitionExpression(sSQL);
						SysFreeString(sSQL);
					}
				}
				ipLayer.Release();
				ipEnumLayer->Next(&ipLayer);
			}
		}
		ipElement.Release();
		ipGraphicsContainer->Next(&ipElement);
	}
	ipPageLayoutControl->Refresh(esriViewAll);

	//�������ƥ��֮������Ӧͼ��
	//�ȶ�ȡXML�������
	//������������
	//double dbXZScale,dbXScale,dbZXCQScale;
	CString strXZSCALE,strXScale,strZXCQScale;
	strMapConfig.Lookup(_T("XZSCALE"),strXZSCALE);
	strMapConfig.Lookup(_T("XSCALE"),strXScale);
	strMapConfig.Lookup(_T("ZXCQSCALE"),strZXCQScale);
	swscanf_s(_bstr_t(strXZSCALE),_T("%lf"),&dbXZScale);
	swscanf_s(_bstr_t(strXScale),_T("%lf"),&dbXScale);
	swscanf_s(_bstr_t(strZXCQScale),_T("%lf"),&dbZXCQScale);
	//��ֽ�ŷ�Χ����
	//double dbMaxPageWidth,dbMinPageWidth;
	CString strMaxPageWidth,strMinPageWidth;
	strMapConfig.Lookup(_T("PAGE_MAX_WIDTH"),strMaxPageWidth);
	strMapConfig.Lookup(_T("PAGE_MIN_WIDTH"),strMinPageWidth);
	swscanf_s(_bstr_t(strMaxPageWidth),_T("%lf"),&dbMaxPageWidth);
	swscanf_s(_bstr_t(strMinPageWidth),_T("%lf"),&dbMinPageWidth);
	//��ȡֽ������
	//double dbPageHead,dbPageButton,dbPage_LeftRight;
	CString strPageHead,strPageButton,strPage_LeftRight;
	strMapConfig.Lookup(_T("PAGE_HEAD"),strPageHead);
	strMapConfig.Lookup(_T("PAGE_BUTTON"),strPageButton);
	strMapConfig.Lookup(_T("PAGE_LEFT_RIGHT"),strPage_LeftRight);
	swscanf_s(_bstr_t(strPageHead),_T("%lf"),&dbPageHead);
	swscanf_s(_bstr_t(strPageButton),_T("%lf"),&dbPageButton);
	swscanf_s(_bstr_t(strPage_LeftRight),_T("%lf"),&dbPage_LeftRight);
	//��ȡͼ������
	//double dbFrameBlank;
	CString strFrameBlank;
	strMapConfig.Lookup(_T("FRAME_BLANK"),strFrameBlank);
	swscanf_s(_bstr_t(strFrameBlank),_T("%lf"),&dbFrameBlank);
	//����λ����
	dbMoveLenth = 1.0;

	//���ȸ��ݳ�ͼ��Χ����ֽ�ź�ͼ��
	CComQIPtr<IElement> spLayersElement;
	CComQIPtr<IGeometry> spEleGeo;
	CComQIPtr<IEnvelope> spLayersEnv;

	double dbLayersEleHeight = 0.0,dbLayersEleWidth = 0.0;//��¼����������mapframe��С
	double dbScale = 0.0;//��¼��ʾ����
	double dbXZQHeight = 0.0,dbXZQWidth = 0.0;//��¼������map��Χ��С
	CComQIPtr<IEnvelope> spXZQEnv;//��¼һ������µ���������Χ
	CComQIPtr<IPoint> spActiveViewCenter;//��¼��ǰ��ͼ���ĵ�

	ipPageLayoutControl->FindElementByName(_T("��ʾͼ"),1,&spLayersElement);
	if (spLayersElement == NULL) return;
	//��ȡͼ���С
	spLayersElement->get_Geometry(&spEleGeo);
	spEleGeo->get_Envelope(&spLayersEnv);
	spLayersEnv->get_Width(&dbLayersEleWidth);
	spLayersEnv->get_Height(&dbLayersEleHeight);

	CComQIPtr<IMapFrame> spMapFrame = spLayersElement;
	//��ͼ���Ľ�����
	double dbNewMapFrameMaxX,dbNewMapFrameMaxY,dbNewMapFrameMinX,dbNewMapFrameMinY;
	spLayersEnv->get_XMax(&dbNewMapFrameMaxX);
	spLayersEnv->get_XMin(&dbNewMapFrameMinX);
	spLayersEnv->get_YMax(&dbNewMapFrameMaxY);
	spLayersEnv->get_YMin(&dbNewMapFrameMinY);

	//����������
	double dbScaleChanged;
	//ֽ�ų���
	double dbPageWidth,dbPageHeight;
	//�����ͼ�򳤿�
	double dbNewFrameHeight,dbNewFrameWidth;
	//�Ƿ�����
	BOOL bIsPortrait = TRUE;
	//ͼ���Ľ����꣬�����ж��ĽǵĿհ״�С
	double dbMapMaxX,dbMapMaxY,dbMapMinX,dbMapMinY;
	//�Ƿ񽫵�ǰԪ�ط�����࣬�������ƶ�����ʱȷ���Ӽ�����
	//BOOL bIsPutLeft = TRUE;

	CComQIPtr<IMap> spMap;
	spMapFrame->get_Map(&spMap);
	spMap->putref_SpatialReference(spSpatial);//����֮ǰ��ȡ������ϵ��map������
	CComQIPtr<IActiveView> spActiveView = spMap;
	if (spMapFrame != NULL)
	{				
		//���õ�ͼ������
		if (bIsZXCQ == TRUE)
		{
			nScale = dbZXCQScale;
			spMap->put_MapScale(dbZXCQScale);
		}else if (bIsZXCQ == FALSE)
		{
			nScale = dbXZScale;
			spMap->put_MapScale(dbXZScale);
		}
		//��ȡ��ͼ��Χ��С,������ʾ��Χ��ͼ���С�ı���
		//��Ϊ֮ǰ����ƥ������ϵ�Ĳ���������ȷ������ͳһ		
		CComQIPtr<IEnvelope> spMapEnv;
		double dbMapWidth,dbMapHeight;
		spActiveView->get_Extent(&spMapEnv);
		spMapEnv->get_Width(&dbMapWidth);
		dbScale = dbLayersEleWidth/dbMapWidth;
		spMapEnv->get_Height(&dbMapHeight);
		dbScale = dbLayersEleHeight/dbMapHeight;

		//��ȡ���������µ���������Χ
		CComQIPtr<IFeatureCursor> spFtCursor;
		CComQIPtr<IFeature> spFt;
		spXZQXZFtLayer->Search(NULL,VARIANT_FALSE,&spFtCursor);
		spFtCursor->NextFeature(&spFt);
		spXZQEnv.CoCreateInstance(CLSID_Envelope);
		while(spFt != NULL)
		{
			CComQIPtr<IGeometry> spTmpGeo;
			CComQIPtr<IEnvelope> spTmpEnv;
			spFt->get_Shape(&spTmpGeo);
			spTmpGeo->get_Envelope(&spTmpEnv);
			spXZQEnv->Union(spTmpEnv);
			spFt.Release();
			spFtCursor->NextFeature(&spFt);
		}
		//��Ϊ��������ϵƥ�䣬��������ϵ��ͬ�����
		spXZQEnv->get_Width(&dbXZQWidth);
		spXZQEnv->get_Height(&dbXZQHeight);
		dbLayersEleHeight = dbXZQHeight * dbScale;
		dbLayersEleWidth = dbXZQWidth *dbScale;
		//�����ʼ��������Χ���ĵ�
		double dbXZQMaxX,dbXZQMaxY,dbXZQMinX,dbXZQMinY;
		spXZQEnv->get_XMax(&dbXZQMaxX);
		spXZQEnv->get_YMax(&dbXZQMaxY);
		spXZQEnv->get_XMin(&dbXZQMinX);
		spXZQEnv->get_YMin(&dbXZQMinY);
		spActiveViewCenter.CoCreateInstance(CLSID_Point);
		spActiveViewCenter->put_X((dbXZQMaxX + dbXZQMinX) / 2);
		spActiveViewCenter->put_Y((dbXZQMaxY + dbXZQMinY) / 2);
		//����
		dbLayersEleHeight += dbFrameBlank;
		dbLayersEleWidth += dbFrameBlank;
		dbPageHeight  = dbLayersEleHeight + dbPageHead + dbPageButton;
		dbPageWidth = dbLayersEleWidth + dbPage_LeftRight * 2;
		if (bIsLockScale == FALSE)
		{
			double dbPageMinLine = (dbPageHeight < dbPageWidth) ? dbPageHeight : dbPageWidth;
			double dbPageMaxLine = (dbPageHeight > dbPageWidth) ? dbPageHeight : dbPageWidth;
			//�ж��Ƿ񳬹�ֽ��С�޶���Χ
			//������һ���ڷ�Χ�ڣ��Ϳ��Բ���������ֽ��
			if ( (dbPageMinLine > dbMinPageWidth && dbPageMinLine < dbMaxPageWidth) || (dbPageMaxLine > dbMinPageWidth && dbPageMaxLine < dbMaxPageWidth))
			{
				//Todo...
			}else if (dbPageMinLine > dbMaxPageWidth)//С�ڻ��ߴ���ֽ�ŷ�Χ
			{
				//���ձ������ŵ�ֽ�ŷ�Χ�ڵı����� 
				double dbScaleChange = dbMaxPageWidth / dbPageMinLine;
				dbScaleChanged = nScale / dbScaleChange;
				if (dbScaleChanged < 10000)
				{
					dbScaleChanged = ceil(dbScaleChanged / 2000) * 2000;
				}else
				{
					dbScaleChanged = ceil(dbScaleChanged / 5000) * 5000;
				}
				//�Ƚϱ������Ƿ������С
				double dbMaxScale = 25000;
				double dbMinScale = 2000;
				if (dbScaleChanged > dbMaxScale)
				{
					dbScaleChanged = dbMaxScale;
				}else if (dbScaleChanged < dbMinScale)
				{
					dbScaleChanged = dbMinScale;
				}
				nScale = dbScaleChanged;
				spMap->put_MapScale(dbScaleChanged);
				ipPageLayoutControl->Refresh(esriViewAll);
				GetSizeScale(spMapFrame,&dbScale);
				GetFrameSize(spXZQXZFtLayer,dbScale,&dbNewFrameWidth,&dbNewFrameHeight);
				dbNewFrameHeight += dbFrameBlank;
				dbNewFrameWidth += dbFrameBlank;
				dbPageHeight  = dbNewFrameHeight + dbPageHead + dbPageButton;
				dbPageWidth = dbNewFrameWidth + dbPage_LeftRight * 2;
			}else if (dbPageMinLine < dbMinPageWidth)
			{
				double dbScaleChange = dbMinPageWidth / dbPageMinLine;
				dbScaleChanged = nScale / dbScaleChange;
				if (dbScaleChanged < 10000)
				{
					dbScaleChanged = floor(dbScaleChanged / 2000) * 2000;
				}else
				{
					dbScaleChanged = floor(dbScaleChanged / 5000) * 5000;
				}
				//�Ƚϱ������Ƿ������С
				double dbMaxScale = 25000;
				double dbMinScale = 2000;
				if (dbScaleChanged > dbMaxScale)
				{
					dbScaleChanged = dbMaxScale;
				}else if (dbScaleChanged < dbMinScale)
				{
					dbScaleChanged = dbMinScale;
				}
				nScale = dbScaleChanged;
				spMap->put_MapScale(dbScaleChanged);
				ipPageLayoutControl->Refresh(esriViewAll);
				GetSizeScale(spMapFrame,&dbScale);
				GetFrameSize(spXZQXZFtLayer,dbScale,&dbNewFrameWidth,&dbNewFrameHeight);
				dbNewFrameHeight += dbFrameBlank;
				dbNewFrameWidth += dbFrameBlank;
				dbPageHeight  = dbNewFrameHeight + dbPageHead + dbPageButton;
				dbPageWidth = dbNewFrameWidth + dbPage_LeftRight * 2;
			}else
			{
				//Todo...ʣ�����
			}
		}else
		{
			//Todo...���Լ�������ߣ�ʲô������
		}
		//���ݳ������������,�����޸�ֽ�ŵĺ�������Ҫ�ֶ��޸�mapframe�ĺ���

		CComQIPtr<IPage> spPage;
		ipPageLayoutControl->get_Page(&spPage);
	
		//���㳤�ڷ�Χ�ڻ��ǿ��ڷ�Χ��
		if (dbPageHeight < dbMaxPageWidth && dbPageHeight > dbMinPageWidth)
		{
			//���ڷ�Χ�ڣ��Ǻ��
			bIsPortrait = FALSE;

		}else if (dbPageWidth < dbMaxPageWidth && dbPageWidth > dbMinPageWidth)
		{
			//���ڷ�Χ�ڣ�������
			bIsPortrait = TRUE;
		}
		//if (dbPageHeight/dbPageWidth > 1)
		if (bIsPortrait == TRUE)
		{
			spPage->put_Orientation(short(1));
			spPage->put_FormID(esriPageFormCUSTOM);
			spPage->PutCustomSize(dbPageWidth,dbPageHeight);
		}else
		{
			spPage->put_Orientation(short(2));
			spPage->put_FormID(esriPageFormCUSTOM);
			spPage->PutCustomSize(dbPageWidth,dbPageHeight);
		}
		ipPageLayoutControl->Refresh(esriViewAll);
	}
		
	CComQIPtr<IEnvelope> spNewXZQEnvelop;
	spNewXZQEnvelop.CoCreateInstance(CLSID_Envelope);
	//���ݼ�����ı������¼�����������Χ��ͼ���С
	spMap->put_MapScale(dbScaleChanged);

	CComQIPtr<IFeatureCursor> spFtCursor;
	CComQIPtr<IFeature> spFt;
	spFtCursor.Release();
	if (spFt != NULL)
	{
		spFt.Release();
	}
	spXZQXZFtLayer->Search(NULL,VARIANT_FALSE,&spFtCursor);
	spFtCursor->NextFeature(&spFt);
	while(spFt != NULL)
	{
		CComQIPtr<IGeometry> spTmpGeo;
		CComQIPtr<IEnvelope> spTmpEnv;
		spFt->get_Shape(&spTmpGeo);
		spTmpGeo->get_Envelope(&spTmpEnv);
		spNewXZQEnvelop->Union(spTmpEnv);
		spFt.Release();
		spFtCursor->NextFeature(&spFt);
	}

	//�����������ҵĵ�
	//�Ȼ��㳤��Ϊֽ���С,����1.1������
	SetFullExtent(spXZQEnv,FALSE);
	double dbXZQNewHeight,dbXZQNewWidth;
	spXZQEnv->get_Height(&dbXZQNewHeight);
	spXZQEnv->get_Width(&dbXZQNewWidth);
	dbNewFrameHeight = dbXZQNewHeight * dbScale  + 5;
	dbNewFrameWidth = dbXZQNewWidth *dbScale  + 5;
	//����Ԥ��ֵ����ͼ���Ľ�����
	dbNewMapFrameMinY = (dbPageHeight - dbNewFrameHeight) / 2;
	dbNewMapFrameMaxY = dbNewMapFrameMinY + dbNewFrameHeight;
	dbNewMapFrameMinX = (dbPageWidth - dbNewFrameWidth) / 2;
	dbNewMapFrameMaxX = dbNewMapFrameMinX + dbNewFrameWidth;

	CComQIPtr<IPolygon> spPolygon;
	spPolygon.CoCreateInstance(CLSID_Polygon);
	GetPolygonByPoint(dbNewMapFrameMaxX,dbNewMapFrameMaxY,dbNewMapFrameMinX,dbNewMapFrameMinY,spPolygon);
	spLayersElement->put_Geometry(spPolygon);
	//���ͼ�����֮�������ʾ��Χ
	spActiveView->put_Extent(spXZQEnv);
	//��Ϊ���ŵ�ͼ���ƻ�����ǧ�ı�������������һ�α���
	spMap->put_MapScale(nScale);

	//HRESULT hrLegendLeft,hrLegendRight;
	HRESULT hrSYTLeft,hrSYTRight;
	//�ƶ�ʾ��ͼλ��
	MoveLegendElement(ipPageLayoutControl,spXZQXZFtLayer,spActiveViewCenter);

	//ˢ��ֽ�ź�ͼ���С��Ϣ
	spEleGeo.Release();
	spLayersEnv.Release();
	spLayersElement->get_Geometry(&spEleGeo);
	spEleGeo->get_Envelope(&spLayersEnv);
	spLayersEnv->get_Width(&dbNewFrameWidth);
	spLayersEnv->get_Height(&dbNewFrameHeight);
	CComQIPtr<IPage> spPage;
	ipPageLayoutControl->get_Page(&spPage);
	spPage->QuerySize(&dbPageWidth,&dbPageHeight);
	dbNewMapFrameMinY = (dbPageHeight - dbNewFrameHeight) / 2;
	dbNewMapFrameMaxY = dbNewMapFrameMinY + dbNewFrameHeight;
	dbNewMapFrameMinX = (dbPageWidth - dbNewFrameWidth) / 2;
	dbNewMapFrameMaxX = dbNewMapFrameMinX + dbNewFrameWidth;

	//���ڱ���ʾ��ͼԪ�صĶ���
	CComQIPtr<IElement> spSYTElement;
	ipPageLayoutControl->FindElementByName(_T("ʾ��ͼ"),1,&spSYTElement);
	//��ȡʾ��ͼ��map�еĳ���
	CComQIPtr<IGeometry> spSYTGeo;
	CComQIPtr<IEnvelope> spSYTEnvelop;
	spSYTElement->get_Geometry(&spSYTGeo);
	spSYTGeo->get_Envelope(&spSYTEnvelop);

	double dbSYTEleHeight,dbSYTEleWidth;
	spSYTEnvelop->get_Height(&dbSYTEleHeight);
	spSYTEnvelop->get_Width(&dbSYTEleWidth);
	double dbSYTMapHeight,dbSYTMapWidth;
	dbSYTMapHeight = dbSYTEleHeight / dbScale;
	dbSYTMapWidth = dbSYTEleWidth / dbScale;

	//��ȡλ�����֣�ָ����ĳ���
	CComQIPtr<IElement> spMapWZElement;//λ������
	CComQIPtr<IElement> spArrowElement;//ָ����
	ipPageLayoutControl->FindElementByName(_T("MAPWZ"),1,&spMapWZElement);
	ipPageLayoutControl->FindElementByName(_T("MAPARROW"),1,&spArrowElement);
	double dbMapWZTextXSize,dbMapWZTextYSize;//ʾ��ͼ���ֳ���
	double dbArrowWidth,dbArrowHeight;
	double dbArrowBlank = 1.0;

	//��ȡʾ��ͼ���ֳ���
	if (spMapWZElement != NULL)
	{
		GetTextElementSize(spMapWZElement,spActiveView,&dbMapWZTextXSize,&dbMapWZTextYSize);
	}
	if (spArrowElement != NULL)
	{
		CComQIPtr<IGeometry> spTmpGeo;
		CComQIPtr<IEnvelope> spTmpEnv;
		spArrowElement->get_Geometry(&spTmpGeo);
		spTmpGeo->get_Envelope(&spTmpEnv);
		spTmpEnv->get_Width(&dbArrowWidth);
		spTmpEnv->get_Height(&dbArrowHeight);
	}
	//ȷ��λ��
	double dbMapWZX,dbMapWZY;//ʾ��ͼ��������
	CComQIPtr<IPoint> spMapWZPoint;//ʾ��ͼ���ֵ�
	spMapWZPoint.CoCreateInstance(CLSID_Point);
	CComQIPtr<IPolygon> spSYTPolygon;//ʾ��ͼ����
	CComQIPtr<IPolygon> spArrowPolygon;//ָ�������
	spSYTPolygon.CoCreateInstance(CLSID_Polygon);
	spArrowPolygon.CoCreateInstance(CLSID_Polygon);

	//��ȡ���ͼ�Ľ�����
	GetNewActiveViewPoint(&dbMapMaxX,&dbMapMaxY,&dbMapMinX,&dbMapMinY,spActiveView);
	CComQIPtr<IPoint> spLeftPoint;//����
	CComQIPtr<IPoint> spRightPoint;//�Ҳ��
	spLeftPoint.CoCreateInstance(CLSID_Point);
	spRightPoint.CoCreateInstance(CLSID_Point);
	spLeftPoint->put_X(dbMapMinX);
	spLeftPoint->put_Y(dbMapMaxY);
	spRightPoint->put_X(dbMapMaxX);
	spRightPoint->put_Y(dbMapMaxY);
	//���ݿհ״�Сȷ�����ı�
	double dbLeftDistance,dbRightDistance;
	GetDistance(spXZQXZFtLayer,(CComQIPtr<IGeometry>)spLeftPoint,&dbLeftDistance);
	GetDistance(spXZQXZFtLayer,(CComQIPtr<IGeometry>)spRightPoint,&dbRightDistance);

	//���ҷֿ��������Կհ׽϶��һ��������У��
	if (dbLeftDistance > dbRightDistance)//��߿յĶ࣬�����
	{
		//�ռ��ѯ��Ҫ��4������
		double dbSYTLeftMaxX,dbSYTLeftMaxY,dbSYTLeftMinX,dbSYTLeftMinY;
		GetPolygonPoint(spActiveView,dbSYTMapWidth,dbSYTMapHeight,LeftTop,&dbSYTLeftMaxX,&dbSYTLeftMaxY,&dbSYTLeftMinX,&dbSYTLeftMinY);
		hrSYTLeft = CheckCover(spXZQXZFtLayer,dbSYTLeftMaxX,dbSYTLeftMaxY,dbSYTLeftMinX,dbSYTLeftMinY);

		while(hrSYTLeft == E_FAIL)
		{
			CComQIPtr<IPage> spPage;
			ipPageLayoutControl->get_Page(&spPage);
			double dbCenterX,dbCenterY;
			spActiveViewCenter->get_X(&dbCenterX);
			spActiveViewCenter->get_Y(&dbCenterY);
			//��������ˣ�����ֽ��,��Ӧ������ͼ����ƫ��
			if (dbPageHeight < dbPageWidth)
			{
				if (dbPageWidth + dbMoveLenth > dbMaxPageWidth)
				{
					dbNewFrameHeight += dbMoveLenth;
					dbPageHeight = dbNewFrameHeight + dbPageHead + dbPageButton;
					dbCenterY += (dbMoveLenth / 2) / dbScale;
				}else
				{
					dbNewFrameWidth += dbMoveLenth;
					dbPageWidth = dbNewFrameWidth + dbPage_LeftRight * 2;
					dbCenterX -= (dbMoveLenth / 2) / dbScale;
				}
			} 
			else
			{
				if (dbPageHeight + dbMoveLenth > dbMaxPageWidth)
				{
					dbNewFrameWidth += dbMoveLenth;
					dbPageWidth = dbNewFrameWidth + dbPage_LeftRight * 2;
					dbCenterX -= (dbMoveLenth / 2) / dbScale;
				}else
				{
					dbNewFrameHeight += dbMoveLenth;
					dbPageHeight = dbNewFrameHeight + dbPageHead + dbPageButton;		
					dbCenterY += (dbMoveLenth / 2) / dbScale;
				}

			}
			//Ӧ�õ���
			spActiveViewCenter->put_X(dbCenterX);
			spActiveViewCenter->put_Y(dbCenterY);
			spPage->PutCustomSize(dbPageWidth,dbPageHeight);
			//���¼���ͼ���Ľ�����
			dbNewMapFrameMinY = (dbPageHeight - dbNewFrameHeight) / 2;
			dbNewMapFrameMaxY = dbNewMapFrameMinY + dbNewFrameHeight;
			dbNewMapFrameMinX = (dbPageWidth - dbNewFrameWidth) / 2;
			dbNewMapFrameMaxX = dbNewMapFrameMinX + dbNewFrameWidth;

			CComQIPtr<IPolygon> spPolygon;
			spPolygon.CoCreateInstance(CLSID_Polygon);
			GetPolygonByPoint(dbNewMapFrameMaxX,dbNewMapFrameMaxY,dbNewMapFrameMinX,dbNewMapFrameMinY,spPolygon);
			spLayersElement->put_Geometry(spPolygon);
			//�ƶ���ͼ�����ĵ���
			spXZQEnv->CenterAt(spActiveViewCenter);
			spActiveView->put_Extent(spXZQEnv);
			//��Ϊ���ŵ�ͼ���ƻ�����ǧ�ı�������������һ�α���
			spMap->put_MapScale(nScale);
			//�ƶ���֮�����һ���ж�
			GetPolygonPoint(spActiveView,dbSYTMapWidth,dbSYTMapHeight,LeftTop,&dbSYTLeftMaxX,&dbSYTLeftMaxY,&dbSYTLeftMinX,&dbSYTLeftMinY);
			hrSYTLeft = CheckCover(spXZQXZFtLayer,dbSYTLeftMaxX,dbSYTLeftMaxY,dbSYTLeftMinX,dbSYTLeftMinY);
		}
		//ȷ��ʾ��ͼλ��
		GetPolygonByPoint(dbNewMapFrameMinX + dbSYTEleWidth,dbNewMapFrameMaxY,dbNewMapFrameMinX,dbNewMapFrameMaxY - dbSYTEleHeight,spSYTPolygon);
		//ȷ��ʾ��ͼ����λ��
		dbMapWZX = dbNewMapFrameMinX + (dbSYTEleWidth / 2);
		dbMapWZY = dbNewMapFrameMaxY - 0.1;
		spMapWZPoint->put_X(dbMapWZX);
		spMapWZPoint->put_Y(dbMapWZY);
		//ȷ��ָ����λ�ã����ұߵ�λ��
		GetPolygonByPoint(dbNewMapFrameMaxX - dbArrowBlank,dbNewMapFrameMaxY - dbArrowBlank,dbNewMapFrameMaxX - dbArrowWidth - dbArrowBlank,dbNewMapFrameMaxY - dbArrowHeight - dbArrowBlank,spArrowPolygon);
	}else//�ұ߿յĶ࣬���ұ� 
	{
		double dbSYTRightMaxX,dbSYTRightMaxY,dbSYTRightMinX,dbSYTRightMinY;
		GetPolygonPoint(spActiveView,dbSYTMapWidth,dbSYTMapHeight,RightTop,&dbSYTRightMaxX,&dbSYTRightMaxY,&dbSYTRightMinX,&dbSYTRightMinY);
		hrSYTRight = CheckCover(spXZQXZFtLayer,dbSYTRightMaxX,dbSYTRightMaxY,dbSYTRightMinX,dbSYTRightMinY);
		while(hrSYTRight == E_FAIL)
		{
			CComQIPtr<IPage> spPage;
			ipPageLayoutControl->get_Page(&spPage);
			double dbCenterX,dbCenterY;
			spActiveViewCenter->get_X(&dbCenterX);
			spActiveViewCenter->get_Y(&dbCenterY);
			//��������ˣ�����ֽ��,��Ӧ������ͼ����ƫ��
			if (dbPageHeight < dbPageWidth)
			{
// 				if (dbPageWidth + dbMoveLenth > dbMaxPageWidth)
// 				{
// 					dbNewFrameHeight += dbMoveLenth;
// 					dbPageHeight = dbNewFrameHeight + dbPageHead + dbPageButton;
// 					dbCenterY += (dbMoveLenth / 2) / dbScale;
// 				}else
// 				{
					dbNewFrameWidth += dbMoveLenth;
					dbPageWidth = dbNewFrameWidth + dbPage_LeftRight * 2;
					dbCenterX += (dbMoveLenth / 2) / dbScale;
//				}
			} 
			else
			{
// 				if (dbPageHeight + dbMoveLenth > dbMaxPageWidth)
// 				{
// 					dbNewFrameWidth += dbMoveLenth;
// 					dbPageWidth = dbNewFrameWidth + dbPage_LeftRight * 2;
// 					dbCenterX += (dbMoveLenth / 2) / dbScale;
// 				}else
// 				{
					dbNewFrameHeight += dbMoveLenth;
					dbPageHeight = dbNewFrameHeight + dbPageHead + dbPageButton;		
					dbCenterY += (dbMoveLenth / 2) / dbScale;
//				}

			}
			//Ӧ�õ���
			spActiveViewCenter->put_X(dbCenterX);
			spActiveViewCenter->put_Y(dbCenterY);
			spPage->PutCustomSize(dbPageWidth,dbPageHeight);
			//���¼���ͼ���Ľ�����
			dbNewMapFrameMinY = (dbPageHeight - dbNewFrameHeight) / 2;
			dbNewMapFrameMaxY = dbNewMapFrameMinY + dbNewFrameHeight;
			dbNewMapFrameMinX = (dbPageWidth - dbNewFrameWidth) / 2;
			dbNewMapFrameMaxX = dbNewMapFrameMinX + dbNewFrameWidth;

			CComQIPtr<IPolygon> spPolygon;
			spPolygon.CoCreateInstance(CLSID_Polygon);
			GetPolygonByPoint(dbNewMapFrameMaxX,dbNewMapFrameMaxY,dbNewMapFrameMinX,dbNewMapFrameMinY,spPolygon);
			spLayersElement->put_Geometry(spPolygon);
			//�ƶ���ͼ�����ĵ���
			spXZQEnv->CenterAt(spActiveViewCenter);
			spActiveView->put_Extent(spXZQEnv);
			//��Ϊ���ŵ�ͼ���ƻ�����ǧ�ı�������������һ�α���
			spMap->put_MapScale(nScale);
			//�ƶ���֮�����һ���ж�
			GetPolygonPoint(spActiveView,dbSYTMapWidth,dbSYTMapHeight,RightTop,&dbSYTRightMaxX,&dbSYTRightMaxY,&dbSYTRightMinX,&dbSYTRightMinY);
			hrSYTRight = CheckCover(spXZQXZFtLayer,dbSYTRightMaxX,dbSYTRightMaxY,dbSYTRightMinX,dbSYTRightMinY);
		}
		///ȷ��ʾ��ͼλ��
		GetPolygonByPoint(dbNewMapFrameMaxX,dbNewMapFrameMaxY,dbNewMapFrameMaxX - dbSYTEleWidth,dbNewMapFrameMaxY - dbSYTEleHeight,spSYTPolygon);
		//ȷ��ʾ��ͼ����λ��
		dbMapWZX = dbNewMapFrameMaxX - (dbSYTEleWidth / 2);
		dbMapWZY = dbNewMapFrameMaxY - 0.1;
		spMapWZPoint->put_X(dbMapWZX);
		spMapWZPoint->put_Y(dbMapWZY);
		//ȷ��ָ����λ�ã�����ߵ�λ��
		GetPolygonByPoint(dbNewMapFrameMinX + dbArrowWidth + dbArrowBlank,dbNewMapFrameMaxY - dbArrowBlank,dbNewMapFrameMinX + dbArrowBlank,dbNewMapFrameMaxY - dbArrowHeight - dbArrowBlank,spArrowPolygon);
	}
	MoveLegendElement(ipPageLayoutControl,spXZQXZFtLayer,spActiveViewCenter);
	spSYTElement->put_Geometry(spSYTPolygon);

	//�ƶ�ʾ��ͼ��ͼ����λ�ã���ֹ�����ڸ�
	double dbSYTMoveLength,dbSYTScale,nSYTScale;
	//��ȡʾ��ͼ��ͼ�泤�Ⱥ�ֽ�泤�ȱ�ֵ	
	CComQIPtr<IMapFrame> spSYTMapFrame = spSYTElement;
	CComQIPtr<IMap> spSYTMap;
	CComQIPtr<IEnvelope> spSYTMapEnvlope;
	double dbSYTEnvHeight;
	double dbSYTMapMaxX,dbSYTMapMaxY,dbSYTMapMinX,dbSYTMapMinY;
	spSYTMapFrame->get_Map(&spSYTMap);
	spSYTMap->get_MapScale(&nSYTScale);
	CComQIPtr<IActiveView> spSYTActiveView = spSYTMap;
	spSYTActiveView->get_Extent(&spSYTMapEnvlope);
	spSYTMapEnvlope->get_XMax(&dbSYTMapMaxX);
	spSYTMapEnvlope->get_YMax(&dbSYTMapMaxY);
	spSYTMapEnvlope->get_XMin(&dbSYTMapMinX);
	spSYTMapEnvlope->get_YMin(&dbSYTMapMinY);
	spSYTMapEnvlope->get_Height(&dbSYTEnvHeight);
	dbSYTScale = dbSYTEleHeight / dbSYTEnvHeight;
	dbSYTMoveLength = 0.5 / dbSYTScale;
	CComQIPtr<IPoint> spSYTPoint;
	spSYTPoint.CoCreateInstance(CLSID_Point);
	spSYTPoint->put_X((dbSYTMapMaxX + dbSYTMapMinX) / 2);
	spSYTPoint->put_Y((dbSYTMapMaxY + dbSYTMapMinY) / 2 + dbSYTMoveLength);
	dbSYTEnvHeight += dbSYTMoveLength;
	spSYTMapEnvlope->put_Height(dbSYTEnvHeight);
	spSYTMapEnvlope->CenterAt(spSYTPoint);
	spSYTActiveView->put_Extent(spSYTMapEnvlope);
	spSYTActiveView->put_FullExtent(spSYTMapEnvlope);
	spSYTMap->put_MapScale(nSYTScale * 1.1);

	if (spMapWZElement != NULL)
	{
		spMapWZElement->put_Geometry(spMapWZPoint);
	}
	if (spArrowElement != NULL)
	{
		spArrowElement->put_Geometry(spArrowPolygon);
	}

	//������������ƶ��������ֱ���
	//�ƶ�����λ��
	double dbTitleWidth,dbTiteHeight;
	CComQIPtr<IElement> spTitleElement;
	ipPageLayoutControl->FindElementByName(_T("MAPTITLE"),1,&spTitleElement);
	if (spTitleElement != NULL)
	{
		GetTextElementSize(spTitleElement,spActiveView,&dbTitleWidth,&dbTiteHeight);
		CComQIPtr<IPoint> spTitlePoint;
		spTitlePoint.CoCreateInstance(CLSID_Point);
		spTitlePoint->put_X((dbNewMapFrameMaxX + dbNewMapFrameMinX) / 2);
		spTitlePoint->put_Y(dbNewMapFrameMaxY + 1 + dbTiteHeight);
		spTitleElement->put_Geometry(spTitlePoint);
	}
	//�ƶ���1980������...��λ��
	CComQIPtr<IElement> spMapBZElement;
	ipPageLayoutControl->FindElementByName(_T("MAPBZ"),1,&spMapBZElement);
	if (spMapBZElement != NULL)
	{
		double dbElementHeight,dbElementWidth;
		GetTextElementSize(spMapBZElement,spActiveView,&dbElementWidth,&dbElementHeight);
		CComQIPtr<IPoint> spPoint;
		spPoint.CoCreateInstance(CLSID_Point);
		spPoint->put_X(dbNewMapFrameMinX + (dbElementWidth / 2));
		spPoint->put_Y(dbNewMapFrameMinY - 0.4);
		spMapBZElement->put_Geometry(spPoint);
	}
	//�ƶ���ͼ��λ������λ��
	CComQIPtr<IElement> spMapZFRQElement;
	ipPageLayoutControl->FindElementByName(_T("MAPZFRQ"),1,&spMapZFRQElement);
	if (spMapZFRQElement != NULL)
	{
		double dbElementHeight,dbElementWidth;
		GetTextElementSize(spMapZFRQElement,spActiveView,&dbElementWidth,&dbElementHeight);
		CComQIPtr<IPoint> spPoint;
		spPoint.CoCreateInstance(CLSID_Point);
		spPoint->put_X(dbNewMapFrameMinX);
		spPoint->put_Y(dbNewMapFrameMinY - 1.5);
		spMapZFRQElement->put_Geometry(spPoint);
	}
	//�ƶ�Scale Textλ��
	CComQIPtr<IElement> spScaleElement;
	ipPageLayoutControl->FindElementByName(_T("Scale Text"),1,&spScaleElement);
	if (spScaleElement != NULL)
	{
		double dbElementHeight,dbElementWidth;
		CComQIPtr<IGeometry> spTmpGeo;
		CComQIPtr<IEnvelope> spTmpEnv;
		spScaleElement->get_Geometry(&spTmpGeo);
		spTmpGeo->get_Envelope(&spTmpEnv);
		spTmpEnv->get_Width(&dbElementWidth);
		spTmpEnv->get_Height(&dbElementHeight);
		CComQIPtr<IPolygon> spScalePolygon;
		spScalePolygon.CoCreateInstance(CLSID_Polygon);
		//double dbNewMapFrameWidth = ;
		GetPolygonByPoint((dbNewMapFrameMaxX + dbNewMapFrameMinX + dbElementWidth) / 2, dbNewMapFrameMinY - 0.4, (dbNewMapFrameMaxX + dbNewMapFrameMinX - dbElementWidth) / 2, dbNewMapFrameMinY - 0.4 - dbElementHeight,spScalePolygon);
		spScaleElement->put_Geometry(spScalePolygon);
	}
	//�ƶ�MAPPUBLICλ��
	CComQIPtr<IElement> spMapPublicElement;
	ipPageLayoutControl->FindElementByName(_T("MAPPUBLIC"),1,&spMapPublicElement);
	if (spMapPublicElement != NULL)
	{
		double dbElementHeight,dbElementWidth;
		GetTextElementSize(spMapPublicElement,spActiveView,&dbElementWidth,&dbElementHeight);
		CComQIPtr<IPoint> spPoint;
		spPoint.CoCreateInstance(CLSID_Point);
		spPoint->put_X(dbNewMapFrameMaxX);
		spPoint->put_Y(dbNewMapFrameMinY - 0.4);
		spMapPublicElement->put_Geometry(spPoint);
	}

// 		//����ֽ�Ŵ�С
// 		double dbPageHeight = dbEleHeight + PAGE_BUTTON + PAGE_HEAD;
// 		double dbPageWidth = dbEleWidth + PAGE_LEFT_RIGHT * 2;
// 		double dbPageMaxLenth = ((dbPageHeight - dbPageWidth) > 0) ? dbPageHeight:dbPageWidth;
// 		double dbPageMinLengh = ((dbPageHeight - dbPageWidth) < 0) ? dbPageHeight:dbPageWidth;


	//�����е���ͼˢ�¶���������һ��ˢ�£�������˸
	ipPageLayoutControl->ZoomToWholePage();
	ipPageLayoutControl->Refresh(esriViewAll);

}


void CExportThemeMap::ExportThemeMapTDZZ2(CComQIPtr<IWorkspace> ipGHWorkspace,
										  CString strXZQDM,
										  CComQIPtr<ILayer> spZDQYLayer,
										  CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig,
										  BOOL bIsZXCQ,
										  BOOL bIsLockScale)
{
	double    nScale = 0;
	CComQIPtr<ISpatialReference> spSpatial = (IUnknown*)NULL;
	HRESULT hr;

	if(strXZQDM.IsEmpty() || strMapConfig.IsEmpty())return;
	//���ò�����ͼΪ��ǰ���ͼ
	CComQIPtr<GisqLib::IGSActiveView> ipGSActiveView;
	for (int i = 0;i < m_ipGisqFramework->GetViewCount();i++)
	{
		ipGSActiveView =m_ipGisqFramework->GetViewItem(i);
		if (ipGSActiveView->GetViewType() == (short)VT_PageLayoutView)
		{
			GisqLib::IGSActiveView* ipTemp = ipGSActiveView;
			m_ipGisqFramework->SetActiveView(ipTemp);
			break;
		}
	}
	ASSERT(ipGSActiveView != NULL);
	//��ȡ������ͼ�ؼ�����ȡֽ�Ŷ���
	CComQIPtr<IPageLayoutControl> ipPageLayoutControl = ipGSActiveView->GetControl();	
	CString strValue = _T("");
	//���ò�����ͼ����(���ڡ�����...)
	if(strMapConfig.Lookup(_T("MODELPATH"),strValue))
	{
		//��ȡMXD�ĵ�����Ŀ¼
		TCHAR szAppFullPath[MAX_PATH];
		DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(CHAR);
		GetModuleFileName(NULL,szAppFullPath,dBufferSize);
		CString strAppFolder(szAppFullPath);
		strAppFolder = strAppFolder.Mid(0,strAppFolder.ReverseFind('\\'));
		CString strMXDFilePath = strAppFolder + _T("\\data\\template\\");
		strMXDFilePath += strValue;
		_bstr_t strDocumentPath = strMXDFilePath;
		//ʹ��arcgis�ӿڼ����ĵ�������ʹ��AE��ܣ�
		//��Ҫ����Ϊʹ��AE����ظ�����MXD�ĵ��ᱻ�Ż������ظ�����ʱ���ɹ�
		VARIANT_BOOL bSecceed;
		ipPageLayoutControl->CheckMxFile(strDocumentPath,&bSecceed);
		if(bSecceed == VARIANT_FALSE)return;
		HRESULT hr = ipPageLayoutControl->LoadMxFile(strDocumentPath);
		if (FAILED(hr))return;
		strValue = _T("");
	}
	POSITION pItem = strMapConfig.GetStartPosition();
	while(pItem != NULL)
	{
		CString strItemName;
		CString strItemValue;
		CComPtr<IElement> ipElement = NULL;
		strMapConfig.GetNextAssoc(pItem,strItemName,strItemValue);
		ipPageLayoutControl->FindElementByName(strItemName.AllocSysString(),1,&ipElement);
		if (ipElement != NULL)
		{
			CComBSTR strText;
			CComQIPtr<ITextElement> ipTextElement = ipElement;
			strText = strItemValue;
			ipTextElement->put_Text(strText);
		}
	}

	CComQIPtr<IFeatureWorkspace> ipGHFeatureWorkspace = ipGHWorkspace;
	CComQIPtr<IGraphicsContainer> ipGraphicsContainer;
	ipPageLayoutControl->get_GraphicsContainer(&ipGraphicsContainer);
	ipGraphicsContainer->Reset();
	CComQIPtr<IElement> ipElement;
	ipGraphicsContainer->Next(&ipElement);
	//����Element

	//����һ�����ڱ���������������Ķ���
	CComQIPtr<IFeatureLayer> spXZQXZFtLayer;
	while(ipElement != NULL)
	{
		CComQIPtr<IEnvelope> ipEnvelope;
		CComQIPtr<IMapFrame> ipMapFrame = ipElement;
		if (ipMapFrame != NULL)
		{
			CComQIPtr<IMap> ipMap;
			ipMapFrame->get_Map(&ipMap);
			CComQIPtr<IEnumLayer> ipEnumLayer;
			CComQIPtr<IUID> uid;
			uid.CoCreateInstance(CLSID_UID);
			uid->put_Value(_variant_t(_T("{40A9E885-5533-11d0-98BE-00805F7CED21}")));
			ipMap->get_Layers(uid,TRUE,&ipEnumLayer);
			ipEnumLayer->Reset();
			CComQIPtr<ILayer> ipLayer;
			ipEnumLayer->Next(&ipLayer);
			//����ͼ��
			while (ipLayer != NULL)
			{
				CComQIPtr<IFeatureClass> ipFeatureClass;
				CComQIPtr<IFeatureLayer> ipFeatureLayer = ipLayer;
				CComQIPtr<IDataLayer> ipDataLayer = ipFeatureLayer;
				CComQIPtr<IName> ipName;
				ipDataLayer->get_DataSourceName(&ipName);
				CComQIPtr<IDatasetName> ipDatasetName = ipName;
				CComBSTR bstrName;
				ipDatasetName->get_Name(&bstrName);
				CString strName(bstrName);
				if (strName.Find(_T(".")) >= 0)
				{
					strName = strName.Right(strName.GetLength() - strName.Find(_T(".")) - 1);
				}
				//�����ص�����ͼ��
				if (strName.Find(_T("TDZZZDQY")) >= 0)
				{
					CComQIPtr<IFeatureLayer> spFtZDQYLayer = spZDQYLayer;
					spFtZDQYLayer->get_FeatureClass(&ipFeatureClass);
					//ipFeatureLayer->putref_FeatureClass(ipFeatureClass);
				}else
				{
					ipGHFeatureWorkspace->OpenFeatureClass(_bstr_t(strName),&ipFeatureClass);
				}
				//ע��ipFeatureClassһ�㲻ΪNULL�����ΪNULL������Ҫ���SDE���Ƿ�������
				if(ipFeatureClass == NULL)
				{
					ipLayer.Release();
					ipEnumLayer->Next(&ipLayer);
					continue;
				}				
				if (strName == _T("XZQX"))
				{
					CComBSTR sMapName;
					ipMap->get_Name(&sMapName);
					CString strMapName(sMapName);
					if (strMapName.Trim() == _T("ʾ��ͼ"))//���������ŵ�������
					{
						CComQIPtr<IQueryFilter> ipQueryFilter;
						ipQueryFilter.CoCreateInstance(CLSID_QueryFilter);
						CString strTemp1 = _T("xzqdm in ('") + strXZQDM.Left(6) + _T("')");
						CComBSTR sCondition = strTemp1.AllocSysString();
						ipQueryFilter->put_WhereClause(sCondition);
						CComQIPtr<IFeatureCursor> ipFeatureCursor;
						ipFeatureClass->Search(ipQueryFilter,FALSE,&ipFeatureCursor);
						CComQIPtr<IFeature> ipFeature;
						ipFeatureCursor->NextFeature(&ipFeature);
						ipEnvelope.Release();
						ipEnvelope.CoCreateInstance(CLSID_Envelope);
						while(ipFeature != NULL)
						{
							CComQIPtr<IEnvelope> spEnv;
							CComQIPtr<IGeometry> ipGeo;
							ipFeature->get_Shape(&ipGeo);
							esriGeometryType Geotype;
							ipGeo->get_GeometryType(&Geotype);
							ipGeo->get_Envelope(&spEnv);
							ipEnvelope->Union(spEnv);
							ipFeature.Release();
							ipFeatureCursor->NextFeature(&ipFeature);
						}
					}
					CComQIPtr<IActiveView> spActiveView = ipMap;
					spActiveView->put_Extent(ipEnvelope);
					//��ȡʾ��ͼͼ���С
					//spSYTElement = ipElement;
				}else if(strName == _T("XZQXZ"))
				{
					CComBSTR sMapName;
					ipMap->get_Name(&sMapName);
					CString strMapName(sMapName);
					if (strMapName.Trim() == _T("��ʾͼ"))//���������ŵ�������
					{
						spXZQXZFtLayer = ipFeatureLayer;
					}
				}
				if (ipFeatureClass != NULL)
				{

					if (spSpatial == NULL)
					{
						CComQIPtr<IGeoDataset> spGeoDataset = ipFeatureClass;
						spGeoDataset->get_SpatialReference(&spSpatial);
					}
					CComQIPtr<IDataset> ipDataset = ipFeatureClass;
					CComBSTR sName;
					ipDataset->get_BrowseName(&sName);
					ipDatasetName->put_Name(sName);
					CComQIPtr<IName> ipName1;
					ipDataset->get_FullName(&ipName1);
					ipDatasetName = ipName1;
					VARIANT_BOOL bSucceed;//0 == FALSE, -1 == TRUE
					ipDataLayer->Connect((CComQIPtr<IName>)ipDatasetName,&bSucceed);

					ipFeatureLayer->putref_FeatureClass(ipFeatureClass);
					ipFeatureLayer->put_Visible(VARIANT_TRUE);
					//����û���������ֶε�ͼ��
					esriGeometryType LayerType;
					ipFeatureClass->get_ShapeType(&LayerType);
					long nIndexXZQDM = -1;
					long nIndexXZDM = -1;
					ipFeatureClass->FindField(_T("XZQDM"),&nIndexXZQDM);
					ipFeatureClass->FindField(_T("XZDM"),&nIndexXZDM);
					if ((nIndexXZQDM < 0 /*|| nIndexXZDM < 0*/) && LayerType == esriGeometryPolygon)
					{
						CComQIPtr<IFeatureLayerDefinition> ipFeatureLayerDefinition = ipFeatureLayer;
						ipFeatureLayerDefinition->put_DefinitionExpression(_T(""));
						ipLayer.Release();
						ipEnumLayer->Next(&ipLayer);						
						continue;
					}
					CComQIPtr<IFeatureLayerDefinition> ipFeatureLayerDefinition = ipFeatureLayer;
					CComBSTR sSQL;
					ipFeatureLayerDefinition->get_DefinitionExpression(&sSQL);
					CString strSQL(sSQL);
					if (!strSQL.IsEmpty())
					{
						strSQL.Replace(_T("left"),_T("substr"));
						strSQL.Replace(_T("["),_T(""));
						strSQL.Replace(_T("]"),_T(""));
						//add by lcc 2016-11-14 �޸���ͼ���ݹ���BUG
						if (strSQL.Find(_T("XZDM")) >= 0 || strSQL.Find(_T("XZQDM")) >= 0 || strSQL.Find(_T("xzdm")) >= 0 || strSQL.Find(_T("xzqdm")) >= 0)
						{
							int nDMStart =  strSQL.Find(_T("'"));
							int nDMEnd = strSQL.ReverseFind('\'');
							CString DM = strSQL.Mid(nDMStart + 1,nDMEnd - nDMStart -1);
							if (DM.GetLength() < 9)
							{
								strSQL.Replace(DM,strXZQDM.Left(6));
							}else
							{
								strSQL.Replace(DM,strXZQDM);
							}
						}
						//end 2016-11-14
						sSQL = strSQL.AllocSysString();
						ipFeatureLayerDefinition->put_DefinitionExpression(sSQL);
						SysFreeString(sSQL);
					}
				}
				ipLayer.Release();
				ipEnumLayer->Next(&ipLayer);
			}
		}
		ipElement.Release();
		ipGraphicsContainer->Next(&ipElement);
	}
	ipPageLayoutControl->Refresh(esriViewAll);

	//�������ƥ��֮������Ӧͼ��
	//�ȶ�ȡXML�������
	//������������
	//double dbXZScale,dbXScale,dbZXCQScale;
	CString strXZSCALE,strXScale,strZXCQScale;
	strMapConfig.Lookup(_T("XZSCALE"),strXZSCALE);
	strMapConfig.Lookup(_T("XSCALE"),strXScale);
	strMapConfig.Lookup(_T("ZXCQSCALE"),strZXCQScale);
	swscanf_s(_bstr_t(strXZSCALE),_T("%lf"),&dbXZScale);
	swscanf_s(_bstr_t(strXScale),_T("%lf"),&dbXScale);
	swscanf_s(_bstr_t(strZXCQScale),_T("%lf"),&dbZXCQScale);
	//��ֽ�ŷ�Χ����
	//double dbMaxPageWidth,dbMinPageWidth;
	CString strMaxPageWidth,strMinPageWidth;
	strMapConfig.Lookup(_T("PAGE_MAX_WIDTH"),strMaxPageWidth);
	strMapConfig.Lookup(_T("PAGE_MIN_WIDTH"),strMinPageWidth);
	swscanf_s(_bstr_t(strMaxPageWidth),_T("%lf"),&dbMaxPageWidth);
	swscanf_s(_bstr_t(strMinPageWidth),_T("%lf"),&dbMinPageWidth);
	//��ȡֽ������
	//double dbPageHead,dbPageButton,dbPage_LeftRight;
	CString strPageHead,strPageButton,strPage_LeftRight;
	strMapConfig.Lookup(_T("PAGE_HEAD"),strPageHead);
	strMapConfig.Lookup(_T("PAGE_BUTTON"),strPageButton);
	strMapConfig.Lookup(_T("PAGE_LEFT_RIGHT"),strPage_LeftRight);
	swscanf_s(_bstr_t(strPageHead),_T("%lf"),&dbPageHead);
	swscanf_s(_bstr_t(strPageButton),_T("%lf"),&dbPageButton);
	swscanf_s(_bstr_t(strPage_LeftRight),_T("%lf"),&dbPage_LeftRight);
	//��ȡͼ������
	//double dbFrameBlank;
	CString strFrameBlank;
	strMapConfig.Lookup(_T("FRAME_BLANK"),strFrameBlank);
	swscanf_s(_bstr_t(strFrameBlank),_T("%lf"),&dbFrameBlank);
	//����λ����
	dbMoveLenth = 1.0;

	//���ȸ��ݳ�ͼ��Χ����ֽ�ź�ͼ��
	CComQIPtr<IElement> spLayersElement;
	CComQIPtr<IGeometry> spEleGeo;
	CComQIPtr<IEnvelope> spLayersEnv;

	double dbLayersEleHeight = 0.0,dbLayersEleWidth = 0.0;//��¼����������mapframe��С
	double dbScale = 0.0;//��¼��ʾ����
	double dbXZQHeight = 0.0,dbXZQWidth = 0.0;//��¼������map��Χ��С
	CComQIPtr<IEnvelope> spXZQEnv;//��¼һ������µ���������Χ
	CComQIPtr<IPoint> spActiveViewCenter;//��¼��ǰ��ͼ���ĵ�

	ipPageLayoutControl->FindElementByName(_T("��ʾͼ"),1,&spLayersElement);
	if (spLayersElement == NULL) return;
	//��ȡͼ���С
	spLayersElement->get_Geometry(&spEleGeo);
	spEleGeo->get_Envelope(&spLayersEnv);
	spLayersEnv->get_Width(&dbLayersEleWidth);
	spLayersEnv->get_Height(&dbLayersEleHeight);

	CComQIPtr<IMapFrame> spMapFrame = spLayersElement;
	//��ͼ���Ľ�����
	double dbNewMapFrameMaxX,dbNewMapFrameMaxY,dbNewMapFrameMinX,dbNewMapFrameMinY;
	spLayersEnv->get_XMax(&dbNewMapFrameMaxX);
	spLayersEnv->get_XMin(&dbNewMapFrameMinX);
	spLayersEnv->get_YMax(&dbNewMapFrameMaxY);
	spLayersEnv->get_YMin(&dbNewMapFrameMinY);

	//����������
	double dbScaleChanged;
	//ֽ�ų���
	double dbPageWidth,dbPageHeight;
	//�����ͼ�򳤿�
	double dbNewFrameHeight,dbNewFrameWidth;
	//�Ƿ�����
	BOOL bIsPortrait = TRUE;
	//ͼ���Ľ����꣬�����ж��ĽǵĿհ״�С
	double dbMapMaxX,dbMapMaxY,dbMapMinX,dbMapMinY;
	//�Ƿ񽫵�ǰԪ�ط�����࣬�������ƶ�����ʱȷ���Ӽ�����
	//BOOL bIsPutLeft = TRUE;

	CComQIPtr<IMap> spMap;
	spMapFrame->get_Map(&spMap);
	spMap->putref_SpatialReference(spSpatial);//����֮ǰ��ȡ������ϵ��map������
	CComQIPtr<IActiveView> spActiveView = spMap;
	if (spMapFrame != NULL)
	{				
		//���õ�ͼ������
		if (bIsZXCQ == TRUE)
		{
			nScale = dbZXCQScale;
			spMap->put_MapScale(dbZXCQScale);
		}else if (bIsZXCQ == FALSE)
		{
			nScale = dbXZScale;
			spMap->put_MapScale(dbXZScale);
		}
		//��ȡ��ͼ��Χ��С,������ʾ��Χ��ͼ���С�ı���
		//��Ϊ֮ǰ����ƥ������ϵ�Ĳ���������ȷ������ͳһ		
		CComQIPtr<IEnvelope> spMapEnv;
		double dbMapWidth,dbMapHeight;
		spActiveView->get_Extent(&spMapEnv);
		spMapEnv->get_Width(&dbMapWidth);
		dbScale = dbLayersEleWidth/dbMapWidth;
		spMapEnv->get_Height(&dbMapHeight);
		dbScale = dbLayersEleHeight/dbMapHeight;

		//��ȡ���������µ���������Χ
		CComQIPtr<IFeatureCursor> spFtCursor;
		CComQIPtr<IFeature> spFt;
		spXZQXZFtLayer->Search(NULL,VARIANT_FALSE,&spFtCursor);
		spFtCursor->NextFeature(&spFt);
		spXZQEnv.CoCreateInstance(CLSID_Envelope);
		while(spFt != NULL)
		{
			CComQIPtr<IGeometry> spTmpGeo;
			CComQIPtr<IEnvelope> spTmpEnv;
			spFt->get_Shape(&spTmpGeo);
			spTmpGeo->get_Envelope(&spTmpEnv);
			spXZQEnv->Union(spTmpEnv);
			spFt.Release();
			spFtCursor->NextFeature(&spFt);
		}
		//��Ϊ��������ϵƥ�䣬��������ϵ��ͬ�����
		SetFullExtent(spXZQEnv,FALSE);
		spXZQEnv->get_Width(&dbXZQWidth);
		spXZQEnv->get_Height(&dbXZQHeight);
		dbLayersEleHeight = dbXZQHeight * dbScale;
		dbLayersEleWidth = dbXZQWidth *dbScale;
		//�����ʼ��������Χ���ĵ�
		double dbXZQMaxX,dbXZQMaxY,dbXZQMinX,dbXZQMinY;
		spXZQEnv->get_XMax(&dbXZQMaxX);
		spXZQEnv->get_YMax(&dbXZQMaxY);
		spXZQEnv->get_XMin(&dbXZQMinX);
		spXZQEnv->get_YMin(&dbXZQMinY);
		spActiveViewCenter.CoCreateInstance(CLSID_Point);
		spActiveViewCenter->put_X((dbXZQMaxX + dbXZQMinX) / 2);
		spActiveViewCenter->put_Y((dbXZQMaxY + dbXZQMinY) / 2);
		//����
		dbLayersEleHeight += dbFrameBlank;
		dbLayersEleWidth += dbFrameBlank;
		dbPageHeight  = dbLayersEleHeight + dbPageHead + dbPageButton;
		dbPageWidth = dbLayersEleWidth + dbPage_LeftRight * 2;
		if (bIsLockScale == FALSE)
		{
			double dbPageMinLine = (dbPageHeight < dbPageWidth) ? dbPageHeight : dbPageWidth;
			double dbPageMaxLine = (dbPageHeight > dbPageWidth) ? dbPageHeight : dbPageWidth;
			//�ж��Ƿ񳬹�ֽ��С�޶���Χ
			//������һ���ڷ�Χ�ڣ��Ϳ��Բ���������ֽ��
			if ( (dbPageMinLine > dbMinPageWidth && dbPageMinLine < dbMaxPageWidth) || (dbPageMaxLine > dbMinPageWidth && dbPageMaxLine < dbMaxPageWidth))
			{
				//Todo...
			}else if (dbPageMinLine > dbMaxPageWidth)//С�ڻ��ߴ���ֽ�ŷ�Χ
			{
				//���ձ������ŵ�ֽ�ŷ�Χ�ڵı����� 
				double dbScaleChange = dbMaxPageWidth / dbPageMinLine;
				dbScaleChanged = nScale / dbScaleChange;
				if (dbScaleChanged < 10000)
				{
					dbScaleChanged = ceil(dbScaleChanged / 2000) * 2000;
				}else
				{
					dbScaleChanged = ceil(dbScaleChanged / 5000) * 5000;
				}
				//�Ƚϱ������Ƿ������С
				double dbMaxScale = 25000;
				double dbMinScale = 2000;
				if (dbScaleChanged > dbMaxScale)
				{
					dbScaleChanged = dbMaxScale;
				}else if (dbScaleChanged < dbMinScale)
				{
					dbScaleChanged = dbMinScale;
				}
				nScale = dbScaleChanged;
				spMap->put_MapScale(dbScaleChanged);
				ipPageLayoutControl->Refresh(esriViewAll);
				GetSizeScale(spMapFrame,&dbScale);
				GetFrameSize(spXZQXZFtLayer,dbScale,&dbNewFrameWidth,&dbNewFrameHeight);
				dbNewFrameHeight += dbFrameBlank;
				dbNewFrameWidth += dbFrameBlank;
				dbPageHeight  = dbNewFrameHeight + dbPageHead + dbPageButton;
				dbPageWidth = dbNewFrameWidth + dbPage_LeftRight * 2;
			}else if (dbPageMinLine < dbMinPageWidth)
			{
				double dbScaleChange = dbMinPageWidth / dbPageMinLine;
				dbScaleChanged = nScale / dbScaleChange;
				if (dbScaleChanged < 10000)
				{
					dbScaleChanged = floor(dbScaleChanged / 2000) * 2000;
				}else
				{
					dbScaleChanged = floor(dbScaleChanged / 5000) * 5000;
				}
				//�Ƚϱ������Ƿ������С
				double dbMaxScale = 25000;
				double dbMinScale = 2000;
				if (dbScaleChanged > dbMaxScale)
				{
					dbScaleChanged = dbMaxScale;
				}else if (dbScaleChanged < dbMinScale)
				{
					dbScaleChanged = dbMinScale;
				}
				nScale = dbScaleChanged;
				spMap->put_MapScale(dbScaleChanged);
				ipPageLayoutControl->Refresh(esriViewAll);
				GetSizeScale(spMapFrame,&dbScale);
				GetFrameSize(spXZQXZFtLayer,dbScale,&dbNewFrameWidth,&dbNewFrameHeight);
				dbNewFrameHeight += dbFrameBlank;
				dbNewFrameWidth += dbFrameBlank;
				dbPageHeight  = dbNewFrameHeight + dbPageHead + dbPageButton;
				dbPageWidth = dbNewFrameWidth + dbPage_LeftRight * 2;
			}else
			{
				//Todo...ʣ�����
			}
		}else
		{
			//Todo...���Լ�������ߣ�ʲô������
		}
		//���ݳ������������,�����޸�ֽ�ŵĺ�������Ҫ�ֶ��޸�mapframe�ĺ���

		CComQIPtr<IPage> spPage;
		ipPageLayoutControl->get_Page(&spPage);

		//���㳤�ڷ�Χ�ڻ��ǿ��ڷ�Χ��
		if (dbPageHeight < dbMaxPageWidth && dbPageHeight > dbMinPageWidth)
		{
			//���ڷ�Χ�ڣ��Ǻ��
			bIsPortrait = FALSE;

		}else if (dbPageWidth < dbMaxPageWidth && dbPageWidth > dbMinPageWidth)
		{
			//���ڷ�Χ�ڣ�������
			bIsPortrait = TRUE;
		}
		//if (dbPageHeight/dbPageWidth > 1)
		if (bIsPortrait == TRUE)
		{
			spPage->put_Orientation(short(1));
			spPage->put_FormID(esriPageFormCUSTOM);
			spPage->PutCustomSize(dbPageWidth,dbPageHeight);
		}else
		{
			spPage->put_Orientation(short(2));
			spPage->put_FormID(esriPageFormCUSTOM);
			spPage->PutCustomSize(dbPageWidth,dbPageHeight);
		}
		ipPageLayoutControl->Refresh(esriViewAll);
	}

	CComQIPtr<IEnvelope> spNewXZQEnvelop;
	spNewXZQEnvelop.CoCreateInstance(CLSID_Envelope);
	//���ݼ�����ı������¼�����������Χ��ͼ���С
	spMap->put_MapScale(dbScaleChanged);

	CComQIPtr<IFeatureCursor> spFtCursor;
	CComQIPtr<IFeature> spFt;
	spFtCursor.Release();
	if (spFt != NULL)
	{
		spFt.Release();
	}
	spXZQXZFtLayer->Search(NULL,VARIANT_FALSE,&spFtCursor);
	spFtCursor->NextFeature(&spFt);
	while(spFt != NULL)
	{
		CComQIPtr<IGeometry> spTmpGeo;
		CComQIPtr<IEnvelope> spTmpEnv;
		spFt->get_Shape(&spTmpGeo);
		spTmpGeo->get_Envelope(&spTmpEnv);
		spNewXZQEnvelop->Union(spTmpEnv);
		spFt.Release();
		spFtCursor->NextFeature(&spFt);
	}

	//�����������ҵĵ�
	//�Ȼ��㳤��Ϊֽ���С,����1.1������
	double dbXZQNewHeight,dbXZQNewWidth;
	spXZQEnv->get_Height(&dbXZQNewHeight);
	spXZQEnv->get_Width(&dbXZQNewWidth);
	dbNewFrameHeight = dbXZQNewHeight * dbScale  + 5;
	dbNewFrameWidth = dbXZQNewWidth *dbScale  + 5;
	//����Ԥ��ֵ����ͼ���Ľ�����
	dbNewMapFrameMinY = (dbPageHeight - dbNewFrameHeight) / 2;
	dbNewMapFrameMaxY = dbNewMapFrameMinY + dbNewFrameHeight;
	dbNewMapFrameMinX = (dbPageWidth - dbNewFrameWidth) / 2;
	dbNewMapFrameMaxX = dbNewMapFrameMinX + dbNewFrameWidth;

	CComQIPtr<IPolygon> spPolygon;
	spPolygon.CoCreateInstance(CLSID_Polygon);
	GetPolygonByPoint(dbNewMapFrameMaxX,dbNewMapFrameMaxY,dbNewMapFrameMinX,dbNewMapFrameMinY,spPolygon);
	spLayersElement->put_Geometry(spPolygon);
	//���ͼ�����֮�������ʾ��Χ
	spActiveView->put_Extent(spXZQEnv);
	//��Ϊ���ŵ�ͼ���ƻ�����ǧ�ı�������������һ�α���
	spMap->put_MapScale(nScale);

	//HRESULT hrLegendLeft,hrLegendRight;
	HRESULT hrSYTLeft,hrSYTRight;
	//�ƶ�ʾ��ͼλ��
	MoveLegendElement(ipPageLayoutControl,spXZQXZFtLayer,spActiveViewCenter);

	//ˢ��ֽ�ź�ͼ���С��Ϣ
	spEleGeo.Release();
	spLayersEnv.Release();
	spLayersElement->get_Geometry(&spEleGeo);
	spEleGeo->get_Envelope(&spLayersEnv);
	spLayersEnv->get_Width(&dbNewFrameWidth);
	spLayersEnv->get_Height(&dbNewFrameHeight);
	CComQIPtr<IPage> spPage;
	ipPageLayoutControl->get_Page(&spPage);
	spPage->QuerySize(&dbPageWidth,&dbPageHeight);
	dbNewMapFrameMinY = (dbPageHeight - dbNewFrameHeight) / 2;
	dbNewMapFrameMaxY = dbNewMapFrameMinY + dbNewFrameHeight;
	dbNewMapFrameMinX = (dbPageWidth - dbNewFrameWidth) / 2;
	dbNewMapFrameMaxX = dbNewMapFrameMinX + dbNewFrameWidth;

	//���ڱ���ʾ��ͼԪ�صĶ���
	CComQIPtr<IElement> spSYTElement;
	ipPageLayoutControl->FindElementByName(_T("ʾ��ͼ"),1,&spSYTElement);
	//��ȡʾ��ͼ��map�еĳ���
	CComQIPtr<IGeometry> spSYTGeo;
	CComQIPtr<IEnvelope> spSYTEnvelop;
	spSYTElement->get_Geometry(&spSYTGeo);
	spSYTGeo->get_Envelope(&spSYTEnvelop);

	double dbSYTEleHeight,dbSYTEleWidth;
	spSYTEnvelop->get_Height(&dbSYTEleHeight);
	spSYTEnvelop->get_Width(&dbSYTEleWidth);
	double dbSYTMapHeight,dbSYTMapWidth;
	dbSYTMapHeight = dbSYTEleHeight / dbScale;
	dbSYTMapWidth = dbSYTEleWidth / dbScale;

	//��ȡλ�����֣�ָ����ĳ���
	CComQIPtr<IElement> spMapWZElement;//λ������
	CComQIPtr<IElement> spArrowElement;//ָ����
	ipPageLayoutControl->FindElementByName(_T("MAPWZ"),1,&spMapWZElement);
	ipPageLayoutControl->FindElementByName(_T("MAPARROW"),1,&spArrowElement);
	double dbMapWZTextXSize,dbMapWZTextYSize;//ʾ��ͼ���ֳ���
	double dbArrowWidth,dbArrowHeight;
	double dbArrowBlank = 1.0;

	//��ȡʾ��ͼ���ֳ���
	if (spMapWZElement != NULL)
	{
		GetTextElementSize(spMapWZElement,spActiveView,&dbMapWZTextXSize,&dbMapWZTextYSize);
	}
	if (spArrowElement != NULL)
	{
		CComQIPtr<IGeometry> spTmpGeo;
		CComQIPtr<IEnvelope> spTmpEnv;
		spArrowElement->get_Geometry(&spTmpGeo);
		spTmpGeo->get_Envelope(&spTmpEnv);
		spTmpEnv->get_Width(&dbArrowWidth);
		spTmpEnv->get_Height(&dbArrowHeight);
	}
	//ȷ��λ��
	double dbMapWZX,dbMapWZY;//ʾ��ͼ��������
	CComQIPtr<IPoint> spMapWZPoint;//ʾ��ͼ���ֵ�
	spMapWZPoint.CoCreateInstance(CLSID_Point);
	CComQIPtr<IPolygon> spSYTPolygon;//ʾ��ͼ����
	CComQIPtr<IPolygon> spArrowPolygon;//ָ�������
	spSYTPolygon.CoCreateInstance(CLSID_Polygon);
	spArrowPolygon.CoCreateInstance(CLSID_Polygon);

	//��ȡ���ͼ�Ľ�����
	GetNewActiveViewPoint(&dbMapMaxX,&dbMapMaxY,&dbMapMinX,&dbMapMinY,spActiveView);
	CComQIPtr<IPoint> spLeftPoint;//����
	CComQIPtr<IPoint> spRightPoint;//�Ҳ��
	spLeftPoint.CoCreateInstance(CLSID_Point);
	spRightPoint.CoCreateInstance(CLSID_Point);
	spLeftPoint->put_X(dbMapMinX);
	spLeftPoint->put_Y(dbMapMaxY);
	spRightPoint->put_X(dbMapMaxX);
	spRightPoint->put_Y(dbMapMaxY);
	//���ݿհ״�Сȷ�����ı�
	double dbLeftDistance,dbRightDistance;
	GetDistance(spXZQXZFtLayer,(CComQIPtr<IGeometry>)spLeftPoint,&dbLeftDistance);
	GetDistance(spXZQXZFtLayer,(CComQIPtr<IGeometry>)spRightPoint,&dbRightDistance);

	//���ҷֿ��������Կհ׽϶��һ��������У��
	if (dbLeftDistance > dbRightDistance)//��߿յĶ࣬�����
	{
		//�ռ��ѯ��Ҫ��4������
		double dbSYTLeftMaxX,dbSYTLeftMaxY,dbSYTLeftMinX,dbSYTLeftMinY;
		GetPolygonPoint(spActiveView,dbSYTMapWidth,dbSYTMapHeight,LeftTop,&dbSYTLeftMaxX,&dbSYTLeftMaxY,&dbSYTLeftMinX,&dbSYTLeftMinY);
		hrSYTLeft = CheckCover(spXZQXZFtLayer,dbSYTLeftMaxX,dbSYTLeftMaxY,dbSYTLeftMinX,dbSYTLeftMinY);

		while(hrSYTLeft == E_FAIL)
		{
			CComQIPtr<IPage> spPage;
			ipPageLayoutControl->get_Page(&spPage);
			double dbCenterX,dbCenterY;
			spActiveViewCenter->get_X(&dbCenterX);
			spActiveViewCenter->get_Y(&dbCenterY);
			//��������ˣ�����ֽ��,��Ӧ������ͼ����ƫ��
			if (dbPageHeight < dbPageWidth)
			{
				if (dbPageWidth + dbMoveLenth > dbMaxPageWidth)
				{
					dbNewFrameHeight += dbMoveLenth;
					dbPageHeight = dbNewFrameHeight + dbPageHead + dbPageButton;
					dbCenterY += (dbMoveLenth / 2) / dbScale;
				}else
				{
					dbNewFrameWidth += dbMoveLenth;
					dbPageWidth = dbNewFrameWidth + dbPage_LeftRight * 2;
					dbCenterX -= (dbMoveLenth / 2) / dbScale;
				}
			} 
			else
			{
				if (dbPageHeight + dbMoveLenth > dbMaxPageWidth)
				{
					dbNewFrameWidth += dbMoveLenth;
					dbPageWidth = dbNewFrameWidth + dbPage_LeftRight * 2;
					dbCenterX -= (dbMoveLenth / 2) / dbScale;
				}else
				{
					dbNewFrameHeight += dbMoveLenth;
					dbPageHeight = dbNewFrameHeight + dbPageHead + dbPageButton;		
					dbCenterY += (dbMoveLenth / 2) / dbScale;
				}

			}
			//Ӧ�õ���
			spActiveViewCenter->put_X(dbCenterX);
			spActiveViewCenter->put_Y(dbCenterY);
			spPage->PutCustomSize(dbPageWidth,dbPageHeight);
			//���¼���ͼ���Ľ�����
			dbNewMapFrameMinY = (dbPageHeight - dbNewFrameHeight) / 2;
			dbNewMapFrameMaxY = dbNewMapFrameMinY + dbNewFrameHeight;
			dbNewMapFrameMinX = (dbPageWidth - dbNewFrameWidth) / 2;
			dbNewMapFrameMaxX = dbNewMapFrameMinX + dbNewFrameWidth;

			CComQIPtr<IPolygon> spPolygon;
			spPolygon.CoCreateInstance(CLSID_Polygon);
			GetPolygonByPoint(dbNewMapFrameMaxX,dbNewMapFrameMaxY,dbNewMapFrameMinX,dbNewMapFrameMinY,spPolygon);
			spLayersElement->put_Geometry(spPolygon);
			//�ƶ���ͼ�����ĵ���
			spXZQEnv->CenterAt(spActiveViewCenter);
			spActiveView->put_Extent(spXZQEnv);
			//��Ϊ���ŵ�ͼ���ƻ�����ǧ�ı�������������һ�α���
			spMap->put_MapScale(nScale);
			//�ƶ���֮�����һ���ж�
			GetPolygonPoint(spActiveView,dbSYTMapWidth,dbSYTMapHeight,LeftTop,&dbSYTLeftMaxX,&dbSYTLeftMaxY,&dbSYTLeftMinX,&dbSYTLeftMinY);
			hrSYTLeft = CheckCover(spXZQXZFtLayer,dbSYTLeftMaxX,dbSYTLeftMaxY,dbSYTLeftMinX,dbSYTLeftMinY);
		}
		//ȷ��ʾ��ͼλ��
		GetPolygonByPoint(dbNewMapFrameMinX + dbSYTEleWidth,dbNewMapFrameMaxY,dbNewMapFrameMinX,dbNewMapFrameMaxY - dbSYTEleHeight,spSYTPolygon);
		//ȷ��ʾ��ͼ����λ��
		dbMapWZX = dbNewMapFrameMinX + (dbSYTEleWidth / 2);
		dbMapWZY = dbNewMapFrameMaxY - 0.1;
		spMapWZPoint->put_X(dbMapWZX);
		spMapWZPoint->put_Y(dbMapWZY);
		//ȷ��ָ����λ�ã����ұߵ�λ��
		GetPolygonByPoint(dbNewMapFrameMaxX - dbArrowBlank,dbNewMapFrameMaxY - dbArrowBlank,dbNewMapFrameMaxX - dbArrowWidth - dbArrowBlank,dbNewMapFrameMaxY - dbArrowHeight - dbArrowBlank,spArrowPolygon);
	}else//�ұ߿յĶ࣬���ұ� 
	{
		double dbSYTRightMaxX,dbSYTRightMaxY,dbSYTRightMinX,dbSYTRightMinY;
		GetPolygonPoint(spActiveView,dbSYTMapWidth,dbSYTMapHeight,RightTop,&dbSYTRightMaxX,&dbSYTRightMaxY,&dbSYTRightMinX,&dbSYTRightMinY);
		hrSYTRight = CheckCover(spXZQXZFtLayer,dbSYTRightMaxX,dbSYTRightMaxY,dbSYTRightMinX,dbSYTRightMinY);
		while(hrSYTRight == E_FAIL)
		{
			CComQIPtr<IPage> spPage;
			ipPageLayoutControl->get_Page(&spPage);
			double dbCenterX,dbCenterY;
			spActiveViewCenter->get_X(&dbCenterX);
			spActiveViewCenter->get_Y(&dbCenterY);
			//��������ˣ�����ֽ��,��Ӧ������ͼ����ƫ��
			if (dbPageHeight < dbPageWidth)
			{
				// 				if (dbPageWidth + dbMoveLenth > dbMaxPageWidth)
				// 				{
				// 					dbNewFrameHeight += dbMoveLenth;
				// 					dbPageHeight = dbNewFrameHeight + dbPageHead + dbPageButton;
				// 					dbCenterY += (dbMoveLenth / 2) / dbScale;
				// 				}else
				// 				{
				dbNewFrameWidth += dbMoveLenth;
				dbPageWidth = dbNewFrameWidth + dbPage_LeftRight * 2;
				dbCenterX += (dbMoveLenth / 2) / dbScale;
				//				}
			} 
			else
			{
				// 				if (dbPageHeight + dbMoveLenth > dbMaxPageWidth)
				// 				{
				// 					dbNewFrameWidth += dbMoveLenth;
				// 					dbPageWidth = dbNewFrameWidth + dbPage_LeftRight * 2;
				// 					dbCenterX += (dbMoveLenth / 2) / dbScale;
				// 				}else
				// 				{
				dbNewFrameHeight += dbMoveLenth;
				dbPageHeight = dbNewFrameHeight + dbPageHead + dbPageButton;		
				dbCenterY += (dbMoveLenth / 2) / dbScale;
				//				}

			}
			//Ӧ�õ���
			spActiveViewCenter->put_X(dbCenterX);
			spActiveViewCenter->put_Y(dbCenterY);
			spPage->PutCustomSize(dbPageWidth,dbPageHeight);
			//���¼���ͼ���Ľ�����
			dbNewMapFrameMinY = (dbPageHeight - dbNewFrameHeight) / 2;
			dbNewMapFrameMaxY = dbNewMapFrameMinY + dbNewFrameHeight;
			dbNewMapFrameMinX = (dbPageWidth - dbNewFrameWidth) / 2;
			dbNewMapFrameMaxX = dbNewMapFrameMinX + dbNewFrameWidth;

			CComQIPtr<IPolygon> spPolygon;
			spPolygon.CoCreateInstance(CLSID_Polygon);
			GetPolygonByPoint(dbNewMapFrameMaxX,dbNewMapFrameMaxY,dbNewMapFrameMinX,dbNewMapFrameMinY,spPolygon);
			spLayersElement->put_Geometry(spPolygon);
			//�ƶ���ͼ�����ĵ���
			spXZQEnv->CenterAt(spActiveViewCenter);
			spActiveView->put_Extent(spXZQEnv);
			//��Ϊ���ŵ�ͼ���ƻ�����ǧ�ı�������������һ�α���
			spMap->put_MapScale(nScale);
			//�ƶ���֮�����һ���ж�
			GetPolygonPoint(spActiveView,dbSYTMapWidth,dbSYTMapHeight,RightTop,&dbSYTRightMaxX,&dbSYTRightMaxY,&dbSYTRightMinX,&dbSYTRightMinY);
			hrSYTRight = CheckCover(spXZQXZFtLayer,dbSYTRightMaxX,dbSYTRightMaxY,dbSYTRightMinX,dbSYTRightMinY);
		}
		//ȷ��ʾ��ͼλ��
		GetPolygonByPoint(dbNewMapFrameMaxX,dbNewMapFrameMaxY,dbNewMapFrameMaxX - dbSYTEleWidth,dbNewMapFrameMaxY - dbSYTEleHeight,spSYTPolygon);
		//ȷ��ʾ��ͼ����λ��
		dbMapWZX = dbNewMapFrameMaxX - (dbSYTEleWidth / 2);
		dbMapWZY = dbNewMapFrameMaxY - 0.1;
		spMapWZPoint->put_X(dbMapWZX);
		spMapWZPoint->put_Y(dbMapWZY);
		//ȷ��ָ����λ�ã�����ߵ�λ��
		GetPolygonByPoint(dbNewMapFrameMinX + dbArrowWidth + dbArrowBlank,dbNewMapFrameMaxY - dbArrowBlank,dbNewMapFrameMinX + dbArrowBlank,dbNewMapFrameMaxY - dbArrowHeight - dbArrowBlank,spArrowPolygon);
	}
	MoveLegendElement(ipPageLayoutControl,spXZQXZFtLayer,spActiveViewCenter);
	spSYTElement->put_Geometry(spSYTPolygon);

	//�ƶ�ʾ��ͼ��ͼ����λ�ã���ֹ�����ڸ�
	double dbSYTMoveLength,dbSYTScale,nSYTScale;
	//��ȡʾ��ͼ��ͼ�泤�Ⱥ�ֽ�泤�ȱ�ֵ	
	CComQIPtr<IMapFrame> spSYTMapFrame = spSYTElement;
	CComQIPtr<IMap> spSYTMap;
	CComQIPtr<IEnvelope> spSYTMapEnvlope;
	double dbSYTEnvHeight;
	double dbSYTMapMaxX,dbSYTMapMaxY,dbSYTMapMinX,dbSYTMapMinY;
	spSYTMapFrame->get_Map(&spSYTMap);
	spSYTMap->get_MapScale(&nSYTScale);
	CComQIPtr<IActiveView> spSYTActiveView = spSYTMap;
	spSYTActiveView->get_Extent(&spSYTMapEnvlope);
	spSYTMapEnvlope->get_XMax(&dbSYTMapMaxX);
	spSYTMapEnvlope->get_YMax(&dbSYTMapMaxY);
	spSYTMapEnvlope->get_XMin(&dbSYTMapMinX);
	spSYTMapEnvlope->get_YMin(&dbSYTMapMinY);
	spSYTMapEnvlope->get_Height(&dbSYTEnvHeight);
	dbSYTScale = dbSYTEleHeight / dbSYTEnvHeight;
	dbSYTMoveLength = 0.5 / dbSYTScale;
	CComQIPtr<IPoint> spSYTPoint;
	spSYTPoint.CoCreateInstance(CLSID_Point);
	spSYTPoint->put_X((dbSYTMapMaxX + dbSYTMapMinX) / 2);
	spSYTPoint->put_Y((dbSYTMapMaxY + dbSYTMapMinY) / 2 + dbSYTMoveLength);
	dbSYTEnvHeight += dbSYTMoveLength;
	spSYTMapEnvlope->put_Height(dbSYTEnvHeight);
	spSYTMapEnvlope->CenterAt(spSYTPoint);
	spSYTActiveView->put_Extent(spSYTMapEnvlope);
	spSYTActiveView->put_FullExtent(spSYTMapEnvlope);
	spSYTMap->put_MapScale(nSYTScale * 1.1);

	if (spMapWZElement != NULL)
	{
		spMapWZElement->put_Geometry(spMapWZPoint);
	}
	if (spArrowElement != NULL)
	{
		spArrowElement->put_Geometry(spArrowPolygon);
	}

	//������������ƶ��������ֱ���
	//�ƶ�����λ��
	double dbTitleWidth,dbTiteHeight;
	CComQIPtr<IElement> spTitleElement;
	ipPageLayoutControl->FindElementByName(_T("MAPTITLE"),1,&spTitleElement);
	if (spTitleElement != NULL)
	{
		GetTextElementSize(spTitleElement,spActiveView,&dbTitleWidth,&dbTiteHeight);
		CComQIPtr<IPoint> spTitlePoint;
		spTitlePoint.CoCreateInstance(CLSID_Point);
		spTitlePoint->put_X((dbNewMapFrameMaxX + dbNewMapFrameMinX) / 2);
		spTitlePoint->put_Y(dbNewMapFrameMaxY + 1 + dbTiteHeight);
		spTitleElement->put_Geometry(spTitlePoint);
	}
	//�ƶ���1980������...��λ��
	CComQIPtr<IElement> spMapBZElement;
	ipPageLayoutControl->FindElementByName(_T("MAPBZ"),1,&spMapBZElement);
	if (spMapBZElement != NULL)
	{
		double dbElementHeight,dbElementWidth;
		GetTextElementSize(spMapBZElement,spActiveView,&dbElementWidth,&dbElementHeight);
		CComQIPtr<IPoint> spPoint;
		spPoint.CoCreateInstance(CLSID_Point);
		spPoint->put_X(dbNewMapFrameMinX + (dbElementWidth / 2));
		spPoint->put_Y(dbNewMapFrameMinY - 0.4);
		spMapBZElement->put_Geometry(spPoint);
	}
	//�ƶ���ͼ��λ������λ��
	CComQIPtr<IElement> spMapZFRQElement;
	ipPageLayoutControl->FindElementByName(_T("MAPZFRQ"),1,&spMapZFRQElement);
	if (spMapZFRQElement != NULL)
	{
		double dbElementHeight,dbElementWidth;
		GetTextElementSize(spMapZFRQElement,spActiveView,&dbElementWidth,&dbElementHeight);
		CComQIPtr<IPoint> spPoint;
		spPoint.CoCreateInstance(CLSID_Point);
		spPoint->put_X(dbNewMapFrameMinX);
		spPoint->put_Y(dbNewMapFrameMinY - 1.5);
		spMapZFRQElement->put_Geometry(spPoint);
	}
	//�ƶ�Scale Textλ��
	CComQIPtr<IElement> spScaleElement;
	ipPageLayoutControl->FindElementByName(_T("Scale Text"),1,&spScaleElement);
	if (spScaleElement != NULL)
	{
		double dbElementHeight,dbElementWidth;
		CComQIPtr<IGeometry> spTmpGeo;
		CComQIPtr<IEnvelope> spTmpEnv;
		spScaleElement->get_Geometry(&spTmpGeo);
		spTmpGeo->get_Envelope(&spTmpEnv);
		spTmpEnv->get_Width(&dbElementWidth);
		spTmpEnv->get_Height(&dbElementHeight);
		CComQIPtr<IPolygon> spScalePolygon;
		spScalePolygon.CoCreateInstance(CLSID_Polygon);
		//double dbNewMapFrameWidth = ;
		GetPolygonByPoint((dbNewMapFrameMaxX + dbNewMapFrameMinX + dbElementWidth) / 2, dbNewMapFrameMinY - 0.4, (dbNewMapFrameMaxX + dbNewMapFrameMinX - dbElementWidth) / 2, dbNewMapFrameMinY - 0.4 - dbElementHeight,spScalePolygon);
		spScaleElement->put_Geometry(spScalePolygon);
	}
	//�ƶ�MAPPUBLICλ��
	CComQIPtr<IElement> spMapPublicElement;
	ipPageLayoutControl->FindElementByName(_T("MAPPUBLIC"),1,&spMapPublicElement);
	if (spMapPublicElement != NULL)
	{
		double dbElementHeight,dbElementWidth;
		GetTextElementSize(spMapPublicElement,spActiveView,&dbElementWidth,&dbElementHeight);
		CComQIPtr<IPoint> spPoint;
		spPoint.CoCreateInstance(CLSID_Point);
		spPoint->put_X(dbNewMapFrameMaxX);
		spPoint->put_Y(dbNewMapFrameMinY - 0.4);
		spMapPublicElement->put_Geometry(spPoint);
	}

	// 		//����ֽ�Ŵ�С
	// 		double dbPageHeight = dbEleHeight + PAGE_BUTTON + PAGE_HEAD;
	// 		double dbPageWidth = dbEleWidth + PAGE_LEFT_RIGHT * 2;
	// 		double dbPageMaxLenth = ((dbPageHeight - dbPageWidth) > 0) ? dbPageHeight:dbPageWidth;
	// 		double dbPageMinLengh = ((dbPageHeight - dbPageWidth) < 0) ? dbPageHeight:dbPageWidth;


	//�����е���ͼˢ�¶���������һ��ˢ�£�������˸
	ipPageLayoutControl->ZoomToWholePage();
	ipPageLayoutControl->Refresh(esriViewAll);

}


HRESULT CExportThemeMap::GetPolygonByPoint(double MaxX, double MaxY, double MinX, double MinY, CComQIPtr<IPolygon> &spPolygon)
{
	CComQIPtr<IPointCollection> spPointCol(spPolygon);
	//����
	CComQIPtr<IPoint> spPoint;
	spPoint.CoCreateInstance(CLSID_Point);
	spPoint->put_X(MinX);
	spPoint->put_Y(MinY);
	spPointCol->AddPoint(spPoint);
	spPoint.Release();
	//����
	spPoint.CoCreateInstance(CLSID_Point);
	spPoint->put_X(MaxX);
	spPoint->put_Y(MinY);
	spPointCol->AddPoint(spPoint);
	spPoint.Release();
	//����
	spPoint.CoCreateInstance(CLSID_Point);
	spPoint->put_X(MaxX);
	spPoint->put_Y(MaxY);
	spPointCol->AddPoint(spPoint);
	spPoint.Release();
	//����
	spPoint.CoCreateInstance(CLSID_Point);
	spPoint->put_X(MinX);
	spPoint->put_Y(MaxY);
	spPointCol->AddPoint(spPoint);
	spPoint.Release();
	//��ΪҪ�պϣ��ټ�һ������
	spPoint.CoCreateInstance(CLSID_Point);
	spPoint->put_X(MinX);
	spPoint->put_Y(MinY);
	spPointCol->AddPoint(spPoint);
	spPoint.Release();

	spPolygon->Close();
	return S_OK;
}

HRESULT CExportThemeMap::CheckCover(CComQIPtr<IFeatureLayer> &spXZQLayer, double MaxX, double MaxY, double MinX, double MinY)
{
	CComQIPtr<IFeatureClass> spFtClass;
	CComBSTR bstrShapeFieldName;
	CComQIPtr<IPolygon> spPolygon;
	spPolygon.CoCreateInstance(CLSID_Polygon);
	GetPolygonByPoint(MaxX,MaxY,MinX,MinY,spPolygon);

	spXZQLayer->get_FeatureClass(&spFtClass);
	if (spFtClass == NULL) return E_POINTER;
// 	spFtClass->get_ShapeFieldName(&bstrShapeFieldName);
// 	CComQIPtr<ISpatialFilter> spSpatailFilter;
// 	spSpatailFilter.CoCreateInstance(CLSID_SpatialFilter);
// 	spSpatailFilter->putref_Geometry(spPolygon);
// 	spSpatailFilter->put_SpatialRel(esriSpatialRelIntersects);
// 	spSpatailFilter->put_GeometryField(bstrShapeFieldName);
	
	CComQIPtr<IFeatureCursor> spFtCurSor;
	spXZQLayer->Search(NULL,VARIANT_FALSE,&spFtCurSor);
	CComQIPtr<IFeature> spXZFt;
	spFtCurSor->NextFeature(&spXZFt);
	while(spXZFt != NULL)
	{
// 		long nIndex;
// 		spFtClass->FindField(_T("XZQMC"),&nIndex);
// 		VARIANT vt;
// 		spXZFt->get_Value(nIndex,&vt);
		CComQIPtr<IGeometry> spGeo;
		spXZFt->get_Shape(&spGeo);
		VARIANT_BOOL bIsEmpty;
		spGeo->get_IsEmpty(&bIsEmpty);
		if(bIsEmpty == VARIANT_FALSE)
		{
// 			CComQIPtr<ITopologicalOperator> spTopo = spGeo;
// 			CComQIPtr<IGeometry> spTmpGeo;		
// 			spTopo->Intersect(spPolygon,esriGeometry2Dimension,&spTmpGeo);
// 			spTmpGeo->get_IsEmpty(&bIsEmpty);
// 			if (bIsEmpty == VARIANT_FALSE) return E_FAIL;//������
			CComQIPtr<IRelationalOperator> spTopo = spGeo;
			CComQIPtr<IGeometry> spTmpGeo;
			spTopo->Disjoint(spPolygon,&bIsEmpty);
			if (bIsEmpty == VARIANT_FALSE) return E_FAIL;//������
		}
		spXZFt.Release();
		spFtCurSor->NextFeature(&spXZFt);
	}

	return S_OK;
}

HRESULT CExportThemeMap::GetNewActiveViewPoint(double *MaxX, double *MaxY, double *MinX, double *MinY,CComQIPtr<IActiveView> &spActiveView)
{
	spActiveView->Refresh();
	CComQIPtr<IEnvelope> spNewLayersEnvelope;
	spActiveView->get_Extent(&spNewLayersEnvelope);
	spNewLayersEnvelope->get_XMin(MinX);
	spNewLayersEnvelope->get_XMax(MaxX);
	spNewLayersEnvelope->get_YMin(MinY);
	spNewLayersEnvelope->get_YMax(MaxY);
	return S_OK;
}

HRESULT CExportThemeMap::GetPolygonPoint(CComQIPtr<IActiveView> &spActiveView, double Width, double Height, ElementPosition Position, double *PolygonMaxX, double *PolygonMaxY, double *PolygonMinX, double *PolygonMinY)
{
	double dbActiveviewMaxX,dbActiveviewMaxY,dbActiveviewMinX,dbActiveviewMinY;
	GetNewActiveViewPoint(&dbActiveviewMaxX,&dbActiveviewMaxY,&dbActiveviewMinX,&dbActiveviewMinY, spActiveView);

// 	Width *= 0.5;
// 	Height *= 0.5;
	if (Position == LeftBotton)
	{
		*PolygonMaxX = dbActiveviewMinX + Width;
		*PolygonMaxY = dbActiveviewMinY + Height;
		*PolygonMinX = dbActiveviewMinX;
		*PolygonMinY = dbActiveviewMinY;
	}else if (Position == RightBotton)
	{
		*PolygonMaxX = dbActiveviewMaxX;
		*PolygonMaxY = dbActiveviewMinY + Height;
		*PolygonMinX = dbActiveviewMaxX - Width;
		*PolygonMinY = dbActiveviewMinY;
	}else if (Position == LeftTop)
	{
		*PolygonMaxX = dbActiveviewMinX + Width;
		*PolygonMaxY = dbActiveviewMaxY;
		*PolygonMinX = dbActiveviewMinX;
		*PolygonMinY = dbActiveviewMaxY - Height;
	}else if (Position == RightTop)
	{
		*PolygonMaxX = dbActiveviewMaxX;
		*PolygonMaxY = dbActiveviewMaxY;
		*PolygonMinX = dbActiveviewMaxX - Width;
		*PolygonMinY = dbActiveviewMaxY - Height;
	}
	return S_OK;
}

void CExportThemeMap::CreateSpatial(int nProjCS)
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

HRESULT CExportThemeMap::GetTextElementSize(CComQIPtr<IElement> &spElement, CComQIPtr<IActiveView> spActiveView, double *dbWidth, double *dbHeight)
{
	CComQIPtr<ITextElement> spTextElement = spElement;
	CComQIPtr<ITextSymbol> spTextSymbol;
	spTextElement->get_Symbol(&spTextSymbol);
	CComBSTR bstrText;
	spTextElement->get_Text(&bstrText);
	int nTextLenth = bstrText.Length();
	CComQIPtr<IScreenDisplay> spScreenDisplay;
	spActiveView->get_ScreenDisplay(&spScreenDisplay);
	OLE_HANDLE hDC;
	spScreenDisplay->get_CacheMemDC(0,&hDC);
	CComQIPtr<ITransformation> spTransformation;
	CComQIPtr<IDisplayTransformation> spDisplayTransformation;
	spScreenDisplay->get_DisplayTransformation(&spDisplayTransformation);
	spTransformation = spDisplayTransformation;
	spTextSymbol->GetTextSize(hDC,spTransformation,bstrText,dbWidth,dbHeight);
	*dbWidth = *dbWidth / 72 * 2.54;
	*dbHeight = *dbHeight / 72 * 2.54;
	return S_OK;
}

HRESULT CExportThemeMap::GetFrameSize(CComQIPtr<IFeatureLayer> &spXZQFtLayer, double dbScale, double *dbWidth, double *dbHeight)
{
	//��ȡ���������µ���������Χ
	CComQIPtr<IFeatureCursor> spFtCursor;
	CComQIPtr<IFeature> spFt;
	CComQIPtr<IEnvelope> spXZQEnv;
	spXZQFtLayer->Search(NULL,VARIANT_FALSE,&spFtCursor);
	spFtCursor->NextFeature(&spFt);
	spXZQEnv.CoCreateInstance(CLSID_Envelope);
	while(spFt != NULL)
	{
		CComQIPtr<IGeometry> spTmpGeo;
		CComQIPtr<IEnvelope> spTmpEnv;
		spFt->get_Shape(&spTmpGeo);
		spTmpGeo->get_Envelope(&spTmpEnv);
		spXZQEnv->Union(spTmpEnv);
		spFt.Release();
		spFtCursor->NextFeature(&spFt);
	}
	//��Ϊ��������ϵƥ�䣬��������ϵ��ͬ�����
	double dbXZQWidth,dbXZQHeight;
	spXZQEnv->get_Width(&dbXZQWidth);
	spXZQEnv->get_Height(&dbXZQHeight);
	*dbHeight = dbXZQHeight * dbScale;
	*dbWidth = dbXZQWidth *dbScale;
	return S_OK;
}

HRESULT CExportThemeMap::GetSizeScale(CComQIPtr<IMapFrame> &spMapFrame, double *dbScale)
{

	double dbLayersEleHeight = 0.0,dbLayersEleWidth = 0.0;//��¼����������mapframe��С
	double dbXZQHeight = 0.0,dbXZQWidth = 0.0;//��¼������map��Χ��С
	CComQIPtr<IEnvelope> spLayersEnv;
	CComQIPtr<IGeometry> spEleGeo;
	//��ȡͼ���С
	CComQIPtr<IElement> spMapElement = spMapFrame;
	spMapElement->get_Geometry(&spEleGeo);
	spEleGeo->get_Envelope(&spLayersEnv);
	spLayersEnv->get_Width(&dbLayersEleWidth);
	spLayersEnv->get_Height(&dbLayersEleHeight);

	CComQIPtr<IMap> spMap;
	spMapFrame->get_Map(&spMap);
	CComQIPtr<IActiveView> spActiveView = spMap;
	CComQIPtr<IEnvelope> spMapEnv;
	double dbMapWidth,dbMapHeight;
	spActiveView->get_Extent(&spMapEnv);

	spMapEnv->get_Width(&dbMapWidth);
	*dbScale = dbLayersEleWidth/dbMapWidth;
	spMapEnv->get_Height(&dbMapHeight);
	*dbScale = dbLayersEleHeight/dbMapHeight;
	return S_OK;
}

HRESULT CExportThemeMap::GetDistance(CComQIPtr<IFeatureLayer> &spXZQFtLayer, CComQIPtr<IGeometry> spGeometry, double *dbDistance)
{
	CComQIPtr<IFeatureClass> spFtClass;
	spXZQFtLayer->get_FeatureClass(&spFtClass);
	if (spFtClass == NULL) return E_POINTER;

	CComQIPtr<IFeatureCursor> spFtCurSor;
	spXZQFtLayer->Search(NULL,VARIANT_FALSE,&spFtCurSor);
	CComQIPtr<IFeature> spXZFt;
	spFtCurSor->NextFeature(&spXZFt);

	CComQIPtr<IPolygon> spXZGeometry;
	spXZGeometry.CoCreateInstance(CLSID_Polygon);

	while(spXZFt != NULL)
	{
		CComQIPtr<IGeometry> spTmpGeo;
		CComQIPtr<IGeometry> spResGeo;
		spXZFt->get_Shape(&spTmpGeo);
		CComQIPtr<ITopologicalOperator> spTopo = spXZGeometry;
		spTopo->Union(spTmpGeo,&spResGeo);
		if (spResGeo != NULL)
		{
			spXZGeometry = spResGeo;
		}

		spXZFt.Release();
		spFtCurSor->NextFeature(&spXZFt);
	}

	CComQIPtr<IProximityOperator> spProximity = spXZGeometry;
	spProximity->ReturnDistance(spGeometry,dbDistance);

	return S_OK;
}

HRESULT CExportThemeMap::MoveLegendElement(CComQIPtr<IPageLayoutControl> spPageLayoutControl, CComQIPtr<IFeatureLayer> &spXZQXZFtLayer, CComQIPtr<IPoint> spActiveViewCenter)
{
	if (spPageLayoutControl == NULL || spXZQXZFtLayer == NULL)
	{
		return E_FAIL;
	}
	double dbPageWidth,dbPageHeight;
	CComQIPtr<IPage> spTmpPage;
	spPageLayoutControl->get_Page(&spTmpPage);
	spTmpPage->QuerySize(&dbPageWidth,&dbPageHeight);

	//ͼ�泤����ֽ�泤�ȱ�����ͼ�������
	double dbScale,nScale;
	//�����ͼ�򳤿�
	double dbNewFrameHeight,dbNewFrameWidth;
	CComQIPtr<IElement> spLayersElement;
	spPageLayoutControl->FindElementByName(_T("��ʾͼ"),1,&spLayersElement);
	if (spLayersElement == NULL)
	{
		return E_FAIL;
	}
	CComQIPtr<IMapFrame> spMapFrame = spLayersElement;
	CComQIPtr<IMap> spMap;
	spMapFrame->get_Map(&spMap);
	spMap->get_MapScale(&nScale);
	CComQIPtr<IActiveView> spActiveView = spMap;
	GetSizeScale(spMapFrame,&dbScale);

	//��ȡͼ���С
	CComQIPtr<IGeometry> spEleGeo;
	CComQIPtr<IEnvelope> spLayersEnv;
	spLayersElement->get_Geometry(&spEleGeo);
	spEleGeo->get_Envelope(&spLayersEnv);
	spLayersEnv->get_Width(&dbNewFrameWidth);
	spLayersEnv->get_Height(&dbNewFrameHeight);

	//��ͼ���Ľ�����
	double dbNewMapFrameMaxX,dbNewMapFrameMaxY,dbNewMapFrameMinX,dbNewMapFrameMinY;
	spLayersEnv->get_XMax(&dbNewMapFrameMaxX);
	spLayersEnv->get_XMin(&dbNewMapFrameMinX);
	spLayersEnv->get_YMax(&dbNewMapFrameMaxY);
	spLayersEnv->get_YMin(&dbNewMapFrameMinY);


	//��ȡ���������µ���������Χ
	double dbXZQWidth,dbXZQHeight;
	double dbLayersEleHeight,dbLayersEleWidth;

	CComQIPtr<IFeatureCursor> spFtCursor;
	CComQIPtr<IFeature> spFt;
	CComQIPtr<IEnvelope> spXZQEnv;
	spXZQXZFtLayer->Search(NULL,VARIANT_FALSE,&spFtCursor);
	spFtCursor->NextFeature(&spFt);
	spXZQEnv.CoCreateInstance(CLSID_Envelope);
	while(spFt != NULL)
	{
		CComQIPtr<IGeometry> spTmpGeo;
		CComQIPtr<IEnvelope> spTmpEnv;
		spFt->get_Shape(&spTmpGeo);
		spTmpGeo->get_Envelope(&spTmpEnv);
		spXZQEnv->Union(spTmpEnv);
		spFt.Release();
		spFtCursor->NextFeature(&spFt);
	}
	//��Ϊ��������ϵƥ�䣬��������ϵ��ͬ�����
	spXZQEnv->get_Width(&dbXZQWidth);
	spXZQEnv->get_Height(&dbXZQHeight);
	dbLayersEleHeight = dbXZQHeight * dbScale;
	dbLayersEleWidth = dbXZQWidth *dbScale;

	//ͼ���Ľ����꣬�����ж��ĽǵĿհ״�С
	double dbMapMaxX,dbMapMaxY,dbMapMinX,dbMapMinY;

	HRESULT hrLegendLeft,hrLegendRight;
	//��ȡͼ����map�еĳ���
	CComQIPtr<IElement> spLegendElement;
	CComQIPtr<IGeometry> spLegendGeo;
	CComQIPtr<IEnvelope> spLegendEnv;
	spPageLayoutControl->FindElementByName(_T("MAPLEGEND"),1,&spLegendElement);
	double dbLegendEleHeight,dbLegendEleWidth;
	double dbLegendMapHeight,dbLegendMapWidth;
	double dbLeftDistance,dbRightDistance;
	if (spLegendElement == NULL)
	{
		return E_FAIL;
	}
	spLegendElement->get_Geometry(&spLegendGeo);
	spLegendGeo->get_Envelope(&spLegendEnv);
	spLegendEnv->get_Height(&dbLegendEleHeight);
	spLegendEnv->get_Width(&dbLegendEleWidth);
	dbLegendMapHeight = dbLegendEleHeight / dbScale;
	dbLegendMapWidth = dbLegendEleWidth / dbScale;

	//��ȡ���ͼ�Ľ�����
	GetNewActiveViewPoint(&dbMapMaxX,&dbMapMaxY,&dbMapMinX,&dbMapMinY,spActiveView);
	CComQIPtr<IPoint> spLeftPoint;//����
	CComQIPtr<IPoint> spRightPoint;//�Ҳ��
	spLeftPoint.CoCreateInstance(CLSID_Point);
	spRightPoint.CoCreateInstance(CLSID_Point);
	spLeftPoint->put_X(dbMapMinX);
	spLeftPoint->put_Y(dbMapMinY);
	spRightPoint->put_X(dbMapMaxX);
	spRightPoint->put_Y(dbMapMinY);
	//���ݿհ״�Сȷ�����ı�
	GetDistance(spXZQXZFtLayer,(CComQIPtr<IGeometry>)spLeftPoint,&dbLeftDistance);
	GetDistance(spXZQXZFtLayer,(CComQIPtr<IGeometry>)spRightPoint,&dbRightDistance);

	//bIsPutLeft = ( dbLeftDistance > dbRightDistance ) ? TRUE:FALSE;

	//���ҷֿ��������Կհ׽϶��һ��������У��
	if (dbLeftDistance > dbRightDistance)//��߿յĶ࣬�����
	{
		//�ռ��ѯ��Ҫ��4������
		double dbLegendLeftMaxX,dbLegendLeftMaxY,dbLegendLeftMinX,dbLegendLeftMinY;
		GetPolygonPoint(spActiveView,dbLegendMapWidth,dbLegendMapHeight,LeftBotton, &dbLegendLeftMaxX,&dbLegendLeftMaxY,&dbLegendLeftMinX,&dbLegendLeftMinY);
		hrLegendLeft = CheckCover(spXZQXZFtLayer,dbLegendLeftMaxX,dbLegendLeftMaxY,dbLegendLeftMinX,dbLegendLeftMinY);

		while(hrLegendLeft == E_FAIL)
		{
			CComQIPtr<IPage> spPage;
			spPageLayoutControl->get_Page(&spPage);
			double dbCenterX,dbCenterY;
			spActiveViewCenter->get_X(&dbCenterX);
			spActiveViewCenter->get_Y(&dbCenterY);
			//��������ˣ�����ֽ��,��Ӧ������ͼ����ƫ��
			if (dbPageHeight > dbPageWidth)
			{
// 				if (dbPageWidth + 10.0 > dbMaxPageWidth)
// 				{
					dbNewFrameHeight += dbMoveLenth;
					dbPageHeight = dbNewFrameHeight + dbPageHead + dbPageButton;
					dbCenterY -= (dbMoveLenth/2) / dbScale;
// 				}else
// 				{
// 					dbNewFrameWidth += 10.0;
// 					dbPageWidth = dbNewFrameWidth + dbPage_LeftRight * 2;
// 					dbCenterX -= 5.0 / dbScale;
// 				}
			} 
			else
			{
// 				if (dbPageHeight + 10.0 > dbMaxPageWidth)
// 				{
					dbNewFrameWidth += dbMoveLenth;
					dbPageWidth = dbNewFrameWidth + dbPage_LeftRight * 2;
					dbCenterX -= (dbMoveLenth/2) / dbScale;
// 				}else
// 				{
// 					dbNewFrameHeight += 10.0;
// 					dbPageHeight = dbNewFrameHeight + dbPageHead + dbPageButton;		
// 					dbCenterY -= 5.0 / dbScale;
// 				}

			}
			//Ӧ�õ���
			spActiveViewCenter->put_X(dbCenterX);
			spActiveViewCenter->put_Y(dbCenterY);
			//����ֽ��
			spPage->PutCustomSize(dbPageWidth,dbPageHeight);
			//���¼���ͼ���Ľ�����
			dbNewMapFrameMinY = (dbPageHeight - dbNewFrameHeight) / 2;
			dbNewMapFrameMaxY = dbNewMapFrameMinY + dbNewFrameHeight;
			dbNewMapFrameMinX = (dbPageWidth - dbNewFrameWidth) / 2;
			dbNewMapFrameMaxX = dbNewMapFrameMinX + dbNewFrameWidth;

			CComQIPtr<IPolygon> spPolygon;
			spPolygon.CoCreateInstance(CLSID_Polygon);
			GetPolygonByPoint(dbNewMapFrameMaxX,dbNewMapFrameMaxY,dbNewMapFrameMinX,dbNewMapFrameMinY,spPolygon);
			spLayersElement->put_Geometry(spPolygon);
			//�ƶ���ͼ�����ĵ���
			spXZQEnv->CenterAt(spActiveViewCenter);
			spActiveView->put_Extent(spXZQEnv);
			//��Ϊ���ŵ�ͼ���ƻ�����ǧ�ı�������������һ�α���
			spMap->put_MapScale(nScale);
			//�ƶ���֮�����һ���ж�
			GetPolygonPoint(spActiveView,dbLegendMapWidth,dbLegendMapHeight,LeftBotton, &dbLegendLeftMaxX,&dbLegendLeftMaxY,&dbLegendLeftMinX,&dbLegendLeftMinY);
			hrLegendLeft = CheckCover(spXZQXZFtLayer,dbLegendLeftMaxX,dbLegendLeftMaxY,dbLegendLeftMinX,dbLegendLeftMinY);
		}
		//����ж�֮���ȡ���õ�λ��
		CComQIPtr<IPolygon> spLegendPolygon;
		spLegendPolygon.CoCreateInstance(CLSID_Polygon);
		GetPolygonByPoint(dbNewMapFrameMinX + dbLegendEleWidth,dbNewMapFrameMinY + dbLegendEleHeight,dbNewMapFrameMinX,dbNewMapFrameMinY,spLegendPolygon);
		//��ɵ���֮������Ԫ��λ��
		spLegendElement->put_Geometry(spLegendPolygon);
	}else//�ұ߿յĶ࣬���ұ� 
	{
		//�ռ��ѯ��Ҫ��4������
		double dbLegendRightMaxX,dbLegendRightMaxY,dbLegendRightMinX,dbLegendRightMinY;
		GetPolygonPoint(spActiveView,dbLegendMapWidth,dbLegendMapHeight,RightBotton, &dbLegendRightMaxX,&dbLegendRightMaxY,&dbLegendRightMinX,&dbLegendRightMinY);
		hrLegendRight = CheckCover(spXZQXZFtLayer,dbLegendRightMaxX,dbLegendRightMaxY,dbLegendRightMinX,dbLegendRightMinY);

		while(hrLegendRight == E_FAIL)
		{
			CComQIPtr<IPage> spPage;
			spPageLayoutControl->get_Page(&spPage);
			double dbCenterX,dbCenterY;
			spActiveViewCenter->get_X(&dbCenterX);
			spActiveViewCenter->get_Y(&dbCenterY);
			//��������ˣ�����ֽ��,��Ӧ������ͼ����ƫ��
			if (dbPageHeight > dbPageWidth)
			{
// 				if (dbPageWidth + 10.0 > dbMaxPageWidth)
// 				{
					dbNewFrameHeight += 10.0;
					dbPageHeight = dbNewFrameHeight + dbPageHead + dbPageButton;
					dbCenterY -= 5.0 / dbScale;
// 				}else
// 				{
// 					dbNewFrameWidth += 10.0;
// 					dbPageWidth = dbNewFrameWidth + dbPage_LeftRight * 2;
// 					dbCenterX += 5.0 / dbScale;
// 				}
			} 
			else
			{
// 				if (dbPageHeight + 10.0 > dbMaxPageWidth)
// 				{
					dbNewFrameWidth += 10.0;
					dbPageWidth = dbNewFrameWidth + dbPage_LeftRight * 2;
					dbCenterX += 5.0 / dbScale;
// 				}else
// 				{
// 					dbNewFrameHeight += 10.0;
// 					dbPageHeight = dbNewFrameHeight + dbPageHead + dbPageButton;		
// 					dbCenterY -= 5.0 / dbScale;
// 				}

			}
			//Ӧ�õ���
			spActiveViewCenter->put_X(dbCenterX);
			spActiveViewCenter->put_Y(dbCenterY);
			spPage->PutCustomSize(dbPageWidth,dbPageHeight);
			//���¼���ͼ���Ľ�����
			dbNewMapFrameMinY = (dbPageHeight - dbNewFrameHeight) / 2;
			dbNewMapFrameMaxY = dbNewMapFrameMinY + dbNewFrameHeight;
			dbNewMapFrameMinX = (dbPageWidth - dbNewFrameWidth) / 2;
			dbNewMapFrameMaxX = dbNewMapFrameMinX + dbNewFrameWidth;

			CComQIPtr<IPolygon> spPolygon;
			spPolygon.CoCreateInstance(CLSID_Polygon);
			GetPolygonByPoint(dbNewMapFrameMaxX,dbNewMapFrameMaxY,dbNewMapFrameMinX,dbNewMapFrameMinY,spPolygon);
			spLayersElement->put_Geometry(spPolygon);
			//�ƶ���ͼ�����ĵ���
			spXZQEnv->CenterAt(spActiveViewCenter);
			spActiveView->put_Extent(spXZQEnv);
			//��Ϊ���ŵ�ͼ���ƻ�����ǧ�ı�������������һ�α���
			spMap->put_MapScale(nScale);
			//ˢ����ͼ
			//ipPageLayoutControl->Refresh(esriViewAll);
			//�ƶ���֮�����һ���ж�
			GetPolygonPoint(spActiveView,dbLegendMapWidth,dbLegendMapHeight,RightBotton, &dbLegendRightMaxX,&dbLegendRightMaxY,&dbLegendRightMinX,&dbLegendRightMinY);
			hrLegendRight = CheckCover(spXZQXZFtLayer,dbLegendRightMaxX,dbLegendRightMaxY,dbLegendRightMinX,dbLegendRightMinY);
		}
		//����ж�֮���ȡ���õ�λ��
		CComQIPtr<IPolygon> spLegendPolygon;
		spLegendPolygon.CoCreateInstance(CLSID_Polygon);
		GetPolygonByPoint(dbNewMapFrameMaxX,dbNewMapFrameMinY + dbLegendEleHeight,dbNewMapFrameMaxX - dbLegendEleWidth,dbNewMapFrameMinY,spLegendPolygon);
		//��ɵ���֮������Ԫ��λ��
		spLegendElement->put_Geometry(spLegendPolygon);
	}
	spPageLayoutControl->Refresh(esriViewAll);
	return S_OK;
}

void CExportThemeMap::SetFullExtent(CComQIPtr<IEnvelope> &spEnvelope, BOOL bIsJBT)
{

	CComQIPtr<IZoomToTopXZQ> spZoomTo;
	spZoomTo.CoCreateInstance(CLSID_ZoomToTopXZQ);
	spZoomTo->SetIsLayoutJBT(bIsJBT);
	IUnknown* pUnknown;
	if (spEnvelope!= NULL)
	{
		spEnvelope->QueryInterface(&pUnknown);
		spZoomTo->SetFullExtent(pUnknown);
	}
}
