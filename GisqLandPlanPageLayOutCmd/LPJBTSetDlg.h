#pragma once

#include "Resource.h"
#include "..\include\tinyxml.h"
#include "BCGPDateTimeCtrlEx.h"
#include "..\include\ComboBoxTreeCtrl.h"
#include "TipComboboxTreeCtrl.h"
#include "afxwin.h"
#include <string>
using namespace std;
// CLPJBTSetDlg 对话框

class CLPJBTSetDlg : public CGSBCGDialog
{
	DECLARE_DYNAMIC(CLPJBTSetDlg)

public:
	CLPJBTSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLPJBTSetDlg();

// 对话框数据
	enum { IDD = IDD_JBT_SETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_MapPublic;
	CString m_strProducer;
	CString m_strChecker;
	BOOL m_bIsAnalysis;
	CBCGPDateTimeCtrlEx m_wndMakeTime;
	virtual BOOL OnInitDialog();
	CMap<CString,LPCTSTR,CString,LPCTSTR> m_strMapConfig;
	CString m_strRootName;
	CString m_strConfigFilePath;
	BOOL InitViewConfig(CString strConfigFile, CString strRootName);
	void InitConfigEdits();
	void InitTimeCtrl();
protected:
	virtual void OnOK();
	void SetMakeTimetoMap();
	BOOL m_bIsAnalysisVisible;
public:
	void SetIsAnalysisVisible(BOOL bIsAnalysis);
	void SaveConfigFile();
	BOOL m_bIsSaveConfig;
	void utf8ToUnicode(LPCSTR src, CString *Result);
	string UnicodeToUTF8(CString &strIn);
	//CComboBoxTreeCtrl m_wndDKLXCombo;
	CTipComboboxTreeCtrl m_wndDKLXCombo;
	afx_msg void OnBnClickedIsanalysis();

	BOOL InitDLMCComboTree(CString DictID);
	void InsertDLMCTreeItems(TiXmlElement *CodeNamesElement, HTREEITEM pParentItem);
	void RefreshConfigDLMC();
};
