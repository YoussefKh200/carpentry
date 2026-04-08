#include "connection.h"

#include <QDebug>
#include <QSqlError>

Connection *Connection::instance_ = nullptr;

Connection::Connection()
{
}

Connection *Connection::instance()
{
    if (!instance_) {
        instance_ = new Connection();
    }
    return instance_;
}

bool Connection::createConnect()
{
    if (db_.isValid() && db_.isOpen()) {
        return true;
    }

    if (!QSqlDatabase::contains("fullpi_oracle")) {
        db_ = QSqlDatabase::addDatabase("QODBC", "fullpi_oracle");
    } else {
        db_ = QSqlDatabase::database("fullpi_oracle");
    }

    db_.setDatabaseName("PROJET");
    db_.setUserName("projet1");
    db_.setPassword("abc123");

    if (!db_.open()) {
        qWarning() << "Oracle connection failed:" << db_.lastError().text();
        return false;
    }

    qDebug() << "Oracle connection successful";
    return true;
}

bool Connection::isConnected() const
{
    return db_.isValid() && db_.isOpen();
}

QSqlDatabase Connection::database() const
{
    return db_;
}
