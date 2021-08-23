//Copyright © 2006- EricWang(wwssllabcd@gmail.com). All rights reserved

#include "stdafx.h"

#include "SequenceWrite.h"
#include "Utility/Utility.h"
#include "Utility/Observer.h"
#include "Utility/EricException.h"
#include "DriverAdapter/UsbCmdStruct.h"


#define DATA_BUFFER_SIZE (_256K)


SequenceWrite::SequenceWrite(const CmdIf& usbCmd) {
	this->m_usbCmd = usbCmd;
}

SequenceWrite::~SequenceWrite() {
}

void SequenceWrite::lbaWrite(eu32 lba, eu16 cnt, eu8* buf) {
	m_usbCmd.write10(lba, cnt, buf);
}

void SequenceWrite::lbaRead(eu32 lba, eu16 cnt, eu8* buf) {
	m_usbCmd.read10(lba, cnt, buf);
}

void SequenceWrite::vdrReboot() {
	m_usbCmd.vdrReboot();
}

bool SequenceWrite::sequenceWrite(SequenceWriteUi& ui) {
	return sequenceWrite(
		ui.getStartLba()
		, ui.getEndLba()
		, ui.getStep()
		, ui.getSecCnt()
		, ui.isNoWrite()
		, ui.isNoRead()
		, ui
	);
}

estring SequenceWrite::genErrorMsg(eu32 startLba, eu32 endLba, eu16 secCnt, eu32 curLba, eu32 step, int howManyStep, eu8* pWriteBuf, eu8* pReadBuf){
	estring msg;
    Utility su;
	msg = Utility::crLf();
	msg += _ET("start Addr=") + su.toHexString(startLba) + _ET(", End LBA=") + su.toHexString(endLba);
	msg += _ET(", i+=") + su.toHexString(step) + _ET(", Len=") + su.toHexString(secCnt) + Utility::crLf();
	msg += _ET("Stop at step No.") + su.toString(howManyStep) + Utility::crLf() + Utility::crLf();
	msg += getDiffStringInTwoBuf(curLba, secCnt, pWriteBuf, pReadBuf);

	estring strReadBuf = su.makeHexTable(secCnt * BYTE_PER_SECTOR, pReadBuf);
	msg += Utility::crLf() + Utility::crLf() + _ET("======= Read Buf ========= ") + Utility::crLf() + strReadBuf;

	return msg;
}

bool SequenceWrite::sequenceWrite(eu32 startLba, eu32 endLba, eu32 step, eu16 secCnt, bool isNoWrite, bool isNoRead, SequenceWriteUi& ui)
{
	eu8 m_readBuf[DATA_BUFFER_SIZE];
	eu8 m_writeBuf[DATA_BUFFER_SIZE];

	eu8* pWriteBuf = m_writeBuf;
	eu8* pReadBuf = m_readBuf;

	eu32 dataLen = secCnt * BYTE_PER_SECTOR;
	if (dataLen > sizeof(m_readBuf)) {
		THROW_MYEXCEPTION(0, _ET("SeqW: buffer OFB"));
	}

	estring msg;
	int howManyStep = 0;
	int result = 0;
	if(secCnt == 0) {
		THROW_MYEXCEPTION(0, _ET("SeqW: SecCnt=0"));
	}

	for(eu32 lbaAddr = startLba; lbaAddr <= endLba; lbaAddr += step) {
		DialogUtility::update_message();
		if(ui.isStop()) {
			break;
		}

		// adjust lbaOffset
		if((lbaAddr + secCnt) > endLba) {
			break;
		}

        SEND_MSG_CLEAR();

		m_u.makeBuf(lbaAddr, dataLen, pWriteBuf);
		if(isNoWrite) {
			SEND_MSG(_ET("No Write LBA"));
		} else {
			SEND_MSG(_ET("Write LBA (0x%x), secCnt=0x%X"), lbaAddr, secCnt);
			lbaWrite(lbaAddr, secCnt, pWriteBuf);
			vdrReboot();
		}

		if(isNoRead == true) {
			continue;
		}

		SEND_MSG(_ET("Read  LBA (0x%x), secCnt=0x%X"), lbaAddr, secCnt);
		lbaRead(lbaAddr, secCnt, pReadBuf);

		result = memcmp(pReadBuf, pWriteBuf, dataLen);

		//印出哪裡有問題
		if(result != 0) {
			msg = genErrorMsg(startLba, endLba, secCnt, lbaAddr, step, howManyStep, pWriteBuf, pReadBuf);
            SEND_MSG_STR(msg);
			return false;
		}


        SEND_MSG(_ET("Verify .. Pass"));
		howManyStep++;
	}
    SEND_MSG(_ET("Finish"));
	return true;

}

estring SequenceWrite::getDiffStringInTwoBuf(eu32 lbaAddr, eu16 secCnt, eu8* writeBuf, eu8* readBuf) {
	estring result, msg;
	Utility su;
	int errorCnt = 0;
	result = Utility::crLf() + su.strFormat(_ET(" cmp error in lbaAddr = %x"), lbaAddr);

	for (int g = 0; g < secCnt*BYTE_PER_SECTOR; g++) {
		if (writeBuf[g] != readBuf[g]) {
			if (errorCnt < 20) {
				msg = su.strFormat(_ET("  writeBuf[%x]=0x%x,readBuf[%x]=0x%x"), g, writeBuf[g], g, readBuf[g]);
				result += Utility::crLf() + msg;
			}
			errorCnt++;
		}
	}

	if (errorCnt >= 20) {
		result += Utility::crLf() + _ET("over 20 error");
	}

	result += Utility::crLf() + su.strFormat(_ET("Fail ECC Cnt = 0x%X"), errorCnt);
	return result;
}