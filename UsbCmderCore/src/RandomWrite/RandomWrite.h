#pragma once

#include "RandomWriteUi.h"
#include "Utility/Utility.h"
#include "DriverAdapter/CmdIf.h"

using namespace EricCore;

class RandomWrite
{
public:
	RandomWrite(CmdIf& usbcmd);
	~RandomWrite();

	void randomWrite(RandomWriteUi& ui);

private:
	typedef struct {
		eu32 lba;
		eu16 secCnt;
		eu32 value;
	}RdmWriteCk;

	vector<RdmWriteCk> m_lbaColl;
    CmdIf m_usbCmd;

	eu32 getWriteLba(eu32 startLba, eu32 endLba, eu32 secCnt, eu32 count);
	eu8 getSecCnt(eu16 secRange);

	estring makeHeader(eu32 startLba, eu32 endLba, eu32 seed);
	void recordLbaAddr(eu32 Lba, eu8 secCnt);
	void verifyRecordLba();

	void pendingProcess(RandomWriteUi& ui, eu32 count);
	void lbaWrite(eu32 lba, eu16 secLen, eu8* buffer);
	void lbaRead(eu32 lba, eu16 secLen, eu8* buffer);
	void clearTextArea(estring titleMsg, eu32 count);
	void vdrReboot();
	void compareData(eu8* writeBuf, eu8* readBuf, int length, eu32 writeLba);

	estring getDiffStringInTwoBuf(eu32 lbaAddr, int length, eu8* writeBuf, eu8* readBuf);

    Utility m_u;

};

