#pragma once
#include "afxcmn.h"
#include "..\..\PackHead.h"
#include "MFC_ServerDlg.h"
#include "resource.h"
#include "afxwin.h"
// cProcesslistDlg 对话框

class cProcesslistDlg : public CDialogEx
{
	DECLARE_DYNAMIC(cProcesslistDlg)

public:
	cProcesslistDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~cProcesslistDlg();

// 对话框数据
	enum { IDD = IDD_CPROCESSLISTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listprocess;
	CListCtrl m_listmodule;
	SOCKET m_sock;
	
	afx_msg void OnNMClickProcess(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnInjectsc();
	afx_msg void OnNMRClickProcess(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnInjectdll();
	CEdit m_editpath;
};
