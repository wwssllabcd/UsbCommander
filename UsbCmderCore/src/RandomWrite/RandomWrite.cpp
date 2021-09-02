

#include "stdafx.h"
#include "RandomWrite.h"
#include "Utility/Utility.h"
#include "Utility/Observer.h"
#include "Utility/EricException.h"

#define DATA_BUFFER_SIZE    (_256K)
RandomWrite::RandomWrite(ScsiIf& usbcmd) {
    this->m_usbCmd = usbcmd;
}

RandomWrite::~RandomWrite() {
}

void RandomWrite::lbaWrite(eu32 lba, eu16 cnt, eu8_p buf) {
    m_usbCmd.write_10(lba, cnt, buf);
}

void RandomWrite::lbaRead(eu32 lba, eu16 cnt, eu8_p buf) {
    m_usbCmd.read_10(lba, cnt, buf);
}

void RandomWrite::vdrReboot() {
    //m_usbCmd.vdrReboot();
}

estring RandomWrite::makeHeader(eu32 startLba, eu32 endLba, eu32 seed) {
    estring titleMsg;
    Utility su;
    titleMsg += _ET("Start Random W/R") + Utility::crLf() + _ET("random Seed = ") + su.toHexString((eu32)seed) + Utility::crLf();
    titleMsg += _ET("Start LBA = ") + su.toHexString(startLba) + _ET(", End LBA = ") + su.toHexString(endLba) + Utility::crLf();
    return titleMsg;
}

eu32 RandomWrite::getWriteLba(eu32 startLba, eu32 endLba, eu32 secCnt, eu32 count) {
    eu32 writeLba;
    while (1) {
        //writeLba = static_cast<eu32>(GetRandom(startLba, endLba));
        writeLba = m_u.getRandom(startLba, endLba);

        // test Write to cluster(4k)
        if ((count % 0x20) == 3) {
            writeLba &= 0x0000FFFF;
        }

        // test Write for alignment
        if ((count % 0x20) == 13) {
            writeLba &= 0xFFFF0000;
        }

        //shift to each byte
        if ((count % 0x10) == 0) {
            eu8 no = (count >> 4) & 0xFF;
            writeLba = (writeLba & 0xFFFFFF00) + no;
        }

        if ((writeLba + secCnt) < endLba) {
            break;
        }
    }
    return writeLba;
}

eu8 RandomWrite::getSecCnt(eu16 secRange) {
    eu8 secCnt = 1;
    // to avoid secCnt=0, retry 10 time to get secCnt
    for (int i = 0; i < 10; i++) {
        secCnt = rand() % secRange;
        if (secCnt) {
            break;
        } else {
            if (i == 9) {
                secCnt = 1;
            }
        }
    }
    return secCnt;
}

void RandomWrite::clearTextArea(estring titleMsg, eu32 count) {
    if ((count % 0x80) == 0) {
        SEND_MSG_CLEAR();
        SEND_MSG_STR(titleMsg);
    }
    DialogUtility::update_message();
}

void RandomWrite::recordLbaAddr(eu32 lba, eu8 secCnt) {
    eu32 curLbaStart = lba;
    eu32 curLbaEnd = lba + secCnt;

    for (size_t i = 0; i < m_lbaColl.size(); i++) {
        eu32 recordLbaStart = m_lbaColl[i].lba;
        eu32 recordLbaEnd = recordLbaStart + m_lbaColl[i].secCnt;

        //if locate at right side
        if (recordLbaStart < curLbaStart) {
            if (curLbaStart < recordLbaEnd) {
                //modify old record's secCnt
                eu8 secCnt = (eu8)(curLbaStart - recordLbaStart);
                m_lbaColl[i].secCnt = secCnt;
            }
        } else {
            if (curLbaEnd < recordLbaStart) {
                //no hit, continue
                continue;
            }
            //locate left side and hit => always replace old
            m_lbaColl[i].lba = lba;
            m_lbaColl[i].secCnt = secCnt;
        }
    }

    RdmWriteCk lbaObj;


    lbaObj.lba = lba;
    lbaObj.secCnt = secCnt;
    lbaObj.value = lba;
    m_lbaColl.push_back(lbaObj);
}

void RandomWrite::pendingProcess(RandomWriteUi& ui, eu32 count) {

    eu32 pendAt = ui.getStopAt();

    if (count == pendAt) {
        ui.setPending();
    }

    if (ui.isPending() == true) {
        SEND_MSG(_ET("Pending before starting step = %d"), count);
        while (1) {
            DialogUtility::update_message();
            if (ui.isPending() == false) {
                break;
            }
        }
    }
}

