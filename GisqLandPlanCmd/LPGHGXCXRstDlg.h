#pragma once
#include "Resource.h"
#include "afxcmn.h"
#include "../include/tinyxml.h"
#include "BCGPTipListBox.h"
#include "..\include\Global.h"
#include "LPXZQDataTree.h"

//�����ָ���ܼ��࣬���ڵؿ����Ŀ��
class CSumInfo
{
public:
	CSumInfo()
	{
		dbSumZMJ = dbSumNYDMJ = dbSumGDJMJ = dbSumJSYDMJ = dbSumWLYDMJ = dbSumJBNTJMJ = 0.0;
		dbSumXZJSYD1 = dbSumXZJSYD2 = dbSumXZJSYD3 = dbSumXZJSYD4 = 0.0;
		dbSumJBNT1 = dbSumJBNT2 = dbSumJBNT3 = dbSumJBNT4 = 0.0;
	}
	void ClearInfo()
	{
		dbSumZMJ = dbSumNYDMJ = dbSumGDJMJ = dbSumJSYDMJ = dbSumWLYDMJ = dbSumJBNTJMJ = 0.0;
		dbSumXZJSYD1 = dbSumXZJSYD2 = dbSumXZJSYD3 = dbSumXZJSYD4 = 0.0;
		dbSumJBNT1 = dbSumJBNT2 = dbSumJBNT3 = dbSumJBNT4 = 0.0;
	}
	//�����������ũ����������ؾ�����������õ������δ���õ����������ũ�ﾻ���
	double dbSumZMJ,dbSumNYDMJ,dbSumGDJMJ,dbSumJSYDMJ,dbSumWLYDMJ,dbSumJBNTJMJ;
	//���������õ�ʡ�������ļ�ָ��
	double dbSumXZJSYD1,dbSumXZJSYD2,dbSumXZJSYD3,dbSumXZJSYD4;
	//����ũ��ʡ�������ļ�ָ��
	double dbSumJBNT1,dbSumJBNT2,dbSumJBNT3,dbSumJBNT4;
};
//�ؿ���Ϣ��
class CDKInfoClass
{
public:
	CDKInfoClass()
	{
		strDKBSM = strXZDM = strCUNDM = strCUNMC = strXZMC = _T("");
	}
	void ClearInfo()
	{
		strDKBSM = strXZDM = strCUNDM = strCUNMC = strXZMC = _T("");
	}
	CString strDKBSM;
	CString strXZDM;
	CString strXZMC;
	CString strCUNDM;
	CString strCUNMC;
};
//��Ŀ��Ϣ��
class CXMInfoClass
{
public:
	CXMInfoClass()
	{
		strXMBSM = _T("");
		strXMMC = _T("");
		strXMLX = _T("");
		dbZMJ = dbNYDMJ = dbGDJMJ = dbJSYDMJ = dbWLYDMJ = dbJBNTJMJ = dbXGDKMJ = 0.0;
		dbJSYDZBsheng = dbJSYDZBshi = dbJSYDZBxian = dbJSYDZBxiang = 0.0;
		dbJBNTZBsheng = dbJBNTZBshi = dbJBNTZBxian = dbJBNTZBxiang = 0.0;
	}
	void ClearInfo()
	{
		strXMBSM = _T("");
		strXMMC = _T("");
		strXMLX = _T("");
		dbZMJ = dbNYDMJ = dbGDJMJ = dbJSYDMJ = dbWLYDMJ = dbJBNTJMJ = dbXGDKMJ = 0.0;
		dbJSYDZBsheng = dbJSYDZBshi = dbJSYDZBxian = dbJSYDZBxiang = 0.0;
		dbJBNTZBsheng = dbJBNTZBshi = dbJBNTZBxian = dbJBNTZBxiang = 0.0;
		for (int i = 0; i < m_arrDKInfo.GetCount(); i++)
		{
			if (m_arrDKInfo.GetAt(i) != NULL)
			{
				delete m_arrDKInfo.GetAt(i);
				m_arrDKInfo.GetAt(i) = NULL;
			}
		}
		m_arrDKInfo.RemoveAll();
		m_DKSumInfo.ClearInfo();
	}
	//��Ŀ��ʶ��
	CString strXMBSM;
	//��Ŀ����
	CString strXMMC;
	//��Ŀ����
	CString strXMLX;
	//�����,ũ�õ���������ؾ�����������õ������δ���õ����������ũ�ﾻ������޸ĵؿ����
	double dbZMJ,dbNYDMJ,dbGDJMJ,dbJSYDMJ,dbWLYDMJ,dbJBNTJMJ,dbXGDKMJ;
	//���������õء�����ũ�ʡ�������ļ�ָ��
	double dbJSYDZBsheng,dbJSYDZBshi,dbJSYDZBxian,dbJSYDZBxiang;
	double dbJBNTZBsheng,dbJBNTZBshi,dbJBNTZBxian,dbJBNTZBxiang;
	//�ؿ���Ϣ����
	CArray<CDKInfoClass*,CDKInfoClass*> m_arrDKInfo;
	//�ؿ���Ϣ�ܼ���
	CSumInfo m_DKSumInfo;
};

