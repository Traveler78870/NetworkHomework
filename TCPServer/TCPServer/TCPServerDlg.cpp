
// TCPServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TCPServer.h"
#include "TCPServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTCPServerDlg 对话框


CTCPServerDlg::CTCPServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTCPServerDlg::IDD, pParent)
	, m_IP(_T(""))
	, m_port(_T("8888"))
	, m_msg(_T(""))
	, m_send(_T(""))
	, m_bSendAll(FALSE)
	, m_ByteNum10(FALSE)
	, m_ByteNum20(FALSE)
	, m_ByteNum30(FALSE)
	, m_ByteNum40(FALSE)
	, m_ByteNum50(FALSE)
{
	m_serverSocket = NULL;
	m_sendBuf = m_send;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_IP, m_IP);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDX_Text(pDX, IDC_EDIT_MSG, m_msg);
	DDX_Control(pDX, IDC_EDIT_MSG, m_editMsg);
	DDX_Control(pDX, IDC_LIST_CLIENT, m_listBox);
	DDX_Text(pDX, IDC_EDIT_SEND, m_send);
	DDX_Check(pDX, IDC_CHECK1, m_bSendAll);
	DDX_Control(pDX, IDC_STATIC_POINT, m_static);
	DDX_Check(pDX, IDC_CHECK2, m_ByteNum10);
	DDX_Check(pDX, IDC_CHECK3, m_ByteNum20);
	DDX_Check(pDX, IDC_CHECK4, m_ByteNum30);
	DDX_Check(pDX, IDC_CHECK5, m_ByteNum40);
	DDX_Check(pDX, IDC_CHECK6, m_ByteNum50);
	DDX_Control(pDX, ID_Timu, m_static1);
	DDX_Text(pDX, IDC_EDIT_SEND2, m_loopcontrol);
}

BEGIN_MESSAGE_MAP(CTCPServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CTCPServerDlg::OnBnClickedButtonStart)
	//ON_MESSAGE(ACCEPTTHREAD_CLOSE,acceptThreadProc)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CTCPServerDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CTCPServerDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_CHECK1, &CTCPServerDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CTCPServerDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CTCPServerDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CTCPServerDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &CTCPServerDlg::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK6, &CTCPServerDlg::OnBnClickedCheck6)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SEND3, &CTCPServerDlg::OnBnClickedButtonSend3)
	ON_BN_CLICKED(IDC_BUTTON_SEND_REGULAR, &CTCPServerDlg::OnBnClickedButtonSendRegular)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_LOOP_SEND, &CTCPServerDlg::OnBnClickedButtonLoopSend)
END_MESSAGE_MAP()


// CTCPServerDlg 消息处理程序

BOOL CTCPServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_Font.CreatePointFont(200, "Arial", NULL);
	m_static1.SetFont(&m_Font, true);
	m_static1.SetWindowTextA("基于以太网的通信协议设计");

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_SEND)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(false);
	GetDlgItem(IDC_LIST_CLIENT)->EnableWindow(false);
	GetDlgItem(IDC_CHECK1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(true);
	SetTimer(1, 100, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTCPServerDlg::OnPaint()
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
	DrawWave();

}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTCPServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTCPServerDlg::printfMsg(CString str )
{
	UpdateData(true);
	m_msg += str;
	m_msg += "\r\n";
	UpdateData(false);
	m_editMsg.LineScroll(m_editMsg.GetLineCount() - 1,0);
}

void CTCPServerDlg::printfError(CString str)
{
	CString strErr;
	strErr.Format(_T("%d"),GetLastError());
	str = _T("Error : ")  + str + _T(":") + strErr;
	printfMsg(str);
}

void CTCPServerDlg::updateListForClient()
{
	CString clientList;
	for(int i = 0;i < m_listBox.GetCount();i++)
	{
		CString client;
		m_listBox.GetText(i,client);
		clientList += client + _T("||");
	}
	clientList = _T("LIST:") + clientList;
	for(int i = 0;i < m_socketList.GetCount();i++)
	{
		POSITION pos = m_socketList.FindIndex(i);
		NSocket *sendSocket = m_socketList.GetAt(pos);
		sendSocket->Send(clientList, clientList.GetLength());
	}
}

void CTCPServerDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_port.IsEmpty())
	{
		printfMsg(_T("Error : Port is Empty!"));
		return;
	}
	char szName[256] ={0};
	hostent *pHostent = gethostbyname(szName);//获取本机信息
	char *pszIP = inet_ntoa(*(in_addr*)pHostent->h_addr_list[0]);
	m_IP = pszIP;
	UpdateData(false);

	m_serverSocket = new NSocket(this);
	if(!AfxSocketInit())
	{
		delete m_serverSocket;
		m_serverSocket = NULL;
		printfError("Init Faild");
		return;
	}
	if(!m_serverSocket->Socket())
	{
		delete m_serverSocket;
		m_serverSocket = NULL;
		printfError("Creat Faild");
		return;
	}
	BOOL boptValue = false;
	int optValueLen = sizeof(boptValue);
	m_serverSocket->SetSockOpt(SO_REUSEADDR, (void *)&boptValue, optValueLen, SOL_SOCKET);

	//UINT nPort = atoi(m_port);
	if(!m_serverSocket->Bind(atoi(m_port)))
	{
		m_serverSocket->Close();
		delete m_serverSocket;
		m_serverSocket = NULL;
		printfError("Bind Faild");
		return;
	}
	if(!m_serverSocket->Listen())
	{
		m_serverSocket->Close();
		delete m_serverSocket;
		m_serverSocket = NULL;
		printfError("Listen Faild");
		return;
	}	
	printfMsg(_T("Server has been started."));
	printfMsg(_T("Waiting for client ..."));
	
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_SEND)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(true);
	GetDlgItem(IDC_LIST_CLIENT)->EnableWindow(true);
	GetDlgItem(IDC_CHECK1)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(false);	
}

