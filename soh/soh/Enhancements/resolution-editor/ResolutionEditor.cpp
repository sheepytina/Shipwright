#include "ResolutionEditor.h"
#include <ImGui/imgui.h>
#include <libultraship/libultraship.h>

#include <soh/UIWidgets.hpp> // This dependency is why the UI needs to be on SoH's side.
#include <graphic/Fast3D/gfx_pc.h>
#include <AdvancedResolution.h>

namespace AdvancedResolutionSettings {
enum setting { UPDATE_aspectRatioX, UPDATE_aspectRatioY, UPDATE_verticalPixelCount, UPDATE_integerScaleFactor };

const char* aspectRatioPresetLabels[] = { "Off", "Custom", "4:3", "16:9", "5:3 (Nintendo 3DS)", "16:10 (8:5)" };
const float aspectRatioPresetsX[] = { 0.0f, 12.0f, 4.0f, 16.0f, 5.0f, 16.0f };
const float aspectRatioPresetsY[] = { 0.0f, 9.0f, 3.0f, 9.0f, 3.0f, 10.0f };
const int default_AspectRatio = 1; // Default combo list option

const char* pixelCountPresetLabels[] = { "Custom",     "Native N64 (240p)", "2x (480p)",       "3x (720p)", "4x (960p)",
                                         "5x (1200p)", "6x (1440p)",        "Full HD (1080p)", "4K (2160p)" };
const int pixelCountPresets[] = { 480, 240, 480, 720, 960, 1200, 1440, 1080, 2160, 480 };
const int default_pixelCount = 0;           // Default combo list option
const uint32_t minVerticalPixelCount = 240; // see: LUS::AdvancedResolution()
const uint32_t maxVerticalPixelCount = 4320;

const unsigned short default_maxIntegerScaleFactor = 6;

void AdvancedResolutionSettingsWindow::InitElement() {
}

void AdvancedResolutionSettingsWindow::DrawElement() {
#ifndef __APPLE__
    ImGui::SetNextWindowSize(ImVec2(444, 484), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Advanced Resolution Settings", &mIsVisible)) {
        bool update[sizeof(setting)];
        for (unsigned short i = 0; i < sizeof(setting); i++)
            update[i] = false;
        short max_integerScaleFactor = default_maxIntegerScaleFactor; // default value, which may or may not get
                                                                      // overridden depending on viewport res
        unsigned short integerScale_maximumBounds = gfx_current_game_window_viewport.height /
                                                    gfx_current_dimensions.height; // can change when window is resized
        if (integerScale_maximumBounds < 1)
            integerScale_maximumBounds = 1; // it should never be less than 1x.
        // Stored Values
        static float aspectRatioX = CVarGetFloat("gAdvancedResolution_aspectRatioX", 16.0f);
        static float aspectRatioY = CVarGetFloat("gAdvancedResolution_aspectRatioY", 9.0f);
        static int verticalPixelCount = CVarGetInteger("gAdvancedResolution_verticalPixelCount", 480);
        static int integerScaleFactor = CVarGetInteger("gAdvancedResolution_IntegerScaleFactor", 1);
        // Combo List defaults
        static int item_aspectRatio = default_AspectRatio;
        static int item_pixelCount = default_pixelCount;
        // Pointers
        float* p_aspectRatioX = &aspectRatioX;
        float* p_aspectRatioY = &aspectRatioY;
        int* p_verticalPixelCount = &verticalPixelCount;
        int* p_integerScaleFactor = &integerScaleFactor;

        // The original resolution slider (also for convenience)
        if (UIWidgets::EnhancementSliderFloat("Internal Resolution: %d %%", "##IMul", "gInternalResolution", 0.5f, 2.0f,
                                              "", 1.0f, true, true,
                                              CVarGetInteger("gAdvancedResolution_verticalResolutionToggle", 0))) {
            LUS::Context::GetInstance()->GetWindow()->SetResolutionMultiplier(CVarGetFloat("gInternalResolution", 1));
        };
        UIWidgets::Tooltip("Multiplies your output resolution by the value inputted, as a more intensive but effective "
                           "form of anti-aliasing");
        // N64 Mode toggle (for convenience)
        bool enableN64Mode = CVarGetInteger("gLowResMode", 0);
        if (ImGui::Checkbox("(Enhancements>Graphics) N64 Mode", &enableN64Mode)) {
            CVarSetInteger("gLowResMode", enableN64Mode);
        }

        UIWidgets::PaddedSeparator(true, true, 3.0f, 3.0f);
        // Activator
        bool activateAdvancedMode = CVarGetInteger("gAdvancedResolutionMode", 0);
        if (ImGui::Checkbox("Enable \"Advanced Resolution Settings\" settings.", &activateAdvancedMode)) {
            CVarSetInteger("gAdvancedResolutionMode", activateAdvancedMode);
        }
        // Resolution visualiser
        ImGui::Text("Viewport: %d by %d", gfx_current_game_window_viewport.width,
                    gfx_current_game_window_viewport.height);
        ImGui::Text("Internal res: %d by %d", gfx_current_dimensions.width, gfx_current_dimensions.height);
        if (IsDroppingFrames()) {
            ImGui::TextColored({ 0.85f, 0.85f, 0.0f, 1.0f },
                               ICON_FA_EXCLAMATION_TRIANGLE " Significant frame rate (FPS) drop detected.");
        } else {
            ImGui::Text(" ");
        }

        UIWidgets::PaddedSeparator(true, true, 3.0f, 3.0f);

        // Aspect Ratio
        ImGui::Text("Force aspect ratio:");
        if (ImGui::Combo("Aspect Ratio Presets", &item_aspectRatio, aspectRatioPresetLabels,
                         IM_ARRAYSIZE(aspectRatioPresetLabels))) {
            aspectRatioX = aspectRatioPresetsX[item_aspectRatio];
            aspectRatioY = aspectRatioPresetsY[item_aspectRatio];
            update[UPDATE_aspectRatioX] = true;
            update[UPDATE_aspectRatioY] = true;
        }
        if (ImGui::InputFloat("X", p_aspectRatioX, 0.1f, 1.0f, "%.3f") ||
            ImGui::InputFloat("Y", p_aspectRatioY, 0.1f, 1.0f, "%.3f")) {
            item_aspectRatio = default_AspectRatio;
            update[UPDATE_aspectRatioX] = true;
            update[UPDATE_aspectRatioY] = true;
        }

        UIWidgets::Spacer(0);
        // Vertical Resolution
        UIWidgets::PaddedEnhancementCheckbox("Set fixed vertical resolution (disables Resolution slider)",
                                             "gAdvancedResolution_verticalResolutionToggle", true, false, false, "",
                                             UIWidgets::CheckboxGraphics::Cross, true);
        UIWidgets::Tooltip(
            "Override the resolution scale slider and use the settings below, irrespective of window size.");
        if (ImGui::Combo("Pixel Count Presets", &item_pixelCount, pixelCountPresetLabels,
                         IM_ARRAYSIZE(pixelCountPresetLabels))) {
            verticalPixelCount = pixelCountPresets[item_pixelCount];
            update[UPDATE_verticalPixelCount] = true;
        }
        if (ImGui::InputInt("Pixel Count", p_verticalPixelCount, 8, 240)) {
            item_pixelCount = default_pixelCount;
            update[UPDATE_verticalPixelCount] = true;
        }

        UIWidgets::Spacer(0);
        // Pixel-perfect Mode
        UIWidgets::PaddedEnhancementCheckbox("Pixel-perfect Mode", "gAdvancedResolution_PixelPerfectMode", true, true,
                                             !CVarGetInteger("gAdvancedResolution_verticalResolutionToggle", 0), "",
                                             UIWidgets::CheckboxGraphics::Cross, true);
        UIWidgets::Tooltip("Don't scale image to fill window.");

        if (!CVarGetInteger("gAdvancedResolution_verticalResolutionToggle", 0)) {
            CVarSetInteger("gAdvancedResolution_PixelPerfectMode", (int)false);
        }

        if (default_maxIntegerScaleFactor < integerScale_maximumBounds) {
            max_integerScaleFactor = integerScale_maximumBounds + 1;
            // the +1 allows people do things like cropped 5x scaling at 1080p
        }

        // Integer Scaling
        UIWidgets::EnhancementSliderInt("Integer scale factor: %d", "##ARSIntScale",
                                        "gAdvancedResolution_IntegerScaleFactor", 1, max_integerScaleFactor, "%d", 1,
                                        true,
                                        !CVarGetInteger("gAdvancedResolution_PixelPerfectMode", 0) ||
                                            CVarGetInteger("gAdvancedResolution_IntegerScale_FitAutomatically", 0));
        UIWidgets::Tooltip("Integer scales the image. Only available in pixel-perfect mode.");

        UIWidgets::PaddedEnhancementCheckbox("Automatically scale image to fit viewport",
                                             "gAdvancedResolution_IntegerScale_FitAutomatically", true, true,
                                             !CVarGetInteger("gAdvancedResolution_PixelPerfectMode", 0), "",
                                             UIWidgets::CheckboxGraphics::Cross, true);
        UIWidgets::Tooltip("Automatically sets scale factor to fit window. Only available in pixel-perfect mode.");
        if (CVarGetInteger("gAdvancedResolution_IntegerScale_FitAutomatically", 0)) {
            CVarSetInteger("gAdvancedResolution_IntegerScaleFactor", integerScale_maximumBounds);
        }

        // Clamp and update CVars
        if (update[UPDATE_aspectRatioX]) {
            if (aspectRatioX < 0.0f) {
                aspectRatioX = 0.0f;
            }
            CVarSetFloat("gAdvancedResolution_aspectRatioX", aspectRatioX);
        }
        if (update[UPDATE_aspectRatioY]) {
            if (aspectRatioY < 0.0f) {
                aspectRatioY = 0.0f;
            }
            CVarSetFloat("gAdvancedResolution_aspectRatioY", aspectRatioY);
        }
        if (update[UPDATE_verticalPixelCount]) {
            if (verticalPixelCount < minVerticalPixelCount) {
                verticalPixelCount = minVerticalPixelCount;
            }
            if (verticalPixelCount > maxVerticalPixelCount) {
                verticalPixelCount = maxVerticalPixelCount;
            }
            CVarSetInteger("gAdvancedResolution_verticalPixelCount", (int32_t)verticalPixelCount);
        }
    }
    ImGui::End();
#endif
}

void AdvancedResolutionSettingsWindow::UpdateElement() {
}

bool AdvancedResolutionSettingsWindow::IsDroppingFrames() {
    // a rather imprecise way of checking for frame drops.
    // but it's mostly there to inform the player of large drops.
    const float threshold = 1.1f;
    return ImGui::GetIO().Framerate < (CVarGetInteger("gInterpolationFPS", 20) - threshold);
}
} // namespace AdvancedResolutionSettings