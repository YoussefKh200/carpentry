#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>

class Connection
{
public:
    static Connection *instance();

    bool createConnect();
    bool isConnected() const;
    QSqlDatabase database() const;

private:
    Connection();
    Connection(const Connection &) = delete;
    Connection &operator=(const Connection &) = delete;

    static Connection *instance_;
    QSqlDatabase db_;
};

#endif
