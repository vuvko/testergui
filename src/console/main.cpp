//#include <QCoreApplication>
#include <vector>
#include <string>
#include <QTime>

#include <QDebug>

#include "IMMPui.h"
#include "consoleui.h"
#include "application.h"
#include "arg_parser.h"

using namespace std;
using namespace mmp::console;
using namespace mmp::logic;

enum
{
    FIRST_PATH = '1',
    SECOND_PATH = '2',
    NEW_GAME = 'g',
    LOAD_FIELD = 'f',
    KNIGHTS = 'k',
    MMP = 'm'
};

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    //QCoreApplication::setOrganizationName("Nizhibitsky Eugene");
    //QCoreApplication::setApplicationName("MMP Board");
    
    //return a.exec();

    typedef pair<char, const char *> command;

    vector<command> commands;

    const Arg_parser::Option Options[] =
    {
        {FIRST_PATH, "first_player", Arg_parser::yes},
        {SECOND_PATH, "second_player", Arg_parser::yes},
        {NEW_GAME, "game", Arg_parser::yes},
        {KNIGHTS, "knights", Arg_parser::no},
        {MMP, "mmp", Arg_parser::no},
        {LOAD_FIELD, "field", Arg_parser::yes}
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
        /*
        else
        {
            if (sSourcePath.empty())
                sSourcePath = ArgParser.argument(k);
            else if (sResultPath.empty())
                sResultPath = ArgParser.argument(k);
            else
            {
                cerr<<"invalid unnamed command-line argument"<<endl;
                return -1;
            }
        }
        */
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
        case NEW_GAME:
            if (app.getGameId() != 0) {
                cerr << "Game type has already been set to " <<
                        app.getGameId() << " ." << endl;
            } else {
                app.setGameId(atoi(commands[i].second));
            }
            break;
        case LOAD_FIELD:
            // Load game field
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
        default:
            cout << "Unknown option: " << commands[i].first
                 << "; its argument: " << commands[i].second << endl;
            break;
        }
    }


    app.setFirstPath("/home/andrey/CompPrac/game-knights");
    app.setSecondPath("/home/andrey/CompPrac/game-knights2");
    app.setGameId(1);



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
            app.loafField(":/res/1.txt");
            break;
        case 2:
            app.loafField(":/res/2.txt");
            break;
        case 3:
            app.loafField(":/res/3.txt");
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
    int status;
    QTime timer;
    Position prevPos, curPos;
    bool first = true;
    int player;
    for (player = 1; !(status = app.playStep(player)); player ^= 1) {
        double workTime = timer.elapsed() / 1000.0;
        cout << "-------------" << endl;
        if (player == FIRST_PLAYER) {
            cout << "A's turn." << endl;
        } else {
            cout << "B's turn." << endl;
        }

        try {
            curPos = manager.parsePos("matrix.txt", app.getGameId());
        } catch (Error &e) {
            cerr << e.message.toStdString() << endl;
            return 0;
            // some error.
        }

        switch (curPos.state) {
        case DRAW_GAME:
            cout << "Draw game." << endl;
            return 0;
        case A_HAS_WON:
            cout << "A has won!" << endl;
            return 0;
        case B_HAS_WON:
            cout << "B has won!" << endl;
            return 0;
        default:
            break;
        }

        string turn;
        if (first) {
            first = false;
        } else {
            try {
               char *tmp = manager.parseTurn(prevPos, curPos, workTime);
               turn = tmp;
               delete tmp;
            } catch (Error &e) {
                cerr << e.message.toStdString() << endl;
                return 0;
                // some error
            }
        }
        // painting
        //cout << "| A " << curPos.scoreA << " : " << curPos.scoreB << " B |" << endl;
        //cout << "| " << curPos.leftA << " | " << curPos.leftB << " |" << endl;

        cout << curPos.step << endl;
        cout << curPos.scoreA << ' ' << curPos.leftA << endl;
        cout << curPos.scoreB << ' ' << curPos.leftB << endl;
        manager.paintPos(curPos, &cui);
        cout << endl << endl;

        prevPos = curPos;
        app.setLeftA(curPos.leftA);
        app.setLeftB(curPos.leftB);

        timer.restart();
    }

    if (status == 1) {
        cout << "Programm was too slow." << endl;
        if (player == FIRST_PLAYER) {
            cout << "B has won!" << endl;
        } else {
            cout << "A has won!" << endl;
        }
    }

    return 0;
}