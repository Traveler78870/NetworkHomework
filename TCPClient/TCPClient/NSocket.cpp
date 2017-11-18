// NSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "TCPClient.h"
#include "NSocket.h"


// NSocket

NSocket::NSocket(CTCPClientDlg * dlg)
{
	m_parentDlg = dlg;
}

NSocket::~NSocket()
{
}


// NSocket 成员函数

void NSocket::OnConnect(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(!nErrorCode)
	{
		m_parentDlg->printfMsg(_T("Connecting to server!"));
		m_parentDlg->OnConnectSucceed();
	}
	else
	{
		switch(nErrorCode)
		{
			case WSAEADDRINUSE: 
			m_parentDlg->printfMsg(_T("Connect Faild:The specified address is already in use."));
			break;
			case WSAEADDRNOTAVAIL: 
			m_parentDlg->printfMsg(_T("Connect Faild:The specified address is not available from the local machine.\n"));
			break;
			case WSAEAFNOSUPPORT: 
			m_parentDlg->printfMsg(_T("Connect Faild:Addresses in the specified family cannot be used with this socket.\n"));
			break;
			case WSAECONNREFUSED: 
			m_parentDlg->printfMsg(_T("Connect Faild:The attempt to connect was forcefully rejected."));
			break;
			case WSAEDESTADDRREQ: 
			m_parentDlg->printfMsg(_T("Connect Faild:A destination address is required."));
			break;
			case WSAEFAULT: 
			m_parentDlg->printfMsg(_T("Connect Faild:The lpSockAddrLen argument is incorrect."));
			break;
			case WSAEINVAL: 
			m_parentDlg->printfMsg(_T("Connect Faild:The socket is already bound to an address."));
			break;
			case WSAEISCONN: 
			m_parentDlg->printfMsg(_T("Connect Faild:The socket is already connected."));
			break;
			case WSAEMFILE: 
			m_parentDlg->printfMsg(_T("Connect Faild:No more file descriptors are available."));
			break;
			case WSAENETUNREACH: 
			m_parentDlg->printfMsg(_T("Connect Faild:The network cannot be reached from this host at this time."));
			break;
			case WSAENOBUFS: 
			m_parentDlg->printfMsg(_T("Connect Faild:No buffer space is available. The socket cannot be connected."));
			break;
			case WSAENOTCONN: 
			m_parentDlg->printfMsg(_T("Connect Faild:The socket is not connected."));
			break;
			case WSAENOTSOCK: 
			m_parentDlg->printfMsg(_T("Connect Faild:The descriptor is a file, not a socket."));
			break;
			case WSAETIMEDOUT: 
			m_parentDlg->printfMsg(_T("Connect Faild:The attempt to connect timed out without establishing a connection. "));
			break;
			default:
			TCHAR szError[256];
			_stprintf_s(szError, _T("Connect Faild: %d"), nErrorCode);
			m_parentDlg->printfMsg(szError);
			break;
		}
	}
	CAsyncSocket::OnConnect(nErrorCode);
}
 
void NSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_parentDlg->onReceive();
	CAsyncSocket::OnReceive(nErrorCode);
}


void NSocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_parentDlg->onClose();
	CAsyncSocket::OnClose(nErrorCode);
}
