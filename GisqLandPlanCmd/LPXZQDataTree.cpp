#include "stdafx.h"
#include "LPXZQDataTree.h"


void GetLevelandParent(CString strDM, CString strMC, int *nLevel, CString *pstrParent)
{
	if (strDM.GetLength() == 2)//ʡ��
	{
		*nLevel = DISTRICT_TREE_SHENG_ICON;
		*pstrParent = _T("");
	} else if(strDM.GetLength() == 4)//�м�
	{
		*nLevel = DISTRICT_TREE_SHI_ICON;
		*pstrParent = strDM.Left(2);
	} else if(strDM.GetLength() == 6)//����
	{
		*nLevel = DISTRICT_TREE_XIAN_ICON;
		*pstrParent = strDM.Left(4);
	} else if(strDM.GetLength() == 9)//����
	{

		*nLevel = DISTRICT_TREE_XIANG_ICON;
		*pstrParent = strDM.Left(6);
	} else if(strDM.GetLength() == 12)//��
	{
		*nLevel = DISTRICT_TREE_CUN_ICON;
		*pstrParent = strDM.Left(9);
	}
}

//���������ݽṹ
CComQIPtr<ILPXZQDataItem> CLPXZQDataTree::XZQTreeRoot = (IUnknown*)NULL;
BOOL CLPXZQDataTree::bIsXZQInit = FALSE;
CString CLPXZQDataTree::m_strParentID = _T("1234567890123");//��һ������12λ���ַ������ڻ�ȡ��߼���������
CMapStringToString *CLPXZQDataTree::m_mapXZQ = new CMapStringToString;


// LPXZQDataTree.cpp : CLPXZQDataTree ��ʵ��

HRESULT CLPXZQDataTree::InitXZQTree(CComQIPtr<IFeatureWorkspace> spFtWorkspace)
 {
 	//�����ȡ�ɹ���ʼ����
 	if (spFtWorkspace == NULL) return E_FAIL;
 	CComQIPtr<ITable>spTable;
 	spFtWorkspace->OpenTable(_T("T_XZQMC"),&spTable);
 	if (spTable != NULL)
 	{
 		CComQIPtr<esriICursor> spCursor;
 		spTable->Search(NULL,VARIANT_FALSE,&spCursor);
 		if (spCursor == NULL)
 		{return FALSE;}
 		CComQIPtr<esriIRow> spRow;
 		spCursor->NextRow(&spRow);
 		while(spRow != NULL)
 		{
 			CComQIPtr<IFields> spFields;
 			spRow->get_Fields(&spFields);
 			long nXZQIndex;
 
 			spFields->FindField(_T("XZQDM"),&nXZQIndex);
 			VARIANT DMtmp;
 			spRow->get_Value(nXZQIndex,&DMtmp);
 
 			spFields->FindField(_T("XZQMC"),&nXZQIndex);
 			VARIANT MCtmp;
 			spRow->get_Value(nXZQIndex,&MCtmp);
 			//���뵽����
 			if (DMtmp.bstrVal != NULL && MCtmp.bstrVal != NULL)
 			{
 				m_mapXZQ->SetAt(DMtmp.bstrVal,MCtmp.bstrVal);
 				CString strDM = DMtmp.bstrVal;
 				if (strDM.GetLength() < m_strParentID.GetLength())
 				{
 					m_strParentID = strDM;
 				}
 			}
 			spRow.Release();
 			spCursor->NextRow(&spRow);
 		}		
 	}
 	//����map��ӵ�tree
 	CreateDataTree(m_strParentID);
 
 	bIsXZQInit = TRUE;
 	return S_OK;
 }
 
 HRESULT CLPXZQDataTree::CreateDataTree(CString strParentID)
 {
 	CString strDM;
 	CString strMC;
 	int nLevel = 0;
 	CString strParent = _T("");
 
 	if (XZQTreeRoot == NULL)
 	{
		XZQTreeRoot.CoCreateInstance(CLSID_LPXZQDataItem);
 		m_mapXZQ->Lookup(m_strParentID,strMC);
 		GetLevelandParent(m_strParentID,strMC,&nLevel,&strParent);
 		XZQTreeRoot->Set_ItemName(strMC.AllocSysString());
 		XZQTreeRoot->Set_ItemCode(m_strParentID.AllocSysString());
 		XZQTreeRoot->Set_ItemLevel((SHORT)nLevel);
 		XZQTreeRoot->Set_ItemParentID(NULL);
 		XZQTreeRoot->InsertTreeItem(XZQTreeRoot,(IUnknown*)m_mapXZQ);//�Ӹ���ʼ�ݹ�Ĳ���������
 		return S_OK;
 	}
 	return S_OK;
 }

 STDMETHODIMP CLPXZQDataTree::GetXZQMap(IUnknown** mapXZQData)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: �ڴ����ʵ�ִ���
	 *mapXZQData = (IUnknown*) m_mapXZQ;
	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataTree::GetRootItem(IUnknown* pFtWorkspace, IUnknown** RootItem)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());
	 CComQIPtr<IFeatureWorkspace> spFtWorkSpace = (CComQIPtr<IFeatureWorkspace>)pFtWorkspace;
	 // TODO: �ڴ����ʵ�ִ���
	 if (bIsXZQInit == FALSE)
	 {
		 InitXZQTree2(spFtWorkSpace);
	 }
	 *RootItem = XZQTreeRoot;

	 return S_OK;
 }
