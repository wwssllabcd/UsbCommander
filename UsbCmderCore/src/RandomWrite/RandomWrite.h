#pragma once

#include "RandomWriteUi.h"
#include "Utility/Utility.h"
#include "ScsiUtility/ScsiIf.h"

using namespace EricCore;

class RandomWrite
{
public:
	RandomWrite(ScsiIf& usbcmd);
	~RandomWrite();

	void randomWrite(RandomWriteUi& ui);

private:
	typedef struct {
		eu32 lba;
		eu16 secCnt;
		eu32 value;
	}RdmWriteCk;

	vector<RdmWriteCk> m_lbaColl;
	ScsiIf m_usbCmd;

	eu32 getWriteLba(eu32 startLba, eu32 endLba, eu32 secCnt, eu32 count);
	eu8 getSecCnt(eu16 secRange);

	estring makeHeader(eu32 startLba, eu32 endLba, eu32 seed);
	void recordLbaAddr(eu32 Lba, eu8 secCnt);
	void verifyRecordLba();

	void pendingProcess(RandomWriteUi& ui, eu32 count);
	void lbaWrite(eu32 lba, eu16 secLen, eu8_p buffer);
	void lbaRead(eu32 lba, eu16 secLen, eu8_p buffer);
	void clearTextArea(estring titleMsg, eu32 count);
	void vdrReboot();
	void compareData(eu8_p writeBuf, eu8_p readBuf, eu32 byteCnt, eu32 writeLba);

	estring getDiffStringInTwoBuf(eu32 lbaAddr, eu32 byteCnt, eu8_p writeBuf, eu8_p readBuf);

    Utility m_u;

};

