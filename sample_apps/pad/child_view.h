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

#ifndef GLINT_SAMPLE_APPS_PAD_CHILD_VIEW_H__
#define GLINT_SAMPLE_APPS_PAD_CHILD_VIEW_H__

namespace glint {

class ChildView : public CRichEditCtrl {
 public:
  ChildView();
  virtual ~ChildView();

  afx_msg void OnEnChange();
  afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
  afx_msg void OnEditCut();
  afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
  afx_msg void OnEditCopy();
  afx_msg void OnEditPaste();
  afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
  afx_msg void OnEditUndo();
  afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
  afx_msg void OnEditRedo();
  afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
  afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);
  afx_msg void OnFileOpen();
  afx_msg void OnFileSave();
  afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
  afx_msg void OnFileSaveAs();
  afx_msg void OnUpdateFileSaveAs(CCmdUI *pCmdUI);
  afx_msg void OnFileNew();
  afx_msg void OnUpdateFileNew(CCmdUI *pCmdUI);

  afx_msg void OnTimer(UINT_PTR nIDEvent);

 protected:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  DECLARE_MESSAGE_MAP()

 private:
  bool TryUpdate(const CString& text);
  bool LoadFromFile(CString file_name);
  bool SaveToFile(const CString& file_name);
  void SetModified(bool modified);
  bool UserWantsToDiscardChanges();

  COLORREF text_color_;
  CString base_dir_;
  CString file_name_;
  bool modified_;
  CFont* font_;
  int parse_delay_;
};

}  // namespace glint

#endif  // GLINT_SAMPLE_APPS_PAD_CHILD_VIEW_H__
