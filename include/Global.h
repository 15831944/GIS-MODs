/*********************************************
�ļ���:  Global.h
������:  liuhao
��д����:2016-11-10
��Ҫ����:�滮ϵͳʹ�õ�ͨ�ò�����
�޸ļ�¼(who-when-why):
********************************************/
#pragma once

#include <vector>
using namespace std;

class CConfigFile
{
public:
	//edit by lh in 2016.09.07 begin  
	//��ȡ�����ļ��С��顱�����м���key��������ļ���Ч�����顱�����ڣ��򷵻ؿ������ָ��
	//static CStringArray* GetSectionKeys(CString strConfigFile,CString strSection);
	//��ȡ���顱�����еġ���-ֵ����
	//static CMap<CString,LPCTSTR,CString,LPCTSTR>* GetAllSectionItems(CString strConfigFile,CString strSection);
	//���������������޸����£������ڴ���Ҫ�ֶ��ͷŵ�����
	static void GetSectionKeys(CString strConfigFile,CString strSection,CStringArray& szArrSectionKeys);
	static void GetAllSectionItems(CString strConfigFile,CString strSection,CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig);
	//edit by  lh end

	//��ȡ���顱�еļ���ֵ�����ָ�����ļ���Ч�����������ߡ��顱�����ڣ��򷵻�Ĭ��ֵ��strDefault��
	static CString GetValue(CString strConfigFile,CString strSection,CString strKey,CString strDefault = _T(""));
	//��������ļ���ָ��Ŀ¼���Ƿ����
	static BOOL CheckFile(CString strConfigFile);
};


class CGlobalWorkspace
{
public:
	//ʹ��IPropertySet����ʼ�������ļ��С�TDGH���͡�TDLY����������SDE�Ĺ����ռ�
	static BOOL InitAllWorkspace(CComQIPtr<IPropertySet> ipGHPropertySet,CComQIPtr<IPropertySet> ipLYPropertySet);
	static BOOL InitGHWorkspace(CComQIPtr<IPropertySet> ipGHPropertySet);
	static BOOL InitLYWorkspace(CComQIPtr<IPropertySet> ipLYPropertySet);
	//��ȡ�����ռ�
	////�����������m_ipGHWorkspace��m_ipLYWorkspace�������Ƿ�Ϊ����Ҫ�û���ʹ�õ�ʱ���Լ�ȥ�ж�
	static CComQIPtr<IWorkspace> GetGHWorkspace();//����m_ipGHWorkspace
	static CComQIPtr<IWorkspace> GetLYWorkspace();//����m_ipGHWorkspace
private:
	static CComQIPtr<IWorkspace> m_ipGHWorkspace;//���ع滮�����ռ䣨�����õع����������ռ䣩
	static CComQIPtr<IWorkspace> m_ipLYWorkspace;//������״�����ռ䣨�������ù����ռ䣩
	
};

//����ͨ�ú���VARIANT���͵�CString���͵�ת��
extern CString VariantToString(VARIANT var);


//���ڹ�������ָ�������
class CSPUnknownVector
{
public:
	CSPUnknownVector();//��ʼ������
	//�������ظ�ֵ����������ҲҲͬ�����ؿ������캯��
	/*CSPUnknownVector vec1(vec2);//�������캯��
	CSPUnknownVector vec1 = vec2;//�������캯��
	CSPUnknownVector vec1;
	vec1 = vec2;//��ֵ�����*/
	CSPUnknownVector(CSPUnknownVector& vecSPUnknownContainer);
	~CSPUnknownVector();//�ͷ�֮����ָ������
public:
	void		Push(IUnknown*);
	IUnknown*	GetAt(int);
	void		Clear();
	int	Count();
	//���ظ�ֵ�������=�������Ҳͬʱ���ؿ������캯��
	CSPUnknownVector& operator = (CSPUnknownVector& vecSPUnknownContainer);
	//ʹ�������Ա�������洢����ָ��ֵ
private:
	vector<IUnknown*> m_vectorPointer;
};


//��װͨ�õ�Arcgis����
enum  enmWorkspaceType
{
	enmWSTShp,
	enmWSTMdb,
	enmWSTGdb,
	enmWSTSde
};
class CGeometryOperation
{
public:
	CGeometryOperation();
	~CGeometryOperation();
public:
	
	//�򿪹����ռ�(enmWSTShp��enmWSTMdb��enmWSTGdb��enmWSTSde)
	static HRESULT OpenWorkspaceFromFile(enmWorkspaceType workspaceType,BSTR bstrFilePath,CComQIPtr<IWorkspace>& spWorkspace);

	//���������ռ�(enmWSTShp��enmWSTMdb��enmWSTGdb��enmWSTSde)
	static HRESULT CreateWorkspace(enmWorkspaceType workspaceType,BSTR bstrFolder,BSTR bstrName,CComQIPtr<IWorkspaceName>& spWorkspaceName);

