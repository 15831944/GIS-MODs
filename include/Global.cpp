#include "StdAfx.h"
#include "Global.h"

//CConfigFile  implementation
void CConfigFile::GetSectionKeys(CString strConfigFile,CString strSection,CStringArray& szArrSectionKeys)
{
	if(!CheckFile(strConfigFile))return;
	const DWORD dSize = 1024*10;
	TCHAR szRet[dSize];
	ZeroMemory(szRet,dSize);
	::GetPrivateProfileString(strSection.GetBuffer(),NULL,NULL,szRet,dSize,strConfigFile.GetBuffer());
	int iStringStart = 0;
	for(int i = 0;i < dSize-1;i++)
	{
		if (szRet[i] == _T('\0'))
		{
			//空
			if(i == iStringStart)break;
			//TODO
			TCHAR* szTemp = &szRet[iStringStart];
			CString strTemp(szTemp);
			szArrSectionKeys.Add(strTemp);		
			
			if (szRet[i+1] != _T('\0'))
			{
				iStringStart = i+1;
			}
			else
			{
				break;
			}
		}
	}
	return;
}

void CConfigFile::GetAllSectionItems(CString strConfigFile,CString strSection,CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig)
{
	CStringArray szArrSectionKeys;
	GetSectionKeys(strConfigFile,strSection,szArrSectionKeys);
	if(szArrSectionKeys.IsEmpty())return;

	int nCount = szArrSectionKeys.GetCount();
	CString strDefaultValue = _T("");
	for(int i = 0;i < nCount;i++)
	{
		CString strValue;
		strValue = GetValue(strConfigFile,strSection,szArrSectionKeys[i],strDefaultValue);
		strMapConfig.SetAt(szArrSectionKeys[i],strValue);

	}
	return;
}

CString CConfigFile::GetValue(CString strConfigFile,CString strSection,CString strKey,CString strDefault /* = _T("") */)
{
	CString strValue;
	if(!CheckFile(strConfigFile))return strDefault;

	const DWORD dSize = 1024;
	TCHAR szRet[dSize];
	ZeroMemory(szRet,dSize);
	GetPrivateProfileString(strSection.GetBuffer(),strKey.GetBuffer(),strDefault.GetBuffer(),szRet,dSize,strConfigFile.GetBuffer());
	strValue.Format(_T("%s"),szRet);
	return strValue;
}

BOOL CConfigFile::CheckFile(CString strConfigFile)
{
	WIN32_FIND_DATA hFindData;
	HANDLE hFind;
	hFind = ::FindFirstFile(strConfigFile.GetBuffer(),&hFindData);
	return hFind != INVALID_HANDLE_VALUE;
}

//CGlobalWorkspace implementation
CComQIPtr<IWorkspace> CGlobalWorkspace::m_ipGHWorkspace;
CComQIPtr<IWorkspace> CGlobalWorkspace::m_ipLYWorkspace;

BOOL CGlobalWorkspace::InitAllWorkspace(CComQIPtr<IPropertySet> ipGHPropertySet,CComQIPtr<IPropertySet> ipLYPropertySet)
{
	BOOL bSucceed = FALSE;
	if (m_ipGHWorkspace != NULL)
	{
		m_ipGHWorkspace.Release();
	}else if (m_ipGHWorkspace == NULL)
	{
		CComQIPtr<IWorkspaceFactory> ipWorkspaceFactory;
		ipWorkspaceFactory.CoCreateInstance(CLSID_SdeWorkspaceFactory);
		ASSERT( ipWorkspaceFactory != NULL);
		ipWorkspaceFactory->Open(ipGHPropertySet,NULL,&m_ipGHWorkspace);
		if (m_ipGHWorkspace != NULL)
		{
			bSucceed = TRUE;
		}
	}
	if (m_ipLYWorkspace == NULL)
	{
		CComQIPtr<IWorkspaceFactory> ipWorkspaceFactory;
		ipWorkspaceFactory.CoCreateInstance(CLSID_SdeWorkspaceFactory);
		ASSERT( ipWorkspaceFactory != NULL);
		ipWorkspaceFactory->Open(ipLYPropertySet,NULL,&m_ipLYWorkspace);
		if (m_ipLYWorkspace != NULL)
		{
			bSucceed = TRUE;
		}
	}
	return bSucceed;
}

