#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>
#include <string>

#include <QString>
#include <QProcess>
#include <QCoreApplication>
#include <QFile>

#include "logic.h"
#include "manager.h"
#include "consoleui.h"

namespace mmp
{
namespace console
{

class ConsoleApplication
{
    QString aPath;
    QString bPath;
    double aLeft;
    double bLeft;
    QProcess process;

    mmp::logic::Manager manager;
    std::vector<mmp::logic::Position> history;

    int gameId;

    bool hasField;

public:
    ConsoleApplication();
    void setFirstPath(const char *path);
    void setSecondPath(const char *path);
    void setGameId(int gameId_);
    void setLeftA(double aLeft_);
    void setLeftB(double bLeft_);
    void loafField(const char *path);

    std::string getFirstPath(void) const;
    std::string getSecondPath(void) const;
    int getGameId(void) const;
    bool fieldLoaded(void) const;

    int playStep(int player);
};

} // end of console namespace
} // end of mmp namespace

#endif // APPLICATION_H
