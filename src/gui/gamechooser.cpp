#include "gamechooser.h"
#include "ui_gamechooser.h"

GameChooser::GameChooser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameChooser)
{
    ui->setupUi(this);
    setLayout(ui->verticalLayout);
    ui->verticalLayout->setMargin(15);
    ui->verticalLayout->setSpacing(10);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accepting()));
}

GameChooser::~GameChooser()
{
    delete ui;
}

int GameChooser::getType() const
{
    return type;
}

void GameChooser::accepting()
{
    if (ui->type1->isChecked()) {
        type = 1;
    } else if (ui->type2->isChecked()) {
        type = 2;
    } else if (ui->type3->isChecked()) {
        type = 3;
    }

    accept();
}
