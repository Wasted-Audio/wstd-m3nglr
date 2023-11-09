/**
 * Copyright (c) Wasted Audio 2023 - GPL-3.0-or-later
 */

#include "DistrhoUI.hpp"
#include "ResizeHandle.hpp"
#include "veramobd.hpp"
#include "wstdcolors.hpp"
#include <tuple>
// #include "mangler_widget.hpp"


START_NAMESPACE_DISTRHO


const char* comChar(const char* char1, const char* char2)
{
    char *s = new char[strlen(char1)+strlen(char2)+1];
    strcpy(s,char1);
    strcat(s,char2);
    return s;
}

// --------------------------------------------------------------------------------------------------------------------
class ImGuiPluginUI : public UI
{
    float fhigh = 0.0f;
    int fhigh_crshr = 512.0;
    float fhigh_fldr = 1.0f;
    bool fhigh_lmtr = 1.0f != 0.0f;
    float fhigh_mix = 50.0f;
    float fhigh_smthr = 1.0f;
    int fhigh_sqnc = 0.0;

    float flow = 0.0f;
    int flow_crshr = 512.0;
    float flow_fldr = 1.0f;
    bool flow_lmtr = 1.0f != 0.0f;
    float flow_mix = 50.0f;
    float flow_smthr = 1.0f;
    int flow_sqnc = 0.0;

    float fmid = 0.0f;
    int fmid_crshr = 512.0;
    float fmid_fldr = 1.0f;
    float fmid_freq = 1337.0f;
    bool fmid_lmtr = 1.0f != 0.0f;
    float fmid_mix = 50.0f;
    float fmid_smthr = 1.0f;
    int fmid_sqnc = 0.0;

    ResizeHandle fResizeHandle;

    // ----------------------------------------------------------------------------------------------------------------

public:
   /**
      UI class constructor.
      The UI should be initialized to a default state that matches the plugin side.
    */
    ImGuiPluginUI()
        : UI(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true),
          fResizeHandle(this)
    {
        setGeometryConstraints(DISTRHO_UI_DEFAULT_WIDTH, DISTRHO_UI_DEFAULT_HEIGHT, true);

        ImGuiIO& io(ImGui::GetIO());

        ImFontConfig fc;
        fc.FontDataOwnedByAtlas = true;
        fc.OversampleH = 1;
        fc.OversampleV = 1;
        fc.PixelSnapH = true;

        io.Fonts->AddFontFromMemoryCompressedTTF((void*)veramobd_compressed_data, veramobd_compressed_size, 16.0f * getScaleFactor(), &fc);
        io.Fonts->AddFontFromMemoryCompressedTTF((void*)veramobd_compressed_data, veramobd_compressed_size, 21.0f * getScaleFactor(), &fc);
        io.Fonts->AddFontFromMemoryCompressedTTF((void*)veramobd_compressed_data, veramobd_compressed_size, 11.0f * getScaleFactor(), &fc);
        io.Fonts->AddFontFromMemoryCompressedTTF((void*)veramobd_compressed_data, veramobd_compressed_size, 12.5f * getScaleFactor(), &fc);
        io.Fonts->Build();
        io.FontDefault = io.Fonts->Fonts[1];

        fResizeHandle.hide();
    }

protected:
    // ----------------------------------------------------------------------------------------------------------------
    // DSP/Plugin Callbacks

