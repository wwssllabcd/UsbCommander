#include "stdafx.h"

#include "SimpleFtl.h"
#include "UsbCmdStruct.h"

eu8 m_fakeDevice[_256K];

void lba_read(eu32 lba, eu32 secCnt, eu8_p buffer) {
	memcpy(buffer, m_fakeDevice, secCnt * BYTE_PER_SECTOR);
}

void lba_write(eu32 lba, eu32 secCnt, eu8_p buffer) {
	memcpy((void*)m_fakeDevice, buffer, secCnt * BYTE_PER_SECTOR);
}

void get_ufi_capacity(eu8_p buffer) {
	eu32 cap = 0xF0000000;

	//MSB format
	buffer[0] = ((cap >> 0x18) & 0xFF);
	buffer[1] = ((cap >> 0x10) & 0xFF);
	buffer[2] = ((cap >> 0x08) & 0xFF);
	buffer[3] = ((cap >> 0x00) & 0xFF);
}
