// ColorSelectorDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CColorSelectorDlg �Ի���
class CColorSelectorDlg : public CDialog
{
// ����
public:
	CColorSelectorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_COLORSELECTOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CEdit m_e;
	afx_msg void OnEnSetfocusEdit3();
	CButton m_b;
	afx_msg void OnClickStatic();
	CStatic m_s;
};