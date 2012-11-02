#include "manager.h"

namespace mmp
{
namespace logic
{

char CharField::at(int x, int y) const
{
    return field[x][y];
}

void CharField::set(int x, int y, char c)
{
    field[x][y] = c;
}

int CharField::width() const
{
    return field_width;
}

int CharField::height() const
{
    return field_height;
}

Position Manager::parsePos(const char *matrixPath, int gameId)
{
    Position p;

    p.gameId = gameId;

    ifstream fin(matrixPath, ifstream::in);
    if (!fin.good())
    {
        throw Error("No input file");
    }
    char team;
    fin >> team;
    switch (team)
    {
    case 'A':
        p.state = A_GOES;
        break;
    case 'B':
        p.state = B_GOES;
        break;
    default:
        throw Error("Corrupted team symbol");
    }

    fin >> p.step;
    if (p.step > 2*(30) + 1)
    {
        if (!Warning::askToContinue("Turns limit is reached already"))
        {
            throw Error("Turns limit reached");
        }
    }
    // There is no such rule in game rules
    if ((p.step % 2 == 0 && team != 'B')
     || (p.step % 2 == 1 && team != 'A'))
    {
        throw Error("Error with step/player going");
    }

    char state;
    fin >> state;
    switch (state)
    {
    case 'A':
        p.state = A_HAS_WON;
        break;
    case 'B':
        p.state = B_HAS_WON;
        break;
    case 'D':
        p.state = DRAW_GAME;
        break;
    case 'U':
        break;
    default:
        throw Error("Corrupted state symbol");
    }

    fin >> p.scoreA;
    fin >> p.leftA;
    fin >> p.scoreB;
    fin >> p.leftB;

    if (p.leftA < 0 || p.leftB < 0)
    {
        if (!Warning::askToContinue("Time has already gone"))
        {
            throw Error("Time has already gone");
        }
    }

    char symbol;
    for (int y = 0; y < field_height; ++y)
    {
        for (int x = 0; x < field_width; ++x)
        {
            if (fin.eof())
            {
                throw Error("File has ended too early");
            }
            fin >> symbol;
            switch (symbol)
            {
            case 'A':
            case 'B':
                break;
            case '1':
            case '2':
                if (gameId == 2)
                {
                    throw Error("Numbers detected");
                }
                break;
            case '*':
                if (gameId == 1)
                {
                    throw Error("Asterisks detected");
                }
            case '-':
                break;
            default:
                QString error = "Bad symbol detected: ";
                error.push_back('"');
                error.push_back(symbol);
                error.push_back('"');
                throw Error(error.toStdString().c_str());
            }
            p.field.set(x, y, symbol);
        }
    }

    fin.close();

    return p;
}

char *Manager::parseTurn(Position &p1, Position &p2, double realTime)
{
    char diff[8][8];

    if (p2.leftA > p1.leftA + 0.1 || p2.leftB > p1.leftB + 0.1)
    {
        if (!Warning::askToContinue("Time left number was incremented"))
        {
            throw Error("Corrupted matrix.txt");
        }
    }

    if (p2.leftA < 0)
    {
        if (!Warning::askToContinue("Time left for A is negative"))
        {
            p2.leftA = 0;
            throw Error("Corrupted matrix.txt");
        }
    }

    if (p2.leftB < 0)
    {
        if (!Warning::askToContinue("Time left for B is negative"))
        {
            p2.leftB = 0;
            throw Error("Corrupted matrix.txt");
        }
    }

    double workTime;
    if (p1.state == A_GOES && p2.state == B_GOES)
    {
        if (qAbs(p1.leftB - p2.leftB) > eps)
        {
            throw Error("Corrupted matrix.txt");
        }
        workTime = p1.leftA - p2.leftA;
    }
    else if (p1.state == B_GOES && p2.state == A_GOES)
    {
        if (qAbs(p1.leftA - p2.leftA) > eps)
        {
            throw Error("Corrupted matrix.txt");
        }
        workTime = p1.leftB - p2.leftB;
    }
    else
    {
        throw Error("Corrupted matrix.txt");
    }

    if ((realTime - workTime > IOTime + eps) || (realTime < workTime + eps))
    {
        throw Error("Corrupted matrx.txt");
    }

    if (p2.step != p1.step + 1)
    {
        if (!Warning::askToContinue("Current step was not incremented by 1"))
        {
            p2.step = p1.step + 1;
            throw Error("Corrupted matrix.txt");
        }
    }

    mmp::gui::Point from = mmp::gui::Point(-1, -1);
    mmp::gui::Point to = mmp::gui::Point(-1, -1);

    int kills = 0;
    int pluses = 0;
    int minuses = 0;

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            diff[x][y] = '0';
            if (p1.field.at(x, y) != p2.field.at(x,y))
            {
                if (p2.field.at(x, y) == 'A' || p2.field.at(x, y) == 'B')
                {
                    if (p1.field.at(x, y) == 'A' || p1.field.at(x, y) == 'B')
                    {
                        diff[x][y] = 'x';
                        if (p1.gameId != 2)
                        {
                            to = mmp::gui::Point(x, y);
                        }
                        kills++;
                    }
                    else
                    {
                        diff[x][y] = '+';
                        to = mmp::gui::Point(x, y);
                        pluses++;
                    }
                }
                else
                {
                    if (p1.field.at(x, y) == 'A' || p1.field.at(x, y) == 'B')
                    {
                        diff[x][y] = '-';
                        from = mmp::gui::Point(x, y);
                        minuses++;
                    }
                }
            }
        }
    }

