#include "TinaSpecialParameterWindow.h"
#include <ImGui/imgui.h>
#include <libultraship/libultraship.h>

#include <soh/UIWidgets.hpp>
#include <Fast3D/gfx_pc.h>

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
        static float aspectRatioX = CVarGetFloat("gDifferentHUDAspect.AspectRatioX", 4.0f);
        static float aspectRatioY = CVarGetFloat("gDifferentHUDAspect.AspectRatioY", 3.0f);

        // UIWidgets::PaddedEnhancementCheckbox("gDifferentHUDAspect.Enabled", "gDifferentHUDAspect.Enabled", false, false, false, "", UIWidgets::CheckboxGraphics::Cross, false);

        if (ImGui::InputFloat("X", &aspectRatioX, 0.1f, 1.0f, "%.3f")) {
            CVarSetFloat("gDifferentHUDAspect.AspectRatioX", aspectRatioX);
            CVarSave();
        }
        if (ImGui::InputFloat("Y", &aspectRatioY, 0.1f, 1.0f, "%.3f")) {
            CVarSetFloat("gDifferentHUDAspect.AspectRatioY", aspectRatioY);
            CVarSave();
        }
        float difference = gfx_current_dimensions.aspect_ratio - aspectRatioX / aspectRatioY;
        {
            ImGui::Text("Game AR: %.2f:1", gfx_current_dimensions.aspect_ratio);
            ImGui::Text(" HUD AR: %.2f:1", aspectRatioX / aspectRatioY);
            ImGui::Text("Difference: %.2f", difference);
        }
        if (ImGui::Button("Apply difference to current margins")) {
            short margin = round(100 * difference);
            // gHUDMargin_T
            CVarSetInteger("gHUDMargin_L", margin); // gHUDMargin_L
            CVarSetInteger("gHUDMargin_R", -margin); // gHUDMargin_R
            // gHUDMargin_B
        }
        ImGui::Text("(You'll need to enable margins in Cosmetics Editor.)");
    }
    ImGui::End();
}

void TinaSpecialParameterWindow::UpdateElement() {
}
} // namespace TinaSpecialParameters
