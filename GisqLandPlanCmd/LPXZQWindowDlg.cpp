// LPXZQWindowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LPXZQWindowDlg.h"

#include "LPXZQWindowCmd.h"
// CLPXZQWindowDlg 对话框

IMPLEMENT_DYNAMIC(CLPXZQWindowDlg, CGSBCGDialog)

CLPXZQWindowDlg::CLPXZQWindowDlg(_DGisqFrameworkCOMPtr	ipFrameworkCom, CWnd* pParent /*=NULL*/)
	: CGSBCGDialog(CLPXZQWindowDlg::IDD, pParent)
{
	m_pFrameworkCom = ipFrameworkCom;
	m_parrDistrict = new CArray<_tagDistrictItem *,_tagDistrictItem *>;
	m_ipFtWorkSpace = NULL;
	//add by lcc 2016.12.23
	m_pParentCOM = NULL;
	m_pmapXZQ = NULL;
}

CLPXZQWindowDlg::~CLPXZQWindowDlg()
{
	m_parrDistrict->RemoveAll();
	delete m_parrDistrict;
}

void CLPXZQWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CGSBCGDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_XZQ_TREE, m_wndXZQTree);
}


BEGIN_MESSAGE_MAP(CLPXZQWindowDlg, CGSBCGDialog)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_XZQ_TREE, &CLPXZQWindowDlg::OnNMDblclkXzqTree)
	ON_NOTIFY(NM_CLICK, IDC_XZQ_TREE, &CLPXZQWindowDlg::OnNMClickXzqTree)
END_MESSAGE_MAP()


// CLPXZQWindowDlg 消息处理程序

void CLPXZQWindowDlg::OnSize(UINT nType, int cx, int cy)
{
	CGSBCGDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect DlgRect;
	GetClientRect(&DlgRect);
	if (DlgRect.Width() == 0 || DlgRect.Height() == 0) return;
	if (m_wndXZQTree.GetSafeHwnd() != NULL)
	{
		m_wndXZQTree.MoveWindow(DlgRect);
	}

}

