#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include <iostream>
#include <map>
#include <QFile>
#include <QTextStream>

#include "IMMPui.h"
#include "logic.h"

namespace mmp
{
namespace console
{
//! Константы, требующиеся для вывода интерфейса
enum
{
    FIRST_PLAYER = 0,
    SECOND_PLAYER = 1,
    FIRST_LETTER = 'A',
    SECOND_LETTER = 'B',
    SPACE = '-',
    MARK1 = '1',
    MARK2 = '2',
    ASTERISK = '*'
};

//! Класс, предоставляющий консольный интерфейс управления
class ConsoleUI : public mmp::ui::IMMPui
{
    std::vector<std::vector<char> > field;

    // Судя по всему ненужные поля
    QFile logFile;
    QTextStream log;
public:
    ConsoleUI();
    ~ConsoleUI();

    //! Функия открытия файла для логирования
    bool setLogFile(const QString &fileName);
    //! Функция возвращения файла логирования
    QFile getLogFile() const;

    void SetStar(const mmp::ui::Symbol *star);
    void SetBlock(const mmp::ui::Symbol *block);
    void SetEmpty(const mmp::ui::Symbol *empty);
    void SetNumber(const mmp::ui::Token *number);
    void SetChecker(const mmp::ui::Token *checker);

    void Clear();
    void Paint();
    void BeginPaint();
    void EndPaint();
};

} // end of console namespace
} // end of mmp namespace

#endif // CONSOLEUI_H
