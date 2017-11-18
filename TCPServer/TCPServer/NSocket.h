#pragma once
#include "TCPServerDlg.h"

class CTCPServerDlg;

// NSocket ����Ŀ��

class NSocket : public CAsyncSocket//CAsyncSocket // CSocket
{
public:
	NSocket(CTCPServerDlg *);
	virtual ~NSocket();
	
	const CString& GetClientName(); 
	void SetClientName(CString &);

private:
	CTCPServerDlg *m_parentDlg;//����Ի���ָ��
	CString m_clientName;//����ͻ�������

public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


