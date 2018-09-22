#include "stdafx.h"
#include "LPXZQDataTree.h"


void GetLevelandParent(CString strDM, CString strMC, int *nLevel, CString *pstrParent)
{
	if (strDM.GetLength() == 2)//省级
	{
		*nLevel = DISTRICT_TREE_SHENG_ICON;
		*pstrParent = _T("");
	} else if(strDM.GetLength() == 4)//市级
	{
		*nLevel = DISTRICT_TREE_SHI_ICON;
		*pstrParent = strDM.Left(2);
	} else if(strDM.GetLength() == 6)//县区
	{
		*nLevel = DISTRICT_TREE_XIAN_ICON;
		*pstrParent = strDM.Left(4);
	} else if(strDM.GetLength() == 9)//乡镇
	{

		*nLevel = DISTRICT_TREE_XIANG_ICON;
		*pstrParent = strDM.Left(6);
	} else if(strDM.GetLength() == 12)//村
	{
		*nLevel = DISTRICT_TREE_CUN_ICON;
		*pstrParent = strDM.Left(9);
	}
}

//行政区数据结构
CComQIPtr<ILPXZQDataItem> CLPXZQDataTree::XZQTreeRoot = (IUnknown*)NULL;
BOOL CLPXZQDataTree::bIsXZQInit = FALSE;
CString CLPXZQDataTree::m_strParentID = _T("1234567890123");//给一个高于12位的字符串用于获取最高级别行政区
CMapStringToString *CLPXZQDataTree::m_mapXZQ = new CMapStringToString;


// LPXZQDataTree.cpp : CLPXZQDataTree 的实现

HRESULT CLPXZQDataTree::InitXZQTree(CComQIPtr<IFeatureWorkspace> spFtWorkspace)
 {
 	//如果获取成功初始化树
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
 			//插入到数组
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
 	//遍历map添加到tree
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
 		XZQTreeRoot->InsertTreeItem(XZQTreeRoot,(IUnknown*)m_mapXZQ);//从根开始递归的插入整个树
 		return S_OK;
 	}
 	return S_OK;
 }

 STDMETHODIMP CLPXZQDataTree::GetXZQMap(IUnknown** mapXZQData)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: 在此添加实现代码
	 *mapXZQData = (IUnknown*) m_mapXZQ;
	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataTree::GetRootItem(IUnknown* pFtWorkspace, IUnknown** RootItem)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());
	 CComQIPtr<IFeatureWorkspace> spFtWorkSpace = (CComQIPtr<IFeatureWorkspace>)pFtWorkspace;
	 // TODO: 在此添加实现代码
	 if (bIsXZQInit == FALSE)
	 {
		 InitXZQTree2(spFtWorkSpace);
	 }
	 *RootItem = XZQTreeRoot;

	 return S_OK;
 }