//edit by lcc 2016.12.14 ��������������ȡ��ʽΪADO�ӿ�
 HRESULT CLPXZQDataTree::InitXZQTree2(CComQIPtr<IFeatureWorkspace> spFtWorkspace)
 {
	 //�����ȡ�ɹ���ʼ����
	//��������
	// BCGPMessageBox(_T("��ʼ��ȡ��������"),_T(""),MB_OK);
	 HRESULT hr;
	 _variant_t  vt;
	_ConnectionPtr ipADOConn;
	_RecordsetPtr pRst; 
	hr = ipADOConn.CreateInstance(__uuidof(Connection));
	hr = pRst.CreateInstance(__uuidof(Recordset));
	//��������
// 	CString connStr=_T("");  
// 	connStr.Format(_T("Provider=OraOLEDB.Oracle.1;User ID=%s;Password=%s;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=%s)(PORT=%s))(CONNECT_DATA=(SERVICE_NAME=%s)));Persist Security Info=False"), 
// 		_T("tdgh"), _T("tdgh"),_T( "192.168.8.136"), _T("1521"), _T("orcl"));
// 	hr = ipADOConn->Open(connStr.AllocSysString(), _T("tdgh"), _T("tdgh"), adConnectUnspecified);

	hr = ConnToOracle(ipADOConn);
	if (hr != S_OK)
	{
		ipADOConn->Close();
		return hr;
	}
	//����������view
	CComQIPtr<IWorkspace2> spWorkspace2;
	spWorkspace2 = spFtWorkspace;
	if (spWorkspace2 == NULL) return E_FAIL;
	VARIANT_BOOL bIsProvince = VARIANT_FALSE;
	VARIANT_BOOL bIsCity = VARIANT_FALSE;
	VARIANT_BOOL bIsCounty = VARIANT_FALSE;
	//�����������������Ƿ����
	spWorkspace2->get_NameExists(esriDTFeatureClass,_bstr_t("XZQZ"),&bIsProvince);
	spWorkspace2->get_NameExists(esriDTFeatureClass,_bstr_t("XZQS"),&bIsCity);
	spWorkspace2->get_NameExists(esriDTFeatureClass,_bstr_t("XZQX"),&bIsCounty);
	//�����������������Ƿ�Ϊ��
	if (bIsProvince == VARIANT_TRUE)
	{
		CComQIPtr<IFeatureClass> spFtClass;
		long nCount;
		spFtWorkspace->OpenFeatureClass(_T("XZQZ"),&spFtClass);
		spFtClass->FeatureCount(NULL,&nCount);
		if (nCount == 0)
		{
			bIsProvince = VARIANT_FALSE;
		}
	}
	if (bIsCity == VARIANT_TRUE)
	{
		CComQIPtr<IFeatureClass> spFtClass;
		long nCount;
		spFtWorkspace->OpenFeatureClass(_T("XZQS"),&spFtClass);
		spFtClass->FeatureCount(NULL,&nCount);
		if (nCount == 0)
		{
			bIsCity = VARIANT_FALSE;
		}
	}
	if (bIsCounty == VARIANT_TRUE)
	{
		CComQIPtr<IFeatureClass> spFtClass;
		long nCount;
		spFtWorkspace->OpenFeatureClass(_T("XZQXZ"),&spFtClass);
		spFtClass->FeatureCount(NULL,&nCount);
		if (nCount == 0)
		{
			bIsCounty = VARIANT_FALSE;
		}
	}

	if (bIsCounty == VARIANT_FALSE) return E_FAIL;

	//edit by lcc 2016.12.17 �޸��˴������ͼ����SQL���
	CString strCreateView = _T("");
	strCreateView += _T("create or replace view xzq_view as \r\n");
	strCreateView += _T("select t.xzdm ||'-'||t.xzqdm as ���򼰴����,\r\n ");
	strCreateView += _T("t.xzqdm as �弶����,\r\n");
	strCreateView += _T("t.xzqmc as ����,\r\n");
	strCreateView += _T("s.xzqdm as �������,\r\n");
	strCreateView += _T("s.xzqmc as ��������,\r\n");
	strCreateView += _T("s.zxcq as �Ƿ����ĳ���,\r\n");
	strCreateView += _T("w.xzqdm as �ؼ�����,\r\n");
	strCreateView += _T("w.xzqmc as ����\r\n");
	if (bIsCity == VARIANT_FALSE)//����м�������
	{
		strCreateView += _T("from xzq t, xzqxz s, xzqx w\r\n");
		strCreateView += _T("where t.xzdm = s.xzqdm and substr(s.xzqdm, 0, 6�� = w.xzqdm");
	}else if (bIsProvince == VARIANT_FALSE)//���ʡ��������
	{
		strCreateView += _T(",");
		strCreateView += _T("x.xzqdm as �м�����,\r\n");
		strCreateView += _T("x.xzqmc as ����\r\n");
		strCreateView += _T("from xzq t, xzqxz s, xzqx w, xzqs x\r\n");
		strCreateView += _T("where t.xzdm = s.xzqdm and substr(s.xzqdm, 0, 6�� = w.xzqdm and substr(s.xzqdm, 0, 4�� = x.xzqdm");
	}else//���ʡ����
	{
		strCreateView += _T(",");
		strCreateView += _T("x.xzqdm as �м�����,\r\n");
		strCreateView += _T("x.xzqmc as ����,\r\n");
		strCreateView += _T("y.xzqdm as ʡ������,\r\n");
		strCreateView += _T("y.xzqmc as ʡ�� \r\n");
		strCreateView += _T("from xzq t, xzqxz s, xzqx w, xzqs x, xzqz y \r\n");
		strCreateView += _T("where t.xzdm = s.xzqdm and substr(s.xzqdm, 0, 6�� = w.xzqdm and substr(s.xzqdm, 0, 4�� = x.xzqdm and substr(s.xzqdm, 0, 2�� = y.xzqdm");
	}
	//edit by lcc 2016.12.17 end


	hr = ipADOConn->Execute(_bstr_t(strCreateView.AllocSysString()),&vt,adCmdText,&pRst);
	if (hr != S_OK)
	{
		return hr;
	}
	
	//��ȡ������view��
	CString strSQL = _T("select * from XZQ_VIEW order by ���򼰴����");
	hr = pRst->Open(_variant_t(_bstr_t(strSQL.AllocSysString())),_variant_t(ipADOConn.GetInterfacePtr()), adOpenStatic,adLockOptimistic,adCmdText);
	//��ȡ������Ϣ
	long nCount = 0;
	pRst->get_RecordCount(&nCount);
	hr = pRst->GetRows(nCount,vtMissing,vtMissing,&vt);
	if (!SUCCEEDED(hr)) return E_FAIL;
	//��ȡ����ָ�������
	void *pvd = NULL;
	SafeArrayAccessData(vt.parray,&pvd);
	LONG nFieldNum = 0;
	SafeArrayGetUBound(vt.parray,(UINT)1,&nFieldNum);
	//��������ȷ�����ڵ㼶��

	//������������
	m_mapXZQ = new CMapStringToString;
	CString strXZCUN = _T("");
	CString strXZQDM = _T("");
	CString strXZQCUNDM = _T("");
	CString strXZQXZDM = _T("");
	CString strXZQXDM = _T("");
	CString strXZQSDM = _T("");
	CString strXZQZDM = _T("");
	CString strXZQMC = _T("");
	CString strXZQXZMC = _T("");
	CString strXZQXMC = _T("");
	CString strXZQSMC = _T("");
	CString strXZQZMC = _T("");
	BSTR bstrXZQDM;
	BOOL bIsXZQSame = FALSE;//�����Ƿ���ͬ

	SHORT nZXCQ = 0;
	CComQIPtr<ILPXZQDataItem> SHIDataItem;
	CComQIPtr<ILPXZQDataItem> XIANDataItem;
	CComQIPtr<ILPXZQDataItem> ZXCQDataItem;
	CComQIPtr<ILPXZQDataItem> XZDataItem;
	CComQIPtr<ILPXZQDataItem> CUNDataItem;

	CString strLastXZCUN = _T("");
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		for (LONG nField = 0; nField <= nFieldNum; nField++)
		{
			VARIANT vtTmp = ((VARIANT*)pvd)[nField+(nIndex)*(nFieldNum + 1)];
			switch (nField)
			{
			case 0://���򼰴����
				strXZCUN = VariantToString(vtTmp);
				if (nIndex == 0)
				{
					strLastXZCUN = strXZCUN;
				}
// 				if (strLastXZCUN.Compare(VariantToString(vtTmp)) != 0)
// 				{
// 					strLastXZCUN = VariantToString(vtTmp);
// 				}
				break;
			case 1://�弶����
 				if (CUNDataItem != NULL)
 				{//����������һ����ͬ,������һ��
 					//CUNDataItem->Get_ItemCode(&bstrXZQDM);
 					//strXZQDM.Format(_T("%s"),bstrXZQDM);
 					if (strLastXZCUN.Compare(strXZCUN) == 0)//�����Ϊ�������ʹ弶���������������жϣ�����������
 					{
 						bIsXZQSame = TRUE;
 						break;
 					}else
 					{//����ͬ���ͷ���һ��ָ��,��ʼ������һ��
						strLastXZCUN = strXZCUN;
 						CUNDataItem.Release();
 					}
 				}
 				CUNDataItem.CoCreateInstance(CLSID_LPXZQDataItem);
 				CUNDataItem->Set_ItemCode(vtTmp.bstrVal);
 				CUNDataItem->Set_ItemLevel(DISTRICT_TREE_CUN_ICON);
 				strXZQCUNDM.Format(_T("%s"),vtTmp.bstrVal);
 				
				break;
			case 2://����
				CUNDataItem->Set_ItemName(vtTmp.bstrVal);
				strXZQMC.Format(_T("%s"),vtTmp.bstrVal);
				break;
			case 3://�������
				if (XZDataItem != NULL)
				{//����������һ����ͬ
					XZDataItem->Get_ItemCode(&bstrXZQDM);
					strXZQDM.Format(_T("%s"),bstrXZQDM);
					if (strXZQDM.Compare(vtTmp.bstrVal) == 0)
					{
 						if (CUNDataItem != NULL)
 						{
							CUNDataItem->Set_ItemParentID(vtTmp.bstrVal);
 							XZDataItem->InsertTreeItem2(CUNDataItem);
							m_mapXZQ->SetAt(strXZQCUNDM,strXZQMC);
 						}
						bIsXZQSame = TRUE;//������һ��ʣ�²��ֵı���
						break;
					}else
					{//����ͬ���ͷ���һ��ָ��,��ʼ������һ��
						XZDataItem.Release();
					}
				}
				XZDataItem.CoCreateInstance(CLSID_LPXZQDataItem);
				XZDataItem->Set_ItemCode(vtTmp.bstrVal);
				XZDataItem->Set_ItemLevel(DISTRICT_TREE_XIANG_ICON);
				strXZQXZDM.Format(_T("%s"),vtTmp.bstrVal);
				strXZQDM = strXZQXZDM.Left(6);
				XZDataItem->Set_ItemParentID(_bstr_t(strXZQDM));
				if (CUNDataItem != NULL)
				{
					CUNDataItem->Set_ItemParentID(vtTmp.bstrVal);
					XZDataItem->InsertTreeItem2(CUNDataItem);
					m_mapXZQ->SetAt(strXZQCUNDM,strXZQMC);
				}
				break;
			case 4://��������
				strXZQXZMC.Format(_T("%s"),vtTmp.bstrVal);
				XZDataItem->Set_ItemName(vtTmp.bstrVal);
				break;
			case 5://�Ƿ����ĳ���
				nZXCQ = vtTmp.iVal;
				break;
			case 6://�ؼ�����
				if (nField == nFieldNum - 1 && nIndex == 0)//������Ƕ���������,�ҵ�ǰ�ǵ�һ��
				{
					XZQTreeRoot.CoCreateInstance(CLSID_LPXZQDataItem);
					XIANDataItem = XZQTreeRoot;
				}else if (nField == nFieldNum - 1 && nIndex != 0)//������Ƕ���������,�Ҳ��ǵ�һ��,����
				{
					bIsXZQSame = TRUE;
				}else if (XIANDataItem != NULL)//���Ƕ�������
				{
					//����������һ����ͬ,������һ��
					XIANDataItem->Get_ItemCode(&bstrXZQDM);
					strXZQDM.Format(_T("%s"),bstrXZQDM);
					if (strXZQDM.Compare(vtTmp.bstrVal) == 0)
					{
						bIsXZQSame = TRUE;
					}else
					{//����ͬ���ͷ���һ��ָ��,��ʼ������һ��
						XIANDataItem.Release();
						XIANDataItem.CoCreateInstance(CLSID_LPXZQDataItem);
					}
				}else if (XIANDataItem == NULL)
				{
					XIANDataItem.CoCreateInstance(CLSID_LPXZQDataItem);
				}

				if (nZXCQ != 0)//��������ĳ���
				{
					if (ZXCQDataItem != NULL)
					{
						//����������һ����ͬ,������һ��
						ZXCQDataItem->Get_ItemCode(&bstrXZQDM);
						strXZQDM.Format(_T("%s"),bstrXZQDM);
						strXZQDM = strXZQDM.Left(strXZQDM.GetLength() - 3);
						if (strXZQDM.Compare(vtTmp.bstrVal) == 0)
						{
							//׷����������������
							BSTR bstrTmpDM;
							BSTR bstrTmpXZDM;
							CString strTmpDM;
							ZXCQDataItem->Get_ItemParentID(&bstrTmpDM);
							XZDataItem->Get_ItemCode(&bstrTmpXZDM);
							strTmpDM.Format(_T("%s,%s"),bstrTmpDM,bstrTmpXZDM);
							ZXCQDataItem->Set_ItemParentID(_bstr_t(strTmpDM));

							//���������������ڵ�
							ZXCQDataItem->InsertTreeItem2(XZDataItem);
							m_mapXZQ->SetAt(strXZQXZDM,strXZQXZMC);
							bIsXZQSame = TRUE;
						}else
						{
							ZXCQDataItem.Release();
							ZXCQDataItem.CoCreateInstance(CLSID_LPXZQDataItem);
							strXZQDM.Format(_T("%s000"),vtTmp.bstrVal);
							ZXCQDataItem->Set_ItemLevel(DISTRICT_TREE_XIANG_ICON);
							ZXCQDataItem->Set_ItemCode(_bstr_t(strXZQDM));
							ZXCQDataItem->Set_ItemName(_T("���ĳ���"));
							ZXCQDataItem->Set_ItemParentID(vtTmp.bstrVal);

							XIANDataItem->InsertTreeItem2(ZXCQDataItem);
							//׷����������������
							BSTR bstrTmpXZDM;
							XZDataItem->Get_ItemCode(&bstrTmpXZDM);
							ZXCQDataItem->Set_ItemParentID(bstrTmpXZDM);
							//���������������ڵ�
							ZXCQDataItem->InsertTreeItem2(XZDataItem);
							m_mapXZQ->SetAt(strXZQXZDM,strXZQXZMC);
						}
					}else
					{
						ZXCQDataItem.CoCreateInstance(CLSID_LPXZQDataItem);
						strXZQDM.Format(_T("%s000"),vtTmp.bstrVal);
						ZXCQDataItem->Set_ItemLevel(DISTRICT_TREE_XIANG_ICON);
						ZXCQDataItem->Set_ItemCode(_bstr_t(strXZQDM));
						ZXCQDataItem->Set_ItemName(_T("���ĳ���"));
						ZXCQDataItem->Set_ItemParentID(vtTmp.bstrVal);

						XIANDataItem->InsertTreeItem2(ZXCQDataItem);
						//׷����������������
						BSTR bstrTmpXZDM;
						XZDataItem->Get_ItemCode(&bstrTmpXZDM);
						ZXCQDataItem->Set_ItemParentID(bstrTmpXZDM);

						//���������������ڵ�
						ZXCQDataItem->InsertTreeItem2(XZDataItem);
						m_mapXZQ->SetAt(strXZQXZDM,strXZQXZMC);
					}
				}else//����������ĳ���
				{
					XIANDataItem->InsertTreeItem2(XZDataItem);
					m_mapXZQ->SetAt(strXZQXZDM,strXZQMC);
				}
				if (bIsXZQSame)
				{
					break;
				}
				XIANDataItem->Set_ItemCode(vtTmp.bstrVal);
				XIANDataItem->Set_ItemLevel(DISTRICT_TREE_XIAN_ICON);
				strXZQXDM.Format(_T("%s"),vtTmp.bstrVal);
				strXZQDM = strXZQXDM.Left(4);
				XIANDataItem->Set_ItemParentID(_bstr_t(strXZQDM));
				break;
			case 7://����
				XIANDataItem->Set_ItemName(vtTmp.bstrVal);
				strXZQXMC.Format(_T("%s"),vtTmp.bstrVal);
				break;
			case 8://�м�����
				if (nField == nFieldNum - 1 && nIndex == 0)//������Ƕ���������,�ҵ�ǰ�ǵ�һ��
				{
					XZQTreeRoot.CoCreateInstance(CLSID_LPXZQDataItem);
					SHIDataItem = XZQTreeRoot;
				}else if (nField == nFieldNum - 1 && nIndex != 0)//������Ƕ���������,�Ҳ��ǵ�һ��,����
				{
					if (XIANDataItem != NULL)
					{
						SHIDataItem->InsertTreeItem2(XIANDataItem);
						m_mapXZQ->SetAt(strXZQXDM,strXZQXMC);
					}
					bIsXZQSame = TRUE;
					break;
				}else if (SHIDataItem != NULL)//���Ƕ�������
				{
					//����������һ����ͬ,������һ��
					SHIDataItem->Get_ItemCode(&bstrXZQDM);
					strXZQDM.Format(_T("%s"),bstrXZQDM);
					if (strXZQDM.Compare(vtTmp.bstrVal) == 0)
					{
						if (XIANDataItem != NULL)
						{
							SHIDataItem->InsertTreeItem2(XIANDataItem);
							m_mapXZQ->SetAt(strXZQXDM,strXZQXMC);
						}
						bIsXZQSame = TRUE;
						break;
					}else
					{//����ͬ���ͷ���һ��ָ��,��ʼ������һ��
						SHIDataItem.Release();
						SHIDataItem.CoCreateInstance(CLSID_LPXZQDataItem);
					}
				}else if (SHIDataItem == NULL)
				{
					SHIDataItem.CoCreateInstance(CLSID_LPXZQDataItem);
				}
				SHIDataItem->Set_ItemCode(vtTmp.bstrVal);
				SHIDataItem->Set_ItemLevel(DISTRICT_TREE_SHI_ICON);
				strXZQSDM.Format(_T("%s"),vtTmp.bstrVal);
				strXZQDM = strXZQSDM.Left(2);
				SHIDataItem->Set_ItemParentID(_bstr_t(strXZQDM));
				if (XIANDataItem != NULL)
				{
					SHIDataItem->InsertTreeItem2(XIANDataItem);
					m_mapXZQ->SetAt(strXZQXDM,strXZQXMC);
				}
				break;
			case 9://����
				SHIDataItem->Set_ItemName(vtTmp.bstrVal);
				strXZQSMC.Format(_T("%s"),vtTmp.bstrVal);
				break;
			case 10://ʡ������
				if (nIndex == 0)//�����ǰ�ǵ�һ��(�����ʡһ���Ƕ���),�����Լ�,�����нڵ�
				{
					XZQTreeRoot.CoCreateInstance(CLSID_LPXZQDataItem);
					XZQTreeRoot->Set_ItemCode(vtTmp.bstrVal);
					XZQTreeRoot->Set_ItemLevel(DISTRICT_TREE_SHENG_ICON);
					XZQTreeRoot->Set_ItemParentID(_T(""));
					if (SHIDataItem != NULL)
					{
						XZQTreeRoot->InsertTreeItem2(SHIDataItem);
						m_mapXZQ->SetAt(strXZQSDM,strXZQSMC);
					}
				}else//������ǵ�һ��,�����нڵ�,����
				{
					if (SHIDataItem != NULL)
					{
						XZQTreeRoot->InsertTreeItem2(SHIDataItem);
						m_mapXZQ->SetAt(strXZQSDM,strXZQSMC);
					}
					bIsXZQSame = TRUE;
					break;
				}
				break;
			case 11://ʡ��
				//��Ҫ�������������һ���ֶ��Ѿ�������
				XZQTreeRoot->Set_ItemName(vtTmp.bstrVal);
				m_mapXZQ->SetAt(strXZQDM,vtTmp.bstrVal);
				break;
			default:
				break;
			}
			if (bIsXZQSame)//���������ͬ,������һ�еı���
			{
				bIsXZQSame = FALSE;
				break;
			}

		}
	}

	//��������������ʡ�������ֱ�������ﴦ��һ�λ�Ƚϼ�
	//�����ڵ�������������������ӵ�map
	CComBSTR bstrRootCode;
	CComBSTR bstrRootName;
	XZQTreeRoot->Get_ItemCode(&bstrRootCode);
	XZQTreeRoot->Get_ItemName(&bstrRootName);
	CString strRootCode(bstrRootCode);
	CString strRootName(bstrRootName);
	m_mapXZQ->SetAt(bstrRootCode,bstrRootName);
	bIsXZQInit = TRUE;
	 return S_OK;
 }
