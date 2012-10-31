#ifndef GAMECHOOSER_H
#define GAMECHOOSER_H

#include <QDialog>

namespace Ui {
class GameChooser;
}

class GameChooser : public QDialog
{
    Q_OBJECT

public slots:
    void accepting();
    
public:
    explicit GameChooser(QWidget *parent = 0);
    int getType() const;
    ~GameChooser();
    
private:
    Ui::GameChooser *ui;
    int type;
};

#endif // GAMECHOOSER_H
