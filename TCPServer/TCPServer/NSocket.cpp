// NSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "TCPServer.h"
#include "NSocket.h"


// NSocket

NSocket::NSocket(CTCPServerDlg * dlg)
{
	m_parentDlg = dlg;
	m_clientName = _T("");
}

NSocket::~NSocket()
{
}


// NSocket 成员函数

const CString& NSocket::GetClientName()
{
	return m_clientName;
}

void NSocket::SetClientName(CString & str)
{
	m_clientName = str;
}

void NSocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_parentDlg->onAccept();
	CAsyncSocket::OnAccept(nErrorCode);
}


void NSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_parentDlg->onReceive(this);
	CAsyncSocket::OnReceive(nErrorCode);
}


void NSocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_parentDlg->onClose(this);
	CAsyncSocket::OnClose(nErrorCode);
}
