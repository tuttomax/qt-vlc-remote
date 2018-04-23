#ifndef CONNECTION_H
#define CONNECTION_H

#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include <QString>

enum ConnectionStatus {
  Connected = 0,
  Connecting = 1,
  NoConnectionOrError = 2
};
class Connection : public QObject {
  Q_OBJECT

  Q_ENUM(ConnectionStatus)

  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(QString ip READ ip WRITE setIp NOTIFY ipChanged)
  Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
  Q_PROPERTY(int status READ status WRITE setStatus NOTIFY statusChanged)
  Q_PROPERTY(
      QJsonObject context READ context WRITE setContext NOTIFY contextChanged)
    Q_PROPERTY(QJsonArray inodes READ inodes WRITE setInodes NOTIFY inodesChanged)
  Q_PROPERTY(QString error READ error WRITE setError NOTIFY errorChanged)
 public:
  Connection(const QString& name, const QString& ip, int port,
             const QString& password, QObject* parent = nullptr);

  QString name() const;
  QString ip() const;
  int port() const;
  int status() const;
  QJsonObject context() const;
  QString password() const;
  QString error() const;
    QJsonArray inodes() const;
  void setName(const QString& value);
  void setIp(const QString& value);
  void setPort(int value);
  void setStatus(int value);
  void setContext(const QJsonObject& value);
  void setPassword(const QString& password);
  void setError(const QString& error);
  void setInodes(const QJsonArray& inodes);

  inline bool operator==(const Connection& other);

 signals:
  void nameChanged();
  void ipChanged();
  void portChanged();
  void statusChanged(Connection*);
  void contextChanged();
  void errorChanged(Connection*);
    void inodesChanged(Connection*);
 private:
  QString m_name, m_ip;
  int m_port;
  int m_status = NoConnectionOrError;
  QJsonObject m_context;
  QJsonArray m_inodes;
  QString m_password;
  QString m_error;
};

#endif  // CONNECTION_H
