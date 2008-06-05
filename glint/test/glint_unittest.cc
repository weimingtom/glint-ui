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

#include "glint/include/platform.h"
#include "glint/test/test.h"

namespace glint {

static GlobalTestSuite* test_suite = NULL;
static int kErrorStringLength = 10240;

GlobalTestSuite::GlobalTestSuite() {
  // We simply leak this array, since GlobalTestSuite is never destroyed.
  error_string_ = new char[kErrorStringLength];
  had_error_ = false;
}

bool GlobalTestSuite::AddTestSuite(TestSuite* test_suite) {
  return suites_.Add(test_suite);
}

bool GlobalTestSuite::RunTestSuites() {
  bool success = true;
  for (int suite_index = 0; suite_index < suites_.length(); ++suite_index) {
    TestSuite* suite = suites_[suite_index];
    glint::platform()->Trace("Running %s\n", suite->name());
    for (int test_index = 0; test_index < suite->test_count(); ++test_index) {
      TestFunction test = suite->test_function_at(test_index);
      ResetError();
      test();
      if (HadError()) {
        success = false;
        platform()->Trace("\tTest %s failed.\n\t%s\n",
                          suite->test_name_at(test_index),
                          error_string_);
      }
    }
  }
  return success;
}

void GlobalTestSuite::Log(const char *format, ...) {
  va_list args;
  va_start(args, format);
  platform()->Sprintf(error_string_, kErrorStringLength, format, args);
  va_end(args);

  had_error_ = true;
}

GlobalTestSuite* GetGlobalTestSuite() {
  if (!test_suite) {
   test_suite = new GlobalTestSuite();
  }
  return test_suite;
}

TestSuite::TestSuite(const char* name) : name_(name) {
}

bool TestSuite::AddTest(TestFunction test_function, const char* test_name) {
  TestPair* pair = new TestPair();
  pair->function = test_function;
  pair->name = test_name;
  return tests_.Add(pair);
}

}  // namespace glint

int main() {
  bool ok = glint::GetGlobalTestSuite()->RunTestSuites();
  if (ok) {
    glint::platform()->Trace("\nGlint unittests succeeded!\n");
  } else {
    glint::platform()->Trace("\nGlint unittests FAILED!\n");
  }
  return ok ? 0 : 1;
}





