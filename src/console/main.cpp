#include <vector>
#include <string>
#include <iomanip>
#include <map>
#include <QTime>

#include <QDebug>

#include "IMMPui.h"
#include "consoleui.h"
#include "application.h"
#include "arg_parser.h"
#include "lerrors.h"

using namespace std;
using namespace mmp::console;
using namespace mmp::logic;

enum
{
    FIRST_PATH = '1',
    SECOND_PATH = '2',
    GAME_TYPE = 'g',
    LOAD_FIELD = 'f',
    KNIGHTS = 'k',
    MMP = 'm',
    FIGHT = 'i',
    HELP = 'h'
};

static const char *FIRST_PATH_LONG = "first_player";
static const char *SECOND_PATH_LONG = "second_player";
static const char *GAME_TYPE_LONG = "game";
static const char *LOAD_FIELD_LONG = "field";
static const char *KNIGHTS_LONG = "knights";
static const char *MMP_LONG = "mmp";
static const char *FIGHT_LONG = "fight";
static const char *HELP_LONG = "help";

typedef pair<char, const char *> command;

void print_help(void)
{
    cerr << "Application....." << endl;
    cerr << "Options:" << endl;

    cerr << " -" << char(FIRST_PATH) << " --" << FIRST_PATH_LONG << endl;
    cerr << "    Declare path to first player." << endl;
    cerr << endl;

    cerr << " -" << char(SECOND_PATH) << " --" << SECOND_PATH_LONG << endl;
    cerr << "    Declare path to second player." << endl;
    cerr << endl;

    cerr << " -" << char(GAME_TYPE) << " --" << GAME_TYPE_LONG << endl;
    cerr << "    Declare game type." << endl;
    cerr << "      1: Knights" << endl;
    cerr << "      2: MMP" << endl;
    cerr << "      3: Fight-B" << endl;
    cerr << endl;

    cerr << " -" << char(LOAD_FIELD) << " --" << LOAD_FIELD_LONG << endl;
    cerr << "    Declare path to field file." << endl;
    cerr << endl;

    cerr << " -" << char(KNIGHTS) << " --" << KNIGHTS_LONG << endl;
    cerr << "    Set game type to 'Knights'" << endl;
    cerr << endl;

    cerr << " -" << char(MMP) << " --" << MMP_LONG << endl;
    cerr << "    Set game type to 'MMP'." << endl;
    cerr << endl;

    cerr << " -" << char(FIGHT) << " --" << FIGHT_LONG << endl;
    cerr << "    Set game type to 'Fight-B'." << endl;
    cerr << endl;

    cerr << " -" << char(HELP) << " --" << HELP_LONG << endl;
    cerr << "    See this output." << endl;
    cerr << endl;
}

void walkover(int player)
{
    if (player == FIRST_PLAYER)
        cout << "B has won!" << endl;
    else
        cout << "A has won!" << endl;
}

