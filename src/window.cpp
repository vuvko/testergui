#include "window.h"

#include <QMessageBox>

Window::Window(QWidget *parent) : QMainWindow(parent)
{
    /*
     * BIG initialization part... MB just move it to window.ui file... It's TOO BIG
     */
    timer = new QTimer(this);
    runningApp = NULL;
    // Time-left labels
    leftA = new QLabel;
    leftA->setAlignment(Qt::AlignCenter);
    leftA->setFont(QFont("Calibri", 11));

    leftB = new QLabel;
    leftB->setAlignment(Qt::AlignCenter);
    leftB->setFont(QFont("Calibri", 11));

    // Default applications' paths
    aPath = "A.exe";
    bPath = "B.exe";

    // Players' labels
    captA = new QLabel("<a href=Red><font color=#ff0000>Red</font></a>");
    captA->setTextFormat(Qt::RichText);
    captA->setToolTip("A.exe");
    captA->setAlignment(Qt::AlignCenter);
    captA->setFont(QFont("Calibri", 13));
    connect(captA, SIGNAL(linkActivated(QString)), this, SLOT(chooseAexe(QString)));

    captB = new QLabel("<a href=Blue><font color=#0000ff>Blue</font></a>");
    captB->setToolTip("B.exe");
    captB->setAlignment(Qt::AlignCenter);
    captB->setFont(QFont("Calibri", 13));
    connect(captB, SIGNAL(linkActivated(QString)), this, SLOT(chooseBexe(QString)));

    // Score labels
    scoreA = new QLabel;
    scoreA->setAlignment(Qt::AlignCenter);
    scoreA->setFont(QFont("Calibri", 13));

    QLabel *scoreDelim = new QLabel(":");
    scoreDelim->setAlignment(Qt::AlignCenter);
    scoreDelim->setMaximumWidth(3);

    scoreB = new QLabel;
    scoreB->setAlignment(Qt::AlignCenter);
    scoreB->setFont(QFont("Calibri", 13));

    scoreBoard = new QHBoxLayout;
    scoreBoard->addWidget(leftA);
    scoreBoard->addWidget(captA);
    scoreBoard->addWidget(scoreA);
    scoreBoard->addWidget(scoreDelim);
    scoreBoard->addWidget(scoreB);
    scoreBoard->addWidget(captB);
    scoreBoard->addWidget(leftB);

    // Game log visualization
    textLog = new QListWidget();
    textLog->setFixedWidth(110);
    connect(textLog, SIGNAL(clicked(QModelIndex)), this, SLOT(reloadGUIMod(QModelIndex)));
    connect(textLog, SIGNAL(currentRowChanged(int)), this, SLOT(reloadGUI(int)));

    fieldContainer = new QGridLayout;
    field = new FieldWidget();
    fieldContainer->addWidget(field, 1, 1, 8, 8);

    // Field Coordinates
    // TODO: do smth with this...
    QLabel *tmp = NULL;
    tmp = new QLabel("a"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 0, 1, 1, 1);
    tmp = new QLabel("b"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 0, 2, 1, 1);
    tmp = new QLabel("c"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 0, 3, 1, 1);
    tmp = new QLabel("d"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 0, 4, 1, 1);
    tmp = new QLabel("e"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 0, 5, 1, 1);
    tmp = new QLabel("f"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 0, 6, 1, 1);
    tmp = new QLabel("g"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 0, 7, 1, 1);
    tmp = new QLabel("h"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 0, 8, 1, 1);
    tmp = new QLabel("a"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 9, 1, 1, 1);
    tmp = new QLabel("b"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 9, 2, 1, 1);
    tmp = new QLabel("c"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 9, 3, 1, 1);
    tmp = new QLabel("d"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 9, 4, 1, 1);
    tmp = new QLabel("e"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 9, 5, 1, 1);
    tmp = new QLabel("f"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 9, 6, 1, 1);
    tmp = new QLabel("g"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 9, 7, 1, 1);
    tmp = new QLabel("h"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 9, 8, 1, 1);
    tmp = new QLabel("8"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 1, 0, 1, 1);
    tmp = new QLabel("7"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 2, 0, 1, 1);
    tmp = new QLabel("6"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 3, 0, 1, 1);
    tmp = new QLabel("5"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 4, 0, 1, 1);
    tmp = new QLabel("4"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 5, 0, 1, 1);
    tmp = new QLabel("3"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 6, 0, 1, 1);
    tmp = new QLabel("2"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 7, 0, 1, 1);
    tmp = new QLabel("1"); tmp->setAlignment(Qt::AlignCenter); fieldContainer->addWidget(tmp, 8, 0, 1, 1);
    tmp = NULL;

    // Control button
    controlButton = new QPushButton("Play");
    controlButton->setEnabled(false);

    // Result label
    resultExtension = new QLabel("You shouldn't see this text!");
    resultExtension->setFont(QFont("Calibri", 14));
    resultExtension->setAlignment(Qt::AlignCenter);

    constLayout = new QGridLayout;
    constLayout->addLayout(scoreBoard, 1, 0, 1, 2);
    constLayout->addWidget(controlButton, 1, 2, 1, 1);
    constLayout->addLayout(fieldContainer, 2, 0, 1, 2);
    constLayout->addWidget(textLog, 2, 2, 1, 1);

    layout = new QVBoxLayout;
    layout->addLayout(constLayout);
    layout->addWidget(resultExtension);

    createActions();
    createMenus();

    using namespace mmp::gui;
    gui = dynamic_cast<MMPQtGuiImpl *>(IMMPGui::getGui());
    if (gui)
    {
        gui->init(field);
    }
    field->setGui(gui);

    QWidget* blankWidget = new QWidget;
    setCentralWidget(blankWidget);
    centralWidget()->setLayout(layout);

    QPoint center = QApplication::desktop()->screenGeometry(QApplication::desktop()->primaryScreen()).center();
    // FIXME: change to application size
    center.setX(center.x() - 120);
    center.setY(center.y() - 200);
    move(center);

    QIcon appIcon;
    appIcon.addFile(":/res/tray.png", QSize(16, 16));
    appIcon.addFile(":/res/app.png", QSize(32, 32));
    setWindowIcon(appIcon);

    setWindowTitle(tr("MMP Board"));

    resetGUI();
}

