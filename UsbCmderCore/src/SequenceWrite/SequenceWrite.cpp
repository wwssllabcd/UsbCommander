

#include "stdafx.h"

#include "SequenceWrite.h"
#include "Utility/Utility.h"
#include "Utility/Observer.h"
#include "Utility/EricException.h"
#include "ScsiUtility/ScsiCmd.h"


#define DATA_BUFFER_SIZE (_256K)


SequenceWrite::SequenceWrite(const ScsiIf& usbCmd) {
	this->m_usbCmd = usbCmd;
}

SequenceWrite::~SequenceWrite() {
}

void SequenceWrite::lbaWrite(eu32 lba, eu16 cnt, eu8_p buf) {
	m_usbCmd.write_10(lba, cnt, buf);
}

void SequenceWrite::lbaRead(eu32 lba, eu16 cnt, eu8_p buf) {
	m_usbCmd.read_10(lba, cnt, buf);
}

void SequenceWrite::vdrReboot() {
	//m_usbCmd.vdrReboot();
}

SeqWriteParam SequenceWrite::getSeqParamFromUi(SequenceWriteUi& ui) {
	SeqWriteParam swp;

	swp.sLba = ui.getStartLba();
	swp.eLba = ui.getEndLba();
	swp.step = ui.getStep();
	swp.secCnt = ui.getSecCnt();
	swp.isNoWrite = ui.isNoWrite();
	swp.isNoRead = ui.isNoRead();
	return swp;
}

bool SequenceWrite::sequenceWrite(SequenceWriteUi& ui) {
	SeqWriteParam swp = getSeqParamFromUi(ui);
	return sequenceWrite(
		swp.sLba
		, swp.eLba
		, swp.step
		, swp.secCnt
		, swp.isNoWrite
		, swp.isNoRead
		, ui
	);
}

estring SequenceWrite::genErrorMsg(eu32 startLba, eu32 endLba, eu16 secCnt, eu32 curLba, eu32 step, int howManyStep, eu8_p pWriteBuf, eu8_p pReadBuf){
	estring msg;
    Utility su;
	msg = Utility::crLf();
	msg += _ET("start Addr=") + su.toHexString(startLba) + _ET(", End LBA=") + su.toHexString(endLba);
	msg += _ET(", i+=") + su.toHexString(step) + _ET(", Len=") + su.toHexString(secCnt) + Utility::crLf();
	msg += _ET("Stop at step No.") + su.toString(howManyStep) + Utility::crLf() + Utility::crLf();
	msg += su.strFormat(_ET("fail in lba = %x"), curLba) + Utility::crLf();
	msg += getDiffStringInTwoBuf(curLba, secCnt, pWriteBuf, pReadBuf);

	estring strReadBuf = su.makeHexTable(SEC_TO_BYTE(secCnt), pReadBuf);
	msg += Utility::crLf() + Utility::crLf() + _ET("======= Read Buf ========= ") + Utility::crLf() + strReadBuf;

	return msg;
}

void SequenceWrite::preCheck(eu16 secCnt, eu32 bufSize) {
	if (secCnt == 0) {
		THROW_MYEXCEPTION(0, _ET("SeqW: SecCnt=0"));
	}

	eu32 dataLen = SEC_TO_BYTE(secCnt);
	if (dataLen > bufSize) {
		THROW_MYEXCEPTION(0, _ET("SeqW: buffer OFB"));
	}
}

bool SequenceWrite::execSeqOnce(eu32 lba, eu16 secCnt, bool isNoWrite, bool isNoRead, eu8_p wBuf, eu8_p rBuf) {
	eu32 dataLen = SEC_TO_BYTE(secCnt);

	m_u.makeBuf(lba, dataLen, wBuf);
	if (isNoWrite) {
		SEND_MSG(_ET("No Write LBA"));
	} else {
		SEND_MSG(_ET("Write LBA (0x%x), secCnt=0x%X"), lba, secCnt);

		lbaWrite(lba, secCnt, wBuf);
		SEND_MSG_CTRL(false, false, _ET(" ... pass"));
		vdrReboot();
	}

	if (isNoRead) {
		return true;
	}

	SEND_MSG(_ET("Read  LBA (0x%x), secCnt=0x%X"), lba, secCnt);
	lbaRead(lba, secCnt, rBuf);
	SEND_MSG_CTRL(false, false, _ET(" ... pass"));

	int result = memcmp((eu8_p)rBuf, (eu8_p)wBuf, dataLen);
	if (result != 0) {
		return false;
	}
	return true;
}

bool SequenceWrite::sequenceWrite(eu32 startLba, eu32 endLba, eu32 step, eu16 secCnt, bool isNoWrite, bool isNoRead, SequenceWriteUi& ui)
{
	eu8 m_readBuf[DATA_BUFFER_SIZE];
	eu8 m_writeBuf[DATA_BUFFER_SIZE];

	preCheck(secCnt, sizeof(m_readBuf));

	estring msg;
	int result = 0;
	int howManyStep = 0;

	for(eu32 lbaAddr = startLba; lbaAddr <= endLba; lbaAddr += step, howManyStep++) {
		DialogUtility::update_message();
		if(ui.isStop()) {
			break;
		}

		// adjust lbaOffset
		if((lbaAddr + secCnt) > endLba) {
			break;
		}

        SEND_MSG_CLEAR();



		bool res = execSeqOnce(lbaAddr, secCnt, isNoWrite, isNoRead, m_writeBuf, m_readBuf);

		if (res == false) {
			SEND_MSG(_ET("Verify ... fail"));
			msg = genErrorMsg(startLba, endLba, secCnt, lbaAddr, step, howManyStep, m_writeBuf, m_readBuf);
			SEND_MSG_ESTR_CTRL(msg, true, false);
			return false;
		}

		SEND_MSG(_ET("Verify ... Pass"));
	
	}
    SEND_MSG(_ET("Finish"));
	return true;

}

estring SequenceWrite::getDiffStringInTwoBuf(eu32 lbaAddr, eu16 secCnt, eu8_p writeBuf, eu8_p readBuf) {
	estring result, msg;
	Utility su;
	int errorCnt = 0;
	result = Utility::crLf() + su.strFormat(_ET(" cmp error in lbaAddr = %x"), lbaAddr);

	for (int g = 0; g < SEC_TO_BYTE(secCnt); g++) {
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