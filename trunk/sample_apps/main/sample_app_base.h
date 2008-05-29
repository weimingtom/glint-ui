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

// Base class for a sample app using glint

#ifndef GLINT_SAMPLE_APPS_MAIN_SAMPLE_APP_BASE_H__
#define GLINT_SAMPLE_APPS_MAIN_SAMPLE_APP_BASE_H__

#include "glint/include/root_ui.h"

namespace glint {

class SampleAppBase {
 public:
  SampleAppBase() {}

  static void Init() {
    // Create a root UI - equivalent to a window.
    root_ = new RootUI(true);
  }

  static void Cleanup() {
    if (root_ != NULL) {
      delete root_;
      root_ = NULL;
    }
  }

  virtual bool InitUI(const char* resource_path) = 0;
  virtual void CleanupUI() = 0;

  static void CloseAction(const std::string& button_id, void* data);

  static SampleAppBase* GetInstance() {
    if (instance_ == NULL) {
      instance_ = Create();
    }
    return instance_;
  }

 protected:
  static RootUI* root_;

 private:
  static SampleAppBase* Create();

  static SampleAppBase* instance_;

  DISALLOW_EVIL_CONSTRUCTORS(SampleAppBase);
};

}  // namespace glint

#endif  // GLINT_SAMPLE_APPS_MAIN_SAMPLE_APP_BASE_H__
