#ifndef ABVST3EDITOR_H
#define ABVST3EDITOR_H

#pragma once

#include "include/remoteuserchannel.h"
#include "messagetypes.h"
#include "plugcontroller.h"
#include "plugids.h"

#include "vstgui/plugin-bindings/vst3editor.h"

using namespace VSTGUI;

namespace AbNinjam {
namespace Vst3 {

class AbVST3Editor : public VST3Editor {
public:
  AbVST3Editor(Steinberg::Vst::EditController *controller,
               UTF8StringPtr templateName, UTF8StringPtr xmlFile);
  void valueChanged(CControl *pControl) override;
  void controlEndEdit(VSTGUI::CControl *pControl) override;
  Steinberg::tresult PLUGIN_API canResize() override;

private:
  static constexpr VSTGUI::CViewAttributeID kCViewUserIdAttrID = 'uidx';
  static constexpr VSTGUI::CViewAttributeID kCViewChannelIdAttrID = 'cidx';
  PlugController *getController() const {
    return static_cast<PlugController *>(Vst::VSTGUIEditor::getController());
  }
  void sendMixingTouched();
};

} // namespace Vst3
} // namespace AbNinjam

#endif // ABVST3EDITOR_H
