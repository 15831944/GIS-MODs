#pragma once
#include "Resource.h"

// LPGXGXFADKTabWnd �Ի���

class LPGHGXFADKTabWnd : public CGSBCGDialog
{
	DECLARE_DYNAMIC(LPGHGXFADKTabWnd)

public:
	LPGHGXFADKTabWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~LPGHGXFADKTabWnd();

// �Ի�������
	enum { IDD = IDD_FADKTABWND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CBCGPGridCtrl *m_pwndResFADKGrid;
};
