// GHJQDLTBCXDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LPGHJQDLTBCXDlg.h"


// CLPGHJQDLTBCXDlg �Ի���

IMPLEMENT_DYNAMIC(CLPGHJQDLTBCXDlg, CGSBCGDialog)

CLPGHJQDLTBCXDlg::CLPGHJQDLTBCXDlg(GisqLib::_DGisqFrameworkCOMPtr	ipFramework, CWnd* pParent /*=NULL*/)
	: CGSBCGDialog(CLPGHJQDLTBCXDlg::IDD, pParent)
	, m_nTBDL_Min(0)
	, m_nTBDL_Max(0)
	, m_nTBDLMJ_Min(0)
	, m_nTBDLMJ_Max(0)
	, m_strTown(_T(""))
	, m_strVillage(_T(""))
	, m_strGHDL(_T(""))
	, m_strResultNum(_T(""))
{
	m_strUnit = _T("(ƽ����)");
	m_ipFramework = ipFramework;
	m_parrDistrict = new CArray<_tagDistrictItem *,_tagDistrictItem *>;
}

CLPGHJQDLTBCXDlg::~CLPGHJQDLTBCXDlg()
{
	if (m_parrDistrict != NULL)
	{
		m_parrDistrict->RemoveAll();
		delete m_parrDistrict;
	}
	if (m_wndResultGrid.GetSafeHwnd() != NULL)
	{
		m_wndResultGrid.DeleteAllColumns();//ɾ��������
		m_wndResultGrid.RemoveAll();//ɾ��������
	}
}

void CLPGHJQDLTBCXDlg::DoDataExchange(CDataExchange* pDX)
{
	CGSBCGDialog::DoDataExchange(pDX);
	//��λ�ı�
	DDX_Text(pDX, IDC_UNITTEXT1, m_strUnit);
	DDX_Text(pDX, IDC_UNITTEXT2, m_strUnit);
	DDX_Text(pDX, IDC_UNITTEXT3, m_strUnit);
	DDX_Text(pDX, IDC_UNITTEXT4, m_strUnit);
	//����ı���
	DDX_Text(pDX, IDC_TBMJMIN, m_nTBDL_Min);
	DDX_Text(pDX, IDC_TBMJMAX, m_nTBDL_Max);
	DDX_Text(pDX, IDC_TBDLMJMIN, m_nTBDLMJ_Min);
	DDX_Text(pDX, IDC_TBDLMJMAX, m_nTBDLMJ_Max);
	DDX_Control(pDX, IDC_TBMJMIN, m_editTBMJ_Min);
	DDX_Control(pDX, IDC_TBMJMAX, m_editTBMJ_Max);
	DDX_Control(pDX, IDC_TBDLMJMIN, m_editTBDLMJ_Min);
	DDX_Control(pDX, IDC_TBDLMJMAX, m_editTBDLMJ_Max);
	//��ѯ��Χ
	DDX_Control(pDX, IDC_XZCOMBO, m_cbxTown);
	DDX_Control(pDX, IDC_CUNCOMBO, m_cbxVillage);
	DDX_Control(pDX, IDC_GHDLCOMBO3, m_cbxGHDL);

	DDX_Control(pDX, IDC_GRIDAREA, m_RectForGrid);
	DDX_CBString(pDX, IDC_XZCOMBO, m_strTown);
	DDX_CBString(pDX, IDC_CUNCOMBO, m_strVillage);
	DDX_CBString(pDX, IDC_GHDLCOMBO3, m_strGHDL);
	DDX_Text(pDX, IDC_RESCOUNT, m_strResultNum);
}


