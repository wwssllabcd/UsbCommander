#include "stdafx.h"

#include "NormalTest.h"
#include "Utility/Observer.h"

#define DATA_BUFFER_SIZE (_256K)




NormalTest::NormalTest(WriteReadCmp& wrc) {
	this->m_wrc = wrc;
}

NormalTest::~NormalTest() {

}

bool NormalTest::seq_write_test(CmderView& view, SeqWriteTestParam& params){
	eu8 readBuf[DATA_BUFFER_SIZE];	
	eu8 writeBuf[DATA_BUFFER_SIZE];

	eu32 startLba = params.startLba;
	eu32 endLba = params.endLba;
	eu8 secCnt = params.secCnt;
	eu32 step = params.step;
	bool isNoWrite = params.isNoWrite;
	bool isNoRead = params.isNoRead;

	if (endLba == NULL_32) {
		endLba = m_wrc.getCapacity();
    }

	estring msg;
	estring baseParam = m_u.strFormat(_ET("seq_write_test, S=%X, E=%X, sp=%X, sc=%X, noW=%X, noR=%X"), startLba, endLba, step, secCnt, isNoWrite, isNoRead) + m_u.crLf();

	for (eu32 curLba = startLba; (curLba + secCnt) <= endLba; curLba += step) {
		DialogUtility::update_message();
		if (view.m_seqWriteUi.isStop()) {
			SEND_MSG(_ET("User Stop"));
			return false;
		}

		SEND_MSG_ESTR(baseParam);

		bool res = m_wrc.write_read_cmp(curLba, secCnt, isNoWrite, isNoRead, writeBuf, readBuf, curLba);

		if (res) {
			SEND_MSG(_ET("Verify ... Pass"));
		} else {
			SEND_MSG(_ET("Verify ... fail"));
			estring msg = m_wrc.gen_cmp_error_string(curLba, secCnt, writeBuf, readBuf);
			SEND_MSG_ESTR_CTRL(msg, true, false);
			return false;
		}
	}
	SEND_MSG(_ET("Finish"));
	return true;
}

