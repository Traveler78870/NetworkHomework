
// TCPServerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "NSocket.h"
#define POINT_COUNT 256
class NSocket;

class RECVPARAM  
{
public:
	RECVPARAM(){}
	virtual ~RECVPARAM(){}
	CSocket *m_recvSocket;//���յ��Ŀͻ��˵��׽���
	CSocket *m_serverSocket;//�������˸����������׽���
	SOCKET m_pSOCKET;
	HWND m_hwnd;//���������ڵľ��
};
/*
class ClientInfo
{
public:
	ClientInfo(){}
	~ClientInfo(){}
	NSocket *
}*/

// CTCPServerDlg �Ի���
class CTCPServerDlg : public CDialogEx
{
// ����
public:
	CTCPServerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	

// �Ի�������
	enum { IDD = IDD_TCPSERVER_DIALOG };

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
	CEdit m_editMsg;
	CListBox m_listBox;
	BOOL m_bSendAll;
	CString m_send;//�ؼ�ֵ
	CString m_sendBuf;//����ֵ���壨�ȴ����رȽϣ�
	CFont m_Font;
	


	NSocket *m_serverSocket;//������socket
	CList<NSocket*> m_socketList;//��ſͻ���socket�б�

	//��ӡ��Ϣ
	void printfMsg(CString);
	
	//���¿ͻ��˵��û��б�
	void updateListForClient();

public:
	//��ӡ����
	void printfError(CString);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedCheck1();
	void onAccept();
	void onReceive(NSocket *);
	void onClose(NSocket *);
public:
	CStatic m_static;
	unsigned char Draw_Flag;
	void DrawWave();
	unsigned char m_RecvValues[POINT_COUNT];
	int m_nzValues[256][POINT_COUNT];
	int Point_Count;
	int Pre_Point_Count;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL m_ByteNum10;
	BOOL m_ByteNum20;
	BOOL m_ByteNum30;
	BOOL m_ByteNum40;
	BOOL m_ByteNum50;
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedButtonSend3();
	afx_msg void OnBnClickedButtonSendRegular();
	unsigned long long Count_Send_Regular;
	CStatic m_static1;
	CStatic m_static2;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString m_loopcontrol;
	afx_msg void OnBnClickedButtonLoopSend();
};