//������Ϣ��
//����ũ��ĸ���������ǻ���ũ�����
class CFAInfoClass
{
public:
	CFAInfoClass()
	{
		strFABSM = _T("");
		strSLH = _T("");
		strFALX = _T("");
		strFAMC = _T("");
		strSPJG = _T("");
		strSPSJ = _T("");
		strGXSJ = _T("");
		strGXZT = _T("");
		strGHMC = _T("");
		strSJXMMC = _T("");
		strXMC = _T("");
		strXDM = _T("");
		m_mapstrSJXZMC.RemoveAll();
		nXMCount = nXZCount = nDKCount = 0;
		dbZMJ = dbNYDMJ = dbGDJMJ = dbJSYDMJ = dbWLYDMJ = dbJBNTJMJ = dbXMZMJ = 0.0;
		dbJSYDZBsheng = dbJSYDZBshi = dbJSYDZBxian = dbJSYDZBxiang = 0.0;
		dbJBNTZBsheng = dbJBNTZBshi = dbJBNTZBxian = dbJBNTZBxiang = 0.0;
	}
	//ͬ��ʼ���������������Ϣ
	void ClearInfo()
	{
		strFABSM = _T("");
		strSLH = _T("");
		strFALX = _T("");
		strFAMC = _T("");
		strSPJG = _T("");
		strSPSJ = _T("");
		strGXSJ = _T("");
		strGXZT = _T("");
		strGHMC = _T("");
		strSJXMMC = _T("");
		strXMC = _T("");
		strXDM = _T("");
		m_mapstrSJXZMC.RemoveAll();
		nXMCount = nXZCount = nDKCount = 0;
		dbZMJ = dbNYDMJ = dbGDJMJ = dbJSYDMJ = dbWLYDMJ = dbJBNTJMJ = dbXMZMJ = 0.0;
		dbJSYDZBsheng = dbJSYDZBshi = dbJSYDZBxian = dbJSYDZBxiang = 0.0;
		dbJBNTZBsheng = dbJBNTZBshi = dbJBNTZBxian = dbJBNTZBxiang = 0.0;
		for (int i = 0; i < m_arrXMInfo.GetCount(); i++)
		{
			if (m_arrXMInfo.GetAt(i) != NULL)
			{
				(m_arrXMInfo.GetAt(i))->ClearInfo();
				delete m_arrXMInfo.GetAt(i);
				m_arrXMInfo.GetAt(i) = NULL;
			}
		}
		m_arrXMInfo.RemoveAll();
		m_XMSumInfo.ClearInfo();
	}
	//��Ŀ��ʶ��
	CString strFABSM;
	//�����
	CString strSLH;
	//��������
	CString strFALX;
	//��������
	CString strFAMC;
	//��������
	CString strSPJG;
	//����ʱ��
	CString strSPSJ;
	//����ʱ��
	CString strGXSJ;
	//����״̬
	CString strGXZT;