//edit by lcc 2016.12.14 end
  // LPXZQDataTree.cpp : CLPXZQDataEnum ��ʵ��

 STDMETHODIMP CLPXZQDataEnum::InsertItem(IUnknown* pNewItem, IUnknown* DataMap)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: �ڴ����ʵ�ִ���
	 CComQIPtr<ILPXZQDataItem> pItem = (CComQIPtr<ILPXZQDataItem>)pNewItem;
	 if (pNewItem == NULL) return S_OK;
	 int nCount = ChildItemsArray.GetCount();
	 if (nCount == 0)//���û���ֽڵ�ֱ�Ӳ���
	 {
		 ChildItemsArray.Add(pItem);
	 }else
	 {
		 CString strNewItemCode;
		 BSTR bstrNewItemCode = NULL;
		 CString strTmpCode;
		 BSTR bstrTmpCode = NULL;

		 pItem->Get_ItemCode(&bstrNewItemCode);
		 strNewItemCode = bstrNewItemCode;
		//���ж����β�Ľڵ�Ƚϴ�С
		 CComQIPtr<ILPXZQDataItem> pTmpItem = ChildItemsArray.GetAt(nCount-1);
		 pTmpItem->Get_ItemCode(&bstrTmpCode);
		 strTmpCode = bstrTmpCode;
		 if (strTmpCode < strNewItemCode)//����Ƚ�β�Ĵ󣬲��뵽��β
		 {
			 ChildItemsArray.Add(pItem);
			 pItem->InsertTreeItem(pItem,DataMap);
			 return S_OK;
		 }else
		 {
			 //�������нڵ㣬�ҵ���һ�����Լ��������Ŵ��λ�ò���ڵ�
			 for(int i = 0; i < nCount; i++)
			 {
				 CComQIPtr<ILPXZQDataItem> pTmpItem = ChildItemsArray.GetAt(i);
				 pTmpItem->Get_ItemCode(&bstrTmpCode);
				 strTmpCode = bstrTmpCode;
				 if (strTmpCode > strNewItemCode)
				 {
					 ChildItemsArray.InsertAt(i,pItem);
					 break;
				 }
			 }
		 }
		
	 }
	 pItem->InsertTreeItem(pItem,DataMap);
	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataEnum::GetItemCount(int* nCount)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: �ڴ����ʵ�ִ���
	 *nCount = ChildItemsArray.GetCount();

	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataEnum::GetItemAt(int nItem,IUnknown** pItem)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: �ڴ����ʵ�ִ���
	 *pItem = (IUnknown*)ChildItemsArray.GetAt(nItem);

	 return S_OK;
 }

 // LPXZQDataTree.cpp : CLPXZQDataItem ��ʵ��

 STDMETHODIMP CLPXZQDataItem::Get_ItemName(BSTR* bstrName)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: �ڴ����ʵ�ִ���
	 *bstrName = strName.AllocSysString();

	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataItem::Set_ItemName(BSTR bstrName)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: �ڴ����ʵ�ִ���
	 if (bstrName != NULL)
	 {
		 strName = bstrName;
	 }
	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataItem::Get_ItemCode(BSTR* bstrCode)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: �ڴ����ʵ�ִ���
	 *bstrCode = strCode.AllocSysString();

	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataItem::Set_ItemCode(BSTR bstrCode)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: �ڴ����ʵ�ִ���
	 if (bstrCode != NULL)
	 {
		 strCode = bstrCode;
	 }
	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataItem::Get_ItemParentID(BSTR* bstrParentID)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: �ڴ����ʵ�ִ���
	 *bstrParentID = strParentID.AllocSysString();

	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataItem::Set_ItemParentID(BSTR bstrParentID)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: �ڴ����ʵ�ִ���
	 if (bstrParentID != NULL)
	 {
		 strParentID = bstrParentID;
	 }

	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataItem::Get_ItemLevel(SHORT* snLevel)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: �ڴ����ʵ�ִ���
	 *snLevel = (SHORT)nLevel;

	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataItem::Set_ItemLevel(SHORT snLevel)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: �ڴ����ʵ�ִ���
	 if (snLevel >=0)
	 {
		 nLevel = (int)snLevel;
	 }

	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataItem::InsertTreeItem(IUnknown* pParentItem, IUnknown* DataMap)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());
	 CComQIPtr<ILPXZQDataItem> pParentDataItem = (CComQIPtr<ILPXZQDataItem>)pParentItem;
	 BSTR bstrParentID;
	 pParentDataItem->Get_ItemCode(&bstrParentID);
	 IUnknown* pTmpItemEnum;
	 pParentDataItem->Get_ChildItems(&pTmpItemEnum);
	 CComQIPtr<ILPXZQDataEnum> spDataEnum = (CComQIPtr<ILPXZQDataEnum>)pTmpItemEnum;

	 // TODO: �ڴ����ʵ�ִ���
	 CString strDM;
	 CString strMC;
	 int ntmpLevel = 0;
	 CString strParent = _T("");

	 CMapStringToString *mapXZQData;
	 mapXZQData = (CMapStringToString*)DataMap;

	 POSITION pItem = mapXZQData->GetStartPosition();
	 while(pItem != NULL)
	 {
		 mapXZQData->GetNextAssoc(pItem,strDM,strMC);
		 GetLevelandParent(strDM,strMC,&ntmpLevel,&strParent);
		 if (strParent.Compare(bstrParentID) == 0)
		 {
			 CComQIPtr<ILPXZQDataItem> spTreeItem;
			 spTreeItem.CoCreateInstance(CLSID_LPXZQDataItem);
			 spTreeItem->Set_ItemName(strMC.AllocSysString());
			 spTreeItem->Set_ItemCode(strDM.AllocSysString());
			 spTreeItem->Set_ItemParentID(strParent.AllocSysString());
			 spTreeItem->Set_ItemLevel((SHORT)ntmpLevel);
			 ((IUnknown*)spTreeItem)->AddRef();
			 spDataEnum->InsertItem((IUnknown*)spTreeItem,(IUnknown*)DataMap);

// 			 if (nLevel < DISTRICT_TREE_CUN_ICON)
// 			 {InsertTreeItem(spTreeItem,DataMap);
// 			 }
		 }
	 }
	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataItem::Get_ChildItems(IUnknown** ChildItemEnum)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: �ڴ����ʵ�ִ���
	 if (m_ChildItemsEnum == NULL)
	 {
		 m_ChildItemsEnum.CoCreateInstance(CLSID_LPXZQDataEnum);
	 }
	 *ChildItemEnum = (IUnknown*)m_ChildItemsEnum;

	 return S_OK;
 }


