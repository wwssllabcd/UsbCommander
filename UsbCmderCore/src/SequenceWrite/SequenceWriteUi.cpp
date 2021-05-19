//Copyright © 2006- EricWang(wwssllabcd@gmail.com). All rights reserved

#include "stdafx.h"
#include "SequenceWriteUi.h"
#include "DriverAdapter\UsbCmdStruct.h"

SequenceWriteUi::SequenceWriteUi()
    :m_startLba(0)
    , m_endLba(0)
    , m_step(0)
    , m_secCnt(0)
    , m_noRead(0)
    , m_noWrite(0)
    , m_stop(0) {
}

SequenceWriteUi::~SequenceWriteUi() {
}

void SequenceWriteUi::setDefaultValue(estring_cr startLba, estring_cr endLba, estring_cr secCnt, estring_cr step) {
    m_du.setText(m_startLba, startLba);
    m_du.setText(m_endLba, endLba);
    m_du.setText(m_secCnt, secCnt);
    m_du.setText(m_step, step);
}

void SequenceWriteUi::setItem(CEdit* startLba, CEdit* endLba, CEdit* secCnt, CEdit* step, CButton* noRead, CButton* noWrite, CButton* stop) {
    m_startLba = startLba;
    m_endLba = endLba;
    m_secCnt = secCnt;
    m_step = step;
    m_noRead = noRead;
    m_noWrite = noWrite;
    m_stop = stop;

	setDefaultValue(_ET("0"), _ET("FFFFFFFF"), _ET("80"), _ET("80"));
	if (BYTE_PER_SECTOR == _4K) {
		setDefaultValue(_ET("0"), _ET("FFFFFFFF"), _ET("20"), _ET("20"));
	}
}

void SequenceWriteUi::set_ui_item(CDialog* dialog, SeqWriteUiNum_p numOjb) {
    DialogUtility du;
    setItem(
        du.getDlgItemPtr<CEdit>(dialog, numOjb->startLba)
        , du.getDlgItemPtr<CEdit>(dialog, numOjb->endLba)
        , du.getDlgItemPtr<CEdit>(dialog, numOjb->secCnt)
        , du.getDlgItemPtr<CEdit>(dialog, numOjb->lbaStep)

        , du.getDlgItemPtr<CButton>(dialog, numOjb->noRead)
        , du.getDlgItemPtr<CButton>(dialog, numOjb->noWrite)
        , du.getDlgItemPtr<CButton>(dialog, numOjb->stop)
    );
}

eu32 SequenceWriteUi::getStartLba() {
    return m_du.getHexValue(m_startLba);
}

eu32 SequenceWriteUi::getEndLba() {
    return m_du.getHexValue(m_endLba);
}

void SequenceWriteUi::setEndLba(estring_cr endLba) {
	m_du.setText(m_endLba, endLba);
}

eu32 SequenceWriteUi::getStep() {
    return m_du.getHexValue(m_step);
}

eu16 SequenceWriteUi::getSecCnt() {
    return (eu16)m_du.getHexValue(m_secCnt);
}

bool SequenceWriteUi::isNoRead() {
    return m_u.toBool(m_noRead->GetCheck());
}

bool SequenceWriteUi::isNoWrite() {
    return m_u.toBool(m_noWrite->GetCheck());
}

bool SequenceWriteUi::isStop() {
    return m_u.toBool(m_stop->GetCheck());
}

void SequenceWriteUi::stop() {
	this->m_stop->SetCheck(1);
}