void CTCPServerDlg::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(true);
	if(m_send.IsEmpty())
	{
		return;
	}
	//将信息放到缓冲区，并清空控件内容
	m_sendBuf = m_send;
	m_send = "";
	UpdateData(false);
	CString strSend = _T("SEND:") + m_sendBuf;

	if (m_bSendAll)
	{
		for(int i = 0;i < m_socketList.GetCount();i++)
		{
			POSITION pos = m_socketList.FindIndex(i);
			NSocket *sendSocket = m_socketList.GetAt(pos);
			sendSocket->Send(strSend, strSend.GetLength());
		}
	}
	else
	{
		CString strSendName;
		m_listBox.GetText(m_listBox.GetCurSel(), strSendName);
		for(int i =0 ;i < m_socketList.GetCount();i++)
		{
			POSITION pos = m_socketList.FindIndex(i);
			NSocket *sendSocket = m_socketList.GetAt(pos);
			if(strSendName == sendSocket->GetClientName())
			{
				sendSocket->Send(strSend, strSend.GetLength());
				return;
			}
		}
	}
}

void CTCPServerDlg::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_serverSocket != NULL)
	{
		m_serverSocket->Close();
		delete m_serverSocket;
		m_serverSocket = NULL;

		printfMsg(_T("Server has been closed."));

		for(int i = 0;i < m_socketList.GetCount();i++)
		{
			POSITION pos = m_socketList.FindIndex(i);
			NSocket *tmpsocket = m_socketList.GetAt(pos);
			m_socketList.RemoveAt(pos);
			if(tmpsocket != NULL)
			{
				tmpsocket->Close();
				delete tmpsocket;
				tmpsocket = NULL;			
			}		
		}
		m_listBox.ResetContent();

		GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_SEND)->EnableWindow(false);
		GetDlgItem(IDC_CHECK1)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(false);
		GetDlgItem(IDC_LIST_CLIENT)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(true);
	}
}

void CTCPServerDlg::onAccept()
{
	//收到连接后保存socket指针
	NSocket *m_recvSocket = new NSocket(this);
	if(!m_serverSocket->Accept(*m_recvSocket))
	{
		delete m_recvSocket;
		m_recvSocket = NULL;
		printfError("Accept Faild");
		return;
	}
	m_socketList.AddTail(m_recvSocket);
}

