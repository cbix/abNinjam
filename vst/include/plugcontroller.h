#pragma once

#include "common.h"
#ifdef stricmp
#undef stricmp
#endif
#ifdef strnicmp
#undef strnicmp
#endif
#include "chatcontroller.h"
#include "editorsizecontroller.h"
#include "include/remoteuser.h"
#include "public.sdk/source/vst/vsteditcontroller.h"
#include "volumeparameter.h"
#include "vstgui/lib/cbitmap.h"
#include "vstgui/lib/controls/csegmentbutton.h"
#include "vstgui/lib/controls/cslider.h"
#include "vstgui/lib/controls/ctextlabel.h"
#include "vstgui/lib/cscrollview.h"
#include "vstgui/lib/vstguifwd.h"
#include "vstgui/plugin-bindings/vst3editor.h"

#include <array>
#include <limits>
#include <vector>

using namespace Steinberg;
using namespace Vst;

namespace AbNinjam {
namespace Vst3 {

template <typename T> class AbUIMessageController;

//-----------------------------------------------------------------------------
class PlugController : public EditController,
                       public VSTGUI::VST3EditorDelegate {
public:
  using UIMessageController = AbUIMessageController<PlugController>;
  using UTF8StringPtr = VSTGUI::UTF8StringPtr;
  using IUIDescription = VSTGUI::IUIDescription;
  using IController = VSTGUI::IController;
  using VST3Editor = VSTGUI::VST3Editor;
  using CTextLabel = VSTGUI::CTextLabel;
  using CView = VSTGUI::CView;
  using UIAttributes = VSTGUI::UIAttributes;
  //------------------------------------------------------------------------
  // create function required for Plug-in factory,
  // it will be called to create new instances of this controller
  //------------------------------------------------------------------------
  static FUnknown *createInstance(void *) {
    return static_cast<IEditController *>(new PlugController());
  }

  //---from IPluginBase--------
  tresult PLUGIN_API initialize(FUnknown *context) SMTG_OVERRIDE;

  //---from EditController-----
  IPlugView *PLUGIN_API createView(const char *name) SMTG_OVERRIDE;
  tresult PLUGIN_API setState(IBStream *state) SMTG_OVERRIDE;
  tresult PLUGIN_API getState(IBStream *state) SMTG_OVERRIDE;
  tresult PLUGIN_API setComponentState(IBStream *state) SMTG_OVERRIDE;

  /** We want to receive status message. */
  tresult PLUGIN_API notify(Vst::IMessage *message) SMTG_OVERRIDE;

  //------------------------------------------------------------------------
  void editorAttached(EditorView *editor) SMTG_OVERRIDE;
  void editorRemoved(EditorView *editor) SMTG_OVERRIDE;
  void editorDestroyed(EditorView *editor) SMTG_OVERRIDE;
  //---from VST3EditorDelegate-----------
  IController *createSubController(UTF8StringPtr name,
                                   const IUIDescription *description,
                                   VST3Editor *editor) SMTG_OVERRIDE;
  CView *createCustomView(UTF8StringPtr name, const UIAttributes &attributes,
                          const IUIDescription *description,
                          VST3Editor *editor) SMTG_OVERRIDE;
  void willClose(VST3Editor *editor) SMTG_OVERRIDE;

  //---Internal functions-------
  void addUIMessageController(UIMessageController *controller);
  void removeUIMessageController(UIMessageController *controller);
  // void removeChatController(ChatController *controller);
  VSTGUI::UTF8String getChatHistory();
  auto &gsLastChatTextHolderViewSize() { return lastChatTextHolderViewSize; }

  void setMessageText(String128 text, unsigned long index);
  TChar *getMessageText(unsigned long index);
  std::vector<Common::RemoteUser> remoteUsers;

private:
  using UIMessageControllerList = std::vector<UIMessageController *>;
  UIMessageControllerList uiMessageControllers;
  std::array<String128, 3> messageTexts;
  CTextLabel *notificationLabel;
  VSTGUI::CSegmentButton *menu;
  VSTGUI::CScrollView *mixerScrollView;
  VSTGUI::CBitmap *sliderHandle;
  VSTGUI::CBitmap *sliderBackground;
  static constexpr VSTGUI::CViewAttributeID kCViewUserIdAttrID = 'uidx';
  static constexpr VSTGUI::CViewAttributeID kCViewChannelIdAttrID = 'cidx';
  VSTGUI::CTextLabel *createLabel(std::string labelText,
                                  VSTGUI::CFontRef inFontID,
                                  VSTGUI::CRect labelPlacer,
                                  VSTGUI::CColor fontColor);
  VSTGUI::CSlider *createSlider(VSTGUI::CRect sliderPlacer, int controlTag,
                                float value, VST3Editor *editor, int userId,
                                int channelId);
  void createMixer(VST3Editor *editor);
  void cleanNotConnected(std::string notification);
  void extractCurrentInfo(EditorView *editor);
  VST3Editor *vst3Editor;
  ChatController *chatController;
  VSTGUI::UTF8String chatHistory;
  VSTGUI::CRect lastChatTextHolderViewSize;
  using EditorVector = std::vector<Steinberg::Vst::EditorView *>;
  EditorVector editors;
  using EditorMap =
      std::map<Steinberg::Vst::EditorView *, EditorSizeController *>;
  EditorMap editorsSubCtlerMap;
  uint32 width = 0;
  uint32 height = 0;
  double sizeFactor = 0;
};

//------------------------------------------------------------------------
} // namespace Vst3
} // namespace AbNinjam