BOOL CLPXZQWindowDlg::OnInitDialog()
{
	CGSBCGDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//获取规划工作空间
	CComQIPtr<GisqLib::IGSDataExchangeS> spGSDataExchangeS = m_pFrameworkCom->GetDataExchangeS();
	BSTR bstrTag(_T("GHWS"));
	short index = spGSDataExchangeS->FindIndex(bstrTag);
	if (index >= 0)
	{
		CComQIPtr<GisqLib::IGSDataExchange> spGSDataExchange = spGSDataExchangeS->Item(index);
		CComVariant var = spGSDataExchange->Item(0);
		IWorkspacePtr spWorkspace = var.punkVal;
		m_ipFtWorkSpace = spWorkspace;
		//TODO…
	}
	//如果获取成功初始化树
	if (m_ipFtWorkSpace != NULL)
	{
		//InitXzqTree(m_ipFtWorkSpace);
		CComQIPtr<IFeatureWorkspace> ipFtwork = m_ipFtWorkSpace;
		InitXzqTree2(ipFtwork);
	}else
	{
		
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CLPXZQWindowDlg::InitXzqTree(IFeatureWorkspace *ipFtWork)
{
	m_imageList.Create(IDB_DISTRICT_ICON_DOCKWND,16,1,RGB(255,255,255));
	m_wndXZQTree.SetImageList(&m_imageList,TVSIL_NORMAL);
	//从工作空间读取数据
	GetXzqDm(ipFtWork);
	SortArray(0,m_parrDistrict->GetCount() - 1);
	AddDistrictTreeItem(NULL,m_strParent);
	HTREEITEM item = m_wndXZQTree.GetFirstVisibleItem();
	m_wndXZQTree.Expand(item,TVE_EXPAND);
	return TRUE;
}

BOOL CLPXZQWindowDlg::GetXzqDm(IFeatureWorkspace *ipFtWorkSpace)
{
	CMapStringToString mapTMP;//定义一个临时map用于查重时快速检索
	mapTMP.InitHashTable(601);
	//遍历表
	ITablePtr ipTable;
	ipFtWorkSpace->OpenTable(_T("T_XZQMC"),&ipTable);
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
			//插入到数组
			if (DMtmp.bstrVal != NULL && MCtmp.bstrVal != NULL)
			{
				AddToArray(DMtmp.bstrVal,MCtmp.bstrVal);
				mapTMP.SetAt(DMtmp.bstrVal,MCtmp.bstrVal);
			}

			ipCursor->NextRow(&ipRow);
		}		
	}

	//遍历要素集，补全表中没有的值，并建立一个只与map相关的值

	IFeatureClassPtr ipFtClass = NULL;
	ipFtWorkSpace->OpenFeatureClass(_T("XZQ"),&ipFtClass);
	if (ipFtClass != NULL)
	{
		//保存为全局，在查找时使用
		m_ipFtClass = ipFtClass;
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
					//如果表中没有，添加到数组
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
	//如果数据库中不存在表和要素集，返回空
	if (ipTable == NULL/* && ipFtClass == NULL*/)
	{
		return FALSE;
	}
	return TRUE;
};

void CLPXZQWindowDlg::AddToArray(BSTR bstrDM,BSTR bstrMC)
{
	CString strDM = bstrDM;
	CString strMC = bstrMC;
	if (strDM.GetLength() == 2)//省级
	{
		_tagDistrictItem *tmp = new _tagDistrictItem;
		tmp->nLevel = DISTRICT_TREE_SHENG_ICON;
		tmp->strCode = strDM;
		tmp->strName = strMC;
		tmp->strParentID = _T("");
		m_parrDistrict->Add(tmp);
		if (strDM.GetLength() < m_strParent.GetLength())
		{
			m_strParent = strDM;
		}

	} else if(strDM.GetLength() == 4)//市级
	{
		_tagDistrictItem *tmp = new _tagDistrictItem;
		tmp->nLevel = DISTRICT_TREE_SHI_ICON;
		tmp->strCode = strDM;
		tmp->strName = strMC;
		tmp->strParentID = strDM.Left(2);
		m_parrDistrict->Add(tmp);
		if (strDM.GetLength() < m_strParent.GetLength())
		{
			m_strParent = strDM;
		}

	} else if(strDM.GetLength() == 6)//县区
	{
		if (strMC.Find(_T("县"),0) > 0)//县
		{
			_tagDistrictItem *tmp = new _tagDistrictItem;
			tmp->nLevel = DISTRICT_TREE_XIAN_ICON;
			tmp->strCode = strDM;
			tmp->strName = strMC;
			tmp->strParentID = strDM.Left(4);
			m_parrDistrict->Add(tmp);
			if (strDM.GetLength() < m_strParent.GetLength())
			{
				m_strParent = strDM;
			}
		} 
		else//区
		{
			_tagDistrictItem *tmp = new _tagDistrictItem;
			tmp->nLevel = DISTRICT_TREE_QU_ICON;
			tmp->strCode = strDM;
			tmp->strName = strMC;
			tmp->strParentID = strDM.Left(4);
			tmp->strParentID = _T("");
			m_parrDistrict->Add(tmp);
			if (strDM.GetLength() < m_strParent.GetLength())
			{
				m_strParent = strDM;
			}
		}

	} else if(strDM.GetLength() == 9)//乡镇
	{
		if (strMC.Find(_T("镇"),0) > 0)//镇
		{
			_tagDistrictItem *tmp = new _tagDistrictItem;
			tmp->nLevel = DISTRICT_TREE_ZHEN_ICON;
			tmp->strCode = strDM;
			tmp->strName = strMC;
			tmp->strParentID = strDM.Left(6);
			m_parrDistrict->Add(tmp);
			if (strDM.GetLength() < m_strParent.GetLength())
			{
				m_strParent = strDM;
			}
		} 
		else//乡
		{
			_tagDistrictItem *tmp = new _tagDistrictItem;
			tmp->nLevel = DISTRICT_TREE_XIANG_ICON;
			tmp->strCode = strDM;
			tmp->strName = strMC;
			tmp->strParentID = strDM.Left(6);
			m_parrDistrict->Add(tmp);
			if (strDM.GetLength() < m_strParent.GetLength())
			{
				m_strParent = strDM;
			}
		}
	} else if(strDM.GetLength() == 12)//村
	{
		_tagDistrictItem *tmp = new _tagDistrictItem;
		tmp->nLevel = DISTRICT_TREE_CUN_ICON;
		tmp->strCode = strDM;
		tmp->strName = strMC;
		tmp->strParentID = strDM.Left(9);
		m_parrDistrict->Add(tmp);
		if (strDM.GetLength() < m_strParent.GetLength())
		{
			m_strParent = strDM;
		}
	}
}

void CLPXZQWindowDlg:: AddDistrictTreeItem(HTREEITEM parentItem/*=NULL*/,CString parentId/*=_T("")*/)
{
	//将数据中的数据添加到comboxTree中
	for ( int i=0;i<m_parrDistrict->GetCount();i++)
	{
		_tagDistrictItem* item = m_parrDistrict->GetAt(i);
		//省
		if (item->nLevel==DISTRICT_TREE_SHENG_ICON  && item->strParentID.Compare(parentId)==0)
		{   
			if (parentItem!=NULL)
				continue;
			CString showValue;
			showValue.Format(_T("[%s]%s"),item->strCode.Right(2),item->strName);
			HTREEITEM hItem = m_wndXZQTree.InsertItem(showValue,item->nLevel,item->nLevel,parentItem);
			m_wndXZQTree.SetItemData(hItem,(DWORD_PTR)item);
			AddDistrictTreeItem(hItem,item->strCode);
			
		}//市
		else if (item->nLevel==DISTRICT_TREE_SHI_ICON  && item->strParentID.Compare(parentId)==0)
		{
			if (item==NULL)
				continue;
			CString showValue;
			showValue.Format(_T("[%s]%s"),item->strCode.Right(2),item->strName);
			HTREEITEM hItem = m_wndXZQTree.InsertItem(showValue,item->nLevel,item->nLevel,parentItem);
			m_wndXZQTree.SetItemData(hItem,(DWORD_PTR)item);
			AddDistrictTreeItem(hItem,item->strCode);
		}//县区
		else if  (item->nLevel==DISTRICT_TREE_XIAN_ICON  && item->strParentID.Compare(parentId)==0)
		{
			if (item==NULL)
				continue;
			CString showValue;
			showValue.Format(_T("[%s]%s"),item->strCode.Right(2),item->strName);
			HTREEITEM hItem = m_wndXZQTree.InsertItem(showValue,item->nLevel,item->nLevel,parentItem);
			m_wndXZQTree.SetItemData(hItem,(DWORD_PTR)item);
			AddDistrictTreeItem(hItem,item->strCode);

		}else if  (item->nLevel==DISTRICT_TREE_QU_ICON  && item->strParentID.Compare(parentId)==0)
		{
			if (item==NULL)
				continue;
			CString showValue;
			showValue.Format(_T("[%s]%s"),item->strCode.Right(2),item->strName);
			HTREEITEM hItem = m_wndXZQTree.InsertItem(showValue,item->nLevel,item->nLevel,parentItem);
			m_wndXZQTree.SetItemData(hItem,(DWORD_PTR)item);
			AddDistrictTreeItem(hItem,item->strCode);
		}//乡镇
		else if (item->nLevel==DISTRICT_TREE_XIANG_ICON && item->strParentID.Compare(parentId)==0)
		{
			if (item==NULL)
				continue;
			CString showValue;
			showValue.Format(_T("[%s]%s"),item->strCode.Right(3),item->strName);
			HTREEITEM hItem = m_wndXZQTree.InsertItem(showValue,item->nLevel,item->nLevel,parentItem);
			m_wndXZQTree.SetItemData(hItem,(DWORD_PTR)item);
			AddDistrictTreeItem(hItem,item->strCode);

		}else if (item->nLevel==DISTRICT_TREE_ZHEN_ICON && item->strParentID.Compare(parentId)==0)
		{
			if (item==NULL)
				continue;
			CString showValue;
			showValue.Format(_T("[%s]%s"),item->strCode.Right(3),item->strName);
			HTREEITEM hItem = m_wndXZQTree.InsertItem(showValue,item->nLevel,item->nLevel,parentItem);
			m_wndXZQTree.SetItemData(hItem,(DWORD_PTR)item);
			AddDistrictTreeItem(hItem,item->strCode);
		} 
		//村
		else if (item->nLevel==DISTRICT_TREE_CUN_ICON && item->strParentID.Compare(parentId)==0)
		{
			if (item==NULL)
				continue;
			CString showValue;
			showValue.Format(_T("[%s]%s"),item->strCode.Right(3),item->strName);
			HTREEITEM hItem = m_wndXZQTree.InsertItem(showValue,item->nLevel,item->nLevel,parentItem);
			m_wndXZQTree.SetItemData(hItem,(DWORD_PTR)item);
			AddDistrictTreeItem(hItem,item->strCode);			
		} 

	}

}

void CLPXZQWindowDlg::OnNMDblclkXzqTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	//获取点击项目
	CPoint point;
	GetCursorPos(&point);
	m_wndXZQTree.ScreenToClient(&point);
	HTREEITEM item = m_wndXZQTree.HitTest(point);

	if (item != NULL)
	{
		//写入全局变量
		_tagDistrictItem *DisItem;
		DisItem = (_tagDistrictItem *)m_wndXZQTree.GetItemData(item);

		//change by lcc 2016-11-14 将全局变量由bstr变更为cstring
		m_strName = DisItem->strName;
		m_strCode = DisItem->strCode;
		//change end

		if (m_strName.GetLength() != 0)
		{//获取mapctrl
			short sType;
			IGSActiveViewPtr ipGSActiveView;
			IMapControl2Ptr ipMapControl;

			ipGSActiveView = m_pFrameworkCom->GetActiveView();
			ipGSActiveView->GetViewType(&sType);
			if (sType == VT_MapControlView)
			{
				HRESULT hr;
				IUnknownPtr ipUnknown;
				hr = ipGSActiveView->GetControl(&ipUnknown);
				ipMapControl = ipUnknown;

				CString strXZQMC = m_wndXZQTree.GetItemText(item);
				if (strXZQMC.Compare(_T("[000]中心城区")) == 0)//如果是中心城区
				{
					ZoomToXZQ(ipMapControl,DisItem->strParentID,NULL);
				}else
				{
					ZoomToXZQ(ipMapControl,DisItem->strCode,DisItem->strParentID);
				}
			}

		}
	}else
	{
		//change by lcc 2016-11-14 将全局变量由bstr变更为cstring
		m_strName = _T("");
		m_strCode = _T("");
	}
	*pResult = TRUE;
}

