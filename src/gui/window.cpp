#include "window.h"

using namespace mmp::gui;
using namespace mmp::ui;
using namespace mmp::logic;

Window::Window(QWidget *parent) : QMainWindow(parent), ui(new Ui::Window)
{
    ui->setupUi(this);
    ui->centralwidget->setLayout(ui->mainLayout);
    ui->fieldFrame->setLayout(ui->fieldLayout);
    ui->mainLayout->setMargin(MARGIN_SIZE);
    ui->mainLayout->setSpacing(SPACING_SIZE);
    ui->fieldLayout->setMargin(MARGIN_SIZE);
    ui->fieldLayout->setSpacing(0);
    ui->resultExtension->setVisible(false);
    ui->mainLayout->setAlignment(ui->controlButton, Qt::AlignCenter);
    ui->mainLayout->setAlignment(ui->fieldFrame, Qt::AlignCenter);
    ui->resizingFrame->setLayout(ui->vertLayout);
    ui->fieldFrame->setFixedSize(MINIMUM_SIZE, MINIMUM_SIZE);

    timer = new QTimer(this);
    runningThread = 0;

    // Default applications' paths
    aPath = "A.exe";
    bPath = "B.exe";

    connect(ui->captA, SIGNAL(linkActivated(QString)), this, SLOT(chooseAexe(QString)));
    connect(ui->captB, SIGNAL(linkActivated(QString)), this, SLOT(chooseBexe(QString)));
    connect(ui->textLog, SIGNAL(clicked(QModelIndex)), this, SLOT(reloadGUIMod(QModelIndex)));
    connect(ui->textLog, SIGNAL(currentRowChanged(int)), this, SLOT(reloadGUI(int)));

    field = new mmp::gui::FieldWidget();
    ui->fieldLayout->addWidget(field, 1, 1, QFieldObject::CELL_NUM, QFieldObject::CELL_NUM);

    // Field
    for (int i = 0; i < QFieldObject::CELL_NUM; ++i)
    {
        QLabel *tmp = new QLabel(QString('a' + i));
        tmp->setAlignment(Qt::AlignCenter);
        tmp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        tmp->setMinimumHeight(LABEL_SIZE);
        ui->fieldLayout->addWidget(tmp, 0, i + 1);
    }
    for (int i = 0; i < QFieldObject::CELL_NUM; ++i)
    {
        QLabel *tmp = new QLabel(QString::number(i + 1));
        tmp->setAlignment(Qt::AlignCenter);
        tmp->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        tmp->setMinimumWidth(LABEL_SIZE);
        ui->fieldLayout->addWidget(tmp, i + 1, 0);
    }

    createActions();
    createMenus();

    //gui = dynamic_cast<MMPQtGuiImpl *>(IMMPGui::getGui());
    gui = new MMPQtGuiImpl;
    if (gui)
    {
        gui->init(field);
    }
    field->setGui(gui);

    QPoint center = QApplication::desktop()->screenGeometry(QApplication::desktop()->primaryScreen()).center();
    center.setX(center.x() - width() / 2);
    center.setY(center.y() - height() / 2);
    move(center);

    resetGUI();
}

Window::~Window()
{
    delete ui;
    delete timer;
    if (runningThread)
        delete runningThread;
    delete field;
}

void Window::createActions()
{
    QObject::connect(ui->actionNew_game, SIGNAL(triggered()), this, SLOT(newGame()));
    QObject::connect(ui->actionLoad_map, SIGNAL(triggered()), this, SLOT(loadGame()));
}

void Window::createMenus()
{
}

void Window::chooseAexe(const QString&)
{
    //QString from = QFileDialog::getOpenFileName(this, "Open file", "", "*.exe");
    QString from = QFileDialog::getOpenFileName(this, "Open file");

    if (!from.isEmpty())
    {
        aPath = from;
        ui->captA->setToolTip(from.split("/").last());
    }
}

void Window::chooseBexe(const QString&)
{
    //QString from = QFileDialog::getOpenFileName(this, "Open file", "", "*.exe");
    QString from = QFileDialog::getOpenFileName(this, "Open file");

    if (!from.isEmpty())
    {
        bPath = from;
        ui->captB->setToolTip(from.split("/").last());
    }
}