BEGIN_MESSAGE_MAP(CLPGHJQDLTBCXDlg, CGSBCGDialog)
	ON_BN_CLICKED(IDC_METER_RD, &CLPGHJQDLTBCXDlg::OnBnClickedMeterRd)
	ON_BN_CLICKED(IDC_MU_RD, &CLPGHJQDLTBCXDlg::OnBnClickedMuRd)
	ON_BN_CLICKED(IDC_HECTARE_RD, &CLPGHJQDLTBCXDlg::OnBnClickedHectareRd)
	ON_CBN_SELCHANGE(IDC_XZCOMBO, &CLPGHJQDLTBCXDlg::OnCbnSelchangeXzcombo)
	ON_BN_CLICKED(IDC_CXBUTTON, &CLPGHJQDLTBCXDlg::OnBnClickedCxbutton)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CLPGHJQDLTBCXDlg ��Ϣ�������

BOOL CLPGHJQDLTBCXDlg::OnInitDialog()
{
	CGSBCGDialog::OnInitDialog();
	
	//��ʼ��ѡ�в�ѯ���Ϊƽ����
	((CButton *)GetDlgItem(IDC_METER_RD))->SetCheck(TRUE);
	//��ʼ���ı���ؼ�
	m_editTBMJ_Min.SetValidChars(_T("1234567890."));//�����ַ��б�
	m_editTBMJ_Max.SetValidChars(_T("1234567890."));//�����ַ��б�
	m_editTBDLMJ_Min.SetValidChars(_T("1234567890."));//�����ַ��б�
	m_editTBDLMJ_Max.SetValidChars(_T("1234567890."));//�����ַ��б�
	m_editTBMJ_Min.SetWindowText(_T("0"));
	m_editTBMJ_Max.SetWindowText(_T("0"));
	m_editTBDLMJ_Min.SetWindowText(_T("0"));
	m_editTBDLMJ_Max.SetWindowText(_T("0"));
	UpdateData(FALSE);

	//��ʼ�����ؼ�
	InitGridCtrl();

	if (m_ipFramework == NULL) return FALSE;
	//��ȡ�����ռ�
	IWorkspacePtr ipWorkspace;
	CComQIPtr<GisqLib::IGSDataExchangeS> spGSDataExchangeS = m_ipFramework->GetDataExchangeS();
	BSTR bstrTag(_T("GHWS"));
	short index = spGSDataExchangeS->FindIndex(bstrTag);
	if (index >= 0)
	{
		CComQIPtr<GisqLib::IGSDataExchange> spGSDataExchange = spGSDataExchangeS->Item(index);
		CComVariant var = spGSDataExchange->Item(0);
		ipWorkspace = var.punkVal;
	}
	if (ipWorkspace == NULL) return FALSE;
	m_ipFtWorkspace = ipWorkspace;
	//��ʼ����ѯ��Χ�ؼ�
	InitTownComboBox();
	InitDlComboBox();
	m_cbxTown.SelectString(0,_T("ȫ��"));
	m_cbxVillage.EnableWindow(FALSE);
	m_cbxGHDL.SelectString(0,_T("ȫ��"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CLPGHJQDLTBCXDlg::InitTownComboBox()
{



	if (GetXzqDm() == TRUE)
	{
		m_cbxTown.AddString(_T("ȫ��"));
	}	
	SortArray(0,m_parrDistrict->GetCount() - 1);
	
	for (int i = 0; i < m_parrDistrict->GetCount() - 1; i++)
	{
		_tagDistrictItem *item;
		item = m_parrDistrict->GetAt(i);
		if (item->nLevel == DISTRICT_TREE_XIANG_ICON ||item->nLevel == DISTRICT_TREE_ZHEN_ICON)
		{
			m_cbxTown.AddString(item->strName);
			m_cbxTown.SetItemData(m_cbxTown.GetCount() - 1, (DWORD_PTR)item);
		}
	}

	return TRUE;
}

BOOL CLPGHJQDLTBCXDlg::GetXzqDm()
{
	CMapStringToString mapTMP;//����һ����ʱmap���ڲ���ʱ���ټ���
	mapTMP.InitHashTable(601);
	//������
	ITablePtr ipTable;
	m_ipFtWorkspace->OpenTable(_T("T_XZQMC"),&ipTable);
	if (ipTable != NULL)
	{
		ICursorPtr ipCursor = NULL;
		ipTable->Search(NULL,VARIANT_FALSE,&ipCursor);
		if (ipCursor == NULL)
		{return FALSE;}
		IRowPtr ipRow = NULL;
		ipCursor->NextRow(&ipRow);
		while(ipRow != NULL)
		{
			IFieldsPtr ipFields;
			ipRow->get_Fields(&ipFields);
			long nXZQIndex;

			ipFields->FindField(_T("XZQDM"),&nXZQIndex);
			VARIANT DMtmp;
			ipRow->get_Value(nXZQIndex,&DMtmp);

			ipFields->FindField(_T("XZQMC"),&nXZQIndex);
			VARIANT MCtmp;
			ipRow->get_Value(nXZQIndex,&MCtmp);
			//���뵽����
			if (DMtmp.bstrVal != NULL && MCtmp.bstrVal != NULL)
			{
				AddToArray(DMtmp.bstrVal,MCtmp.bstrVal);
				mapTMP.SetAt(DMtmp.bstrVal,MCtmp.bstrVal);
			}

			ipCursor->NextRow(&ipRow);
		}		
	}

	//����Ҫ�ؼ�����ȫ����û�е�ֵ��������һ��ֻ��map��ص�ֵ

	IFeatureClassPtr ipFtClass = NULL;
	m_ipFtWorkspace->OpenFeatureClass(_T("XZQ"),&ipFtClass);
	if (ipFtClass != NULL)
	{
		//����Ϊȫ�֣��ڲ���ʱʹ��
		IFeatureCursorPtr ipFtCursor = NULL;
		ipFtClass->Search(NULL,FALSE,&ipFtCursor);
		if (ipFtCursor != NULL)
		{
			IFeaturePtr ipFt = NULL;
			ipFtCursor->NextFeature(&ipFt);
			while(ipFt != NULL)
			{
				IFieldsPtr ipFields;
				ipFt->get_Fields(&ipFields);
				long nXZQIndex;

				ipFields->FindField(_T("XZQDM"),&nXZQIndex);
				VARIANT DMtmp;
				ipFt->get_Value(nXZQIndex,&DMtmp);

				ipFields->FindField(_T("XZQMC"),&nXZQIndex);
				VARIANT MCtmp;
				ipFt->get_Value(nXZQIndex,&MCtmp);

				if (DMtmp.bstrVal != NULL && MCtmp.bstrVal != NULL)
				{
					//�������û�У���ӵ�����
					CString tmp;
					if (mapTMP.Lookup(DMtmp.bstrVal,tmp) == FALSE)
					{
						mapTMP.SetAt(DMtmp.bstrVal,MCtmp.bstrVal);
						AddToArray(DMtmp.bstrVal,MCtmp.bstrVal);
					}
				}
				ipFtCursor->NextFeature(&ipFt);
			}
		}

	}
	mapTMP.RemoveAll();
	//������ݿ��в����ڱ��Ҫ�ؼ������ؿ�
	if (ipTable == NULL/* && ipFtClass == NULL*/)
	{
		return FALSE;
	}
	return TRUE;
};

void CLPGHJQDLTBCXDlg::AddToArray(BSTR bstrDM,BSTR bstrMC)
{
	CString strDM = bstrDM;
	CString strMC = bstrMC;
	if (strDM.GetLength() == 2)//ʡ��
	{
		_tagDistrictItem *tmp = new _tagDistrictItem;
		tmp->nLevel = DISTRICT_TREE_SHENG_ICON;
		tmp->strCode = strDM;
		tmp->strName = strMC;
		tmp->strParentID = _T("");
		m_parrDistrict->Add(tmp);
		if (strDM.GetLength() < m_strParentDM.GetLength())
		{
			m_strParentDM = strDM;
			m_strParentMC = strMC;
		}
	} else if(strDM.GetLength() == 4)//�м�
	{
		_tagDistrictItem *tmp = new _tagDistrictItem;
		tmp->nLevel = DISTRICT_TREE_SHI_ICON;
		tmp->strCode = strDM;
		tmp->strName = strMC;
		tmp->strParentID = strDM.Left(2);
		m_parrDistrict->Add(tmp);
		if (strDM.GetLength() < m_strParentDM.GetLength())
		{
			m_strParentDM = strDM;
			m_strParentMC = strMC;
		}
	} else if(strDM.GetLength() == 6)//����
	{
		if (strMC.Find(_T("��"),0) > 0)//��
		{
			_tagDistrictItem *tmp = new _tagDistrictItem;
			tmp->nLevel = DISTRICT_TREE_XIAN_ICON;
			tmp->strCode = strDM;
			tmp->strName = strMC;
			tmp->strParentID = strDM.Left(4);
			m_parrDistrict->Add(tmp);
			if (strDM.GetLength() < m_strParentDM.GetLength())
			{
				m_strParentDM = strDM;
				m_strParentMC = strMC;
			}
		} 
		else//��
		{
			_tagDistrictItem *tmp = new _tagDistrictItem;
			tmp->nLevel = DISTRICT_TREE_QU_ICON;
			tmp->strCode = strDM;
			tmp->strName = strMC;
			tmp->strParentID = strDM.Left(4);
			tmp->strParentID = _T("");
			m_parrDistrict->Add(tmp);
			if (strDM.GetLength() < m_strParentDM.GetLength())
			{
				m_strParentDM = strDM;
				m_strParentMC = strMC;
			}
		}

	} else if(strDM.GetLength() == 9)//����
	{
		if (strMC.Find(_T("��"),0) > 0)//��
		{
			_tagDistrictItem *tmp = new _tagDistrictItem;
			tmp->nLevel = DISTRICT_TREE_ZHEN_ICON;
			tmp->strCode = strDM;
			tmp->strName = strMC;
			tmp->strParentID = strDM.Left(6);
			m_parrDistrict->Add(tmp);
			if (strDM.GetLength() < m_strParentDM.GetLength())
			{
				m_strParentDM = strDM;
				m_strParentMC = strMC;
			}
		} 
		else//��
		{
			_tagDistrictItem *tmp = new _tagDistrictItem;
			tmp->nLevel = DISTRICT_TREE_XIANG_ICON;
			tmp->strCode = strDM;
			tmp->strName = strMC;
			tmp->strParentID = strDM.Left(6);
			m_parrDistrict->Add(tmp);
			if (strDM.GetLength() < m_strParentDM.GetLength())
			{
				m_strParentDM = strDM;
				m_strParentMC = strMC;
			}
		}
	} else if(strDM.GetLength() == 12)//��
	{
		_tagDistrictItem *tmp = new _tagDistrictItem;
		tmp->nLevel = DISTRICT_TREE_CUN_ICON;
		tmp->strCode = strDM;
		tmp->strName = strMC;
		tmp->strParentID = strDM.Left(9);
		m_parrDistrict->Add(tmp);
		if (strDM.GetLength() < m_strParentDM.GetLength())
		{
			m_strParentDM = strDM;
			m_strParentMC = strMC;
		}
	}
}

void CLPGHJQDLTBCXDlg::SortArray(int left, int right)
{
	//��������������������
	if (left>=right)
	{
		return;
	}

	int i = left;
	int j = right;
	//�ο���
	_tagDistrictItem *keyitem;
	CString strkeyCode;

	//�ο�ֵ
	keyitem = m_parrDistrict->GetAt(left);
	strkeyCode = keyitem->strCode;

	while(i < j)
	{
		CString strtmpCode;
		strtmpCode = m_parrDistrict->GetAt(j)->strCode;
		while(i < j && strkeyCode < strtmpCode)
		{
			j--;
			strtmpCode = m_parrDistrict->GetAt(j)->strCode;
		}
		m_parrDistrict->SetAt(i,m_parrDistrict->GetAt(j));
		strtmpCode = m_parrDistrict->GetAt(i)->strCode;
		while(i < j && strkeyCode > strtmpCode)
		{
			i++;
			strtmpCode = m_parrDistrict->GetAt(i)->strCode;
		}
		m_parrDistrict->SetAt(j,m_parrDistrict->GetAt(i));
	}
	m_parrDistrict->SetAt(i,keyitem);

	SortArray(left, i - 1);/*��ͬ���ķ�ʽ�Էֳ�������ߵ�С����п���*/
	SortArray(i + 1, right);/*��ͬ���ķ�ʽ�Էֳ������ұߵ�С����п���*/

}

BOOL CLPGHJQDLTBCXDlg::InitDlComboBox()
{
	if (GetGHDLMC() == TRUE)
	{
		m_cbxGHDL.AddString(_T("ȫ��"));
	}

	for (int i = 0; i < m_arrDLMC.GetCount() - 1; i++)
	{
		m_cbxGHDL.AddString(m_arrDLMC.GetAt(i));
	}

	return TRUE;
}

BOOL CLPGHJQDLTBCXDlg::GetGHDLMC()
{
	ITablePtr ipTable;
	m_ipFtWorkspace->OpenTable(_T("T_EnumValue"),&ipTable);

	if (ipTable == NULL) return FALSE;

	IQueryFilterPtr ipQuery(CLSID_QueryFilter);
	ipQuery->put_SubFields(_T("*"));
	ipQuery->put_WhereClause(_T("ENUMNAME = 'GHDLDM'"));
	ICursorPtr ipCursor;
	ipTable->Search(ipQuery,VARIANT_FALSE,&ipCursor);

	if (ipCursor == NULL) return FALSE;

	IRowPtr ipRow = NULL;
	ipCursor->NextRow(&ipRow);

	while(ipRow != NULL)
	{
		IFieldsPtr ipFields;
		ipRow->get_Fields(&ipFields);
		long nXZQIndex;

		ipFields->FindField(_T("ENUMMC"),&nXZQIndex);
		VARIANT MCtmp;
		ipRow->get_Value(nXZQIndex,&MCtmp);


		//���뵽����
		if (MCtmp.bstrVal != NULL)
		{
			m_arrDLMC.Add(MCtmp.bstrVal);
		}
		ipCursor->NextRow(&ipRow);
	}	

	return TRUE;
}

void CLPGHJQDLTBCXDlg::OnBnClickedMeterRd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	////�����ı��������
	//UpdateData(TRUE);
	//if (m_strUnit == _T("(Ķ)"))
	//{
	//	m_nTBDL_Max *= 666.67;
	//	m_nTBDL_Min *= 666.67;
	//	m_nTBDLMJ_Max *= 666.67;
	//	m_nTBDLMJ_Min *= 666.67;
	//}else if (m_strUnit == _T("(����)"))
	//{
	//	m_nTBDL_Max *= 10000.0;
	//	m_nTBDL_Min *= 10000.0;
	//	m_nTBDLMJ_Max *= 10000.0;
	//	m_nTBDLMJ_Min *= 10000.0;
	//}
	//���µ�λ�ı���������ʾ
	m_strUnit = _T("(ƽ����)");
	UpdateData(FALSE);
}

void CLPGHJQDLTBCXDlg::OnBnClickedMuRd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	////���ı��������
	//UpdateData(TRUE);
	//if (m_strUnit == _T("(ƽ����)"))
	//{
	//	m_nTBDL_Max *= 0.0015;
	//	m_nTBDL_Min *= 0.0015;
	//	m_nTBDLMJ_Max *= 0.0015;
	//	m_nTBDLMJ_Min *= 0.0015;
	//}else if (m_strUnit == _T("(����)"))
	//{
	//	m_nTBDL_Max *= 15.0;
	//	m_nTBDL_Min *= 15.0;
	//	m_nTBDLMJ_Max *= 15.0;
	//	m_nTBDLMJ_Min *= 15.0;
	//}
	//���µ�λ�ı���������ʾ
	m_strUnit = _T("(Ķ)");
	UpdateData(FALSE);
}

void CLPGHJQDLTBCXDlg::OnBnClickedHectareRd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	////�����ı��������
	//UpdateData(TRUE);
	//if (m_strUnit == _T("(ƽ����)"))
	//{
	//	m_nTBDL_Max /= 10000.0;
	//	m_nTBDL_Min /= 10000.0;
	//	m_nTBDLMJ_Max /= 10000.0;
	//	m_nTBDLMJ_Min /= 10000.0;
	//}else if (m_strUnit == _T("(Ķ)"))
	//{
	//	m_nTBDL_Max *= 0.0667;
	//	m_nTBDL_Min *= 0.0667;
	//	m_nTBDLMJ_Max *= 0.0667;
	//	m_nTBDLMJ_Min *= 0.0667;
	//}
	//���µ�λ�ı���������ʾ
	m_strUnit = _T("(����)");
	UpdateData(FALSE);
}
void CLPGHJQDLTBCXDlg::OnCbnSelchangeXzcombo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nindex = m_cbxTown.GetCurSel();
	if (nindex < 0) return;
	CString strTmp;
	m_cbxTown.GetLBText(nindex,strTmp);
	if (strTmp == _T("ȫ��"))
	{
		m_cbxVillage.SelectString(0,_T("ȫ��"));
		m_cbxVillage.EnableWindow(FALSE);
	}

	_tagDistrictItem *item;
	 item = (_tagDistrictItem* )m_cbxTown.GetItemData(nindex);
	 if (item != NULL)
	 {
		 m_cbxVillage.EnableWindow(TRUE);
		 FlashComboBox(item);
	 }	
}