void CTCPServerDlg::onReceive(NSocket *recvSocket)
{
	CString strRecv;
	unsigned char szRecv[256] = {0};
	recvSocket->Receive(szRecv,sizeof(szRecv));
	strRecv = szRecv;
	CString recvName = recvSocket->GetClientName();
	//printf("%s:%s\n\r",recvName,strRecv);
	if(strRecv.Left(5) == _T("NAME:"))//客户端发过来的名字
	{
		strRecv.Delete(0,5);
		BOOL bSame = false;
		for(int i = 0;i < m_listBox.GetCount();i++)
		{
			CString listText;
			m_listBox.GetText(i,listText);
			if(listText == strRecv)
				bSame = true;
			break;
		}
		if(bSame)//服务器发出名字有冲突提示
		{
			CString strReturn(_T("ERRO:Name repetition"));
			recvSocket->Send(strReturn,strReturn.GetLength());
			return;
		}
		m_listBox.AddString(strRecv);
		printfMsg(strRecv + _T(" connected."));
		recvSocket->SetClientName(strRecv);
		if(-1 == m_listBox.GetCurSel())
		{
			m_listBox.SetCurSel(0);
			GetDlgItem(IDC_LIST_CLIENT)->EnableWindow(true);
			GetDlgItem(IDC_CHECK1)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(true);
			GetDlgItem(IDC_EDIT_SEND)->EnableWindow(true);
		}
		updateListForClient();	
	}
	else if(strRecv.Left(5) == _T("RETU:"))//服务器发出信息后，收到的反馈
	{
		strRecv.Delete(0,5);
		if(strRecv == m_sendBuf)
		{
			printfMsg(_T("SEND TO ") + recvName + _T(":") + m_sendBuf + _T(" ...√"));
		}
	}
	else if(strRecv.Left(5) == _T("SEND:"))//服务器接收到的信息
	{
		for (int temp = 0; temp < 256; temp++)
			m_RecvValues[temp] = 0;
		unsigned char Recv_temp[256] = { 0 };
		for (int temp = 0; temp < 200; temp++)
			Recv_temp[temp] = szRecv[5 + temp];
		int i = strlen((char *)Recv_temp);
		//解析收到的数据
		for (int j = 0, k = 0, l = 0; j < i; j++)
		{
			if ((Recv_temp[j] == 32))
			{
				if ((j - l) == 3)
				{
					m_RecvValues[k] = (Recv_temp[j - 1] - 48) + (Recv_temp[j - 2] - 48) * 10 + (Recv_temp[j - 3] - 48) * 100;
					k++;
				}
				else if ((j - l) == 2)
				{
					m_RecvValues[k] = (Recv_temp[j - 1] - 48) + (Recv_temp[j - 2] - 48) * 10;
					k++;
				}
				else if ((j - l) == 1)
				{
					m_RecvValues[k] = (Recv_temp[j - 1] - 48);
					k++;
				}
				l = j + 1;
			}
			if (j == i - 1)
			{
				if ((j - l) == 2)
				{
					m_RecvValues[k] = (Recv_temp[j] - 48) + (Recv_temp[j - 1] - 48) * 10 + (Recv_temp[j - 2] - 48) * 100;
					k++;
				}
				else if ((j - l) == 1)
				{
					m_RecvValues[k] = (Recv_temp[j] - 48) + (Recv_temp[j - 1] - 48) * 10;
					k++;
				}
				else if ((j - l) == 0)
				{
					m_RecvValues[k] = (Recv_temp[j] - 48);
					k++;
				}
			}
		}
		Draw_Flag = 1;
		DrawWave();
		strRecv.Delete(0,5);
		CString Name = recvSocket->GetClientName();
		printfMsg(Name + _T(":") + strRecv);
		CString strReturn(_T("RETU:") + strRecv);
		recvSocket->Send(strReturn, strReturn.GetLength());
	}
	else if(strRecv.Left(5) == _T("SETO:"))//服务器接收到需要转发到其它客户端的信息
	{
		strRecv.Delete(0,5);
		int n = strRecv.Find(_T("::"));
		CString recvName = strRecv.Left(n);
		strRecv.Delete(0,n + 2);
		CString senderName = recvSocket->GetClientName();
		printfMsg(senderName + _T(" to ") + recvName + _T(":") + strRecv);
		for(int i = 0;i < m_socketList.GetCount();i++)
		{
			POSITION pos = m_socketList.FindIndex(i);
			NSocket *sendSocket =  m_socketList.GetAt(pos);
			if(recvName == sendSocket->GetClientName())
			{
				CString str = _T("SETO:") + senderName + _T("::") + strRecv;
				sendSocket->Send(str,str.GetLength());
				break;
			}
		}
	}
	else if(strRecv.Left(5) == _T("SERE:"))//服务器收到转发信息后接收端的反馈（将反馈转发给发送端）
	{
		strRecv.Delete(0,5);
		int n = strRecv.Find(_T("::"));
		CString recvName = strRecv.Left(n);
		strRecv.Delete(0, n + 2);
		for(int i = 0;i < m_socketList.GetCount();i++)
		{
			POSITION pos = m_socketList.FindIndex(i);
			NSocket *sendSocket =  m_socketList.GetAt(pos);
			if(recvName == sendSocket->GetClientName())
			{
				CString strReturn(_T("RETU:") + strRecv);
				sendSocket->Send(strReturn, strReturn.GetLength());
				break;
			}
		}
	}
	else if (strRecv.Left(5) == _T("AUTO:"))//服务器转发闭环控制数据
	{
		strRecv.Delete(0, 5);
		CString strLoop;
		strLoop = _T("AUTO:") + strRecv;
		POSITION pos = m_socketList.FindIndex(1);
		NSocket *sendSocket = m_socketList.GetAt(pos);
		sendSocket->Send(strLoop, strLoop.GetLength());
	}
}