BOOL CGlobalWorkspace::InitGHWorkspace(CComQIPtr<IPropertySet> ipGHPropertySet)
{
	BOOL bSucceed = TRUE;
	if (m_ipGHWorkspace != NULL)
	{
		m_ipGHWorkspace.Release();
	}else if (m_ipGHWorkspace == NULL)
	{
		CComQIPtr<IWorkspaceFactory> ipWorkspaceFactory;
		ipWorkspaceFactory.CoCreateInstance(CLSID_SdeWorkspaceFactory);
		ASSERT( ipWorkspaceFactory != NULL);
		ipWorkspaceFactory->Open(ipGHPropertySet,NULL,&m_ipGHWorkspace);
		if (m_ipGHWorkspace == NULL)
		{
			bSucceed = FALSE;
		}
	}
	return bSucceed;
}

BOOL CGlobalWorkspace::InitLYWorkspace(CComQIPtr<IPropertySet> ipLYPropertySet)
{
	BOOL bSucceed = TRUE;
	if (m_ipLYWorkspace == NULL)
	{
		CComQIPtr<IWorkspaceFactory> ipWorkspaceFactory;
		ipWorkspaceFactory.CoCreateInstance(CLSID_SdeWorkspaceFactory);
		ASSERT( ipWorkspaceFactory != NULL);
		ipWorkspaceFactory->Open(ipLYPropertySet,NULL,&m_ipLYWorkspace);
		if (m_ipLYWorkspace == NULL)
		{
			bSucceed = FALSE;
		}
	}
	return bSucceed;
}

CComQIPtr<IWorkspace> CGlobalWorkspace::GetGHWorkspace()
{
	return m_ipGHWorkspace;
}

CComQIPtr<IWorkspace> CGlobalWorkspace::GetLYWorkspace()
{
	return m_ipLYWorkspace;
}

CString VariantToString(VARIANT var)
{
	CString strValue;
	_variant_t var_t;
	_bstr_t bstr_t;
	//time_t cur_time;
	//CTime time_value;
	COleCurrency var_currency;
	int nYear = 0;
	int nMonth = 0;
	int nDay = 0;
	int nHour = 0;
	int nMinute = 0;
	int nSecond = 0;
	COleDateTime tTime;
	switch(var.vt)
	{
	case VT_EMPTY:
	case VT_NULL:
		strValue=_T("");
		break;
	case VT_UI1:
		strValue.Format(_T("%d"),var.bVal);
		break;
	case VT_I2:
		strValue.Format(_T("%d"),var.iVal);
		break;
	case VT_I4:
		strValue.Format(_T("%d"),var.lVal);
		break;
	case VT_R4:
		strValue.Format(_T("%f"),var.fltVal);
		break;
	case VT_R8:
		strValue.Format(_T("%f"),var.dblVal);
		break;
	case VT_CY:
		var_currency=var;
		strValue=var_currency.Format(0);
		break;
	case VT_BSTR:
		var_t =var;
		bstr_t=var_t;
		strValue.Format(_T("%s"),(TCHAR*)bstr_t);
		break;
	case VT_DATE:
		//edit by lcc 2017/2/17 修改错误的时间格式解析方式
// 		cur_time=var.date;
// 		time_value=cur_time;
// 		strValue.Format(_T("%A,%B,%d,%Y"));
		tTime = var.date;
		nYear = tTime.GetYear();
		nMonth = tTime.GetMonth();
		nDay = tTime.GetDay();
		nHour = tTime.GetHour();
		nMinute = tTime.GetMinute();
		nSecond = tTime.GetSecond();
		strValue.Format(_T("%d-%d-%d %d:%d:%d"),nYear,nMonth,nDay,nHour,nMinute,nSecond);
		//edit by lcc 2017/2/17 end
		break;
	case VT_BOOL:
		strValue.Format(_T("%d"),var.boolVal);
		break;
		//add by lcc 2017/2/17 数据格式转字符串的最简单方式
	case VT_DECIMAL:
		strValue.Format(_T("%lf"),(double)(_variant_t)var);
		break;
		//add by lcc 2017/2/17 end
	default:
		strValue=_T("");
		break;
	}
	return strValue;
}