BOOL CLPGHJQDLTBCXDlg::FlashComboBox(_tagDistrictItem *tagParent)
{
	_tagDistrictItem *item;
	//����ֻ��Ҫ�õ��弶�����ݸ��ڵ㼶�𣬳�ʼ���Ӽ�
	switch(tagParent->nLevel)
	{
	case DISTRICT_TREE_SHENG_ICON:
	case DISTRICT_TREE_SHI_ICON:
	case DISTRICT_TREE_XIAN_ICON:
	case DISTRICT_TREE_QU_ICON:
		break;
	case DISTRICT_TREE_XIANG_ICON:		
	case DISTRICT_TREE_ZHEN_ICON:
		m_cbxVillage.ResetContent();
		m_cbxVillage.AddString(_T("ȫ��"));
		for (int i = 0; i < m_parrDistrict->GetCount() - 1; i++)
		{
			item = m_parrDistrict->GetAt(i);
			if (item->strParentID == tagParent->strCode)
			{
				m_cbxVillage.AddString(item->strName);
				m_cbxVillage.SetItemData(m_cbxVillage.GetCount() - 1, (DWORD_PTR)item);
			}
		}
		m_cbxVillage.SelectString(0,_T("ȫ��"));
		break;
	default:
		break;
	}
	return TRUE;
}

