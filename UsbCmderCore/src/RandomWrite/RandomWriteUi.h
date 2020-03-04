#pragma once
#include "afxwin.h"
#include "UtilityDialog/DialogUtility.h"
#include "Utility/Utility.h"

using namespace EricCore;

typedef struct {
    int startLba;
    int endLba;
    int ramdomSeed;
    int stopAt;
    int stop;
    int isPending;
	int isFake;
	int isNoRead;
}RdmWriteUiNum;
ERIC_GEN_OTHER_TYPE(RdmWriteUiNum);

class RandomWriteUi
{
public:
	RandomWriteUi();
	~RandomWriteUi();

	eu32 getStartLba();
	eu32 getEndLba();

	eu32 getSeed();
	eu32 getStopAt();
	bool isStop();
	bool isPending();
	bool isFake();
	void setPending();
	void stop();
	bool isNoRead();

	void setItem(CEdit* startLba, CEdit* endLba, CEdit* ramdomSeed, CEdit* stopAt,  CButton* stop, CButton* isPending, CButton* isFake, CButton* isNoRead);
    void set_ui_item(CDialog* dialog, RdmWriteUiNum_p numOjb);
private:
	

	CEdit* m_startLba;
	CEdit* m_endLba;

	CEdit* m_randomSeed;
	CEdit* m_pendingAt;

	CButton* m_stop;
	CButton* m_isPending;
	CButton* m_fake;

	CButton* m_isNoRead;

	DialogUtility m_du;
	Utility m_u;


};

