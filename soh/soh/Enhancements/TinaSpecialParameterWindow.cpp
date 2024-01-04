#include "TinaSpecialParameterWindow.h"
#include <ImGui/imgui.h>
#include <libultraship/libultraship.h>

#include <soh/UIWidgets.hpp>

namespace TinaSpecialParameters {
enum messageType { MESSAGE_ERROR, MESSAGE_WARNING, MESSAGE_QUESTION, MESSAGE_INFO, MESSAGE_GRAY_75 };
const ImVec4 messageColor[]{
    { 0.85f, 0.0f, 0.0f, 1.0f },  // MESSAGE_ERROR
    { 0.85f, 0.85f, 0.0f, 1.0f }, // MESSAGE_WARNING
    { 0.0f, 0.85f, 0.85f, 1.0f }, // MESSAGE_QUESTION
    { 0.0f, 0.85f, 0.55f, 1.0f }, // MESSAGE_INFO
    { 0.75f, 0.75f, 0.75f, 1.0f } // MESSAGE_GRAY_75
};
const float enhancementSpacerHeight = 19.0f;

void TinaSpecialParameterWindow::InitElement() {
}

void TinaSpecialParameterWindow::DrawElement() {
    ImGui::SetNextWindowSize(ImVec2(497, 599), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("TinaSpecialParameterWindow", &mIsVisible)) {
        
    }
    ImGui::End();
}

void TinaSpecialParameterWindow::UpdateElement() {
}
} // namespace TinaSpecialParameters
