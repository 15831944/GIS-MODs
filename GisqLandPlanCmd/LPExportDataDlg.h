#pragma once

#include "resource.h"
#include "..\include\ComboBoxTreeCtrl.h"
#include "LPXZQDataTree.h"
#include "BCGPDateTimeCtrlEx.h"

#include "afxwin.h"

//重写CBCGPEdit的按钮，实现保存对话框
class CBCGPSaveEdit : public CBCGPEdit
{
public:
	void OnBrowse()
	{
		CString strEditorText;
		GetWindowText (strEditorText);
		TCHAR szDesktopFullPath[MAX_PATH];
		DWORD dBufferSize = sizeof(szDesktopFullPath)/sizeof(CHAR);
		SHGetSpecialFolderPath(NULL,szDesktopFullPath,CSIDL_DESKTOPDIRECTORY,NULL);
		CString strDesktopPath(szDesktopFullPath);
		CString strConfigFile = strDesktopPath + _T("\\数据导出.gdb");
		CFileDialog dlg(FALSE, NULL, strConfigFile, NULL, _T("GDB(*.gdb)|*.gdb||"), GetParent ());
		//CFileDialog dlg(FALSE, NULL, strConfigFile,  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("TXT(*.txt)|*.txt||"), GetParent ());
		if (dlg.DoModal () == IDOK &&
			strEditorText != dlg.GetPathName ())
		{
			CString strFilePath =  dlg.GetPathName ();
			if (strFilePath.Find(_T(".gdb")) < 0)
			{
				strFilePath += _T(".gdb");
			}
			SetWindowText (strFilePath);
			SetModify (TRUE);
			OnAfterUpdate ();
		}
	}
};

// CLPExportDataDlg 对话框

class CLPExportDataDlg : public CGSBCGDialog
{
	DECLARE_DYNAMIC(CLPExportDataDlg)

public:
	CLPExportDataDlg(GisqLib::_DGisqFrameworkCOMPtr pFrameworkCom, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLPExportDataDlg();

// 对话框数据
	enum { IDD = IDD_LPEXPORTDATADLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//记录框架接口
	CComQIPtr<GisqLib::_DGisqFrameworkCOM> m_pFrameworkCom;
	//记录当前规划工作空间
	CComQIPtr<IFeatureWorkspace> m_ipFtWorkspace;
	//记录本地保存数据的工作空间
	CComQIPtr<IWorkspace> m_spExpWorkspace;
	//保存数据的dataset
	CComQIPtr<IDataset> m_spExpDataset;
	//行政区下拉树
	CComboBoxTreeCtrl m_wndXZQTree;
	//保存路径文本框
	CBCGPSaveEdit m_wndSavePath;
	//时间日期控件
	CBCGPDateTimeCtrlEx m_wndMakeTime;

	virtual BOOL OnInitDialog();
	//对于所有下拉树菜单进行统一的初始化，如果不初始化，在点击时会崩溃
	void InitComboTree();
	//初始化行政区树
	BOOL InitXZQComboTree(int nIndex);
	//创建行政区前的图片列表（现在没用
	void InitImageList(CImageList** ppImageList);
	//递归的插入行政区节点
	void AddDistrictTreeItem(HTREEITEM parentItem = NULL,CComQIPtr<ILPXZQDataItem> spDataItem = (IUnknown*)NULL, int nStopLevel = 7);
	//初始化时间控件，初始化时间为上一年的一月一号
	void InitTimeCtrl();
	//点击确定时调用
	afx_msg void OnBnClickedOk();
	//退出时调用销毁
	afx_msg void OnClose();
protected:
	//退出时调用销毁
	virtual void OnCancel();
	//销毁窗口时删除自身
	virtual void PostNcDestroy();
};
