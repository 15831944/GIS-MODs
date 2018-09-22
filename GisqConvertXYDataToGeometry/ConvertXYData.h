// ConvertXYData.h : CConvertXYData 的声明

#pragma once
#include "resource.h"       // 主符号

#include "GisqConvertXYDataToGeometry_i.h"
#include "..\include\Global.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif
using namespace ADO;
enum FileCodeType
{
	unkown = 0,
	utf8 = 1,
	unicode = 2,
	asni = 3
};
struct DKPointData
{
	DKPointData()
	{
		strDK_GUID = _T("");
		nDKH = 0;
		nQH = 0;
		nDH = 0;
		X = 0.0;
		Y = 0.0;
	}
	CString strDK_GUID;
	int nDKH;
	int nQH;
	int nDH;
	double X;
	double Y;
};
// CConvertXYData

class ATL_NO_VTABLE CConvertXYData :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CConvertXYData, &CLSID_ConvertXYData>,
	public IDispatchImpl<IConvertXYData, &IID_IConvertXYData, &LIBID_GisqConvertXYDataToGeometryLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CConvertXYData()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CONVERTXYDATA)


BEGIN_COM_MAP(CConvertXYData)
	COM_INTERFACE_ENTRY(IConvertXYData)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}
private:

	CComQIPtr<IWorkspace> m_spWorkspace;
	CComQIPtr<IFeatureDataset> m_spFtDataset;
	CComQIPtr<IFeatureClass> m_spFeatureClass;
	CComQIPtr<ISpatialReference> m_spSpatialRederence;
	_ConnectionPtr m_ipADOConn;
	CString m_strErrorString;

	FileCodeType CheckFile(CFile *pCoordFile);//获取给定文件指针的编码类型是Unicode还是asni
	BOOL ReadCoordFile(BSTR bstrTXTFilePath,CArray<CString,CString> *arstrTmp);//按编码预处理文本，返回Cstring数组
	int GetSpatialNum(BSTR strCoord);//获取带号
	void CreateSpatial(int nProjCS);//根据给定的带号创建空间坐标
	void CreatFeatureClass(BSTR bstrFeatureClassName);//创建要素集
	HRESULT ConvertCoordToFeature(CComQIPtr<IFeatureClass> spFtClass, CArray<CString,CString> &arstrTmp);//将数组转化为要素
	HRESULT AddFieldsToFeature(CString &strFields, CComQIPtr<IFeature> spFt);//将每个要素的首行解析出来，将其中的属性添加到属性表
	HRESULT SaveFeature(CComQIPtr<IGeometry> spGeometry, CComQIPtr<IFeature> &spFt);//将给定点集中的数据保存到给定要素中
	HRESULT PolygonCutHollow(CComQIPtr<IPointCollection> spPointCollection, CComQIPtr<IGeometry> spGeometry);//切割要素
	HRESULT GetRing(CComQIPtr<IPointCollection> spPointCollection, CComQIPtr<IRing> &spRing);//将给定的点集合转化为圈

	//add by lcc 2016.12.13 以下函数用于解析MDB文件
	HRESULT ReadMDBFile(BSTR bstrMDBFilePath,CArray<CString,CString> *arstrTmp);//解析MDB，返回Cstring数组
	void CreatFeatureClass2(BSTR bstrFeatureClassName);//创建要素集
	HRESULT ReadFieldsFromMDB(CString &strFields, CComQIPtr<IFeature> spFt);//将每个要素的首行解析出来，将其中的属性添加到属性表
	HRESULT ConvertMDBCoordToFeature(CComQIPtr<IFeatureClass> spFtClass, CArray<CString,CString> &arstrTmp);//将数组转化为要素
	HRESULT AddFieldsToFeature2(CString &strFields, CComQIPtr<IFeature> spFt);//将每个要素的首行解析出来，将其中的属性添加到属性表
	//add by lcc 2016.12.13 end
	HRESULT ReadMDBFile2(BSTR bstrMDBFilePath, BSTR bstrJZDTableName, CArray<CString,CString> *arstrTmp);//解析MDB，返回Cstring数组
	HRESULT AddGHYTDMToFeature(BSTR bstrGHGXDKTableName);
public:
	STDMETHOD(GetWorkspace)(IUnknown **ipWorkspace);
	STDMETHOD(GetDataset)(IUnknown **ipDataset);
	STDMETHOD(GetFeatureClass)(IUnknown **ipFeatureClass);
	STDMETHOD(GetSpatialRefrence)(IUnknown **ipSpatialRefrence);
	STDMETHOD(ConvertToFeatureClass)(BSTR bstrTXTFilePath, BSTR bstrDestFolder, BSTR bstrDestName, BSTR bstrDatasetName, BSTR bstrFeatureClassName);
	//add by lcc 2016.12.13 以下函数用于解析MDB文件
	STDMETHOD(ConvertMDBToFeatureClass)(BSTR bstrMDBFilePath, BSTR bstrDestFolder, BSTR bstrDestName, BSTR bstrDatasetName, BSTR bstrFeatureClassName);
	//add by lcc 2016.12.13 end
	//根据新的标准mdb转换为图形，提供给选址分析的接口
	STDMETHOD(ConvertJZDToPolygon)(	BSTR bstrMDBFilePath,
									BSTR bstrJZDTableName,
									BSTR bstrGHGXDKTableName,
									BSTR bstrTargetFolder,
									BSTR bstrGDBName, 
									BSTR bstrFeatureDatasetName,
									BSTR bstrPolygonName);
	//提供给盛琦.net用的接口
	STDMETHOD(ConvertJZDToPolygon2)(BSTR bstrMDBFilePath,
									BSTR bstrJZDTableName,
									BSTR bstrPolygonName);
	void CreatFeatureClass3(BSTR bstrFeatureClassName);//创建要素集

};

OBJECT_ENTRY_AUTO(__uuidof(ConvertXYData), CConvertXYData)