void CLPXZQWindowDlg::ZoomToXZQ(IMapControl2Ptr ipMapControl,CString strCode,CString strParentCode)
{
	//根据条件查找符合条件的要素
	CString strWhere = _T("");
	CString strLayerName = _T("");
	switch(strCode.GetLength())
	{
	case 2:
		strLayerName = _T("XZQZ");
		strWhere = _T("XZQDM='") + strCode + _T("'");
		break;
	case 4:
		strLayerName = _T("XZQS");
		strWhere = _T("XZQDM='") + strCode + _T("'");
		break;
	case 6:
		strLayerName = _T("XZQX");
		strWhere = _T("XZQDM='") + strCode + _T("'");
		break;
	case 9:
		strLayerName = _T("XZQXZ");
		strWhere = _T("XZQDM='") + strCode + _T("'");
		break;
	case 12:
		strLayerName = _T("XZQ");
		strWhere = _T("XZQDM='") + strCode + _T("' and XZDM='") + strParentCode + _T("'");
		break;
	default:
		strLayerName = _T("XZQXZ");
		strCode.Replace(_T(","),_T("','"));
		strWhere = _T("XZQDM in ('") + strCode + _T("')");
		break;
	}


	CComQIPtr<IMap> spMap;
	ipMapControl->get_Map(&spMap);
	
	CComQIPtr<IUID> spUID;
	spUID.CoCreateInstance(CLSID_UID);
	CComQIPtr<IEnumLayer> spEnumLayer;
	spUID->put_Value(_variant_t("{40A9E885-5533-11d0-98BE-00805F7CED21}"));
	spMap->get_Layers(spUID,VARIANT_TRUE,&spEnumLayer);
	if (spEnumLayer == NULL) return ;
	CComQIPtr<ILayer> spLayer;

	spEnumLayer->Next(&spLayer);
	while(spLayer != NULL)
	{
		BSTR bstrLayerName;
		CComQIPtr<IFeatureLayer> spFtLayer;
		spFtLayer = spLayer;
		CComQIPtr<IFeatureClass> spFtClass;
		spFtLayer->get_FeatureClass(&spFtClass);
		//edit by lcc 2016.12.06 添加验证防止有图层没有匹配上导致错误
		if (spFtClass == NULL)
		{
			spLayer.Release();
			spEnumLayer->Next(&spLayer);
			continue;
		}
		//edit by lcc 2016.12.06 end
		spFtClass->get_AliasName(&bstrLayerName);
		CComQIPtr<IDataset> spDataset;
		spDataset = spFtClass;
		spDataset->get_Name(&bstrLayerName);
		CString strFtName(bstrLayerName);
		strFtName = strFtName.Right(strFtName.GetLength() - strFtName.Find(_T(".")) - 1);
 		if (strLayerName.Compare(strFtName) == 0)
 		{	
			IActiveViewPtr ipActiveView;
			ipMapControl->get_ActiveView(&ipActiveView);
			CComQIPtr<IMap> spMap;
			ipActiveView->get_FocusMap(&spMap);
			CComQIPtr<ISpatialReference> spSpatialReference;
			spMap->get_SpatialReference(&spSpatialReference);
			
			IQueryFilterPtr ipQuery(CLSID_QueryFilter);
			ipQuery->put_SubFields(_T("*"));
			ipQuery->put_WhereClause(strWhere.AllocSysString());
			IFeatureCursorPtr ipCursor;
			spFtClass->Search(ipQuery,VARIANT_FALSE,&ipCursor);

			//IPolygonPtr ipPolygon(CLSID_Polygon);
			IGeometryPtr spGeometry(CLSID_GeometryBag);//用于闪烁的图像
			spGeometry->putref_SpatialReference(spSpatialReference);
			CComQIPtr<IGeometryCollection> spGeoCollection;
			spGeometry->QueryInterface(&spGeoCollection);

	//edit by lcc 2016.12.14 添加对多要素行政区缩放的支持
			CComQIPtr<IEnvelope> ipZoomToEnv;//用于缩放的图形
			ipZoomToEnv.CoCreateInstance(CLSID_Envelope);
			CComQIPtr<IFeature> ipFt;
			ipCursor->NextFeature(&ipFt);
			while (ipFt != NULL)
			{	
				IGeometryPtr ipGeo;
				IEnvelopePtr ipEnvelope;

				ipFt->get_Shape(&ipGeo);
					
				esriGeometryType Geotype;
				ipGeo->get_GeometryType(&Geotype);
				ipGeo->get_Envelope(&ipEnvelope);
				if (ipEnvelope != NULL && Geotype == esriGeometryPolygon)
				{
					//获取要素的外接矩形
					ipZoomToEnv->Union(ipEnvelope);
					spGeoCollection->AddGeometry(ipGeo);
				}
				ipFt.Release();
				ipCursor->NextFeature(&ipFt);
			}
			HRESULT hr;
			CComQIPtr<ICommand> spCommand;
			hr = spCommand.CoCreateInstance(CLSID_ControlsClearSelectionCommand);
			if (SUCCEEDED(hr))
			{
				//IUnknown *pUnk = spGSActiveView->GetControl();
				spCommand->OnCreate((IDispatch *)ipMapControl);
				spCommand->OnClick();
// 				IActiveViewPtr ipActiveView;
// 				ipMapControl->get_ActiveView(&ipActiveView);
// 				CComQIPtr<IScreenDisplay> spScreenDisplay;
// 				ipActiveView->get_ScreenDisplay(&spScreenDisplay);
// 				spScreenDisplay->UpdateWindow();
			}
			//以这个矩形为边界进行缩放			
			ipActiveView->put_Extent(ipZoomToEnv);

			if (ipActiveView->PartialRefresh(esriViewAll,NULL,NULL) == S_OK)
			{
				FlashShapes(ipMapControl,spGeometry);
				::Sleep(200);
				FlashShapes(ipMapControl,spGeometry);
			}

			//ipActiveView->PartialRefresh(esriViewForeground, NULL, NULL);
			break;
//edit by lcc 2016.12.14 end
 		}
 		spLayer.Release();
 		spEnumLayer->Next(&spLayer);
	}
	ipMapControl->Refresh(esriViewAll);
}

