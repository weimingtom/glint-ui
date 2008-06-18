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

IMPLEMENT_DYNAMIC(MainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(MainFrame, CFrameWnd)
  ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
  ID_SEPARATOR,           // status line indicator
  ID_INDICATOR_CAPS,
  ID_INDICATOR_NUM,
  ID_INDICATOR_SCRL,
};

int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;
  // create a view to occupy the client area of the frame
  if (!child_view_.Create(AFX_WS_DEFAULT_VIEW |
                        WS_VSCROLL |
                        WS_HSCROLL |
                        ES_MULTILINE |
                        ES_WANTRETURN |
                        ES_AUTOVSCROLL |
                        ES_AUTOHSCROLL,
                        CRect(0, 0, 0, 0),
                        this,
                        AFX_IDW_PANE_FIRST))
  {
    TRACE0("Failed to create view window\n");
    return -1;
  }
  if (!status_bar_.Create(this) ||
      !status_bar_.SetIndicators(indicators,
      sizeof(indicators)/sizeof(UINT)))
  {
    TRACE0("Failed to create status bar\n");
    return -1;      // fail to create
  }

  return 0;
}

BOOL MainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  if( !CFrameWnd::PreCreateWindow(cs) )
    return FALSE;
  // TODO: Modify the Window class or styles here by modifying
  //  the CREATESTRUCT cs

  cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
  cs.lpszClass = AfxRegisterWndClass(0);
  return TRUE;
}

BOOL MainFrame::OnCmdMsg(UINT nID,
                          int nCode,
                          void* pExtra,
                          AFX_CMDHANDLERINFO* pHandlerInfo)
{
  // let the view have first crack at the command
  if (child_view_.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
    return TRUE;

  // otherwise, do default handling
  return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

}  // namespace glint


