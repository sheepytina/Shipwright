#pragma once
#include <libultraship/libultraship.h>

namespace TinaSpecialParameters {
class TinaSpecialParameterWindow : public LUS::GuiWindow {
  public:
    using LUS::GuiWindow::GuiWindow;

    void InitElement() override;
    void DrawElement() override;
    void UpdateElement() override;
};
} // namespace TinaSpecialParameters
