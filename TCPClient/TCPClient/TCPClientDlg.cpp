
// TCPClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TCPClient.h"
#include "TCPClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTCPClientDlg 对话框


CTCPClientDlg::CTCPClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTCPClientDlg::IDD, pParent)
	, m_IP(_T("127.0.0.1"))
	, m_port(_T("8888"))
	, m_msg(_T(""))
	, m_localIP(_T(""))
	, m_send(_T(""))
	, m_name(_T(""))
{
	m_clientSocket = NULL;
	m_sendBuf = m_send;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IP, m_IP);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDX_Text(pDX, IDC_EDIT_MSG, m_msg);
	DDX_Text(pDX, IDC_STATIC_LOCALIP, m_localIP);
	DDX_Text(pDX, IDC_EDIT_SEND, m_send);
	DDX_Control(pDX, IDC_EDIT_MSG, m_editMsg);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDX_Control(pDX, IDC_LIST_Client, m_listBox);
}

BEGIN_MESSAGE_MAP(CTCPClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CTCPClientDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CTCPClientDlg::OnBnClickedButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CTCPClientDlg::OnBnClickedButtonSend)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTCPClientDlg 消息处理程序

BOOL CTCPClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_EDIT_SEND)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(false);
	GetDlgItem(IDC_LIST_Client)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_IP)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_NAME)->EnableWindow(true);
	SetTimer(1, 100, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTCPClientDlg::OnPaint()
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
HCURSOR CTCPClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTCPClientDlg::printfMsg(CString str )
{
	UpdateData(true);
	m_msg += str;
	m_msg += "\r\n";
	UpdateData(false);
	m_editMsg.LineScroll(m_editMsg.GetLineCount() - 1, 0);
}

void CTCPClientDlg::printfError(CString str)
{
	CString strErr;
	strErr.Format(_T("%d"),GetLastError());
	str = _T("Error : ")  + str + _T(":") + strErr;
	printfMsg(str);
}

void CTCPClientDlg::onClose()
{
	printfMsg(_T("Server has disconnected!"));	
	OnBnClickedButtonDisconnect();
}

void CTCPClientDlg::onReceive()
{
	CString strRecv;
	char szRecv[256] = {0};
	m_clientSocket->Receive(szRecv,sizeof(szRecv));
	strRecv = szRecv;
	if(strRecv.Left(5) == _T("RETU:"))//发送数据后接到的反馈
	{
		strRecv.Delete(0,5);
		if(strRecv == m_sendBuf)
		{
			printfMsg(_T("SEND TO ") + m_sender+_T(" : ") + m_sendBuf + _T(" ...√"));
		}
	}
	else if(strRecv.Left(5) == _T("SEND:"))//服务器发来的数据
	{
		strRecv.Delete(0,5);
		printfMsg(_T("Server:") + strRecv);
		CString strReturn(_T("RETU:") + strRecv);
		m_clientSocket->Send(strReturn, strReturn.GetLength());
	}
	else if(strRecv.Left(5) == _T("LIST:"))//服务器发来的，所有用户列表
	{
		strRecv.Delete(0,5);
		UpdateData(true);
		m_listBox.ResetContent();
		m_listBox.AddString(_T("Server"));
		int n = strRecv.Find(_T("||"));
		while(n != -1)
		{
			CString listItem = strRecv.Left(n);
			if(listItem != m_name)
			{
				m_listBox.AddString(listItem);
			}			
			strRecv.Delete(0,n + 2);
			n = strRecv.Find(_T("||"));
		}          
		m_listBox.SetCurSel(0);
	}
	else if(strRecv.Left(5) == _T("SETO:"))//别的客户端通过服务器发来的数据
	{
		CString str = strRecv;
		strRecv.Delete(0,5);
		int n = strRecv.Find(_T("::"));
		CString senderName = strRecv.Left(n);
		strRecv.Delete(0,n + 2);
		printfMsg(senderName + _T(":") + strRecv);		
		CString strReturn(_T("SERE:") + senderName + _T("::") + strRecv);
		m_clientSocket->Send(strReturn, strReturn.GetLength());
	}
	else if(strRecv.Left(5) == _T("ERRO:"))//服务器发来的，名字重复提示
	{
		strRecv.Delete(0,5);
		if(_T("Name repetition") == strRecv)
		{
			printfMsg(_T("Server:") + strRecv);
			OnBnClickedButtonDisconnect();
		}
	}
	else if (strRecv.Left(5) == _T("REGU:"))//服务器发来的定时数据
	{
		strRecv.Delete(0, 5);
		printfMsg(_T("Server Regular:") + strRecv);
		CString strReturn(_T("SEND:") + strRecv);
		m_clientSocket->Send(strReturn, strReturn.GetLength());
	}
	else if (strRecv.Left(5) == _T("TIME:"))//服务器发来的同步时间信息
	{
		strRecv.Delete(0, 5);
		printfMsg(_T("Server Time:") + strRecv);
		CString CDateTimes;
		CDateTimes = strRecv;
		GetDlgItem(IDC_STATIC_TIME)->SetWindowText(CDateTimes);		
	}
	else if (strRecv.Left(5) == _T("LOOP:"))//服务器发来的闭环控制数据（第一个客户端）
	{
		strRecv.Delete(0, 5);
		printfMsg(_T("Server Loop First:") + strRecv);
		CString strLoop;
		strLoop = _T("AUTO:") + strRecv;
		m_clientSocket->Send(strLoop, strLoop.GetLength());
	}
	else if (strRecv.Left(5) == _T("AUTO:"))//服务器发来的闭环控制数据（第二个客户端）
	{
		strRecv.Delete(0, 5);
		printfMsg(_T("Server Loop Sencond:") + strRecv);
		CString strLoop;
		strLoop = _T("SEND:") + strRecv;
		m_clientSocket->Send(strLoop, strLoop.GetLength());
	}
}

void CTCPClientDlg::OnBnClickedButtonConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_IP.IsEmpty())
	{
		printfMsg(_T("Error : IP is Empty!"));
		return;
	}
	if(m_port.IsEmpty())
	{
		printfMsg(_T("Error : Port is Empty!"));
		return;
	}
	if(m_name.IsEmpty())
	{
		printfMsg(_T("Error : Name is Empty!"));
		return;
	}
	if(m_name == _T("Server") || -1 != m_name.Find(_T(":")) || -1 != m_name.Find(_T("|")))
	{
		printfMsg(_T("Error : Name contains illegal characters!"));
		return;
	}
	m_clientSocket = new NSocket(this);
	if(!AfxSocketInit())
	{
		printfError(_T("Init Faild"));
		return;
	}
	if(!m_clientSocket->Create())
	{
		printfError(_T("Creat Faild"));
		return;
	}
	UpdateData(true);
	UINT nPort = atoi(m_port);
	m_clientSocket->Connect(m_IP, nPort);
	
	
	/////////////////////////////////////
}
	