void CTCPServerDlg::onClose(NSocket *recvSocket)
{
	//删除listBox中的用户名
	CString name = recvSocket->GetClientName();
	if(!name.IsEmpty())
	{
		int num = m_listBox.FindString(-1, name);
		m_listBox.DeleteString(num);
		if(-1 == m_listBox.GetCount())
		{
			GetDlgItem(IDC_LIST_CLIENT)->EnableWindow(false);
			GetDlgItem(IDC_CHECK1)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_SEND)->EnableWindow(false);
		}
		else if(-1 == m_listBox.GetCurSel())
		{
			m_listBox.SetCurSel(0);
		}
		updateListForClient();
		printfMsg(name + _T(" has disconnected!"));	
	}
	
	//移除socketlist中的socket
	for(int i = 0;i < m_socketList.GetCount();i++)
	{
		POSITION pos = m_socketList.FindIndex(i);
		if(recvSocket == m_socketList.GetAt(pos))
		{
			m_socketList.RemoveAt(pos);
			break;
		}
	}
	recvSocket->Close();
	
	delete recvSocket;
	recvSocket = NULL;

}


void CTCPServerDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	//选择发送到全部是禁止listbox的使用
	UpdateData(true);
	if(m_bSendAll)
	{
		m_listBox.SetCurSel(-1);
		GetDlgItem(IDC_LIST_CLIENT)->EnableWindow(false);
	}
	else
	{
		m_listBox.SetCurSel(0);
		GetDlgItem(IDC_LIST_CLIENT)->EnableWindow(true);
	}
}

void CTCPServerDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	//选择发送到全部是禁止listbox的使用
	UpdateData(true);
	//CButton *ptn2 = (CButton *)GetDlgItem(IDC_CHECK2);
	//ptn2->SetCheck(1);
	CButton *ptn3 = (CButton *)GetDlgItem(IDC_CHECK3);
	ptn3->SetCheck(0);
	CButton *ptn4 = (CButton *)GetDlgItem(IDC_CHECK4);
	ptn4->SetCheck(0);
	CButton *ptn5 = (CButton *)GetDlgItem(IDC_CHECK5);
	ptn5->SetCheck(0);
	CButton *ptn6 = (CButton *)GetDlgItem(IDC_CHECK6);
	ptn6->SetCheck(0);

	//m_ByteNum10 = 1;
	m_ByteNum20 = 0;
	m_ByteNum30 = 0;
	m_ByteNum40 = 0;
	m_ByteNum50 = 0;
	OnBnClickedButtonSendRegular();
}


void CTCPServerDlg::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	//选择发送到全部是禁止listbox的使用
	UpdateData(true);
	CButton *ptn2 = (CButton *)GetDlgItem(IDC_CHECK2);
	ptn2->SetCheck(0);
	//CButton *ptn3 = (CButton *)GetDlgItem(IDC_CHECK3);
	//ptn3->SetCheck(1);
	CButton *ptn4 = (CButton *)GetDlgItem(IDC_CHECK4);
	ptn4->SetCheck(0);
	CButton *ptn5 = (CButton *)GetDlgItem(IDC_CHECK5);
	ptn5->SetCheck(0);
	CButton *ptn6 = (CButton *)GetDlgItem(IDC_CHECK6);
	ptn6->SetCheck(0);
	m_ByteNum10 = 0;
	//m_ByteNum20 = 1;
	m_ByteNum30 = 0;
	m_ByteNum40 = 0;
	m_ByteNum50 = 0;
	OnBnClickedButtonSendRegular();
}