void CLPGHJQDLTBCXDlg::OnBnClickedCxbutton()
{

	UpdateData();
	CString strXZQ = _T("");//��֯��ѯ����
	CString strArea = _T("");
	CString strDLMC = _T("");
	CString strWhere = _T("");

	//��ѯ��������Χ
	if (m_strTown == _T("ȫ��"))
	{
		strXZQ += _T("XZQMC = '") + m_strParentMC + _T("'");
	}else if (m_strVillage == _T("ȫ��"))
	{
		strXZQ += _T("XZQMC = '") + m_strTown + _T("'");
	}else
	{
		strXZQ += _T("XZQMC = '") + m_strVillage + _T("'");
	}
	//��ѯ��������
	if (m_strGHDL == _T("ȫ��"))
	{
		
	}else
	{
		strDLMC += _T(" and DLMC = '") + m_strGHDL + _T("'");
	}	
	//��ѯ����,����Ϊƽ����
	double nTBDL_Max,nTBDL_Min,nTBDLMJ_Max,nTBDLMJ_Min;
	if (m_strUnit == _T("��"))
	{
		nTBDL_Max = m_nTBDL_Max;
		nTBDL_Min = m_nTBDL_Min;
		nTBDLMJ_Max = m_nTBDLMJ_Max;
		nTBDLMJ_Min = m_nTBDLMJ_Min;
	}else if (m_strUnit == _T("(Ķ)"))
	{
		nTBDL_Max = m_nTBDL_Max * 666.67;
		nTBDL_Min = m_nTBDL_Min * 666.67;
		nTBDLMJ_Max = m_nTBDLMJ_Max * 666.67;
		nTBDLMJ_Min = m_nTBDLMJ_Min * 666.67;
	}else if (m_strUnit == _T("(����)"))
	{
		nTBDL_Max = m_nTBDL_Max * 10000.0;
		nTBDL_Min = m_nTBDL_Min * 10000.0;
		nTBDLMJ_Max = m_nTBDLMJ_Max * 10000.0;
		nTBDLMJ_Min = m_nTBDLMJ_Min * 10000.0;
	}
	//�ų���ѯ������ֵ
	if (m_nTBDL_Min != 0)
	{
		CString strTmp;
		strTmp.Format(_T(" and TBMJ >= %lf "), nTBDL_Min);
		strArea += strTmp;
	}
	if (m_nTBDL_Max != 0)
	{
		CString strTmp;
		strTmp.Format(_T(" and TBMJ <= %lf "), nTBDL_Max);
		strArea += strTmp;
	}
	if (m_nTBDLMJ_Min != 0)
	{
		CString strTmp;
		strTmp.Format(_T(" and TBMJ >= %lf "), nTBDLMJ_Min);
		strArea += strTmp;
	}if (m_nTBDLMJ_Max != 0)
	{
		CString strTmp;
		strTmp.Format(_T(" and TBMJ <= %lf "), nTBDLMJ_Max);
		strArea += strTmp;
	}
	
	//�ϲ���ѯ����
	strWhere += strXZQ + strDLMC + strArea;

	IQueryFilterPtr ipQuery(CLSID_QueryFilter);
	ipQuery->put_SubFields(_T("*"));
	ipQuery->put_WhereClause(_bstr_t(strWhere));
	InitGridCtrl();
	ShowResrult(ipQuery);
	
}

