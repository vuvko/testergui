#include "gamefight.h"
#include <QDebug>

int sign(int x)
{
    if (x < 0)
        return -1;
    if (x == 0)
        return 0;
    return 1;
}

enum
{
    LEFT_UP = -2,
    UP = 1,
    RIGHT_UP = 4,
    RIGHT = 3,
    RIGHT_DOWN = 2,
    DOWN = -1,
    LEFT_DOWN = -4,
    LEFT = -3,
    STAY = 0
};

using namespace mmp::ui;
using namespace std;

namespace mmp
{
namespace logic
{

GameFight::GameFight() {}

Position GameFight::checkMove(const ui::Point &from, const ui::Point &to)
{
    if (from.x < 0 && from.y < 0)
        return findMove();
    if (from.x >= field_width || from.y >= field_height ||
            to.x >= field_width || to.y >= field_height)
    {
        throw Error("Illegal move");
    }
    if (!isToken(pos.field.at(from)))
    {
        throw Error("Not a token");
    }

    player = pos.step % 2;
    if ((player != FIRST_PLAYER || pos.field.at(from) != FIRST_LETTER) &&
            (player != SECOND_PLAYER || pos.field.at(from) != SECOND_LETTER))
    {
        throw Error("Wrong player");
    }

    if (to.x < 0 && to.y < 0)
        return unknownMove(from);

    int dx = to.x - from.x;
    int dy = to.y - from.y;
    if (dy && dx)
    {
        int d = dx / dy;
        if (qAbs(d) != 1 || dx != dy * d)
        {
            throw Error("Illegal move");
        }
    }
    int dir = -sign(dy) + 3 * sign(dx);
    qDebug() << "Diraction: " << dir;

    pos.field.set(from, SPACE);

    Point toUpd = moveFrom(from, dir);

    moveTo(toUpd);

    return pos;
}

bool GameFight::checkEnd()
{
    for (int i = 0; i < field_width; ++i) {
        for (int j = 0; j < field_height; ++j) {
            if (pos.field.at(i, j) == ASTERISK) {
                return false;
            }
        }
    }

    return true;
}

bool GameFight::checkField()
{
    for (int i = 0; i < field_width; ++i) {
        for (int j = 0; j < field_height; ++j) {
            char sym = pos.field.at(i, j);
            if (sym != FIRST_LETTER && sym != SECOND_LETTER &&
                    sym != ASTERISK && sym != SPACE &&
                    sym != MARK1 && sym != MARK2) {
                return false;
            }
        }
    }

    return true;
}

Point GameFight::moveFrom(const Point &from, int dir) const
{
    Point toUpd;
    int tokenNum = 1;
    switch (dir)
    {
    case LEFT_UP:
    case RIGHT_DOWN:
        for (int i = -field_width; i < field_width; ++i) // нагло предполагаю, что поле квадратное
        {
            Point p(from.x + i, from.y + i);
            if (p.x < 0 || p.y < 0 || p.x >= field_width || p.y >= field_height)
                continue;
            if (isToken(pos.field.at(p)))
                ++tokenNum;
        }
        toUpd.x = from.x + sign(dir) * tokenNum;
        toUpd.y = from.y + sign(dir) * tokenNum;
        break;
    case UP:
    case DOWN:
        for (int i = 0; i < field_width; ++i) // нагло предполагаю, что поле квадратное
        {
            if (isToken(pos.field.at(from.x, i)))
                ++tokenNum;
        }
        toUpd.x = from.x;
        toUpd.y = from.y - sign(dir) * tokenNum;
        break;
    case RIGHT_UP:
    case LEFT_DOWN:
        for (int i = -field_width; i < field_width; ++i) // нагло предполагаю, что поле квадратное
        {
            Point p(from.x + i, from.y - i);
            if (p.x < 0 || p.y < 0 || p.x >= field_width || p.y >= field_height)
                continue;
            if (isToken(pos.field.at(p)))
                ++tokenNum;
        }
        toUpd.x = from.x + sign(dir) * tokenNum;
        toUpd.y = from.y - sign(dir) * tokenNum;
        break;
    case RIGHT:
    case LEFT:
        for (int i = 0; i < field_width; ++i) // нагло предполагаю, что поле квадратное
        {
            if (isToken(pos.field.at(i, from.y)))
                ++tokenNum;
        }
        toUpd.x = from.x + sign(dir) * tokenNum;
        toUpd.y = from.y;
        break;
    default:
        throw Error("Illegal move");
        break;
    }

    qDebug() << "Tokens:" << tokenNum;

    return toUpd;
}

void GameFight::moveTo(const Point &toUpd)
{
    if (toUpd.x < 0 || toUpd.y >= field_width || toUpd.x < 0 || toUpd.y >= field_height ||
            pos.field.at(toUpd) == ASTERISK)
    {
        throw Error("Illegal move");
    }

    if (player == FIRST_PLAYER)
        pos.field.set(toUpd, FIRST_LETTER);
    else
        pos.field.set(toUpd, SECOND_LETTER);

    qDebug() << toUpd.x << toUpd.y;

    for (int i = -1; i <= 1; ++i)
    {
        if (toUpd.x + i < 0 || toUpd.x + i >= field_width)
            continue;
        for (int j = -1; j <= 1; ++j)
        {
            if (toUpd.y + j < 0 || toUpd.y + j >= field_height || (i == 0 && j == 0) ||
                    isToken(pos.field.at(toUpd.x + i, toUpd.y + j)))
                continue;
            if (player == FIRST_PLAYER)
                pos.field.set(toUpd.x + i, toUpd.y + j, MARK1);
            else
                pos.field.set(toUpd.x + i, toUpd.y + j, MARK2);
        }
    }

    if (checkEnd())
    {
        int firstNum = 0, secondNum = 0;
        for (int i = 0; i < field_width; ++i)
            for (int j = 0; j < field_height; ++j)
            {
                if (pos.field.at(i, j) == MARK1)
                    ++firstNum;
                else if (pos.field.at(i, j) == MARK2)
                    ++secondNum;
            }
        if (firstNum > secondNum)
            pos.state = A_HAS_WON;
        else if (firstNum == secondNum)
            pos.state = DRAW_GAME;
        else
            pos.state = B_HAS_WON;
    }
    else
    {
        if (player == FIRST_PLAYER)
        {
            pos.state = B_GOES;
        }
        else
        {
            pos.state = A_GOES;
        }
    }

    int countFirst = 0, countSecond = 0;
    for (int j = 0; j < field_height; ++j)
    {
        for (int i = 0; i < field_width; ++i)
        {
            if (pos.field.at(i, j) == MARK1)
                ++countFirst;
            else if (pos.field.at(i, j) == MARK2)
                ++countSecond;
        }
    }

    pos.scoreA = countFirst;
    pos.scoreB = countSecond;

    ++pos.step;

}

Position GameFight::unknownMove(const Point &from)
{
    oldPos = pos;
    oldFrom = from;

    pos.step = -1;
    return pos;
}

Position GameFight::findMove(void)
{
    Position newPos = pos;
    bool found = false;
    for (int dir = -4; dir <= 4 && !found; ++dir)
    {
        if (dir == 0)
            continue;
        setPosition(oldPos);
        pos.field.set(oldFrom, SPACE);
        found = true;
        Point toUpd = moveFrom(oldFrom, dir);
        try
        {
            moveTo(toUpd);
        }
        catch (mmp::Error &)
        {
            found = false;
            continue;
        }

        if (newPos.state != pos.state || newPos.scoreA != pos.scoreA ||
                newPos.scoreB != pos.scoreB)
        {
            found = false;
            continue;
        }
        for (int i = 0; i < field_width; ++i)
            for (int j = 0; j < field_height; ++j)
                if (newPos.field.at(i, j) != pos.field.at(i, j))
                {
                    found = false;
                    continue;
                }
    }

    if (found)
        return pos;

    pos.step = -1;
    return pos;
}

} // end of logic namespace
} // end of mmp namespace