void CLPXZQWindowDlg::GetSelectedRegion(BSTR* strName /* = NULL */,BSTR* strCode /* = NULL */)
{
	if (strName == NULL && strCode == NULL)
	{
		return;
	}
	if (m_strName == _T(""))
	{
		*strName = NULL;
		*strCode = NULL;
		return;
	}
	else
	{
		//change by lcc 2016-11-14 将全局变量由bstr变更为cstring
		*strName = m_strName.AllocSysString();
		*strCode = m_strCode.AllocSysString();
	}
	
}
void CLPXZQWindowDlg::OnNMClickXzqTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	_tagDistrictItem *DisItem;
	//获取点击项目
	CPoint point;
	GetCursorPos(&point);
	m_wndXZQTree.ScreenToClient(&point);
	HTREEITEM item = m_wndXZQTree.HitTest(point);
	CString strXZQName;
	if (item != NULL)
	{
		m_wndXZQTree.SelectItem(item);
		//写入全局变量
		
		DisItem = (_tagDistrictItem *)m_wndXZQTree.GetItemData(item);
		//change by lcc 2016-11-14 将全局变量由bstr变更为cstring
// 		*m_bstrName = DisItem->strName.AllocSysString();
// 		*m_bstrCode = DisItem->strCode.AllocSysString();
		m_strName = DisItem->strName;
		m_strCode = DisItem->strCode;
	}else
	{
		m_wndXZQTree.SelectItem(NULL);
		m_strName = _T("");
		m_strCode = _T("");
	}
	//add by lcc 2016.12.23
	if (m_pmapXZQ == NULL)
	{
		CComQIPtr<ILPXZQDataTree> spXZQDatatree;
		spXZQDatatree.CoCreateInstance(CLSID_LPXZQDataTree);
		IUnknown *pUnknown;
		spXZQDatatree->GetXZQMap(&pUnknown);
		m_pmapXZQ = (CMapStringToString *)pUnknown;
	}

	if (m_strCode.GetLength() == 12)//村级返回乡镇代码
	{
		m_strCode = DisItem->strParentID;
		m_pmapXZQ->Lookup(m_strCode,m_strName);
	}
	if (m_strName.Compare(_T("中心城区")) == 0)//中心城区返回空
	{
		m_strName = _T("");
		m_strCode = _T("");
	}

	if (m_strName.GetLength() > 0 && m_strCode.GetLength() > 0)
	{
		BSTR bstrName = m_strName.AllocSysString();
		BSTR bstrCode = m_strCode.AllocSysString();
		m_pParentCOM->Fire_OnSelectXZQItem(&bstrCode,&bstrName);
	}else
	{
		m_pParentCOM->Fire_OnSelectXZQItem(NULL,NULL);
	}

}

