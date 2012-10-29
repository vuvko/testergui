#include "IMMPGui.h"

#include "FieldWidget.h"
#include "IMMPGuiImpl.h"

namespace mmp
{
namespace gui
{
    void IMMPGui::ShowError(const std::string error)
    {
        QMessageBox::critical(0, "Fatal Error", QString(error.c_str()), QMessageBox::Ok);
    }

    IMMPGui* IMMPGui::getGui()
    {
        return new MMPQtGuiImpl();
    }
}
}
