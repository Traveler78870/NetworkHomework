#pragma once
#include "TCPServerDlg.h"

class CTCPServerDlg;

// NSocket 命令目标

class NSocket : public CAsyncSocket//CAsyncSocket // CSocket
{
public:
	NSocket(CTCPServerDlg *);
	virtual ~NSocket();
	
	const CString& GetClientName(); 
	void SetClientName(CString &);

private:
	CTCPServerDlg *m_parentDlg;//保存对话框指针
	CString m_clientName;//保存客户端名称

public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


