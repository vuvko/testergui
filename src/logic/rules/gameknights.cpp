#include "gameknights.h"

namespace mmp
{
namespace logic
{

GameKnights::GameKnights() {}

Position GameKnights::checkMove(const mmp::gui::Point &from, const mmp::gui::Point &to)
{
    if (from.x < 0 || from.x >= field_width || from.y < 0 || from.y >= field_height ||
            to.x < 0 || to.y >= field_width || to.x < 0 || to.y >= field_height ||
            qAbs((from.x - to.x) * (from.y - to.y)) != 2)
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
    char letter = FIRST_LETTER;
    if (player == SECOND_PLAYER)
    {
        letter = SECOND_LETTER;
    }
    if (letter == pos.field.at(to))
    {
        throw Error("Illegal move");
    }

    // making move
    pos.field.set(from, SPACE);
    switch (pos.field.at(to))
    {
    case FIRST_LETTER:
    case SECOND_LETTER:
        if (player == FIRST_PLAYER)
        {
            ++pos.scoreA;
        }
        else
        {
            ++pos.scoreB;
        }
        break;
    case MARK1:
        if (player == FIRST_PLAYER)
        {
            ++pos.scoreA;
        }
        else
        {
            ++pos.scoreB;
        }
        break;
    case MARK2:
        if (player == FIRST_PLAYER)
        {
            pos.scoreA += 2;
        }
        else
        {
            pos.scoreB += 2;
        }
        break;
    default:
        break;
    }
    pos.field.set(to, letter);
    if (checkEnd())
    {
        if (pos.scoreA > pos.scoreB)
        {
            pos.state = A_HAS_WON;
        }
        else if (pos.scoreA < pos.scoreB)
        {
            pos.state = B_HAS_WON;
        }
        else
        {
            pos.state = DRAW_GAME;
        }
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

bool GameKnights::checkEnd()
{
    std::vector<mmp::gui::Point> tokensA;
    std::vector<mmp::gui::Point> tokensB;

    for (int y = 0; y < field_height; ++y)
    {
        for (int x = 0; x < field_width; ++x)
        {
            switch (pos.field.at(x, y))
            {
            case FIRST_LETTER:
                tokensA.push_back(mmp::gui::Point(x, y));
                break;
            case SECOND_LETTER:
                tokensB.push_back(mmp::gui::Point(x, y));
                break;
            default:
                break;
            }
        }
    }

    bool vert = true, hor = true, lineA = false, lineB = false;
    int x = tokensA[0].x;
    int y = tokensA[0].y;
    for (int i = 1; (i < tokensA.size()) && (vert || hor); ++i)
    {
        if (tokensA[i].x != x)
        {
            vert = false;
        }
        if (tokensA[i].y != y)
        {
            hor = false;
        }
    }
    lineA = vert || hor;
    x = tokensB[0].x;
    y = tokensB[0].y;
    vert = true;
    hor = true;
    for (int i = 1; (i < tokensB.size()) && (vert || hor); ++i)
    {
        if (tokensB[i].x != x)
        {
            vert = false;
        }
        if (tokensB[i].y != y)
        {
            hor = false;
        }
    }
    lineB = vert || hor;

    if (lineA)
    {
        pos.scoreA += 3;
    }
    if (lineB)
    {
        pos.scoreB += 3;
    }

    return lineA || lineB;
}

bool GameKnights::checkField()
{
    for (int y = 0; y < field_height; ++y)
    {
        for (int x = 0; x < field_width; ++x)
        {
            char sym = pos.field.at(x, y);
            if (sym != FIRST_LETTER && sym != SECOND_LETTER &&
                    sym != MARK1 && sym != MARK2 && sym != SPACE)
            {
                return false;
            }
        }
    }

    return true;
}

} // end of logic namespace
} // end of mmp namespace
