// LPSystemSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LPSystemSetDlg.h"


// CLPSystemSetDlg 对话框

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


// CLPSystemSetDlg 消息处理程序

BOOL CLPSystemSetDlg::OnInitDialog()
{
	CGSBCGDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化周combo
	m_wndGXSJDate.AddString(_T("周日"));
	m_wndGXSJDate.AddString(_T("周一"));
	m_wndGXSJDate.AddString(_T("周二"));
	m_wndGXSJDate.AddString(_T("周三"));
	m_wndGXSJDate.AddString(_T("周四"));
	m_wndGXSJDate.AddString(_T("周五"));
	m_wndGXSJDate.AddString(_T("周六"));
	m_wndGXSJDate.SelectString(0,_T("周日"));

	InitTimeCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLPSystemSetDlg::InitTimeCtrl()
{
	//设置哪些配置
	const UINT stateMask = 
		CBCGPDateTimeCtrl::DTM_SPIN |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR | 
		CBCGPDateTimeCtrl::DTM_DATE |
		CBCGPDateTimeCtrl::DTM_TIME24H |
		CBCGPDateTimeCtrl::DTM_CHECKBOX |
		CBCGPDateTimeCtrl::DTM_TIME | 
		CBCGPDateTimeCtrl::DTM_TIME24HBYLOCALE |
		CBCGPDateTimeCtrl::DTM_SECONDS;
	//是否使用这些设置（不用则不写）
	UINT DataState = 
		CBCGPDateTimeCtrl::DTM_SPIN |
		CBCGPDateTimeCtrl::DTM_DROPCALENDAR | 
		CBCGPDateTimeCtrl::DTM_TIME | 
		CBCGPDateTimeCtrl::DTM_TIME24HBYLOCALE;
	m_wndGXSJTime.SetState(DataState,stateMask);
	m_wndGXSJTime.m_monthFormat = 2;

}
