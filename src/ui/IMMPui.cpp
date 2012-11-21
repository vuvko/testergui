#include "IMMPui.h"

namespace mmp
{
namespace ui
{
    void IMMPui::ShowError(const std::string error)
    {
        QMessageBox::critical(0, "Fatal Error", QString(error.c_str()), QMessageBox::Ok);
    }
}
}
