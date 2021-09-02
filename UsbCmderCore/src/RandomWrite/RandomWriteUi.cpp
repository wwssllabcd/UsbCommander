

#include "stdafx.h"
#include "RandomWriteUi.h"

RandomWriteUi::RandomWriteUi()
    :m_startLba(0)
    , m_endLba(0)
    , m_randomSeed(0)
    , m_pendingAt(0)
    , m_stop(0)
    , m_isPending(0) 
	, m_fake(0)
{
}

RandomWriteUi::~RandomWriteUi() {
}

void RandomWriteUi::setItem(CEdit* startLba, CEdit* endLba, CEdit* ramdomSeed, CEdit* stopAt, CButton* stop, CButton* isPending, CButton* isFake, CButton* isNoRead) {
    m_randomSeed = ramdomSeed;
    m_pendingAt = stopAt;
    m_stop = stop;

    m_startLba = startLba;
    m_endLba = endLba;

    m_isPending = isPending;

	m_fake = isFake;

	m_isNoRead = isNoRead;

    m_du.setText(m_randomSeed, _ET("0"));
    m_du.setText(m_pendingAt, _ET("0"));
}

void RandomWriteUi::set_ui_item(CDialog* dialog, RdmWriteUiNum_p numOjb) {
    DialogUtility du;
    setItem(
        du.getDlgItemPtr<CEdit>(dialog, numOjb->startLba)
        , du.getDlgItemPtr<CEdit>(dialog, numOjb->endLba)
        , du.getDlgItemPtr<CEdit>(dialog, numOjb->ramdomSeed)
        , du.getDlgItemPtr<CEdit>(dialog, numOjb->stopAt)

        , du.getDlgItemPtr<CButton>(dialog, numOjb->stop)
        , du.getDlgItemPtr<CButton>(dialog, numOjb->isPending)
		, du.getDlgItemPtr<CButton>(dialog, numOjb->isFake)
		, du.getDlgItemPtr<CButton>(dialog, numOjb->isNoRead)
    );
}

eu32 RandomWriteUi::getSeed() {
    return m_du.getHexValue(m_randomSeed);
}

eu32 RandomWriteUi::getStopAt() {
    return m_du.getHexValue(m_pendingAt);
}

eu32 RandomWriteUi::getStartLba() {
    return m_du.getHexValue(m_startLba);
}

eu32 RandomWriteUi::getEndLba() {
    return m_du.getHexValue(m_endLba);
}

bool RandomWriteUi::isStop() {
    return m_u.toBool(m_stop->GetCheck());
}

bool RandomWriteUi::isPending() {
    return m_u.toBool(m_isPending->GetCheck());
}

bool RandomWriteUi::isNoRead() {
	return m_u.toBool(m_isNoRead->GetCheck());
}

void RandomWriteUi::setPending() {
    m_isPending->SetCheck(1);
}

void RandomWriteUi::stop() {
	this->m_stop->SetCheck(1);
}

bool RandomWriteUi::isFake() {
	return m_u.toBool(m_fake->GetCheck());
}