void Window::createActions()
{
    newAction = new QAction(tr("&New game..."), this);
    newAction->setIcon(QIcon(":/res/new.png"));
    newAction->setShortcut(Qt::ALT + Qt::Key_N);

    loadAction = new QAction(tr("&Load map..."), this);
    loadAction->setIcon(QIcon(":/res/load.png"));
    loadAction->setShortcut(Qt::ALT + Qt::Key_L);

    quitAction = new QAction(tr("&Quit"), this);
    quitAction->setIcon(QIcon(":/res/exit.png"));
    quitAction->setShortcut(Qt::ALT + Qt::Key_Q);

    QObject::connect(newAction, SIGNAL(triggered()), this, SLOT(newGame()));
    QObject::connect(loadAction, SIGNAL(triggered()), this, SLOT(loadGame()));
    QObject::connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Window::createMenus()
{
    QMenu *mainMenu = menuBar()->addMenu("Main");

    mainMenu->addAction(newAction);
    mainMenu->addAction(loadAction);
    mainMenu->addSeparator();
    mainMenu->addAction(quitAction);
}

void Window::chooseAexe(const QString&)
{
    //QString from = QFileDialog::getOpenFileName(this, "Open file", "", "*.exe");
    QString from = QFileDialog::getOpenFileName(this, "Open file");

    if (!from.isEmpty())
    {
        aPath = from;
        captA->setToolTip(from.split("/").last());
    }
}

void Window::chooseBexe(const QString&)
{
    //QString from = QFileDialog::getOpenFileName(this, "Open file", "", "*.exe");
    QString from = QFileDialog::getOpenFileName(this, "Open file");

    if (!from.isEmpty())
    {
        bPath = from;
        captB->setToolTip(from.split("/").last());
    }
}

void Window::newGame()
{
    bool ok;
    gameId = QInputDialog::getInt(this, "New game", "Select game type:", 1, 1, 3, 1, &ok);

    if (!ok) return;

    QFile old("matrix.txt");

    if (old.exists())
    {
        if (!old.remove())
        {
            QMessageBox::critical(this, "Error", "Can't remove old matrix.txt");
            return;
        }
    }

    switch (gameId)
    {
    case 1:
        QFile::copy(":res/1.txt", "matrix.txt"); break;
    case 2:
        QFile::copy(":res/2.txt", "matrix.txt"); break;
    case 3:
        QFile::copy(":res/3.txt", "matrix.txt"); break;
    }

    QFile::setPermissions("matrix.txt",
                          QFile::ReadUser | QFile::WriteUser |
                          QFile::ReadGroup | QFile::WriteGroup |
                          QFile::ReadOther | QFile::WriteOther);

    resetGUI();
    updatePosition();
}

void Window::loadGame()
{
    bool ok;
    gameId = QInputDialog::getInt(this, "Load map", "Select game type:", 1, 1, 3, 1, &ok);

    if (!ok) return;

    QString from = QFileDialog::getOpenFileName(this, "Open file", "", "*.txt");

    if (from.isEmpty()) return;

    if (from != QDir::currentPath().append("/matrix.txt"))
    {
        QFile old("matrix.txt");

        if (old.exists())
        {
            if (!old.remove())
            {
                // TODO: check this on windows
                int perm = old.permissions();
                qDebug() << perm;
                QMessageBox::critical(this, "Error", "Can't remove old matrix.txt");
                return;
            }
        }

        QFile::copy(from, "matrix.txt");

        QFile::setPermissions("matrix.txt",
                              QFile::ReadUser | QFile::WriteUser |
                              QFile::ReadGroup | QFile::WriteGroup |
                              QFile::ReadOther | QFile::WriteOther);
    }

    resetGUI();
    updatePosition();
}

void Window::beginStep()
{
    timer->disconnect();
    timer->stop();

    mmp::logic::Position p = *history.rbegin();

    switch (p.state)
    {
    case mmp::logic::A_GOES:
        if (QFile(aPath).exists())
        {
            controlButton->disconnect();
            controlButton->setText("Running...");
            controlButton->setEnabled(false);
            reloadGUI();

            running = true;
            runningApp = new QProcess();
            runningApp->start(aPath);
            runningApp->waitForStarted();
            connect(runningApp, SIGNAL(finished(int)), this, SLOT(endStep(int)));
            if (!runningApp->waitForFinished(p.leftA * 1000))
            {
                runningApp->disconnect();
                runningApp->kill();
                running = false;
                mmp::logic::Position p = *history.rbegin();
                p.step++;
                history.push_back(p);
                walkover('B', "A was too slow");
            }
        }
        else
        {
            mmp::gui::IMMPGui::ShowError("Cant find A.exe!");
        }
        break;
    case mmp::logic::B_GOES:
        if (QFile(bPath).exists())
        {
            controlButton->disconnect();
            controlButton->setText("Running...");
            controlButton->setEnabled(false);
            reloadGUI();

            running = true;
            runningApp = new QProcess();
            runningApp->start(QString(bPath));
            runningApp->waitForStarted();
            connect(runningApp, SIGNAL(finished(int)), this, SLOT(endStep(int)));
            if (!runningApp->waitForFinished(p.leftB * 1000))
            {
                runningApp->disconnect();
                runningApp->kill();
                running = false;
                mmp::logic::Position p = *history.rbegin();
                p.step++;
                history.push_back(p);
                walkover('A', "B was too slow");
            }
        }
        else
        {
            mmp::gui::IMMPGui::ShowError("Cant find B.exe!");
        }
        break;
    default:
        //QMessageBox::information(this, "Warning", "Window::beginStep() is called when the game has already ended!");
        break;
    }
}

void Window::endStep(int)
{
    timer->disconnect();
    timer->stop();
    controlButton->disconnect();

    if (!runningApp) return;
    if (!running) return;

    runningApp = NULL;

    if (!updatePosition()) return;
    if (!updateLog()) return;
}

bool Window::updatePosition()
{
    try
    {
        mmp::logic::Position p = manager.parsePos("matrix.txt", gameId);
        if (history.empty())
        {
            QString entry = QString::number((p.step-1)/2);
            if (p.step % 2 == 0)
            {
                entry.push_back(". start");
                new QListWidgetItem(entry, textLog);
            }
            else
            {
                entry.push_back(". ... start");
                new QListWidgetItem(entry, textLog);
            }
        }
        history.push_back(p);
    }
    catch (mmp::logic::Error& e)
    {
        QString msg = e.message;

        if (history.empty())
        {
            QMessageBox::critical(this, "Error loading map", msg);
            return false;
        }

        mmp::logic::Position p = *history.rbegin();

        char winner = '?';

        if (msg == "Turns limit reached")
        {
            msg.append(". Draw game!");
            showResult(msg);
        }
        else
        {
            if (p.step % 2 == 0)
            {
                winner = 'A';
            }
            else
            {
                winner = 'B';
            }
            mmp::logic::Position p = *history.rbegin();
            p.step++;
            history.push_back(p);
            walkover(winner, msg);
        }

        return false;
    }

    mmp::logic::Position p = *history.rbegin();

    switch (p.state)
    {
    case mmp::logic::A_GOES:
    case mmp::logic::B_GOES:
        if (!running)
        {
            controlButton->setText("Play");
            controlButton->setEnabled(true);
            connect(controlButton, SIGNAL(clicked()), this, SLOT(beginStep()));
        }
        break;
    case mmp::logic::DRAW_GAME:
        showResult("Draw game!");
        break;
    case mmp::logic::A_HAS_WON:
        showResult("Player A wins the game!");
        break;
    case mmp::logic::B_HAS_WON:
        showResult("Player B wins the game!");
        break;
    }

    reloadGUI();

    return true;
}

bool Window::updateLog()
{
    mmp::logic::Position *curr = &history.at(history.size() - 1);
    mmp::logic::Position *prev = &history.at(history.size() - 2);

    QString entry("error");
    try
    {
        char *turn = manager.parseTurn(*prev, *curr);
        entry = QString(turn);
        delete[] turn;
    }
    catch (mmp::logic::Error& e)
    {
        QString msg = e.message;

        mmp::logic::Position p = *history.rbegin();

        char winner = '?';

        if (p.step % 2 == 0)
        {
            winner = 'B';
        }
        else
        {
            winner = 'A';
        }
        walkover(winner, msg);

        return false;
    }

    if (curr->state == mmp::logic::DRAW_GAME) entry.push_back("=");
    if (curr->state == mmp::logic::A_HAS_WON) entry.push_back("#");
    if (curr->state == mmp::logic::B_HAS_WON) entry.push_back("#");
    if (curr->step % 2 == 0)
    {
        entry.push_front(". ");
        entry.push_front(QString::number(curr->step/2));
        new QListWidgetItem(entry, textLog);
    }
    else
    {
        entry.push_front(". ... ");
        entry.push_front(QString::number(curr->step/2));
        new QListWidgetItem(entry, textLog);
    }

    reloadGUI();

    if (curr->state == mmp::logic::A_GOES || curr->state == mmp::logic::B_GOES)
    {
        controlButton->setText("Pause");
        controlButton->setEnabled(true);
        timer->setSingleShot(true);
        timer->start(666);
        connect(timer, SIGNAL(timeout()), this, SLOT(beginStep()));
        connect(controlButton, SIGNAL(clicked()), this,  SLOT(stopUpdatingLog()));
    }
    else
    {
        controlButton->setText("Play");
        controlButton->setEnabled(false);
    }

    reloadGUI();

    return true;
}

void Window::stopUpdatingLog()
{
    timer->disconnect();
    timer->stop();
    controlButton->disconnect();

    controlButton->setText("Unpause");
    connect(controlButton, SIGNAL(clicked()), this, SLOT(endUpdatingLog()));
}

void Window::endUpdatingLog()
{
    controlButton->disconnect();

    controlButton->setEnabled(false);
    beginStep();
}

void Window::showResult(QString result)
{
    timer->disconnect();
    timer->stop();
    controlButton->disconnect();

    running = false;

    controlButton->setText("Play");
    controlButton->setEnabled(false);
    resultExtension->setText(result);
    resultExtension->setVisible(true);
}

void Window::walkover(char winner, const QString reason)
{
    timer->disconnect();
    timer->stop();
    controlButton->disconnect();

    mmp::logic::Position p = *history.rbegin();
    if (winner == 'A')
    {
        p.scoreA = 3;
        p.scoreB = 0;
    }
    else
    {
        p.scoreA = 0;
        p.scoreB = 3;
    }
    *history.rbegin() = p;

    if (p.step % 2 == 0)
    {
        QString entry = (". w.o.");
        entry.push_front(QString::number(p.step/2));
        new QListWidgetItem(entry, textLog);
    }
    else
    {
        QString entry = (". ... w.o.");
        entry.push_front(QString::number(p.step/2));
        new QListWidgetItem(entry, textLog);
    }

    QString result = reason;
    result.append(". ");
    result.append(winner);
    result.append(" wins! (Walkover)");

    showResult(result);
}

void Window::resetGUI()
{
    timer->disconnect();
    timer->stop();
    controlButton->disconnect();

    if (runningApp)
    {
        runningApp->kill();
        runningApp->waitForFinished();
        delete runningApp;
        runningApp = NULL;
    }

    running = false;

    resultExtension->setVisible(false);

    history.clear();
    textLog->clear();

    controlButton->setText("Play");
    controlButton->setEnabled(false);

    textLog->setFixedHeight(field->height());
    // TODO: Remove fixing size. Do it after fixing layouts.
    centralWidget()->adjustSize();
    adjustSize();
    setFixedSize(size());

    reloadGUI();
}

void Window::reloadGUI(int showStep)
{
    mmp::logic::Position p;
    if (!history.empty())
    {
        if (showStep == -1)
        {
            p = *history.rbegin();
        }
        else
        {
            if (unsigned (showStep) >= history.size())
            {
                QMessageBox::warning(this, "Error",
                                    "Tried to show \"after history\"-moment."
                                    "Showing last instead.");
                p = *history.rbegin();
            }
            else
            {
                p = history.at(showStep);
            }
        }
    }
    else
    {
        for (int x = 0; x < p.map.width(); x++)
        {
            for (int y = 0; y < p.map.height(); y++)
            {
                p.map.set(x, y, '-');
            }
        }
        p.leftA = 30.0;
        p.leftB = 30.0;
        p.scoreA = 0;
        p.scoreB = 0;
    }

    leftA->setText(QString::number(p.leftA, 'f', 2));
    scoreA->setText(QString::number(p.scoreA));
    scoreB->setText(QString::number(p.scoreB));
    leftB->setText(QString::number(p.leftB, 'f', 2));

    field->setUpdatesEnabled(false);
    manager.paintPos(p, gui);
    field->setUpdatesEnabled(true);
    field->repaint();

    repaint();
}

void Window::reloadGUIMod(QModelIndex i)
{
    reloadGUI(i.row());
}
