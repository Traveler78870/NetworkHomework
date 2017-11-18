#pragma once

#include "TCPClientDlg.h"

class CTCPClientDlg;

// NSocket ÃüÁîÄ¿±ê

class NSocket : public CAsyncSocket//CAsyncSocket//CSocket
{
public:
	NSocket(CTCPClientDlg *dlg);
	virtual ~NSocket();

private:
	CTCPClientDlg *m_parentDlg;
	
public:
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


