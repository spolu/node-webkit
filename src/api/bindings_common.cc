// Copyright (c) 2012 Intel Corp
// Copyright (c) 2012 The Chromium Authors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy 
// of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell co
// pies of the Software, and to permit persons to whom the Software is furnished
//  to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in al
// l copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IM
// PLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNES
// S FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
//  OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WH
// ETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "content/nw/src/api/bindings_common.h"

#include "base/file_path.h"
#include "base/file_util.h"
#include "base/logging.h"
#include "base/values.h"
#include "content/nw/src/api/api_messages.h"
#include "content/public/renderer/render_view.h"
#include "content/public/renderer/render_thread.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebView.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebFrame.h"
#include "content/public/renderer/v8_value_converter.h"
#include "chrome/renderer/static_v8_external_string_resource.h"
#include "ui/base/resource/resource_bundle.h"


using content::RenderView;
using WebKit::WebFrame;
using WebKit::WebView;
using content::V8ValueConverter;

RenderView* GetCurrentRenderView() {
  WebFrame* frame = WebFrame::frameForCurrentContext();
  if (!frame)
    return NULL;

  WebView* view = frame->view();
  if (!view)
    return NULL;  // can happen during closing.

  RenderView* render_view = RenderView::FromWebView(view);
  return render_view;
}

base::StringPiece GetStringResource(int resource_id) {
  return ResourceBundle::GetSharedInstance().GetRawDataResource(resource_id);
}

v8::Handle<v8::String> WrapSource(v8::Handle<v8::String> source) {
  v8::HandleScope handle_scope;
  v8::Handle<v8::String> left =
      v8::String::New("(function(nw, exports) {");
  v8::Handle<v8::String> right = v8::String::New("\n})");
  return handle_scope.Close(
      v8::String::Concat(left, v8::String::Concat(source, right)));
}

// Similar to node's `require` function, save functions in `exports`.
void RequireFromResource(v8::Handle<v8::Object> root,
                         v8::Handle<v8::Object> gui,
                         v8::Handle<v8::String> name,
                         int resource_id) {
  v8::HandleScope scope;

  v8::Handle<v8::String> source = v8::String::NewExternal(
      new StaticV8ExternalAsciiStringResource(
          GetStringResource(resource_id)));
  v8::Handle<v8::String> wrapped_source = WrapSource(source);

  v8::Handle<v8::Script> script(v8::Script::New(wrapped_source, name));
  v8::Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(script->Run());
  v8::Handle<v8::Value> args[] = { root, gui };
  func->Call(root, 2, args);
}

bool MakePathAbsolute(FilePath* file_path) {
  DCHECK(file_path);

  FilePath current_directory;
  if (!file_util::GetCurrentDirectory(&current_directory))
    return false;

  if (file_path->IsAbsolute())
    return true;

  if (current_directory.empty())
    return file_util::AbsolutePath(file_path);

  if (!current_directory.IsAbsolute())
    return false;

  *file_path = current_directory.Append(*file_path);
  return true;
}