//CSPUnknownVector
CSPUnknownVector::CSPUnknownVector()
{
	m_vectorPointer.clear();
}

CSPUnknownVector::CSPUnknownVector(CSPUnknownVector& vecSPUnknownContainer)
{
	m_vectorPointer.clear();
	for (int i = 0;i < vecSPUnknownContainer.Count();i++)
	{
		//m_vectorPointer.push_back(vecSPUnknownContainer.GetAt(i));
		Push(vecSPUnknownContainer.GetAt(i));
	}
}

CSPUnknownVector::~CSPUnknownVector()
{
	vector<IUnknown*>::iterator it;
	for(it = m_vectorPointer.begin();it != m_vectorPointer.end();it++)
	{
		(*it)->Release();
	}
}

void CSPUnknownVector::Push(IUnknown* ipUnknown)
{
	if (ipUnknown != NULL)
	{
		ipUnknown->AddRef();
		m_vectorPointer.push_back(ipUnknown);
	}
}

IUnknown* CSPUnknownVector::GetAt(int index)
{
	if (m_vectorPointer.size() > (unsigned)index)
	{
		return m_vectorPointer.at(index);
	}
	else
	{
		return NULL;
	}
}

int CSPUnknownVector::Count()
{
	return m_vectorPointer.size();
}

void CSPUnknownVector::Clear()
{
	int nSize = m_vectorPointer.size();
	for(int i = 0;i < nSize;i++)
	{
		IUnknown* pUnknown = GetAt(i);
		pUnknown->Release();
		pUnknown = NULL;
	}
	m_vectorPointer.clear();
}

CSPUnknownVector& CSPUnknownVector::operator =(CSPUnknownVector& vecSPUnknownContainer)
{
	m_vectorPointer.clear();
	for (int i = 0;i < vecSPUnknownContainer.Count();i++)
	{
		//m_vectorPointer.push_back(vecSPUnknownContainer.GetAt(i));
		Push(vecSPUnknownContainer.GetAt(i));
	}
	return *this;
}


//CGeometryOperation
HRESULT CGeometryOperation::OpenWorkspaceFromFile(enmWorkspaceType workspaceType,BSTR bstrFilePath,CComQIPtr<IWorkspace>& spWorkspace)
{
	HRESULT hr = S_FALSE;
	if(spWorkspace != NULL)spWorkspace.Release();
	CComQIPtr<IWorkspaceFactory> spWorkspaceFactory;
	switch(workspaceType)
	{
	case enmWSTShp:
		hr = spWorkspaceFactory.CoCreateInstance(CLSID_ShapefileWorkspaceFactory);
		break;
	case enmWSTGdb:
		hr = spWorkspaceFactory.CoCreateInstance(CLSID_FileGDBWorkspaceFactory);
		break;
	case enmWSTMdb:
		hr = spWorkspaceFactory.CoCreateInstance(CLSID_AccessWorkspaceFactory);
		break;
	case enmWSTSde:
		hr = spWorkspaceFactory.CoCreateInstance(CLSID_SdeWorkspaceFactory);
		break;
	default:
		return hr;
	}
	ASSERT(spWorkspaceFactory != NULL);
	hr = spWorkspaceFactory->OpenFromFile(bstrFilePath,NULL,&spWorkspace);
	return hr;
}

HRESULT CGeometryOperation::CreateWorkspace(enmWorkspaceType workspaceType,BSTR bstrFolder,BSTR bstrName,CComQIPtr<IWorkspaceName>& spWorkspaceName)
{
	HRESULT hr = S_FALSE;
	if(spWorkspaceName != NULL)spWorkspaceName.Release();
	CComQIPtr<IWorkspaceFactory> spWorkspaceFactory;
	switch(workspaceType)
	{
	case enmWSTShp:
		hr = spWorkspaceFactory.CoCreateInstance(CLSID_ShapefileWorkspaceFactory);
	case enmWSTGdb:
		hr = spWorkspaceFactory.CoCreateInstance(CLSID_FileGDBWorkspaceFactory);
		break;
	case enmWSTMdb:
		hr = spWorkspaceFactory.CoCreateInstance(CLSID_AccessWorkspaceFactory);
		break;
	case enmWSTSde:
		hr =spWorkspaceFactory.CoCreateInstance(CLSID_SdeWorkspaceFactory);
		break;
	default:
		return hr;
	}
	ASSERT(spWorkspaceFactory != NULL);
	hr = spWorkspaceFactory->Create(bstrFolder,bstrName,NULL,NULL,&spWorkspaceName);
	return hr;
}