   /**
      A parameter has changed on the plugin side.@n
      This is called by the host to inform the UI about parameter changes.
    */
    void parameterChanged(uint32_t index, float value) override
    {
        switch (index) {
            case 0:
                fhigh = value;
                break;
            case 1:
                fhigh_crshr = value;
                break;
            case 2:
                fhigh_fldr = value;
                break;
            case 3:
                fhigh_lmtr = value != 0.0f;
                break;
            case 4:
                fhigh_mix = value;
                break;
            case 5:
                fhigh_smthr = value;
                break;
            case 6:
                fhigh_sqnc = value;
                break;
            case 7:
                flow = value;
                break;
            case 8:
                flow_crshr = value;
                break;
            case 9:
                flow_fldr = value;
                break;
            case 10:
                flow_lmtr = value != 0.0f;
                break;
            case 11:
                flow_mix = value;
                break;
            case 12:
                flow_smthr = value;
                break;
            case 13:
                flow_sqnc = value;
                break;
            case 14:
                fmid = value;
                break;
            case 15:
                fmid_crshr = value;
                break;
            case 16:
                fmid_fldr = value;
                break;
            case 17:
                fmid_freq = value;
                break;
            case 18:
                fmid_lmtr = value != 0.0f;
                break;
            case 19:
                fmid_mix = value;
                break;
            case 20:
                fmid_smthr = value;
                break;
            case 21:
                fmid_sqnc = value;
                break;

            default: return;
        }

        repaint();
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Widget Callbacks

    int showCrshr(const char* name, uint32_t param, int fcrshr, int crshstep, float hundred, float knobWidth, float KnobFlags, float intense)
    {
        auto CrshrActive = ColorBright(Green, intense);
        auto CrshrHovered = ColorBright(GreenBr, intense);

        ImGui::BeginGroup();
        {
            // ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)CrshrActive);
            // ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)CrshrHovered);
            ImGui::PushStyleColor(ImGuiCol_Text, TextClr);
            CenterTextX("Crshr", knobWidth);
            ImGui::PopStyleColor();
            if (ImGuiKnobs::KnobInt(
                comChar("##Crshr", name), &fcrshr, 2, 512, crshstep, "%i",
                ImGuiKnobVariant_SteppedTick, hundred, KnobFlags, 9))
            {
                if (ImGui::IsItemActivated())
                {
                    editParameter(param, true);
                    if (ImGui::IsMouseDoubleClicked(0))
                        fcrshr = 512;
                }
                setParameterValue(param, fcrshr);
            }
            // ImGui::PopStyleColor(2);
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        return fcrshr;
    }

    float showFldr(const char* name, uint32_t param, float ffldr, float elevstep, float hundred, float knobWidth, float KnobFlags, float intense)
    {
        auto FldrActive  = ColorBright(Red, intense);
        auto FldrHovered = ColorBright(RedBr, intense);

        ImGui::BeginGroup();
        {
            // ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)FldrActive);
            // ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)FldrHovered);
            ImGui::PushStyleColor(ImGuiCol_Text, TextClr);
            CenterTextX("Fldr", knobWidth);
            ImGui::PopStyleColor();
            if (ImGuiKnobs::Knob(
                comChar("##Fldr", name), &ffldr, 1.0f, 13.37f, elevstep, "%.2f",
                ImGuiKnobVariant_SteppedTick, hundred, KnobFlags, 11))
            {
                if (ImGui::IsItemActivated())
                {
                    editParameter(param, true);
                    if (ImGui::IsMouseDoubleClicked(0))
                        ffldr = 1.0f;
                }
                setParameterValue(param, ffldr);
            }
            // ImGui::PopStyleColor(2);
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        return ffldr;
    }

    float showSmthr(const char* name, uint32_t param, float fsmthr, float elevstep, float hundred, float knobWidth, float KnobFlags, float intense)
    {
        auto SmthrActive  = ColorBright(Blue, intense);
        auto SmthrHovered = ColorBright(BlueBr, intense);

        ImGui::BeginGroup();
        {
            // ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)SmthrActive);
            // ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)SmthrHovered);
            ImGui::PushStyleColor(ImGuiCol_Text, TextClr);
            CenterTextX("Smthr", knobWidth);
            ImGui::PopStyleColor();
            if (ImGuiKnobs::Knob(
                comChar("##Smthr", name), &fsmthr, 1.0f, 13.37f, elevstep, "%.2f",
                ImGuiKnobVariant_SteppedTick, hundred, KnobFlags, 11))
            {
                if (ImGui::IsItemActivated())
                {
                    editParameter(param, true);
                    if (ImGui::IsMouseDoubleClicked(0))
                        fsmthr = 1.0f;
                }
                setParameterValue(param, fsmthr);
            }
            // ImGui::PopStyleColor(2);
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        return fsmthr;
    }

    std::tuple<int, int, float, float, bool, float>
    showManglr(const char* name, int offset, int fsqnc, int fcrshr, float ffldr, float fsmthr, bool flmtr, float fmix, float feq,
                float scaleFactor, float comboWidth, float toggleWidth, float knobWidth, float hundred,
                float crshstep, float elevstep, float percstep, ImFont* defaultFont, ImFont* mediumFont,
                ImColor colorActive, ImColor colorHovered, ImColor colorHeader)
        {

        auto ImGuiKnob_Flags    = ImGuiKnobFlags_DoubleClickReset + ImGuiKnobFlags_ValueTooltip + ImGuiKnobFlags_NoInput + ImGuiKnobFlags_ValueTooltipHideOnClick + ImGuiKnobFlags_NoTitle;
        auto ImGuiKnob_FlagsDB  = ImGuiKnob_Flags + ImGuiKnobFlags_dB;
        auto ImGuiKnob_FlagsLog = ImGuiKnob_Flags + ImGuiKnobFlags_Logarithmic;

        auto MixActive       = ColorMix(colorActive,  Yellow,   feq, fmix);
        auto MixHovered      = ColorMix(colorHovered, YellowBr, feq, fmix);

        auto SyncSw          = ColorBright(WhiteDr, feq, false);
        auto SyncGr          = ColorBright(Grey, feq);
        auto SyncGrHovered   = ColorBright(GreyBr, feq);
        auto SyncAct         = ColorBright(colorHeader, feq);
        auto SyncActHovered  = colorActive;

        auto prm_crshr = 1 + offset;
        auto prm_fldr  = 2 + offset;
        auto prm_lmtr  = 3 + offset;
        auto prm_mix   = 4 + offset;
        auto prm_smthr = 5 + offset;
        auto prm_sqnc  = 6 + offset;

        const char* sqnc_list[6] = {
            "C~F~S",
            "C~S~F",
            "F~C~S",
            "F~S~C",
            "S~C~F",
            "S~F~C",
        };

        ImGui::Dummy(ImVec2(7.0f * scaleFactor, 0.0f));
        ImGui::SameLine();

        ImGui::BeginGroup();
        {
            ImGui::Dummy(ImVec2(0.0f, 9.0f * scaleFactor));

            ImGui::PushStyleColor(ImGuiCol_Text,            TextClr);
            ImGui::PushStyleColor(ImGuiCol_FrameBg,         (ImVec4)ColorMix(WstdWindowBg, colorHeader, 0.5f, 50.0f));
            ImGui::PushStyleColor(ImGuiCol_Header,          (ImVec4)colorHeader);
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered,   (ImVec4)colorActive);
            ImGui::PushStyleColor(ImGuiCol_HeaderActive,    (ImVec4)colorHovered);
            ImGui::PushFont(mediumFont);
            if (ImGui::BeginListBox(comChar("##Sqnc", name), ImVec2(comboWidth, 101 * scaleFactor)))
            {
                for (int n = 0; n < 6; n++)
                {
                    bool is_selected = (fsqnc == n);
                    if (ImGui::Selectable(sqnc_list[n], is_selected))
                    {
                        fsqnc = n;
                        editParameter(prm_sqnc, true);
                        setParameterValue(prm_sqnc, fsqnc);
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }
            ImGui::PopStyleColor(5);
        }
        ImGui::EndGroup();
        ImGui::PopFont();
        ImGui::PushFont(defaultFont);
        ImGui::SameLine();

        ImGui::Dummy(ImVec2(5.0f * scaleFactor, 0.0f));
        ImGui::SameLine();

        {

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar + ImGuiWindowFlags_NoScrollWithMouse;
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4)colorHeader);
            ImGui::BeginChild(comChar("##FX", name), ImVec2(333 * scaleFactor, 127 * scaleFactor), true, window_flags);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)colorActive);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)colorHovered);
            switch (fsqnc) {
                case 0:
                    fcrshr = showCrshr(name, prm_crshr, fcrshr, crshstep, hundred, knobWidth, ImGuiKnob_FlagsLog, feq);
                    ffldr  = showFldr(name, prm_fldr, ffldr, elevstep, hundred, knobWidth, ImGuiKnob_Flags, feq);
                    fsmthr = showSmthr(name, prm_smthr, fsmthr, elevstep, hundred, knobWidth, ImGuiKnob_Flags, feq);
                    break;
                case 1:
                    fcrshr = showCrshr(name, prm_crshr, fcrshr, crshstep, hundred, knobWidth, ImGuiKnob_FlagsLog, feq);
                    fsmthr = showSmthr(name, prm_smthr, fsmthr, elevstep, hundred, knobWidth, ImGuiKnob_Flags, feq);
                    ffldr  = showFldr(name, prm_fldr, ffldr, elevstep, hundred, knobWidth, ImGuiKnob_Flags, feq);
                    break;
                case 2:
                    ffldr  = showFldr(name, prm_fldr, ffldr, elevstep, hundred, knobWidth, ImGuiKnob_Flags, feq);
                    fcrshr = showCrshr(name, prm_crshr, fcrshr, crshstep, hundred, knobWidth, ImGuiKnob_FlagsLog, feq);
                    fsmthr = showSmthr(name, prm_smthr, fsmthr, elevstep, hundred, knobWidth, ImGuiKnob_Flags, feq);
                    break;
                case 3:
                    ffldr  = showFldr(name, prm_fldr, ffldr, elevstep, hundred, knobWidth, ImGuiKnob_Flags, feq);
                    fsmthr = showSmthr(name, prm_smthr, fsmthr, elevstep, hundred, knobWidth, ImGuiKnob_Flags, feq);
                    fcrshr = showCrshr(name, prm_crshr, fcrshr, crshstep, hundred, knobWidth, ImGuiKnob_FlagsLog, feq);
                    break;
                case 4:
                    fsmthr = showSmthr(name, prm_smthr, fsmthr, elevstep, hundred, knobWidth, ImGuiKnob_Flags, feq);
                    fcrshr = showCrshr(name, prm_crshr, fcrshr, crshstep, hundred, knobWidth, ImGuiKnob_FlagsLog, feq);
                    ffldr  = showFldr(name, prm_fldr, ffldr, elevstep, hundred, knobWidth, ImGuiKnob_Flags, feq);
                    break;
                case 5:
                    fsmthr = showSmthr(name, prm_smthr, fsmthr, elevstep, hundred, knobWidth, ImGuiKnob_Flags, feq);
                    ffldr  = showFldr(name, prm_fldr, ffldr, elevstep, hundred, knobWidth, ImGuiKnob_Flags, feq);
                    fcrshr = showCrshr(name, prm_crshr, fcrshr, crshstep, hundred, knobWidth, ImGuiKnob_FlagsLog, feq);
                    break;
            }
            ImGui::PopStyleColor(2);
            ImGui::EndChild();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
        }
        ImGui::SameLine();

