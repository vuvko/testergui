#ifndef WINDOW_H
#define WINDOW_H

#include <QtGui>
#include <QThread>
#include <QDebug>
#include <QMessageBox>
#include <QResizeEvent>

#include "manager.h"
#include "IMMPGuiImpl.h"
#include "FieldWidget.h"
#include "ui_window.h"
#include "gamechooser.h"

using namespace mmp::gui;
using namespace mmp::logic;

namespace Ui
{
    class Window;
}

class QProcessThread;

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);
    ~Window();

public slots:
    void chooseAexe(const QString &);
    void chooseBexe(const QString &);
    void newGame();
    void loadGame();
    void appTerminated(int);

private slots:
    void beginStep();
    void endStep(int flag = 0);

    bool updatePosition();
    bool updateLog();
    void stopUpdatingLog();
    void endUpdatingLog();

    void showResult(QString result);
    void walkover(char winner,
                  QString reason);

    void resetGUI();
    void reloadGUI(int showStep = -1);
    void reloadGUIMod(QModelIndex);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    enum
    {
        LABEL_SIZE = 16,
        MARGIN_SIZE = 10,
        SPACING_SIZE = 10,
        MINIMUM_SIZE = 250
    };
    Ui::Window *ui;

    QString aPath;
    QString bPath;

    mmp::gui::MMPQtGuiImpl *gui;
    mmp::logic::Manager manager;
    vector<mmp::logic::Position> history;

    int gameId;
    bool running;
    QTimer *timer;
    //QProcess *runningApp;
    QProcessThread *runningThread;
    mmp::gui::FieldWidget *field;

    void createActions();
    void createMenus();
    void resizeField();
};

class QProcessThread : public QThread
{
    Q_OBJECT

public:
    QProcessThread(
            const QString &path = "",
            int gamer_ = 1,
            double maxTime = 30,
            Window *parent_ = 0) :
        appPath(path),
        gamer(gamer_),
        timeLeft(maxTime),
        app(),
        parent(parent_) {}

signals:
    void appTerminated(int);

protected:
    void run();

private slots:
    void appFinished();

private:
    QString appPath;
    int gamer;
    double timeLeft;
    QProcess *app;
    Window *parent;
};

#endif // WINDOW_H
