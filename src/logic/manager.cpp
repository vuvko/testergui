#include "manager.h"

namespace mmp
{
namespace logic
{

Manager::Manager() : game(NULL)
{
    game = 0;
}

Position Manager::parsePos(const char *matrixPath, int gameId)
{
    Position p;

    p.gameId = gameId;

    std::ifstream fin(matrixPath, std::ifstream::in);
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
    if (p.step > 2 * MAX_TURNS + 1)
    {
        //if (!Warning::askToContinue("Turns limit is reached already"))
        //{
            throw Error("Turns limit reached");
        //}
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
        //if (!Warning::askToContinue("Time has already gone"))
        //{
            throw Error("Time has already gone");
        //}
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
            p.field.set(x, y, symbol);
        }
    }
    if (game)
    {
        delete game;
    }

    switch (gameId)
    {
    case 1:
        game = new GameKnights();
        break;
    case 2:
        game = new GameMMP();
        break;
    default:
        throw Error("Unknow game type");
        break;
    }

    game->setPosition(p);
    if (!game->checkField())
    {
        throw Error("Corrupted matrix.txt");
    }

    fin.close();

    return p;
}

char *Manager::parseTurn(Position &p1, Position &p2, double realTime)
{
    // + Time checking
    // |-- Check for incrementing
    if (p2.leftA > p1.leftA + 0.1 || p2.leftB > p1.leftB + 0.1)
    {
        //if (!Warning::askToContinue("Time left number was incremented"))
        //{
            throw Error("Corrupted matrix.txt");
        //}
    }
    // |-+ Check for non-positive
    // |-|-- for first player
    if (p2.leftA < 0)
    {
        //if (!Warning::askToContinue("Time left for A is negative"))
        //{
            p2.leftA = 0;
            throw Error("Corrupted matrix.txt");
        //}
    }
    // |-\-- for second player
    if (p2.leftB < 0)
    {
        //if (!Warning::askToContinue("Time left for B is negative"))
        //{
            p2.leftB = 0;
            throw Error("Corrupted matrix.txt");
        //}
    }

    //  |-- Check for changing other player's time
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
    // \-- Check for real time working
    if ((realTime - workTime > IOTime + eps) || (realTime < workTime + eps))
    {
        throw Error("Corrupted matrx.txt");
    }

    if (p2.step != p1.step + 1)
    {
        //if (!Warning::askToContinue("Current step was not incremented by 1"))
        //{
            p2.step = p1.step + 1;
            throw Error("Corrupted matrix.txt");
        //}
    }

    mmp::ui::Point from = mmp::ui::Point(-1, -1);
    mmp::ui::Point to = mmp::ui::Point(-1, -1);

    /*
     * TODO: do this check as smth like...
     * check what token was moved. Than try to do such move
     * and check whether it will generate the same field and score.
     */
    int kills = 0;
    int pluses = 0;
    int minuses = 0;
    /*
    for (int x = 0; x < field_width; x++)
    {
        for (int y = 0; y < field_height; y++)
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
                            to = mmp::ui::Point(x, y);
                        }
                        kills++;
                    }
                    else
                    {
                        diff[x][y] = '+';
                        to = mmp::ui::Point(x, y);
                        pluses++;
                    }
                }
                else
                {
                    if (p1.field.at(x, y) == 'A' || p1.field.at(x, y) == 'B')
                    {
                        diff[x][y] = '-';
                        from = mmp::ui::Point(x, y);
                        minuses++;
                    }
                }
            }
        }
    }
    */
    // Find the token we moved
    for (int y = 0; y < field_height; ++y)
    {
        for (int x = 0; x < field_width; ++x)
        {
            if (p1.field.at(x,y) != p2.field.at(x,y))
            {
                if (isToken(p2.field.at(x,y)))
                {
                    to = mmp::ui::Point(x, y);
                }
                else
                {
                    from = mmp::ui::Point(x, y);
                }
            }
        }
    }
    /*
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
    */
    game->setPosition(p1);
    Position p3 = game->checkMove(from, to);
    if (p3.scoreA != p2.scoreA || p3.scoreB != p2.scoreB || p3.state != p2.state)
    {
        throw Error("Corrupted matrix.txt");
    }
    for (int y = 0; y < field_height; ++y)
    {
        for (int x = 0; x < field_width; ++x)
        {
            if (p3.field.at(x, y) != p2.field.at(x, y))
            {
                throw Error("Corrupted matrix.txt");
            }
        }
    }

    char *answer = new char[5];
    answer[0] = 'a' + from.x;
    answer[1] = '8' - from.y;
    answer[2] = 'a' + to.x;
    answer[3] = '8' - to.y;
    answer[4] = '\0';

    return answer;
}

void Manager::paintPos(const Position &p, mmp::ui::IMMPui *ui)
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
                field.emptys.push_back(Empty(mmp::ui::Point(x, y)));
                break;
            case 'A':
                field.checkers.push_back(Checker((mmp::ui::Point(x, y)), 0));
                break;
            case 'B':
                field.checkers.push_back(Checker((mmp::ui::Point(x, y)), 1));
                break;
            case '1':
                field.numbers.push_back(Number((mmp::ui::Point(x, y)), 1));
                break;
            case '2':
                field.numbers.push_back(Number((mmp::ui::Point(x, y)), 2));
                break;
            case '*':
                if (p.gameId == 2)
                    field.blocks.push_back(Block(mmp::ui::Point(x, y)));
                if (p.gameId == 3)
                    field.stars.push_back(Star(mmp::ui::Point(x, y)));
                break;
            }
        }
    }

    ui->BeginPaint();
    for (std::list<Star>::iterator s_it = field.stars.begin(); s_it != field.stars.end(); s_it++)
    {
        ui->SetStar(&*s_it);
    }
    for (std::list<Block>::iterator b_it = field.blocks.begin(); b_it != field.blocks.end(); b_it++)
    {
        ui->SetBlock(&*b_it);
    }
    for (std::list<Empty>::iterator e_it = field.emptys.begin(); e_it != field.emptys.end(); e_it++)
    {
        ui->SetEmpty(&*e_it);
    }
    for (std::list<Number>::iterator n_it = field.numbers.begin(); n_it != field.numbers.end(); n_it++)
    {
        ui->SetNumber(&*n_it);
    }
    for (std::list<Checker>::iterator c_it = field.checkers.begin(); c_it != field.checkers.end(); c_it++)
    {
        ui->SetChecker(&*c_it);
    }
    ui->EndPaint();
}


} // end of logic namespace
} // end of mmp namespace
