#pragma once
#include "afxwin.h"
#include "DefineFiles/EricType.h"
#include "UsbCmder/CmderView.h"
#include "Utility/Utility.h"
#include "WriteReadCmp.h"

typedef struct {
	eu32 startLba;
	eu32 endLba;
	eu8 secCnt;
	eu32 step;
	bool isNoWrite;
	bool isNoRead;
}SeqWriteTestParam;
ERIC_GEN_OTHER_TYPE(SeqWriteTestParam);

class NormalTest
{
public:
	NormalTest(WriteReadCmp& wrc);
	~NormalTest(void);

	bool seq_write_test(CmderView& view, SeqWriteTestParam& params);
	

private:
	WriteReadCmp m_wrc;
	Utility m_u;
};

