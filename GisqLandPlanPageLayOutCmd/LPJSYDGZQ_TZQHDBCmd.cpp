// LPJSYDGZQ_TZQHDBCmd.cpp : CLPJSYDGZQ_TZQHDBCmd 的实现

#include "stdafx.h"
#include "LPJSYDGZQ_TZQHDBCmd.h"
#include "LPPrintMapSetsDlg.h"

// CLPJSYDGZQ_TZQHDBCmd

// STDMETHODIMP CLPJSYDGZQ_TZQHDBCmd::get_Enabled(VARIANT_BOOL * bEnabled )
// {
// 	//获取"行政区"信息
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

STDMETHODIMP CLPJSYDGZQ_TZQHDBCmd::OnClick()
{
	//add by lcc 2016.11.23 添加资源切换支持
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//add end
	//获取"行政区"信息
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

	//土地规划工作空间（建设用地管制区工作空间）
	CComQIPtr<IWorkspace> spGHWorkspace;
	//土地现状工作空间（土地利用工作空间）
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
		//解析导出模板的配置文件内容
		TCHAR szAppFullPath[MAX_PATH];
		DWORD dBufferSize = sizeof(szAppFullPath)/sizeof(CHAR);
		GetModuleFileName(NULL,szAppFullPath,dBufferSize);
		CString strAppPath(szAppFullPath);
		strAppPath = strAppPath.Mid(0,strAppPath.ReverseFind('\\'));
		//CString strConfigFile = strAppPath + _T("\\data\\template\\土地利用建设用地空间管制区调整前后对比图.xml");
		CString strConfigFile = strAppPath + CONFIGPATH;
		CString strSection(_T("JSYDGZQ_TZQHDB"));

// 		CStringArray strArrDatasetName;
// 		strArrDatasetName.Add(_T("JQXZDW"));
// 		strArrDatasetName.Add(_T("XZQJX"));
// 		strArrDatasetName.Add(_T("XJXZQJX"));
// 		strArrDatasetName.Add(_T("JSYDGZQ"));
// 		strArrDatasetName.Add(_T("XJXZQ"));
// 		strArrDatasetName.Add(_T("XZQ"));

		CLPPrintMapSetsDlg dlg;
		//dlg.InitViewConfig(strConfigFile,strSection);
		//edit by lcc 20170208 修改了读取配置的接口
		dlg.InitViewConfig2(strConfigFile,strSection);
		//edit by lcc end
		if (dlg.DoModal() == IDOK)
		{
			CExportThemeMap exportThemeMap((GisqLib::_DGisqFrameworkCOM*)m_ipFramework);
			//edit by lcc 2016.11.23 行政区信息改从窗口设置
			//CString strXZQDM = m_bstrXZQCode;
			//exportThemeMap.ExportThemeMap2(spLYWorkspace,spGHWorkspace,strXZQDM,strConfigFile,strSection,strArrDatasetName);
			//因为使用了设置窗口，修改了制图输出接口
			//exportThemeMap.ExportThemeMap3(spLYWorkspace,spGHWorkspace,dlg.m_strXZQDM,dlg.m_strMapConfig,strArrDatasetName);
			//edit by lcc end
			//exportThemeMap.ExportThemeMap4(spGHWorkspace,dlg.m_strXZQDM,dlg.m_strMapConfig);
			//edit by lcc 20170208 专题图自适应比例及纸张
			exportThemeMap.ExportThemeMap5(spGHWorkspace,dlg.m_strXZQDM,dlg.m_strMapConfig,dlg.m_bIsZXCQ,dlg.m_bIsLockScale);
			//edit by lcc end
		}
	}
	else
	{
		BCGPMessageBox(_T("工作空间未初始化！"),_T("提示"),MB_OK);
	}
	return S_OK;
}