// LPRealtimeUpdateCmd.cpp : CLPRealtimeUpdateCmd ��ʵ��

#include "stdafx.h"
#include "LPRealtimeUpdateCmd.h"


// CLPRealtimeUpdateCmd

STDMETHODIMP CLPRealtimeUpdateCmd::OnClick()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CComQIPtr<GisqLib::_DGisqFrameworkCOM> spDGisqFramework = m_ipFramework;
	HWND hParent = (HWND)spDGisqFramework->GetParentHWnd();

	TCHAR szAppFullPath[MAX_PATH];
	DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(CHAR);
	GetModuleFileName(NULL,szAppFullPath,dBufferSize);
	CString strAppPath(szAppFullPath);
	strAppPath = strAppPath.Mid(0,strAppPath.ReverseFind('\\'));
	//CString strConfigFile = strAppPath + _T("\\�滮ϵͳ������\\�滮ϵͳ������.exe");
	CString strConfigFile = strAppPath + _T("\\ʵʱ����.exe");

	::ShellExecute(hParent,_T("open"),strConfigFile,NULL,NULL,SW_SHOW);
	return S_OK;
}