void CTCPServerDlg::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
	//选择发送到全部是禁止listbox的使用
	UpdateData(true);
	CButton *ptn2 = (CButton *)GetDlgItem(IDC_CHECK2);
	ptn2->SetCheck(0);
	CButton *ptn3 = (CButton *)GetDlgItem(IDC_CHECK3);
	ptn3->SetCheck(0);
	//CButton *ptn4 = (CButton *)GetDlgItem(IDC_CHECK4);
	//ptn4->SetCheck(1);
	CButton *ptn5 = (CButton *)GetDlgItem(IDC_CHECK5);
	ptn5->SetCheck(0);
	CButton *ptn6 = (CButton *)GetDlgItem(IDC_CHECK6);
	ptn6->SetCheck(0);
	m_ByteNum10 = 0;
	m_ByteNum20 = 0;
	//m_ByteNum30 = 1;
	m_ByteNum40 = 0;
	m_ByteNum50 = 0;
	OnBnClickedButtonSendRegular();
}


void CTCPServerDlg::OnBnClickedCheck5()
{
	// TODO: 在此添加控件通知处理程序代码
	//选择发送到全部是禁止listbox的使用
	UpdateData(true);
	CButton *ptn2 = (CButton *)GetDlgItem(IDC_CHECK2);
	ptn2->SetCheck(0);
	CButton *ptn3 = (CButton *)GetDlgItem(IDC_CHECK3);
	ptn3->SetCheck(0);
	CButton *ptn4 = (CButton *)GetDlgItem(IDC_CHECK4);
	ptn4->SetCheck(0);
	//CButton *ptn5 = (CButton *)GetDlgItem(IDC_CHECK5);
	//ptn5->SetCheck(1);
	CButton *ptn6 = (CButton *)GetDlgItem(IDC_CHECK6);
	ptn6->SetCheck(0);
	m_ByteNum10 = 0;
	m_ByteNum20 = 0;
	m_ByteNum30 = 0;
	//m_ByteNum40 = 1;
	m_ByteNum50 = 0;
	OnBnClickedButtonSendRegular();
}


void CTCPServerDlg::OnBnClickedCheck6()
{
	// TODO: 在此添加控件通知处理程序代码
	//选择发送到全部是禁止listbox的使用
	UpdateData(true);
	CButton *ptn2 = (CButton *)GetDlgItem(IDC_CHECK2);
	ptn2->SetCheck(0);
	CButton *ptn3 = (CButton *)GetDlgItem(IDC_CHECK3);
	ptn3->SetCheck(0);
	CButton *ptn4 = (CButton *)GetDlgItem(IDC_CHECK4);
	ptn4->SetCheck(0);
	CButton *ptn5 = (CButton *)GetDlgItem(IDC_CHECK5);
	ptn5->SetCheck(0);
	//CButton *ptn6 = (CButton *)GetDlgItem(IDC_CHECK6);
	//ptn6->SetCheck(1);
	m_ByteNum10 = 0;
	m_ByteNum20 = 0;
	m_ByteNum30 = 0;
	m_ByteNum40 = 0;
	//m_ByteNum50 = 1;

	OnBnClickedButtonSendRegular();
	Sleep(1000);
}


