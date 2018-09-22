// ConvertXYData.h : CConvertXYData ������

#pragma once
#include "resource.h"       // ������

#include "GisqConvertXYDataToGeometry_i.h"
#include "..\include\Global.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
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

	FileCodeType CheckFile(CFile *pCoordFile);//��ȡ�����ļ�ָ��ı���������Unicode����asni
	BOOL ReadCoordFile(BSTR bstrTXTFilePath,CArray<CString,CString> *arstrTmp);//������Ԥ�����ı�������Cstring����
	int GetSpatialNum(BSTR strCoord);//��ȡ����
	void CreateSpatial(int nProjCS);//���ݸ����Ĵ��Ŵ����ռ�����
	void CreatFeatureClass(BSTR bstrFeatureClassName);//����Ҫ�ؼ�
	HRESULT ConvertCoordToFeature(CComQIPtr<IFeatureClass> spFtClass, CArray<CString,CString> &arstrTmp);//������ת��ΪҪ��
	HRESULT AddFieldsToFeature(CString &strFields, CComQIPtr<IFeature> spFt);//��ÿ��Ҫ�ص����н��������������е�������ӵ����Ա�
	HRESULT SaveFeature(CComQIPtr<IGeometry> spGeometry, CComQIPtr<IFeature> &spFt);//�������㼯�е����ݱ��浽����Ҫ����
	HRESULT PolygonCutHollow(CComQIPtr<IPointCollection> spPointCollection, CComQIPtr<IGeometry> spGeometry);//�и�Ҫ��
	HRESULT GetRing(CComQIPtr<IPointCollection> spPointCollection, CComQIPtr<IRing> &spRing);//�������ĵ㼯��ת��ΪȦ

	//add by lcc 2016.12.13 ���º������ڽ���MDB�ļ�
	HRESULT ReadMDBFile(BSTR bstrMDBFilePath,CArray<CString,CString> *arstrTmp);//����MDB������Cstring����
	void CreatFeatureClass2(BSTR bstrFeatureClassName);//����Ҫ�ؼ�
	HRESULT ReadFieldsFromMDB(CString &strFields, CComQIPtr<IFeature> spFt);//��ÿ��Ҫ�ص����н��������������е�������ӵ����Ա�
	HRESULT ConvertMDBCoordToFeature(CComQIPtr<IFeatureClass> spFtClass, CArray<CString,CString> &arstrTmp);//������ת��ΪҪ��
	HRESULT AddFieldsToFeature2(CString &strFields, CComQIPtr<IFeature> spFt);//��ÿ��Ҫ�ص����н��������������е�������ӵ����Ա�
	//add by lcc 2016.12.13 end
	HRESULT ReadMDBFile2(BSTR bstrMDBFilePath, BSTR bstrJZDTableName, CArray<CString,CString> *arstrTmp);//����MDB������Cstring����
	HRESULT AddGHYTDMToFeature(BSTR bstrGHGXDKTableName);
public:
	STDMETHOD(GetWorkspace)(IUnknown **ipWorkspace);
	STDMETHOD(GetDataset)(IUnknown **ipDataset);
	STDMETHOD(GetFeatureClass)(IUnknown **ipFeatureClass);
	STDMETHOD(GetSpatialRefrence)(IUnknown **ipSpatialRefrence);
	STDMETHOD(ConvertToFeatureClass)(BSTR bstrTXTFilePath, BSTR bstrDestFolder, BSTR bstrDestName, BSTR bstrDatasetName, BSTR bstrFeatureClassName);
	//add by lcc 2016.12.13 ���º������ڽ���MDB�ļ�
	STDMETHOD(ConvertMDBToFeatureClass)(BSTR bstrMDBFilePath, BSTR bstrDestFolder, BSTR bstrDestName, BSTR bstrDatasetName, BSTR bstrFeatureClassName);
	//add by lcc 2016.12.13 end
	//�����µı�׼mdbת��Ϊͼ�Σ��ṩ��ѡַ�����Ľӿ�
	STDMETHOD(ConvertJZDToPolygon)(	BSTR bstrMDBFilePath,
									BSTR bstrJZDTableName,
									BSTR bstrGHGXDKTableName,
									BSTR bstrTargetFolder,
									BSTR bstrGDBName, 
									BSTR bstrFeatureDatasetName,
									BSTR bstrPolygonName);
	//�ṩ��ʢ��.net�õĽӿ�
	STDMETHOD(ConvertJZDToPolygon2)(BSTR bstrMDBFilePath,
									BSTR bstrJZDTableName,
									BSTR bstrPolygonName);
	void CreatFeatureClass3(BSTR bstrFeatureClassName);//����Ҫ�ؼ�

};

OBJECT_ENTRY_AUTO(__uuidof(ConvertXYData), CConvertXYData)
