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

#import "glint/mac/glint_work_item_dispatcher.h"

#import "glint/mac/darwin_platform.h"

using namespace glint;
using namespace glint_darwin;

@implementation GlintWorkItemDispatcher

- (id)initForWorkItem:(WorkItem*)item withUI:(RootUI*)ui {
  if ((self = [super init])) {
    ui_ = ui;
    workItem_ = item;
    isCanceled_ = NO;
  }

  return self;
}

+ (GlintWorkItemDispatcher*)dispatcherForWorkItem:(WorkItem*)item
                                           withUI:(RootUI*)ui {
  return [[[GlintWorkItemDispatcher alloc] initForWorkItem:item withUI:ui] autorelease];
}

- (void)setContainer:(NSMutableSet*)container {
  [container_ autorelease];
  container_ = [container retain];
}

- (void)dispatch {
  if (isCanceled_ == NO) { 
    if (ui_ != NULL) {
      // Remove itself from list of all work items.
      [container_ removeObject:self];
      
      // Tell Glint about this workitem
      Message message;
      
      message.code = GL_MSG_WORK_ITEM;
      message.ui = ui_;
      message.work_item = workItem_;
      
      ui_->HandleMessage(message);
    } else {
      if (workItem_ != NULL) {
        // ui-less workitem; we'll just run it ourselves
        workItem_->Run();
        delete workItem_;
      }
    }
  }
}

- (void)cancel {
  isCanceled_ = YES;
}

@end
