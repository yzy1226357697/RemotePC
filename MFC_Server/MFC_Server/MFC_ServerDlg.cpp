
// MFC_ServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_Server.h"
#include "MFC_ServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif





// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFC_ServerDlg 对话框



CMFC_ServerDlg::CMFC_ServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC_ServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_ServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CMFC_ServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


//多字节到宽字节
std::wstring AsciiToUnicode(const std::string& str)

{

	int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);

	wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);

	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pUnicode, unicodeLen);

	std::wstring ret_str = pUnicode;

	free(pUnicode);

	return ret_str;

}



//多线程等待用户连接的回调
static DWORD _stdcall CallBackAccept(LPVOID p) {
	CMFC_ServerDlg* pthis = (CMFC_ServerDlg*)p;
	while (TRUE) {
		//客户端的addr
		sockaddr_in addr = { 0 };
		int size = sizeof(sockaddr_in);
		SOCKET clientScok = accept(pthis->m_ServerSock,(sockaddr*)&addr, &size);
		if (clientScok == INVALID_SOCKET)
			return 0;
		//在堆里创建回话结构体，以防被栈释放掉
		s_MySession* pSession = new s_MySession;
		pSession->clientSock = clientScok;
		pSession->addr = addr;
		pSession->dwLastTickout = GetTickCount();
		{
			//在map操作的时候要加上互斥体，以防多线程同一时间操作一个map元素
			std::lock_guard<std::mutex> lg(pthis->m_Acceptmutex);
			pthis->m_map.insert(std::pair<SOCKET, s_MySession*>(clientScok, pSession));

		}
		//将客户session内容插入列表
		TCHAR addrbuf[256] = { 0 };
		std::wstring wstr = AsciiToUnicode(inet_ntoa(addr.sin_addr));
		
		_swprintf(addrbuf, _T("%s:%d"), wstr.c_str(), ntohs( addr.sin_port));
		pthis->m_list.InsertItem(pthis->m_map.size(), addrbuf);
		

		std::thread thd([&]() {
		
			char data[100] = { 0 };
			char buf[256] = { 0 };
			sockaddr_in formaddr = addr;
			formaddr.sin_port = ntohs(addr.sin_port);
			int n =recv(clientScok, buf, sizeof(buf)-1, 0);
			sprintf(data, "%s:%d", inet_ntoa(formaddr.sin_addr), formaddr.sin_port);
			MessageBoxA(0, data, 0, 0);
			MessageBoxA(0, buf, 0, 0);

		});
		thd.detach();

	}

	return 0;
}

//初始化SOCKET
BOOL CMFC_ServerDlg::InitSock() {
	m_ServerSock = socket(AF_INET, SOCK_STREAM, 0);//创建服务器SOCKET 为TCP类型
	if (m_ServerSock == INVALID_SOCKET)
		return FALSE;

	//绑定的sockaddr
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("0.0.0.0");//接受所有连接
	addr.sin_port = htons(8848); //服务器端口

	int res = bind(m_ServerSock, (sockaddr*)&addr, sizeof(sockaddr_in));
	if (res == INVALID_SOCKET)
		return FALSE;
	res = listen(m_ServerSock, 5);
	if (res == INVALID_SOCKET)
		return FALSE;

	//应该开启一个线程循环接受客户连接
	CreateThread(NULL,//安全属性
		NULL,//栈初始化大小
		(LPTHREAD_START_ROUTINE)CallBackAccept,//回调函数
		this,//回调参数
		NULL,//创建线程立即启动
		NULL);//线程id

	return TRUE;

}

// CMFC_ServerDlg 消息处理程序

BOOL CMFC_ServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}


	//初始化socket
	InitSock();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_GRIDLINES);
	m_list.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);    //报表样式
	m_list.InsertColumn(0, TEXT("IP:Port"), LVCFMT_LEFT, 120);//增加一列，左对齐，120px
	m_list.InsertColumn(1, TEXT("system"), LVCFMT_LEFT, 120);//增加一列，左对齐，120px
	m_list.InsertColumn(2, TEXT("位置"), LVCFMT_LEFT, 120);//增加一列，左对齐，120px


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFC_ServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFC_ServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFC_ServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

