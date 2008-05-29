// Copyright 2008, Google Inc.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Simple program showing how to use Glint. Also useful as a testbed.

#include <map>
#include <string>

#include "glint/sample_apps/main/sample_app_base.h"
#include "glint/include/animation_timeline.h"
#include "glint/include/button.h"
#include "glint/include/column.h"
#include "glint/include/nine_grid.h"
#include "glint/include/row.h"
#include "glint/include/simple_text.h"
#include "glint/include/xml_parser.h"

using namespace glint;

namespace hello_world {

static const char* kEffectButton = "effect_button";
static const char* kCloseButton = "close_button";

class HelloWorldApp : public glint::SampleAppBase {
 public:
  HelloWorldApp() {}

  virtual bool InitUI(const char* resource_path) {
    Init();

    std::string base_path(resource_path);
    if (!base_path.empty() &&
        base_path[base_path.length() - 1] != '/') {
      base_path.append("/");
    }

#ifdef GLINT_ENABLE_XML
    // Load the Glint XML UI definition in a string
    void* buffer = NULL;
    int size = 0;
    if (!platform()->GetResourceByName("test.xml", &buffer, &size))
      return false;
    std::string content = std::string(reinterpret_cast<char*>(buffer), size);

    XmlParser parser;
    Node* node = NULL;
    // Parse the Glint XML, get the root node of the UI tree.
    if (!parser.Parse(content, resource_path, &node))
      return false;
#else  // GLINT_ENABLE_XML
    Node* node = new Node();
    node->set_min_width(300);
    node->set_min_height(100);
    Transform offset;
    offset.AddOffset(Vector(250.0f, 250.0f));
    node->set_transform(offset);
    node->set_alpha(230);

    NineGrid* background = new NineGrid();
    background->ReplaceImage(base_path + "dialog_bg.png");
    background->set_center_height(10);
    background->set_center_width(10);
    background->set_shadow(true);
    node->AddChild(background);

    Column* column = new Column();
    node->AddChild(column);

    SimpleText* text = new SimpleText();
    text->set_text("Hello World!\nClick \"Blink\" button below \nto see alpha animation.");
    text->set_font_size(14);
    Rectangle margin;
    margin.Set(10, 10, 10, 10);
    text->set_margin(margin);
    column->AddChild(text);

    Row* row = new Row();
    //row->set_background(Color(0x80808060));
    margin.Set(30, 10, 20, 10);
    row->set_margin(margin);
    row->set_direction(Row::RIGHT_TO_LEFT);
    column->AddChild(row);

    Button* button = new Button();
    button->set_id("close_button");
    button->ReplaceImage(base_path + "button_strip.png");
    button->set_min_height(22);
    button->set_min_width(70);
    margin.Set(5, 1, 5, 1);
    button->set_margin(margin);
    button->set_shadow(true);
    row->AddChild(button);

    text = new SimpleText();
    text->set_text("Close");
    margin.Set(2, 3, 2, 3);
    text->set_margin(margin);
    button->AddChild(text);

    button = new Button();
    button->set_id("effect_button");
    button->ReplaceImage(base_path + "button_strip.png");
    button->set_min_height(22);
    button->set_min_width(70);
    margin.Set(5, 1, 5, 1);
    button->set_margin(margin);
    button->set_shadow(true);
    row->AddChild(button);

    text = new SimpleText();
    text->set_text("Blink");
    margin.Set(2, 3, 2, 3);
    text->set_margin(margin);
    button->AddChild(text);
#endif  // GLINT_ENABLE_XML

    // Hookup the window and the root node
    root_->set_root_node(node);


    // Find couple of buttons in UI, set their click handlers.
    glint::Button* effect_button = static_cast<glint::Button*>(
        root_->FindNodeById(kEffectButton));
    if (effect_button) {
      effect_button->SetClickHandler(EffectAction, node);
    }

    glint::Button* close_button = static_cast<glint::Button*>(
        root_->FindNodeById(kCloseButton));
    if (close_button) {
      close_button->SetClickHandler(CloseAction, node);
    }

    // Set user-defined animation. It has 3 segments (keyframes):
    // 1. starts at whatever current value (no duration and interpolation)
    // 2. 0.5 seconds of going to transparent with smooth decel interpolation.
    // 3. 0.5 seconds of going back (START and FINAL are the same here since
    // this is user-triggered animation and not change-triggered one).
    AnimationTimeline* timeline = new AnimationTimeline();
    AlphaAnimationSegment* segment =
        new AlphaAnimationSegment(RELATIVE_TO_START,
                                  0,  // offset to start value
                                  0,  // duration - ignored on first segment
                                  NULL);  // interpolation - also ignored
    timeline->AddAlphaSegment(segment);

    segment = new AlphaAnimationSegment(ABSOLUTE_VALUE,
                                        colors::kTransparentAlpha,
                                        0.5,  // duration in seconds
                                        Interpolation::Smooth);
    timeline->AddAlphaSegment(segment);

    segment = new AlphaAnimationSegment(RELATIVE_TO_FINAL,
                                        0,
                                        0.5,
                                        Interpolation::Linear);
    timeline->AddAlphaSegment(segment);
    // Sets the constructed timeline as a user-triggered transition.
    node->SetTransition(USER_TRANSITION, timeline);

    root_->ShowInteractive();

    return true;
  }


  void CleanupUI() {
    platform()->Trace("Cleaning up!\n");
    Cleanup();
  }

 private:
  static void EffectAction(const std::string& button_id, void* data) {
    Node* root_node = reinterpret_cast<Node*>(data);
    root_node->TriggerTransition(USER_TRANSITION);
  }

  DISALLOW_EVIL_CONSTRUCTORS(HelloWorldApp);
};


}  // namespace hello_world

namespace glint {

SampleAppBase* SampleAppBase::Create() {
  return new hello_world::HelloWorldApp();
}

}  // namespace glint
