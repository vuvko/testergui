#include "application.h"

using namespace std;

namespace mmp
{
namespace console
{

ConsoleApplication::ConsoleApplication() :
    aPath(), bPath(), aLeft(30), bLeft(30), process(), manager(),
    history(), gameId(0), hasField(false)
{
}

void ConsoleApplication::setFirstPath(const char *path)
{
    aPath = path;
}

void ConsoleApplication::setSecondPath(const char *path)
{
    bPath = path;
}

void ConsoleApplication::setGameId(int gameId_)
{
    gameId = gameId_;
}

void ConsoleApplication::loadField(const char *path)
{
    QFile oldField("matrix.txt");

    if (oldField.exists())
    {
        if (!oldField.remove())
        {
            cerr << "Can't remove old matrix.txt." << endl;
            return;
        }
    }

    QFile::copy(path, "matrix.txt");
    QFile::setPermissions("matrix.txt",
                          QFile::ReadUser | QFile::WriteUser |
                          QFile::ReadGroup | QFile::WriteGroup |
                          QFile::ReadOther | QFile::WriteOther);
    hasField = true;
}

void ConsoleApplication::setLeftA(double aLeft_)
{
    aLeft = aLeft_;
}

void ConsoleApplication::setLeftB(double bLeft_)
{
    bLeft = bLeft_;
}

string ConsoleApplication::getFirstPath(void) const
{
    return aPath.toStdString();
}

string ConsoleApplication::getSecondPath(void) const
{
    return bPath.toStdString();
}

int ConsoleApplication::getGameId(void) const
{
    return gameId;
}

bool ConsoleApplication::fieldLoaded(void) const
{
    return hasField;
}

int ConsoleApplication::playStep(int player)
{
    if (process.state() != QProcess::NotRunning) {
        cerr << "Process is still running." << endl;
        return -1;
    }
    QString path;
    double timeLeft;
    switch(player)
    {
    case FIRST_PLAYER:
        path = aPath;
        timeLeft = aLeft;
        break;
    case SECOND_PLAYER:
        path = bPath;
        timeLeft = bLeft;
        break;
    default:
        cerr << "Unknown player." << endl;
        return -1;
    }
    process.start(path);
    //cerr << path.toStdString() << " : " << timeLeft << endl;
    if (!process.waitForFinished(timeLeft * 1000)) {
        //cout << "" << endl;
        return 1;
    }

    return 0;
}

} // end of console namespace
} // end of mmp namespace
