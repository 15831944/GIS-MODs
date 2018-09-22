// LPZTGHCmd.cpp : CLPZTGHCmd ��ʵ��

#include "stdafx.h"
#include "LPZTGHCmd.h"
#include "LPPrintMapSetsDlg.h"

// CLPZTGHCmd

// STDMETHODIMP CLPZTGHCmd::get_Enabled(VARIANT_BOOL * bEnabled )
// {
// 	//��ȡ"������"��Ϣ
// 	CComQIPtr<ICLPXZQWindowCmd> spCLPXZQWindowCmd;
// 	CComQIPtr<GisqLib::_DGisqFrameworkCOM> spGisqFramework = m_ipFramework;
// 	CComQIPtr<GisqLib::IGSDockableWindowManager> spGSDockableWindowManager = spGisqFramework->GetDockableWindowManager();
// 	short nCount = spGSDockableWindowManager->Count();
// 	for(short i = 0;i < nCount;i++)
// 	{
// 		CComQIPtr<GisqLib::IGSDockableWindow> spGSDockableWindow =	spGSDockableWindowManager->Item(i);
// 		_bstr_t strTabGUID = spGSDockableWindow->GetDockWindowDefGUID();
// 		if (strTabGUID == _bstr_t("GisqLandPlanCmd.CLPXZQWindowCmd.1"))
// 		{
// 			spCLPXZQWindowCmd = spGSDockableWindow->GetDockableWindowTabDef();
// 			break;
// 		}
// 
// 	}	
// 	if(spCLPXZQWindowCmd != NULL)
// 	{
// 		spCLPXZQWindowCmd->GetSelectedRegion(&m_bstrXZQName,&m_bstrXZQCode);
// 	}
// 	*bEnabled = m_bstrXZQCode == NULL ? VARIANT_FALSE:VARIANT_TRUE;
// 	return S_OK;
// }

STDMETHODIMP CLPZTGHCmd::OnClick()
{
	//add by lcc 2016.11.23 �����Դ�л�֧��
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//add end
	//��ȡ"������"��Ϣ
// 	CComQIPtr<ICLPXZQWindowCmd> spCLPXZQWindowCmd;
// 	CComQIPtr<GisqLib::_DGisqFrameworkCOM> spGisqFramework = m_ipFramework;
// 	CComQIPtr<GisqLib::IGSDockableWindowManager> spGSDockableWindowManager = spGisqFramework->GetDockableWindowManager();
// 	short nCount = spGSDockableWindowManager->Count();
// 	for(short i = 0;i < nCount;i++)
// 	{
// 		CComQIPtr<GisqLib::IGSDockableWindow> spGSDockableWindow =	spGSDockableWindowManager->Item(i);
// 		_bstr_t strTabGUID = spGSDockableWindow->GetDockWindowDefGUID();
// 		if (strTabGUID == _bstr_t("GisqLandPlanCmd.CLPXZQWindowCmd.1"))
// 		{
// 			spCLPXZQWindowCmd = spGSDockableWindow->GetDockableWindowTabDef();
// 			break;
// 		}
// 
// 	}	
// 	if(spCLPXZQWindowCmd != NULL)
// 	{
// 		spCLPXZQWindowCmd->GetSelectedRegion(&m_bstrXZQName,&m_bstrXZQCode);
// 	}
// 	if (m_bstrXZQCode == NULL)return S_OK;

	//���ع滮�����ռ䣨�����õع����������ռ䣩
	CComQIPtr<IWorkspace> spGHWorkspace;
	//������״�����ռ䣨�������ù����ռ䣩
	//CComQIPtr<IWorkspace> spLYWorkspace;
	CComQIPtr<GisqLib::_DGisqFrameworkCOM> spGisqFramework = m_ipFramework;
	CComQIPtr<GisqLib::IGSDataExchangeS> spGSDataExchangeS = spGisqFramework->GetDataExchangeS();
	//CComBSTR bstrTag(_T("GHWS"));
	short index = spGSDataExchangeS->FindIndex(_bstr_t(_T("GHWS")));
	if (index >= 0)
	{
		CComQIPtr<GisqLib::IGSDataExchange> spGSDataExchange = spGSDataExchangeS->Item(index);
		CComVariant var = spGSDataExchange->Item(0);
		spGHWorkspace = var.punkVal;
	}
// 	index = spGSDataExchangeS->FindIndex(_bstr_t(_T("LYWS")));
// 	if (index >= 0)
// 	{
// 		CComQIPtr<GisqLib::IGSDataExchange> spGSDataExchange = spGSDataExchangeS->Item(index);
// 		CComVariant var = spGSDataExchange->Item(0);
// 		spLYWorkspace = var.punkVal;
// 	}

	if (spGHWorkspace != NULL/* && spLYWorkspace != NULL*/)
	{
		//��������ģ��������ļ�����
		TCHAR szAppFullPath[MAX_PATH];
		DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(CHAR);
		GetModuleFileName(NULL,szAppFullPath,dBufferSize);
		CString strAppPath(szAppFullPath);
		strAppPath = strAppPath.Mid(0,strAppPath.ReverseFind('\\'));
		//CString strConfigFile = strAppPath + _T("\\data\\template\\������������滮ͼ.xml");
		CString strConfigFile = strAppPath + CONFIGPATH;
		CString strSection(_T("ZTGH"));

// 		CStringArray strArrDatasetName;
// 		strArrDatasetName.Add(_T("XZXW"));
// 		strArrDatasetName.Add(_T("KZBJM"));
// 		strArrDatasetName.Add(_T("XZQJX"));
// 		strArrDatasetName.Add(_T("XJXZQJX"));
// 		strArrDatasetName.Add(_T("XJXZQ"));
// 		strArrDatasetName.Add(_T("XZQ"));
// 		strArrDatasetName.Add(_T("GHYT"));

		CLPPrintMapSetsDlg dlg;
		//dlg.InitViewConfig(strConfigFile,strSection);
		//edit by lcc 20170208 �޸��˶�ȡ���õĽӿ�
		dlg.InitViewConfig2(strConfigFile,strSection);
		//edit by lcc end
		if (dlg.DoModal() == IDOK)
		{
			CExportThemeMap exportThemeMap((GisqLib::_DGisqFrameworkCOM*)m_ipFramework);
			//edit by lcc 2016.11.23 ��������Ϣ�ĴӴ�������
			//CString strXZQDM = m_bstrXZQCode;
			//exportThemeMap.ExportThemeMap2(spLYWorkspace,spGHWorkspace,strXZQDM,strConfigFile,strSection,strArrDatasetName);
			//��Ϊʹ�������ô��ڣ��޸�����ͼ����ӿ�
			//exportThemeMap.ExportThemeMap3(spLYWorkspace,spGHWorkspace,dlg.m_strXZQDM,dlg.m_strMapConfig,strArrDatasetName);
			//edit by lcc end
			//exportThemeMap.ExportThemeMap4(spGHWorkspace,dlg.m_strXZQDM,dlg.m_strMapConfig);
			//edit by lcc 20170208 ר��ͼ����Ӧ������ֽ��
			exportThemeMap.ExportThemeMap5(spGHWorkspace,dlg.m_strXZQDM,dlg.m_strMapConfig,dlg.m_bIsZXCQ,dlg.m_bIsLockScale);
			//edit by lcc end
		}
	}
	else
	{
		BCGPMessageBox(_T("�����ռ�δ��ʼ����"),_T("��ʾ"),MB_OK);
	}
	return S_OK;
}