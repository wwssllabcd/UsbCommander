#pragma once
#include "DefineFiles/EricType.h"
#include "ScsiUtility/ScsiIf.h"
#include "Utility/Utility.h"

using EricCore::Utility;

typedef struct {
	eu32 lba;
	eu8 secCnt;
}WriteLba;

class WriteReadCmp
{
public:
	WriteReadCmp(const ScsiIf& usbCmd);
	WriteReadCmp(void) {};
	~WriteReadCmp(void) {};

	bool write_read_cmp(eu32 lba, eu8 secCnt, bool isNoWrite, bool isNoRead, eu8_p wBuf, eu8_p rBuf, eu32 step = 0);

	bool verify_lba(vector<WriteLba> lbaColls, eu32 start, eu32 end, eu8_p compBuf, eu8_p readBuf);

	estring get_diff_string_in_two_buffer(eu32 lbaAddr, eu32 byteCnt, eu8_p writeBuf, eu8_p readBuf);

	estring gen_cmp_error_string(eu32 lbaAddr, eu8 secCnt, eu8_p writeBuf, eu8_p readBuf);
	eu32 getCapacity();

	void preCheck(eu16 secCnt, eu32 bufSize);

	void lbaWrite(eu32 lba, eu16 cnt, eu8_p buf);
	void lbaRead(eu32 lba, eu16 cnt, eu8_p buf);

	ScsiIf m_usbCmd;
private:

	
	estring gen_write_msg(bool isNoWrite, eu32 step, eu32 lba, eu8 secCnt);

	
	Utility m_u;
};

