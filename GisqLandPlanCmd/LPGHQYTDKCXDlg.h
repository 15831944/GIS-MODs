/*********************************************
�ļ���:  LPGHQYTDKCXDlg
������:  luchencheng
��д����:2016-10-013
��Ҫ����:�滮����;�ؿ��ѯ���ܴ���
�޸ļ�¼(who-when-why):
********************************************/
#pragma once

#include "resource.h"
#include "..\include\GlobalDefine.h"
#include "afxwin.h"
// CLPGHQYTDKCXDlg �Ի���

class CLPGHQYTDKCXDlg : public CGSBCGDialog
{
	DECLARE_DYNAMIC(CLPGHQYTDKCXDlg)

public:
	CLPGHQYTDKCXDlg(GisqLib::_DGisqFrameworkCOMPtr	ipFramework, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLPGHQYTDKCXDlg();

// �Ի�������
	enum { IDD = IDD_LPGHQYTDKCXDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	GisqLib::_DGisqFrameworkCOMPtr	m_ipFramework;

public:
	IFeatureWorkspacePtr			m_ipFtWorkspace;
	CArray<CString,CString>			m_arrDLMC;
	CString							m_strParentDM;//��ʶ��߼����������
	CString							m_strParentMC;
	CArray<_tagDistrictItem *,_tagDistrictItem *> *m_parrDistrict;

	CString m_strUnit;//��λ�ı�
	//��ѯ��Χ
	CString m_strTown;
	CString m_strGHDL;
	CBCGPComboBox m_cbxTown;
	CBCGPComboBox m_cbxGHDL;

	double m_nAREA_Min;
	double m_nAREA_Max;
	CBCGPMaskEdit m_editAREA_Min;
	CBCGPMaskEdit m_editAREA_Max;

	//�����ʾ
	CStatic m_RectForGrid;
	CBCGPGridCtrl m_wndResultGrid;
	CString m_strResultNum;

	//��ʼ��
	BOOL InitTownComboBox();
	BOOL InitDlComboBox();
	BOOL InitGridCtrl();
	//��ȡ������
	BOOL GetXzqDm();
	BOOL GetGHDLMC();
	void AddToArray(BSTR bstrDM,BSTR bstrMC);
	void SortArray(int left,int right);//��������
	//�����ʾ
	BOOL ShowResrult(IQueryFilterPtr ipQuery);

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedMeterRd3();
	afx_msg void OnBnClickedMuRd3();
	afx_msg void OnBnClickedHectareRd3();
	afx_msg void OnBnClickedCxbutton3();
};
