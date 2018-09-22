/*********************************************
�ļ���:  LPGHDLTBCXDlg
������:  luchencheng
��д����:2016-10-09
��Ҫ����:�滮����ͼ�߲�ѯ���ܴ���
�޸ļ�¼(who-when-why):
********************************************/
#pragma once

#include "Resource.h"

#include "..\include\GlobalDefine.h"
#include "afxwin.h"
// CLPGHDLTBCXDlg �Ի���

class CLPGHDLTBCXDlg : public CGSBCGDialog
{
	DECLARE_DYNAMIC(CLPGHDLTBCXDlg)

public:
	CLPGHDLTBCXDlg(GisqLib::_DGisqFrameworkCOMPtr	ipFramework, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLPGHDLTBCXDlg();

// �Ի�������
	enum { IDD = IDD_LPGHDLTBCXDLG };

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
 	//��ѯ����
 	double m_nGHDL_Min;
 	double m_nGHDL_Max;
 	CBCGPMaskEdit m_editGHDLMJ_Min;
 	CBCGPMaskEdit m_editGHDLMJ_Max;
 
 	CString m_strTown;
 	CString m_strVillage;
 	CString m_strGHDL;
 	CBCGPComboBox m_cbxTown;
 	CBCGPComboBox m_cbxVillage;
 	CBCGPComboBox m_cbxGHDL;
 
 	//������ʾ�����grid
 	CBCGPStatic m_RectForGrid;
 	CBCGPGridCtrl m_wndResultGrid;	
	CString m_strResultNum;

	//��ʼ��
	BOOL InitTownComboBox();
	BOOL InitDlComboBox();
	BOOL InitGridCtrl();

	BOOL GetXzqDm();
	BOOL GetGHDLMC();
	void AddToArray(BSTR bstrDM,BSTR bstrMC);
	void SortArray(int left,int right);//��������

	BOOL FlashComboBox(_tagDistrictItem *tagParent);//ˢ�¸�������������combobox
	BOOL ShowResrult(IQueryFilterPtr ipQuery);
	CString VTtoCString(VARIANT m_variant);

	//�¼���Ӧ
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedMeterRd2();
	afx_msg void OnBnClickedMuRd2();
	afx_msg void OnBnClickedHectareRd2();
	afx_msg void OnBnClickedCxbutton2();
	afx_msg void OnCbnSelchangeXzcombo2();
	//�����ǹرմ���ʱ�ͷŴ�����Դ
	afx_msg void OnClose();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnDestroy();
};
