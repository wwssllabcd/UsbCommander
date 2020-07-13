#pragma once

#include "DefineFiles/DefineFile.h"
#include <vector>
#include "UsbCmdStruct.h"

using namespace std;

class CmdIf
{
public:
    CmdIf(void);
    ~CmdIf(void);

	void write10(eu32 lba, eu16 secCnt, eu8* buffer);
    void read10(eu32 lba, eu16 secCnt, eu8* buffer);
    void testUnitReady();
    void inquiry(eu8* buffer);
    void readCapacity(eu8* buffer);
    void vdrReboot(void);
    vector<estring> get_device_name(void);
	void send_cmd(UsbCmdStruct cmd, eu8* buffer);
    void release_device(void);

    int m_curSel;
    int m_type;

private:
	void send_cmd(eu8* cdb, eu8* buffer, eu32 byteCnt, eu8 direction, estring_cr desc);

    eu8 m_fakeDevice[_128K];

	void lba_read(eu32 lba, eu32 secCnt, eu8_p buffer);

	void lba_write(eu32 lba, eu32 secCnt, eu8_p buffer);

	void get_ufi_capacity(eu8_p buffer);

};

