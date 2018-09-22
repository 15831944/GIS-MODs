// LPExportDataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LPExportDataDlg.h"


// CLPExportDataDlg 对话框

IMPLEMENT_DYNAMIC(CLPExportDataDlg, CGSBCGDialog)

CLPExportDataDlg::CLPExportDataDlg(GisqLib::_DGisqFrameworkCOMPtr pFrameworkCom, CWnd* pParent /*=NULL*/)
	: CGSBCGDialog(CLPExportDataDlg::IDD, pParent)
{
	m_pFrameworkCom = pFrameworkCom;
}

CLPExportDataDlg::~CLPExportDataDlg()
{
}

void CLPExportDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CGSBCGDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPDATAXZQ_CBX, m_wndXZQTree);
	DDX_Control(pDX, IDC_EXPDATA_TIMECTRL, m_wndMakeTime);
	DDX_Control(pDX, IDC_EDIT1, m_wndSavePath);
}


BEGIN_MESSAGE_MAP(CLPExportDataDlg, CGSBCGDialog)
	ON_BN_CLICKED(IDOK, &CLPExportDataDlg::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CLPExportDataDlg 消息处理程序

BOOL CLPExportDataDlg::OnInitDialog()
{
	CGSBCGDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CComQIPtr<GisqLib::IGSDataExchangeS> spGSDataExchangeS = m_pFrameworkCom->GetDataExchangeS();
	BSTR bstrTag(_T("GHWS"));
	short index = spGSDataExchangeS->FindIndex(bstrTag);
	if (index >= 0)
	{
		CComQIPtr<GisqLib::IGSDataExchange> spGSDataExchange = spGSDataExchangeS->Item(index);
		CComVariant var = spGSDataExchange->Item(0);
		IWorkspacePtr spWorkspace = var.punkVal;
		m_ipFtWorkspace = spWorkspace;
		//TODO…
	}

	InitComboTree();
	InitXZQComboTree(4);
	m_wndXZQTree.SetSelectString(_T("[33]浙江省"));
	InitTimeCtrl();

	CString strEditorText;
	GetWindowText (strEditorText);
	TCHAR szDesktopFullPath[MAX_PATH];
	DWORD dBufferSize = sizeof(szDesktopFullPath)/sizeof(CHAR);
	SHGetSpecialFolderPath(NULL,szDesktopFullPath,CSIDL_DESKTOPDIRECTORY,NULL);
	CString strDesktopPath(szDesktopFullPath);
	CString strConfigFile = strDesktopPath + _T("\\数据导出.gdb");
	m_wndSavePath.SetWindowText (strConfigFile);

	m_wndSavePath.EnableBrowseButton();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLPExportDataDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	CGSBCGDialog::PostNcDestroy();
	delete this;
}

void CLPExportDataDlg::InitComboTree()
{
	if (m_wndXZQTree.m_wndTreeList.GetSafeHwnd() == NULL)
	{
		m_wndXZQTree.InitTree(200,200);
		CImageList *TreeImage = NULL;
		InitImageList(&TreeImage);
		m_wndXZQTree.m_wndTreeList.SetImageList(TreeImage,4);
	}
}

BOOL CLPExportDataDlg::InitXZQComboTree(int nIndex)
{
	m_wndXZQTree.DeleteAllItems();
	CComQIPtr<ILPXZQDataTree> ipTree;
	CComQIPtr<ILPXZQDataItem> ipRoot;
	IUnknown *ipTmpItem;
	ipTree.CoCreateInstance(CLSID_LPXZQDataTree);
	ipTree->GetRootItem(m_ipFtWorkspace,&ipTmpItem);
	ipRoot = (CComQIPtr<ILPXZQDataItem>) ipTmpItem;
	if (ipRoot == NULL) return TRUE;
	BSTR bstrTmp;
	ipRoot->Get_ItemCode(&bstrTmp);
	//m_strParent = bstrTmp;
	AddDistrictTreeItem(NULL,ipRoot,nIndex);
	HTREEITEM item = m_wndXZQTree.m_wndTreeList.GetFirstVisibleItem();
	m_wndXZQTree.m_wndTreeList.Expand(item,TVE_EXPAND);
	UpdateData(FALSE);
	return TRUE;
}

void CLPExportDataDlg::AddDistrictTreeItem(HTREEITEM parentItem/*=NULL*/,CComQIPtr<ILPXZQDataItem> spDataItem, int nStopLevel)
{
	//组织当前节点数据
	BSTR bstrTmp;
	short nTmpLevel;
	_tagDistrictItem *ItemData = new _tagDistrictItem;
	spDataItem->Get_ItemName(&bstrTmp);
	ItemData->strName = bstrTmp;
	spDataItem->Get_ItemCode(&bstrTmp);
	ItemData->strCode = bstrTmp;
	spDataItem->Get_ItemParentID(&bstrTmp);
	ItemData->strParentID = bstrTmp;
	spDataItem->Get_ItemLevel(&nTmpLevel);
	ItemData->nLevel = (int)nTmpLevel;
	//插入当前节点
	if (ItemData->nLevel > nStopLevel)
	{
		delete ItemData;
		return;
	}
	HTREEITEM hItem;
	if (ItemData->nLevel == 0)//省
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(2),ItemData->strName);
		hItem = m_wndXZQTree.AddItem(showValue,0,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 1)//市
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(2),ItemData->strName);
		hItem = m_wndXZQTree.AddItem(showValue,1,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 2)//县区
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(2),ItemData->strName);
		hItem = m_wndXZQTree.AddItem(showValue,2,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 4)//乡镇街道
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(3),ItemData->strName);
		hItem = m_wndXZQTree.AddItem(showValue,3,(DWORD)ItemData,parentItem);
	}else if (ItemData->nLevel  == 6)//村
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(3),ItemData->strName);
		hItem = m_wndXZQTree.AddItem(showValue,-1,(DWORD)ItemData,parentItem);
	}
	//遍历递归插入子节点
	IUnknown *ipTmpItem = NULL;
	CComQIPtr<ILPXZQDataEnum> ipEnum;
	spDataItem->Get_ChildItems(&ipTmpItem);
	ipEnum = (CComQIPtr<ILPXZQDataEnum>) ipTmpItem;
	if (ipEnum == NULL) return ;
	int nCount;
	ipEnum->GetItemCount(&nCount);
	for (int i = 0; i < nCount; i++)
	{
		IUnknown* pItemTmp = NULL;
		ipEnum->GetItemAt(i,&pItemTmp);
		CComQIPtr<ILPXZQDataItem> spChlidDataItem = (CComQIPtr<ILPXZQDataItem>)pItemTmp;
		AddDistrictTreeItem(hItem,spChlidDataItem,nStopLevel);
	}

}

