/*********************************************
文件名:  Global.h
创建者:  liuhao
编写日期:2016-11-10
简要描述:规划系统使用的通用操作，
修改记录(who-when-why):
********************************************/
#pragma once

#include <vector>
using namespace std;

class CConfigFile
{
public:
	//edit by lh in 2016.09.07 begin  
	//获取配置文件中“块”的所有键（key），如果文件无效、“块”不存在，则返回空数组的指针
	//static CStringArray* GetSectionKeys(CString strConfigFile,CString strSection);
	//获取“块”中所有的“键-值”对
	//static CMap<CString,LPCTSTR,CString,LPCTSTR>* GetAllSectionItems(CString strConfigFile,CString strSection);
	//将上述两个函数修改如下，避免内存需要手动释放的问题
	static void GetSectionKeys(CString strConfigFile,CString strSection,CStringArray& szArrSectionKeys);
	static void GetAllSectionItems(CString strConfigFile,CString strSection,CMap<CString,LPCTSTR,CString,LPCTSTR>& strMapConfig);
	//edit by  lh end

	//获取“块”中的键的值，如果指定的文件无效、“键”或者“块”不存在，则返回默认值（strDefault）
	static CString GetValue(CString strConfigFile,CString strSection,CString strKey,CString strDefault = _T(""));
	//检查配置文件在指定目录下是否存在
	static BOOL CheckFile(CString strConfigFile);
};


class CGlobalWorkspace
{
public:
	//使用IPropertySet来初始化配置文件中“TDGH”和“TDLY”所描述的SDE的工作空间
	static BOOL InitAllWorkspace(CComQIPtr<IPropertySet> ipGHPropertySet,CComQIPtr<IPropertySet> ipLYPropertySet);
	static BOOL InitGHWorkspace(CComQIPtr<IPropertySet> ipGHPropertySet);
	static BOOL InitLYWorkspace(CComQIPtr<IPropertySet> ipLYPropertySet);
	//获取工作空间
	////这里仅仅返回m_ipGHWorkspace或m_ipLYWorkspace，至于是否为空需要用户在使用的时候自己去判断
	static CComQIPtr<IWorkspace> GetGHWorkspace();//返回m_ipGHWorkspace
	static CComQIPtr<IWorkspace> GetLYWorkspace();//返回m_ipGHWorkspace
private:
	static CComQIPtr<IWorkspace> m_ipGHWorkspace;//土地规划工作空间（建设用地管制区工作空间）
	static CComQIPtr<IWorkspace> m_ipLYWorkspace;//土地现状工作空间（土地利用工作空间）
	
};

//定义通用函数VARIANT类型到CString类型的转换
extern CString VariantToString(VARIANT var);


//用于管理智能指针的数组
class CSPUnknownVector
{
public:
	CSPUnknownVector();//初始化数组
	//对于重载赋值运算符的最好也也同事重载拷贝构造函数
	/*CSPUnknownVector vec1(vec2);//拷贝构造函数
	CSPUnknownVector vec1 = vec2;//拷贝构造函数
	CSPUnknownVector vec1;
	vec1 = vec2;//赋值运算符*/
	CSPUnknownVector(CSPUnknownVector& vecSPUnknownContainer);
	~CSPUnknownVector();//释放之智能指针数组
public:
	void		Push(IUnknown*);
	IUnknown*	GetAt(int);
	void		Clear();
	int	Count();
	//重载赋值运算符“=”，最好也同时重载拷贝构造函数
	CSPUnknownVector& operator = (CSPUnknownVector& vecSPUnknownContainer);
	//使用数组成员变量来存储智能指针值
private:
	vector<IUnknown*> m_vectorPointer;
};


//封装通用的Arcgis操作
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
	
	//打开工作空间(enmWSTShp、enmWSTMdb、enmWSTGdb、enmWSTSde)
	static HRESULT OpenWorkspaceFromFile(enmWorkspaceType workspaceType,BSTR bstrFilePath,CComQIPtr<IWorkspace>& spWorkspace);

	//创建工作空间(enmWSTShp、enmWSTMdb、enmWSTGdb、enmWSTSde)
	static HRESULT CreateWorkspace(enmWorkspaceType workspaceType,BSTR bstrFolder,BSTR bstrName,CComQIPtr<IWorkspaceName>& spWorkspaceName);

	//创建Feature Dataset
	static HRESULT CreateFeatureDataset(CComQIPtr<IFeatureWorkspace> spFeatureWorkspace,CComQIPtr<ISpatialReference> spSpatialRefenence,BSTR bstrName,CComQIPtr<IFeatureDataset> &spFeatureDataset);
	
	//创建空间参考
	static HRESULT CreateSpatialReference(CComQIPtr<ISpatialReference>& spSpatialReference,double dxMin=-450359962737.05,double dxMax=450359962737.05,double dyMin=-450359962737.05, double dyMax=450359962737.05,double  dxyTolerance=0.001);

	//给表添加字段
	static HRESULT  CGeometryOperation::AddFieldToTable(CComQIPtr<ITable> spTable,BSTR strFieldName,BSTR strFieldAliasName,esriFieldType  enmFieldType,long nLength,VARIANT_BOOL bNull);
};