void CLPXZQWindowDlg::SortArray(int left, int right)
{
	//如果左右索引相遇则结束
	if (left>=right)
	{
		return;
	}

	int i = left;
	int j = right;
	//参考点
	_tagDistrictItem *keyitem;
	CString strkeyCode;
	
	//参考值
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

	SortArray(left, i - 1);/*用同样的方式对分出来的左边的小组进行快排*/
	SortArray(i + 1, right);/*用同样的方式对分出来的右边的小组进行快排*/

}

void CLPXZQWindowDlg::FlashShapes(IMapControl2Ptr ipMapControl, IGeometryPtr ipGeo )
{
	if (ipGeo == NULL)	return;
	
	IActiveViewPtr ipActiveView;
	ipMapControl->get_ActiveView(&ipActiveView);
	IMapPtr ipMap;
	ipActiveView->get_FocusMap(&ipMap);
	IScreenDisplayPtr ipScreenDisp;
	ipActiveView->get_ScreenDisplay(&ipScreenDisp);
	OLE_HANDLE hdc;
	ipScreenDisp->get_hDC(&hdc);

	// 为了提高绘制效率, 先在CacheMemDC里绘, 然后再绘制到ScreenDisplay上
	short nMemCacheID;
	ipScreenDisp->AddCache(&nMemCacheID);
	OLE_HANDLE hMemDC;
	ipScreenDisp->get_CacheMemDC(nMemCacheID, &hMemDC);

	// 设置symbol
	IColorPtr ipRgbColor(CLSID_RgbColor);
	ISimpleFillSymbolPtr ipStylePg(CLSID_SimpleFillSymbol);
	ISimpleLineSymbolPtr ipStyLn(CLSID_SimpleLineSymbol);
	//边界样式
	ipRgbColor->put_RGB(RGB(255, 123, 128));
	ipStyLn->put_Style(esriSLSSolid);
	ipStyLn->put_Color(ipRgbColor);
	ipStyLn->put_Width(4);
	//多边形样式
	ipStylePg->put_Color(ipRgbColor);
	ipStylePg->put_Style(esriSFSBackwardDiagonal);
	ipStylePg->put_Outline(ipStyLn);

	ISymbolPtr ipSymbol = ipStylePg;

	// 绘图形
	double dPointBufferDis = 6;
	double dLineBufferDis = 2;

	IDisplayTransformationPtr ipDisplayTransformation;
	ipScreenDisp->get_DisplayTransformation(&ipDisplayTransformation);
	ipSymbol->SetupDC(hMemDC, ipDisplayTransformation);

	//先更新屏幕到缩放后的位置
	ipScreenDisp->UpdateWindow();
//edit by lcc 2016.12.14 修改行政区缩放逻辑
	esriGeometryType eShapeType;
	ipGeo->get_GeometryType(&eShapeType);

 		CComQIPtr<IGeometryCollection> spGeoCollection;
 		ipGeo->QueryInterface(&spGeoCollection);
 		long nCount;
 		spGeoCollection->get_GeometryCount(&nCount);
 		for (long i = 0;i < nCount; i++)
 		{
 			IGeometryPtr ipShape, ipBufferShape;
 			spGeoCollection->get_Geometry(i, &ipShape);
 			ipBufferShape = ipShape;
 			esriGeometryType eShapeType;
 			ipShape->get_GeometryType(&eShapeType);
 			if (eShapeType != esriGeometryPolygon)
 			{
 				double dBufferDis = 0;
 				if (eShapeType == esriGeometryPoint)
 					dBufferDis = dPointBufferDis;
 				else if (eShapeType == esriGeometryPolyline)
 					dBufferDis = dLineBufferDis;
 
 				ITopologicalOperatorPtr ipTopOper = ipShape;
 				ipTopOper->Buffer(dBufferDis, &ipBufferShape);
 			}
 			ipSymbol->Draw(ipBufferShape);
 		}

	//遍历集合
	ipSymbol->ResetDC();
	ipScreenDisp->DrawCache(hdc, nMemCacheID, NULL, NULL);
	ipScreenDisp->RemoveCache(nMemCacheID);
	// 延时100毫秒擦掉绘制
	::Sleep(200);
	ipActiveView->PartialRefresh(esriViewForeground, NULL, NULL);

//edit by lcc 2016.12.14 end

}

