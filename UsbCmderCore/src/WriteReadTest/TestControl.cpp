#include "stdafx.h"

#include "TestControl.h"
#include "Utility/Observer.h"

#include "NormalTest.h"

#define DATA_BUFFER_SIZE (_256K)

TestControl::TestControl() {
}

TestControl::~TestControl() {
}

void TestControl::remove_comment(estring& cmd) {
    size_t found = cmd.find(estring(_ET("//")));
    if (found != std::string::npos) {
        cmd = cmd.substr(0, found);
    }
}

bool TestControl::exec_cmd_line(estring cmd, CmderView& view, ScsiIf& usbCmd) {
    remove_comment(cmd);

    if (!cmd.length()) {
        return true;
    }

    if (cmd.length() == 1 && cmd[0] == *"\r") {
        return true;
    }

    size_t leftBracket = cmd.find(estring(_ET("(")));
    if (leftBracket == std::string::npos) {
        SEND_MSG(_ET("leftBracket error"));
        return true;
    }
    size_t rightBracket = cmd.find(estring(_ET(")")));
    if (rightBracket == std::string::npos) {
        SEND_MSG(_ET("rightBracket error"));
        return true;
    }

    estring fun = cmd.substr(0, leftBracket);
    estring param = cmd.substr(leftBracket + 1, rightBracket - 1);

    WriteReadCmp wrc(usbCmd);
    NormalTest nt(wrc);

    if (fun == _ET("seq_write_test")) {

        SeqWriteTestParam params;
        vector<estring> strParams = m_u.stringSplit(param, *_ET(","));

        params.startLba = m_u.toInt(strParams[0]);
        params.endLba = m_u.toInt(strParams[1]);
        params.secCnt = m_u.toInt(strParams[2]);
        params.step = m_u.toInt(strParams[3]);
        params.isNoWrite = m_u.toInt(strParams[4]);
        params.isNoRead = m_u.toInt(strParams[5]);
        return nt.seq_write_test(view, params);
    } 
    
    return true;
}

void TestControl::exec_script(ScsiIf& usbCmd, CmderView& view) {
    estring filePath = _ET("test_script.txt");
    estring script = m_u.getFileData(filePath);
    vector<estring> cmds = m_u.stringSplit(script, *_ET("\n"));
    for (auto cmd : cmds) {
        bool res = exec_cmd_line(cmd, view, usbCmd);

        if (res == false) {
            return;
        }
    }
}