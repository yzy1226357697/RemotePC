// cProcesslistDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_Server.h"
#include "cProcesslistDlg.h"
#include "afxdialogex.h"


/*
//发送头
BOOL sendData(SOCKET s, LPCVOID data, DWORD len) {

	if (s == INVALID_SOCKET)
		return FALSE;
	int countlen = 0;//发送的总长
	while (countlen < len) {
		int n = send(s, ((char*)data) + countlen, len - countlen, 0);//单次发送的长度
		if (n <= 0)
			return FALSE;
		else {
			countlen += n;
		}
	}
	return TRUE;
}

BOOL sendData(SOCKET s, DWORD code, LPCVOID data, DWORD len) {
	PackHeader PH;
	PH.type = code;
	PH.length = len;
	if (s == INVALID_SOCKET || data == NULL || len == 0)
		return FALSE;
	int countlen = 0;//发送的总长
	while (countlen < len) {
		int n = send(s, ((char*)data) + countlen, len - countlen, 0);//单次发送的长度
		if (n <= 0)
			return FALSE;
		else {
			countlen += n;
		}
	}
	return TRUE;
}
*/

// cProcesslistDlg 对话框

IMPLEMENT_DYNAMIC(cProcesslistDlg, CDialogEx)

cProcesslistDlg::cProcesslistDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(cProcesslistDlg::IDD, pParent)
{

}

cProcesslistDlg::~cProcesslistDlg()
{
}

void cProcesslistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROCESS, m_listprocess);
	DDX_Control(pDX, IDC_MODULE, m_listmodule);
	DDX_Control(pDX, IDC_DLLPATH, m_editpath);
}


BEGIN_MESSAGE_MAP(cProcesslistDlg, CDialogEx)
	
	ON_NOTIFY(NM_CLICK, IDC_PROCESS, &cProcesslistDlg::OnNMClickProcess)
	ON_COMMAND(ID_INJECTSC, &cProcesslistDlg::OnInjectsc)
	ON_NOTIFY(NM_RCLICK, IDC_PROCESS, &cProcesslistDlg::OnNMRClickProcess)
	ON_COMMAND(ID_INJECTDLL, &cProcesslistDlg::OnInjectdll)
END_MESSAGE_MAP()








void cProcesslistDlg::OnNMClickProcess(NMHDR *pNMHDR, LRESULT *pResult) {
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	
	if (!(m_listprocess.GetSelectedCount()) )
		return;
	//有选中行的处理
	m_listmodule.DeleteAllItems();
	PackHeader PH;
	PH.type = STOC_MODULE;
	PH.length = sizeof(DWORD);

	DWORD processid = m_listprocess.GetItemData( m_listprocess.GetSelectionMark());
	sendData(m_sock, &PH, sizeof(PackHeader));
	sendData(m_sock, &processid, sizeof(DWORD));
	
	*pResult = 0;
}


void cProcesslistDlg::OnInjectsc() {
	// TODO:  在此添加命令处理程序代码
	//发送shellcode指令
	PackHeader PH;
	PH.type = STOC_SHELLCODE;
	PH.length = sizeof(DWORD);
	DWORD dwProcess = m_listprocess.GetItemData(m_listprocess.GetSelectionMark());
	sendData(m_sock, &PH, sizeof(PackHeader));
	sendData(m_sock, &dwProcess, sizeof(DWORD));
}


void cProcesslistDlg::OnNMRClickProcess(NMHDR *pNMHDR, LRESULT *pResult) {
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	if (m_listprocess.GetNextItem(-1, LVIS_SELECTED) == -1)
		return;
	CPoint  cPint;
	CMenu cMenu;
	GetCursorPos(&cPint);
	cMenu.LoadMenuW(IDR_MENU2);
	CMenu* psubMenu = cMenu.GetSubMenu(0);
	psubMenu->TrackPopupMenu(TPM_LEFTALIGN, cPint.x, cPint.y, this);


	*pResult = 0;
}


void cProcesslistDlg::OnInjectdll() {
	// TODO:  在此添加命令处理程序代码
	PackHeader PH;
	PH.type = STOC_INJECTDLL;
	//获取进程id
	DWORD dwprocess = m_listprocess.GetItemData(m_listprocess.GetSelectionMark());
	
	//获取dll路径
	CString dllPath;
	GetDlgItemText(IDC_DLLPATH, dllPath);
	//先发送进程id
	PH.length = sizeof(DWORD)+(_tcslen(dllPath) * 2)+2;
	sendData(m_sock, &PH, sizeof(PackHeader));
	TCHAR* data = new  TCHAR[sizeof(DWORD)+(_tcslen(dllPath) * 2)];
	memcpy(data, &dwprocess, sizeof(DWORD));
	_tcscpy((LPTSTR)((DWORD)data + sizeof(DWORD)), dllPath);
	//发送数据
	sendData(m_sock, data, PH.length);
	delete[] data;


	
}