BOOL CLPXZQWindowDlg::InitXzqTree2(CComQIPtr<IFeatureWorkspace> ipFtWork/* = NULL*/)
{
// 	//这里还是要保持每一个行政区土层的链接，用于缩放时快速查找
// 	ipFtWork->OpenFeatureClass(_("XZQZ"),&m_spZFtClass);
// 	ipFtWork->OpenFeatureClass(_("XZQZ"),&m_spSFtClass);
// 	ipFtWork->OpenFeatureClass(_("XZQZ"),&m_spXFtClass);
// 	ipFtWork->OpenFeatureClass(_("XZQZ"),&m_spXZFtClass);
// 	ipFtWork->OpenFeatureClass(_("XZQZ"),&m_spCUNFtClass);
	CImageList *TreeImage = NULL;
	InitImageList(&TreeImage);
	m_wndXZQTree.SetImageList(TreeImage,TVSIL_NORMAL);

	CComQIPtr<ILPXZQDataTree> ipTree;

	CComQIPtr<ILPXZQDataItem> ipRoot;
	IUnknown *ipTmpItem;
	ipTree.CoCreateInstance(CLSID_LPXZQDataTree);
	ipTree->GetRootItem(ipFtWork,&ipTmpItem);
	ipRoot = (CComQIPtr<ILPXZQDataItem>) ipTmpItem;
	if (ipRoot == NULL) return TRUE;
	BSTR bstrTmp;
	ipRoot->Get_ItemCode(&bstrTmp);
	m_strParent = bstrTmp;
	AddDistrictTreeItem2(NULL,ipRoot);
	HTREEITEM item = m_wndXZQTree.GetFirstVisibleItem();
	m_wndXZQTree.Expand(item,TVE_EXPAND);
// 	ipTree->GetXZQMap(&ipTmpItem);
// 	CMapStringToString *tmpMap = (CMapStringToString*)ipTmpItem;
	
	return TRUE;
}
void CLPXZQWindowDlg::InitImageList(CImageList** ppImageList)
{
	CImageList *TreeImage = new CImageList;
	TreeImage->Create(16, 16, ILC_COLOR32 | ILC_MASK,0,6);
	CBitmap *BitmapTmp = new CBitmap;
	BitmapTmp->LoadBitmap(IDB_BMP_ICON_SHENG);
	TreeImage->Add(BitmapTmp,RGB(255,0,255));
	
	BitmapTmp = NULL;
	BitmapTmp = new CBitmap;
	BitmapTmp->LoadBitmap(IDB_BMP_ICON_SHI);
	TreeImage->Add(BitmapTmp,RGB(255,0,255));

	BitmapTmp = NULL;
	BitmapTmp = new CBitmap;
	BitmapTmp->LoadBitmap(IDB_BMP_ICON_XIANQU);
	TreeImage->Add(BitmapTmp,RGB(255,0,255));

	BitmapTmp = NULL;
	BitmapTmp = new CBitmap;
	BitmapTmp->LoadBitmap(IDB_BMP_ICON_XIANGZHEN);
	TreeImage->Add(BitmapTmp,RGB(255,0,255));
//edit by lcc 2016.12.14 添加村级和中心城区图片
	BitmapTmp = NULL;
	BitmapTmp = new CBitmap;
	BitmapTmp->LoadBitmap(IDB_BMP_ICON_CUN);
	TreeImage->Add(BitmapTmp,RGB(255,0,255));
	
	BitmapTmp = NULL;
	BitmapTmp = new CBitmap;
	BitmapTmp->LoadBitmap(IDB_BMP_ICON_ZXQC);
	TreeImage->Add(BitmapTmp,RGB(255,0,255));
	*ppImageList = TreeImage;
	//edit by lcc 2016.12.14 end
}

