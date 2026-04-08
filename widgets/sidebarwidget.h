#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include <QFrame>
#include <QList>

class SidebarWidget : public QFrame
{
    Q_OBJECT
public:
    explicit SidebarWidget(QWidget *parent = nullptr);
    void setActiveIndex(int index);

signals:
    void navigateTo(int index);
    void logoutRequested();

private:
    void addButton(const QString &text, int index);
    QList<class QPushButton *> buttons_;
    class QPushButton *settingsBtn_;
    class QPushButton *logoutBtn_;
};

#endif
