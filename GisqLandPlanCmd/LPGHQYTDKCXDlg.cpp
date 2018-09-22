/*********************************************
�ļ���:  LPGHQYTDKCXDlg
������:  luchencheng
��д����:2016-10-013
��Ҫ����:�滮����;�ؿ��ѯ���ܴ���
�޸ļ�¼(who-when-why):
********************************************/
// LPGHQYTDKCXDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LPGHQYTDKCXDlg.h"


// CLPGHQYTDKCXDlg �Ի���

IMPLEMENT_DYNAMIC(CLPGHQYTDKCXDlg, CGSBCGDialog)

CLPGHQYTDKCXDlg::CLPGHQYTDKCXDlg(GisqLib::_DGisqFrameworkCOMPtr	ipFramework, CWnd* pParent /*=NULL*/)
	: CGSBCGDialog(CLPGHQYTDKCXDlg::IDD, pParent)
	, m_strTown(_T(""))
	, m_strGHDL(_T(""))
	, m_nAREA_Min(0)
	, m_nAREA_Max(0)
	, m_strUnit(_T("(ƽ����)"))
	, m_strResultNum(_T(""))
{
	m_ipFramework = ipFramework;
	m_parrDistrict = new CArray<_tagDistrictItem *,_tagDistrictItem *>;
}

CLPGHQYTDKCXDlg::~CLPGHQYTDKCXDlg()
{
	if (m_parrDistrict != NULL)
	{
		m_parrDistrict->RemoveAll();
		delete m_parrDistrict;
	}
}

void CLPGHQYTDKCXDlg::DoDataExchange(CDataExchange* pDX)
{
	CGSBCGDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_XZCOMBO_3, m_strTown);
	DDX_CBString(pDX, IDC_GHDLCOMBO_3, m_strGHDL);
	DDX_Control(pDX, IDC_XZCOMBO_3, m_cbxTown);
	DDX_Control(pDX, IDC_GHDLCOMBO_3, m_cbxGHDL);

	DDX_Text(pDX, IDC_TDYTQMJMIN, m_nAREA_Min);
	DDX_Text(pDX, IDC_TDYTQMJMAX, m_nAREA_Max);
	DDX_Control(pDX, IDC_TDYTQMJMIN, m_editAREA_Min);
	DDX_Control(pDX, IDC_TDYTQMJMAX, m_editAREA_Max);

	DDX_Text(pDX, IDC_UNITTEXT31, m_strUnit);
	DDX_Text(pDX, IDC_UNITTEXT32, m_strUnit);

	DDX_Control(pDX, IDC_GRIDAREA_3, m_RectForGrid);
	DDX_Text(pDX, IDC_RESCOUNT_3, m_strResultNum);
}


BEGIN_MESSAGE_MAP(CLPGHQYTDKCXDlg, CGSBCGDialog)
	ON_BN_CLICKED(IDC_METER_RD_3, &CLPGHQYTDKCXDlg::OnBnClickedMeterRd3)
	ON_BN_CLICKED(IDC_MU_RD_3, &CLPGHQYTDKCXDlg::OnBnClickedMuRd3)
	ON_BN_CLICKED(IDC_HECTARE_RD_3, &CLPGHQYTDKCXDlg::OnBnClickedHectareRd3)
	ON_BN_CLICKED(IDC_CXBUTTON_3, &CLPGHQYTDKCXDlg::OnBnClickedCxbutton3)
END_MESSAGE_MAP()


// CLPGHQYTDKCXDlg ��Ϣ�������

BOOL CLPGHQYTDKCXDlg::OnInitDialog()
{
	CGSBCGDialog::OnInitDialog();

	//��ʼ��ѡ�в�ѯ���Ϊƽ����
	((CButton *)GetDlgItem(IDC_METER_RD_3))->SetCheck(TRUE);
	//��ʼ���ı���ؼ�
	m_editAREA_Min.SetValidChars(_T("1234567890."));//�����ַ��б�
	m_editAREA_Max.SetValidChars(_T("1234567890."));//�����ַ��б�

	m_editAREA_Min.SetWindowText(_T("0"));
	m_editAREA_Max.SetWindowText(_T("0"));

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
	m_cbxGHDL.SelectString(0,_T("ȫ��"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CLPGHQYTDKCXDlg::InitTownComboBox()
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

BOOL CLPGHQYTDKCXDlg::GetXzqDm()
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

void CLPGHQYTDKCXDlg::AddToArray(BSTR bstrDM,BSTR bstrMC)
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

void CLPGHQYTDKCXDlg::SortArray(int left, int right)
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

BOOL CLPGHQYTDKCXDlg::InitDlComboBox()
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

BOOL CLPGHQYTDKCXDlg::GetGHDLMC()
{
	ITablePtr ipTable;
	m_ipFtWorkspace->OpenTable(_T("T_EnumValue"),&ipTable);

	if (ipTable == NULL) return FALSE;

	IQueryFilterPtr ipQuery(CLSID_QueryFilter);
	ipQuery->put_SubFields(_T("*"));
	ipQuery->put_WhereClause(_T("ENUMNAME = 'TDYTQLXDM'"));
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

BOOL CLPGHQYTDKCXDlg::InitGridCtrl()
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
void CLPGHQYTDKCXDlg::OnBnClickedMeterRd3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_strUnit = _T("(ƽ����)");
	UpdateData(FALSE);
}

void CLPGHQYTDKCXDlg::OnBnClickedMuRd3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_strUnit = _T("(Ķ)");
	UpdateData(FALSE);
}

void CLPGHQYTDKCXDlg::OnBnClickedHectareRd3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_strUnit = _T("(����)");
	UpdateData(FALSE);
}

void CLPGHQYTDKCXDlg::OnBnClickedCxbutton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	CString strXZQ = _T("");//��֯��ѯ����
	CString strArea = _T("");
	CString strDLMC = _T("");
	CString strWhere = _T("");

	//��ѯ��������Χ
	if (m_strTown == _T("ȫ��"))
	{
		strXZQ += _T("XZQMC = '*'");
	}else
	{
		strXZQ += _T("XZQMC = '") + m_strTown + _T("'");
	}
	//��ѯ��������
	if (m_strGHDL == _T("ȫ��"))
	{
		
	}else
	{
		strDLMC += _T(" and TDYTQLX = '") + m_strGHDL + _T("'");
	}	
	//��ѯ����,����Ϊƽ����
	double nAREA_Max,nAREA_Min;
	if (m_strUnit == _T("��"))
	{
		nAREA_Max = m_nAREA_Max;
		nAREA_Min = m_nAREA_Min;
	}else if (m_strUnit == _T("(Ķ)"))
	{
		nAREA_Max = m_nAREA_Max * 666.67;
		nAREA_Min = m_nAREA_Min * 666.67;

	}else if (m_strUnit == _T("(����)"))
	{
		nAREA_Max = m_nAREA_Max * 10000.0;
		nAREA_Min = m_nAREA_Min * 10000.0;
	}
	//�ų���ѯ������ֵ
	if (m_nAREA_Min != 0)
	{
		CString strTmp;
		strTmp.Format(_T(" and TBMJ >= %lf "), nAREA_Min);
		strArea += strTmp;
	}
	if (m_nAREA_Max != 0)
	{
		CString strTmp;
		strTmp.Format(_T(" and TBMJ <= %lf "), nAREA_Max);
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

BOOL CLPGHQYTDKCXDlg::ShowResrult(IQueryFilterPtr ipQuery)
{
	IFeatureClassPtr ipFtClass;
	m_ipFtWorkspace->OpenFeatureClass(_T("TDYTQ"),&ipFtClass);
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