//edit by lcc 2016.12.14 更改行政区树读取方式为ADO接口
 HRESULT CLPXZQDataTree::InitXZQTree2(CComQIPtr<IFeatureWorkspace> spFtWorkspace)
 {
	 //如果获取成功初始化树
	//创建对象
	// BCGPMessageBox(_T("开始读取行政区树"),_T(""),MB_OK);
	 HRESULT hr;
	 _variant_t  vt;
	_ConnectionPtr ipADOConn;
	_RecordsetPtr pRst; 
	hr = ipADOConn.CreateInstance(__uuidof(Connection));
	hr = pRst.CreateInstance(__uuidof(Recordset));
	//创建连接
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
	//创建行政区view
	CComQIPtr<IWorkspace2> spWorkspace2;
	spWorkspace2 = spFtWorkspace;
	if (spWorkspace2 == NULL) return E_FAIL;
	VARIANT_BOOL bIsProvince = VARIANT_FALSE;
	VARIANT_BOOL bIsCity = VARIANT_FALSE;
	VARIANT_BOOL bIsCounty = VARIANT_FALSE;
	//检查各级别行政区表是否存在
	spWorkspace2->get_NameExists(esriDTFeatureClass,_bstr_t("XZQZ"),&bIsProvince);
	spWorkspace2->get_NameExists(esriDTFeatureClass,_bstr_t("XZQS"),&bIsCity);
	spWorkspace2->get_NameExists(esriDTFeatureClass,_bstr_t("XZQX"),&bIsCounty);
	//检查各级别行政区表是否为空
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

	//edit by lcc 2016.12.17 修改了错误的视图创建SQL语句
	CString strCreateView = _T("");
	strCreateView += _T("create or replace view xzq_view as \r\n");
	strCreateView += _T("select t.xzdm ||'-'||t.xzqdm as 乡镇及村代码,\r\n ");
	strCreateView += _T("t.xzqdm as 村级代码,\r\n");
	strCreateView += _T("t.xzqmc as 村名,\r\n");
	strCreateView += _T("s.xzqdm as 乡镇代码,\r\n");
	strCreateView += _T("s.xzqmc as 乡镇名称,\r\n");
	strCreateView += _T("s.zxcq as 是否中心城区,\r\n");
	strCreateView += _T("w.xzqdm as 县级代码,\r\n");
	strCreateView += _T("w.xzqmc as 县名\r\n");
	if (bIsCity == VARIANT_FALSE)//如果市级不存在
	{
		strCreateView += _T("from xzq t, xzqxz s, xzqx w\r\n");
		strCreateView += _T("where t.xzdm = s.xzqdm and substr(s.xzqdm, 0, 6） = w.xzqdm");
	}else if (bIsProvince == VARIANT_FALSE)//如果省级不存在
	{
		strCreateView += _T(",");
		strCreateView += _T("x.xzqdm as 市级代码,\r\n");
		strCreateView += _T("x.xzqmc as 市名\r\n");
		strCreateView += _T("from xzq t, xzqxz s, xzqx w, xzqs x\r\n");
		strCreateView += _T("where t.xzdm = s.xzqdm and substr(s.xzqdm, 0, 6） = w.xzqdm and substr(s.xzqdm, 0, 4） = x.xzqdm");
	}else//如果省存在
	{
		strCreateView += _T(",");
		strCreateView += _T("x.xzqdm as 市级代码,\r\n");
		strCreateView += _T("x.xzqmc as 市名,\r\n");
		strCreateView += _T("y.xzqdm as 省级代码,\r\n");
		strCreateView += _T("y.xzqmc as 省名 \r\n");
		strCreateView += _T("from xzq t, xzqxz s, xzqx w, xzqs x, xzqz y \r\n");
		strCreateView += _T("where t.xzdm = s.xzqdm and substr(s.xzqdm, 0, 6） = w.xzqdm and substr(s.xzqdm, 0, 4） = x.xzqdm and substr(s.xzqdm, 0, 2） = y.xzqdm");
	}
	//edit by lcc 2016.12.17 end


	hr = ipADOConn->Execute(_bstr_t(strCreateView.AllocSysString()),&vt,adCmdText,&pRst);
	if (hr != S_OK)
	{
		return hr;
	}
	
	//读取行政区view表
	CString strSQL = _T("select * from XZQ_VIEW order by 乡镇及村代码");
	hr = pRst->Open(_variant_t(_bstr_t(strSQL.AllocSysString())),_variant_t(ipADOConn.GetInterfacePtr()), adOpenStatic,adLockOptimistic,adCmdText);
	//获取所有信息
	long nCount = 0;
	pRst->get_RecordCount(&nCount);
	hr = pRst->GetRows(nCount,vtMissing,vtMissing,&vt);
	if (!SUCCEEDED(hr)) return E_FAIL;
	//获取数据指针和列数
	void *pvd = NULL;
	SafeArrayAccessData(vt.parray,&pvd);
	LONG nFieldNum = 0;
	SafeArrayGetUBound(vt.parray,(UINT)1,&nFieldNum);
	//根据列数确定根节点级别

	//遍历所有数据
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
	BOOL bIsXZQSame = FALSE;//本级是否相同

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
			case 0://乡镇及村代码
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
			case 1://村级代码
 				if (CUNDataItem != NULL)
 				{//如果代码和上一行相同,跳过这一行
 					//CUNDataItem->Get_ItemCode(&bstrXZQDM);
 					//strXZQDM.Format(_T("%s"),bstrXZQDM);
 					if (strLastXZCUN.Compare(strXZCUN) == 0)//这里改为乡镇代码和村级行政区代码联合判断，不相等则继续
 					{
 						bIsXZQSame = TRUE;
 						break;
 					}else
 					{//不相同则释放上一个指针,开始创建下一个
						strLastXZCUN = strXZCUN;
 						CUNDataItem.Release();
 					}
 				}
 				CUNDataItem.CoCreateInstance(CLSID_LPXZQDataItem);
 				CUNDataItem->Set_ItemCode(vtTmp.bstrVal);
 				CUNDataItem->Set_ItemLevel(DISTRICT_TREE_CUN_ICON);
 				strXZQCUNDM.Format(_T("%s"),vtTmp.bstrVal);
 				
				break;
			case 2://村名
				CUNDataItem->Set_ItemName(vtTmp.bstrVal);
				strXZQMC.Format(_T("%s"),vtTmp.bstrVal);
				break;
			case 3://乡镇代码
				if (XZDataItem != NULL)
				{//如果代码和上一行相同
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
						bIsXZQSame = TRUE;//跳过这一行剩下部分的遍历
						break;
					}else
					{//不相同则释放上一个指针,开始创建下一个
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
			case 4://乡镇名称
				strXZQXZMC.Format(_T("%s"),vtTmp.bstrVal);
				XZDataItem->Set_ItemName(vtTmp.bstrVal);
				break;
			case 5://是否中心城区
				nZXCQ = vtTmp.iVal;
				break;
			case 6://县级代码
				if (nField == nFieldNum - 1 && nIndex == 0)//如果这是顶级行政区,且当前是第一行
				{
					XZQTreeRoot.CoCreateInstance(CLSID_LPXZQDataItem);
					XIANDataItem = XZQTreeRoot;
				}else if (nField == nFieldNum - 1 && nIndex != 0)//如果这是顶级行政区,且不是第一行,跳过
				{
					bIsXZQSame = TRUE;
				}else if (XIANDataItem != NULL)//不是顶级行政
				{
					//如果代码和上一行相同,跳过这一行
					XIANDataItem->Get_ItemCode(&bstrXZQDM);
					strXZQDM.Format(_T("%s"),bstrXZQDM);
					if (strXZQDM.Compare(vtTmp.bstrVal) == 0)
					{
						bIsXZQSame = TRUE;
					}else
					{//不相同则释放上一个指针,开始创建下一个
						XIANDataItem.Release();
						XIANDataItem.CoCreateInstance(CLSID_LPXZQDataItem);
					}
				}else if (XIANDataItem == NULL)
				{
					XIANDataItem.CoCreateInstance(CLSID_LPXZQDataItem);
				}

				if (nZXCQ != 0)//如果是中心城区
				{
					if (ZXCQDataItem != NULL)
					{
						//如果代码和上一行相同,跳过这一行
						ZXCQDataItem->Get_ItemCode(&bstrXZQDM);
						strXZQDM.Format(_T("%s"),bstrXZQDM);
						strXZQDM = strXZQDM.Left(strXZQDM.GetLength() - 3);
						if (strXZQDM.Compare(vtTmp.bstrVal) == 0)
						{
							//追加乡镇行政区代码
							BSTR bstrTmpDM;
							BSTR bstrTmpXZDM;
							CString strTmpDM;
							ZXCQDataItem->Get_ItemParentID(&bstrTmpDM);
							XZDataItem->Get_ItemCode(&bstrTmpXZDM);
							strTmpDM.Format(_T("%s,%s"),bstrTmpDM,bstrTmpXZDM);
							ZXCQDataItem->Set_ItemParentID(_bstr_t(strTmpDM));

							//插入乡镇行政区节点
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
							ZXCQDataItem->Set_ItemName(_T("中心城区"));
							ZXCQDataItem->Set_ItemParentID(vtTmp.bstrVal);

							XIANDataItem->InsertTreeItem2(ZXCQDataItem);
							//追加乡镇行政区代码
							BSTR bstrTmpXZDM;
							XZDataItem->Get_ItemCode(&bstrTmpXZDM);
							ZXCQDataItem->Set_ItemParentID(bstrTmpXZDM);
							//插入乡镇行政区节点
							ZXCQDataItem->InsertTreeItem2(XZDataItem);
							m_mapXZQ->SetAt(strXZQXZDM,strXZQXZMC);
						}
					}else
					{
						ZXCQDataItem.CoCreateInstance(CLSID_LPXZQDataItem);
						strXZQDM.Format(_T("%s000"),vtTmp.bstrVal);
						ZXCQDataItem->Set_ItemLevel(DISTRICT_TREE_XIANG_ICON);
						ZXCQDataItem->Set_ItemCode(_bstr_t(strXZQDM));
						ZXCQDataItem->Set_ItemName(_T("中心城区"));
						ZXCQDataItem->Set_ItemParentID(vtTmp.bstrVal);

						XIANDataItem->InsertTreeItem2(ZXCQDataItem);
						//追加乡镇行政区代码
						BSTR bstrTmpXZDM;
						XZDataItem->Get_ItemCode(&bstrTmpXZDM);
						ZXCQDataItem->Set_ItemParentID(bstrTmpXZDM);

						//插入乡镇行政区节点
						ZXCQDataItem->InsertTreeItem2(XZDataItem);
						m_mapXZQ->SetAt(strXZQXZDM,strXZQXZMC);
					}
				}else//如果不是中心城区
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
			case 7://县名
				XIANDataItem->Set_ItemName(vtTmp.bstrVal);
				strXZQXMC.Format(_T("%s"),vtTmp.bstrVal);
				break;
			case 8://市级代码
				if (nField == nFieldNum - 1 && nIndex == 0)//如果这是顶级行政区,且当前是第一行
				{
					XZQTreeRoot.CoCreateInstance(CLSID_LPXZQDataItem);
					SHIDataItem = XZQTreeRoot;
				}else if (nField == nFieldNum - 1 && nIndex != 0)//如果这是顶级行政区,且不是第一行,跳过
				{
					if (XIANDataItem != NULL)
					{
						SHIDataItem->InsertTreeItem2(XIANDataItem);
						m_mapXZQ->SetAt(strXZQXDM,strXZQXMC);
					}
					bIsXZQSame = TRUE;
					break;
				}else if (SHIDataItem != NULL)//不是顶级行政
				{
					//如果代码和上一行相同,跳过这一行
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
					{//不相同则释放上一个指针,开始创建下一个
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
			case 9://市名
				SHIDataItem->Set_ItemName(vtTmp.bstrVal);
				strXZQSMC.Format(_T("%s"),vtTmp.bstrVal);
				break;
			case 10://省级代码
				if (nIndex == 0)//如果当前是第一行(如果有省一定是顶级),创建自己,插入市节点
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
				}else//如果不是第一行,插入市节点,跳过
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
			case 11://省名
				//需要跳过的情况在上一个字段已经处理了
				XZQTreeRoot->Set_ItemName(vtTmp.bstrVal);
				m_mapXZQ->SetAt(strXZQDM,vtTmp.bstrVal);
				break;
			default:
				break;
			}
			if (bIsXZQSame)//如果本级相同,跳过这一行的遍历
			{
				bIsXZQSame = FALSE;
				break;
			}

		}
	}

	//顶级行政区不是省级的情况直接在这里处理一次会比较简单
	//将根节点的行政区代码和名称添加到map
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
  // LPXZQDataTree.cpp : CLPXZQDataEnum 的实现

 STDMETHODIMP CLPXZQDataEnum::InsertItem(IUnknown* pNewItem, IUnknown* DataMap)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: 在此添加实现代码
	 CComQIPtr<ILPXZQDataItem> pItem = (CComQIPtr<ILPXZQDataItem>)pNewItem;
	 if (pNewItem == NULL) return S_OK;
	 int nCount = ChildItemsArray.GetCount();
	 if (nCount == 0)//如果没有字节点直接插入
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
		//先判断与结尾的节点比较大小
		 CComQIPtr<ILPXZQDataItem> pTmpItem = ChildItemsArray.GetAt(nCount-1);
		 pTmpItem->Get_ItemCode(&bstrTmpCode);
		 strTmpCode = bstrTmpCode;
		 if (strTmpCode < strNewItemCode)//如果比结尾的大，插入到结尾
		 {
			 ChildItemsArray.Add(pItem);
			 pItem->InsertTreeItem(pItem,DataMap);
			 return S_OK;
		 }else
		 {
			 //遍历所有节点，找到第一个比自己行政区号大的位置插入节点
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

	 // TODO: 在此添加实现代码
	 *nCount = ChildItemsArray.GetCount();

	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataEnum::GetItemAt(int nItem,IUnknown** pItem)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: 在此添加实现代码
	 *pItem = (IUnknown*)ChildItemsArray.GetAt(nItem);

	 return S_OK;
 }

 // LPXZQDataTree.cpp : CLPXZQDataItem 的实现

 STDMETHODIMP CLPXZQDataItem::Get_ItemName(BSTR* bstrName)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: 在此添加实现代码
	 *bstrName = strName.AllocSysString();

	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataItem::Set_ItemName(BSTR bstrName)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: 在此添加实现代码
	 if (bstrName != NULL)
	 {
		 strName = bstrName;
	 }
	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataItem::Get_ItemCode(BSTR* bstrCode)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: 在此添加实现代码
	 *bstrCode = strCode.AllocSysString();

	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataItem::Set_ItemCode(BSTR bstrCode)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: 在此添加实现代码
	 if (bstrCode != NULL)
	 {
		 strCode = bstrCode;
	 }
	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataItem::Get_ItemParentID(BSTR* bstrParentID)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: 在此添加实现代码
	 *bstrParentID = strParentID.AllocSysString();

	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataItem::Set_ItemParentID(BSTR bstrParentID)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: 在此添加实现代码
	 if (bstrParentID != NULL)
	 {
		 strParentID = bstrParentID;
	 }

	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataItem::Get_ItemLevel(SHORT* snLevel)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: 在此添加实现代码
	 *snLevel = (SHORT)nLevel;

	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataItem::Set_ItemLevel(SHORT snLevel)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: 在此添加实现代码
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

	 // TODO: 在此添加实现代码
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

	 // TODO: 在此添加实现代码
	 if (m_ChildItemsEnum == NULL)
	 {
		 m_ChildItemsEnum.CoCreateInstance(CLSID_LPXZQDataEnum);
	 }
	 *ChildItemEnum = (IUnknown*)m_ChildItemsEnum;

	 return S_OK;
 }