	//�漰�滮����
	CString strGHMC;
	//�漰��Ŀ����
	CString strSJXMMC;
	//�ش���
	CString strXDM;
	//������
	CString strXMC;
	//�漰��������
	CMapStringToString m_mapstrSJXZMC;
	//�漰��Ŀ����
	int nXMCount;
	//�漰��������
	int nXZCount;
	//�漰�ؿ����
	int nDKCount;

	//�����
	double dbZMJ;
	//ũ�õ����
	double dbNYDMJ;
	//���ؾ����
	double dbGDJMJ;
	//�����õ����
	double dbJSYDMJ;
	//δ���õ����
	double dbWLYDMJ;
	//����ũ�����
	double dbJBNTJMJ;
	//��Ŀ�����
	double dbXMZMJ;
	//���������õء�����ũ�ʡ�������ļ�ָ��
	double dbJSYDZBsheng,dbJSYDZBshi,dbJSYDZBxian,dbJSYDZBxiang;
	double dbJBNTZBsheng,dbJBNTZBshi,dbJBNTZBxian,dbJBNTZBxiang;

	//�漰��Ŀ����
	CArray<CXMInfoClass*,CXMInfoClass*> m_arrXMInfo;
	//��Ŀ�ܼ��� 
	CSumInfo m_XMSumInfo;
};
//��������ؼ��Զ����и�
class CCXResBCGGridCtrl : public CBCGPGridCtrl
{
public:
	virtual void SetRowHeight ()
	{
		CBCGPGridCtrl::SetRowHeight();
		m_nRowHeight += 10;
	}
};
// CLPGHGXCXRstDlg �Ի���

class CLPGHGXCXRstDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CLPGHGXCXRstDlg)

public:
	CLPGHGXCXRstDlg(GisqLib::_DGisqFrameworkCOMPtr	ipFramework,CWnd* pParent = NULL);   // ��׼���캯��
	CLPGHGXCXRstDlg(CComQIPtr<GisqLib::_DGisqFrameworkCOM> ipFramework,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLPGHGXCXRstDlg();

// �Ի�������
	enum { IDD = IDD_GHGXCXRSTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//������ѯ�����
	CBCGPTipListBox m_wndFARstList;
	//��¼��ǰѡ�еķ�������ֹ�ظ�ˢ��
	int m_nFASelectIndex;
	//���tab�ؼ�
	CBCGPTabWnd *m_pwndResTabWnd;
	//���������Ϣҳ
	CCXResBCGGridCtrl *m_pwndFAInfoGrid;
	//�����Ŀ�ؿ�ҳ
	CBCGPGridCtrl *m_pwndFADKGrid;
	//������Ϣ��
	CFAInfoClass m_csFAInfo;
	//���
	CComQIPtr<GisqLib::_DGisqFrameworkCOM> m_spFramework;
	//����״̬Map
	CMapStringToString m_strmapGXZT;
	//��������Map
	CMapStringToString m_strmapFALX;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLbnSelchangeFalist();
	//���ݸ���������ѯ�������ƺͱ�ʶ�룬������listbox
	HRESULT SearchFA(CComBSTR strQrueyQueryFilter);
	//���ӵ����ݿ�
	HRESULT ConnToOracle(_ConnectionPtr &ipADOConn);
	HRESULT getConnString(CComBSTR &bstrConnSet,CComBSTR &bstrName,CComBSTR &bstrPasswd);
	//��ȡ������Ϣ
	HRESULT GetFAInfoByUID(_ConnectionPtr &ipADOConn, CString &strFABSM);
	//��ʾ������Ϣ
	HRESULT ShowFAInfo();
	//��ȡ��ѯ����
	void GetSQLstring(CString &strFABSM, CString &strSQL);
	//��ʼ�������ֵ�
	void InitMaps();
	void utf8ToUnicode(LPCSTR src, CString *Result);
};
