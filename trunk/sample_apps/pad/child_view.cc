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

#include <string>

#include "glint/sample_apps/pad/precompile.h"
#include "glint/sample_apps/pad/glint_pad.h"

#include "glint/include/root_ui.h"
#include "glint/sample_apps/pad/child_view.h"
#include "glint/sample_apps/pad/main_frame.h"

namespace glint {

static TCHAR kFileExtensionFilter[] =
    { _T("XML Files (*.xml)|*.xml|All Files (*.*)|*.*||") };

static TCHAR kDefaultXml[] =
  _T("<Node xmlns=\"http://www.google.com/glint\"\n")
  _T("  background=\"#C0606060\"\n")
  _T("  min_width=\"200\"\n")
  _T("  min_height=\"100\"\n")
  _T("  transform=\"translate(50 50)\">\n")
  _T("    <SimpleText font_size=\"14\" text=\"Hello, World!\" ")
  _T("       foreground=\"#C0F0A0\"/>\n")
  _T("  </Node>\n");

BEGIN_MESSAGE_MAP(ChildView, CEdit)
  ON_CONTROL_REFLECT(EN_CHANGE, &ChildView::OnEnChange)
  ON_WM_CTLCOLOR_REFLECT()
  ON_WM_CREATE()
  ON_WM_TIMER()
  ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &ChildView::OnUpdateEditCut)
  ON_COMMAND(ID_EDIT_CUT, &ChildView::OnEditCut)
  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &ChildView::OnUpdateEditCopy)
  ON_COMMAND(ID_EDIT_COPY, &ChildView::OnEditCopy)
  ON_COMMAND(ID_EDIT_PASTE, &ChildView::OnEditPaste)
  ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &ChildView::OnUpdateEditPaste)
  ON_COMMAND(ID_EDIT_UNDO, &ChildView::OnEditUndo)
  ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &ChildView::OnUpdateEditUndo)
  ON_COMMAND(ID_EDIT_REDO, &ChildView::OnEditRedo)
  ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &ChildView::OnUpdateEditRedo)
  ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &ChildView::OnUpdateFileOpen)
  ON_COMMAND(ID_FILE_OPEN, &ChildView::OnFileOpen)
  ON_COMMAND(ID_FILE_SAVE, &ChildView::OnFileSave)
  ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &ChildView::OnUpdateFileSave)
  ON_COMMAND(ID_FILE_SAVE_AS, &ChildView::OnFileSaveAs)
  ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, &ChildView::OnUpdateFileSaveAs)
  ON_COMMAND(ID_FILE_NEW, &ChildView::OnFileNew)
  ON_UPDATE_COMMAND_UI(ID_FILE_NEW, &ChildView::OnUpdateFileNew)
END_MESSAGE_MAP()

ChildView::ChildView() : font_(NULL), parse_delay_(-1) {
}

ChildView::~ChildView() {
  delete font_;
}

BOOL ChildView::PreCreateWindow(CREATESTRUCT& cs) {  // NOLINT - MFC-defined
  if (!CWnd::PreCreateWindow(cs))
    return FALSE;

  cs.dwExStyle |= WS_EX_CLIENTEDGE;
  cs.style &= ~WS_BORDER;

  return TRUE;
}

void ChildView::SetModified(bool modified) {
  CString file_name = file_name_;
  if (file_name.IsEmpty())
    file_name = _T("Untitled.xml");
  CString window_title = CString(_T("glint_pad - ")) + file_name;
  if (modified)
    window_title += _T(" *");
  CWnd* frame = GetParentFrame();
  frame->SetWindowText(window_title);
  modified_ = modified;
}

bool ChildView::LoadFromFile(CString file_name) {
  CString base_dir;
  int last_slash = file_name.ReverseFind('\\');
  if (last_slash > 0) {
    base_dir = file_name.Mid(0, last_slash + 1);  // including slash
  }

  base_dir_ = base_dir;

  CFile file(file_name, CFile::modeRead | CFile::typeBinary);

  int string_length = static_cast<int>(file.GetLength());
  char* buffer = new char[string_length + 1];
  file.Read(buffer, string_length);
  buffer[string_length] = '\0';
  file.Close();

  CString text = CString(Utf16FromUtf8(buffer));
  SetWindowText(text.GetString());
  TryUpdate(text);
  parse_delay_ = -1;
  delete [] buffer;

  SetModified(false);
  return true;
}

bool ChildView::SaveToFile(const CString& file_name) {
  bool result = false;
  CString text;
  GetWindowText(text);
  CFile file(file_name,
             CFile::modeCreate |
             CFile::modeWrite |
             CFile::typeBinary);
  CStringA utf8_text(Utf8FromUtf16(text.GetString()));
  file.Write(utf8_text.GetString(), utf8_text.GetLength());
  file.Close();
  result = true;
  return result;
}

void ChildView::OnEnChange() {
  // Note: Since this is a RICHEDIT control, the control will not
  // send this notification unless you override the OnInitDialog()
  // function and call CRichEditCtrl().SetEventMask()
  // with the ENM_CHANGE flag ORed into the mask.
  parse_delay_ = 5;  // timer clicks
  SetModified(true);

  Invalidate();
}

