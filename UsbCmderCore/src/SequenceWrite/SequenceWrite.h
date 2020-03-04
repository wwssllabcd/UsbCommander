#pragma once

#include "DefineFiles/EricType.h"
#include "SequenceWriteUi.h"
#include "DriverAdapter/CmdIf.h"

#include "Utility/Utility.h"

using namespace EricCore;

class SequenceWrite
{
public:
	SequenceWrite(const CmdIf& usbCmd);
	~SequenceWrite();

	bool sequenceWrite(SequenceWriteUi& ui);
	bool sequenceWrite(eu32 startLba, eu32 endLba, eu32 step, eu16 secCnt, bool isNoWrite, bool isNoRead, SequenceWriteUi& ui);
	

private:
    CmdIf m_usbCmd;
	void makeButterflyPattern(eu32 number, int length, eu8_p buf);
	void lbaWrite(eu32 lba, eu16 secLen, eu8* buffer);
	void lbaRead(eu32 lba, eu16 secLen, eu8* buffer);
	void vdrReboot();
	estring genMsg(eu32 startLba, eu32 endLba, eu16 secCnt, eu32 curLba, eu32 step, int howManyStep, eu8* pWriteBuf, eu8* pReadBuf);
	estring getDiffStringInTwoBuf(eu32 lbaAddr, int length, eu8* writeBuf, eu8* readBuf);

	Utility m_u;

};

