// ComboTreeCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "ComboTreeCtrl.h"
#include "ComboBoxTreeCtrl.h"
#include "Resource.h"
// CComboTreeCtrl

IMPLEMENT_DYNAMIC(CComboTreeCtrl, CTreeCtrl)

CComboTreeCtrl::CComboTreeCtrl()
{
   m_parentComboBox=NULL;
   //next line add by lucc 2016.10.10
   m_selectedItem = NULL;
}

CComboTreeCtrl::~CComboTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CComboTreeCtrl, CTreeCtrl)
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, &CComboTreeCtrl::OnTvnItemexpanded)
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CComboTreeCtrl 消息处理程序



BOOL CComboTreeCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
    if (pMsg->message ==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE
		&& pMsg->hwnd==m_hWnd)
    {
		ShowWindow( SW_HIDE);
		return TRUE;
    }
	else if (pMsg->message ==WM_KEYDOWN && pMsg->wParam ==VK_RETURN
		&& pMsg->hwnd==m_hWnd)
	{
		ShowWindow(SW_HIDE);
		m_parentComboBox->ResetContent();
		HTREEITEM item=CTreeCtrl::GetSelectedItem();
		m_parentComboBox->AddString(GetItemText(item));
		m_parentComboBox->SetCurSel(0);
		return TRUE;
	}
	return CTreeCtrl::PreTranslateMessage(pMsg);
}

void CComboTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
// 	UINT nflag;
// 	HTREEITEM item=CTreeCtrl::HitTest(point,&nflag);
// 	if (item!=NULL)
// 	{
// 		SelectItem(item);
// 	}
	CTreeCtrl::OnMouseMove(nFlags, point);
}

void CComboTreeCtrl::OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//没有进行数据处理
	NM_TREEVIEW* pNMTreeView=(NM_TREEVIEW*)pNMHDR;
	HTREEITEM item=pNMTreeView->itemNew.hItem;

	*pResult = 0;
}

void CComboTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
 	// TODO: 在此添加消息处理程序代码和/或调用默认值
    UINT  nflag;
	HTREEITEM item=CTreeCtrl::HitTest(point,&nflag);
	if (nflag!=TVHT_ONITEMBUTTON)
	{
		if (item!=NULL)
		{
			SelectItem(item);
			//m_selectedItem=item;
			m_selectedItem=CTreeCtrl::GetSelectedItem();
			m_parentComboBox->ResetContent();
			m_parentComboBox->AddString(GetItemText(item));
			m_parentComboBox->SetCurSel(0);
			m_parentComboBox->SetItemData(0,GetItemData(item));
		}
		Hide();
		SendMessageNotiFy();
		return;
	}
	else
	    CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CComboTreeCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CPoint point;
	GetCursorPos(&point);
	CWnd* wnd = WindowFromPoint(point);
	if (wnd->GetSafeHwnd() != m_parentComboBox->GetSafeHwnd())
		ShowWindow(SW_HIDE);
	CTreeCtrl::OnKillFocus(pNewWnd);
	// TODO: 在此处添加消息处理程序代码
}

void CComboTreeCtrl::OnDestroy()
{
	CTreeCtrl::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}


BOOL CComboTreeCtrl::Init(CComboBoxTreeCtrl* parent)
{
     m_parentComboBox=parent;
	 return TRUE;
}
//创建树图标
BOOL CComboTreeCtrl::CreateImageList(UINT  nid,COLORREF crMask)
{
	int ncoutn=0;
	if (nid!=0)
	{
         m_imageList.Create(nid, 16, 1,crMask);
		 ncoutn=m_imageList.GetImageCount();
	}
// 	else
// 	      m_imageList.Create(IDB_DISTRICT_ICON, 16, 1,crMask);
	SetImageList(&m_imageList, TVSIL_NORMAL);
	return TRUE;
}
void CComboTreeCtrl::SelectItem(HTREEITEM item)
{
    m_selectedItem =item;//?
	CTreeCtrl::SelectItem(item);
}
HTREEITEM CComboTreeCtrl::GetSelectItem()
{
   return m_selectedItem;
}
HTREEITEM CComboTreeCtrl::SelectPrevVisibleItem()
{
    HTREEITEM item=GetSelectItem();
	item =GetPrevVisibleItem(item);
	if (item!=NULL)
	{
		SelectItem(item);
		SendMessageNotiFy();
	}
	return item;
}
HTREEITEM CComboTreeCtrl::SelectNextVisibleItem()
{
	HTREEITEM item=GetSelectItem();
	item =GetNextVisibleItem(item);
	if (item!=NULL)
	{
		SelectItem(item);
		SendMessageNotiFy();
	}
	return item;
}

BOOL CComboTreeCtrl::DeleteAllItems()
{
	m_selectedItem = NULL;
	return CTreeCtrl::DeleteAllItems();
}
void CComboTreeCtrl::Display(CRect rect)
{
    SetWindowPos(&wndNoTopMost,rect.left,rect.top,rect.right,rect.bottom,SWP_SHOWWINDOW);
	//this->SetFocus();
	CWnd* pwnd=GetParent()->GetParentOwner();
	if (pwnd!=NULL)
	{
		pwnd->SendMessage(WM_NCACTIVATE,TRUE);
		//pwnd->RedrawWindow();
	}
}
//向父窗口发送 层层转发
void CComboTreeCtrl::SendMessageNotiFy()
{
	CWnd* parent = m_parentComboBox->GetParent();
	if (parent != NULL && m_selectedItem != NULL)
	{	
		DWORD dwValue = (DWORD)GetItemData(m_selectedItem);
		//增加ctrlid的参数
		parent->SendMessage(WM_SELECTITEM_CHANGE, (WPARAM)dwValue,(LPARAM)m_parentComboBox->GetDlgCtrlID());
		m_parentComboBox->SetFocus();
	}
}
void CComboTreeCtrl::Hide()
{
   ShowWindow(SW_HIDE);
}
HTREEITEM CComboTreeCtrl::FindData(HTREEITEM item,DWORD dwData)
{
   HTREEITEM hFind;
   if (item==NULL)
       return NULL;
   while(item)
   {
	   DWORD dwValue=(DWORD)GetItemData(item);
	   if (dwValue==dwData)
	   {
		   return item;
	   }
	   if (ItemHasChildren(item))
	   {
		  HTREEITEM childItem=GetChildItem(item);
		  hFind=FindData(childItem,dwData);
		  if (hFind!=NULL)
		  {
			  return hFind;
		  }
		  else
			  item=GetNextSiblingItem(GetParentItem(childItem));
	   }
	   else
	   {
		   item=GetNextSiblingItem(item);
	   }
   }
   return item;
}