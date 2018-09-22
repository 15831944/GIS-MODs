// LPXZQDataTree.h : CLPXZQDataTree 的声明

#pragma once
#include "resource.h"       // 主符号

#include "GisqLandPlanCmd_i.h"
#include "..\include\GlobalDefine.h"
#include <vector>
#include "..\include\tinyxml.h"
#include "..\include\Global.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif


static void GetLevelandParent(CString strDM, CString strMC, int *nLevel, CString *pstrParent);
// CLPXZQDataItem

class ATL_NO_VTABLE CLPXZQDataItem :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPXZQDataItem, &CLSID_LPXZQDataItem>,
	public IDispatchImpl<ILPXZQDataItem, &IID_ILPXZQDataItem, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public _tagDistrictItem
{
public:
	CLPXZQDataItem():_tagDistrictItem()
	{
		m_ChildItemsEnum.CoCreateInstance(CLSID_LPXZQDataEnum);
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_LPXZQDATAITEM)


	BEGIN_COM_MAP(CLPXZQDataItem)
		COM_INTERFACE_ENTRY(ILPXZQDataItem)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		DWORD dw = ::GetLastError();
		return S_OK;
	}

	void FinalRelease()
	{
	}
private:

	CComQIPtr<ILPXZQDataEnum> m_ChildItemsEnum;
public:

	STDMETHOD(Get_ItemName)(BSTR* bstrName);
	STDMETHOD(Set_ItemName)(BSTR bstrName);
	STDMETHOD(Get_ItemCode)(BSTR* bstrCode);
	STDMETHOD(Set_ItemCode)(BSTR bstrCode);
	STDMETHOD(Get_ItemParentID)(BSTR* bstrParentID);
	STDMETHOD(Set_ItemParentID)(BSTR bstrParentID);
	STDMETHOD(Get_ItemLevel)(SHORT* snLevel);
	STDMETHOD(Set_ItemLevel)(SHORT snLevel);
	STDMETHOD(InsertTreeItem)(IUnknown* pParentItem, IUnknown* DataMap);
	STDMETHOD(Get_ChildItems)(IUnknown** ChildItemEnum);
	STDMETHOD(InsertTreeItem2)(IUnknown* pNewitem);
};

OBJECT_ENTRY_AUTO(__uuidof(LPXZQDataItem), CLPXZQDataItem)

// CLPXZQDataEnum

class ATL_NO_VTABLE CLPXZQDataEnum :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPXZQDataEnum, &CLSID_LPXZQDataEnum>,
	public IDispatchImpl<ILPXZQDataEnum, &IID_ILPXZQDataEnum, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CLPXZQDataEnum()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_LPXZQDATAENUM)


	BEGIN_COM_MAP(CLPXZQDataEnum)
		COM_INTERFACE_ENTRY(ILPXZQDataEnum)
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
	CArray<CComQIPtr<ILPXZQDataItem>,CComQIPtr<ILPXZQDataItem>> ChildItemsArray;
public:

	STDMETHOD(InsertItem)(IUnknown* pNewItem, IUnknown* DataMap);
	STDMETHOD(GetItemCount)(int* nCount);
	STDMETHOD(GetItemAt)(int nItem,IUnknown** pItem);

	STDMETHOD(InsertItem2)(IUnknown* pNewItem);
};

OBJECT_ENTRY_AUTO(__uuidof(LPXZQDataEnum), CLPXZQDataEnum)


// CLPXZQDataTree

class ATL_NO_VTABLE CLPXZQDataTree :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPXZQDataTree, &CLSID_LPXZQDataTree>,
	public IDispatchImpl<ILPXZQDataTree, &IID_ILPXZQDataTree, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CLPXZQDataTree()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LPXZQDATATREE)


BEGIN_COM_MAP(CLPXZQDataTree)
	COM_INTERFACE_ENTRY(ILPXZQDataTree)
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
	static CComQIPtr<ILPXZQDataItem> XZQTreeRoot;
	static BOOL bIsXZQInit;
	static CString m_strParentID;
	static CMapStringToString *m_mapXZQ;
	static HRESULT InitXZQTree(CComQIPtr<IFeatureWorkspace> spFtWorkspace);//如果get时行政区树尚未创建
	static HRESULT InitXZQTree2(CComQIPtr<IFeatureWorkspace> spFtWorkspace);//如果get时行政区树尚未创建
	static HRESULT CreateDataTree(CString strParentID);
	static HRESULT ConnToOracle(_ConnectionPtr ipADOConn);
	static HRESULT getConnString(CComBSTR &bstrConnSet,CComBSTR &bstrName,CComBSTR &bstrPasswd);
public:

	STDMETHOD(GetRootItem)(IUnknown* pFtWorkSpace, IUnknown** RootItem);
	STDMETHOD(GetXZQMap)(IUnknown** mapXZQData);

};

OBJECT_ENTRY_AUTO(__uuidof(LPXZQDataTree), CLPXZQDataTree)

