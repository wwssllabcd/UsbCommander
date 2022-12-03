#pragma once

#include "DefineFiles/EricType.h"
#include "SequenceWriteUi.h"
#include "ScsiUtility/ScsiIf.h"

#include "Utility/Utility.h"

using namespace EricCore;

typedef struct {
	eu32 sLba;
	eu32 eLba;
	eu32 step;
	eu16 secCnt;
	bool isNoWrite;
	bool isNoRead;
}SeqWriteParam;

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
	SeqWriteParam getSeqParamFromUi(SequenceWriteUi& ui);
	void preCheck(eu16 secCnt, eu32 bufSize);
	bool execSeqOnce(eu32 lba, eu16 secCnt, bool isNoWrite, bool isNoRead, eu8_p wBuf, eu8_p rBuf);
	Utility m_u;

};

