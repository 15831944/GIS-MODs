// LPAttrSearchCmd.h : CLPAttrSearchCmd 的声明

#pragma once
#include "resource.h"       // 主符号

#include "GisqLandPlanCmd_i.h"
#include "BaseCommand.h"
#include "LPAttrSearchDlg.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif



// CLPAttrSearchCmd

class ATL_NO_VTABLE CLPAttrSearchCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLPAttrSearchCmd, &CLSID_LPAttrSearchCmd>,
	public IDispatchImpl<ILPAttrSearchCmd, &IID_ILPAttrSearchCmd, &LIBID_GisqLandPlanCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CLPAttrSearchCmd():CBaseCommand(0,_T("属性查询"),NULL,_T("属性查询"),_T("属性查询")
		,_T("属性查询"),_T("属性查询"))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LPATTRSEARCHCMD)


BEGIN_COM_MAP(CLPAttrSearchCmd)
	COM_INTERFACE_ENTRY(ILPAttrSearchCmd)
	COM_INTERFACE_ENTRY2(IDispatch, IGSCommandEx)
	COM_INTERFACE_ENTRY(IGSCommandEx)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
// 		HWND hwndImpDlg = NULL;
// 		hwndImpDlg = FindWindow(NULL,_T("规划要素查询"));
// 		if (hwndImpDlg != NULL)
// 		{
// 			CWnd *CXDlg;
// 			CXDlg = CWnd::FromHandle(hwndImpDlg);
// 			CXDlg->SendMessage(WM_DESTROY);
// 		}
	}

public:
	STDMETHOD(OnClick)();
private:
	CLPAttrSearchDlg *m_AttrSearchDlg;
};

OBJECT_ENTRY_AUTO(__uuidof(LPAttrSearchCmd), CLPAttrSearchCmd)
