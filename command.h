#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
struct Arg {
  QString name, value;
  explicit Arg(const QString &name, const QString &value)
      : name(name), value(value) {}
};

struct Command {
  explicit Command(const QString &page, const QString &command_name,QList<Arg>&& args) : page(page),command(command_name), args(args) { }
  QString page, command;
  QList<Arg>& args;
};

#endif // COMMAND_H