//edit by lcc 2016.12.14 �޸Ĳ���ڵ�ķ�ʽ
 STDMETHODIMP CLPXZQDataItem::InsertTreeItem2(IUnknown* pNewitem)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: �ڴ����ʵ�ִ���

	 m_ChildItemsEnum->InsertItem2(pNewitem);
	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataEnum::InsertItem2(IUnknown* pNewItem)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: �ڴ����ʵ�ִ���
	 CComQIPtr<ILPXZQDataItem> pItem = (CComQIPtr<ILPXZQDataItem>)pNewItem;
	 if (pNewItem == NULL) return S_OK;

	 //�жϽڵ������������λ��
	 //������Ǵ弶��ֱ�Ӳ�����Ϊsql���Ѿ����й�������
	 //����Ǵ弶����һ������Ҫ�ų����粢������

	 //������Ҫ��ȡ�弶����ǰ9λ���������Ƚ�
	 //�����������벻ͬ��ֱ�Ӳ��뵽ĩβ
	 //�����ͬ�������ı��Ƚϵõ���һ������ͬ��λ�ã����뵽ǰ��
	 CComBSTR bstrCode;
	 pItem->Get_ItemCode(&bstrCode);
	 CComBSTR bstrParentCode;
	 pItem->Get_ItemParentID(&bstrParentCode);
	 //��ȡ����������ǰ9λ
	 CString strCodeLeft9(bstrCode);
	 strCodeLeft9 = strCodeLeft9.Left(9);
	 CComBSTR bstrCodeLeft9;
	 bstrCodeLeft9 = strCodeLeft9;

	 if (bstrCodeLeft9 == _T("331102206"))
	 {
		 int n = 1;
	 }
	 if (bstrCode.Length() != 12)//���Ǵ弶,ֱ�Ӳ���
	 {
		 ChildItemsArray.Add((CComQIPtr<ILPXZQDataItem>)pNewItem);
	 }else if (bstrParentCode != bstrCodeLeft9)//ǰ9λ��������벻ͬ
	 {
		 ChildItemsArray.Add((CComQIPtr<ILPXZQDataItem>)pNewItem);
	 }else if (bstrParentCode == bstrCodeLeft9)//ǰ9λ�����������ͬ
	 {
		 CComBSTR bstrNewItemCode;
		 CComBSTR bstrTmpCode;
		 pItem->Get_ItemCode(&bstrNewItemCode);
		 int nCount = ChildItemsArray.GetCount();
		 //���û�ӽڵ㣬ֱ�Ӳ���
		 if (nCount == 0)
		 {
			 ChildItemsArray.Add((CComQIPtr<ILPXZQDataItem>)pNewItem);
		 }else
		 {
			 //��ȷ�����һ���ӽڵ��Ƿ���ԭ����Ĵ壬����ǣ�ֱ�Ӽ�
			 CComQIPtr<ILPXZQDataItem> pTmpItem = ChildItemsArray.GetAt(nCount-1);
			 pTmpItem->Get_ItemCode(&bstrTmpCode);
			 CString strTmpCodeLeft9;
			 CComBSTR bstrTmpCodeLeft9;
			 strTmpCodeLeft9 = bstrTmpCode;
			 strTmpCodeLeft9 = strTmpCodeLeft9.Left(9);
			 bstrTmpCodeLeft9 = strTmpCodeLeft9;
			 if (strTmpCodeLeft9 == bstrParentCode)
			 {
				 ChildItemsArray.Add((CComQIPtr<ILPXZQDataItem>)pNewItem);

			 }else
			 {
				 //ѭ���������ҵ���һ��ǰ9λ��������벻ͬ�ĵ������ǰ��
				 for (int i = 0;i < nCount; i++)
				 {
					 CComQIPtr<ILPXZQDataItem> pTmpItem = ChildItemsArray.GetAt(i);
					 pTmpItem->Get_ItemCode(&bstrTmpCode);
					 CString strTmpCodeLeft9;
					 CComBSTR bstrTmpCodeLeft9;
					 strTmpCodeLeft9 = bstrTmpCode;
					 strTmpCodeLeft9 = strTmpCodeLeft9.Left(9);
					 bstrTmpCodeLeft9 = strTmpCodeLeft9;
					 if (strTmpCodeLeft9 != bstrParentCode)
					 {
						 ChildItemsArray.InsertAt(i,pItem);
						 break;
					 }
				 }
			 }
		 }
		
	 }

	 return S_OK;
 }
