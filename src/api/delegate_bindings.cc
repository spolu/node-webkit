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

#include "content/nw/src/api/delegate_bindings.h"

#include "base/logging.h"
#include "base/values.h"
#include "base/command_line.h"
#include "content/nw/src/api/bindings_common.h"
#include "content/public/renderer/render_view.h"
#include "content/public/renderer/render_thread.h"
#include "content/public/renderer/v8_value_converter.h"
#include "grit/nw_resources.h"

using content::RenderView;
using content::V8ValueConverter;

namespace net {

DelegateBindings::DelegateBindings()
    : v8::Extension("delegate_bindings.js",
                    GetStringResource(
                        IDR_NW_NET_DELEGATE_BINDINGS_JS).data(),
                    0,     // num dependencies.
                    NULL,  // dependencies array.
                    GetStringResource(
                        IDR_NW_NET_DELEGATE_BINDINGS_JS).size()) {
}

DelegateBindings::~DelegateBindings() {
}

v8::Handle<v8::FunctionTemplate>
DelegateBindings::GetNativeFunction(v8::Handle<v8::String> name) {
  if (name->Equals(v8::String::New("RequireNwDelegate")))
    return v8::FunctionTemplate::New(RequireNwDelegate);

  NOTREACHED() << "Trying to get an non-exist function in DelegateBindings:"
               << *v8::String::Utf8Value(name);
  return v8::FunctionTemplate::New();
}

// static
v8::Handle<v8::Value>
DelegateBindings::RequireNwDelegate(const v8::Arguments& args) {
  v8::HandleScope scope;

  // Initialize lazily
  v8::Local<v8::String> NwDelegateSymbol = v8::String::NewSymbol("nwDelegate");
  v8::Local<v8::Value> NwDelegateHidden = args.This()->Get(NwDelegateSymbol);
  if (NwDelegateHidden->IsObject())
    return scope.Close(NwDelegateHidden);

  v8::Local<v8::Object> NwDelegate = v8::Object::New();
  args.This()->Set(NwDelegateSymbol, NwDelegate);
  /*
  RequireFromResource(args.This(),
      NwGui, v8::String::New("base.js"), IDR_NW_API_BASE_JS);
  RequireFromResource(args.This(),
      NwGui, v8::String::New("menuitem.js"), IDR_NW_API_MENUITEM_JS);
  RequireFromResource(args.This(),
      NwGui, v8::String::New("menu.js"), IDR_NW_API_MENU_JS);
  RequireFromResource(args.This(),
      NwGui, v8::String::New("tray.js"), IDR_NW_API_TRAY_JS);
  RequireFromResource(args.This(),
      NwGui, v8::String::New("clipboard.js"), IDR_NW_API_CLIPBOARD_JS);
  RequireFromResource(args.This(),
      NwGui, v8::String::New("window.js"), IDR_NW_API_WINDOW_JS);
  RequireFromResource(args.This(),
      NwGui, v8::String::New("shell.js"), IDR_NW_API_SHELL_JS);
  RequireFromResource(args.This(),
      NwGui, v8::String::New("app.js"), IDR_NW_API_APP_JS);
  */

  return scope.Close(NwDelegate);
}

}  // namespace net
