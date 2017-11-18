// NSocket.cpp : ʵ���ļ�
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


// NSocket ��Ա����

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
	// TODO: �ڴ����ר�ô����/����û���
	m_parentDlg->onAccept();
	CAsyncSocket::OnAccept(nErrorCode);
}


void NSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	m_parentDlg->onReceive(this);
	CAsyncSocket::OnReceive(nErrorCode);
}


void NSocket::OnClose(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	m_parentDlg->onClose(this);
	CAsyncSocket::OnClose(nErrorCode);
}
