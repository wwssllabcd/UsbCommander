#pragma once
#include "afxwin.h"

#include "Utility/Utility.h"
#include "ScsiUtility/ScsiIf.h"
#include "UsbCmder/CmderView.h"

class TestControl {
public:
    TestControl();
    ~TestControl(void);

    void remove_comment(estring& cmd);

    bool exec_cmd_line(estring cmd, CmderView& view, ScsiIf& usbCmd);
    void exec_script(ScsiIf& usbCmd, CmderView& view);

private:
    Utility m_u;
};
