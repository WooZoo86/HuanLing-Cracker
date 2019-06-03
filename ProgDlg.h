
// ProgDlg.h : ͷ�ļ�
//

#pragma once

#include "MyBits.h"
#include "FreeImage.h"
#include "afxcmn.h"
#include "afxwin.h"
#define MEM_MB 12

struct index_unit
{
	unsigned long id;
	unsigned long pos;
	unsigned long length;
	unsigned long space;
};

// CProgDlg �Ի���
class CProgDlg : public CDialog
{
// ����
public:
	CProgDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HUANLINGCRACKER_DIALOG };

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
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
private:
	CString SrcPath;
	CString Root;
	void proc_file(CString file_name);
	MyBits Bits;
	unsigned char *buffer;
	void LoopImage(CString s);
	void OneFile(CString DestFile);
	double Hue2RGB(double v1, double v2, double vH);
public:
	CSliderCtrl sl_H;
	CSliderCtrl sl_S;
	CSliderCtrl sl_L;
	CEdit ed_H;
	CEdit ed_S;
	CEdit ed_L;
	CSpinButtonCtrl sp_H;
	CSpinButtonCtrl sp_S;
	CSpinButtonCtrl sp_L;
};
