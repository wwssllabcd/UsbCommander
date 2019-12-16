
// UsbCommanderDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "UsbCommander.h"
#include "UsbCommanderDlg.h"
#include "afxdialogex.h"

#include "UsbCmder/CmderCtrller.h"
#include "Utility/Observer.h"
#include "Utility/EricException.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUsbCommanderDlg 對話方塊



CUsbCommanderDlg::CUsbCommanderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_USBCOMMANDER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUsbCommanderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUsbCommanderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_CBN_SELCHANGE(IDC_CBO_CMD_SELECT, &CUsbCommanderDlg::OnCbnSelchangeCboCmdSelect)
    ON_BN_CLICKED(IDC_BTN_REFRESH, &CUsbCommanderDlg::OnBnClickedBtnRefresh)
    ON_BN_CLICKED(IDC_BTN_EXECUTE, &CUsbCommanderDlg::OnBnClickedBtnExecute)
    ON_BN_CLICKED(IDC_btnSeqStart, &CUsbCommanderDlg::OnBnClickedbtnseqstart)
    ON_BN_CLICKED(IDC_btnRdmStart, &CUsbCommanderDlg::OnBnClickedbtnrdmstart)
END_MESSAGE_MAP()


CmderCtrller m_ctrler;
void dialogShowMsg(estring& tmsg, bool isClean) {
    m_ctrler.m_view.sendMsgToDialogArea(isClean, tmsg);
}

void CUsbCommanderDlg::init_var(){
    UsbCmderMfcUiNum cmderNumObj;
    for (int i = 0; i < MAX_CDB_OF_UI; i++) {
        cmderNumObj.cdb[i] = IDC_CDB_00 + i;
    }
    
    cmderNumObj.devceSel = IDC_CBO_DRIVE_SEL;
    cmderNumObj.refresh = IDC_BTN_REFRESH;
    cmderNumObj.execute = IDC_BTN_EXECUTE;
    cmderNumObj.cmdSel = IDC_CBO_CMD_SELECT;
    cmderNumObj.dataIn = IDC_RDO_DATA_IN;
    cmderNumObj.dataOut = IDC_RDO_DATA_OUT;
    cmderNumObj.dataLen = IDC_TXT_DATA_LENGTH;
    cmderNumObj.message = IDC_TXT_MAIN_MSG;

    m_ctrler.m_view.setDlgPointer(this);
    m_ctrler.m_view.setup_ui_num_to_view(&cmderNumObj);

    SeqWriteUiNum seqNumObj;
    seqNumObj.startLba = IDC_txtSeqStartLba;
    seqNumObj.endLba = IDC_txtSeqEndLba;
    seqNumObj.secCnt = IDC_txtSeqSecCnt;
    seqNumObj.lbaStep = IDC_txtSeqLbaStep;
    seqNumObj.noRead = IDC_btnNoRead;
    seqNumObj.noWrite = IDC_btnNoWrite;
    seqNumObj.stop = IDC_btnStop;

    m_ctrler.m_view.m_seqWriteUi.set_ui_item(this, &seqNumObj);

    RdmWriteUiNum rdmNumObj;
    rdmNumObj.startLba = IDC_txtSeqStartLba;
    rdmNumObj.endLba = IDC_txtSeqEndLba;
    rdmNumObj.stop = IDC_btnStop;
    rdmNumObj.ramdomSeed = IDC_txtRdmSeed;
    rdmNumObj.stopAt = IDC_txtStopAt;
    rdmNumObj.isPending = IDC_btnRdmIsPending;
    rdmNumObj.isFake = IDC_btnRdmIsFake;
    m_ctrler.m_view.m_rdmWriteUi.set_ui_item(this, &rdmNumObj);

    m_ctrler.init();
    Observer::observerRegister(0, &dialogShowMsg);
}
// CUsbCommanderDlg 訊息處理常式

BOOL CUsbCommanderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	ShowWindow(SW_SHOWNORMAL);

	// TODO: 在此加入額外的初始設定
    init_var();
    return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CUsbCommanderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CUsbCommanderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CUsbCommanderDlg::OnQueryDragIcon(){
	return static_cast<HCURSOR>(m_hIcon);
}

void CUsbCommanderDlg::OnCbnSelchangeCboCmdSelect(){
    m_ctrler.m_view.selectChange();
}

void CUsbCommanderDlg::OnBnClickedBtnRefresh(){
    m_ctrler.refresh();
}

void CUsbCommanderDlg::OnBnClickedBtnExecute(){
    m_ctrler.execute();
}

BOOL CUsbCommanderDlg::PreTranslateMessage(MSG* pMsg) {
    if (pMsg->message == WM_KEYDOWN) {
        switch (pMsg->wParam) {
        case VK_PRIOR:
            m_ctrler.pageUp();
            UpdateWindow();
            break;
        case VK_NEXT:
            m_ctrler.pageDown();
            UpdateWindow();
            break;
        }
    }
    return CDialog::PreTranslateMessage(pMsg);
}

void CUsbCommanderDlg::OnBnClickedbtnseqstart()
{
    // TODO: 在此加入控制項告知處理常式程式碼
    try {
        m_ctrler.squenceWrite();
    } catch (EricException& my_ex) {
        AfxMessageBox(my_ex.what());
    }
}


void CUsbCommanderDlg::OnBnClickedbtnrdmstart()
{
    // TODO: 在此加入控制項告知處理常式程式碼
    try {
        m_ctrler.randomWrite();
    } catch (EricException& my_ex) {
        AfxMessageBox(my_ex.what());
    }
}
