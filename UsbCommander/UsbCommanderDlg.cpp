
// UsbCommanderDlg.cpp : ��@��
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


// �� App About �ϥ� CAboutDlg ��ܤ��

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ܤ�����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

// �{���X��@
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


// CUsbCommanderDlg ��ܤ��



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
// CUsbCommanderDlg �T���B�z�`��

BOOL CUsbCommanderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �N [����...] �\���[�J�t�Υ\���C

	// IDM_ABOUTBOX �����b�t�ΩR�O�d�򤧤��C
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

	// �]�w����ܤ�����ϥܡC�����ε{�����D�������O��ܤ���ɡA
	// �ج[�|�۰ʱq�Ʀ��@�~
	SetIcon(m_hIcon, TRUE);			// �]�w�j�ϥ�
	SetIcon(m_hIcon, FALSE);		// �]�w�p�ϥ�

	ShowWindow(SW_SHOWNORMAL);

	// TODO: �b���[�J�B�~����l�]�w
    init_var();
    return TRUE;  // �Ǧ^ TRUE�A���D�z�ﱱ��]�w�J�I
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

// �p�G�N�̤p�ƫ��s�[�J�z����ܤ���A�z�ݭn�U�C���{���X�A
// �H�Kø�s�ϥܡC���ϥΤ��/�˵��Ҧ��� MFC ���ε{���A
// �ج[�|�۰ʧ������@�~�C

void CUsbCommanderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ø�s���˸m���e

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N�ϥܸm����Τ�ݯx��
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �yø�ϥ�
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ��ϥΪ̩즲�̤p�Ƶ����ɡA
// �t�ΩI�s�o�ӥ\����o�����ܡC
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
    // TODO: �b���[�J����i���B�z�`���{���X
    try {
        m_ctrler.squenceWrite();
    } catch (EricException& my_ex) {
        AfxMessageBox(my_ex.what());
    }
}


void CUsbCommanderDlg::OnBnClickedbtnrdmstart()
{
    // TODO: �b���[�J����i���B�z�`���{���X
    try {
        m_ctrler.randomWrite();
    } catch (EricException& my_ex) {
        AfxMessageBox(my_ex.what());
    }
}
