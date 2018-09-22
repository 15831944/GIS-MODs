// LPGridDockWinCmd.cpp : CLPGridDockWinCmd 的实现

#include "stdafx.h"
#include "LPGridDockWinCmd.h"


// CLPGridDockWinCmd



STDMETHODIMP CLPGridDockWinCmd::get_Caption (/*[out,retval]*/ BSTR * strCaption )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (strCaption == NULL)
	{
		return E_FAIL;
	}
	*strCaption=m_strCaption.AllocSysString();
	return S_OK;
}
STDMETHODIMP CLPGridDockWinCmd::get_ChildHWnd (/*[out,retval]*/ OLE_HANDLE * hWnd )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (hWnd == NULL)
	{
		return E_POINTER;
	}
	if (m_pGridDiolog != NULL)
	{
		*hWnd = (OLE_HANDLE)m_pGridDiolog->GetSafeHwnd();
		return S_OK;
	}
	return E_FAIL;
}
STDMETHODIMP CLPGridDockWinCmd::get_Name (/*[out,retval]*/ BSTR * strName )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (strName == NULL)
	{
		return E_FAIL;
	}
	*strName=m_strName.AllocSysString();
	return S_OK;
}
STDMETHODIMP CLPGridDockWinCmd::OnCreate (/*[in]*/ IDispatch * lpFrameworkCOM )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_pFrameworkCom = lpFrameworkCOM;
	if (m_pGridDiolog == NULL)
	{
		//CWnd* hMainWnd = AfxGetMainWnd();
		
		CComQIPtr<GisqLib::_DGisqFrameworkCOM> spDGisqFramework = m_pFrameworkCom;
		CWnd* cMainWnd = CWnd::FromHandle((HWND)spDGisqFramework->GetParentHWnd());
		m_pGridDiolog = new CLPGridDockWinDlg(m_pFrameworkCom,cMainWnd);
		m_pGridDiolog->Create(CLPGridDockWinDlg::IDD,cMainWnd);
	}
	return S_OK;
}
STDMETHODIMP CLPGridDockWinCmd::OnDestroy ( )
{
	if (m_pGridDiolog != NULL)
	{
		m_pGridDiolog->SendMessage(WM_DESTROY);
	}
	return S_OK;

}

STDMETHODIMP CLPGridDockWinCmd::SetQueryFilter(IUnknown* spQueryFilter, BSTR bstrFeatureClassName, BSTR bstrTabName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (spQueryFilter == NULL || bstrFeatureClassName == NULL)
	{
		return E_POINTER;
	}
	if (bstrTabName == NULL)
	{
		bstrTabName = _T("查询结果");
	}
	m_pGridDiolog->SetQueryFilter(spQueryFilter,bstrFeatureClassName,bstrTabName);
	// TODO: 在此添加实现代码

	return S_OK;
}

STDMETHODIMP CLPGridDockWinCmd::SetDlgCaption(BSTR bstrCaption)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码
	m_strCaption.Format(_T("查询结果-%s"),bstrCaption);
	m_pGridDiolog->SetWindowText(m_strCaption);
	/*m_pGridDiolog->SetWindowRgn();*/
	m_pGridDiolog->UpdateWindow();
	return S_OK;
}