void CLPXZQWindowDlg::AddDistrictTreeItem2(HTREEITEM parentItem/*=NULL*/,CComQIPtr<ILPXZQDataItem> spDataItem)
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
	HTREEITEM hItem;
	if (ItemData->nLevel == 0)//省
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(2),ItemData->strName);
		hItem = m_wndXZQTree.InsertItem(showValue,0,0,parentItem);
		m_wndXZQTree.SetItemData(hItem,(DWORD_PTR)ItemData);
	}else if (ItemData->nLevel  == 1)//市
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(2),ItemData->strName);
		hItem = m_wndXZQTree.InsertItem(showValue,1,1,parentItem);
		m_wndXZQTree.SetItemData(hItem,(DWORD_PTR)ItemData);
	}else if (ItemData->nLevel  == 2)//县区
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(2),ItemData->strName);
		hItem = m_wndXZQTree.InsertItem(showValue,2,2,parentItem);
		m_wndXZQTree.SetItemData(hItem,(DWORD_PTR)ItemData);
	}else if (ItemData->nLevel  == 4)//乡镇街道
	{
		//edit by lcc 2016.12.14 添加中心城区图片
		if (ItemData->strName.Compare(_T("中心城区")) == 0)
		{
			CString showValue;
			showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(3),ItemData->strName);
			hItem = m_wndXZQTree.InsertItem(showValue,5,5,parentItem);
			m_wndXZQTree.SetItemData(hItem,(DWORD_PTR)ItemData);
		}else
		{
			CString showValue;
			showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(3),ItemData->strName);
			hItem = m_wndXZQTree.InsertItem(showValue,3,3,parentItem);
			m_wndXZQTree.SetItemData(hItem,(DWORD_PTR)ItemData);
		}
		//edit by lcc 2016.12.14 end
	
	}else if (ItemData->nLevel  == 6)//村
	{
		CString showValue;
		showValue.Format(_T("[%s]%s"),ItemData->strCode.Right(3),ItemData->strName);
		hItem = m_wndXZQTree.InsertItem(showValue,4,4,parentItem);
		m_wndXZQTree.SetItemData(hItem,(DWORD_PTR)ItemData);
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
		AddDistrictTreeItem2(hItem,spChlidDataItem);
	}

}

//add by lcc 2016.12.23
void CLPXZQWindowDlg::setParentCOMPtr(CLPXZQWindowCmd *pParent)
{
	if (pParent == NULL) return;
	m_pParentCOM = pParent;
}