int main(int argc, char *argv[])
{
    vector<command> commands;

    const Arg_parser::Option Options[] =
    {
        {FIRST_PATH, FIRST_PATH_LONG, Arg_parser::yes},
        {SECOND_PATH, SECOND_PATH_LONG, Arg_parser::yes},
        {GAME_TYPE, GAME_TYPE_LONG, Arg_parser::yes},
        {KNIGHTS, KNIGHTS_LONG, Arg_parser::no},
        {MMP, MMP_LONG, Arg_parser::no},
        {FIGHT, FIGHT_LONG, Arg_parser::no},
        {LOAD_FIELD, LOAD_FIELD_LONG, Arg_parser::yes},

        {HELP, HELP_LONG, Arg_parser::no}
    };

    const Arg_parser ArgParser(argc, argv, Options);
    if(ArgParser.error().size())
    {
        cerr << "Command line error: " << ArgParser.error() << endl;
        return -1;
    }

    for (uint k = 0; k < ArgParser.arguments(); ++k)
    {
        int code = ArgParser.code(k);

        if (code != 0)
        {
            commands.push_back(command((char)code, ArgParser.argument(k).data()));
        }
    }

    // Setting up application
    ConsoleApplication app;
    for (int i = 0; i < commands.size(); ++i)
    {
        switch (commands[i].first)
        {
        case FIRST_PATH:
            if (app.getFirstPath().empty()) {
                app.setFirstPath(commands[i].second);
            } else {
                cerr << "Path to first player has already been set to '" <<
                        app.getFirstPath() << "'." << endl;
            }
            break;
        case SECOND_PATH:
            if (app.getSecondPath().empty()) {
                app.setSecondPath(commands[i].second);
            } else {
                cerr << "Path to second player has already been set to '" <<
                        app.getSecondPath() << "'." << endl;
            }
            break;
        case GAME_TYPE:
            if (app.getGameId() != 0) {
                cerr << "Game type has already been set to " <<
                        app.getGameId() << " ." << endl;
            } else {
                app.setGameId(atoi(commands[i].second));
            }
            break;
        case LOAD_FIELD:
            // Load game field
            app.loadField(commands[i].second);
            break;
        case KNIGHTS:
            if (app.getGameId() != 0) {
                cerr << "Game type has already been set to " <<
                        app.getGameId() << " ." << endl;
            } else {
                app.setGameId(1);
            }
            break;
        case MMP:
            if (app.getGameId() != 0) {
                cerr << "Game type has already been chosen." << endl;
            } else {
                app.setGameId(2);
            }
            break;
        case HELP:
            print_help();
            return 0;
        default:
            cout << "Unknown option: " << commands[i].first
                 << "; its argument: " << commands[i].second << endl;
            break;
        }
    }

    if (app.getFirstPath().empty())
    {
        cerr << "No first player." << endl;
        return -1;
    }

    if (app.getSecondPath().empty())
    {
        cerr << "No second player." << endl;
        return -1;
    }

    if (!app.fieldLoaded())
    {
        switch (app.getGameId()) {
        case 1:
            app.loadField(":/res/1.txt");
            break;
        case 2:
            app.loadField(":/res/2.txt");
            break;
        case 3:
            app.loadField(":/res/3.txt");
            break;
        default:
            cerr << "Uknown game type." << endl;
            return -1;
        }
    }
    // setting up manager
    Manager manager;
    ConsoleUI cui;

    // begin game
    int status = 0;
    QTime timer;
    Position prevPos, curPos;
    bool first = true;
    int player = 1;
    do {
        double workTime = timer.elapsed() / 1000.0;
        cout << "-------------" << endl;
        if (player == FIRST_PLAYER) {
            cout << "A's turn: ";
        } else {
            cout << "B's turn: ";
        }

        try {
            curPos = manager.parsePos("matrix.txt", app.getGameId());
        } catch (mmp::Error &e) {
            cout << e.message.toStdString() << endl;
            walkover(player);
            return 0;
        }

        string turn;
        if (first) {
            first = false;
        } else {
            try {
               char *tmp = manager.parseTurn(prevPos, curPos, workTime);
               turn = string(tmp);
               delete[] tmp;
            } catch (mmp::Error &e) {
                cout << e.message.toStdString() << endl;
                walkover(player);
                return 0;
            }
        }

        // painting
        cout << turn << endl;
        if (player == FIRST_PLAYER) {
            cout << 'B';
        } else {
            cout << 'A';
        }
        cout << ' ' << curPos.step << endl;
        cout << curPos.scoreA << ' ' << curPos.leftA << endl;
        cout << curPos.scoreB << ' ' << curPos.leftB << endl;
        manager.paintPos(curPos, &cui);
        cout << endl << endl;

        prevPos = curPos;
        app.setLeftA(curPos.leftA);
        app.setLeftB(curPos.leftB);

        timer.restart();
        status = app.playStep(player);
        player ^= 1;
    } while (curPos.state == A_GOES || curPos.state == B_GOES);

    if (status == 1) {
        cout << "Programm was too slow." << endl;
        walkover(player);
    } else {
        switch (curPos.state) {
        case DRAW_GAME:
            cout << "Draw game." << endl;
            break;
        case A_HAS_WON:
            cout << "A has won!" << endl;
            break;
        case B_HAS_WON:
            cout << "B has won!" << endl;
            break;
        default:
            break;
        }
    }

    return 0;
}
