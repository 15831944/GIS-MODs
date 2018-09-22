/*********************************************
文件名:  LPJSYDGZQCXDlg
创建者:  luchencheng
编写日期:2016-10-13
简要描述:建设用地管制区查询功能窗口
修改记录(who-when-why):
********************************************/
#pragma once

#include "resource.h"
#include "afxwin.h"
// CLPJSYDGZQCXDlg 对话框

class CLPJSYDGZQCXDlg : public CGSBCGDialog
{
	DECLARE_DYNAMIC(CLPJSYDGZQCXDlg)

public:
	CLPJSYDGZQCXDlg(GisqLib::_DGisqFrameworkCOMPtr	ipFramework, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLPJSYDGZQCXDlg();

// 对话框数据
	enum { IDD = IDD_LPJSYDGZQCXDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	GisqLib::_DGisqFrameworkCOMPtr	m_ipFramework;

public:
	IFeatureWorkspacePtr m_ipFtWorkspace;
	CComboBox m_cbxGZQLX;
	CString m_strGZQLX;

	CString m_strUnit;
	
	double m_nTDYTQMJ_Max;
	double m_nTDYTQMJ_Min;
	CBCGPMaskEdit m_editTDYTQMJ_Max;
	CBCGPMaskEdit m_editTDYTQMJ_Min;
	
	CStatic m_RectForGrid;
	CBCGPGridCtrl m_wndResultGrid;
	CString m_strResultNum;

	BOOL InitGridCtrl();
	BOOL ShowResrult(IQueryFilterPtr ipQuery);

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedMeterRd4();
	afx_msg void OnBnClickedMuRd4();
	afx_msg void OnBnClickedHectareRd4();
	afx_msg void OnBnClickedCxbutton4();

};
