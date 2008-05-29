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

// OSX specific portions for a sample app using Glint.

#import <Cocoa/Cocoa.h>
#import <stdlib.h>
#import <string>

#import "glint/sample_apps/main/sample_app_base.h"
#import "glint/sample_apps/main/sample_app_controller.h"

using namespace glint;

namespace glint {

RootUI* SampleAppBase::root_ = NULL;
SampleAppBase* SampleAppBase::instance_ = NULL;

void SampleAppBase::CloseAction(const std::string& button_id, void* data) {
  [NSApp terminate:nil];
}

extern "C" {

int main(int argc, const char** argv) {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

  SampleAppController* controller =
      [[[SampleAppController alloc] init] autorelease];
  [[NSApplication sharedApplication] setDelegate:controller];

  SampleAppBase* app = SampleAppBase::GetInstance();

  std::string resource_path =
      [[[NSBundle mainBundle] resourcePath] fileSystemRepresentation];

  // All resources get loaded from the bundle's resource path
  if (!app->InitUI(resource_path.c_str()))
    return -1;

  // This does [NSApp run]:
  platform()->RunMessageLoop();

  [pool release];

  return 0;
}

}

}  // namespace glint