void CTCPClientDlg::OnConnectSucceed()
{
	CString strName =_T("NAME:") + m_name;

	m_clientSocket->Send(strName, strName.GetLength());

	char szName[256] ={0};
	hostent *pHostent = gethostbyname(szName);//获取本机信息
	char *pszIP = inet_ntoa(*(in_addr*)pHostent->h_addr_list[0]);
	m_localIP = pszIP;

	UpdateData(false);

	GetDlgItem(IDC_EDIT_SEND)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(true);
	GetDlgItem(IDC_LIST_Client)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_IP)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_NAME)->EnableWindow(false);
}


void CTCPClientDlg::OnBnClickedButtonDisconnect()
{
	// TODO: 在此添加控件通知处理程序代码	
	if(m_clientSocket != NULL)
	{
		m_clientSocket->Close();
		delete m_clientSocket;
		m_clientSocket = NULL;
	}
	m_listBox.ResetContent();
	printfMsg(_T("Client has disconnected!"));

	GetDlgItem(IDC_EDIT_SEND)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(false);
	GetDlgItem(IDC_LIST_Client)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_IP)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_NAME)->EnableWindow(true);
}


void CTCPClientDlg::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_send.IsEmpty())
	{
		return;
	}
	m_sendBuf = m_send;
	m_send = "";
	UpdateData(false);

	int nSendNum = m_listBox.GetCurSel();
	m_listBox.GetText(nSendNum, m_sender);
	CString strSend ;
	if(0 == nSendNum && m_sender == _T("Server"))
	{
		strSend = _T("SEND:") + m_sendBuf;
	}
	else
	{
		strSend = _T("SETO:") + m_sender + _T("::") + m_sendBuf;		
	}
	m_clientSocket->Send(strSend,strSend.GetLength());
	
}


void CTCPClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CString CDateTimes;
	CDateTimes = CTime::GetCurrentTime().Format(_T("%Y年%m月%d日\n%H:%M:%S\n%A"));
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(CDateTimes);
	CDialogEx::OnTimer(nIDEvent);
}
