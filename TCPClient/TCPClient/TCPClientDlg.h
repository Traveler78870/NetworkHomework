
// TCPClientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "NSocket.h"

class NSocket;


// CTCPClientDlg 对话框
class CTCPClientDlg : public CDialogEx
{
// 构造
public:
	CTCPClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TCPCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CString m_IP;
	CString m_port;
	CString m_msg;
	CString m_localIP;
	CString m_send;//控件值
	CString m_sendBuf;//发送值缓冲（等待返回比较）
	CString m_sender;//保存消息接收者
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
