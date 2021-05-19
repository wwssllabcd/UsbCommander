#include "DefineFiles/DefineFile.h"

void lba_read(eu32 lba, eu32 secCnt, eu8_p buffer);
void lba_write(eu32 lba, eu32 secCnt, eu8_p buffer);
void get_ufi_capacity(eu8_p buffer);