HRESULT CGeometryOperation::CreateFeatureDataset(CComQIPtr<IFeatureWorkspace> spFeatureWorkspace,CComQIPtr<ISpatialReference> spSpatialRefenence,BSTR bstrName,CComQIPtr<IFeatureDataset> &spFeatureDataset)
{
	HRESULT hr = S_OK;
	if(spFeatureDataset == NULL)
	{
		if (spSpatialRefenence == NULL)
		{
			hr = CreateSpatialReference(spSpatialRefenence);
			if (SUCCEEDED(hr))
			{
				hr = spFeatureWorkspace->CreateFeatureDataset(_bstr_t(bstrName),spSpatialRefenence,&spFeatureDataset);
			}
		}
		else
		{
			hr = spFeatureWorkspace->CreateFeatureDataset(_bstr_t(bstrName),spSpatialRefenence,&spFeatureDataset);
		}
	}
	return hr;
}

HRESULT CGeometryOperation::CreateSpatialReference(CComQIPtr<ISpatialReference>& spSpatialReference,double dxMin/* =-450359962737.05 */,double dxMax/* =450359962737.05 */,double dyMin/* =-450359962737.05 */, double dyMax/* =450359962737.05 */,double dxyTolerance/* =0.001 */)
{
	HRESULT hr = S_OK;
	if (spSpatialReference == NULL)
	{
		hr=spSpatialReference.CoCreateInstance(CLSID_UnknownCoordinateSystem);
	}
	//空间参考的域必须要设置
	if (spSpatialReference != NULL)
		spSpatialReference->SetDomain(dxMin, dxMax, dyMin, dyMax);

	CComQIPtr<ISpatialReferenceTolerance> spSpatialReferenceTolerance = spSpatialReference;
	hr = spSpatialReferenceTolerance->put_XYTolerance(dxyTolerance);
	return hr;
}

HRESULT CGeometryOperation::AddFieldToTable(CComQIPtr<ITable> spTable,BSTR strFieldName,BSTR strFieldAliasName,esriFieldType enmFieldType,long nLength,VARIANT_BOOL bNull)
{
	HRESULT  hr=S_FALSE;
	CComBSTR bstrFieldName(strFieldName);
	CComBSTR bstrFieldAliasName(strFieldAliasName);
	if (spTable == NULL || enmFieldType < 0 || enmFieldType > esriFieldTypeXML)
		return S_FALSE;
	if (bstrFieldName == CComBSTR(_T("")))return S_FALSE;
	CComQIPtr<ISchemaLock> spSchemaLock = spTable;
	if (spSchemaLock != NULL)
	{
		spSchemaLock->ChangeSchemaLock(esriExclusiveSchemaLock);
		CComQIPtr<IField> spField;
		spField.CoCreateInstance(CLSID_Field);
		CComQIPtr<IFieldEdit> spFieldEdit = spField;
		if (spFieldEdit)
		{
			spFieldEdit->put_Name(bstrFieldName);
			if (bstrFieldAliasName != CComBSTR(_T("")))
			{
				spFieldEdit->put_AliasName(bstrFieldAliasName);
			}
			spFieldEdit->put_Type(enmFieldType);
			spFieldEdit->put_Length(nLength);
			spFieldEdit->put_DefaultValue(_variant_t(0.0));
			spFieldEdit->put_IsNullable(bNull);
			hr= spTable->AddField(spField);
		}
		spSchemaLock->ChangeSchemaLock(esriSharedSchemaLock);
	}
	return hr;
}

