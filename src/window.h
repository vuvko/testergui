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
    QProcess *runningApp;
    mmp::gui::FieldWidget *field;

    void createActions();
    void createMenus();
    void resizeField();
};

#endif // WINDOW_H
