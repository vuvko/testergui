#ifndef MANAGER_H
#define MANAGER_H

// TODO: separate logic from gui. Specifically make logic namespace do not use gui namespace.
//       It might be usefull when implementing console version... Maybe not to do it...

#include <QFile>
#include <QDebug>

#include <list>
#include <fstream>

#include "lerrors.h"
#include "logic.h"
#include "IMMPui.h"
#include "gamerules.h"
#include "rules/gameknights.h"
#include "rules/gamemmp.h"
#include "rules/gamefight.h"

#define IOTIMECONST 0.5

namespace mmp
{
namespace logic
{

class Manager
{
public:
    Manager();
    Position parsePos(const char *matrixPath, int gameId);
    char *parseTurn(Position &p1, Position &p2, double realTime);
    static void paintPos(const Position &p, mmp::ui::IMMPui *ui);

private:

    // static const double IOTime = 0.5; // Maybe change... // HACK
    const double IOTime; // Maybe change..
    enum {MAX_TURNS = 30};

    GameRules *game;
};

} // end of logic namespace
} // end of mmp namespace

#endif
