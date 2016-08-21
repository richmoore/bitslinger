#include "bitslinger.h"

#include "repeatertoolview.h"
#include "ui_repeatertoolview.h"

RepeaterToolView::RepeaterToolView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RepeaterToolView)
{
    ui->setupUi(this);
}

RepeaterToolView::~RepeaterToolView()
{
    delete ui;
}

QByteArray RepeaterToolView::saveState()
{
    return QByteArray();
}

bool RepeaterToolView::restoreState(const QByteArray &state)
{
    Q_UNUSED(state);
    return true;
}

void RepeaterToolView::setBitSlinger(BitSlinger *slinger)
{
    ui->journalView->setJournal(slinger->journal());
}
