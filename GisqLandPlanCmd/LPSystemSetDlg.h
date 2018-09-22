#pragma once

#include "resource.h"
#include "afxwin.h"
#include "BCGPDateTimeCtrlEx.h"
// CLPSystemSetDlg 对话框

class CLPSystemSetDlg : public CGSBCGDialog
{
	DECLARE_DYNAMIC(CLPSystemSetDlg)

public:
	CLPSystemSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLPSystemSetDlg();

// 对话框数据
	enum { IDD = IDD_SYSTEMSETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CBCGPComboBox m_wndGXSJDate;
	CBCGPDateTimeCtrlEx m_wndGXSJTime;
	virtual BOOL OnInitDialog();
	void InitTimeCtrl();
};