void RandomWrite::randomWrite(RandomWriteUi& ui) {
    eu8 pReadBuf[DATA_BUFFER_SIZE];
    eu8 pWriteBuf[DATA_BUFFER_SIZE];
    eu16 secRange = MAX_SEC_LEN;
    estring titleMsg;
    eu32 startLba = ui.getStartLba();
    eu32 endLba = ui.getEndLba();
	eu32 extSeed = ui.getSeed();
	bool isNoRead = ui.isNoRead();
	

    eu32 maxBufLen = SECTOR_TO_BYTE(secRange);
	if (maxBufLen > sizeof(pReadBuf)) {
		THROW_MYEXCEPTION(0, _ET("RdmW: buffer OFB"));
	}

    if (endLba == 0) {
        THROW_MYEXCEPTION(0, _ET("RdmW: endLba == 0"));
    }

    int seed = (unsigned int)time(0);
    if (extSeed) {
        seed = extSeed;
    }
    
    titleMsg = makeHeader(startLba, endLba, seed);
    SEND_MSG_CLEAR();
    SEND_MSG_STR(titleMsg);

    srand(seed);

    int count = 1;
    eu32 totalSecCnt = 0;
    eu32 totalMB = 0;

    Utility fu;
    //write log
    fu.toFile(_ET("RandomWrite.txt"), titleMsg, false);

    while (1) {
        DialogUtility::update_message();

        pendingProcess(ui, count);
        // stop control
        if (ui.isStop()) {
            break;
        }

        // MByte control
        if (totalSecCnt > 2048) {
            totalSecCnt -= 2048;
            totalMB++;
        }

        eu8 secCnt = getSecCnt(secRange);
        totalSecCnt += secCnt;
        eu32 writeLba = getWriteLba(startLba, endLba, secCnt, count);

        clearTextArea(titleMsg, count);

		if (isNoRead) {
			SEND_MSG(_ET("(%x)W only LBA=0x%x, secCnt=0x%x, TotalMB=%d"), count, writeLba, secCnt, totalMB);
		} else {
			SEND_MSG(_ET("(%x)W/R LBA=0x%x, secCnt=0x%x, TotalMB=%d"), count, writeLba, secCnt, totalMB);
		}

        eu32 byteCnt = SECTOR_TO_BYTE(secCnt);

        // make buffer
        m_u.makeBuf(writeLba, byteCnt, pWriteBuf);

        recordLbaAddr(writeLba, secCnt);

        if (ui.isFake() == false) {
            lbaWrite(writeLba, secCnt, pWriteBuf);

			if (isNoRead == false) {
                vdrReboot();
                lbaRead(writeLba, secCnt, pReadBuf);
                compareData(pWriteBuf, pReadBuf, byteCnt, writeLba);
            }

            if ((count % 0x40) == 0) {
                //vdrReboot();
                //verifyRecordLba();
            }
        }
        
        count++;
    }
    SEND_MSG(_ET("\r\nFinish"));
    return;
}

void RandomWrite::compareData(eu8_p writeBuf, eu8_p readBuf, eu32 byteCnt, eu32 writeLba) {
    int result = memcmp(readBuf, writeBuf, byteCnt);
    //印出哪裡有問題
    if (result != 0) {
        estring msg = _ET("..fail");
        SEND_MSG_STR(msg);
        msg = getDiffStringInTwoBuf(writeLba, byteCnt, writeBuf, readBuf);
        SEND_MSG_STR(msg);
        THROW_MYEXCEPTION(0, _ET("RandomWrite Fail"));
    } 
}

void RandomWrite::verifyRecordLba() {
    Utility su;
    eu8 readBuf[DATA_BUFFER_SIZE];
    eu8 compBuf[DATA_BUFFER_SIZE];
    eu32 cnt = (eu32)m_lbaColl.size();
    SEND_MSG(_ET("Verify Record Lba count = %X"), cnt);

    for (eu32 i = 0; i < cnt; i++) {
		DialogUtility::update_message();
        eu32 lba = m_lbaColl[i].lba;
        eu16 secCnt = m_lbaColl[i].secCnt;
        // Read lba 512 byte

        lbaRead(lba, secCnt, readBuf);

        eu32 dataLen = SECTOR_TO_BYTE(secCnt);

        // make buffer
        m_u.makeBuf(lba, dataLen, compBuf);

        int result = memcmp(readBuf, compBuf, dataLen);
        //印出哪裡有問題
        if (result != 0) {
            estring msg = _ET("\r\n--- Verify Record fail step=") + su.toHexString(i);
            msg += _ET("\r\n LBA =") + su.toHexString(lba) + _ET(", len =") + su.toHexString(secCnt);
            msg += getDiffStringInTwoBuf(lba, secCnt, compBuf, readBuf);
            SEND_MSG_STR(msg);
            THROW_MYEXCEPTION(0, _ET("verifyRecordLba error"));
        }
    }

    if (cnt >= 0x400) {
        m_lbaColl.clear();
    }
}

estring RandomWrite::getDiffStringInTwoBuf(eu32 lbaAddr, eu32 byteCnt, eu8_p writeBuf, eu8_p readBuf) {
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