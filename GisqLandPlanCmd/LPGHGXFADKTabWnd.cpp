// LPGXGXFADKTabWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "LPGHGXFADKTabWnd.h"


// LPGXGXFADKTabWnd 对话框

IMPLEMENT_DYNAMIC(LPGHGXFADKTabWnd, CGSBCGDialog)

LPGHGXFADKTabWnd::LPGHGXFADKTabWnd(CWnd* pParent /*=NULL*/)
	: CGSBCGDialog(LPGHGXFADKTabWnd::IDD, pParent)
{

}

LPGHGXFADKTabWnd::~LPGHGXFADKTabWnd()
{
}

void LPGHGXFADKTabWnd::DoDataExchange(CDataExchange* pDX)
{
	CGSBCGDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LPGHGXFADKTabWnd, CGSBCGDialog)
END_MESSAGE_MAP()


// LPGXGXFADKTabWnd 消息处理程序
