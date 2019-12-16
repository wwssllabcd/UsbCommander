#pragma once
//Copyright © 2006- EricWang(wwssllabcd@gmail.com). All rights reserved

#include "CmderView.h"
#include "DriverAdapter/CmdIf.h"

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

    CmderView m_view;

private:
    void executeUsbCmd(UsbCmdStruct cmd);
    eu32 getCapacity();
	eu32 toLba(UsbCmdStruct cmd);
	void lba_up_down_ctrl(UsbCmdStruct& curUsbCmd, bool isIncrease);
    CmdIf get_cmdif();

	eu8 pc_buffer[_256K];
	eu8 readBuf[_64K];
	eu8 writeBuf[_64K];
    CmdIf m_CmdIf;

	Utility m_u;
};

