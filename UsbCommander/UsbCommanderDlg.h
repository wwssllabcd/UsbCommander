
// UsbCommanderDlg.h : ���Y��
//

#pragma once


// CUsbCommanderDlg ��ܤ��
class CUsbCommanderDlg : public CDialogEx
{
// �غc
public:
	CUsbCommanderDlg(CWnd* pParent = NULL);	// �зǫغc�禡

// ��ܤ�����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USBCOMMANDER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �䴩
    BOOL PreTranslateMessage(MSG* pMsg);
    void init_var();

// �{���X��@
protected:
	HICON m_hIcon;

	// ���ͪ��T�������禡
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnCbnSelchangeCboCmdSelect();
    afx_msg void OnBnClickedBtnRefresh();
    afx_msg void OnBnClickedBtnExecute();
    afx_msg void OnBnClickedbtnseqstart();
    afx_msg void OnBnClickedbtnrdmstart();
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeCboDriveSel();
};