	//����Feature Dataset
	static HRESULT CreateFeatureDataset(CComQIPtr<IFeatureWorkspace> spFeatureWorkspace,CComQIPtr<ISpatialReference> spSpatialRefenence,BSTR bstrName,CComQIPtr<IFeatureDataset> &spFeatureDataset);
	
	//�����ռ�ο�
	static HRESULT CreateSpatialReference(CComQIPtr<ISpatialReference>& spSpatialReference,double dxMin=-450359962737.05,double dxMax=450359962737.05,double dyMin=-450359962737.05, double dyMax=450359962737.05,double  dxyTolerance=0.001);

	//��������ֶ�
	static HRESULT  CGeometryOperation::AddFieldToTable(CComQIPtr<ITable> spTable,BSTR strFieldName,BSTR strFieldAliasName,esriFieldType  enmFieldType,long nLength,VARIANT_BOOL bNull);
};

class CGPOperation
{
public:
	CGPOperation();
	~CGPOperation();
public:
	//��ȡGP������Ϣ
	static CString GetGPOperatorErrorInfo();
	//����GP������Intersect_analysis��
	static HRESULT GPIntersect(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<IGpValueTableObject> spInFeatures,BSTR bstrOutPutFeatureClass,VARIANT_BOOL bOverwriteOutput = VARIANT_TRUE);
	//����GP������FeatureClassToFeatureClass_conversion��
	static HRESULT GPFeatureClass2FeatureClass(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<IFeatureClass> spInputFeatureClass,BSTR bstrOutputLocation,BSTR bstrOutputFeatureClassName,BSTR bstrWhereClause = NULL,VARIANT_BOOL bOverwirteOutput = VARIANT_TRUE);
	//����GP������Identity_analysis��
	static HRESULT GPIdentify(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<IFeatureClass> spInputFeatureClass,CComQIPtr<IFeatureClass> spIdentifyFeatureClass,BSTR bstrOutputFeatureClassName,VARIANT_BOOL bOverwriteOutput = VARIANT_TRUE);
	//����GP������MultipartToSinglepart_management��
	static HRESULT GPMultipart2Singlepart(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<IFeatureClass> spInputFeatureClass,BSTR bstrOutFeatureClass,VARIANT_BOOL bOverwriteOutput = VARIANT_TRUE);
	//����GP������Statistics_analysis��
	static HRESULT GPSummary(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<ITable> spInputTable,BSTR bstrOutputTable,CComQIPtr<IGpValueTableObject> spSummaryFields,CComQIPtr<IGpValueTableObject> spCaseFields,VARIANT_BOOL bOverwriteOutput = VARIANT_TRUE );
	//����GP������CalculateField_management��
	static HRESULT GPCalculateField(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<ITable> spInputTable,BSTR bstrFieldName,BSTR bstrExpression,BSTR bstrExpressionType,VARIANT_BOOL bOverwriteOutput = VARIANT_TRUE );
	//����GP������JoinField_management��
	static HRESULT GPJoinField(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<ITable> spInJoinTable,BSTR bstrInJoinField,CComQIPtr<ITable> spOutJoinTable,BSTR bstrOutJoinField,CComQIPtr<IGpValueTableObject> spJoinFields,VARIANT_BOOL bOverwriteOutput = VARIANT_TRUE );
	//����GP������Table to Table��
	//����C++����GP��������δ�������ʱ������
	static HRESULT GPTable2Table(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<ITable> spInTable,BSTR bstrOutLocation,BSTR bstrTableName,BSTR bstrExpression,CComQIPtr<IGpValueTableObject> spFieldsMap,VARIANT_BOOL bOverwriteOutput = VARIANT_TRUE );
	//����GP����(Dissolve)
	static HRESULT GPDissolve(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<IFeatureClass> spInputFeatureClass,BSTR bstrOutputFeatureClass,CComQIPtr<IGpValueTableObject> spDissolveFields,CComQIPtr<IGpValueTableObject> spStatisticsFields = (IGpValueTableObject*)0,VARIANT_BOOL bCreateMultiFeatures = VARIANT_TRUE,VARIANT_BOOL bSplitLines = VARIANT_TRUE,VARIANT_BOOL bOverwriteOutput = VARIANT_TRUE );
	//����GP������Frequency��
	static HRESULT GPFrequency(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<ITable> spInputTable,BSTR bstrOutputTable,CComQIPtr<IGpValueTableObject> spFrequencyFields,CComQIPtr<IGpValueTableObject> spSummaryFields,VARIANT_BOOL bOverwriteOutput = VARIANT_TRUE );
private:
	static CString m_strGPErrorInfo;
};