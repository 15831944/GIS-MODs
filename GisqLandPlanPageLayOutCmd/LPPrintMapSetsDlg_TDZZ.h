#pragma once

#include "Resource.h"
#include "..\include\tinyxml.h"
#include "..\include\ComboBoxTreeCtrl.h"
#include "..\include\GlobalDefine.h"
#include "afxwin.h"
#include "BCGPTipComboBox.h"
#include <string>
using namespace std;
// CLPPrintMapSetsDlg_TDZZ 对话框

class CLPPrintMapSetsDlg_TDZZ : public CGSBCGDialog
{
	DECLARE_DYNAMIC(CLPPrintMapSetsDlg_TDZZ)

public:
	CLPPrintMapSetsDlg_TDZZ(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLPPrintMapSetsDlg_TDZZ();

// 对话框数据
	enum { IDD = IDD_PRINTMAPDLG_TDZZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual void OnOK();
private:
	CString m_strConfigFilePath;
	CString m_strRootName;

	CString m_strParent;
	BOOL m_bIsSaveConfige;
	//CComQIPtr<IFeatureWorkspace> m_spGHFtWorkspace;
	DECLARE_MESSAGE_MAP()
public:

	CMap<CString,LPCTSTR,CString,LPCTSTR> m_strMapConfig;	
	CString m_strXZQDM;
	CComQIPtr<IMap> m_spMap;
	CComQIPtr<IWorkspace> m_spGHWorkspace;
	CComQIPtr<ILayer> m_spZDQYLayer;

	virtual BOOL OnInitDialog();
	void SetConfigFile(CString strConfigFilePath);
	//void InitTimeCtrl();
	BOOL InitXZQComboTree(int nStopLevel = 7);
	void InitImageList(CImageList** ppImageList);
	void AddDistrictTreeItem(HTREEITEM parentItem = NULL,CComQIPtr<ILPXZQDataItem> spDataItem = (ILPXZQDataItem*)NULL, int nStopLevel = 7);

	BOOL InitViewConfig(CString strConfigFile, CString strRootName);
	BOOL InitViewConfig2(CString strConfigFile, CString strRootName);
	void InitConfigEdits();
	void SaveConfigFile();
	void RefleshCofigMap();

	//void SetGHWorkspace(CComQIPtr<IWorkspace> spGHWorkspace);

	CComboBoxTreeCtrl m_wndXZQComboTree;
	CString m_strMapName;
	CString m_strMakeWhere_ZF;
	CString m_strMakeWhere_GTJ;
	//CBCGPDateTimeCtrl m_wndMakeDate;
	afx_msg void OnBnClickedIsSaveSets();

	//add by lcc 2016.12.19 翻译日期
	void GetTimeString(CString &strTime); 
	void InitTimeCtrl();
	CBCGPComboBox m_wndMonthCombo;
	int m_nYear;
	BOOL InitTDZZLayerCbx();
	void SetDataMap(CComQIPtr<IMap> spMap);
	void SetGHWorkspace(CComQIPtr<IWorkspace> spWorkspace);
	CBCGPTipComboBox m_wndZDQYLayerCombo;
	CBCGPMaskEdit m_editYear;
	BOOL m_bIsLockScale;
	BOOL m_bIsZXCQ;
	HTREEITEM m_pXZQItem;
	void utf8ToUnicode(LPCSTR src, CString *Result);
	string UnicodeToUTF8(CString &strIn);
};
