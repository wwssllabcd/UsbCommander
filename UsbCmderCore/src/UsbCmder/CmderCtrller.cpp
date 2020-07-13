//Copyright © 2006- EricWang(wwssllabcd@gmail.com). All rights reserved

#include "stdafx.h"
#include "CmderCtrller.h"

#include "SequenceWrite/SequenceWrite.h"
#include "RandomWrite/RandomWrite.h"

#include "DriverAdapter/UsbCmdStruct.h"

#include "Utility/Observer.h"
#include "Utility/Utility.h"
#include "Utility/EricException.h"

#define NULL_32 (0xFFFFFFFF)

CmderCtrller::CmderCtrller(){
}

CmderCtrller::~CmderCtrller(){
}

void CmderCtrller::lba_up_down_ctrl(UsbCmdStruct& curUsbCmd, bool isIncrease) {
	const int offset = 2;
	int shift = m_view.getShiftNo(isIncrease);

	eu32 addr = m_u.toU32(&curUsbCmd.cdb[offset]);
	addr += shift;
	m_u.toArray(addr, &curUsbCmd.cdb[offset]);

	m_view.setLbaToUI(curUsbCmd);
}

void CmderCtrller::pageUp() {
	UsbCmdStruct usbCmd = m_view.loadCmdSetFromUI();
	eu8 cmd = usbCmd.cdb[0];

	// lba
	if (cmd == 0x28) {
		lba_up_down_ctrl(usbCmd, false);
		execute();
		return;
	}
}

void CmderCtrller::pageDown() {
	UsbCmdStruct usbCmd = m_view.loadCmdSetFromUI();
	eu8 cmd = usbCmd.cdb[0];

	// lba
	if (cmd == 0x28) {
		lba_up_down_ctrl(usbCmd, true);
		execute();
		return;
	}
}

eu32 CmderCtrller::toLba(UsbCmdStruct cmd) {
	return m_u.toU32(&cmd.cdb[2]);
}

void CmderCtrller::executeUsbCmd(UsbCmdStruct cmd) {
	memset(pc_buffer, 0xcc, sizeof(pc_buffer)); // init buffer
	if(cmd.cdb[0] == UFI_OP_WRITE_10) {
		m_u.makeBuf(toLba(cmd), cmd.length, pc_buffer);
	}

	CmdIf usbCmd = get_cmdif();
    usbCmd.send_cmd(cmd, pc_buffer);

    Utility su;
	estring msg = su.makeHexTable(cmd.length, pc_buffer);
	SEND_MSG_STR_CLEAR(msg);

	estring asciiMsg = su.makeAsciiTable(pc_buffer, cmd.length);
	m_view.sendMsgToAsciiArea(true, asciiMsg);
}

void CmderCtrller::execute() {
	try {
		executeUsbCmd(m_view.loadCmdSetFromUI());
	} catch (EricException& mye) {
		m_view.showPopupWindows(mye.what());
	}
}

CmdIf CmderCtrller::get_cmdif() {
    m_CmdIf.m_curSel = m_view.getDriveSel();
    m_CmdIf.m_type = 1;
    return m_CmdIf;
}

eu32 CmderCtrller::getCapacity() {
    CmdIf usbCmd = get_cmdif();
	eu8 buffer[8];
	usbCmd.readCapacity(buffer);
	eu32 res = m_u.toU32(buffer);
	return res;
}

void CmderCtrller::seEndLba() {
	CmderView* view = &m_view;
	if (view->m_seqWriteUi.getEndLba() == NULL_32) {
		eu32 strEndLba = getCapacity();
		Utility u;
		view->m_seqWriteUi.setEndLba(u.toHexString(strEndLba, _ET("%X")));
	}
}

void CmderCtrller::squenceWrite() {
	CmderView* view = &m_view;
    CmdIf usbCmd = get_cmdif();
	seEndLba();
	
	SequenceWrite sw(usbCmd);
	sw.sequenceWrite(view->m_seqWriteUi);
}

void CmderCtrller::randomWrite() {
	CmderView* view = &m_view;
    CmdIf usbCmd = get_cmdif();
	seEndLba();
	
	RandomWrite rw(usbCmd);
	rw.randomWrite(m_view.m_rdmWriteUi);
}

void CmderCtrller::refresh() {
    CmdIf cmdif = get_cmdif();
    cmdif.release_device();
    m_view.set_device_box(cmdif.get_device_name());
}

void CmderCtrller::init() {
    m_view.init();
    refresh();
}

void CmderCtrller::close() {
	m_view.m_seqWriteUi.stop();
	m_view.m_rdmWriteUi.stop();
}

void CmderCtrller::driverChange() {
	m_view.driveSelectChange();
}