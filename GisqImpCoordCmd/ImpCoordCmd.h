// ImpCoordCmd.h : CImpCoordCmd 的声明

#pragma once
#include "resource.h"       // 主符号

#include "GisqImpCoordCmd_i.h"
#include "BaseCommand.h"
#include "ImpCoordDlg.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif



// CImpCoordCmd

class ATL_NO_VTABLE CImpCoordCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CImpCoordCmd, &CLSID_ImpCoordCmd>,
	public IDispatchImpl<IImpCoordCmd, &IID_IImpCoordCmd, &LIBID_GisqImpCoordCmdLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CBaseCommand
{
public:
	CImpCoordCmd():CBaseCommand(0,_T("界址点导入"),NULL,_T("界址点导入"),_T("界址点导入")
		,_T("界址点导入"),_T("界址点导入"))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_IMPCOORDCMD)


BEGIN_COM_MAP(CImpCoordCmd)
	COM_INTERFACE_ENTRY(IImpCoordCmd)
	COM_INTERFACE_ENTRY2(IDispatch,IImpCoordCmd)
	COM_INTERFACE_ENTRY(IGSCommandEx)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
// 		HWND hwndExpDlg = NULL;
// 		hwndExpDlg = FindWindow(NULL,_T("界址点导入"));
// 		if (hwndExpDlg != NULL)
// 		{
// 			CWnd *ImpDlg;
// 			ImpDlg = CWnd::FromHandle(hwndExpDlg);
// 			//ImpDlg->SendMessage(WM_DESTROY);
// 			ImportDlg = (CImpCoordDlg *)ImpDlg;
// 			delete ImportDlg;
// 		}
	}

public:
	/*static */CImpCoordDlg *ImportDlg;
	STDMETHOD (OnClick)( );
};

OBJECT_ENTRY_AUTO(__uuidof(ImpCoordCmd), CImpCoordCmd)
