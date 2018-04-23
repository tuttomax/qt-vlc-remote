#ifndef CONNECTIONLISTMODEL_H
#define CONNECTIONLISTMODEL_H

#include "connection.h"
#include <QAbstractListModel>
#include <QList>
#include <QSharedPointer>
#include <vlc.h>
#include <QScopedPointer>

class ConnectionListModel : public QAbstractListModel {
  Q_OBJECT
  enum ConnectionRoles
  {
      ip = Qt::UserRole,
      port,
      name,
      connectionstate,
      context,
      password,
      error
  };


public:
  explicit ConnectionListModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QHash<int, QByteArray> roleNames() const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role) override;

  Q_INVOKABLE void append(const QString& name,const QString& ip,int port,const QString& password);
  Q_INVOKABLE void remove(int i);
  Q_INVOKABLE void edit(int index,const QVariant& value,const QByteArray& role);
  Q_INVOKABLE void save();
  Q_INVOKABLE void load();


  Q_INVOKABLE void connect(int index);
  
  
  
signals:
  void connected(Connection* connection);
  void connectionError(Connection* connection);

private slots:
  void updateConnectionStatus(Connection*);
  void updateConnectionError(Connection*);
private:
  QList<QSharedPointer<Connection>> m_list;
  void m_append(const QSharedPointer<Connection>& connection);
  void m_remove(int i);
  QSharedPointer<VLC> vlc;
};

#endif // CONNECTIONLISTMODEL_H

