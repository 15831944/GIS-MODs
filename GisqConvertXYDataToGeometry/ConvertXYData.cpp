// ConvertXYData.cpp : CConvertXYData ��ʵ��

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
	//�ȸ��ݸ���·������GDB	
	if (m_spWorkspace != NULL)
	{
		m_spWorkspace.Release();
	}
	CComQIPtr<IWorkspaceName> spWorkspaceName;
	hr = CGeometryOperation::CreateWorkspace(enmWSTGdb,bstrDestFolder,bstrDestName,spWorkspaceName);
	if (spWorkspaceName == NULL) return hr;//����GDBʧ��
	spWorkspaceName->get_PathName(&bstrDestFolder);
	hr = CGeometryOperation::OpenWorkspaceFromFile(enmWSTGdb,bstrDestFolder,m_spWorkspace);
	if (hr != S_OK) return hr; //�����ʧ�ܣ�����ֹ

	//Ȼ���Ƕ�ȡTXT�ļ�������
	CArray<CString,CString> strCoordArray;
	ReadCoordFile(bstrTXTFilePath,&strCoordArray);
	if (strCoordArray.GetCount() == 0)
	{
		return E_FAIL;//˵����ַ���ļ��޷�ʶ��
	}
	//��ȡ���ţ������ռ�ο�
	int nProjCS = GetSpatialNum(strCoordArray.GetAt(13).AllocSysString());
	if (nProjCS == -1) return E_FAIL;//���X����λ��С��8λ
	CreateSpatial(nProjCS);
	if (m_spSpatialRederence == NULL) return E_FAIL;//�����ռ�ο�ʧ��

	//Ȼ���Ǵ���Dataset
	CComQIPtr<IFeatureWorkspace> spFtWorkspace;
	spFtWorkspace = m_spWorkspace;
	//edit by lcc 2016.12.01����Զ���dataset����
	CGeometryOperation::CreateFeatureDataset(spFtWorkspace,m_spSpatialRederence,bstrDatasetName,m_spFtDataset);
	if (m_spFtDataset == NULL) return E_FAIL;//����Datasetʧ��

	//edit by lcc 2016.12.01����Զ���featureclass����
	//��Dataset�д���FeatureClass
	CreatFeatureClass(bstrFeatureClassName);
	//edit end 2016.12.01

	//��֯ͼ�Σ����ͼ�β�Ϊ�գ�����Feature��д��ͼ��
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
	//���ô�������ΪҪ�ؼ�
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
	spFieldEdit->put_AliasName(_T("����������"));
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
	spFieldEdit->put_AliasName(_T("����������"));
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
	spFieldEdit->put_AliasName(_T("�ؿ���"));
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
	spFieldEdit->put_AliasName(_T("�ؿ�����"));
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldEdit->put_Length(100);
	spFieldEdit->put_DefaultValue(_variant_t(_T("")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();
	//add by lcc 2016.12.05 ��ӹ滮��;�ֶ�
	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	/*spFieldEdit->put_Name(_T("GHYT"));
	spFieldEdit->put_AliasName(_T("�滮��;"));*/
	spFieldEdit->put_Name(_T("GHYTDM"));
	spFieldEdit->put_AliasName(_T("�滮��;����"));
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldEdit->put_Length(20);
	spFieldEdit->put_DefaultValue(_variant_t(_T("")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();
	//add end 2016.12.05 ��ӹ滮��;�ֶ�

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

	//ת��Ϊͼ��
	int nStopRingID = 0;
	//�������ǰ12��û���ã�����ӵ�13�п�ʼ��
	for (int i = 12; i < arstrTmp.GetCount(); i++)
	{
		LPCWSTR readtmp;
		readtmp = arstrTmp.GetAt(i).GetString();
		TRACE(readtmp);
		TRACE(_T("\n"));
		if (arstrTmp.GetAt(i).Find(_T("@")) > 0)//һ��Ҫ�ؿ�ʼ
		{
			if (spPointCollection != NULL)//˵��֮ǰ��һ��Ҫ�ػ�Ȧ��Ҫ����
			{
				if (nStopRingID = 1)//֮ǰ��Ҫ��ֻ��һ��Ȧ,�������Ҫ��
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

					SaveFeature(spGeometry,spFt);

				}else if (nStopRingID > 1)//�����ϸ�Ҫ�ص�һ�����и�����,���и�,�ٱ���
				{
					hr = PolygonCutHollow(spPointCollection,spGeometry);
					if (hr != S_OK) return hr;
					SaveFeature(spGeometry,spFt);
				}				
			}

			//����Ҫ�أ�д����������Ϣ	
			if (spFt != NULL) spFt.Release();
			spFtClass->CreateFeature(&spFt);
			AddFieldsToFeature(arstrTmp.GetAt(i),spFt);
			//��ʼ������
			if (spPolyGon != NULL) spPolyGon.Release();
			if (spGeometry != NULL) spGeometry.Release();
			if (spPointCollection != NULL) spPointCollection.Release();
			if (spGeometryCollection != NULL) spGeometryCollection.Release();

			spPolyGon.CoCreateInstance(CLSID_Polygon);
			spGeometry = spPolyGon;
			spGeometry->QueryInterface(&spGeometryCollection);
			spPointCollection.CoCreateInstance(CLSID_Multipoint);
			nStopRingID = 1;
		}else//������ʼ�У������ﴦ�����ݣ�д��㼯		
		{
			LPCWSTR ptmp;
			ptmp = arstrTmp.GetAt(i).GetString();
			TRACE(ptmp);
			int nPointID = 0;
			int nRingID = 0;
			double x = 0;
			double y = 0;
			//�����ʽ����һ�е�����
			swscanf_s(ptmp,_T("J%d,%d,%lf,%lf"),&nPointID,&nRingID,&y,&x);
			if (nPointID == 1 && i > arstrTmp.GetCount() -4)//������
			{
				if (nRingID == 1)//����������ǵ�һ��Ȧ,��ô�ɻ�����Ϳ�����
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

				}else//������ǵ�һ��Ȧ,�и�
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

			}else if (nPointID == 1 && nRingID > nStopRingID)//һ��Ҫ�ص���һ��Ȧ��ʼ
			{
				if (nStopRingID == 1)//����������ǵ�һ��Ȧ,��ô�ɻ�����Ϳ�����
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

				}else//������ǵ�һ��Ȧ,�и�
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
				//�����������,����Ȧ��,���õ㼯
				nStopRingID = nRingID;
				spPointCollection.Release();
				spPointCollection.CoCreateInstance(CLSID_Multipoint);

			}else//����һ����ͨ�ĵ�
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

	//�ȳɻ�,Ȼ���и�
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
	//�������˺��������
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

	//ֻ�е�ͼ�β�Ϊ��ʱ����ֵ						
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
	//add by lcc 2016.12.05 ��ӹ滮��;�ֶ�
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
		//BCGPMessageBox(_T("δ֪�ļ���������"),_T("����"),MB_OK);
		return FALSE;
	}

	if (fType == unicode)
	{
		//������ڶ�ȡUniCode�ļ�ͷ,������һ�ζ������ı�
		CoordFile.SeekToBegin();
		wchar_t Unicodetest[1];
		CoordFile.Read(Unicodetest,2);

		//���������Unicode��ʽ��ȡ�ļ�
		UINT nlenth;
		nlenth = CoordFile.GetLength();
		wchar_t *pwcharBuf = new wchar_t[nlenth +1];
		ZeroMemory(pwcharBuf,nlenth +1);
		//����ط���ȡ��ȫ���ַ���Ȼ�����Ҫ���д���
		CoordFile.Read(pwcharBuf,nlenth+1);
		CString strBuf;
		strBuf.Format(_T("%s"),pwcharBuf);

		//���зָ�Ϊ����
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
					i++;//������ԡ�\n\r\n����β����ô���������1����ɾ������ط��ġ�\n��
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
					//������ԡ�\n\r\n����β����ô���������1����ɾ������ط��ġ�\n��
					nfindn = arstrTmp->GetAt(ntest).Delete(nfindn,1);
					i++;
				}
				break;//���û�ҵ��س�˵�������ļ���β��
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
		//���зָ�Ϊ����
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
					//������ԡ�\n\r\n����β����ô���������1����ɾ������ط��ġ�\n��
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
					//������ԡ�\n\r\n����β����ô���������1����ɾ������ط��ġ�\n��
					nfindn = arstrTmp->GetAt(ntest).Delete(nfindn,1);
					i++;
				}
				break;//���û�ҵ��س�˵�������ļ���β��
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
		//���зָ�Ϊ����
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
					//������ԡ�\n\r\n����β����ô���������1����ɾ������ط��ġ�\n��
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
					//������ԡ�\n\r\n����β����ô���������1����ɾ������ط��ġ�\n��
					nfindn = arstrTmp->GetAt(ntest).Delete(nfindn,1);
					i++;
				}
				break;//���û�ҵ��س�˵�������ļ���β��
			}
		}
		delete result;
		delete pcharBuf;
		return TRUE;
	}

	return TRUE;
}
//add by lcc 2016.12.13����ϵͳ������MDB�����ӿ�
STDMETHODIMP CConvertXYData::ConvertMDBToFeatureClass(BSTR bstrMDBFilePath, BSTR bstrDestFolder, BSTR bstrDestName, BSTR bstrDatasetName, BSTR bstrFeatureClassName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ����ʵ�ִ���
	HRESULT hr;
	//�ȸ��ݸ���·������GDB	
	if (m_spWorkspace != NULL)
	{
		m_spWorkspace.Release();
	}
	CComQIPtr<IWorkspaceName> spWorkspaceName;
	hr = CGeometryOperation::CreateWorkspace(enmWSTGdb,bstrDestFolder,bstrDestName,spWorkspaceName);
	if (spWorkspaceName == NULL) return hr;//����GDBʧ��
	spWorkspaceName->get_PathName(&bstrDestFolder);
	hr = CGeometryOperation::OpenWorkspaceFromFile(enmWSTGdb,bstrDestFolder,m_spWorkspace);
	if (hr != S_OK) return hr; //�����ʧ�ܣ�����ֹ

	//Ȼ���Ƕ�ȡMDB�ļ�������
	CArray<CString,CString> strCoordArray;
	ReadMDBFile(bstrMDBFilePath,&strCoordArray);
	if (strCoordArray.GetCount() == 0)
	{
		return E_FAIL;//˵����ַ���ļ��޷�ʶ��
	}
	//��ȡ���ţ������ռ�ο�
	int nProjCS = GetSpatialNum(_bstr_t(strCoordArray.GetAt(1)));
	if (nProjCS == -1) return E_FAIL;//���X����λ��С��8λ
	CreateSpatial(nProjCS);
	if (m_spSpatialRederence == NULL) return E_FAIL;//�����ռ�ο�ʧ��

	//Ȼ���Ǵ���Dataset
	CComQIPtr<IFeatureWorkspace> spFtWorkspace;
	spFtWorkspace = m_spWorkspace;
	//edit by lcc 2016.12.01����Զ���dataset����
	CGeometryOperation::CreateFeatureDataset(spFtWorkspace,m_spSpatialRederence,bstrDatasetName,m_spFtDataset);
	if (m_spFtDataset == NULL) return E_FAIL;//����Datasetʧ��

	//edit by lcc 2016.12.01����Զ���featureclass����
	//��Dataset�д���FeatureClass
	CreatFeatureClass2(bstrFeatureClassName);
	//edit end 2016.12.01

	//��֯ͼ�Σ����ͼ�β�Ϊ�գ�����Feature��д��ͼ��
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
	//���ô�������ΪҪ�ؼ�
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
// 	//���DKID�ֶ�
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
// 	//���DKMC�ֶ�
// 	spField.CoCreateInstance(CLSID_Field);
// 	spFieldEdit = spField;
// 	spFieldEdit->put_Name(_T("DKMC"));
// 	spFieldEdit->put_Type(esriFieldTypeString);
// 	spFieldEdit->put_DefaultValue(_variant_t(_T("")));
// 	spFieldEdit->put_IsNullable(VARIANT_TRUE);
// 	spFieldsEdit->AddField(spField);
// 	spField.Release();
// 	spFieldEdit.Release();

	//���GHYTDM�ֶ�2016.12.16
	spField.CoCreateInstance(CLSID_Field);
	spFieldEdit = spField;
	spFieldEdit->put_Name(_T("GHYTDM_H"));
	spFieldEdit->put_Type(esriFieldTypeString);
	spFieldEdit->put_DefaultValue(_variant_t(_T(" ")));
	spFieldEdit->put_IsNullable(VARIANT_TRUE);
	spFieldsEdit->AddField(spField);
	spField.Release();
	spFieldEdit.Release();

	//add by lcc 2016.12.21��������ֶ�
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
		//���н���
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
		//ƴ�ӳɹ���
		CString strLine;
		strLine.Format(_T("%s,%d,%lf,%lf"),bstrDH,nQH,x,y);
		if (nDKH != nDKBH)//��������,˵����ǰ����һ����Ԫ�صĿ�ʼ,�����������
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
			strDKInfo = _T("0,0,") + strDK_GUID + _T(",,��,,,,@");
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
	//�������һ����
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

	//ת��Ϊͼ��
	int nStopRingID = 0;
	int nStartPointID = 1;
	for (int i = 0; i < arstrTmp.GetCount(); i++)
	{
		LPCWSTR readtmp;
		readtmp = arstrTmp.GetAt(i).GetString();
		TRACE(readtmp);
		TRACE(_T("\n"));
		if (arstrTmp.GetAt(i).Find(_T("@")) > 0)//һ��Ҫ�ؿ�ʼ
		{
			if (spPointCollection != NULL)//˵��֮ǰ��һ��Ҫ�ػ�Ȧ��Ҫ����
			{
				if (nStopRingID = 1)//֮ǰ��Ҫ��ֻ��һ��Ȧ,�������Ҫ��
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

					SaveFeature(spGeometry,spFt);

				}else if (nStopRingID > 1)//�����ϸ�Ҫ�ص�һ�����и�����,���и�,�ٱ���
				{
					hr = PolygonCutHollow(spPointCollection,spGeometry);
					if (hr != S_OK) return hr;
					SaveFeature(spGeometry,spFt);
				}				
			}

			//����Ҫ�أ�д����������Ϣ	
			if (spFt != NULL) spFt.Release();
			spFtClass->CreateFeature(&spFt);
			AddFieldsToFeature2(arstrTmp.GetAt(i),spFt);
			//��ʼ������
			if (spPolyGon != NULL) spPolyGon.Release();
			if (spGeometry != NULL) spGeometry.Release();
			if (spPointCollection != NULL) spPointCollection.Release();
			if (spGeometryCollection != NULL) spGeometryCollection.Release();

			spPolyGon.CoCreateInstance(CLSID_Polygon);
			spGeometry = spPolyGon;
			spGeometry->QueryInterface(&spGeometryCollection);
			spPointCollection.CoCreateInstance(CLSID_Multipoint);
			nStopRingID = 1;
		}else//������ʼ�У������ﴦ�����ݣ�д��㼯		
		{
			LPCWSTR ptmp;
			ptmp = arstrTmp.GetAt(i).GetString();
			//TRACE(ptmp);
			int nPointID = 0;
			int nRingID = 0;
			double x = 0;
			double y = 0;
			//�����ʽ����һ�е�����
			swscanf_s(ptmp,_T("J%d,%d,%lf,%lf"),&nPointID,&nRingID,&y,&x);
			if (nPointID == nStartPointID && i > arstrTmp.GetCount() -4)//������
			{
				if (nRingID == 1)//����������ǵ�һ��Ȧ,��ô�ɻ�����Ϳ�����
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

				}else//������ǵ�һ��Ȧ,�и�
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

			}else if (nRingID > nStopRingID)//һ��Ҫ�ص���һ��Ȧ��ʼ
			{
				if (nStopRingID == 1)//����������ǵ�һ��Ȧ,��ô�ɻ�����Ϳ�����
				{
					CComQIPtr<IRing> spRing;
					hr = GetRing(spPointCollection,spRing);
					if (hr != S_OK) return hr;
					spGeometryCollection->AddGeometry(spRing);
					spPolyGon->Close();

				}else//������ǵ�һ��Ȧ,�и�
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
				//�����������,����Ȧ��,���õ㼯
				nStopRingID = nRingID;
				nStartPointID = nPointID;
				spPointCollection.Release();
				spPointCollection.CoCreateInstance(CLSID_Multipoint);

				CComQIPtr<IPoint> spPoint;
				spPoint.CoCreateInstance(CLSID_Point);
				spPoint->PutCoords(x,y);
				spPointCollection->AddPoint(spPoint);
			}else//����һ����ͨ�ĵ�
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

	//�ӵؿ�GUID
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

	// TODO: �ڴ����ʵ�ִ���
	HRESULT hr;
	//�ȸ��ݸ���·������GDB	
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
	if (spWorkspaceName == NULL) return hr;//����GDBʧ��
	spWorkspaceName->get_PathName(&bstrTargetFolder);
	hr = CGeometryOperation::OpenWorkspaceFromFile(enmWSTGdb,bstrTargetFolder,m_spWorkspace);
	if (hr != S_OK) return hr; //�����ʧ�ܣ�����ֹ

	//Ȼ���Ƕ�ȡMDB�ļ�������
	CArray<CString,CString> strCoordArray;
	hr = ReadMDBFile2(bstrMDBFilePath,bstrJZDTableName,&strCoordArray);
	if (hr != S_OK) 
	{
		CString strErr;
		strErr.Format(_T("%d"),hr);
		//BCGPMessageBox(_T("��ȡ��ַ��ʱ����ķ���"),strErr,MB_OK);
		return hr;//��ȡ��ַ��ʱ����ķ���
	}
	if (strCoordArray.GetCount() == 0)
	{
		CString strErr;
		strErr.Format(_T("%d"),hr);
		//BCGPMessageBox(_T("˵����ַ���ļ��޷�ʶ��"),strErr,MB_OK);
		return E_FAIL;//˵����ַ���ļ��޷�ʶ��
	}
	//��ȡ���ţ������ռ�ο�
	int nProjCS = GetSpatialNum(_bstr_t(strCoordArray.GetAt(1)));
	if (nProjCS == -1) return E_FAIL;//���X����λ��С��8λ
	CreateSpatial(nProjCS);
	if (m_spSpatialRederence == NULL)
	{
		//BCGPMessageBox(_T("�����ռ�ο�ʧ��"),_T(""),MB_OK);
		return E_FAIL;//�����ռ�ο�ʧ��
	}

	//Ȼ���Ǵ���Dataset
	CComQIPtr<IFeatureWorkspace> spFtWorkspace;
	spFtWorkspace = m_spWorkspace;
	//edit by lcc 2016.12.01����Զ���dataset����
	hr = CGeometryOperation::CreateFeatureDataset(spFtWorkspace,m_spSpatialRederence,bstrFeatureDatasetName,m_spFtDataset);
	if (hr != S_OK) 
	{
		CString strErr;
		strErr.Format(_T("%d"),hr);
		//BCGPMessageBox(_T("����Datasetʧ��"),strErr,MB_OK);
		return E_FAIL;//����Datasetʧ��
	}

	//edit by lcc 2016.12.01����Զ���featureclass����
	//��Dataset�д���FeatureClass
	CreatFeatureClass2(bstrPolygonName);
	//edit end 2016.12.01

	//��֯ͼ�Σ����ͼ�β�Ϊ�գ�����Feature��д��ͼ��
	hr = ConvertMDBCoordToFeature(m_spFeatureClass,strCoordArray);
	if (hr != S_OK) return hr;
	//��ѯ��ƥ���滮��;�����ֶ�
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
		//���н���
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
		//ƴ�ӳɹ���
		CString strLine;
		strLine.Format(_T("%s,%d,%lf,%lf"),bstrDH,nQH,x,y);
		if (nDKH != nDKBH)//��������,˵����ǰ����һ����Ԫ�صĿ�ʼ,�����������
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
			if (nPartIndex < 0) return E_FAIL;//�ؿ����Բ�����Լ���ĸ�ʽ
			nPartIndex = strDK_GUID.Find(_T("$"),nPartIndex);
			if (nPartIndex < 0) return E_FAIL;//�ؿ����Բ�����Լ���ĸ�ʽ
			//end
			strDK_GUID.Replace(_T("$"),_T(","));
			CString strDKInfo;
			strDKInfo = _T("0,0,") + strDK_GUID + _T(",,��,,,,@");
			arstrTmp->Add(strDKInfo);
			strtmptest += strDKInfo;
			strtmptest += _T("\r\n");
			nDKBH = nDKH;
			strEndLine = strLine;
		}else if (nQH != nDKQH)//�ؿ����һ��Ȧ��ʼ��
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
	//�������һ����
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

	//��ȡOID�ֶ�
	CComBSTR bstrOIDFieldName;
	m_spFeatureClass->get_OIDFieldName(&bstrOIDFieldName);
	
	CString strSQL;//��ѯ���ģ��
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
			//add by lcc 2016.12.22�жϽ�����Ƿ���ȷ
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
			//edit by lcc 2016.12.21 ��������ֶ�
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

	// TODO: �ڴ����ʵ�ִ���
	HRESULT hr;
	//����ȫ�ֱ���
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
	//�򿪹����ռ�
	CComQIPtr<IWorkspaceFactory> spWorkspaceFactory;
	spWorkspaceFactory.CoCreateInstance(CLSID_AccessWorkspaceFactory);
	spWorkspaceFactory->OpenFromFile(bstrMDBFilePath,NULL,&m_spWorkspace);
	if (m_spWorkspace == NULL)
	{
		return E_FAIL;
	}
	//��ȡMDB�ļ�������
	CArray<CString,CString> strCoordArray;
	hr = ReadMDBFile2(bstrMDBFilePath,bstrJZDTableName,&strCoordArray);
	if (hr != S_OK) 
	{
		CString strErr;
		strErr.Format(_T("%d"),hr);
		//BCGPMessageBox(_T("��ȡ��ַ��ʱ����ķ���"),strErr,MB_OK);
		return hr;//��ȡ��ַ��ʱ����ķ���
	}
	if (strCoordArray.GetCount() == 0)
	{
		CString strErr;
		strErr.Format(_T("%d"),hr);
		//BCGPMessageBox(_T("˵����ַ���ļ��޷�ʶ��"),strErr,MB_OK);
		return E_FAIL;//˵����ַ���ļ��޷�ʶ��
	}
	//��ȡ���ţ������ռ�ο�
	int nProjCS = GetSpatialNum(_bstr_t(strCoordArray.GetAt(1)));
	if (nProjCS == -1) return E_FAIL;//���X����λ��С��8λ
	CreateSpatial(nProjCS);
	if (m_spSpatialRederence == NULL)
	{
		//BCGPMessageBox(_T("�����ռ�ο�ʧ��"),_T(""),MB_OK);
		return E_FAIL;//�����ռ�ο�ʧ��
	}

	//Ȼ���Ǵ���FeatureClass
	CComQIPtr<IFeatureWorkspace> spFtWorkspace;
	spFtWorkspace = m_spWorkspace;
	spFtWorkspace->OpenFeatureClass(bstrPolygonName,&m_spFeatureClass);
	if (m_spFeatureClass == NULL)
	{
		CreatFeatureClass3(bstrPolygonName);
		if (m_spFeatureClass == NULL) return E_FAIL;
	}

	//��֯ͼ�Σ����ͼ�β�Ϊ�գ�����Feature��д��ͼ��
	hr = ConvertMDBCoordToFeature(m_spFeatureClass,strCoordArray);
	if (hr != S_OK) return hr;
	//��ѯ��ƥ���滮��;�����ֶ�
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
	//����Ҫ������
	spGeometryDefEdit->put_GeometryType(esriGeometryPolygon);
	//��������ϵ
	spGeometryDefEdit->putref_SpatialReference(m_spSpatialRederence);
	spField.Release();
	spFieldEdit.Release();

	//����ֶ�
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

	//���ýӿڴ���Ҫ�ؼ�
	CComQIPtr<IFeatureWorkspace> spFtWorkspace = m_spWorkspace;
	spFtWorkspace->CreateFeatureClass(bstrFeatureClassName,spFields,spUID,NULL,esriFTSimple,_T("Shape"),NULL,&m_spFeatureClass);
}
