#pragma once

#include "DefineFiles/DefineFile.h"


#include <vector>

using namespace std;

class UsbCmdStruct;

typedef vector<UsbCmdStruct> UsbCmdSet;

#define BYTE_PER_SECTOR                 (512)
#define SECTOR_TO_BYTE(SEC)             ((SEC) * BYTE_PER_SECTOR)

#define UFI_OP_READ_10                  (0x28)
#define UFI_OP_WRITE_10                 (0x2A)
#define UFI_OP_READ_CAPACITY            (0x25)

class UsbCmdStruct
{
public:
	UsbCmdStruct(void);
	~UsbCmdStruct(void);

	eu8 cdb[16];
	int length;
	eu8 direction;
	estring description;

	bool operator ==(const UsbCmdStruct& rhs) const;


	UsbCmdSet getAllCommandSet(void);

	UsbCmdStruct requestSense(void);
	UsbCmdStruct inquiry(void);
	UsbCmdStruct readCapacity(void);
	UsbCmdStruct testUnitReady(void);
	UsbCmdStruct readFormatCapacity(void);
	
	UsbCmdStruct preventAllowMediumRemoval(void);

	UsbCmdStruct read10(void);
	UsbCmdStruct read10(eu32 lba, eu16 secCnt);
	UsbCmdStruct write10(void);
	UsbCmdStruct write10(eu32 lba, eu16 secCnt);

	UsbCmdStruct vdrReboot(void);

};

