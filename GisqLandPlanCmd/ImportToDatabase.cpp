// ImportToDatabase.cpp : CImportToDatabase 的实现

#include "stdafx.h"
#include "ImportToDatabase.h"


// CImportToDatabase


STDMETHODIMP CImportToDatabase::OnClick()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComQIPtr<GisqLib::_DGisqFrameworkCOM> spDGisqFramework = m_ipFramework;
	HWND hParent = (HWND)spDGisqFramework->GetParentHWnd();

	TCHAR szAppFullPath[MAX_PATH];
	DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(CHAR);
	GetModuleFileName(NULL,szAppFullPath,dBufferSize);
	CString strAppPath(szAppFullPath);
	strAppPath = strAppPath.Mid(0,strAppPath.ReverseFind('\\'));
	CString strConfigFile = strAppPath + _T("\\数据入库.exe");

	::ShellExecute(hParent,_T("open"),strConfigFile,NULL,NULL,SW_SHOW);
	return S_OK;
}