CString CGPOperation::m_strGPErrorInfo = _T("");
//CGPOperation
CString CGPOperation::GetGPOperatorErrorInfo()
{
	return m_strGPErrorInfo;
}
HRESULT CGPOperation::GPIntersect(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<IGpValueTableObject> spInFeatures,BSTR bstrOutPutFeatureClass,VARIANT_BOOL bOverwriteOutput /* = VARIANT_TRUE */)
{
	HRESULT hr = S_FALSE;
	/*CComQIPtr<IGeoProcessor> spGeoProcessor;
	spGeoProcessor.CoCreateInstance(CLSID_GeoProcessor);*/
	if(spGeoProcessor == NULL)spGeoProcessor.CoCreateInstance(CLSID_GeoProcessor);
	spGeoProcessor->put_OverwriteOutput(bOverwriteOutput);
	CComQIPtr<IGeoProcessorResult> spGeoProcessorResult;
	CComQIPtr<IVariantArray> spVariantArray;
	spVariantArray.CoCreateInstance(CLSID_VarArray);
	spVariantArray->Add(_variant_t(spInFeatures,TRUE));
	spVariantArray->Add(_variant_t(bstrOutPutFeatureClass));
	hr = spGeoProcessor->Execute(_bstr_t(_T("Intersect_analysis")),spVariantArray,NULL,&spGeoProcessorResult);
	if (FAILED(hr))
	{
		CComBSTR bstrErrorMsg;
		spGeoProcessor->GetMessage(-1,&bstrErrorMsg);
		m_strGPErrorInfo = bstrErrorMsg;
	}
	return hr;
}

HRESULT CGPOperation::GPFeatureClass2FeatureClass(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<IFeatureClass> spInputFeatureClass,BSTR bstrOutputLocation,BSTR bstrOutputFeatureClassName,BSTR bstrWhereClause,VARIANT_BOOL bOverwirteOutput /* = VARIANT_TRUE */)
{
	HRESULT hr = S_FALSE;
	/*CComQIPtr<IGeoProcessor> spGeoProcessor;
	spGeoProcessor.CoCreateInstance(CLSID_GeoProcessor);*/
	if(spGeoProcessor == NULL)spGeoProcessor.CoCreateInstance(CLSID_GeoProcessor);
	spGeoProcessor->put_OverwriteOutput(bOverwirteOutput);
	CComQIPtr<IVariantArray> spVariantArray;
	spVariantArray.CoCreateInstance(CLSID_VarArray);
	CComQIPtr<IGeoProcessorResult> spGeoProcessorResult;
	spVariantArray->Add(_variant_t(spInputFeatureClass,TRUE));
	spVariantArray->Add(_variant_t(bstrOutputLocation));
	spVariantArray->Add(_variant_t(bstrOutputFeatureClassName));
	spVariantArray->Add(_variant_t(bstrWhereClause));
	hr = spGeoProcessor->Execute(_bstr_t(_T("FeatureClassToFeatureClass_conversion")),spVariantArray,NULL,&spGeoProcessorResult);
	if (FAILED(hr))
	{
		CComBSTR bstrErrorMsg;
		spGeoProcessor->GetMessage(-1,&bstrErrorMsg);
		m_strGPErrorInfo = bstrErrorMsg;
	}
	return hr;
}

HRESULT CGPOperation::GPIdentify(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<IFeatureClass> spInputFeatureClass,CComQIPtr<IFeatureClass> spIdentifyFeatureClass,BSTR bstrOutputFeatureClassName,VARIANT_BOOL bOverwriteOutput /* = VARIANT_TRUE */)
{
	HRESULT hr = S_FALSE;
	/*CComQIPtr<IGeoProcessor> spGeoProcessor;
	spGeoProcessor.CoCreateInstance(CLSID_GeoProcessor);*/
	if(spGeoProcessor == NULL)spGeoProcessor.CoCreateInstance(CLSID_GeoProcessor);
	spGeoProcessor->put_OverwriteOutput(bOverwriteOutput);
	CComQIPtr<IVariantArray> spVariantArray;
	spVariantArray.CoCreateInstance(CLSID_VarArray);
	CComQIPtr<IGeoProcessorResult> spGeoProcessorResult;
	spVariantArray->Add(_variant_t(spInputFeatureClass,TRUE));
	spVariantArray->Add(_variant_t(spIdentifyFeatureClass,TRUE));
	spVariantArray->Add(_variant_t(bstrOutputFeatureClassName));
	hr = spGeoProcessor->Execute(_bstr_t(_T("Identity_analysis")),spVariantArray,NULL,&spGeoProcessorResult);
	if (FAILED(hr))
	{
		CComBSTR bstrErrorMsg;
		spGeoProcessor->GetMessage(-1,&bstrErrorMsg);
		m_strGPErrorInfo = bstrErrorMsg;
	}
	return hr;
}

