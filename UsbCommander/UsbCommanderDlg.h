
// UsbCommanderDlg.h : 標頭檔
//

#pragma once


// CUsbCommanderDlg 對話方塊
class CUsbCommanderDlg : public CDialogEx
{
// 建構
public:
	CUsbCommanderDlg(CWnd* pParent = NULL);	// 標準建構函式

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USBCOMMANDER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援
    BOOL PreTranslateMessage(MSG* pMsg);
    void init_var();

// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
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
