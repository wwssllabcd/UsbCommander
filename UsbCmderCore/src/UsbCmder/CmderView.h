#pragma once

#include <afxcmn.h>  //CTabCtrl

#include "SequenceWrite/SequenceWriteUi.h"
#include "RandomWrite/RandomWriteUi.h"

#include "DefineFiles/DefineFile.h"
#include "UtilityDialog/DialogUtility.h"
#include "Utility/Utility.h"

#include "ScsiUtility/ScsiCmd.h"

using namespace EricCore;



#define MAX_CDB_OF_UI (16)
typedef struct {
    int cdb[MAX_CDB_OF_UI];
    int devceSel;
    int refresh;
    int execute;
    int cmdSel;
    int dataIn;
    int dataOut;
    int dataLen;
	int message;
	int sysInfo;

	int asciiMessage;

}UsbCmderMfcUiNum;

class CmderView
{
public:
	CmderView();
	~CmderView();
	
	void init(void);
    void setup_ui_item(int deviceSel, int cmdSel, int* srcId_cdb, int dataOut, int dataIn, int dataLen, int mainMsg, int asciiMsg, int sysInfo);
    void setup_ui_num_to_view(UsbCmderMfcUiNum* const uiNumObj);

	void setDlgPointer(CDialog* dialogPtr);
	int getDriveSel(void);
    void set_device_box(vector<estring> deviceName);

	void driveSelectChange();
	void selectChange();
	ScsiCmd loadCmdSetFromUI(eu32 partialSetCtrl = NULL_32);
	int getShiftNo(bool isIncrease);
	void setLbaToUI(const ScsiCmd& cmdset);

	estring getHandleString();
	void closeCheckBtn(void);
	void sendMsgToDialogArea(bool isClean, estring_cr msg);
	void sendMsgToAsciiArea(bool isClean, estring_cr msg);
	void showPopupWindows(estring_cr msg);

	DialogUtility m_du;
	Utility m_u;


	SequenceWriteUi m_seqWriteUi;
	RandomWriteUi m_rdmWriteUi;

private:
	CDialog* m_pDialog;

	// Ui Mapping
	CEdit* m_cdbs[MAX_CDB_OF_UI];
	CEdit* m_pDataLen;
	CButton* m_pDataIn;
	CButton* m_pDataOut;
	CComboBox* m_cboCmdSel;
	CComboBox* m_cboDriveSel;

	CEdit* m_dialogMsg;
	CEdit* m_asciiMsg;
	CStatic* m_lblSysMsg;

	
	ScsiCmdSet m_totalCmdSet;
	ScsiCmdSet loadTotalCmdSet();

	void setCmdSetToUI(const ScsiCmd& objCmd, eu32 partialSetCtrl = NULL_32);
	void loadExtCmdSet(ScsiCmdSet& totalCmdSet);
	void setTotalCmdToForm(const ScsiCmdSet& totalCmdSet);
	void setDataLenToUi(eu32 length);
	int getDataLenFromUi();
	void setDirectionToUi(ScsiIoDir direction);
	ScsiIoDir getDirectionFromUi(void);
	void sendMsgBase(CEdit* pMsgArea, bool isClean, estring_cr msg);
};