BOOL CLPGHJQDLTBCXDlg::InitGridCtrl()
{
	CRect rect;
	CBCGPGridColors GridTheme;

	if (m_wndResultGrid.GetSafeHwnd() == NULL)
	{	
		m_RectForGrid.GetClientRect(&rect);
		m_RectForGrid.MapWindowPoints (this, &rect);
		if (rect == NULL)
		{
			return FALSE;
		}

		m_wndResultGrid.Create (WS_CHILD |WS_VISIBLE, rect, this, 1);
		m_wndResultGrid.EnableMarkSortedColumn (FALSE);//���ñ�����ʼ��Ϊ��
		m_wndResultGrid.EnableRowHeader (TRUE);//��ʾ��ͷ
		m_wndResultGrid.EnableLineNumbers ();//��ʾ�к�
		m_wndResultGrid.SetReadOnly(TRUE);
		GridTheme = m_wndResultGrid.GetColorTheme();
		GridTheme.m_clrHorzLine = RGB (0, 0, 0);//����ˮƽ��������ɫ
		GridTheme.m_clrVertLine = RGB (0, 0, 0);//���ô�ֱ��������ɫ
		m_wndResultGrid.SetColorTheme(GridTheme);

		//���ڵ�һ�ε���ʱ�����һ���տ�
		const int nColumns = 6;
		//�����ͷ
		for (int nColumn = 0; nColumn < nColumns; nColumn++)
		{
			CString strColumn;
			strColumn.Format (_T("%c"), _T('A') + nColumn);

			m_wndResultGrid.InsertColumn (nColumn, strColumn, 80);
		}
		//�����ͷ
		for (int nRow = 0; nRow < 10; nRow++)
		{
			CBCGPGridRow* pRow = m_wndResultGrid.CreateRow (m_wndResultGrid.GetColumnCount ());
			m_wndResultGrid.AddRow (pRow, FALSE);
		}
	}else{
		//������ǵ�һ�ε��ã���ôɾ������֮ǰ������
		m_wndResultGrid.DeleteAllColumns();//ɾ��������
		m_wndResultGrid.RemoveAll();//ɾ��������
	}
	//ˢ����ʾ�����ý���
	m_wndResultGrid.AdjustLayout ();
	m_wndResultGrid.SetFocus ();

	return TRUE;
}