HRESULT CGPOperation::GPMultipart2Singlepart(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<IFeatureClass> spInputFeatureClass,BSTR bstrOutFeatureClass,VARIANT_BOOL bOverwriteOutput /* = VARIANT_TRUE */)
{
	HRESULT hr = S_FALSE;
	/*CComQIPtr<IGeoProcessor> spGeoProcessor;
	spGeoProcessor.CoCreateInstance(CLSID_GeoProcessor);*/
	if(spGeoProcessor == NULL)spGeoProcessor.CoCreateInstance(CLSID_GeoProcessor);
	spGeoProcessor->put_OverwriteOutput(bOverwriteOutput);
	CComQIPtr<IVariantArray> spVariantArray;
	spVariantArray.CoCreateInstance(CLSID_VarArray);
	CComQIPtr<IGeoProcessorResult> spGeoProcessorResult;
	spVariantArray->Add(_variant_t(spInputFeatureClass,TRUE));
	spVariantArray->Add(_variant_t(bstrOutFeatureClass));
	hr = spGeoProcessor->Execute(_bstr_t(_T("MultipartToSinglepart_management")),spVariantArray,NULL,&spGeoProcessorResult);
	if (FAILED(hr))
	{
		CComBSTR bstrErrorMsg;
		spGeoProcessor->GetMessage(-1,&bstrErrorMsg);
		m_strGPErrorInfo = bstrErrorMsg;
	}
	return hr;
}

HRESULT CGPOperation::GPSummary(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<ITable> spInputTable,BSTR bstrOutputTable,CComQIPtr<IGpValueTableObject> spSummaryFields,CComQIPtr<IGpValueTableObject> spCaseFields,VARIANT_BOOL bOverwriteOutput /* = VARIANT_TRUE */ )
{
	HRESULT hr = S_FALSE;
	/*CComQIPtr<IGeoProcessor> spGeoProcessor;
	spGeoProcessor.CoCreateInstance(CLSID_GeoProcessor);*/
	if(spGeoProcessor == NULL)spGeoProcessor.CoCreateInstance(CLSID_GeoProcessor);
	spGeoProcessor->put_OverwriteOutput(bOverwriteOutput);
	CComQIPtr<IVariantArray> spVariantArray;
	spVariantArray.CoCreateInstance(CLSID_VarArray);
	CComQIPtr<IGeoProcessorResult> spGeoProcessorResult;
	spVariantArray->Add(_variant_t(spInputTable,TRUE));
	spVariantArray->Add(_variant_t(bstrOutputTable));
	spVariantArray->Add(_variant_t(spSummaryFields,TRUE));
	spVariantArray->Add(_variant_t(spCaseFields,TRUE));
	hr = spGeoProcessor->Execute(_bstr_t(_T("Statistics_analysis")),spVariantArray,NULL,&spGeoProcessorResult);
	if (FAILED(hr))
	{
		CComBSTR bstrErrorMsg;
		spGeoProcessor->GetMessage(-1,&bstrErrorMsg);
		m_strGPErrorInfo = bstrErrorMsg;
	}
	return hr;
}