void CLPXZQWindowDlg::ZoomToTopXZQ()
{
	//缩放至最顶级的行政区
	CString strCode;
	CString strParentCode;
	short sType;
	IGSActiveViewPtr ipGSActiveView;
	IMapControl2Ptr ipMapControl;

	ipGSActiveView = m_pFrameworkCom->GetActiveView();
	ipGSActiveView->GetViewType(&sType);
	if (sType == VT_MapControlView)
	{
		HRESULT hr;
		IUnknownPtr ipUnknown;
		hr = ipGSActiveView->GetControl(&ipUnknown);
		ipMapControl = ipUnknown;

		HTREEITEM hItem = m_wndXZQTree.GetRootItem();
		if (hItem == NULL)
		{
			return;
		}
		_tagDistrictItem* DisItem = (_tagDistrictItem*)m_wndXZQTree.GetItemData(hItem);
		//ZoomToXZQ(ipMapControl,DisItem->strCode,DisItem->strParentID);
		strCode = DisItem->strCode;
		strParentCode = DisItem->strParentID;
	}

	CString strWhere = _T("");
	CString strLayerName = _T("");
	switch(strCode.GetLength())
	{
	case 2:
		strLayerName = _T("XZQZ");
		strWhere = _T("XZQDM='") + strCode + _T("'");
		break;
	case 4:
		strLayerName = _T("XZQS");
		strWhere = _T("XZQDM='") + strCode + _T("'");
		break;
	case 6:
		strLayerName = _T("XZQX");
		strWhere = _T("XZQDM='") + strCode + _T("'");
		break;
	case 9:
		strLayerName = _T("XZQXZ");
		strWhere = _T("XZQDM='") + strCode + _T("'");
		break;
	case 12:
		strLayerName = _T("XZQ");
		strWhere = _T("XZQDM='") + strCode + _T("' and XZDM='") + strParentCode + _T("'");
		break;
	default:
		strLayerName = _T("XZQXZ");
		strCode.Replace(_T(","),_T("','"));
		strWhere = _T("XZQDM in ('") + strCode + _T("')");
		break;
	}


	CComQIPtr<IMap> spMap;
	ipMapControl->get_Map(&spMap);

	CComQIPtr<IUID> spUID;
	spUID.CoCreateInstance(CLSID_UID);
	CComQIPtr<IEnumLayer> spEnumLayer;
	spUID->put_Value(_variant_t("{40A9E885-5533-11d0-98BE-00805F7CED21}"));
	spMap->get_Layers(spUID,VARIANT_TRUE,&spEnumLayer);
	if (spEnumLayer == NULL) return ;
	CComQIPtr<ILayer> spLayer;

	spEnumLayer->Next(&spLayer);
	while(spLayer != NULL)
	{
		BSTR bstrLayerName;
		CComQIPtr<IFeatureLayer> spFtLayer;
		spFtLayer = spLayer;
		CComQIPtr<IFeatureClass> spFtClass;
		spFtLayer->get_FeatureClass(&spFtClass);
		//edit by lcc 2016.12.06 添加验证防止有图层没有匹配上导致错误
		if (spFtClass == NULL)
		{
			spLayer.Release();
			spEnumLayer->Next(&spLayer);
			continue;
		}
		//edit by lcc 2016.12.06 end
		spFtClass->get_AliasName(&bstrLayerName);
		CComQIPtr<IDataset> spDataset;
		spDataset = spFtClass;
		spDataset->get_Name(&bstrLayerName);
		CString strFtName(bstrLayerName);
		strFtName = strFtName.Right(strFtName.GetLength() - strFtName.Find(_T(".")) - 1);
		if (strLayerName.Compare(strFtName) == 0)
		{	
			IActiveViewPtr ipActiveView;
			ipMapControl->get_ActiveView(&ipActiveView);
			CComQIPtr<IMap> spMap;
			ipActiveView->get_FocusMap(&spMap);
			CComQIPtr<ISpatialReference> spSpatialReference;
			spMap->get_SpatialReference(&spSpatialReference);

			IQueryFilterPtr ipQuery(CLSID_QueryFilter);
			ipQuery->put_SubFields(_T("*"));
			ipQuery->put_WhereClause(strWhere.AllocSysString());
			IFeatureCursorPtr ipCursor;
			spFtClass->Search(ipQuery,VARIANT_FALSE,&ipCursor);

			//IPolygonPtr ipPolygon(CLSID_Polygon);
			IGeometryPtr spGeometry(CLSID_GeometryBag);//用于闪烁的图像
			spGeometry->putref_SpatialReference(spSpatialReference);
			CComQIPtr<IGeometryCollection> spGeoCollection;
			spGeometry->QueryInterface(&spGeoCollection);

			//edit by lcc 2016.12.14 添加对多要素行政区缩放的支持
			CComQIPtr<IEnvelope> ipZoomToEnv;//用于缩放的图形
			ipZoomToEnv.CoCreateInstance(CLSID_Envelope);
			CComQIPtr<IFeature> ipFt;
			ipCursor->NextFeature(&ipFt);
			while (ipFt != NULL)
			{	
				IGeometryPtr ipGeo;
				IEnvelopePtr ipEnvelope;

				ipFt->get_Shape(&ipGeo);

				esriGeometryType Geotype;
				ipGeo->get_GeometryType(&Geotype);
				ipGeo->get_Envelope(&ipEnvelope);
				if (ipEnvelope != NULL && Geotype == esriGeometryPolygon)
				{
					//获取要素的外接矩形
					ipZoomToEnv->Union(ipEnvelope);
					spGeoCollection->AddGeometry(ipGeo);
				}
				ipFt.Release();
				ipCursor->NextFeature(&ipFt);
			}
			HRESULT hr;
			//以这个矩形为边界进行缩放			
			ipActiveView->put_Extent(ipZoomToEnv);
			//ipActiveView->PartialRefresh(esriViewForeground, NULL, NULL);
			break;
			//edit by lcc 2016.12.14 end
		}
		spLayer.Release();
		spEnumLayer->Next(&spLayer);
	}
	ipMapControl->Refresh(esriViewAll);
}
