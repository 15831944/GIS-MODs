#pragma once

#include "resource.h"
#include "afxwin.h"
#include "BCGPDateTimeCtrlEx.h"
// CLPSystemSetDlg �Ի���

class CLPSystemSetDlg : public CGSBCGDialog
{
	DECLARE_DYNAMIC(CLPSystemSetDlg)

public:
	CLPSystemSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLPSystemSetDlg();

// �Ի�������
	enum { IDD = IDD_SYSTEMSETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CBCGPComboBox m_wndGXSJDate;
	CBCGPDateTimeCtrlEx m_wndGXSJTime;
	virtual BOOL OnInitDialog();
	void InitTimeCtrl();
};