HRESULT CGPOperation::GPCalculateField(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<ITable> spInputTable,BSTR bstrFieldName,BSTR bstrExpression,BSTR bstrExpressionType,VARIANT_BOOL bOverwriteOutput /* = VARIANT_TRUE */)
{
	HRESULT hr = S_FALSE;
	/*CComQIPtr<IGeoProcessor> spGeoProcessor;
	spGeoProcessor.CoCreateInstance(CLSID_GeoProcessor);*/
	if(spGeoProcessor == NULL)spGeoProcessor.CoCreateInstance(CLSID_GeoProcessor);
	spGeoProcessor->put_OverwriteOutput(bOverwriteOutput);
	CComQIPtr<IVariantArray> spVariantArray;
	spVariantArray.CoCreateInstance(CLSID_VarArray);
	CComQIPtr<IGeoProcessorResult> spGeoProcessorResult;
	spVariantArray->Add(_variant_t(spInputTable,TRUE));
	spVariantArray->Add(_variant_t(bstrFieldName));
	spVariantArray->Add(_variant_t(bstrExpression));
	spVariantArray->Add(_variant_t(bstrExpressionType));
	hr = spGeoProcessor->Execute(_bstr_t(_T("CalculateField_management")),spVariantArray,NULL,&spGeoProcessorResult);
	if(FAILED(hr))
	{
		CComBSTR bstrErrorMsg;
		spGeoProcessor->GetMessage(-1,&bstrErrorMsg);
		m_strGPErrorInfo = bstrErrorMsg;
	}
	return hr;
}

HRESULT CGPOperation::GPJoinField(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<ITable> spInJoinTable,BSTR bstrInJoinField,CComQIPtr<ITable> spOutJoinTable,BSTR bstrOutJoinField,CComQIPtr<IGpValueTableObject> spJoinFields,VARIANT_BOOL bOverwriteOutput /* = VARIANT_TRUE */)
{
	HRESULT hr = S_FALSE;
	/*CComQIPtr<IGeoProcessor> spGeoProcessor;
	spGeoProcessor.CoCreateInstance(CLSID_GeoProcessor);*/
	if(spGeoProcessor == NULL)spGeoProcessor.CoCreateInstance(CLSID_GeoProcessor);
	spGeoProcessor->put_OverwriteOutput(bOverwriteOutput);
	CComQIPtr<IVariantArray> spVariantArray;
	spVariantArray.CoCreateInstance(CLSID_VarArray);
	CComQIPtr<IGeoProcessorResult> spGeoProcessorResult;
	spVariantArray->Add(_variant_t(spInJoinTable,TRUE));
	spVariantArray->Add(_variant_t(bstrInJoinField));
	spVariantArray->Add(_variant_t(spOutJoinTable,TRUE));
	spVariantArray->Add(_variant_t(bstrOutJoinField));
	spVariantArray->Add(_variant_t(spJoinFields,TRUE));
	hr = spGeoProcessor->Execute(_bstr_t(_T("JoinField_management")),spVariantArray,NULL,&spGeoProcessorResult);
	if(FAILED(hr))
	{
		CComBSTR bstrErrorMsg;
		spGeoProcessor->GetMessage(-1,&bstrErrorMsg);
		m_strGPErrorInfo = bstrErrorMsg;
	}
	return hr;
}

HRESULT CGPOperation::GPTable2Table(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<ITable> spInTable,BSTR bstrOutLocation,BSTR bstrTableName,BSTR bstrExpression,CComQIPtr<IGpValueTableObject> spFieldsMap,VARIANT_BOOL bOverwriteOutput /* = VARIANT_TRUE */)
{
	HRESULT hr = S_FALSE;
	/*CComQIPtr<IGeoProcessor> spGeoProcessor;
	spGeoProcessor.CoCreateInstance(CLSID_GeoProcessor);*/
	if(spGeoProcessor == NULL)spGeoProcessor.CoCreateInstance(CLSID_GeoProcessor);
	spGeoProcessor->put_OverwriteOutput(bOverwriteOutput);
	CComQIPtr<IVariantArray> spVariantArray;
	spVariantArray.CoCreateInstance(CLSID_VarArray);
	CComQIPtr<IGeoProcessorResult> spGeoProcessorResult;
	spVariantArray->Add(_variant_t(spInTable,TRUE));
	spVariantArray->Add(_variant_t(bstrOutLocation));
	spVariantArray->Add(_variant_t(bstrTableName));
	spVariantArray->Add(_variant_t(bstrExpression));
	spVariantArray->Add(_variant_t(spFieldsMap,TRUE));
	hr = spGeoProcessor->Execute(_bstr_t(_T("TableToTable_conversion")),spVariantArray,NULL,&spGeoProcessorResult);
	if(FAILED(hr))
	{
		CComBSTR bstrErrorMsg;
		spGeoProcessor->GetMessage(-1,&bstrErrorMsg);
		m_strGPErrorInfo = bstrErrorMsg;
	}
	return hr;
}

