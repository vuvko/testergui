#ifndef ERRORS_H
#define ERRORS_H

#include <QString>
#include <QMessageBox>

namespace mmp
{
namespace logic
{
    struct Error
    {
        QString message;

        Error(const char *txt)
        {
            message = QString(txt);
        }
    };

    struct Warning
    {
        static bool askToContinue(const char *reason)
        {
            QString msg = QString(reason);
            msg.append(". Continue?");

            QMessageBox msgBox;
            msgBox.setText(msg);
            msgBox.setIcon(QMessageBox::Warning);
            QPushButton *continueButton = msgBox.addButton("Continue", QMessageBox::ActionRole);
            QPushButton *abortButton = msgBox.addButton(QMessageBox::Abort);

            msgBox.exec();

            if (msgBox.clickedButton() == (QAbstractButton*)continueButton) {
                return true;
            } else if (msgBox.clickedButton() == (QAbstractButton*)abortButton) {
                return false;
            }

            return false;
        }
    };
} // end of logic namespace
} // end of mmp namespace

#endif // ERRORS_H