void CLPExportDataDlg::InitImageList(CImageList** ppImageList)
{
	CImageList *TreeImage = new CImageList;
	TreeImage->Create(32, 32, ILC_COLOR16 | ILC_MASK,0,4);
	CBitmap *BitmapTmp = new CBitmap;
	BitmapTmp->LoadBitmap(IDB_BMP_ICON_SHENG);
	TreeImage->Add(BitmapTmp,RGB(255,255,255));

	BitmapTmp = NULL;
	BitmapTmp = new CBitmap;
	BitmapTmp->LoadBitmap(IDB_BMP_ICON_SHI);
	TreeImage->Add(BitmapTmp,RGB(255,255,255));

	BitmapTmp = NULL;
	BitmapTmp = new CBitmap;
	BitmapTmp->LoadBitmap(IDB_BMP_ICON_XIANQU);
	TreeImage->Add(BitmapTmp,RGB(255,255,255));

	BitmapTmp = NULL;
	BitmapTmp = new CBitmap;
	BitmapTmp->LoadBitmap(IDB_BMP_ICON_XIANGZHEN);
	TreeImage->Add(BitmapTmp,RGB(255,255,255));
	*ppImageList = TreeImage;
}

void CLPExportDataDlg::InitTimeCtrl()
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
		CBCGPDateTimeCtrl::DTM_DATE;
	m_wndMakeTime.SetState(DataState,stateMask);
	m_wndMakeTime.m_monthFormat = 2;

	//设置日期为上一年的一月一号
	//获取系统时间
	CTime t;
	t = CTime::GetCurrentTime();
	SYSTEMTIME sys_t;
	t.GetAsSystemTime(sys_t);
	COleDateTime ole_t = COleDateTime(sys_t);
	//计算上一年一月一日
	int nYear = ole_t.GetYear();
	ole_t.SetDate(nYear - 1,1,1);
	DATE date_t = ole_t;
	//设置时间
	m_wndMakeTime.SetDate(date_t);
}

void CLPExportDataDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strSavePath;
	m_wndSavePath.GetWindowText(strSavePath);
	CFileFind filefind;
	BOOL bhasFile = filefind.FindFile(strSavePath);

	CComQIPtr<IWorkspaceFactory> spWorkspaceFact;
	spWorkspaceFact.CoCreateInstance(CLSID_FileGDBWorkspaceFactory);

	if (bhasFile)
	{
		if (BCGPMessageBox(_T("文件已存在，是否依旧写入当前文件"),_T("警告"),MB_OKCANCEL) == IDCANCEL)
		{
			return;
		}else
		{
			spWorkspaceFact->OpenFromFile(_bstr_t(strSavePath),NULL,&m_spExpWorkspace);
		}		
	}else
	{
		CString strDirectory;
		CString strName;
		int nIndex  = strSavePath.ReverseFind('\\');
		strDirectory = strSavePath.Left(nIndex);
		strName = strSavePath.Right(strSavePath.GetLength() - nIndex - 1);
		int n = 0;
		CComQIPtr<IWorkspaceName> spWorkspaceName;
		spWorkspaceFact->Create(_bstr_t(strDirectory),_bstr_t(strName),NULL,NULL,&spWorkspaceName);
		if (spWorkspaceName == NULL) return;
	}

}

void CLPExportDataDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CGSBCGDialog::OnClose();
	CBCGPDialog::DestroyWindow();
}

void CLPExportDataDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CGSBCGDialog::OnCancel();
	CBCGPDialog::DestroyWindow();
}
