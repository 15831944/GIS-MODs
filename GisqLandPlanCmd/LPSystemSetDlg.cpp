// LPSystemSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LPSystemSetDlg.h"


// CLPSystemSetDlg �Ի���

IMPLEMENT_DYNAMIC(CLPSystemSetDlg, CGSBCGDialog)

CLPSystemSetDlg::CLPSystemSetDlg(CWnd* pParent /*=NULL*/)
	: CGSBCGDialog(CLPSystemSetDlg::IDD, pParent)
{

}

CLPSystemSetDlg::~CLPSystemSetDlg()
{
}

void CLPSystemSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CGSBCGDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GXSJ_COMBO, m_wndGXSJDate);
	DDX_Control(pDX, IDC_SYSSET_TIME, m_wndGXSJTime);
}


BEGIN_MESSAGE_MAP(CLPSystemSetDlg, CGSBCGDialog)
END_MESSAGE_MAP()


// CLPSystemSetDlg ��Ϣ�������

BOOL CLPSystemSetDlg::OnInitDialog()
{
	CGSBCGDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ����combo
	m_wndGXSJDate.AddString(_T("����"));
	m_wndGXSJDate.AddString(_T("��һ"));
	m_wndGXSJDate.AddString(_T("�ܶ�"));
	m_wndGXSJDate.AddString(_T("����"));
	m_wndGXSJDate.AddString(_T("����"));
	m_wndGXSJDate.AddString(_T("����"));
	m_wndGXSJDate.AddString(_T("����"));
	m_wndGXSJDate.SelectString(0,_T("����"));

	InitTimeCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CLPSystemSetDlg::InitTimeCtrl()
{
	//������Щ����
	const UINT stateMask = 
		CBCGPDateTimeCtrl::DTM_SPIN |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR | 
		CBCGPDateTimeCtrl::DTM_DATE |
		CBCGPDateTimeCtrl::DTM_TIME24H |
		CBCGPDateTimeCtrl::DTM_CHECKBOX |
		CBCGPDateTimeCtrl::DTM_TIME | 
		CBCGPDateTimeCtrl::DTM_TIME24HBYLOCALE |
		CBCGPDateTimeCtrl::DTM_SECONDS;
	//�Ƿ�ʹ����Щ���ã�������д��
	UINT DataState = 
		CBCGPDateTimeCtrl::DTM_SPIN |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR | 
		CBCGPDateTimeCtrl::DTM_TIME | 
		CBCGPDateTimeCtrl::DTM_TIME24HBYLOCALE;
	m_wndGXSJTime.SetState(DataState,stateMask);
	m_wndGXSJTime.m_monthFormat = 2;

}