void CTCPServerDlg::DrawWave()
{
	CFont font;
	font.CreateFont(13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, NULL);
	CWnd *pWnd = GetDlgItem(IDC_STATIC_POINT);
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	CFont *pOldFont;
	CDC *pDC = pWnd->GetDC();
	int X_axis_value = 440;
	int Y_axis_value = 300;

	//绘制矩形画框
	pDC->Rectangle(0, 0, X_axis_value, Y_axis_value);
	//更换字体
	pOldFont = pDC->SelectObject(&font);

	pDC->TextOut(2, 2, _T("绘制数据"));
	//创建画笔对象
	CPen *pPenRed = new CPen();
	CPen *pPenBlackDotted = new CPen();
	CPen *pPenBlackFull = new CPen();
	pPenBlackDotted->CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	pPenRed->CreatePen(PS_SOLID, 2, RGB(0, 0, 255)); //蓝色画笔
	pPenBlackFull->CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	//选中当前红色画笔,并保存以前的画笔
	CGdiObject *pOldPen = pDC->SelectObject(pPenRed);
	//MoveTo()函数把笔移动要画的起始位置固定；
	//然后要用到LineTo()函数确定终止位置。
	//这样一条线就画出来了。
	pDC->MoveTo(20, 20); //绘制坐标轴
	pDC->LineTo(20, Y_axis_value - 20); //竖起轴//20,280是原点
	pDC->LineTo(X_axis_value - 20, Y_axis_value - 20); //水平轴

	pDC->MoveTo(17, 24);//y轴画箭头
	pDC->LineTo(20, 20);
	pDC->LineTo(23, 24);
	pDC->MoveTo(X_axis_value - 23, Y_axis_value - 23);//x轴画箭头
	pDC->LineTo(X_axis_value - 20, Y_axis_value - 20);
	pDC->LineTo(X_axis_value - 23, Y_axis_value - 17);

	pDC->TextOut(27, 20, _T("数据"));
	pDC->TextOut(X_axis_value - 50, Y_axis_value - 40, _T("个数"));


	CString yText[6] = { _T("300"), _T("250"), _T("200"), _T("150"), _T("100"), _T("50") };
	pDC->SelectObject(pPenBlackDotted);    //换笔
	for (int i = 0; i < 6; i++)
	{
		pDC->TextOut(5, 50 + i * 40, yText[i]);
		pDC->MoveTo(20, 50 + i * 40);
		pDC->LineTo(X_axis_value, 50 + i * 40);
	}

	CString sPIText[10] = { _T("10"), _T("20"), _T("30"), _T("40"), _T("50"), _T("60"), _T("70"), _T("80"), _T("90"), _T("100") };
	for (int i = 0; i < 10; i++)
	{
		pDC->TextOut(60 + i * 40 - 5, 280, sPIText[i]);
		pDC->MoveTo(60 + i * 40, 20);
		pDC->LineTo(60 + i * 40, 280);
	}

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);

	pOldPen->DeleteObject();
	pPenBlackDotted->DeleteObject();

	CRect rectStatic;
	m_static.GetClientRect(&rectStatic);
	float fDeltaX; // x轴相邻两个绘图点的坐标距离
	float fDeltaY; // y轴每个逻辑单位对应的坐标值
	int pX, pY;

	int ddj = rectStatic.Width();
	int dll = rectStatic.Height();
	fDeltaX = (float)rectStatic.Width() / 100;
	fDeltaY = (float)rectStatic.Height() / 300;
	//显示数据
	pDC->SelectObject(pPenBlackFull);
	pDC->MoveTo(20, 280);
	int num = 0;
	if (Draw_Flag == 1)
	{
		Draw_Flag = 0;
		num = strlen((char *)m_RecvValues);
		for (int i = 0; i < num; i++)
		{
			m_nzValues[Point_Count / 100][(Point_Count) % 100] = m_RecvValues[i];
			Point_Count++;
		}
	}

	if (Point_Count > 0 && Point_Count % 100 == 0)
	{
		num = 100;
	}
	else
	{
		num = Point_Count % 100;
	}
	int k = 0;
	int temp = 0;
	if ((Point_Count / 100) - (Pre_Point_Count / 100) > 0)
	{  
		for (k = 0; k < 100; k++)
		{
			//pX = rectStatic.left + (int)(i * fDeltaX);
			pX = 20 + (k + 1) * 4;
			pY = 280 - (int)(m_nzValues[Pre_Point_Count / 100][k] * fDeltaY);
			//pY =m_nzValues[i];
			pDC->LineTo(pX, pY);
		}

		pDC->MoveTo(20, 280);
		for (k = 0; k < Point_Count % 100; k++)
		{
			//pX = rectStatic.left + (int)(i * fDeltaX);
			pX = 20 + (k + 1) * 4;
			pY = 280 - (int)(m_nzValues[Point_Count / 100][k] * fDeltaY);
			//pY =m_nzValues[i];
			pDC->LineTo(pX, pY);
		}
	}
	else
	{
		for (k = 0; k < num; k++)
		{
			//pX = rectStatic.left + (int)(i * fDeltaX);
			pX = 20 + (k + 1) * 4;
			pY = 280 - (int)(m_nzValues[Pre_Point_Count / 100][k] * fDeltaY);
			//pY =m_nzValues[i];
			pDC->LineTo(pX, pY);
		}
	}
	Pre_Point_Count = Point_Count;
	
	pPenBlackFull->DeleteObject();
}


void CTCPServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CString CDateTimes;
	CDateTimes = CTime::GetCurrentTime().Format(_T("%Y年%m月%d日\n%H:%M:%S\n%A"));
	GetDlgItem(IDC_STATIC_IP2)->SetWindowText(CDateTimes);
	if (m_ByteNum10 || m_ByteNum20 || m_ByteNum30 || m_ByteNum40 || m_ByteNum50)
	{
		if (Count_Send_Regular++ % 10 == 0)
		{
			OnBnClickedButtonSendRegular();
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CTCPServerDlg::OnBnClickedButtonSend3()
{
	// TODO:  在此添加控件通知处理程序代码
	unsigned char SendValues[200] = { 0 };
	int i = 0;
	int temp = 0;
	int length = 0;
	CString StrTemp;
	CString CDateTimes;
	CDateTimes = CTime::GetCurrentTime().Format(_T("%Y年%m月%d日\n%H:%M:%S\n%A"));
	CString strTemp = _T("TIME:") + CDateTimes;

	for (int i = 0; i < m_socketList.GetCount(); i++)
	{
		POSITION pos = m_socketList.FindIndex(i);
		NSocket *sendSocket = m_socketList.GetAt(pos);
		sendSocket->Send(strTemp, strTemp.GetLength());
	}
}


void CTCPServerDlg::OnBnClickedButtonSendRegular()
{
	// TODO:  在此添加控件通知处理程序代码
	int SendByteNum = 0;
	int i = 0;
	int temp = 0;
	int length = 0;
	unsigned char SendValues[1024] = { 0 };

	if (m_ByteNum10)
	{
		SendByteNum = 10;
	}
	else if (m_ByteNum20)
	{
		SendByteNum = 20;
	}
	else if (m_ByteNum30)
	{
		SendByteNum = 30;
	}
	else if (m_ByteNum40)
	{
		SendByteNum = 40;
	}
	else if (m_ByteNum50)
	{
		SendByteNum = 50;
	}
	//只有使能发送字节数为10、20、30、40、50时，才发送数据
	if (m_ByteNum10 || m_ByteNum20 || m_ByteNum30 || m_ByteNum40 || m_ByteNum50)
	{
		SendValues[0] = 'R';
		SendValues[1] = 'E';
		SendValues[2] = 'G';
		SendValues[3] = 'U';
		SendValues[4] = ':';
		for (i = 0; i < SendByteNum; i++)
		{
			temp = (rand() + 1) % 255;
			length = strlen((char *)SendValues);
			sprintf((char *)&SendValues[length], "%d ", temp);
		}

		for (int i = 0; i < m_socketList.GetCount(); i++)
		{
			POSITION pos = m_socketList.FindIndex(i);
			NSocket *sendSocket = m_socketList.GetAt(pos);
			sendSocket->Send(SendValues, strlen((char *)SendValues));
		}
	}
}


HBRUSH CTCPServerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	if (ID_Timu == pWnd->GetDlgCtrlID())//判断发出消息的空间是否是该静态文本框
	{
	       pDC->SetTextColor(RGB(255, 0, 0));//设置文本颜色为红色
	       pDC->SetBkColor(RGB(0, 0, 255));//设置文本背景为蓝色
	       //hbr = CreateSolidBrush(RGB(0, 255, 0));//控件的背景色为绿色
	 }

	return hbr;
}


void CTCPServerDlg::OnBnClickedButtonLoopSend()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	if (m_loopcontrol.IsEmpty())
	{
		printfMsg(_T("Error :Loop control is Empty!"));
		return;
	}
	if (m_socketList.GetCount() < 2)
	{
		printfMsg(_T("Error :Client num < 2!"));
		return;
	}
	CString strSend = _T("LOOP:") + m_loopcontrol;

	POSITION pos = m_socketList.FindIndex(0);
	NSocket *sendSocket = m_socketList.GetAt(pos);
	sendSocket->Send(strSend, strSend.GetLength());
}
