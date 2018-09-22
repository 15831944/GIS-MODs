#pragma once

#include "resource.h"
#include "..\include\ComboBoxTreeCtrl.h"
#include "LPXZQDataTree.h"
#include "BCGPDateTimeCtrlEx.h"

#include "afxwin.h"

//��дCBCGPEdit�İ�ť��ʵ�ֱ���Ի���
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
		CString strConfigFile = strDesktopPath + _T("\\���ݵ���.gdb");
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

// CLPExportDataDlg �Ի���

class CLPExportDataDlg : public CGSBCGDialog
{
	DECLARE_DYNAMIC(CLPExportDataDlg)

public:
	CLPExportDataDlg(GisqLib::_DGisqFrameworkCOMPtr pFrameworkCom, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLPExportDataDlg();

// �Ի�������
	enum { IDD = IDD_LPEXPORTDATADLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//��¼��ܽӿ�
	CComQIPtr<GisqLib::_DGisqFrameworkCOM> m_pFrameworkCom;
	//��¼��ǰ�滮�����ռ�
	CComQIPtr<IFeatureWorkspace> m_ipFtWorkspace;
	//��¼���ر������ݵĹ����ռ�
	CComQIPtr<IWorkspace> m_spExpWorkspace;
	//�������ݵ�dataset
	CComQIPtr<IDataset> m_spExpDataset;
	//������������
	CComboBoxTreeCtrl m_wndXZQTree;
	//����·���ı���
	CBCGPSaveEdit m_wndSavePath;
	//ʱ�����ڿؼ�
	CBCGPDateTimeCtrlEx m_wndMakeTime;

	virtual BOOL OnInitDialog();
	//���������������˵�����ͳһ�ĳ�ʼ�����������ʼ�����ڵ��ʱ�����
	void InitComboTree();
	//��ʼ����������
	BOOL InitXZQComboTree(int nIndex);
	//����������ǰ��ͼƬ�б�����û��
	void InitImageList(CImageList** ppImageList);
	//�ݹ�Ĳ����������ڵ�
	void AddDistrictTreeItem(HTREEITEM parentItem = NULL,CComQIPtr<ILPXZQDataItem> spDataItem = (IUnknown*)NULL, int nStopLevel = 7);
	//��ʼ��ʱ��ؼ�����ʼ��ʱ��Ϊ��һ���һ��һ��
	void InitTimeCtrl();
	//���ȷ��ʱ����
	afx_msg void OnBnClickedOk();
	//�˳�ʱ��������
	afx_msg void OnClose();
protected:
	//�˳�ʱ��������
	virtual void OnCancel();
	//���ٴ���ʱɾ������
	virtual void PostNcDestroy();
};
