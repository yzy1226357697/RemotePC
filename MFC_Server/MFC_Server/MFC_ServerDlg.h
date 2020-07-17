
// MFC_ServerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include <map>
#include <thread>
#include <mutex>
#include <string>
#include "CscreenDLG.h"
#include "cProcesslistDlg.h"

struct s_MySession {
	SOCKET clientSock;
	sockaddr_in addr;
	DWORD dwLastTickout;//最后活跃时间，心跳包
	CscreenDLG* pscreen;

};
//DWORD _stdcall CallBackAccept(LPVOID p);

// CMFC_ServerDlg 对话框
class CMFC_ServerDlg : public CDialogEx
{
// 构造
public:
	CMFC_ServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFC_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	
	static DWORD _stdcall CallBackAccept(LPVOID p);

	BOOL InitSock();
	


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	HANDLE m_AcceptThead;//等待接受客户的线程句柄
	SOCKET m_ServerSock;//创建的服务器sock句柄

	std::map<SOCKET, s_MySession*> m_map;
	std::mutex m_Acceptmutex;

	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnScreen();

	VOID sendScreentcmd();
	afx_msg void OnCheckprocess();
};
BOOL sendData(SOCKET s, LPCVOID data, DWORD len);