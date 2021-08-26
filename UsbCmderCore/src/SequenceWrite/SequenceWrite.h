#pragma once

#include "DefineFiles/EricType.h"
#include "SequenceWriteUi.h"
#include "Scsi/ScsiIf.h"

#include "Utility/Utility.h"

using namespace EricCore;

class SequenceWrite
{
public:
	SequenceWrite(const ScsiIf& usbCmd);
	~SequenceWrite();

	bool sequenceWrite(SequenceWriteUi& ui);
	bool sequenceWrite(eu32 startLba, eu32 endLba, eu32 step, eu16 secCnt, bool isNoWrite, bool isNoRead, SequenceWriteUi& ui);
	

private:
	ScsiIf m_usbCmd;
	void lbaWrite(eu32 lba, eu16 secLen, eu8_p buffer);
	void lbaRead(eu32 lba, eu16 secLen, eu8_p buffer);
	void vdrReboot();
	estring genErrorMsg(eu32 startLba, eu32 endLba, eu16 secCnt, eu32 curLba, eu32 step, int howManyStep, eu8_p pWriteBuf, eu8_p pReadBuf);
	estring getDiffStringInTwoBuf(eu32 lbaAddr, eu16 secCnt, eu8_p writeBuf, eu8_p readBuf);

	Utility m_u;

};

