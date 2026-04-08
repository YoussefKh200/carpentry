#ifndef BASEGESTIONPAGE_H
#define BASEGESTIONPAGE_H

#include <QWidget>

class QVBoxLayout;

class BaseGestionPage : public QWidget
{
    Q_OBJECT
public:
    explicit BaseGestionPage(const QString &title, const QString &subtitle, QWidget *parent = nullptr);

protected:
    QVBoxLayout *contentLayout_;
};

#endif
