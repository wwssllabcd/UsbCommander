#include "stdafx.h"
#include "WriteReadCmp.h"

#include "Utility/Observer.h"



WriteReadCmp::WriteReadCmp(const ScsiIf& usbCmd) {
	this->m_usbCmd = usbCmd;
}

void WriteReadCmp::lbaWrite(eu32 lba, eu16 cnt, eu8_p buf) {
	m_usbCmd.write_10(lba, cnt, buf);
}

void WriteReadCmp::lbaRead(eu32 lba, eu16 cnt, eu8_p buf) {
	m_usbCmd.read_10(lba, cnt, buf);
}

eu32 WriteReadCmp::getCapacity() {
    eu8 buffer[8];
    m_usbCmd.read_capacity(buffer);
    eu32 res = m_u.toU32(buffer);
    return res;
}

estring WriteReadCmp::gen_cmp_error_string(eu32 lbaAddr, eu8 secCnt, eu8_p writeBuf, eu8_p readBuf) {
	estring result, msg;
	Utility su;
	eu32 dataLen = SEC_TO_BYTE(secCnt);
	int errorCnt = 0;
	result = Utility::crLf() + su.strFormat(_ET(" cmp error in lbaAddr = %x"), lbaAddr);

	for (eu32 g = 0; g < dataLen; g++) {
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

void WriteReadCmp::preCheck(eu16 secCnt, eu32 bufSize) {
	if (secCnt == 0) {
		THROW_MYEXCEPTION(0, _ET("SeqW: SecCnt=0"));
	}

	eu32 dataLen = SEC_TO_BYTE(secCnt);
	if (dataLen > bufSize) {
		THROW_MYEXCEPTION(0, _ET("SeqW: buffer OFB"));
	}
}

estring WriteReadCmp::get_diff_string_in_two_buffer(eu32 lbaAddr, eu32 byteCnt, eu8_p writeBuf, eu8_p readBuf) {
	estring result, msg;
	Utility su;
	int errorCnt = 0;
	result = Utility::crLf() + su.strFormat(_ET(" cmp error in lbaAddr = %x"), lbaAddr);

	for (eu32 g = 0; g < byteCnt; g++) {
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

bool WriteReadCmp::verify_lba(vector<WriteLba> lbaColls, eu32 start, eu32 end, eu8_p compBuf, eu8_p readBuf) {
	eu32 cnt = (eu32)lbaColls.size();
	SEND_MSG(_ET("Verify Record Lba count = %X"), cnt);

	for (eu32 i = start; i < end; i++) {

		if ((i % 0x200) == 0) {
			SEND_MSG(_ET("Verify Record .. %X"), i);
		}

		eu32 lba = lbaColls[i].lba;
		eu16 secCnt = lbaColls[i].secCnt;

		// Read lba 512 byte
		lbaRead(lba, secCnt, readBuf);
		eu32 dataLen = SEC_TO_BYTE(secCnt);

		// make buffer
		m_u.makeBuf(lba, dataLen, compBuf);

		int result = memcmp(readBuf, compBuf, dataLen);

		if (result != 0) {
			estring msg = _ET("\r\n--- Verify Record fail step=") + m_u.toHexString(i);
			msg += _ET("\r\n LBA =") + m_u.toHexString(lba) + _ET(", len =") + m_u.toHexString(secCnt);
			msg += get_diff_string_in_two_buffer(lba, secCnt, compBuf, readBuf);
			SEND_MSG_ESTR(msg);
			return false;
		}
	}
	SEND_MSG(_ET("Verify Record pass"));
	return true;
}

estring WriteReadCmp::gen_write_msg(bool isNoWrite, eu32 step, eu32 lba, eu8 secCnt) {
	estring msg;
	Utility u;
	if (isNoWrite) {
		msg = u.strFormat(_ET("(%X)No Write LBA"), step);
	} else {
		msg = u.strFormat(_ET("(%X)Write LBA(0x%x), secCnt = 0x %X"), step, lba, secCnt);
	}
	return msg;
}

bool WriteReadCmp::write_read_cmp(eu32 lba, eu8 secCnt, bool isNoWrite, bool isNoRead, eu8_p wBuf, eu8_p rBuf, eu32 step) {
	eu32 dataLen = SEC_TO_BYTE(secCnt);
	
	m_u.makeBuf(lba, dataLen, wBuf);
	if (isNoWrite) {
		SEND_MSG(_ET("(%X)No Write LBA"), step);
	} else {
		SEND_MSG(_ET("(%X)Write LBA (0x%x), secCnt=0x%X"), step, lba, secCnt);
		lbaWrite(lba, secCnt, wBuf);
		SEND_MSG_CTRL(false, false, _ET(" ... pass"));
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