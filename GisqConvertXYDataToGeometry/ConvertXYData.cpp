// ConvertXYData.cpp : CConvertXYData 的实现

#include "stdafx.h"
#include "ConvertXYData.h"

//#define DATASETNAME _T("dt")

// CConvertXYData

STDMETHODIMP CConvertXYData::GetWorkspace(IUnknown **ipWorkspace)
{
	*ipWorkspace = m_spWorkspace;
	(*ipWorkspace)->AddRef();
	return S_OK;
}

STDMETHODIMP CConvertXYData::GetDataset(IUnknown **ipDataset)
{
	*ipDataset = m_spFtDataset;
	(*ipDataset)->AddRef();
	return S_OK;
}

STDMETHODIMP CConvertXYData::GetFeatureClass(IUnknown **ipFeatureClass)
{
	*ipFeatureClass = m_spFeatureClass;
	(*ipFeatureClass)->AddRef();
	return S_OK;
}

STDMETHODIMP CConvertXYData::GetSpatialRefrence(IUnknown **ipSpatialRefrence)
{
	*ipSpatialRefrence = m_spSpatialRederence;
	(*ipSpatialRefrence)->AddRef();
	return S_OK;
}

STDMETHODIMP CConvertXYData::ConvertToFeatureClass(BSTR bstrTXTFilePath, BSTR bstrDestFolder, BSTR bstrDestName, BSTR bstrDatasetName, BSTR bstrFeatureClassName)
{
	HRESULT hr;
	//先根据给定路径创建GDB	
	if (m_spWorkspace != NULL)
	{
		m_spWorkspace.Release();
	}
	CComQIPtr<IWorkspaceName> spWorkspaceName;
	hr = CGeometryOperation::CreateWorkspace(enmWSTGdb,bstrDestFolder,bstrDestName,spWorkspaceName);
	if (spWorkspaceName == NULL) return hr;//创建GDB失败
	spWorkspaceName->get_PathName(&bstrDestFolder);
	hr = CGeometryOperation::OpenWorkspaceFromFile(enmWSTGdb,bstrDestFolder,m_spWorkspace);
	if (hr != S_OK) return hr; //如果打开失败，就终止

	//然后是读取TXT文件的内容
	CArray<CString,CString> strCoordArray;
	ReadCoordFile(bstrTXTFilePath,&strCoordArray);
	if (strCoordArray.GetCount() == 0)
	{
		return E_FAIL;//说明界址点文件无法识别
	}
	//读取带号，创建空间参考
	int nProjCS = GetSpatialNum(strCoordArray.GetAt(13).AllocSysString());
	if (nProjCS == -1) return E_FAIL;//如果X坐标位数小于8位
	CreateSpatial(nProjCS);
	if (m_spSpatialRederence == NULL) return E_FAIL;//创建空间参考失败

	//然后是创建Dataset
	CComQIPtr<IFeatureWorkspace> spFtWorkspace;
	spFtWorkspace = m_spWorkspace;
	//edit by lcc 2016.12.01添加自定义dataset名称
	CGeometryOperation::CreateFeatureDataset(spFtWorkspace,m_spSpatialRederence,bstrDatasetName,m_spFtDataset);
	if (m_spFtDataset == NULL) return E_FAIL;//创建Dataset失败

	//edit by lcc 2016.12.01添加自定义featureclass名称
	//在Dataset中创建FeatureClass
	CreatFeatureClass(bstrFeatureClassName);
	//edit end 2016.12.01

	//组织图形，如果图形不为空，创建Feature并写入图形
	hr = ConvertCoordToFeature(m_spFeatureClass,strCoordArray);
	if (hr != S_OK) return hr;
	return S_OK;
}

int CConvertXYData::GetSpatialNum(BSTR strCoord)
{
	int Unused1 = 0;
	int Unused2 = 0;
	double dbX = 0;
	double dbY = 0;
	swscanf_s(strCoord,_T("J%d,%d,%lf,%lf"),&Unused1,&Unused2,&dbY,&dbX);
	if (dbX < 10000000) return -1;
	return dbX/1000000;	
}

void CConvertXYData::CreateSpatial(int nProjCS)
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

