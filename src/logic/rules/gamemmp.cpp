#include "gamemmp.h"


namespace mmp
{
namespace logic
{

GameMMP::GameMMP() {}

Position GameMMP::checkMove(const ui::Point &from, const ui::Point &to)
{
    if (from.x < 0 || from.x >= field_width || from.y < 0 || from.y >= field_height ||
            to.x < 0 || to.x >= field_width || to.y < 0 || to.y >= field_height ||
            pos.field.at(to) == ASTERISK)
    {
        throw Error("Illegal move");
    }

    if (!isToken(pos.field.at(from)))
    {
        throw Error("Not a token");
    }

    player = pos.step % 2;
    // check move
    if ((player != FIRST_PLAYER || pos.field.at(from) != FIRST_LETTER) &&
            (player != SECOND_PLAYER || pos.field.at(from) != SECOND_LETTER))
    {
        throw Error("Wrong player");
    }
    char letter = FIRST_LETTER;
    if (player == SECOND_PLAYER)
    {
        letter = SECOND_LETTER;
    }

    // making move
    int move = 0;
    if (from.x < to.x)
    {
        move = 1;
    }
    else if (from.x > to.x)
    {
        move = -1;
    }
    int dy = 1;
    if (player == FIRST_PLAYER)
    {
        dy = -1;
    }
    ui::Point fut(from.x + move, from.y + dy);
    pos.field.set(from, SPACE);
    switch (pos.field.at(fut))
    {
    case FIRST_LETTER:
        if (player == FIRST_PLAYER)
        {
            throw Error("Illegal move");
            break;
        }
        pos.field.set(fut, SPACE);
        fut.x += move;
        fut.y += dy;
        // check for jump
        if (fut.x < 0 || fut.x >= pos.field.width() ||
                fut.y < 0 || fut.y >= pos.field.height() ||
                pos.field.at(fut) != SPACE)
        {
            throw Error("Illegal move");
            break;
        }
        break;
    case SECOND_LETTER:
        if (player == SECOND_PLAYER)
        {
            throw Error("Illegal move");
            break;
        }
        pos.field.set(fut, SPACE);
        fut.x += move;
        fut.y += dy;
        // check for jump
        if (fut.x < 0 || fut.x >= pos.field.width() ||
                fut.y < 0 || fut.y >= pos.field.height() ||
                pos.field.at(fut) != SPACE)
        {
            throw Error("Illegal move");
            break;
        }
        break;
    case ASTERISK:
        throw Error("Illegal move");
        break;
    default:
        break;
    }
    pos.field.set(fut, letter);

    if (!checkEnd())
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

bool GameMMP::checkEnd()
{
    for (int i = 0; i < pos.field.width(); ++i) {
        if (pos.field.at(i, pos.field.height() - 1) == SECOND_LETTER)
        {
            pos.state = B_HAS_WON;
            return true;
        }
        else if (pos.field.at(i, 0) == FIRST_LETTER)
        {
            pos.state = A_HAS_WON;
            return true;
        }
    }

    return false;
}

bool GameMMP::checkField()
{
    for (int y = 0; y < field_height; ++y)
    {
        for (int x = 0; x < field_width; ++x)
        {
            char sym = pos.field.at(x, y);
            if (sym != FIRST_LETTER && sym != SECOND_LETTER &&
                    sym != ASTERISK && sym != SPACE)
            {
                return false;
            }
        }
    }

    return true;
}

} // end of logic namespace
} // end of mmp namespace
