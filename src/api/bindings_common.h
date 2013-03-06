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

#ifndef CONTENT_NW_SRC_API_BINDINGS_COMMON_H_
#define CONTENT_NW_SRC_API_BINDINGS_COMMON_H_

#include "base/string_piece.h"
#include "base/file_path.h"
#include "v8/include/v8.h"

namespace content {
class RenderView;
}

// Get RenderView from current js context (only works under window context).
content::RenderView* GetCurrentRenderView();

// Get string from resource_id.
base::StringPiece GetStringResource(int resource_id);

// Similar to node's `require` function, save functions in `exports`.
void RequireFromResource(v8::Handle<v8::Object> root,
                         v8::Handle<v8::Object> gui,
                         v8::Handle<v8::String> name,
                         int resource_id);

// Utility function for paths
bool MakePathAbsolute(FilePath* file_path);

#endif  // CONTENT_NW_SRC_API_BINDINGS_COMMON_H_
