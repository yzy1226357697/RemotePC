#pragma once


// CscreenDLG 对话框

class CscreenDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CscreenDLG)

public:
	CscreenDLG(CWnd* pParent = NULL);   // 标准构造函数

	
	BOOL showdata(char* pbuf, DWORD len );
	
	virtual ~CscreenDLG();

// 对话框数据
	enum { IDD = IDD_CSCREENDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	

	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnClose();

	
	
};