BOOL CLPGHJQDLTBCXDlg::ShowResrult(IQueryFilterPtr ipQuery)
{
	IFeatureClassPtr ipFtClass;
	m_ipFtWorkspace->OpenFeatureClass(_T("JQDLTB"),&ipFtClass);
	if (ipFtClass == NULL) return FALSE;

	IFeatureCursorPtr ipFtCursor;
	ipFtClass->Search(ipQuery,VARIANT_FALSE,&ipFtCursor);
	
	if (ipFtCursor == NULL) return FALSE;

	int nResult = 0;
	IFeaturePtr ipFt;
	ipFtCursor->NextFeature(&ipFt);

	//�����ͷ
	if (ipFt != NULL)
	{
		IFieldsPtr ipFields;
		long nNumField;
		ipFt->get_Fields(&ipFields);
		if (ipFields == NULL)return FALSE;

		ipFields->get_FieldCount(&nNumField);
		for (int i = 0; i < nNumField -1; i++)
		{
			IFieldPtr ipField;
			BSTR bstrName;
			ipFields->get_Field(i,&ipField);
			ipField->get_Name(&bstrName);
			m_wndResultGrid.InsertColumn(i,bstrName,80);
		}
	}
	//�����������
	while(ipFt != NULL)
	{
		nResult ++;

		IFieldsPtr ipFields;
		long nNumField;
		ipFt->get_Fields(&ipFields);
		if (ipFields == NULL)return FALSE;

		ipFields->get_FieldCount(&nNumField);
		CBCGPGridRow *pGridRow = m_wndResultGrid.CreateRow(nNumField);
		for (int i = 0; i < nNumField -1; i++)
		{
			IFieldPtr ipField;
			VARIANT vtTmp;
			ipFields->get_Field(i,&ipField);
			ipFt->get_Value(i,&vtTmp);
			pGridRow->GetItem(i)->SetValue(vtTmp);
		}
		m_wndResultGrid.AddRow(pGridRow);

		ipFtCursor->NextFeature(&ipFt);
	}

	m_wndResultGrid.AdjustLayout();
	m_wndResultGrid.SetFocus();

	m_strResultNum.Format(_T("��%d������"),nResult);
	UpdateData(FALSE);

	return TRUE;
}

CString CLPGHJQDLTBCXDlg::VTtoCString(VARIANT m_variant)
{
	CString temp;
	switch(m_variant.vt)
	{
	 	case 3:
	 		temp.Format(L"%ld",m_variant.iVal);
	 		break;//long
	 	case 5:
	 		temp.Format(L"%lf",m_variant.dblVal);
	 		break;//double
		case 8:
			temp = m_variant.bstrVal;
			break;//BSTR
		case 13:
			temp = L"PolyGon";
			break;//IUnknown FAR*
		default:
			temp.Empty();
			break;
	}
	return temp;
}


void CLPGHJQDLTBCXDlg::OnDestroy()
{
	CGSBCGDialog::OnDestroy();
	delete this;
	// TODO: �ڴ˴������Ϣ����������
}
