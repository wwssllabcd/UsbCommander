//Copyright © 2006~ EricWang(wwssllabcd@gmail.com). All rights reserved

#include "stdafx.h"
#include "CmdIf.h"

//#define _ENABLE_DEVICE_CTRL_

#ifdef _ENABLE_DEVICE_CTRL_

#include "Drive/DriveService.h"
#include "Drive/UsbCommand.h"
#include "Drive/Drive.h"
#include "Utility/EricException.h"

using namespace EricCore;

void CmdIf::send_cmd(eu8* cdb, eu8* buffer, eu32 byteCnt, eu8 direction, estring_cr desc) const {
    DriveService* ds = DriveService::getInstance();

    if (m_curSel == -1) {
		THROW_MYEXCEPTION(0, _ET("cursel = -1"));
    }

    Drive objDrive = ds->getIdelDeviceById(m_curSel);
    UsbCommand usbCmd(objDrive.address);
    usbCmd.sendCommand(cdb, buffer, byteCnt, direction, desc);
}

vector<estring> CmdIf::get_device_name(void) {
    DriveService* ds = DriveService::getInstance();
    vector<estring> dn;
    ds->getDeviceNameList(dn);
    return dn;
}

void CmdIf::release_device(void) {
    DriveService* ds = DriveService::getInstance();
    ds->releaseSingleton();
}

#else
void CmdIf::send_cmd(eu8* cdb, eu8* buffer, eu32 byteCnt, eu8 direction, estring_cr desc) const {
    //Lba read
    eu8 opCode = cdb[0];
	
    if (opCode == UFI_OP_READ_10) {
        memcpy(buffer, m_fakeDevice, byteCnt);
        return;
    }
    if (opCode == UFI_OP_WRITE_10) {
        memcpy((void*)m_fakeDevice, buffer, byteCnt);
        return;
    }

    //UFI: capacity
    if (opCode == UFI_OP_READ_CAPACITY) {
	    eu32 cap = 0xF0000000;
		//MSB format
		buffer[0] = ((cap >> 0x18) & 0xFF);
		buffer[1] = ((cap >> 0x10) & 0xFF);
		buffer[2] = ((cap >> 0x08) & 0xFF);
		buffer[3] = ((cap >> 0x00) & 0xFF);
        return;
    }
}

vector<estring> CmdIf::get_device_name(void) {
    vector<estring> dn;
    dn.push_back(_ET("C:\\"));
    dn.push_back(_ET("D:\\"));
    return dn;
}

void CmdIf::release_device(void) {
    
}

#endif

CmdIf::CmdIf(){
}

CmdIf::~CmdIf(void)
{
}

void CmdIf::send_cmd(UsbCmdStruct cmd, eu8* buffer) const {
    send_cmd(cmd.cdb, buffer, cmd.length, cmd.direction, cmd.description);
}

void CmdIf::write10(eu32 lba, eu16 secCnt, eu8* buffer) const {
    UsbCmdStruct cmd;
    send_cmd(cmd.write10(lba, secCnt), buffer);
}

void CmdIf::read10(eu32 lba, eu16 secCnt, eu8* buffer) const {
    UsbCmdStruct cmd;
    send_cmd(cmd.read10(lba, secCnt), buffer);
}

void CmdIf::testUnitReady() const {
    eu8 buffer[1];
    UsbCmdStruct cmd;
    send_cmd(cmd.testUnitReady(), buffer);
}

void CmdIf::inquiry(eu8* buffer) const {
    UsbCmdStruct cmd;
    send_cmd(cmd.inquiry(), buffer);
}

void CmdIf::readCapacity(eu8* buffer) const {
    UsbCmdStruct cmd;
    send_cmd(cmd.readCapacity(), buffer);
}

void CmdIf::vdrReboot(void) const {
    UsbCmdStruct cmd;
    eu8 buffer[1];
    send_cmd(cmd.vdrReboot(), buffer);
}

