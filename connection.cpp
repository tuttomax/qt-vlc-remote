#include "connection.h"


Connection::Connection(const QString &name, const QString &ip, int port,const QString& password,
                       QObject *parent)
    : QObject(parent), m_name(name), m_ip(ip), m_port(port), m_password(password) {}

QString Connection::name() const { return m_name; }

QString Connection::ip() const { return m_ip; }

int Connection::port() const { return m_port; }

int Connection::status() const { return m_status; }

QJsonObject Connection::context() const { return m_context; }

QString Connection::password() const {return m_password; }

QString Connection::error() const
{
    return m_error;
}

QJsonArray Connection::inodes() const
{
    return m_inodes;
}


void Connection::setName(const QString &value)
{
    if (m_name == value) return;
    m_name = value;
    emit nameChanged();
}

void Connection::setIp(const QString& value)
{
    if (m_ip == value) return;
    m_ip = value;
    emit ipChanged();

}

void Connection::setPort(int value)
{
    if (m_port == value) return;
    m_port = value;
    emit portChanged();
}

void Connection::setStatus(int value)
{
    if (m_status == value) return;
    m_status = value;
    emit statusChanged(this);
}

void Connection::setContext(const QJsonObject& value)
{
    m_context = value;
    emit contextChanged();
}

void Connection::setPassword(const QString &password)
{
    m_password = password;
}

void Connection::setError(const QString &error)
{
    m_error = error;
    emit errorChanged(this);
}

void Connection::setInodes(const QJsonArray &lists)
{
    m_inodes = lists;
    emit inodesChanged(this);
}


bool Connection::operator==(const Connection &other)
{
    return m_ip == other.ip() && m_port== other.port();
}

