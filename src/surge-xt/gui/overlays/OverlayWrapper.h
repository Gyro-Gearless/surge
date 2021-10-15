/*
** Surge Synthesizer is Free and Open Source Software
**
** Surge is made available under the Gnu General Public License, v3.0
** https://www.gnu.org/licenses/gpl-3.0.en.html
**
** Copyright 2004-2021 by various individuals as described by the Git transaction log
**
** All source at: https://github.com/surge-synthesizer/surge.git
**
** Surge was a commercial product from 2004-2018, with Copyright and ownership
** in that period held by Claes Johanson at Vember Audio. Claes made Surge
** open source in September 2018.
*/

#ifndef SURGE_XT_OVERLAYWRAPPER_H
#define SURGE_XT_OVERLAYWRAPPER_H

#include "SkinSupport.h"

#include "juce_gui_basics/juce_gui_basics.h"

class SurgeGUIEditor;
class SurgeImage;
class SurgeStorage;

namespace Surge
{
namespace Overlays
{

class OverlayComponent;
struct OverlayWrapper : public juce::Component,
                        public Surge::GUI::SkinConsumingComponent,
                        public juce::Button::Listener
{
    OverlayWrapper();                             // The default form
    OverlayWrapper(const juce::Rectangle<int> &); // The modal form

    static constexpr int titlebarSize = 14, margin = 1;

    std::string title;
    void setTitle(const std::string t) { title = t; }

    void paint(juce::Graphics &g) override;

    SurgeGUIEditor *editor{nullptr};
    void setSurgeGUIEditor(SurgeGUIEditor *e) { editor = e; }

    SurgeStorage *storage{nullptr};
    void setStorage(SurgeStorage *s) { storage = s; }

    std::unique_ptr<juce::Component> primaryChild;
    void addAndTakeOwnership(std::unique_ptr<juce::Component> c);
    std::unique_ptr<juce::TextButton> closeButton, tearOutButton;
    void buttonClicked(juce::Button *button) override;

    juce::Point<float> distanceFromCornerToMouseDown;
    bool isDragging{false};
    bool allowDrag{true};
    void mouseDown(const juce::MouseEvent &) override;
    void mouseUp(const juce::MouseEvent &) override;
    void mouseDrag(const juce::MouseEvent &) override;
    void mouseDoubleClick(const juce::MouseEvent &e) override;

    SurgeImage *icon{nullptr};
    void setIcon(SurgeImage *d) { icon = d; }

    bool canTearOut{false};
    void setCanTearOut(bool b) { canTearOut = b; }
    void doTearOut(const juce::Point<int> &showAt = juce::Point<int>(-1, -1));
    void doTearIn();
    bool isTornOut();
    juce::Point<int> currentTearOutLocation();
    juce::Rectangle<int> locationBeforeTearOut, childLocationBeforeTearOut;
    juce::Component *parentBeforeTearOut{nullptr};

    bool hasInteriorDec{true};
    void supressInteriorDecoration();

    bool showCloseButton{true};
    void setShowCloseButton(bool b) { showCloseButton = b; }

    void onClose()
    {
        closeOverlay();
        if (isTornOut())
            tearOutParent.reset(nullptr);
    }
    std::function<void()> closeOverlay = []() {};
    void setCloseOverlay(std::function<void()> f) { closeOverlay = std::move(f); }

    juce::Rectangle<int> componentBounds;
    bool isModal{false};

    std::unique_ptr<juce::DocumentWindow> tearOutParent;

    void onSkinChanged() override;

    OverlayComponent *getPrimaryChildAsOverlayComponent();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OverlayWrapper);
};
} // namespace Overlays
} // namespace Surge

#endif // SURGE_XT_OVERLAYWRAPPER_H