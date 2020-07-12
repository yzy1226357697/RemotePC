// CscreenDLG.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_Server.h"
#include "CscreenDLG.h"
#include "afxdialogex.h"
#include "MFC_ServerDlg.h"


// CscreenDLG 对话框

IMPLEMENT_DYNAMIC(CscreenDLG, CDialogEx)

CscreenDLG::CscreenDLG(CWnd* pParent /*=NULL*/)
	: CDialogEx(CscreenDLG::IDD, pParent)
{

}

CscreenDLG::~CscreenDLG()
{
}

void CscreenDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CscreenDLG, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CscreenDLG 消息处理程序
BOOL CscreenDLG::showdata(char* pbuf, DWORD len) {
	
	int hWidth;
	int hHeight;
	memcpy(&hWidth, pbuf, sizeof(int));
	memcpy(&hHeight, pbuf + sizeof(int), sizeof(int));
	CDC cDc;
	CBitmap Cbitmap;
	//获取自己的兼容DC
	cDc.CreateCompatibleDC(GetDC());
	Cbitmap.CreateCompatibleBitmap(GetDC(), hWidth, hHeight);
	cDc.SelectObject(Cbitmap);
	Cbitmap.SetBitmapBits(len - sizeof(int)* 2, pbuf + sizeof(int)* 2);
	GetDC()->BitBlt(0, 0, hWidth, hHeight, &cDc, 0, 0, SRCCOPY);
	

	return TRUE;
}

void CscreenDLG::OnClose() {
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	
	CDialogEx::OnClose();
	

}
