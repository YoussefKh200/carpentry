#include "basegestionpage.h"

#include <QFrame>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>

BaseGestionPage::BaseGestionPage(const QString &title, const QString &subtitle, QWidget *parent)
    : QWidget(parent), contentLayout_(nullptr)
{
    setAutoFillBackground(true);
    setStyleSheet("background: #ffffff;");

    auto *outer = new QVBoxLayout(this);
    outer->setContentsMargins(20, 20, 20, 20);

    auto *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setStyleSheet("QScrollArea { background: #ffffff; } QWidget { background: #ffffff; }");

    auto *container = new QWidget(scroll);
    auto *layout = new QVBoxLayout(container);
    layout->setSpacing(14);

    auto *titleLabel = new QLabel(title, container);
    titleLabel->setObjectName("PageTitle");

    layout->addWidget(titleLabel);
    Q_UNUSED(subtitle);

    auto *card = new QFrame(container);
    card->setObjectName("PageCard");
    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(16, 16, 16, 16);
    cardLayout->setSpacing(12);

    contentLayout_ = cardLayout;

    layout->addWidget(card);
    layout->addStretch();

    scroll->setWidget(container);
    outer->addWidget(scroll);
}