HRESULT CGPOperation::GPDissolve(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<IFeatureClass> spInputFeatureClass,BSTR bstrOutputFeatureClass,CComQIPtr<IGpValueTableObject> spDissolveFields,CComQIPtr<IGpValueTableObject> spStatisticsFields /* = NULL */,VARIANT_BOOL bCreateMultiFeatures /* = VARIANT_TRUE */,VARIANT_BOOL bSplitLines /* = VARIANT_TRUE */,VARIANT_BOOL bOverwriteOutput /* = VARIANT_TRUE */)
{
	HRESULT hr = S_OK;
	if(spGeoProcessor == NULL)spGeoProcessor.CoCreateInstance(CLSID_GeoProcessor);
	spGeoProcessor->put_OverwriteOutput(bOverwriteOutput);
	CComQIPtr<IVariantArray> spVariantArray;
	spVariantArray.CoCreateInstance(CLSID_VarArray);
	CComQIPtr<IGeoProcessorResult> spGeoProcessorResult;
	spVariantArray->Add(_variant_t(spInputFeatureClass,TRUE));
	spVariantArray->Add(_variant_t(bstrOutputFeatureClass));
	spVariantArray->Add(_variant_t(spDissolveFields,TRUE));
	if(spStatisticsFields == NULL)spStatisticsFields.CoCreateInstance(CLSID_GpValueTableObject);
	spVariantArray->Add(_variant_t(spStatisticsFields,TRUE));
	//spVariantArray->Add(_variant_t(bCreateMultiFeatures));
	if(VARIANT_TRUE == bCreateMultiFeatures)
	{
		spVariantArray->Add(_variant_t(_T("MULTI_PART")));
	}
	else
	{
		spVariantArray->Add(_variant_t(_T("SINGLE_PART")));
	}
	spVariantArray->Add(_variant_t(bSplitLines));
	hr = spGeoProcessor->Execute(_bstr_t(_T("Dissolve_management")),spVariantArray,NULL,&spGeoProcessorResult);
	if(FAILED(hr))
	{
		CComBSTR bstrErrorMsg;
		spGeoProcessor->GetMessage(-1,&bstrErrorMsg);
		m_strGPErrorInfo = bstrErrorMsg;
	}
	return hr;
}

HRESULT CGPOperation::GPFrequency(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<ITable> spInputTable,BSTR bstrOutputTable,CComQIPtr<IGpValueTableObject> spFrequencyFields,CComQIPtr<IGpValueTableObject> spSummaryFields,VARIANT_BOOL bOverwriteOutput /* = VARIANT_TRUE */ )
{
	HRESULT hr = S_FALSE;
	if(spGeoProcessor == NULL)spGeoProcessor.CoCreateInstance(CLSID_GeoProcessor);
	spGeoProcessor->put_OverwriteOutput(bOverwriteOutput);
	CComQIPtr<IVariantArray> spVariantArray;
	spVariantArray.CoCreateInstance(CLSID_VarArray);
	CComQIPtr<IGeoProcessorResult> spGeoProcessorResult;
	spVariantArray->Add(_variant_t(spInputTable,TRUE));
	spVariantArray->Add(_variant_t(bstrOutputTable));
	spVariantArray->Add(_variant_t(spFrequencyFields,TRUE));
	if(spSummaryFields != NULL)
	{
		spVariantArray->Add(_variant_t(spSummaryFields,TRUE));
	}
	hr = spGeoProcessor->Execute(_bstr_t(_T("Frequency_analysis")),spVariantArray,NULL,&spGeoProcessorResult);
	if (FAILED(hr))
	{
		CComBSTR bstrErrorMsg;
		spGeoProcessor->GetMessage(-1,&bstrErrorMsg);
		m_strGPErrorInfo = bstrErrorMsg;
	}
	return hr;
}