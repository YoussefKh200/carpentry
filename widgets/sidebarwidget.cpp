#include "sidebarwidget.h"

#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

SidebarWidget::SidebarWidget(QWidget *parent) : QFrame(parent)
{
    setFixedWidth(260);
    setObjectName("Sidebar");

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 22, 16, 22);
    layout->setSpacing(12);

    auto *logo = new QLabel(this);
    logo->setAlignment(Qt::AlignCenter);
    logo->setPixmap(QPixmap(":/img/LOGO.png").scaled(165, 145, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    layout->addWidget(logo);
    layout->addSpacing(12);

    addButton("Gestion Utilisateurs", 0);
    addButton("Gestion Clients", 1);
    addButton("Gestion Commandes", 2);
    addButton("Gestion Fournisseurs", 3);
    addButton("Gestion Materiaux", 4);

    layout->addStretch();
    setActiveIndex(0);

    setStyleSheet(
        "QFrame#Sidebar {"
        "  background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #d7b88f, stop:0.45 #c7a377, stop:1 #b28b61);"
        "  border-right: 1px solid #a27e59;"
        "}"
        "QPushButton {"
        "  text-align:left; padding: 12px 14px; border-radius: 10px; border: 1px solid #b6926b;"
        "  color: #2f2318; font-weight: 700; background: #f8f2e9;"
        "}"
        "QPushButton:checked { background: #4d8fc9; color: #ffffff; border: 1px solid #3b76a8; }"
        "QPushButton:hover { background: #efe4d4; }"
    );
}

void SidebarWidget::addButton(const QString &text, int index)
{
    auto *button = new QPushButton(text, this);
    button->setCheckable(true);
    buttons_.append(button);
    layout()->addWidget(button);
    connect(button, &QPushButton::clicked, this, [this, index]() {
        setActiveIndex(index);
        emit navigateTo(index);
    });
}

void SidebarWidget::setActiveIndex(int index)
{
    for (int i = 0; i < buttons_.size(); ++i) {
        buttons_[i]->setChecked(i == index);
    }
}
