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

using namespace mmp::ui;

namespace mmp
{
namespace logic
{

GameFight::GameFight() {}

Position GameFight::checkMove(const ui::Point &from, const ui::Point &to)
{
    if (from.x < 0 || from.x >= field_width || from.y < 0 || from.y >= field_height ||
            to.x < 0 || to.y >= field_width || to.x < 0 || to.y >= field_height)
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
    int tokenNum = 1;

    pos.field.set(from, SPACE);
    Point toUpd;
    switch (dir)
    {
    case LEFT_UP:
    case RIGHT_DOWN:
        for (int i = 0; i < field_width; ++i) // нагло предполагаю, что поле квадратное
        {
            if (isToken(pos.field.at(i, i)))
                ++tokenNum;
        }
        toUpd.x = from.x + sign(dir) * tokenNum;
        toUpd.y = from.y - sign(dir) * tokenNum;
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
        for (int i = 0; i < field_width; ++i) // нагло предполагаю, что поле квадратное
        {
            if (isToken(pos.field.at(field_width - i, i)))
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

    if (toUpd.x < 0 || toUpd.y >= field_width || toUpd.x < 0 || toUpd.y >= field_height ||
            pos.field.at(toUpd) == ASTERISK)
    {
        throw Error("Illegal move");
    }

    if (player == FIRST_PLAYER)
        pos.field.set(toUpd, FIRST_LETTER);
    else
        pos.field.set(toUpd, SECOND_LETTER);

    for (int i = -1; i <= 1; ++i)
    {
        if (toUpd.x + i < 0 || toUpd.x + i >= field_width)
            continue;
        for (int j = -1; j <= 1; ++j)
        {
            if (toUpd.y + j < 0 || toUpd.y + j >= field_height || i == 0 && j == 0 ||
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
                if (isToken(pos.field.at(i, j)))
                {
                    if (player == FIRST_PLAYER)
                        ++firstNum;
                    else
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

    ++pos.step;
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

} // end of logic namespace
} // end of mmp namespace
