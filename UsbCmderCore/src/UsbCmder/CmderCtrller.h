#pragma once


#include "CmderView.h"

#include "ScsiUtility/ScsiIf.h"
#include "ScsiUtility/ScsiFun.h"

#include "Utility/Utility.h"

using namespace EricCore;

class CmderCtrller
{
public:
	CmderCtrller();
	~CmderCtrller();

    void init();
	void execute();
    void refresh();
	void squenceWrite();
	void randomWrite();
	void pageUp();
	void pageDown();
	void close();
	void driverChange();

    CmderView m_view;

private:
    void executeUsbCmd(ScsiCmd cmd);
    eu32 getCapacity();
	eu32 toLba(ScsiCmd cmd);
	void lba_up_down_ctrl(ScsiCmd& curUsbCmd, bool isIncrease);
	void seEndLba();
	ScsiIf get_cmdif();

	eu8 pc_buffer[_256K];
	eu8 readBuf[_64K];
	eu8 writeBuf[_64K];
	ScsiIf m_CmdIf;

	Utility m_u;

	ScsiFun m_scsiFun;
};