class CGPOperation
{
public:
	CGPOperation();
	~CGPOperation();
public:
	//获取GP错误信息
	static CString GetGPOperatorErrorInfo();
	//调用GP操作（Intersect_analysis）
	static HRESULT GPIntersect(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<IGpValueTableObject> spInFeatures,BSTR bstrOutPutFeatureClass,VARIANT_BOOL bOverwriteOutput = VARIANT_TRUE);
	//调用GP操作（FeatureClassToFeatureClass_conversion）
	static HRESULT GPFeatureClass2FeatureClass(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<IFeatureClass> spInputFeatureClass,BSTR bstrOutputLocation,BSTR bstrOutputFeatureClassName,BSTR bstrWhereClause = NULL,VARIANT_BOOL bOverwirteOutput = VARIANT_TRUE);
	//调用GP操作（Identity_analysis）
	static HRESULT GPIdentify(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<IFeatureClass> spInputFeatureClass,CComQIPtr<IFeatureClass> spIdentifyFeatureClass,BSTR bstrOutputFeatureClassName,VARIANT_BOOL bOverwriteOutput = VARIANT_TRUE);
	//调用GP操作（MultipartToSinglepart_management）
	static HRESULT GPMultipart2Singlepart(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<IFeatureClass> spInputFeatureClass,BSTR bstrOutFeatureClass,VARIANT_BOOL bOverwriteOutput = VARIANT_TRUE);
	//调用GP操作（Statistics_analysis）
	static HRESULT GPSummary(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<ITable> spInputTable,BSTR bstrOutputTable,CComQIPtr<IGpValueTableObject> spSummaryFields,CComQIPtr<IGpValueTableObject> spCaseFields,VARIANT_BOOL bOverwriteOutput = VARIANT_TRUE );
	//调用GP操作（CalculateField_management）
	static HRESULT GPCalculateField(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<ITable> spInputTable,BSTR bstrFieldName,BSTR bstrExpression,BSTR bstrExpressionType,VARIANT_BOOL bOverwriteOutput = VARIANT_TRUE );
	//调用GP操作（JoinField_management）
	static HRESULT GPJoinField(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<ITable> spInJoinTable,BSTR bstrInJoinField,CComQIPtr<ITable> spOutJoinTable,BSTR bstrOutJoinField,CComQIPtr<IGpValueTableObject> spJoinFields,VARIANT_BOOL bOverwriteOutput = VARIANT_TRUE );
	//调用GP操作（Table to Table）
	//由于C++调用GP参数问题未处理好暂时不可用
	static HRESULT GPTable2Table(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<ITable> spInTable,BSTR bstrOutLocation,BSTR bstrTableName,BSTR bstrExpression,CComQIPtr<IGpValueTableObject> spFieldsMap,VARIANT_BOOL bOverwriteOutput = VARIANT_TRUE );
	//调用GP操作(Dissolve)
	static HRESULT GPDissolve(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<IFeatureClass> spInputFeatureClass,BSTR bstrOutputFeatureClass,CComQIPtr<IGpValueTableObject> spDissolveFields,CComQIPtr<IGpValueTableObject> spStatisticsFields = (IGpValueTableObject*)0,VARIANT_BOOL bCreateMultiFeatures = VARIANT_TRUE,VARIANT_BOOL bSplitLines = VARIANT_TRUE,VARIANT_BOOL bOverwriteOutput = VARIANT_TRUE );
	//调用GP操作（Frequency）
	static HRESULT GPFrequency(CComQIPtr<IGeoProcessor>& spGeoProcessor,CComQIPtr<ITable> spInputTable,BSTR bstrOutputTable,CComQIPtr<IGpValueTableObject> spFrequencyFields,CComQIPtr<IGpValueTableObject> spSummaryFields,VARIANT_BOOL bOverwriteOutput = VARIANT_TRUE );
private:
	static CString m_strGPErrorInfo;
};