    if (pluses > 1)
    {
        if (!Warning::askToContinue("Too many checkers appeared"))
        {
            throw Error("Corrupted turn");
        }
    }

    if (minuses == 1 && pluses == 1 && kills == 0)
    {
        switch (p1.gameId)
        {
        case 1:
            if (((from.x - to.x)*(from.x - to.x) + (from.y - to.y)*(from.y - to.y)) != 5)
            {
                if (!Warning::askToContinue("Not knight-ish movement"))
                {
                    throw Error("Corrupted turn");
                }
            }
            break;
        case 2:
            if (p1.step % 2 == 1) // A's turn
            {
                if (to.y != (from.y - 1) || (from.x - to.x)*(from.x - to.x) > 1)
                {
                    if (!Warning::askToContinue("Not valid movement"))
                    {
                        throw Error("Corrupted turn");
                    }
                }
            }
            if (p1.step % 2 == 0) // B's turn
            {
                if (to.y != (from.y + 1) || (from.x - to.x)*(from.x - to.x) > 1)
                {
                    if (!Warning::askToContinue("Not valid movement"))
                    {
                        throw Error("Corrupted turn");
                    }
                }
            }
            break;
        }
    }
    else
    {
        switch (p1.gameId)
        {
        case 1:
            if (minuses != 1)
            {
                if (!Warning::askToContinue("Wrong number of checkers missed"))
                {
                    throw Error("Corrupted turn");
                }
            }
            break;
            if ((pluses + kills) != 1)
            {
                if (!Warning::askToContinue("Wrong number of checkers killed/come"))
                {
                    throw Error("Corrupted turn");
                }
            }
            break;
        case 2:
            if (minuses != 1 && minuses != 2)
            {
                if (!Warning::askToContinue("Wrong number of checkers missed"))
                {
                    throw Error("Corrupted turn");
                }
            }
            if (kills > 0)
            {
                if (!Warning::askToContinue("Some checker was steped over"))
                {
                    throw Error("Corrupted turn");
                }
            }
            if (pluses != 1)
            {
                if (!Warning::askToContinue("Wrong number of checkers come"))
                {
                    throw Error("Corrupted turn");
                }
            }
            break;
        case 3:
            if (minuses != 1)
            {
                if (!Warning::askToContinue("Wrong number of checkers missed"))
                {
                    throw Error("Corrupted turn");
                }
            }
            if ((pluses + kills) > 1)
            {
                if (!Warning::askToContinue("Too many checkers killed/come"))
                {
                    throw Error("Corrupted turn");
                }
            }
            break;
        }
    }

    char* answer = new char[5];
    answer[0] = 'a' + from.x;
    answer[1] = '8' - from.y;
    answer[2] = 'a' + to.x;
    answer[3] = '8' - to.y;
    answer[4] = '\0';

    return answer;
}

void Manager::paintPos(const Position& p, mmp::gui::IMMPGui* gui)
{
    Field field;

    for (int x = 0; x < p.field.width(); x++)
    {
        for (int y = 0; y < p.field.height(); y++)
        {
            char symbol = p.field.at(x, y);
            switch (symbol)
            {
            case '-':
                field.emptys.push_back(Empty(mmp::gui::Point(x, y)));
                break;
            case 'A':
                field.checkers.push_back(Checker((mmp::gui::Point(x, y)), 0));
                break;
            case 'B':
                field.checkers.push_back(Checker((mmp::gui::Point(x, y)), 1));
                break;
            case '1':
                field.numbers.push_back(Number((mmp::gui::Point(x, y)), 1));
                break;
            case '2':
                field.numbers.push_back(Number((mmp::gui::Point(x, y)), 2));
                break;
            case '*':
                if (p.gameId == 2)
                    field.blocks.push_back(Block(mmp::gui::Point(x, y)));
                if (p.gameId == 3)
                    field.stars.push_back(Star(mmp::gui::Point(x, y)));
                break;
            }
        }
    }

    gui->BeginPaint();
    for (list<Star>::iterator s_it = field.stars.begin(); s_it != field.stars.end(); s_it++)
    {
        gui->SetStar(&*s_it);
    }
    for (list<Block>::iterator b_it = field.blocks.begin(); b_it != field.blocks.end(); b_it++)
    {
        gui->SetBlock(&*b_it);
    }
    for (list<Empty>::iterator e_it = field.emptys.begin(); e_it != field.emptys.end(); e_it++)
    {
        gui->SetEmpty(&*e_it);
    }
    for (list<Number>::iterator n_it = field.numbers.begin(); n_it != field.numbers.end(); n_it++)
    {
        gui->SetNumber(&*n_it);
    }
    for (list<Checker>::iterator c_it = field.checkers.begin(); c_it != field.checkers.end(); c_it++)
    {
        gui->SetChecker(&*c_it);
    }
    gui->EndPaint();
}


} // end of logic namespace
} // end of mmp namespace