void Window::newGame()
{
    GameChooser dial;
    if (dial.exec())
    {
        gameId = dial.getType();
    } else {
        return;
    }

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
    GameChooser dial;
    if (dial.exec())
    {
        gameId = dial.getType();
    } else {
        return;
    }

    QString from = QFileDialog::getOpenFileName(this, "Open file", "", "*.txt");

    if (from.isEmpty()) return;

    if (from != QDir::currentPath().append("/matrix.txt"))
    {
        QFile old("matrix.txt");

        if (old.exists())
        {
            if (!old.remove())
            {
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
            ui->controlButton->disconnect();
            ui->controlButton->setText("Pause after turn");
            ui->controlButton->setEnabled(true);
            connect(ui->controlButton, SIGNAL(clicked()), this, SLOT(waitEnd()));
            reloadGUI();

            running = true;
            runningThread = new QProcessThread(aPath, 1, p.leftA);
            runningThread->start();
            curTime.restart();
            connect(runningThread, SIGNAL(finished()), this, SLOT(endStep()));
            connect(runningThread, SIGNAL(appTerminated(int)), this, SLOT(appTerminated(int)));
        }
        else
        {
            mmp::ui::IMMPui::ShowError("Cant find A.exe!");
        }
        break;
    case mmp::logic::B_GOES:
        if (QFile(bPath).exists())
        {
            ui->controlButton->disconnect();
            ui->controlButton->setText("Pause after turn");
            ui->controlButton->setEnabled(true);
            connect(ui->controlButton, SIGNAL(clicked()), this, SLOT(waitEnd()));
            reloadGUI();

            running = true;
            runningThread = new QProcessThread(bPath, 2, p.leftB);
            runningThread->start();
            curTime.restart();
            connect(runningThread, SIGNAL(finished()), this, SLOT(endStep()));
            connect(runningThread, SIGNAL(appTerminated(int)), this, SLOT(appTerminated(int)));
        }
        else
        {
            mmp::ui::IMMPui::ShowError("Cant find B.exe!");
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
    workTime = curTime.elapsed() / 1000.0; // get time in seconds
    ui->controlButton->disconnect();

    if (!runningThread) return;
    if (!running) return;

    delete runningThread; runningThread = 0;

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
            QString entry = QString::number((p.step - 1) / 2);
            if (p.step % 2 == 0)
            {
                entry.push_back(". start");
                new QListWidgetItem(entry, ui->textLog);
            }
            else
            {
                entry.push_back(". ... start");
                new QListWidgetItem(entry, ui->textLog);
            }
        }
        history.push_back(p);
    }
    catch (mmp::logic::Error &e)
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
            ui->controlButton->setText("Play");
            ui->controlButton->setEnabled(true);
            connect(ui->controlButton, SIGNAL(clicked()), this, SLOT(beginStep()));
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

bool Window::updateLog(bool cont)
{
    mmp::logic::Position *curr = &history.at(history.size() - 1);
    mmp::logic::Position *prev = &history.at(history.size() - 2);

    QString entry("error");
    try
    {
        char *turn = manager.parseTurn(*prev, *curr, workTime);
        entry = QString(turn);
        delete[] turn;
    }
    catch (mmp::logic::Error &e)
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
    else if (curr->state == mmp::logic::A_HAS_WON) entry.push_back("#");
    else if (curr->state == mmp::logic::B_HAS_WON) entry.push_back("#");
    if (curr->step % 2 == 0)
    {
        entry.push_front(". ");
        entry.push_front(QString::number(curr->step/2));
        new QListWidgetItem(entry, ui->textLog);
    }
    else
    {
        entry.push_front(". ... ");
        entry.push_front(QString::number(curr->step/2));
        new QListWidgetItem(entry, ui->textLog);
    }

    //reloadGUI();

    if (cont)
    {
        if (curr->state == mmp::logic::A_GOES || curr->state == mmp::logic::B_GOES)
        {
            ui->controlButton->setText("Pause");
            ui->controlButton->setEnabled(true);
            timer->setSingleShot(true);
            timer->start(TIME_PER_TURN);
            connect(timer, SIGNAL(timeout()), this, SLOT(beginStep()));
            connect(ui->controlButton, SIGNAL(clicked()), this,  SLOT(stopUpdatingLog()));
        }
        else
        {
            ui->controlButton->setText("Play");
            ui->controlButton->setEnabled(false);
        }
    }
    else
    {
        stopUpdatingLog();
    }

    reloadGUI();

    return true;
}

void Window::stopUpdatingLog()
{
    timer->disconnect();
    timer->stop();
    ui->controlButton->disconnect();

    ui->controlButton->setText("Unpause");
    ui->controlButton->setEnabled(true);
    connect(ui->controlButton, SIGNAL(clicked()), this, SLOT(endUpdatingLog()));
}

void Window::endUpdatingLog()
{
    ui->controlButton->disconnect();

    ui->controlButton->setEnabled(false);
    beginStep();
}

void Window::showResult(QString result)
{
    timer->disconnect();
    timer->stop();
    ui->controlButton->disconnect();

    running = false;

    ui->controlButton->setText("Play");
    ui->controlButton->setEnabled(false);
    ui->resultExtension->setText(result);
    ui->resultExtension->setVisible(true);

    resizeField();
}

void Window::walkover(char winner, const QString reason)
{
    timer->disconnect();
    timer->stop();
    ui->controlButton->disconnect();

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
        new QListWidgetItem(entry, ui->textLog);
    }
    else
    {
        QString entry = (". ... w.o.");
        entry.push_front(QString::number(p.step/2));
        new QListWidgetItem(entry, ui->textLog);
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
    ui->controlButton->disconnect();

    if (runningThread && runningThread->isRunning())
    {
        runningThread->exit(1);
        delete runningThread; runningThread = 0;
    }

    running = false;

    ui->resultExtension->setVisible(false);

    history.clear();
    ui->textLog->clear();

    ui->controlButton->setText("Play");
    ui->controlButton->setEnabled(false);

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
        for (int x = 0; x < p.field.width(); x++)
        {
            for (int y = 0; y < p.field.height(); y++)
            {
                p.field.set(x, y, '-');
            }
        }
        p.leftA = 30.0;
        p.leftB = 30.0;
        p.scoreA = 0;
        p.scoreB = 0;
    }

    ui->leftA->setText(QString::number(p.leftA, 'f', 2));
    ui->scoreA->setText(QString::number(p.scoreA));
    ui->scoreB->setText(QString::number(p.scoreB));
    ui->leftB->setText(QString::number(p.leftB, 'f', 2));

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

void Window::resizeField()
{
    int w = ui->resizingFrame->size().width();
    int h = ui->resizingFrame->size().height();

    int s = qMax(int(MINIMUM_SIZE), qMin(w, h));
    field->setSize(s - LABEL_SIZE - 2 * MARGIN_SIZE);
    s = field->getSize() * QFieldObject::CELL_NUM + LABEL_SIZE + 2 * MARGIN_SIZE;
    ui->fieldFrame->setFixedSize(s, s);
    field->repaint();
}

void Window::resizeEvent(QResizeEvent *)
{
    resizeField();

    repaint();
}

void Window::appTerminated(int gamer)
{
    QString msg;
    char win;
    if (gamer == 1)
    {
        msg = "A was too slow";
        win = 'B';
    } else {
        msg = "B was too slow";
        win = 'A';
    }

    mmp::logic::Position p = *history.rbegin();
    p.step++;
    history.push_back(p);
    walkover(win, msg);
}

void Window::waitEnd()
{
    runningThread->disconnect();
    ui->controlButton->disconnect();
    ui->controlButton->setEnabled(false);
    timer->disconnect();
    connect(runningThread, SIGNAL(finished()), this, SLOT(pauseGame()));
    connect(runningThread, SIGNAL(appTerminated(int)), this, SLOT(appTerminated(int)));
}

void Window::pauseGame()
{
    timer->disconnect();
    timer->stop();
    workTime = curTime.elapsed() / 1000.0; // get time in seconds
    ui->controlButton->disconnect();

    if (!runningThread) return;
    if (!running) return;

    delete runningThread; runningThread = 0;

    if (!updatePosition()) return;
    if (!updateLog(false)) return;
}

void QProcessThread::run()
{
    app = new QProcess();
    app->start(appPath);
    app->waitForStarted();
    connect(app, SIGNAL(finished(int)), this, SLOT(appFinished()));
    if (!app->waitForFinished(timeLeft * 1000)) {
        app->disconnect();
        app->kill();
        appTerminated(gamer);
    }
}

void QProcessThread::appFinished()
{
}