void CConvertXYData::CreatFeatureClass(BSTR bstrFeatureClassName)
{
	//设置创建类型为要素集
	CComQIPtr<IUID> spUID;
	spUID.CoCreateInstance(CLSID_UID);
	spUID->put_Value(_variant_t("esriGeoDatabase.Feature"));

	CComQIPtr<IObjectClassDescription> spObjectClassDescription;
	spObjectClassDescription.CoCreateInstance(CLSID_FeatureClassDescription);
	CComQIPtr<IFields> spFields;
	CComQIPtr<IFieldsEdit> spFieldsEdit;
	spObjectClassDescription->get_RequiredFields(&spFields);
	spFieldsEdit = spFields;

	CComQIPtr<IField> spField;
	CComQIPtr<IFieldEdit> spFieldEdit;

	spFieldsEdit->get_Field(1,&spField);
	spFieldEdit = spField;
	CComQIPtr<IGeometryDef> spGeometryDef;
	spFieldEdit->get_GeometryDef(&spGeometryDef);
	CComQIPtr<IGeometryDefEdit> spGeometryDefEdit = spGeometryDef;
	spGeometryDefEdit->put_GeometryType(esriGeometryPolygon);
	spField.Release();
	spFieldEdit.Release();

	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_T("XZQMC"));
	spFieldEdit->put_AliasName(_T("行政区名称"));
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldEdit->put_Length(100);
	spFieldEdit->put_DefaultValue(_variant_t(_T("")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();

	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_T("XZQDM"));
	spFieldEdit->put_AliasName(_T("行政区代码"));
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldEdit->put_Length(12);
	spFieldEdit->put_DefaultValue(_variant_t(_T("")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();

	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_T("DKBH"));
	spFieldEdit->put_AliasName(_T("地块编号"));
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldEdit->put_Length(30);
	spFieldEdit->put_DefaultValue(_variant_t(_T("")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();

	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_T("DKMC"));
	spFieldEdit->put_AliasName(_T("地块名称"));
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldEdit->put_Length(100);
	spFieldEdit->put_DefaultValue(_variant_t(_T("")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();
	//add by lcc 2016.12.05 添加规划用途字段
	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	/*spFieldEdit->put_Name(_T("GHYT"));
	spFieldEdit->put_AliasName(_T("规划用途"));*/
	spFieldEdit->put_Name(_T("GHYTDM"));
	spFieldEdit->put_AliasName(_T("规划用途代码"));
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldEdit->put_Length(20);
	spFieldEdit->put_DefaultValue(_variant_t(_T("")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();
	//add end 2016.12.05 添加规划用途字段

	HRESULT hr = m_spFtDataset->CreateFeatureClass(bstrFeatureClassName,spFields,spUID,NULL,esriFTSimple,_T("Shape"),NULL,&m_spFeatureClass);
}

HRESULT CConvertXYData::ConvertCoordToFeature(CComQIPtr<IFeatureClass> spFtClass,CArray<CString,CString> &arstrTmp)
{
	HRESULT hr;
	CComQIPtr<IFeature> spFt;
	CComQIPtr<IPolygon> spPolyGon;
	CComQIPtr<IGeometry> spGeometry;
	CComQIPtr<IPointCollection> spPointCollection;
	CComQIPtr<IGeometryCollection> spGeometryCollection;

	//转化为图形
	int nStopRingID = 0;
	//部坐标的前12行没有用，这里从第13行开始读
	for (int i = 12; i < arstrTmp.GetCount(); i++)
	{
		LPCWSTR readtmp;
		readtmp = arstrTmp.GetAt(i).GetString();
		TRACE(readtmp);
		TRACE(_T("\n"));
		if (arstrTmp.GetAt(i).Find(_T("@")) > 0)//一个要素开始
		{
			if (spPointCollection != NULL)//说明之前有一个要素或圈需要保存
			{
				if (nStopRingID = 1)//之前的要素只有一个圈,保存这个要素
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

					SaveFeature(spGeometry,spFt);

				}else if (nStopRingID > 1)//这是上个要素的一个内切割多边形,先切割,再保存
				{
					hr = PolygonCutHollow(spPointCollection,spGeometry);
					if (hr != S_OK) return hr;
					SaveFeature(spGeometry,spFt);
				}				
			}

			//创建要素，写入属性行信息	
			if (spFt != NULL) spFt.Release();
			spFtClass->CreateFeature(&spFt);
			AddFieldsToFeature(arstrTmp.GetAt(i),spFt);
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
		}else//不是起始行，那这里处理数据，写入点集		
		{
			LPCWSTR ptmp;
			ptmp = arstrTmp.GetAt(i).GetString();
			TRACE(ptmp);
			int nPointID = 0;
			int nRingID = 0;
			double x = 0;
			double y = 0;
			//这里格式化了一行的数据
			swscanf_s(ptmp,_T("J%d,%d,%lf,%lf"),&nPointID,&nRingID,&y,&x);
			if (nPointID == 1 && i > arstrTmp.GetCount() -4)//到底了
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
				break;

			}else if (nPointID == 1 && nRingID > nStopRingID)//一个要素的下一个圈开始
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
				spPointCollection.Release();
				spPointCollection.CoCreateInstance(CLSID_Multipoint);

			}else//就是一个普通的点
			{
				CComQIPtr<IPoint> spPoint;
				spPoint.CoCreateInstance(CLSID_Point);
				spPoint->PutCoords(x,y);
				spPointCollection->AddPoint(spPoint);
			}			
		}
	}

	return S_OK;
}

HRESULT CConvertXYData::PolygonCutHollow(CComQIPtr<IPointCollection> spPointCollection, CComQIPtr<IGeometry> spGeometry)
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
	spGeometryCollection->AddGeometry(spRing);
	spPolyGon->Close();

	VARIANT_BOOL bIsEmpty;
	spPolyGon->get_IsEmpty(&bIsEmpty);

	CComQIPtr<ITopologicalOperator> spTopOper;
	spTopOper = spGeometry;
	CComQIPtr<IGeometry> ipResultGeo;
	spTopOper->Difference(spPolyGon, &ipResultGeo);

	ipResultGeo->get_IsEmpty(&bIsEmpty);
	if (bIsEmpty == VARIANT_FALSE)
	{
		spGeometry = ipResultGeo;
	}

	return S_OK;
}

HRESULT CConvertXYData::SaveFeature(CComQIPtr<IGeometry> spGeometry, CComQIPtr<IFeature> &spFt)
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
	hr = spGeometry->putref_SpatialReference(ipSpat);

	VARIANT_BOOL bIsEmpty;
	spGeometry->get_IsEmpty(&bIsEmpty);

	//只有当图形不为空时，赋值						
	hr = spFt->putref_Shape(spGeometry);
	hr = spFt->Store();	

	return S_OK;
}

HRESULT CConvertXYData::AddFieldsToFeature(CString &strFields, CComQIPtr<IFeature> spFt)
{
	if (spFt == NULL) return E_FAIL;
	
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
	strarrFields.Add(strFields);

	CComQIPtr<IFields> spFields;
	spFt->get_Fields(&spFields);
	long nIndexField;

	spFields->FindField(_T("XZQDM"),&nIndexField);
	spFt->put_Value(nIndexField,_variant_t(strarrFields.GetAt(2).AllocSysString()));

	spFields->FindField(_T("XZQMC"),&nIndexField);
	spFt->put_Value(nIndexField,_variant_t(strarrFields.GetAt(3).AllocSysString()));

	spFields->FindField(_T("DKBH"),&nIndexField);
	spFt->put_Value(nIndexField,_variant_t(strarrFields.GetAt(4).AllocSysString()));
	
	spFields->FindField(_T("DKMC"),&nIndexField);
	spFt->put_Value(nIndexField,_variant_t(strarrFields.GetAt(5).AllocSysString()));
	//add by lcc 2016.12.05 添加规划用途字段
	//spFields->FindField(_T("GHYT"),&nIndexField);
	spFields->FindField(_T("GHYTDM"),&nIndexField);
	spFt->put_Value(nIndexField,_variant_t(strarrFields.GetAt(7).AllocSysString()));
	//add end 2016.12.01
	spFt->Store();
	return S_OK;
}

HRESULT CConvertXYData::GetRing(CComQIPtr<IPointCollection> spPointCollection,CComQIPtr<IRing> &spRing)
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

FileCodeType  CConvertXYData::CheckFile(CFile *pCoordFile)
{

	wchar_t Unicodetest[1];
	pCoordFile->Read(Unicodetest,2);
	if (Unicodetest[0] == 0xFEFF)
	{
		return unicode;
	}
	pCoordFile->SeekToBegin();

	char Utf8test[3];
	pCoordFile->Read(Utf8test,3);
	if (Utf8test[0] == 0xEF && Utf8test[1] == 0xBB && Utf8test[2] == 0xBF)
	{
		return utf8;
	}
	pCoordFile->SeekToBegin();

	return asni;
}

BOOL CConvertXYData::ReadCoordFile(BSTR bstrTXTFilePath,CArray<CString,CString> *arstrTmp)
{
	USES_CONVERSION;

	CFile CoordFile;
	CoordFile.Open(bstrTXTFilePath,CFile::modeReadWrite);
	FileCodeType fType;
	fType = CheckFile(&CoordFile);
	if (fType == unkown)
	{
		//BCGPMessageBox(_T("未知文件编码类型"),_T("错误"),MB_OK);
		return FALSE;
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
		return TRUE;
	}

	return TRUE;
}
//add by lcc 2016.12.13审批系统导出的MDB解析接口
STDMETHODIMP CConvertXYData::ConvertMDBToFeatureClass(BSTR bstrMDBFilePath, BSTR bstrDestFolder, BSTR bstrDestName, BSTR bstrDatasetName, BSTR bstrFeatureClassName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	HRESULT hr;
	//先根据给定路径创建GDB	
	if (m_spWorkspace != NULL)
	{
		m_spWorkspace.Release();
	}
	CComQIPtr<IWorkspaceName> spWorkspaceName;
	hr = CGeometryOperation::CreateWorkspace(enmWSTGdb,bstrDestFolder,bstrDestName,spWorkspaceName);
	if (spWorkspaceName == NULL) return hr;//创建GDB失败
	spWorkspaceName->get_PathName(&bstrDestFolder);
	hr = CGeometryOperation::OpenWorkspaceFromFile(enmWSTGdb,bstrDestFolder,m_spWorkspace);
	if (hr != S_OK) return hr; //如果打开失败，就终止

	//然后是读取MDB文件的内容
	CArray<CString,CString> strCoordArray;
	ReadMDBFile(bstrMDBFilePath,&strCoordArray);
	if (strCoordArray.GetCount() == 0)
	{
		return E_FAIL;//说明界址点文件无法识别
	}
	//读取带号，创建空间参考
	int nProjCS = GetSpatialNum(_bstr_t(strCoordArray.GetAt(1)));
	if (nProjCS == -1) return E_FAIL;//如果X坐标位数小于8位
	CreateSpatial(nProjCS);
	if (m_spSpatialRederence == NULL) return E_FAIL;//创建空间参考失败

	//然后是创建Dataset
	CComQIPtr<IFeatureWorkspace> spFtWorkspace;
	spFtWorkspace = m_spWorkspace;
	//edit by lcc 2016.12.01添加自定义dataset名称
	CGeometryOperation::CreateFeatureDataset(spFtWorkspace,m_spSpatialRederence,bstrDatasetName,m_spFtDataset);
	if (m_spFtDataset == NULL) return E_FAIL;//创建Dataset失败

	//edit by lcc 2016.12.01添加自定义featureclass名称
	//在Dataset中创建FeatureClass
	CreatFeatureClass2(bstrFeatureClassName);
	//edit end 2016.12.01

	//组织图形，如果图形不为空，创建Feature并写入图形
	hr = ConvertMDBCoordToFeature(m_spFeatureClass,strCoordArray);

// 	CComBSTR combstrFtClassName(bstrFeatureClassName);
// 	CComBSTR bstrOIDName;
// 	m_spFeatureClass->get_OIDFieldName(&bstrOIDName);
// 	CComBSTR bstrSQL(_T("UPDATE "));
// 	bstrSQL += combstrFtClassName;
// 	bstrSQL += _T(" set DKID = ");
// 	bstrSQL += bstrOIDName;
// 	hr = m_spWorkspace->ExecuteSQL(bstrSQL);
// 
// 	long nIndex = -1;
// 	m_spFeatureClass->FindField(CComBSTR(_T("XZQMC")),&nIndex);
//  	bstrSQL = (_T("UPDATE "));
//  	bstrSQL += combstrFtClassName;
//  	bstrSQL += _T(" set DKMC = XZQMC");
//  	hr = m_spWorkspace->ExecuteSQL(bstrSQL);
	
	if (hr != S_OK) return hr;
	return S_OK;

	return S_OK;
}

void CConvertXYData::CreatFeatureClass2(BSTR bstrFeatureClassName)
{
	//设置创建类型为要素集
	CComQIPtr<IUID> spUID;
	spUID.CoCreateInstance(CLSID_UID);
	spUID->put_Value(_variant_t("esriGeoDatabase.Feature"));

	CComQIPtr<IObjectClassDescription> spObjectClassDescription;
	spObjectClassDescription.CoCreateInstance(CLSID_FeatureClassDescription);
	CComQIPtr<IFields> spFields;
	CComQIPtr<IFieldsEdit> spFieldsEdit;
	spObjectClassDescription->get_RequiredFields(&spFields);
	spFieldsEdit = spFields;

	CComQIPtr<IField> spField;
	CComQIPtr<IFieldEdit> spFieldEdit;

	spFieldsEdit->get_Field(1,&spField);
	spFieldEdit = spField;
	CComQIPtr<IGeometryDef> spGeometryDef;
	spFieldEdit->get_GeometryDef(&spGeometryDef);
	CComQIPtr<IGeometryDefEdit> spGeometryDefEdit = spGeometryDef;
	spGeometryDefEdit->put_GeometryType(esriGeometryPolygon);
	spField.Release();
	spFieldEdit.Release();

	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_T("DK_GUID"));
	spFieldEdit->put_Type(esriFieldTypeString);
	//spFieldEdit->put_Length();
	spFieldEdit->put_DefaultValue(_variant_t(_T(" ")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();

	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_T("XZQMC"));
	spFieldEdit->put_Type(esriFieldTypeString);
	//spFieldEdit->put_Length(100);
	spFieldEdit->put_DefaultValue(_variant_t(_T(" ")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();

	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_T("XZDM"));
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldEdit->put_Length(12);
	spFieldEdit->put_DefaultValue(_variant_t(_T(" ")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();

	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_T("YTDKBH_H"));
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldEdit->put_Length(30);
	spFieldEdit->put_DefaultValue(_variant_t(_T(" ")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();

	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_T("YGHYTDM"));
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldEdit->put_Length(10);
	spFieldEdit->put_DefaultValue(_variant_t(_T(" ")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();

	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_T("YYTDKBH"));
	spFieldEdit->put_Type(esriFieldTypeString);
	//spFieldEdit->put_Length();
	spFieldEdit->put_DefaultValue(_variant_t(_T(" ")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();
// 	//添加DKID字段
// 	spField.CoCreateInstance(CLSID_Field);
// 	spFieldEdit = spField;
// 	spFieldEdit->put_Name(_T("DKID"));
// 	spFieldEdit->put_Type(esriFieldTypeInteger);
// 	spFieldEdit->put_DefaultValue(_variant_t(_T("")));
// 	spFieldEdit->put_IsNullable(VARIANT_TRUE);
// 	spFieldsEdit->AddField(spField);
// 	spField.Release();
// 	spFieldEdit.Release();
// 
// 	//添加DKMC字段
// 	spField.CoCreateInstance(CLSID_Field);
// 	spFieldEdit = spField;
// 	spFieldEdit->put_Name(_T("DKMC"));
// 	spFieldEdit->put_Type(esriFieldTypeString);
// 	spFieldEdit->put_DefaultValue(_variant_t(_T("")));
// 	spFieldEdit->put_IsNullable(VARIANT_TRUE);
// 	spFieldsEdit->AddField(spField);
// 	spField.Release();
// 	spFieldEdit.Release();

	//添加GHYTDM字段2016.12.16
	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_T("GHYTDM_H"));
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldEdit->put_DefaultValue(_variant_t(_T(" ")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();

	//add by lcc 2016.12.21添加两个字段
	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_T("XMBSM"));
	spFieldEdit->put_Length(50);
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldEdit->put_DefaultValue(_variant_t(_T(" ")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();

	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_T("FABSM"));
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldEdit->put_Length(50);
	spFieldEdit->put_DefaultValue(_variant_t(_T(" ")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();

	HRESULT hr = m_spFtDataset->CreateFeatureClass(bstrFeatureClassName,spFields,spUID,NULL,esriFTSimple,_T("Shape"),NULL,&m_spFeatureClass);
}

HRESULT CConvertXYData::ReadMDBFile(BSTR bstrMDBFilePath,CArray<CString,CString> *arstrTmp)
{
	CString strtmptest = _T("");
	 HRESULT hr;
	_ConnectionPtr ipADOConn;
	_RecordsetPtr pRst; 
	hr = ipADOConn.CreateInstance(__uuidof(Connection));
	hr = pRst.CreateInstance(__uuidof(Recordset));
	CString connStr=_T("");  
	connStr.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s"), bstrMDBFilePath);
	hr = ipADOConn->Open(_bstr_t(connStr), _T(""), _T(""), adConnectUnspecified);


	CString strSQL = _T("select DK_GUID,DKH,QH,DH,X,Y from KCDJ_JZD order by DKH,QH,CLNG(Right(DH, Len(DH) - 1))");
	hr = pRst->Open(_variant_t(_bstr_t(strSQL)),_variant_t(ipADOConn.GetInterfacePtr()), adOpenStatic,adLockOptimistic,adCmdText);

	if (hr == S_OK)
	{
		m_ipADOConn = ipADOConn;
	}
	long nCount;
	VARIANT vt;
	hr = pRst->get_RecordCount(&nCount);
	hr = pRst->GetRows(nCount,vtMissing,vtMissing,&vt);

	void *pvd = NULL;
	SafeArrayAccessData(vt.parray,&pvd);
	LONG nFieldNum = 0;
	SafeArrayGetUBound(vt.parray,(UINT)1,&nFieldNum);

	CString strDK_GUID = _T("");
	CString strEndLine= _T("");
	int nDKBH = 0;

	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{	
		//按行解析
		BSTR bstrDK_GUID;
		int nDKH;
		int nQH;
		BSTR bstrDH;
		double x;
		double y;
		for (LONG nField = 0; nField <= nFieldNum; nField++)
		{
			VARIANT vtTmp = ((VARIANT*)pvd)[nField+(nIndex)*(nFieldNum + 1)];
			switch (nField)
			{
			case 0://DK_GUID
				bstrDK_GUID = vtTmp.bstrVal;
				break;
			case 1://DKH
				nDKH = vtTmp.lVal;
				break;
			case 2://QH
				nQH = vtTmp.lVal;
				break;
			case 3://DH
				bstrDH = vtTmp.bstrVal;
				break;
			case 4://X
				x = vtTmp.dblVal;
			case 5://Y
				y = vtTmp.dblVal;
			default:
				break;
			}
		}
		//拼接成国标
		CString strLine;
		strLine.Format(_T("%s,%d,%lf,%lf"),bstrDH,nQH,x,y);
		if (nDKH != nDKBH)//如果不相等,说明当前行是一个新元素的开始,插入相关属性
		{
			if (nDKBH != 0)
			{
				arstrTmp->Add(strEndLine);
				strtmptest += strEndLine;
				strtmptest += _T("\r\n");
			}
			strDK_GUID.Format(_T("%s"),bstrDK_GUID);
			strDK_GUID.Replace(_T("$"),_T(","));
			CString strDKInfo;
			strDKInfo = _T("0,0,") + strDK_GUID + _T(",,面,,,,@");
			arstrTmp->Add(strDKInfo);
			strtmptest += strDKInfo;
			strtmptest += _T("\r\n");
			nDKBH = nDKH;
			strEndLine = strLine;
		}else
		{
			arstrTmp->Add(strLine);
			strtmptest += strLine;
			strtmptest += _T("\r\n");
		}

	}
	//加上最后一个点
	arstrTmp->Add(strEndLine);
	strtmptest += strEndLine;
	strtmptest += _T("\r\n");
	return S_OK;
}
HRESULT CConvertXYData::ConvertMDBCoordToFeature(CComQIPtr<IFeatureClass> spFtClass, CArray<CString,CString> &arstrTmp)
{
	HRESULT hr;
	CComQIPtr<IFeature> spFt;
	CComQIPtr<IPolygon> spPolyGon;
	CComQIPtr<IGeometry> spGeometry;
	CComQIPtr<IPointCollection> spPointCollection;
	CComQIPtr<IGeometryCollection> spGeometryCollection;

	//转化为图形
	int nStopRingID = 0;
	int nStartPointID = 1;
	for (int i = 0; i < arstrTmp.GetCount(); i++)
	{
		LPCWSTR readtmp;
		readtmp = arstrTmp.GetAt(i).GetString();
		TRACE(readtmp);
		TRACE(_T("\n"));
		if (arstrTmp.GetAt(i).Find(_T("@")) > 0)//一个要素开始
		{
			if (spPointCollection != NULL)//说明之前有一个要素或圈需要保存
			{
				if (nStopRingID = 1)//之前的要素只有一个圈,保存这个要素
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

					SaveFeature(spGeometry,spFt);

				}else if (nStopRingID > 1)//这是上个要素的一个内切割多边形,先切割,再保存
				{
					hr = PolygonCutHollow(spPointCollection,spGeometry);
					if (hr != S_OK) return hr;
					SaveFeature(spGeometry,spFt);
				}				
			}

			//创建要素，写入属性行信息	
			if (spFt != NULL) spFt.Release();
			spFtClass->CreateFeature(&spFt);
			AddFieldsToFeature2(arstrTmp.GetAt(i),spFt);
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
		}else//不是起始行，那这里处理数据，写入点集		
		{
			LPCWSTR ptmp;
			ptmp = arstrTmp.GetAt(i).GetString();
			//TRACE(ptmp);
			int nPointID = 0;
			int nRingID = 0;
			double x = 0;
			double y = 0;
			//这里格式化了一行的数据
			swscanf_s(ptmp,_T("J%d,%d,%lf,%lf"),&nPointID,&nRingID,&y,&x);
			if (nPointID == nStartPointID && i > arstrTmp.GetCount() -4)//到底了
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

	return S_OK;
}

HRESULT CConvertXYData::AddFieldsToFeature2(CString &strFields, CComQIPtr<IFeature> spFt)
{
	if (spFt == NULL) return E_FAIL;

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
	strarrFields.Add(strFields);

	CComQIPtr<IFields> spFields;
	spFt->get_Fields(&spFields);
	long nIndexField;

	spFields->FindField(_T("XZDM"),&nIndexField);
	spFt->put_Value(nIndexField,_variant_t(strarrFields.GetAt(2)));

	spFields->FindField(_T("XZQMC"),&nIndexField);
	spFt->put_Value(nIndexField,_variant_t(strarrFields.GetAt(3)));

// 	spFields->FindField(_T("DKMC"),&nIndexField);
// 	spFt->put_Value(nIndexField,_variant_t(strarrFields.GetAt(3)));

	spFields->FindField(_T("YTDKBH_H"),&nIndexField);
	spFt->put_Value(nIndexField,_variant_t(strarrFields.GetAt(4)));

	//加地块GUID
	spFields->FindField(_T("DK_GUID"),&nIndexField);
	CString strDK_GUID;
	strDK_GUID.Format(_T("%s$%s$%s"),strarrFields.GetAt(2),strarrFields.GetAt(3),strarrFields.GetAt(4));
	spFt->put_Value(nIndexField,_variant_t(_bstr_t(strDK_GUID)));

	spFt->Store();
	return S_OK;
}
//add by lcc 2016.12.13 end
HRESULT CConvertXYData::ConvertJZDToPolygon( BSTR bstrMDBFilePath,
											  BSTR bstrJZDTableName,
											  BSTR bstrGHGXDKTableName,
											  BSTR bstrTargetFolder,
											  BSTR bstrGDBName, 
											  BSTR bstrFeatureDatasetName,
											  BSTR bstrPolygonName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	HRESULT hr;
	//先根据给定路径创建GDB	
	if (m_spWorkspace != NULL)
	{
		m_spWorkspace.Release();
	}
	if (m_ipADOConn != NULL)
	{
		long plObjState;
		m_ipADOConn->get_State(&plObjState);
		if(plObjState)
			m_ipADOConn->Close();
		m_ipADOConn.Release();
	}
	CComQIPtr<IWorkspaceName> spWorkspaceName;
	hr = CGeometryOperation::CreateWorkspace(enmWSTGdb,bstrTargetFolder,bstrGDBName,spWorkspaceName);
	if (spWorkspaceName == NULL) return hr;//创建GDB失败
	spWorkspaceName->get_PathName(&bstrTargetFolder);
	hr = CGeometryOperation::OpenWorkspaceFromFile(enmWSTGdb,bstrTargetFolder,m_spWorkspace);
	if (hr != S_OK) return hr; //如果打开失败，就终止

	//然后是读取MDB文件的内容
	CArray<CString,CString> strCoordArray;
	hr = ReadMDBFile2(bstrMDBFilePath,bstrJZDTableName,&strCoordArray);
	if (hr != S_OK) 
	{
		CString strErr;
		strErr.Format(_T("%d"),hr);
		//BCGPMessageBox(_T("读取界址点时错误的返回"),strErr,MB_OK);
		return hr;//读取界址点时错误的返回
	}
	if (strCoordArray.GetCount() == 0)
	{
		CString strErr;
		strErr.Format(_T("%d"),hr);
		//BCGPMessageBox(_T("说明界址点文件无法识别"),strErr,MB_OK);
		return E_FAIL;//说明界址点文件无法识别
	}
	//读取带号，创建空间参考
	int nProjCS = GetSpatialNum(_bstr_t(strCoordArray.GetAt(1)));
	if (nProjCS == -1) return E_FAIL;//如果X坐标位数小于8位
	CreateSpatial(nProjCS);
	if (m_spSpatialRederence == NULL)
	{
		//BCGPMessageBox(_T("创建空间参考失败"),_T(""),MB_OK);
		return E_FAIL;//创建空间参考失败
	}

	//然后是创建Dataset
	CComQIPtr<IFeatureWorkspace> spFtWorkspace;
	spFtWorkspace = m_spWorkspace;
	//edit by lcc 2016.12.01添加自定义dataset名称
	hr = CGeometryOperation::CreateFeatureDataset(spFtWorkspace,m_spSpatialRederence,bstrFeatureDatasetName,m_spFtDataset);
	if (hr != S_OK) 
	{
		CString strErr;
		strErr.Format(_T("%d"),hr);
		//BCGPMessageBox(_T("创建Dataset失败"),strErr,MB_OK);
		return E_FAIL;//创建Dataset失败
	}

	//edit by lcc 2016.12.01添加自定义featureclass名称
	//在Dataset中创建FeatureClass
	CreatFeatureClass2(bstrPolygonName);
	//edit end 2016.12.01

	//组织图形，如果图形不为空，创建Feature并写入图形
	hr = ConvertMDBCoordToFeature(m_spFeatureClass,strCoordArray);
	if (hr != S_OK) return hr;
	//查询和匹配后规划用途代码字段
	hr = AddGHYTDMToFeature(bstrGHGXDKTableName);

	return hr;
}

HRESULT CConvertXYData::ReadMDBFile2(BSTR bstrMDBFilePath, BSTR bstrJZDTableName, CArray<CString,CString> *arstrTmp)
{
	CString strtmptest = _T("");
	HRESULT hr;
	_ConnectionPtr ipADOConn;
	_RecordsetPtr pRst; 
	hr = ipADOConn.CreateInstance(__uuidof(Connection));
	hr = pRst.CreateInstance(__uuidof(Recordset));
	CString connStr=_T("");  
	connStr.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s"), bstrMDBFilePath);
	hr = ipADOConn->Open(_bstr_t(connStr), _T(""), _T(""), adConnectUnspecified);

	CString strSQL;
	strSQL.Format(_T("select DK_GUID,DKH,QH,DH,X,Y from %s order by DKH,QH,CLNG(Right(DH, Len(DH) - 1))"),bstrJZDTableName);
	hr = pRst->Open(_variant_t(_bstr_t(strSQL)),_variant_t(ipADOConn.GetInterfacePtr()), adOpenStatic,adLockOptimistic,adCmdText);

	if (hr != S_OK) return hr;
	m_ipADOConn = ipADOConn;

	long nCount;
	VARIANT vt;
	hr = pRst->get_RecordCount(&nCount);
	hr = pRst->GetRows(nCount,vtMissing,vtMissing,&vt);
	if (hr != S_OK)
	{
		return hr;
	}

	void *pvd = NULL;
	SafeArrayAccessData(vt.parray,&pvd);
	LONG nFieldNum = 0;
	SafeArrayGetUBound(vt.parray,(UINT)1,&nFieldNum);

	CString strDK_GUID = _T("");
	CString strEndLine= _T("");
	int nDKBH = 0;
	int nDKQH = 1;

	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{	
		//按行解析
		BSTR bstrDK_GUID;
		int nDKH;
		int nQH;
		BSTR bstrDH;
		double x;
		double y;
		for (LONG nField = 0; nField <= nFieldNum; nField++)
		{
			VARIANT vtTmp = ((VARIANT*)pvd)[nField+(nIndex)*(nFieldNum + 1)];
			switch (nField)
			{
			case 0://DK_GUID
				bstrDK_GUID = vtTmp.bstrVal;
				break;
			case 1://DKH
				nDKH = vtTmp.lVal;
				break;
			case 2://QH
				nQH = vtTmp.lVal;
				break;
			case 3://DH
				bstrDH = vtTmp.bstrVal;
				break;
			case 4://X
				x = vtTmp.dblVal;
			case 5://Y
				y = vtTmp.dblVal;
			default:
				break;
			}
		}
		//拼接成国标
		CString strLine;
		strLine.Format(_T("%s,%d,%lf,%lf"),bstrDH,nQH,x,y);
		if (nDKH != nDKBH)//如果不相等,说明当前行是一个新元素的开始,插入相关属性
		{
			if (nDKBH != 0)
			{
				arstrTmp->Add(strEndLine);
				strtmptest += strEndLine;
				strtmptest += _T("\r\n");
			}
			strDK_GUID.Format(_T("%s"),bstrDK_GUID);

			//add by lcc 2016.12.22
			CString strDK_GUID_TMP = strDK_GUID;
			int nPartIndex = strDK_GUID.Find(_T("$"));
			if (nPartIndex < 0) return E_FAIL;//地块属性不符合约定的格式
			nPartIndex = strDK_GUID.Find(_T("$"),nPartIndex);
			if (nPartIndex < 0) return E_FAIL;//地块属性不符合约定的格式
			//end
			strDK_GUID.Replace(_T("$"),_T(","));
			CString strDKInfo;
			strDKInfo = _T("0,0,") + strDK_GUID + _T(",,面,,,,@");
			arstrTmp->Add(strDKInfo);
			strtmptest += strDKInfo;
			strtmptest += _T("\r\n");
			nDKBH = nDKH;
			strEndLine = strLine;
		}else if (nQH != nDKQH)//地块的下一个圈开始了
		{
			arstrTmp->Add(strEndLine);
			strEndLine = strLine;
			nDKQH = nQH;
		}else
		{
			arstrTmp->Add(strLine);
			strtmptest += strLine;
			strtmptest += _T("\r\n");
		}

	}
	//加上最后一个点
	arstrTmp->Add(strEndLine);
	strtmptest += strEndLine;
	strtmptest += _T("\r\n");
	SafeArrayUnaccessData(vt.parray);
	return S_OK;
}

HRESULT CConvertXYData::AddGHYTDMToFeature(BSTR bstrGHGXDKTableName)
{
	HRESULT hr;
	if (m_spFeatureClass == NULL) return E_FAIL;
	if (m_ipADOConn == NULL) return E_FAIL;
	long plObjState;
	m_ipADOConn->get_State(&plObjState);
	if (!plObjState) return E_FAIL;

	//获取OID字段
	CComBSTR bstrOIDFieldName;
	m_spFeatureClass->get_OIDFieldName(&bstrOIDFieldName);
	
	CString strSQL;//查询语句模板
	strSQL.Format(_T("select GHYTDM,XMBSM,FABSM,YGHYTDM,YYTDKBH from %s where XZDM = '%%s' and XZQMC = '%%s' and YTDKBH = '%%s'"),bstrGHGXDKTableName);
	//strSQL.Format(_T("select GHYTDM,XMBSM,FABSM from %s where XZDM = '%%s' and XZQMC = '%%s' and YTDKBH = '%%s'"),bstrGHGXDKTableName);

	CComQIPtr<IFeature> spFt;
	CComQIPtr<IFeatureCursor> spFtCursor;
	m_spFeatureClass->Search(NULL,VARIANT_FALSE,&spFtCursor);
	if (spFtCursor == NULL) return E_FAIL;
	spFtCursor->NextFeature(&spFt);
	while(spFt != NULL)
	{
		_RecordsetPtr pRst; 
		pRst.CreateInstance(__uuidof(Recordset));
		CComQIPtr<IFields> spFields;
		spFt->get_Fields(&spFields);
		long nIndexField;
		VARIANT vtSQLRes;
		VARIANT vtXZDM;
		VARIANT vtXZQMC;
		VARIANT vtYTDKBH;

		spFields->FindField(_T("XZDM"),&nIndexField);
		spFt->get_Value(nIndexField,&vtXZDM);

		spFields->FindField(_T("XZQMC"),&nIndexField);
		spFt->get_Value(nIndexField,&vtXZQMC);

		spFields->FindField(_T("YTDKBH_H"),&nIndexField);
		spFt->get_Value(nIndexField,&vtYTDKBH);

		CString strWhere;
		strWhere.Format(strSQL,vtXZDM.bstrVal,vtXZQMC.bstrVal,vtYTDKBH.bstrVal);
		//BCGPMessageBox(strWhere,_T(""),MB_OK);
		hr = pRst->Open(_variant_t(_bstr_t(strWhere)),_variant_t(m_ipADOConn.GetInterfacePtr()), adOpenStatic,adLockOptimistic,adCmdText);
		if (hr == S_OK)
		{
			hr = pRst->GetRows(1,vtMissing,vtMissing,&vtSQLRes);
			//add by lcc 2016.12.22判断结果集是否正确
			if (hr != S_OK)
			{
				//SafeArrayUnaccessData(vtSQLRes.parray);
				spFt->Store();
				spFt.Release();
				spFtCursor->NextFeature(&spFt);
				continue;
			}
			//add by lcc 2016.12.22 end
			void *pvd = NULL;
			SafeArrayAccessData(vtSQLRes.parray,&pvd);
			if (pvd == NULL)
			{
				SafeArrayUnaccessData(vtSQLRes.parray);
				spFt->Store();
				spFt.Release();
				spFtCursor->NextFeature(&spFt);
				continue;
			}
			VARIANT vtTmp = vtMissing;
			//edit by lcc 2016.12.21 添加两个字段
			for (int i = 0; i < 4; i++)
			{
				switch(i)
				{
				case 0:
					vtTmp = ((VARIANT*)pvd)[0];
					spFields->FindField(_T("GHYTDM_H"),&nIndexField);
					spFt->put_Value(nIndexField,vtTmp);
				case 1:
					vtTmp = ((VARIANT*)pvd)[1];
					spFields->FindField(_T("XMBSM"),&nIndexField);
					spFt->put_Value(nIndexField,vtTmp);
				case 2:
					vtTmp = ((VARIANT*)pvd)[2];
					spFields->FindField(_T("FABSM"),&nIndexField);
					spFt->put_Value(nIndexField,vtTmp);
				case 3:
					vtTmp = ((VARIANT*)pvd)[3];
					spFields->FindField(_T("YGHYTDM"),&nIndexField);
					spFt->put_Value(nIndexField,vtTmp);
				case 4:
					vtTmp = ((VARIANT*)pvd)[4];
					spFields->FindField(_T("YYTDKBH"),&nIndexField);
					spFt->put_Value(nIndexField,vtTmp);
				default:
					break;
				}
			}
			//edit by lcc 2016.12.21 end
			SafeArrayUnaccessData(vtSQLRes.parray);
		}
		spFt->Store();
		spFt.Release();
		spFtCursor->NextFeature(&spFt);
	}
	return S_OK;
}

STDMETHODIMP CConvertXYData::ConvertJZDToPolygon2(BSTR bstrMDBFilePath, BSTR bstrJZDTableName, BSTR bstrPolygonName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	HRESULT hr;
	//清理全局变量
	if (m_spWorkspace != NULL) m_spWorkspace.Release();
	if (m_spFtDataset != NULL) m_spFtDataset.Release();
	if (m_spFeatureClass != NULL) m_spFeatureClass.Release();
	if (m_ipADOConn != NULL)
	{
		long plObjState;
		m_ipADOConn->get_State(&plObjState);
		if(plObjState)
			m_ipADOConn->Close();
		m_ipADOConn.Release();
	}
	//打开工作空间
	CComQIPtr<IWorkspaceFactory> spWorkspaceFactory;
	spWorkspaceFactory.CoCreateInstance(CLSID_AccessWorkspaceFactory);
	spWorkspaceFactory->OpenFromFile(bstrMDBFilePath,NULL,&m_spWorkspace);
	if (m_spWorkspace == NULL)
	{
		return E_FAIL;
	}
	//读取MDB文件的内容
	CArray<CString,CString> strCoordArray;
	hr = ReadMDBFile2(bstrMDBFilePath,bstrJZDTableName,&strCoordArray);
	if (hr != S_OK) 
	{
		CString strErr;
		strErr.Format(_T("%d"),hr);
		//BCGPMessageBox(_T("读取界址点时错误的返回"),strErr,MB_OK);
		return hr;//读取界址点时错误的返回
	}
	if (strCoordArray.GetCount() == 0)
	{
		CString strErr;
		strErr.Format(_T("%d"),hr);
		//BCGPMessageBox(_T("说明界址点文件无法识别"),strErr,MB_OK);
		return E_FAIL;//说明界址点文件无法识别
	}
	//读取带号，创建空间参考
	int nProjCS = GetSpatialNum(_bstr_t(strCoordArray.GetAt(1)));
	if (nProjCS == -1) return E_FAIL;//如果X坐标位数小于8位
	CreateSpatial(nProjCS);
	if (m_spSpatialRederence == NULL)
	{
		//BCGPMessageBox(_T("创建空间参考失败"),_T(""),MB_OK);
		return E_FAIL;//创建空间参考失败
	}

	//然后是创建FeatureClass
	CComQIPtr<IFeatureWorkspace> spFtWorkspace;
	spFtWorkspace = m_spWorkspace;
	spFtWorkspace->OpenFeatureClass(bstrPolygonName,&m_spFeatureClass);
	if (m_spFeatureClass == NULL)
	{
		CreatFeatureClass3(bstrPolygonName);
		if (m_spFeatureClass == NULL) return E_FAIL;
	}

	//组织图形，如果图形不为空，创建Feature并写入图形
	hr = ConvertMDBCoordToFeature(m_spFeatureClass,strCoordArray);
	if (hr != S_OK) return hr;
	//查询和匹配后规划用途代码字段
	//hr = AddGHYTDMToFeature(bstrGHGXDKTableName);

	return hr;
}

void CConvertXYData::CreatFeatureClass3(BSTR bstrFeatureClassName)
{
	CComQIPtr<IUID> spUID;
	spUID.CoCreateInstance(CLSID_UID);
	spUID->put_Value(_variant_t("esriGeoDatabase.Feature"));

	CComQIPtr<IObjectClassDescription> spObjectClassDescription;
	spObjectClassDescription.CoCreateInstance(CLSID_FeatureClassDescription);
	CComQIPtr<IFields> spFields;
	CComQIPtr<IFieldsEdit> spFieldsEdit;
	spObjectClassDescription->get_RequiredFields(&spFields);
	spFieldsEdit = spFields;

	CComQIPtr<IField> spField;
	CComQIPtr<IFieldEdit> spFieldEdit;

	spFieldsEdit->get_Field(1,&spField);
	spFieldEdit = spField;
	CComQIPtr<IGeometryDef> spGeometryDef;
	spFieldEdit->get_GeometryDef(&spGeometryDef);
	CComQIPtr<IGeometryDefEdit> spGeometryDefEdit = spGeometryDef;
	//设置要素类型
	spGeometryDefEdit->put_GeometryType(esriGeometryPolygon);
	//给定坐标系
	spGeometryDefEdit->putref_SpatialReference(m_spSpatialRederence);
	spField.Release();
	spFieldEdit.Release();

	//添加字段
	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_T("DK_GUID"));
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldEdit->put_Length(100);
	spFieldEdit->put_DefaultValue(_variant_t(_T(" ")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();

	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_T("XZDM"));
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldEdit->put_Length(12);
	spFieldEdit->put_DefaultValue(_variant_t(_T(" ")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();
	
	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_T("XZQMC"));
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldEdit->put_Length(100);
	spFieldEdit->put_DefaultValue(_variant_t(_T(" ")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();

	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_T("YTDKBH_H"));
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldEdit->put_Length(30);
	spFieldEdit->put_DefaultValue(_variant_t(_T(" ")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();

	//调用接口创建要素集
	CComQIPtr<IFeatureWorkspace> spFtWorkspace = m_spWorkspace;
	spFtWorkspace->CreateFeatureClass(bstrFeatureClassName,spFields,spUID,NULL,esriFTSimple,_T("Shape"),NULL,&m_spFeatureClass);
}