        ImGui::Dummy(ImVec2(5.0f * scaleFactor, 0.0f));
        ImGui::SameLine();

        ImGui::BeginGroup();
        {
            ImGui::Dummy(ImVec2(0.0f, 4.0f * scaleFactor));

            ImVec2 textSize = ImGui::CalcTextSize("bla");

            ImGui::BeginGroup();
            {
                // Title text height
                ImGui::Dummy(ImVec2(0.0f, textSize.y));

                ImGui::Dummy(ImVec2(0.0f, 35.0f) * scaleFactor);

                // knob
                ImGui::PushStyleColor(ImGuiCol_Text,            (ImVec4)SyncSw);

                // inactive colors
                ImGui::PushStyleColor(ImGuiCol_FrameBg,         (ImVec4)SyncGr);
                ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,  (ImVec4)SyncGrHovered);

                // active colors
                ImGui::PushStyleColor(ImGuiCol_Button,          (ImVec4)SyncAct);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)SyncActHovered);
                if (ImGui::Toggle(comChar("##Lmtr", name), &flmtr, ImGuiToggleFlags_Animated))
                {
                    if (ImGui::IsItemActivated())
                    {
                        editParameter(prm_lmtr, true);
                        setParameterValue(prm_lmtr, flmtr);
                    }
                }
                ImGui::PopStyleColor(5);
            }
            ImGui::EndGroup();
            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                // Title text height
                ImGui::Dummy(ImVec2(0.0f, textSize.y));

                ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)MixActive);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)MixHovered);
                if (ImGuiKnobs::Knob(
                    comChar("##Mix", name), &fmix, 0.0f, 100.0f, percstep, "%.1f%%", ImGuiKnobVariant_SteppedTick, hundred, ImGuiKnob_Flags, 11))
                {
                    if (ImGui::IsItemActivated())
                    {
                        editParameter(prm_mix, true);
                        if (ImGui::IsMouseDoubleClicked(0))
                            fmix = 50.0f;
                    }
                    setParameterValue(prm_mix, fmix);
                }
                ImGui::PopStyleColor(2);
            }
            ImGui::EndGroup();
        }
        ImGui::EndGroup();
        ImGui::PopFont();

        return std::make_tuple(
            fsqnc,
            fcrshr,
            ffldr,
            fsmthr,
            flmtr,
            fmix
        );
    }

   /**
      ImGui specific onDisplay function.
    */
    void onImGuiDisplay() override
    {

        const float width  = getWidth();
        const float height = getHeight();
        const float margin = 0.0f;
        auto scaleFactor   = getScaleFactor();

        ImGui::SetNextWindowPos(ImVec2(margin, margin));
        ImGui::SetNextWindowSize(ImVec2(width - 2 * margin, height - 2 * margin));

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
        style.ChildBorderSize = 1.0f * scaleFactor;

        style.Colors[ImGuiCol_TitleBgActive] = (ImVec4)WstdTitleBgActive;
        style.Colors[ImGuiCol_WindowBg]      = (ImVec4)WstdWindowBg;

        ImGuiIO& io(ImGui::GetIO());
        ImFont* defaultFont  = ImGui::GetFont();
        ImFont* titleBarFont = io.Fonts->Fonts[2];
        ImFont* smallFont    = io.Fonts->Fonts[3];
        ImFont* mediumFont   = io.Fonts->Fonts[4];

        auto ImGuiKnob_Flags    = ImGuiKnobFlags_DoubleClickReset + ImGuiKnobFlags_ValueTooltip + ImGuiKnobFlags_NoInput + ImGuiKnobFlags_ValueTooltipHideOnClick + ImGuiKnobFlags_NoTitle;
        auto ImGuiKnob_FlagsDB  = ImGuiKnob_Flags + ImGuiKnobFlags_dB;
        auto ImGuiKnob_FlagsLog = ImGuiKnob_Flags + ImGuiKnobFlags_Logarithmic;

        // Colors
        auto HighColorActive     = ColorBright(Blue,   fhigh);
        auto HighColorHovered    = ColorBright(BlueBr, fhigh);
        auto HighColorHeader     = ColorBright(BlueDr, fhigh);
        auto MidColorActive      =    ColorMid(Blue,   Green,   Red,   fmid, fmid_freq);
        auto MidColorHovered     =    ColorMid(BlueBr, GreenBr, RedBr, fmid, fmid_freq);
        auto MidColorHeader      =    ColorMid(BlueDr, GreenDr, RedDr, fmid, fmid_freq);
        auto MidFreqColorActive  =    ColorMid(BlueBr, GreenDr, RedBr, fmid, fmid_freq);
        auto MidFreqColorHovered =    ColorMid(Blue,   Green,   Red,   fmid, fmid_freq);
        auto LowColorActive      = ColorBright(Red,    flow);
        auto LowColorHovered     = ColorBright(RedBr,  flow);
        auto LowColorHeader      = ColorBright(RedDr,  flow);

        auto MixActive       = ColorMix(HighColorActive,  Yellow,   fhigh, fhigh_mix);
        auto MixHovered      = ColorMix(HighColorActive, YellowBr, fhigh, fhigh_mix);

        auto SyncSw          = ColorBright(WhiteDr, fhigh, false);
        auto SyncGr          = ColorBright(Grey, fhigh);
        auto SyncGrHovered   = ColorBright(GreyBr, fhigh);
        auto SyncAct         = ColorBright(BlueDr, fhigh);
        auto SyncActHovered  = HighColorActive;

        // Sizes
        const float hundred      = 100 * scaleFactor;
        const float seventy      = 70 * scaleFactor;
        const float knobWidth    = 85 * scaleFactor;
        const float toggleWidth  = 20 * scaleFactor;
        const float comboWidth   = 42 * scaleFactor;
        const float eqText       = 45 * scaleFactor;

        auto crshstep = 8;
        auto elevstep = 0.1f;
        auto percstep = 1.0f;
        auto dbstep   = 0.1f;
        auto hzstep   = 20.0f;

        if (io.KeyShift)
        {
            crshstep = 1;
            elevstep = 0.01f;
            percstep = 0.1f;
            dbstep = 0.01f;
            hzstep = 1.0f;
        }

        const char* sqnc_list[6] = {
            "C~F~S",
            "C~S~F",
            "F~C~S",
            "F~S~C",
            "S~C~F",
            "S~F~C",
        };

        ImGui::PushFont(titleBarFont);
        if (ImGui::Begin("WSTD M3NGLR", nullptr, ImGuiWindowFlags_NoResize + ImGuiWindowFlags_NoCollapse))
        {

            ImGui::Dummy(ImVec2(0.0f, 8.0f) * getScaleFactor());
            ImGui::PushFont(defaultFont);
            auto ImGuiKnob_Flags = ImGuiKnobFlags_DoubleClickReset + ImGuiKnobFlags_ValueTooltip + ImGuiKnobFlags_NoInput + ImGuiKnobFlags_ValueTooltipHideOnClick; // + ImGuiKnobFlags_NoTitle;
            auto ImGuiKnob_FlagsDB = ImGuiKnob_Flags + ImGuiKnobFlags_dB;
            auto ImGuiKnob_FlagsLog = ImGuiKnob_Flags + ImGuiKnobFlags_Logarithmic;

            // EQ Text
            // ImGui::BeginGroup();
            // {
            //     ImGui::PushStyleColor(ImGuiCol_Text, TextClr);
            //     ImGui::Dummy(ImVec2(0.0f, 38.0f) * scaleFactor);
            //     CenterTextX("High", eqText);
            //     ImGui::Dummy(ImVec2(0.0f, 80.0f) * scaleFactor);
            //     CenterTextX("Mid", eqText);
            //     ImGui::PopFont();
            //     ImGui::Dummy(ImVec2(0.0f, 60.0f) * scaleFactor);
            //     ImGui::PushFont(smallFont);
            //     CenterTextX("Mid", eqText);
            //     CenterTextX("Freq", eqText);
            //     ImGui::PopFont();
            //     ImGui::PushFont(defaultFont);
            //     ImGui::Dummy(ImVec2(0.0f, 60.0f) * scaleFactor);
            //     CenterTextX("Low", eqText);
            //     ImGui::PopStyleColor();
            // }
            // ImGui::EndGroup();
            // ImGui::SameLine();

            // EQ Section
            ImGui::BeginGroup();
            {
                ImGui::Dummy(ImVec2(0.0f, 4.0f * scaleFactor));

                ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)HighColorActive);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)HighColorHovered);
                if (ImGuiKnobs::Knob("High", &fhigh, -15.0f, 15.0, dbstep, "%.2fdB", ImGuiKnobVariant_SteppedTick, hundred, ImGuiKnob_FlagsDB, 7))
                {

                    if (ImGui::IsItemActivated())
                    {
                        editParameter(0, true);
                        if (ImGui::IsMouseDoubleClicked(0))
                            fhigh = 0.0f;
                    }
                    setParameterValue(0, fhigh);
                }
                ImGui::PopStyleColor(2);

                ImGui::Dummy(ImVec2(0.0f, 4.0f * scaleFactor));

                ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)MidColorActive);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)MidColorHovered);
                if (ImGuiKnobs::Knob("Mid", &fmid, -15.0f, 15.0, dbstep, "%.2fdB", ImGuiKnobVariant_SteppedTick, hundred, ImGuiKnob_FlagsDB, 7))
                {
                    if (ImGui::IsItemActivated())
                    {
                        editParameter(14, true);
                        if (ImGui::IsMouseDoubleClicked(0))
                            fmid = 0.0f;
                    }
                    setParameterValue(14, fmid);
                }
                ImGui::PopStyleColor(2);

                ImGui::Dummy(ImVec2(7.5f, 0.0f) * getScaleFactor()); ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)MidFreqColorActive);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)MidFreqColorHovered);
                ImGui::PushFont(smallFont);
                if (ImGuiKnobs::Knob("Mid Freq", &fmid_freq, 313.3f, 5705.6f, hzstep, "%.1fHz", ImGuiKnobVariant_SteppedTick, seventy, ImGuiKnob_FlagsLog, 11))
                {
                    if (ImGui::IsItemActivated())
                    {
                        editParameter(17, true);
                        if (ImGui::IsMouseDoubleClicked(0))
                            fmid_freq = 1337.0f;
                    }
                    setParameterValue(17, fmid_freq);
                }
                ImGui::PopFont();
                ImGui::PopStyleColor(2);

                ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)LowColorActive);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)LowColorHovered);
                if (ImGuiKnobs::Knob("Low", &flow, -15.0f, 15.0, dbstep, "%.2fdB", ImGuiKnobVariant_SteppedTick, hundred, ImGuiKnob_FlagsDB, 7))
                {
                    if (ImGui::IsItemActivated())
                    {
                        editParameter(7, true);
                        if (ImGui::IsMouseDoubleClicked(0))
                            flow = 0.0f;
                    }
                    setParameterValue(7, flow);
                }
                ImGui::PopStyleColor(2);
            }
            ImGui::EndGroup(); ImGui::SameLine();
            // ImGui::Dummy(ImVec2(20.0f, 0.0f) * getScaleFactor()); ImGui::SameLine();
            ImGui::Dummy(ImVec2(10.0f, 0.0f) * getScaleFactor()); ImGui::SameLine();

            ImGui::BeginGroup();
            {
                std::tie(fhigh_sqnc, fhigh_crshr, fhigh_fldr, fhigh_smthr, fhigh_lmtr, fhigh_mix
                ) = showManglr("high", 0, fhigh_sqnc, fhigh_crshr, fhigh_fldr, fhigh_smthr, fhigh_lmtr, fhigh_mix, fhigh,
                scaleFactor, comboWidth, toggleWidth, knobWidth, hundred, crshstep, elevstep, percstep, defaultFont, mediumFont,
                HighColorActive, HighColorHovered, HighColorHeader);

                std::tie(fmid_sqnc, fmid_crshr, fmid_fldr, fmid_smthr, fmid_lmtr, fmid_mix
                ) = showManglr("mid", 14, fmid_sqnc, fmid_crshr, fmid_fldr, fmid_smthr, fmid_lmtr, fmid_mix, fmid,
                scaleFactor, comboWidth, toggleWidth, knobWidth, hundred, crshstep, elevstep, percstep, defaultFont, mediumFont,
                MidColorActive, MidColorHovered, MidColorHeader);

                // Effect Headers
                ImGui::Dummy(ImVec2(0.0f, 19.0f) * scaleFactor);
                {
                    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar + ImGuiWindowFlags_NoScrollWithMouse;
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);

                    ImVec2 textSize = ImGui::CalcTextSize("bla");
                    auto textHeight = textSize.y;
                    auto textWidth = textSize.x;
                    auto labelWidth = comboWidth + 333 * scaleFactor + toggleWidth + knobWidth + knobWidth + 15 * scaleFactor;

                    ImGui::BeginChild(comChar("##FX", "labels"), ImVec2(labelWidth, textHeight * 2), true, window_flags);
                    // ImGui::Dummy(ImVec2(0.0f, 28.0f) * scaleFactor);
                    ImGui::BeginGroup();
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, TextClr);
                        CenterTextX("Sqnc", comboWidth); ImGui::SameLine();
                        CenterTextX("FX", 333*scaleFactor); ImGui::SameLine();
                        CenterTextX("Lmtr", toggleWidth); ImGui::SameLine();
                        CenterTextX("Mix", knobWidth); ImGui::SameLine();
                        ImGui::PopStyleColor();
                    }
                    ImGui::EndGroup();
                    // ImGui::Dummy(ImVec2(0.0f, 28.0f) * scaleFactor);
                    ImGui::EndChild();
                    ImGui::PopStyleVar();
                }
                ImGui::Dummy(ImVec2(0.0f, 19.0f) * scaleFactor);

                std::tie(flow_sqnc, flow_crshr, flow_fldr, flow_smthr, flow_lmtr, flow_mix
                ) = showManglr("low", 7, flow_sqnc, flow_crshr, flow_fldr, flow_smthr, flow_lmtr, flow_mix, flow,
                scaleFactor, comboWidth, toggleWidth, knobWidth, hundred, crshstep, elevstep, percstep, defaultFont, mediumFont,
                LowColorActive, LowColorHovered, LowColorHeader);
            }
            ImGui::EndGroup();

            if (ImGui::IsItemDeactivated())
            {
                editParameter(0, false);
                editParameter(1, false);
                editParameter(2, false);
                editParameter(3, false);
                editParameter(4, false);
                editParameter(5, false);
                editParameter(6, false);
                editParameter(7, false);
                editParameter(8, false);
                editParameter(9, false);
                editParameter(10, false);
                editParameter(11, false);
                editParameter(12, false);
                editParameter(13, false);
                editParameter(14, false);
                editParameter(15, false);
                editParameter(16, false);
                editParameter(17, false);
                editParameter(18, false);
                editParameter(19, false);
                editParameter(20, false);
                editParameter(21, false);
            }

            ImGui::PopFont();
        }
        ImGui::PopFont();
        ImGui::End();
    }

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImGuiPluginUI)
};

// --------------------------------------------------------------------------------------------------------------------

UI* createUI()
{
    return new ImGuiPluginUI();
}

// --------------------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
