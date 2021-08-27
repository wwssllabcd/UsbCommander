//Copyright © 2006- EricWang(wwssllabcd@gmail.com). All rights reserved

#include "stdafx.h"
#include "CmderView.h"
#include "DefineFiles\DefBitConst.h"
#include "Utility/Utility.h"

#define MAX_TEXTBOX_SIZE (0x6000)

CmderView::CmderView()
	:m_pDataLen(0)
	, m_pDataIn(0)
	, m_pDataOut(0)
	, m_cboCmdSel(0)
	, m_cboDriveSel(0)
	, m_dialogMsg(0)
	, m_asciiMsg(0)
	, m_pDialog(0)
{
}

CmderView::~CmderView() {
}

void CmderView::init(void) {
    this->m_totalCmdSet = loadTotalCmdSet();
    setTotalCmdToForm(this->m_totalCmdSet);
    m_cboDriveSel->SetCurSel(0);
    m_cboCmdSel->SetCurSel(0);
    selectChange();
}

void CmderView::setup_ui_item(int deviceSel, int cmdSel, int* srcId_cdb,
    int dataOut, int dataIn, int dataLen, int mainMsg, int asciiMsg, int sysInfo)
{
    for (int i = 0; i < MAX_CDB_OF_UI; i++) {
        m_cdbs[i] = m_du.getDlgItemPtr<CEdit>(m_pDialog, srcId_cdb[i]);
    }

    m_cboCmdSel = m_du.getDlgItemPtr<CComboBox>(m_pDialog, cmdSel);
    m_cboDriveSel = m_du.getDlgItemPtr<CComboBox>(m_pDialog, deviceSel);
    m_pDataLen = m_du.getDlgItemPtr<CEdit>(m_pDialog, dataLen);
    m_pDataIn = m_du.getDlgItemPtr<CButton>(m_pDialog, dataIn);
    m_pDataOut = m_du.getDlgItemPtr<CButton>(m_pDialog, dataOut);
	m_dialogMsg = m_du.getDlgItemPtr<CEdit>(m_pDialog, mainMsg);
	
	m_asciiMsg = m_du.getDlgItemPtr<CEdit>(m_pDialog, asciiMsg);

	m_lblSysMsg = m_du.getDlgItemPtr<CStatic>(m_pDialog, sysInfo);
}

void CmderView::setup_ui_num_to_view(UsbCmderMfcUiNum* const uiNumObj) {
    setup_ui_item(
        uiNumObj->devceSel
        , uiNumObj->cmdSel
        , uiNumObj->cdb
        , uiNumObj->dataOut
        , uiNumObj->dataIn
        , uiNumObj->dataLen
		, uiNumObj->message
		, uiNumObj->asciiMessage
		, uiNumObj->sysInfo
    );
}

#define	FLAG_DATA_IN			0x00000001
#define	FLAG_DATA_OUT			0x00000000

void CmderView::setDirectionToUi(ScsiIoDir direction) {
	if (direction == ScsiIoDir::DATA_IN) {
		m_pDataIn->SetCheck(true);
		m_pDataOut->SetCheck(false);
	} else {
		m_pDataIn->SetCheck(false);
		m_pDataOut->SetCheck(true);
	}
}

ScsiIoDir CmderView::getDirectionFromUi(void) {
	int isDataIn = m_pDataIn->GetCheck();
	int isDataOut = m_pDataOut->GetCheck();
	if (isDataIn == 1) {
		return  ScsiIoDir::DATA_IN;
	}
	if (isDataOut == 1) {
		return  ScsiIoDir::DATA_OUT;
	}
	return ScsiIoDir::NON_DATA;
}

void CmderView::setDataLenToUi(eu32 length) {
    Utility u;
	estring res = u.toHexString(length);
	m_pDataLen->SetWindowText(res.c_str());
}

int CmderView::getDataLenFromUi() {
    return m_u.hexToU32(m_du.getText<CEdit>(m_pDataLen));
}