//edit by lcc 2016.12.14 end

 HRESULT CLPXZQDataTree::getConnString(CComBSTR &bstrConnSet,CComBSTR &bstrName,CComBSTR &bstrPasswd)
 {
	 TiXmlDocument XmlDoc;
	 CString strIP;
	 CString strSID;
	 CString strUser;
	 CString strPassword;
	 CString strConnSet;
	 USES_CONVERSION;
	 //edit by lcc 2016.12.06 ���Ļ�ȡxml·���ķ�ʽ
	 TCHAR exeFullPath[MAX_PATH]; 
	 GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	 CString strExePath=exeFullPath;
	 int index =strExePath.ReverseFind('\\');
	 CString strPath = strExePath.Left(index)+CONNECT_CONFIGPATH;
	 //edit by lcc 2016.12.06 end
	 if (XmlDoc.LoadFile(W2A(_bstr_t(strPath))) == false)
	 {
		 //MessageBox(_T("�޷��������ļ�"),_T("����"),MB_OK);
		 return E_FAIL;
	 }
	 //TiXmlElement *pSdeSet = XmlDoc.FirstChildElement("Config")->FirstChildElement("SystemConfig")->FirstChildElement("DbConnections")->FirstChildElement();
	 //��һ���Ƕ�ȡ�����������
	 TiXmlElement *pSdeSetName = XmlDoc.FirstChildElement("Config")->FirstChildElement("RuConfig")->FirstChildElement("ConnectName");
	 CString strSDESetName = A2W(pSdeSetName->GetText());
	 //�ڶ����Ǹ��������������ȥ�Ҷ�Ӧ����Ŀ
	 TiXmlElement *pDbConnections = XmlDoc.FirstChildElement("Config")->FirstChildElement("SystemConfig")->FirstChildElement("DbConnections");
	 TiXmlElement *pSdeSet = pDbConnections->FirstChildElement();
	 while(pSdeSet != NULL)
	 {
		 CString strSetName = A2W(pSdeSet->Attribute("Name"));
		 if (strSDESetName.Compare(strSetName) == 0)
		 {
			 break;
		 }
		 pSdeSet = pSdeSet->NextSiblingElement();
	 }
	 if (pSdeSet != NULL)
	 {
		 strIP = pSdeSet->Attribute("Server");
		 strUser = pSdeSet->Attribute("UserID");
		 strPassword = pSdeSet->Attribute("Password");
		 strSID = pSdeSet->Attribute("SID");
	 }
// 	 TiXmlElement *pSdeSet = XmlDoc.FirstChildElement("connect")->FirstChildElement("gh");
// 	 if (pSdeSet != NULL)
// 	 {
// 		 TiXmlElement *pElement = pSdeSet->FirstChildElement("ip");
// 		 if (pElement != NULL)
// 		 {
// 			 strIP = pElement->FirstAttribute()->Value();
// 		 }
// 		 pElement = NULL;
// 		 pElement = pSdeSet->FirstChildElement("port");
// 		 if (pElement != NULL)
// 		 {
// 			 strPort = pElement->FirstAttribute()->Value();
// 		 }
// 		 pElement = NULL;
// 		 pElement = pSdeSet->FirstChildElement("user");
// 		 if (pElement != NULL)
// 		 {
// 			 strUser = pElement->FirstAttribute()->Value();
// 		 }
// 		 pElement = NULL;
// 		 pElement = pSdeSet->FirstChildElement("passwd");
// 		 if (pElement != NULL)
// 		 {
// 			 strPassword = pElement->FirstAttribute()->Value();
// 		 }
// 	 }
	 strConnSet.Format(_T("Provider=OraOLEDB.Oracle.1;User ID=%s;Password=%s;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=%s)(PORT=%s))(CONNECT_DATA=(SERVICE_NAME=%s)));Persist Security Info=False"),
		 strUser,strPassword,strIP,nPort,strSID);
	 bstrConnSet = (LPCTSTR)strConnSet;
	 bstrName = (LPCTSTR)strUser;
	 bstrPasswd = (LPCTSTR)strPassword;
	 return S_OK;
 }

 HRESULT CLPXZQDataTree::ConnToOracle(_ConnectionPtr ipADOConn)
 {
	 CComBSTR bstrConnstr;
	 CComBSTR bstrName;
	 CComBSTR bstrPasswd;
	 getConnString(bstrConnstr,bstrName,bstrPasswd);
	 if (ipADOConn == NULL)
	 {
		 ipADOConn.CreateInstance(__uuidof(Connection));
	 }
	 HRESULT hr = ipADOConn->Open(_bstr_t(bstrConnstr),_bstr_t(bstrName),_bstr_t(bstrPasswd) , adConnectUnspecified);
	//BCGPMessageBox(bstrConnstr,_T(""),MB_OK);
	CString strhr;
	strhr.Format(_T("%x"),hr);
	//BCGPMessageBox(strhr,_T(""),MB_OK);
	 if (hr != S_OK)
	 {
		 return hr;
	 }
	 return S_OK;
 }