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

#include <afxwin.h>  // MFC core and standard components
#include <afxext.h>  // MFC extensions

#include "glint/sample_apps/pad/glint_pad.h"
#include "glint/sample_apps/pad/main_frame.h"

namespace glint {

BEGIN_MESSAGE_MAP(GlintPadApp, CWinApp)
END_MESSAGE_MAP()

// The one and only GlintPadApp object
GlintPadApp theApp;

BOOL GlintPadApp::InitInstance() {
  // InitCommonControlsEx() is required on Windows XP if an application
  // manifest specifies use of ComCtl32.dll version 6 or later to enable
  // visual styles.  Otherwise, any window creation will fail.
  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(InitCtrls);
  // Set this to include all the common control classes you want to use
  // in your application.
  InitCtrls.dwICC = ICC_WIN95_CLASSES;
  InitCommonControlsEx(&InitCtrls);

  CWinApp::InitInstance();

  // To create the main window, this code creates a new frame window
  // object and then sets it as the application's main window object
  MainFrame* pFrame = new MainFrame;
  if (!pFrame)
    return FALSE;

  m_pMainWnd = pFrame;
  // create and load the frame with its resources
  pFrame->LoadFrame(IDR_MAINFRAME,
                    WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,
                    NULL,
                    NULL);

  // The one and only window has been initialized, so show and update it
  pFrame->ShowWindow(SW_SHOW);
  pFrame->UpdateWindow();

  return TRUE;
}

}  // namespace glint