//edit by lcc 2016.12.14 修改插入节点的方式
 STDMETHODIMP CLPXZQDataItem::InsertTreeItem2(IUnknown* pNewitem)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: 在此添加实现代码

	 m_ChildItemsEnum->InsertItem2(pNewitem);
	 return S_OK;
 }

 STDMETHODIMP CLPXZQDataEnum::InsertItem2(IUnknown* pNewItem)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: 在此添加实现代码
	 CComQIPtr<ILPXZQDataItem> pItem = (CComQIPtr<ILPXZQDataItem>)pNewItem;
	 if (pNewItem == NULL) return S_OK;

	 //判断节点的行政区代码位数
	 //如果不是村级，直接插入因为sql里已经进行过排序了
	 //如果是村级，做一遍排序，要排除撤乡并镇的情况

	 //这里需要截取村级代码前9位与乡镇代码比较
	 //如果与乡镇代码不同，直接插入到末尾
	 //如果相同，根据文本比较得到第一个不相同的位置，插入到前面
	 CComBSTR bstrCode;
	 pItem->Get_ItemCode(&bstrCode);
	 CComBSTR bstrParentCode;
	 pItem->Get_ItemParentID(&bstrParentCode);
	 //获取行政区代码前9位
	 CString strCodeLeft9(bstrCode);
	 strCodeLeft9 = strCodeLeft9.Left(9);
	 CComBSTR bstrCodeLeft9;
	 bstrCodeLeft9 = strCodeLeft9;

	 if (bstrCodeLeft9 == _T("331102206"))
	 {
		 int n = 1;
	 }
	 if (bstrCode.Length() != 12)//不是村级,直接插入
	 {
		 ChildItemsArray.Add((CComQIPtr<ILPXZQDataItem>)pNewItem);
	 }else if (bstrParentCode != bstrCodeLeft9)//前9位与乡镇代码不同
	 {
		 ChildItemsArray.Add((CComQIPtr<ILPXZQDataItem>)pNewItem);
	 }else if (bstrParentCode == bstrCodeLeft9)//前9位与乡镇代码相同
	 {
		 CComBSTR bstrNewItemCode;
		 CComBSTR bstrTmpCode;
		 pItem->Get_ItemCode(&bstrNewItemCode);
		 int nCount = ChildItemsArray.GetCount();
		 //如果没子节点，直接插入
		 if (nCount == 0)
		 {
			 ChildItemsArray.Add((CComQIPtr<ILPXZQDataItem>)pNewItem);
		 }else
		 {
			 //先确定最后一个子节点是否是原乡镇的村，如果是，直接加
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
				 //循环遍历，找到第一个前9位与乡镇代码不同的点插入在前面
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
	 //edit by lcc 2016.12.06 更改获取xml路径的方式
	 TCHAR exeFullPath[MAX_PATH]; 
	 GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	 CString strExePath=exeFullPath;
	 int index =strExePath.ReverseFind('\\');
	 CString strPath = strExePath.Left(index)+CONNECT_CONFIGPATH;
	 //edit by lcc 2016.12.06 end
	 if (XmlDoc.LoadFile(W2A(_bstr_t(strPath))) == false)
	 {
		 //MessageBox(_T("无法打开配置文件"),_T("错误"),MB_OK);
		 return E_FAIL;
	 }
	 //TiXmlElement *pSdeSet = XmlDoc.FirstChildElement("Config")->FirstChildElement("SystemConfig")->FirstChildElement("DbConnections")->FirstChildElement();
	 //第一步是读取配置项的名称
	 TiXmlElement *pSdeSetName = XmlDoc.FirstChildElement("Config")->FirstChildElement("RuConfig")->FirstChildElement("ConnectName");
	 CString strSDESetName = A2W(pSdeSetName->GetText());
	 //第二步是根据配置项的名称去找对应的条目
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