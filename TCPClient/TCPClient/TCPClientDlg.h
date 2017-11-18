
// TCPClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "NSocket.h"

class NSocket;


// CTCPClientDlg �Ի���
class CTCPClientDlg : public CDialogEx
{
// ����
public:
	CTCPClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TCPCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CString m_IP;
	CString m_port;
	CString m_msg;
	CString m_localIP;
	CString m_send;//�ؼ�ֵ
	CString m_sendBuf;//����ֵ���壨�ȴ����رȽϣ�
	CString m_sender;//������Ϣ������
	CEdit m_editMsg;
	CString m_name;
	CListBox m_listBox;

	NSocket *m_clientSocket;

	
	
public:
	void printfMsg(CString);
	void printfError(CString);
	void OnConnectSucceed();
	void onReceive();
	void onClose();

	afx_msg void OnBnClickedButtonConnect();	
	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg void OnBnClickedButtonSend();
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