bool ChildView::TryUpdate(const CString& text) {
  static RootUI* root_ui = NULL;

  if (!root_ui) {
    root_ui = new RootUI(true);
  }

  bool result = false;
  Node* root_node = NULL;
  MainFrame* frame = static_cast<MainFrame*>(GetParentFrame());
  std::string base_uri(Utf8FromUtf16(base_dir_.GetString()));
  XmlParser parser;
  std::string xml_text(Utf8FromUtf16(text.GetString()));
  result = parser.Parse(xml_text, base_uri, &root_node);

  std::string full_message;
  const int buf_len = 1024;
  full_message.reserve(buf_len);

  if (!result) {
    std::string message = parser.error_message();
    int line = parser.error_line_number();
    int pos = parser.error_position();
    text_color_ = RGB(255, 0, 0);
    SetBackgroundColor(0, RGB(255, 238, 240));

    _snprintf(
        const_cast<char*>(full_message.data()),
        buf_len,
        "Error line %d pos %d: %s\n",
        line,
        pos,
        message.c_str());

  } else {
     text_color_ = RGB(0, 0, 0);
     SetBackgroundColor(1, RGB(255, 255, 255));
     root_ui->set_root_node(root_node);

  }

  if (::IsWindow(frame->status_bar()->m_hWnd)) {
    frame->status_bar()->SetPaneText(
        0, Utf16FromUtf8(full_message.c_str()), TRUE);
  }

  root_ui->Show();
  SetFocus();
  return result;
}

HBRUSH ChildView::CtlColor(CDC* pDC, UINT /*nCtlColor*/) {
  // TODO:  Change any attributes of the DC here
  pDC->SetTextColor(text_color_);
  // TODO:  Return a non-NULL brush if the parent's handler should not be called
  return CBrush(RGB(0, 0, 0));
}

void ChildView::OnTimer(UINT_PTR nIDEvent) {
  parse_delay_ -= 1;
  if (parse_delay_ == 0) {
    CString text;
    GetWindowText(text);
    TryUpdate(text);
  }
}

int ChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
  if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
    return -1;

  DWORD mask = GetEventMask();
  SetEventMask(mask | ENM_CHANGE /*| ENM_SELCHANGE*/);
  font_ = new CFont();
  font_->CreateFont(-14, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, 0, 0,
                    ANTIALIASED_QUALITY, 0, CString(L"Courier New"));
  SetFont(font_);

  SetModified(false);
  OnFileNew();
  SetTimer(153, 100, NULL);

  return 0;
}

void ChildView::OnUpdateEditCut(CCmdUI *pCmdUI) {
  pCmdUI->Enable(GetSelectionType() != SEL_EMPTY);
}

void ChildView::OnEditCut() {
  Cut();
}

void ChildView::OnUpdateEditCopy(CCmdUI *pCmdUI) {
  pCmdUI->Enable(GetSelectionType() != SEL_EMPTY);
}

void ChildView::OnEditCopy() {
  Copy();
}

void ChildView::OnEditPaste() {
  PasteSpecial(CF_TEXT);
}

void ChildView::OnUpdateEditPaste(CCmdUI *pCmdUI) {
  pCmdUI->Enable(CanPaste());
}

void ChildView::OnEditUndo() {
  Undo();
}

void ChildView::OnUpdateEditUndo(CCmdUI *pCmdUI) {
  pCmdUI->Enable(CanUndo());
}

void ChildView::OnEditRedo() {
  Redo();
}

void ChildView::OnUpdateEditRedo(CCmdUI *pCmdUI) {
  pCmdUI->Enable(CanRedo());
}


void ChildView::OnUpdateFileOpen(CCmdUI *pCmdUI) {
  pCmdUI->Enable(TRUE);
}

void ChildView::OnFileOpen() {
  if (!UserWantsToDiscardChanges())
    return;
  CFileDialog file_dialog(TRUE, _T(".xml"), NULL, 0, kFileExtensionFilter);
  if (file_dialog.DoModal() == IDOK) {
    CString file_name = file_dialog.GetPathName();
    LoadFromFile(file_name);
    file_name_ = file_name;
  }
}

void ChildView::OnFileSave() {
  if (file_name_.IsEmpty()) {
    OnFileSaveAs();
    return;
  }
  if (!SaveToFile(file_name_))
    return;
  SetModified(false);
}

void ChildView::OnUpdateFileSave(CCmdUI *pCmdUI) {
  pCmdUI->Enable(modified_);
}

void ChildView::OnFileSaveAs() {
  CFileDialog file_dialog(FALSE,
                          _T(".xml"),
                          file_name_,
                          0,
                          kFileExtensionFilter);
  if (file_dialog.DoModal() == IDOK) {
    CString file_name = file_dialog.GetFileName();
    if (!SaveToFile(file_name))
      return;
    file_name_ = file_name;
    SetModified(false);
  }
}

void ChildView::OnUpdateFileSaveAs(CCmdUI *pCmdUI) {
  pCmdUI->Enable(TRUE);
}

void ChildView::OnFileNew() {
  if (!UserWantsToDiscardChanges())
    return;
  file_name_.Empty();
  SetWindowText(kDefaultXml);
  SetModified(false);
}

void ChildView::OnUpdateFileNew(CCmdUI *pCmdUI) {
  pCmdUI->Enable(TRUE);
}

bool ChildView::UserWantsToDiscardChanges() {
  return !modified_ ||
         IDCANCEL != ::MessageBox(
             m_hWnd,
             _T("Do you want to discard changes you've made?"),
             _T("Create new file"),
             MB_OKCANCEL);
}

}  // namespace glint
