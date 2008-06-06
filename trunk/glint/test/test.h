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

#ifndef GLINT_TEST_TEST_H__
#define GLINT_TEST_TEST_H__

#include "glint/include/array.h"

// The mini-unit-test infrastructure.
// To add a unit test do the following (Let's say your component's name is Boo.)
// 1. Create a boo_unittest.cc file
// 2. Add following code to boo_unittest.cc :
//   #include "glint/test/test.h"
//   TEST(BooTestSuite);
//
//   TEST_F(BooTestSuite, TestA) {
//     Boo boo;
//     ASSERT_TRUE(boo.IsAlive());
//   }
//
//   TEST_F(BooTestSuite, TestB) {
//   ...
//
//
// 3. Compile and link the boo_unittest.cc with glint_unittest.cc and other
//    unittests.
//
// The resulting glint_unittest.exe will run all test suites and tests
// sequentially and in case of failed ASSERTs will print detailed error message
// and exit with error code 1.

namespace glint {

class GlobalTestSuite;
extern GlobalTestSuite* GetGlobalTestSuite();

#define TEST(test_suite_name) \
static TestSuite* test_suite_name = new TestSuite(#test_suite_name); \
static bool test_suite_name##_initialized = \
    GetGlobalTestSuite()->AddTestSuite(test_suite_name)

#define TEST_F(test_suite_name, test_method_name) \
static void test_method_name(); \
static bool test_suite_name##_##test_method_name##_added = \
    test_suite_name->AddTest(test_method_name, #test_method_name); \
static void test_method_name()

#define ASSERT_TRUE(expression) \
  do { \
    if (!(expression)) { \
      GetGlobalTestSuite()->Log( \
          "File: %s, function: %s, line: %d,\n\tfailed check: (%s)", \
          __FILE__, \
          __FUNCTION__, \
          __LINE__, \
          #expression); \
       return; \
    } \
  } while (0)

#define ASSERT_FALSE(expression) \
  do { ASSERT_TRUE(!(expression)); } while (0)

#define ASSERT_EQ(left, right) \
  do { ASSERT_TRUE((left) == (right)); } while (0)

// The rest of the file is implemnetation detail.

typedef void (*TestFunction)();
typedef struct {
  TestFunction function;
  const char* name;
} TestPair;

class TestSuite {
 public:
  TestSuite(const char* name);
  bool AddTest(TestFunction test_function, const char* test_name);
  const char* name() { return name_; }
  int test_count() { return tests_.length(); }
  TestFunction test_function_at(int index) { return tests_[index]->function; }
  const char* test_name_at(int index) { return tests_[index]->name; }
private:
  const char* name_;
  Array<TestPair> tests_;
  DISALLOW_EVIL_CONSTRUCTORS(TestSuite);
};

class GlobalTestSuite {
 public:
  GlobalTestSuite();
  bool AddTestSuite(TestSuite* test_suite);
  bool RunTestSuites();
  void ResetError() { had_error_ = false; }
  void Log(const char *format, ...);
  bool HadError() { return had_error_; }
 private:
  Array<TestSuite> suites_;
  char* error_string_;
  bool had_error_;
  DISALLOW_EVIL_CONSTRUCTORS(GlobalTestSuite);
};

}  // namespace glint

#endif  // GLINT_TEST_TEST_H__


