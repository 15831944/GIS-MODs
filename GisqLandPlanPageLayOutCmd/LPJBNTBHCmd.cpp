// LPJBNTBHCmd.cpp : CLPJBNTBHCmd ��ʵ��

#include "stdafx.h"
#include "LPJBNTBHCmd.h"
#include "LPPrintMapSetsDlg.h"

// CLPJBNTBHCmd

// STDMETHODIMP CLPJBNTBHCmd::get_Enabled(VARIANT_BOOL * bEnabled )
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

STDMETHODIMP CLPJBNTBHCmd::OnClick()
{
	//add by lcc 2016.11.23 �����Դ�л�֧��
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//add end
	////�򿪹����ռ�(�滮�����ռ䡢��״�����ռ�)
	//CString strConnFile(_T("E:\\00.Դ����\\GisqLandUsePlan\\bin\\sysFile\\SysConfig.ini"));
	////�滮�����ռ�
	//CMap<CString,LPCTSTR,CString,LPCTSTR> strGHMapConnInfo;
	//CString strGHSection(_T("TDGH"));
	//CConfigFile::GetAllSectionItems(strConnFile,strGHSection,strGHMapConnInfo);
	//IPropertySetPtr ipLoginGHProperty(CLSID_PropertySet);
	//ipLoginGHProperty->SetProperty(_bstr_t(_T("SERVER")),_variant_t(strGHMapConnInfo[_T("IP")]));
	//ipLoginGHProperty->SetProperty(_bstr_t(_T("INSTANCE")),_variant_t(strGHMapConnInfo[_T("Sde")]));
	//ipLoginGHProperty->SetProperty(_bstr_t(_T("DATABASE")),_variant_t(strGHMapConnInfo[_T("Name")]));
	//ipLoginGHProperty->SetProperty(_bstr_t(_T("USER")),_variant_t(strGHMapConnInfo[_T("User")]));
	//ipLoginGHProperty->SetProperty(_bstr_t(_T("PASSWORD")),_variant_t(strGHMapConnInfo[_T("Password")]));
	//ipLoginGHProperty->SetProperty(_bstr_t(_T("VERSION")),_variant_t(_T("")));

	////��״�����ռ�
	//CMap<CString,LPCTSTR,CString,LPCTSTR> strXZMapConnInfo;
	//CString strXZSection(_T("TDLY"));
	//IPropertySetPtr ipLoginLYProperty(CLSID_PropertySet);

	//ipLoginLYProperty->SetProperty(_bstr_t(_T("SERVER")),_variant_t(strGHMapConnInfo[_T("IP")]));
	//ipLoginLYProperty->SetProperty(_bstr_t(_T("INSTANCE")),_variant_t(strGHMapConnInfo[_T("Sde")]));
	//ipLoginLYProperty->SetProperty(_bstr_t(_T("DATABASE")),_variant_t(strGHMapConnInfo[_T("Name")]));
	//ipLoginLYProperty->SetProperty(_bstr_t(_T("USER")),_variant_t(strGHMapConnInfo[_T("User")]));
	//ipLoginLYProperty->SetProperty(_bstr_t(_T("PASSWORD")),_variant_t(strGHMapConnInfo[_T("Password")]));
	//ipLoginLYProperty->SetProperty(_bstr_t(_T("VERSION")),_variant_t(_T("")));

	//CMap<CString,LPCTSTR,CString,LPCTSTR> strMapConfig;
	//CString strConfigFile(_T("E:\\GisqLandPlanPlatform\\bin\\data\\template\\ExportMap.ini"));
	//CString strSection(_T("JBNTBH"));
	//CConfigFile::GetAllSectionItems(strConfigFile,strSection,strMapConfig);

	//CStringArray strArrDatasetName;
	//strArrDatasetName.Add(_T("XZXW"));
	//strArrDatasetName.Add(_T("KZBJM"));
	//strArrDatasetName.Add(_T("XZQJX"));
	//strArrDatasetName.Add(_T("XJXZQJX"));
	//strArrDatasetName.Add(_T("XJXZQ"));
	//strArrDatasetName.Add(_T("XZQ"));
	//strArrDatasetName.Add(_T("JSYDGZQ"));
	//if(CGlobalWorkspace::InitAllWorkspace((IPropertySet*)ipLoginGHProperty,(IPropertySet*)ipLoginLYProperty))
	//{
	//	CExportThemeMap exportThemeMap((GisqLib::_DGisqFrameworkCOM*)m_ipFramework);
	//	exportThemeMap.ExportThemeMap(CGlobalWorkspace::GetGHWorkspace(),CGlobalWorkspace::GetLYWorkspace(),_T("330604101"),strMapConfig,strArrDatasetName);
	//}
	//���ع滮�����ռ䣨�����õع����������ռ䣩
	//if (m_bEnebled == VARIANT_FALSE)return S_OK;

	//edit by lcc 2016.11.23 ��������Ϣ�ĴӴ�������
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
// 	
	if (spGHWorkspace != NULL/* && spLYWorkspace != NULL*/)
	{
		//��������ģ��������ļ�����
		TCHAR szAppFullPath[MAX_PATH];
		DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(CHAR);
		GetModuleFileName(NULL,szAppFullPath,dBufferSize);
		CString strAppPath(szAppFullPath);
		strAppPath = strAppPath.Mid(0,strAppPath.ReverseFind('\\'));
		CString strConfigFile = strAppPath + CONFIGPATH;
		CString strSection(_T("JBNTBH"));

// 		CStringArray strArrDatasetName;
// 		strArrDatasetName.Add(_T("JQXZDW"));
// 		strArrDatasetName.Add(_T("GHJBNTTZ"));
// 		strArrDatasetName.Add(_T("XZQJX"));
// 		strArrDatasetName.Add(_T("XJXZQJX"));
// 		strArrDatasetName.Add(_T("GHYT"));
// 		strArrDatasetName.Add(_T("XZQXZ"));
// 		strArrDatasetName.Add(_T("XZQ"));
// 		strArrDatasetName.Add(_T("ZXCQ"));
		
		CLPPrintMapSetsDlg dlg;
		dlg.InitViewConfig2(strConfigFile,strSection);
		//dlg.SetConfigFile(strConfigFile);
		//dlg.SetGHWorkspace(spGHWorkspace);
		if (dlg.DoModal() == IDOK)
		{
			CExportThemeMap exportThemeMap((GisqLib::_DGisqFrameworkCOM*)m_ipFramework);
			//edit by lcc 2016.11.23 ��������Ϣ�ĴӴ�������
			//CString strXZQDM = m_bstrXZQCode;
			//exportThemeMap.ExportThemeMap2(spLYWorkspace,spGHWorkspace,strXZQDM,strConfigFile,strSection,strArrDatasetName);
			//��Ϊʹ�������ô��ڣ��޸�����ͼ����ӿ�
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