ScsiCmd CmderView::loadCmdSetFromUI(eu32 partialSetCtrl) {
	ScsiCmd objCmd;

	for (int i = 0; i < MAX_CDB_OF_UI; i++) {
		if (partialSetCtrl& (1 << i)) {
			objCmd.cdb[i] = (eu8)m_u.hexToU32(m_du.getText<CEdit>(m_cdbs[i]));
		}
	}
	if (partialSetCtrl & E_BIT_12) {
		objCmd.length = getDataLenFromUi();
	}
	if (partialSetCtrl& E_BIT_13) {
		objCmd.direction = getDirectionFromUi();
	}
	return objCmd;
}

void CmderView::setCmdSetToUI(const ScsiCmd& objCmd, eu32 partialSetCtrl) {
    Utility su;
	for (int i = 0; i < MAX_CDB_OF_UI; i++) {
		if (partialSetCtrl& (1 << i)) {
			m_cdbs[i]->SetWindowText(su.toHexString(objCmd.cdb[i]).c_str());
		}
	}
	if (partialSetCtrl& E_BIT_12) {
		setDataLenToUi(objCmd.length);
	}
	if (partialSetCtrl& E_BIT_13) {
		setDirectionToUi(objCmd.direction);
	}
}

void CmderView::driveSelectChange() {
	estring res = estring(_ET("SysInfo: Select Device - ")) + m_du.getText(m_cboDriveSel);
	m_du.setText(m_lblSysMsg, res);
}

void CmderView::selectChange() {
	int curSel = m_cboCmdSel->GetCurSel();
	if (curSel == -1) {
		return;
	}
	ScsiCmd objCmd = m_totalCmdSet[curSel];
	setCmdSetToUI(objCmd);
}

void CmderView::loadExtCmdSet(ScsiCmdSet& totalCmdSet) {
}

ScsiCmdSet CmderView::loadTotalCmdSet() {
	ScsiCmd usbStruct;
	ScsiCmdSet totalCmdSet = usbStruct.get_all_command_set();
	loadExtCmdSet(totalCmdSet);
	return totalCmdSet;
}

void CmderView::setTotalCmdToForm(const ScsiCmdSet& totalCmdSet) {
	size_t cmdCount = totalCmdSet.size();
    Utility su;
	for (size_t i = 0; i < cmdCount; i++) {
		estring no = su.strFormat(_ET("%02d-"), i) + totalCmdSet[i].description;
		m_cboCmdSel->AddString(no.c_str());
	}
}

int CmderView::getDriveSel(void) {
	return m_cboDriveSel->GetCurSel();
}

void CmderView::setDlgPointer(CDialog* dialogPtr) {
	m_pDialog = dialogPtr;
}

void CmderView::set_device_box(vector<estring> deviceName) {
    m_cboDriveSel->ResetContent();
	for (auto item : deviceName) {
		m_cboDriveSel->AddString(item.c_str());
	}
    m_cboDriveSel->SetCurSel(0);
	driveSelectChange();
}

int CmderView::getShiftNo(bool isIncrease) {
	int res = 0;
	int shiftNo = 1;
	if (isIncrease) {
		res += shiftNo;
	} else {
		res -= shiftNo;
	}
	return res;
}

void CmderView::setLbaToUI(const ScsiCmd& cmdset) {
	setCmdSetToUI(cmdset, 0x3C);
}

estring CmderView::getHandleString() {
    return _ET("");
}

void CmderView::closeCheckBtn(void) {
	this->m_seqWriteUi.stop();
	this->m_rdmWriteUi.stop();
}

void CmderView::sendMsgBase(CEdit* pMsgArea, bool isClean, estring_cr msg) {
	//init
	if(isClean) {
		pMsgArea->SetWindowText(msg.c_str());
	} else {
		int end = pMsgArea->GetWindowTextLength();

		if (end >= MAX_TEXTBOX_SIZE) {
			//force clean text
			pMsgArea->SetWindowText(msg.c_str());
			return;
		}

		pMsgArea->SetSel(end, end);
		pMsgArea->ReplaceSel(msg.c_str());
	}
}

void CmderView::sendMsgToDialogArea(bool isClean, estring_cr msg) {
	sendMsgBase(this->m_dialogMsg, isClean, msg);
}

void CmderView::sendMsgToAsciiArea(bool isClean, estring_cr msg) {
	sendMsgBase(this->m_asciiMsg, isClean, msg);
}

void CmderView::showPopupWindows(estring_cr msg) {
	this->m_pDialog->MessageBox(msg.c_str());
}
