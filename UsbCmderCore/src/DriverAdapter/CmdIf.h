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

    void write10(eu32 lba, eu16 secCnt, eu8* buffer) const;
    void read10(eu32 lba, eu16 secCnt, eu8* buffer) const;
    void testUnitReady() const;
    void inquiry(eu8* buffer) const;
    void readCapacity(eu8* buffer) const;
    void vdrReboot(void) const;
    vector<estring> get_device_name(void);
    void send_cmd(UsbCmdStruct cmd, eu8* buffer) const;
    void release_device(void);

    int m_curSel;
    int m_type;

private:
    void send_cmd(eu8* cdb, eu8* buffer, eu32 byteCnt, eu8 direction, estring desc) const;

    eu8 m_fakeDevice[_128K];
};

