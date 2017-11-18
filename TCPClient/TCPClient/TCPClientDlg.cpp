
// TCPClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TCPClient.h"
#include "TCPClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTCPClientDlg �Ի���


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


// CTCPClientDlg ��Ϣ�������

BOOL CTCPClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(IDC_EDIT_SEND)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(false);
	GetDlgItem(IDC_LIST_Client)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_IP)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_NAME)->EnableWindow(true);
	SetTimer(1, 100, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTCPClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	if(strRecv.Left(5) == _T("RETU:"))//�������ݺ�ӵ��ķ���
	{
		strRecv.Delete(0,5);
		if(strRecv == m_sendBuf)
		{
			printfMsg(_T("SEND TO ") + m_sender+_T(" : ") + m_sendBuf + _T(" ...��"));
		}
	}
	else if(strRecv.Left(5) == _T("SEND:"))//����������������
	{
		strRecv.Delete(0,5);
		printfMsg(_T("Server:") + strRecv);
		CString strReturn(_T("RETU:") + strRecv);
		m_clientSocket->Send(strReturn, strReturn.GetLength());
	}
	else if(strRecv.Left(5) == _T("LIST:"))//�����������ģ������û��б�
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
	else if(strRecv.Left(5) == _T("SETO:"))//��Ŀͻ���ͨ������������������
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
	else if(strRecv.Left(5) == _T("ERRO:"))//�����������ģ������ظ���ʾ
	{
		strRecv.Delete(0,5);
		if(_T("Name repetition") == strRecv)
		{
			printfMsg(_T("Server:") + strRecv);
			OnBnClickedButtonDisconnect();
		}
	}
	else if (strRecv.Left(5) == _T("REGU:"))//�����������Ķ�ʱ����
	{
		strRecv.Delete(0, 5);
		printfMsg(_T("Server Regular:") + strRecv);
		CString strReturn(_T("SEND:") + strRecv);
		m_clientSocket->Send(strReturn, strReturn.GetLength());
	}
	else if (strRecv.Left(5) == _T("TIME:"))//������������ͬ��ʱ����Ϣ
	{
		strRecv.Delete(0, 5);
		printfMsg(_T("Server Time:") + strRecv);
		CString CDateTimes;
		CDateTimes = strRecv;
		GetDlgItem(IDC_STATIC_TIME)->SetWindowText(CDateTimes);		
	}
	else if (strRecv.Left(5) == _T("LOOP:"))//�����������ıջ��������ݣ���һ���ͻ��ˣ�
	{
		strRecv.Delete(0, 5);
		printfMsg(_T("Server Loop First:") + strRecv);
		CString strLoop;
		strLoop = _T("AUTO:") + strRecv;
		m_clientSocket->Send(strLoop, strLoop.GetLength());
	}
	else if (strRecv.Left(5) == _T("AUTO:"))//�����������ıջ��������ݣ��ڶ����ͻ��ˣ�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	hostent *pHostent = gethostbyname(szName);//��ȡ������Ϣ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString CDateTimes;
	CDateTimes = CTime::GetCurrentTime().Format(_T("%Y��%m��%d��\n%H:%M:%S\n%A"));
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(CDateTimes);
	CDialogEx::OnTimer(nIDEvent);
}
