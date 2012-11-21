#include "logic.h"

namespace mmp
{
namespace logic
{

bool isToken(const char c)
{
    return (c == 'A') || (c == 'B');
}

char CharField::at(int x, int y) const
{
    return field[y][x];
}

char CharField::at(const ui::Point &p) const
{
    return field[p.y][p.x];
}

void CharField::set(int x, int y, char c)
{
    field[y][x] = c;
}

void CharField::set(const ui::Point &p, char c)
{
    field[p.y][p.x] = c;
}

int CharField::width() const
{
    return field_width;
}

int CharField::height() const
{
    return field_height;
}

} // end of logic namespace
} // end of mmp namespace
