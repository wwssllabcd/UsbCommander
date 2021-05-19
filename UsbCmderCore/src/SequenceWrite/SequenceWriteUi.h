#pragma once
#include "afxwin.h"
#include "UtilityDialog/DialogUtility.h"
#include "Utility/Utility.h"

using namespace EricCore;

typedef struct {
    int startLba;
    int endLba;
    int secCnt;
    int lbaStep;

    int noRead;
    int noWrite;
    int stop;
}SeqWriteUiNum;
ERIC_GEN_POINTER_TYPE(SeqWriteUiNum);

class SequenceWriteUi
{
public:
	SequenceWriteUi();
	~SequenceWriteUi();
    void set_ui_item(CDialog* dialog, SeqWriteUiNum_p numOjb);
	void setItem(CEdit* startLba, CEdit* endLba, CEdit* secCnt, CEdit* step, CButton* noRead, CButton* noWrite, CButton* stop);
	void setDefaultValue(estring_cr startLba, estring_cr endLba, estring_cr secCnt, estring_cr step);
	void setEndLba(estring_cr endLba);
	
	eu32 getStartLba();
	eu32 getEndLba();
	eu32 getStep();
	eu16 getSecCnt();

	bool isNoRead();
	bool isNoWrite();
	bool isStop();
	void stop();

private:
	CEdit* m_startLba;
	CEdit* m_endLba;
	CEdit* m_step;
	CEdit* m_secCnt;

	CButton* m_noRead;
	CButton* m_noWrite;
	CButton* m_stop;

	DialogUtility m_du;
	Utility m_u;
};

