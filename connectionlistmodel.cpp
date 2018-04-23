 #include "connectionlistmodel.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QtDebug>

ConnectionListModel::ConnectionListModel(QObject *parent)
    : QAbstractListModel(parent) {
  vlc = QSharedPointer<VLC>::create();
}

int ConnectionListModel::rowCount(const QModelIndex &parent) const {
  // For list models only the root node (an invalid parent) should return the
  // list's size. For all
  // other (valid) parents, rowCount() should return 0 so that it does not
  // become a tree model.
  Q_UNUSED(parent)
  return m_list.count();
}

QVariant ConnectionListModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) return QVariant();

  const auto &connection = m_list.at(index.row());
  switch (role) {
    case ip:
      return QVariant::fromValue(connection->ip());
    case port:
      return QVariant::fromValue(connection->port());
    case name:
      return QVariant::fromValue(connection->name());
    case connectionstate:
      return QVariant::fromValue(connection->status());
    case context:
      return QVariant::fromValue(connection->context());
    case password:
      return QVariant::fromValue(connection->password());
  }
  return QVariant();
}

void ConnectionListModel::append(const QString &name, const QString &ip,
                                 int port, const QString &password) {
  Connection *connection = new Connection(name, ip, port, password);
  QObject::connect(connection, &Connection::statusChanged, this,
                   &ConnectionListModel::updateConnectionStatus);

  QObject::connect(connection, &Connection::errorChanged, this,
                   &ConnectionListModel::updateConnectionError);

  m_append(QSharedPointer<Connection>(connection));
}

void ConnectionListModel::updateConnectionError(Connection *connection) {
  if (connection->status() == ConnectionStatus::NoConnectionOrError)
    emit connectionError(connection);

          int foundIndex = -1;
          for (int index = 0; index < m_list.length(); index++) {
            const auto &item = m_list[index];
            if (item == connection) foundIndex = index;
          }
          if (foundIndex != -1)
            emit dataChanged(index(foundIndex), index(foundIndex),
                             {ConnectionRoles::error});
}

void ConnectionListModel::remove(int i) { m_remove(i); }

void ConnectionListModel::edit(int c_index, const QVariant &value,
                               const QByteArray &rolename) {
  int role = roleNames().key(rolename);
  setData(index(c_index), value, role);
}
//TODO: Made this work for Android
void ConnectionListModel::save() {
  QDir appdata(
      QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).at(0));
  if (!appdata.exists()) {
    if (appdata.cdUp())
      appdata.mkdir(QCoreApplication::applicationName());
    else
      qCritical() << "Error craeting directory: "
                  << QCoreApplication::applicationName();
  }

  QDir path(QDir(appdata).filePath("connections.local"));
  QFile file(path.path());
  qDebug() << "Loading file: " << file.fileName();
  if (!file.open(QIODevice::WriteOnly))
    qCritical() << "Error opening file: " << file.fileName();
  file.reset();
  QTextStream baseStream(&file);
  baseStream << m_list.count() << '\n';
  for (const auto &item : m_list) {
    baseStream << item->name() << ' ' << item->ip() << ' ' << item->port()
               << ' ' << item->password() << '\n';
    baseStream.flush();
  }
  file.close();
  qDebug() << "Saving was succesfull";
}

//TODO: Made this work for Android
void ConnectionListModel::load() {
  QDir appdata(
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).at(0));
  if (!appdata.exists()) {
    if (appdata.cdUp())
      appdata.mkdir(QCoreApplication::applicationName());
    else
      qCritical() << "Error craeting directory: "
                  << QCoreApplication::applicationName();
  }

  QDir path(QDir(appdata).filePath("connections.local"));
  QFile file(path.path());
  qDebug() << "Loading file: " << file.fileName();
  if (!file.open(QIODevice::ReadOnly))
    qCritical() << "Error opening file: " << file.fileName();
  char c_null;
  int count;
  QTextStream baseStream(&file);
  baseStream >> count >> c_null;
  for (int i = 0; i < count; i++) {
    QString name, ip;
    int port;
    QString password;
    baseStream >> name >> ip >> port >> password >> c_null;
    append(name, ip, port, password);
  }

  file.close();
}

void ConnectionListModel::connect(int m_index) {
  setData(index(m_index), ConnectionStatus::Connecting,
          ConnectionRoles::connectionstate);
  vlc->sendCommand(m_list[m_index].data(), Command("status.json", "", {}));
}

void ConnectionListModel::updateConnectionStatus(Connection *connection) {
  if (connection->status() == ConnectionStatus::Connected)
    emit connected(connection);

  int foundIndex = -1;
  for (int index = 0; index < m_list.length(); index++) {
    const auto &item = m_list[index];
    if (item == connection) foundIndex = index;
  }
  if (foundIndex != -1)
    emit dataChanged(index(foundIndex), index(foundIndex),
                     {ConnectionRoles::connectionstate});
}

void ConnectionListModel::m_append(
    const QSharedPointer<Connection> &connection) {
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  m_list.append(connection);
  endInsertRows();
}

void ConnectionListModel::m_remove(int index) {
  beginRemoveRows(QModelIndex(), index, index);
  m_list.removeAt(index);
  endRemoveRows();
}

Qt::ItemFlags ConnectionListModel::flags(const QModelIndex &index) const {
  Q_UNUSED(index)
  return Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QHash<int, QByteArray> ConnectionListModel::roleNames() const {
  static QHash<int, QByteArray> map(
      {{ConnectionRoles::ip, "ip"},
       {ConnectionRoles::port, "port"},
       {ConnectionRoles::name, "name"},
       {ConnectionRoles::connectionstate, "connectionstate"},
       {ConnectionRoles::context, "context"},
       {ConnectionRoles::password, "password"},
       {ConnectionRoles::error,"error"}});
  return map;
}

bool ConnectionListModel::setData(const QModelIndex &index,
                                  const QVariant &value, int role) {
  if (!index.isValid()) return false;

  auto &item = m_list[index.row()];
  switch (role) {
    case ConnectionRoles::name:
      item->setName(value.toString());
      emit dataChanged(index, index, {name});
      break;
    case ConnectionRoles::ip:
      item->setIp(value.toString());
      emit dataChanged(index, index, {ip});
      break;
    case ConnectionRoles::port:
      item->setPort(value.toInt());
      emit dataChanged(index, index, {port});
      break;
    case ConnectionRoles::connectionstate:
      item->setStatus(value.toInt());
      emit dataChanged(index, index, {connectionstate});
      break;
    case ConnectionRoles::context:
      item->setContext(value.toJsonObject());
      emit dataChanged(index, index, {context});
      break;
    case ConnectionRoles::password:
      item->setPassword(value.toString());
      emit dataChanged(index, index, {password});
      break;
  }
  return true;
}
