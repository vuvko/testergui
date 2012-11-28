#include "consoleui.h"

using namespace std;
using namespace mmp::ui;
using namespace mmp::logic;

namespace mmp
{
namespace console
{

ConsoleUI::ConsoleUI() :
    field(field_width, vector<char>(field_height, SPACE)), logFile(), log()
{
    /*
    for (int i = 0; i < field_width; ++i)
    {
        for (int j = 0; j < field_height; ++j)
        {
            field[i][j] = SPACE;
        }
    }
    */
}

ConsoleUI::~ConsoleUI()
{
    logFile.close();
}

void ConsoleUI::SetStar(const Symbol *star)
{
    if (!star) return;
    Point p = star->getPoint();
    //cerr << "(" << p.x << ", " << p.y << ") -> " << ASTERISK << endl;
    field[p.x][p.y] = ASTERISK;
}

void ConsoleUI::SetBlock(const Symbol *block)
{
    if (!block) return;
    Point p = block->getPoint();
    //cerr << "(" << p.x << ", " << p.y << ") -> " << ASTERISK << endl;
    field[p.x][p.y] = ASTERISK;
}

void ConsoleUI::SetEmpty(const Symbol *empty)
{
    if (!empty) return;
    Point p = empty->getPoint();
    //cerr << "(" << p.x << ", " << p.y << ") -> " << SPACE << endl;
    field[p.x][p.y] = SPACE;
}

void ConsoleUI::SetNumber(const Token *number)
{
    if (!number) return;
    Point p = number->getPoint();
    char c = MARK1;
    if (number->getTeamId() != 1)
    {
        c = MARK2;
    }
    //cerr << "(" << p.x << ", " << p.y << ") -> " << char(c) << endl;
    field[p.x][p.y] = c;
}

void ConsoleUI::SetChecker(const Token *checker)
{
    if (!checker) return;
    Point p = checker->getPoint();
    char c = FIRST_LETTER;
    if (checker->getTeamId() != FIRST_PLAYER)
    {
        c = SECOND_LETTER;
    }
    //cerr << "(" << p.x << ", " << p.y << ") -> " << char(c) << endl;
    field[p.x][p.y] = c;
}

void ConsoleUI::Clear()
{
    for (int i = 0; i < field_width; ++i)
    {
        for (int j = 0; j < field_height; ++j)
        {
            field[i][j] = SPACE;
        }
    }
}

void ConsoleUI::BeginPaint()
{
    Clear();
    //cerr << "begin painting" << endl;
}

void ConsoleUI::EndPaint()
{
    //cerr << "end painting" << endl;
    Paint();
}

void ConsoleUI::Paint()
{
    //cout << "*****************" << endl;
    //cout << "New turn." << endl;
    /*
    cout << '/';
    for (int i = 0; i < field_width; ++i) {
        cout << '^';
    }
    cout << '\\' <<  endl;
    */
    cout << endl;
    cout << "  ";
    for (int i = 0; i < field_width; ++i) {
        cout << char('a' + i);
    }
    cout << endl << endl;

    for (int i = 0; i < field_height; ++i)
    {
        //cout << '|';
        cout << field_height - i << ' ';
        for (int j = 0; j < field_width; ++j)
        {
            cout << char(field[j][i]);
        }
        //cout << '|' <<  endl;
        cout << endl;
    }
    /*
    cout << '\\';
    for (int i = 0; i < field_width; ++i) {
        cout << '_';
    }
    cout << '/' << endl;
    */
}

} // end of console namespace
} // end of mmp namespace
