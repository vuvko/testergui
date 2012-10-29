#ifndef WINDOW_H
#define WINDOW_H

#include "manager.h"
#include "IMMPGuiImpl.h"

#include <QtGui>
#include <QDebug>

class Window : public QMainWindow
{
    Q_OBJECT

private:
    QString aPath;
    QString bPath;

    mmp::gui::MMPQtGuiImpl *gui;
    mmp::logic::Manager manager;
    vector<mmp::logic::Position> history;

    int gameId;
    bool running;
    QTimer *timer;
    QProcess *runningApp;

    QVBoxLayout *layout;
    QGridLayout     *constLayout;
    QHBoxLayout         *scoreBoard;
    QLabel                  *leftA;
    QLabel                  *captA;
    QLabel                  *scoreA;
    QLabel                  *scoreB;
    QLabel                  *captB;
    QLabel                  *leftB;
    QPushButton         *controlButton;
    QGridLayout         *fieldContainer;
    FieldWidget             *field;
    QListWidget         *textLog;
    QLabel          *resultExtension;

    QIcon *appIcon;
    QAction *newAction;
    QAction *loadAction;
    QAction *quitAction;

    void createActions();
    void createMenus();

public:
    Window(QWidget *parent = 0);

public slots:
    void chooseAexe(const QString&);
    void chooseBexe(const QString&);
    void newGame();
    void loadGame();

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
};

#endif // WINDOW_H
