#include <csetjmp>
#include <csignal>
#include <cstdbool>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>

#include "Dictionary.h"

#define FIRST_TEST Empty_size
#define MAXSCORE 45
#define CHARITY 10

#define RED "\033[0;31m"
#define CYAN "\033[0;36m"
#define GREEN "\033[0;32m"
#define NC "\033[0m"

using namespace std;

static uint8_t testsPassed;
static volatile sig_atomic_t testStatus;
static uint8_t disable_exit_handler;
jmp_buf test_crash;

enum Test_e {
  Empty_size = 0,
  SetValue_size,
  Remove_size,

  Empty_traverse,
  Empty_clear,
  NonEmpty_clear,

  Begin_value,
  End_value,
  Next_value,
  Prev_value,
  SetValue_value,
  Remove_value,
  GetValue_value,

  Repeated_setValue,
  MultiString_equals,
  ToString_equals,
  PreString_equals,
  Contains_value,

  NUM_TESTS,
};

string testName(int test) {
  if (test == Empty_size)
    return "Empty_size";
  if (test == SetValue_size)
    return "SetValue_size";
  if (test == Remove_size)
    return "Remove_size";

  if (test == Empty_traverse)
    return "Empty_traverse";
  if (test == Empty_clear)
    return "Empty_clear";
  if (test == NonEmpty_clear)
    return "NonEmpty_clear";

  if (test == Begin_value)
    return "Begin_value";
  if (test == End_value)
    return "End_value";
  if (test == Next_value)
    return "Next_value";
  if (test == Prev_value)
    return "Prev_value";
  if (test == SetValue_value)
    return "SetValue_value";
  if (test == Remove_value)
    return "Remove_value";
  if (test == GetValue_value)
    return "GetValue_value";

  if (test == Repeated_setValue)
    return "Repeated_setValue";
  if (test == MultiString_equals)
    return "MultiString_equals";
  if (test == ToString_equals)
    return "ToString_equals";
  if (test == PreString_equals)
    return "PreString_equals";
  if (test == Contains_value)
    return "Contains_value";

  return "";
}

uint8_t runTest(Dictionary *pA, Dictionary *pB, int test) {
  Dictionary A = *pA;
  Dictionary B = *pB;
  switch (test) {
  case Empty_size: {
    if (A.size() != 0)
      return 1;
    return 0;
  }
  case SetValue_size: {
    A.setValue("a", 1);
    A.setValue("b", 2);
    A.setValue("c", 3);
    A.setValue("e", 5);
    if (A.size() != 4)
      return 1;
    return 0;
  }
  case Remove_size: {
    A.setValue("c", 3);
    A.setValue("a", 1);
    A.setValue("b", 2);
    A.remove("a");
    if (A.size() != 2)
      return 1;
    A.setValue("e", 5);
    A.remove("c");
    if (A.size() != 2)
      return 2;
    return 0;
  }
  case Empty_traverse: {
    try {
      A.begin();
      A.currentVal();
      return 1;
    } catch (logic_error const &) {
    }
    try {
      A.end();
      A.currentVal();
      return 2;
    } catch (logic_error const &) {
    }
    try {
      A.currentVal();
      return 3;
    } catch (logic_error const &) {
    }
    try {
      A.currentKey();
      return 4;
    } catch (logic_error const &) {
    }

    return 0;
  }
  case Empty_clear: {
    A.clear();
    if (A.size() != 0) {
      return 1;
    }
    try {
      A.currentKey();
      return 2;
    } catch (logic_error const &) {
    }
    return 0;
  }
  case NonEmpty_clear: {
    A.setValue("c", 3);
    A.setValue("b", 2);
    A.setValue("a", 1);
    A.begin();
    A.clear();
    if (A.size() != 0) {
      return 1;
    }
    try {
      A.currentKey();
      return 2;
    } catch (logic_error const &) {
    }
    return 0;
  }
  case Begin_value: {
    A.setValue("d", 1);
    A.setValue("b", 5);
    A.setValue("c", 16);
    A.setValue("f", 176);
    A.setValue("e", 3214);

    A.begin();
    if (A.currentVal() != 5 || A.currentKey() != "b")
      return 1;

    A.remove("b");
    A.begin();

    if (A.currentVal() != 16 || A.currentKey() != "c")
      return 2;
    return 0;
  }
  case End_value: {
    A.setValue("d", 1);
    A.setValue("b", 5);
    A.setValue("c", 16);
    A.setValue("f", 176);
    A.setValue("e", 3214);

    A.end();
    if (A.currentVal() != 176 || A.currentKey() != "f")
      return 1;

    A.remove("f");
    A.end();

    if (A.currentVal() != 3214 || A.currentKey() != "e")
      return 2;
    return 0;
  }
  case Next_value: {
    A.setValue("a", 1);
    A.setValue("b", 5);
    A.setValue("c", 16);
    A.setValue("d", 176);
    A.setValue("e", 3214);
    A.begin();
    A.next();
    A.next();

    if (A.currentKey() != "c" || A.currentVal() != 16)
      return 1;
    A.next();
    A.next();
    A.next();
    if (A.hasCurrent())
      return 2;
    return 0;
  }
  case Prev_value: {
    A.setValue("a", 1);
    A.setValue("b", 5);
    A.setValue("e", 3214);
    A.end();
    A.prev();
    if (A.currentKey() != "b" || A.currentVal() != 5)
      return 1;
    A.prev();
    A.prev();
    if (A.hasCurrent())
      return 2;
    return 0;
  }
  case SetValue_value: {
    A.setValue("a", 1);
    A.setValue("b", 5);
    A.setValue("e", 10);
    A.end();
    A.setValue("h", 15);
    A.setValue("f", 20);
    A.setValue("i", 100);
    if (A.currentKey() != "e" || A.currentVal() != 10)
      return 1;
    A.end();
    A.prev();
    A.prev();
    if (A.currentKey() != "f" || A.currentVal() != 20)
      return 2;
    return 0;
  }
  case Remove_value: {
    A.setValue("d", 1);
    A.setValue("b", 5);
    A.setValue("a", 10);
    A.setValue("c", 15);
    A.setValue("f", 20);
    A.setValue("e", 25);
    A.setValue("g", 30);
    A.begin();
    A.remove("a");
    if (A.hasCurrent())
      return 1;
    A.end();
    A.remove("d");
    if (A.currentKey() != "g" || A.currentVal() != 30)
      return 2;
    A.remove("g");
    if (A.hasCurrent())
      return 3;
    return 0;
  }
  case GetValue_value: {
    if (A.contains("f"))
      return 1;
    A.setValue("a", 1);
    A.setValue("b", 5);
    A.setValue("e", 10);
    A.setValue("h", 15);
    A.setValue("f", 20);
    A.setValue("i", 100);

    if (A.getValue("f") != 20)
      return 2;

    A.remove("f");

    if (A.contains("f"))
      return 3;

    return 0;
  }
  case Repeated_setValue: {
    A.setValue("d", 2);
    A.setValue("d", 5);

    if (A.size() != 1)
      return 1;

    if (A.getValue("d") != 5)
      return 2;

    A.setValue("d", 1);

    if (A.getValue("d") != 1)
      return 3;

    A.remove("d");

    if (A.size() != 0)
      return 4;

    return 0;
  }
  case MultiString_equals: {
    A.clear();
    B.clear();
    if (A.to_string() != B.to_string())
      return 1;

    A.setValue("a", 1);
    A.setValue("b", 5);
    A.setValue("e", 10);
    A.setValue("h", 15);
    A.setValue("f", 20);
    A.setValue("i", 100);

    if (A.to_string() == B.to_string())
      return 2;

    B.setValue("a", 1);
    B.setValue("b", 5);
    B.setValue("e", 10);
    B.setValue("h", 15);
    B.setValue("f", 20);
    B.setValue("i", 100);

    if (A.to_string() != B.to_string())
      return 3;

    B.clear();
    B.setValue("i", 100);
    B.setValue("f", 20);
    B.setValue("h", 15);
    B.setValue("e", 10);
    B.setValue("b", 5);
    B.setValue("a", 1);

    if (A.to_string() != B.to_string())
      return 4;

    if (A.pre_string() == B.pre_string())
      return 5;

    B.clear();
    B.setValue("a", 10);
    B.setValue("b", 50);
    B.setValue("e", 100);
    B.setValue("h", 150);
    B.setValue("f", 200);
    B.setValue("i", 1000);

    if (A.to_string() == B.to_string())
      return 6;
    return 0;
  }

  case ToString_equals: {
    std::string ideal = "a : 1\nb : 5\ne : 10\nf : 20\nh : 15\ni : 100\n";
    A.setValue("a", 1);
    A.setValue("b", 5);
    A.setValue("e", 10);
    A.setValue("h", 15);
    A.setValue("f", 20);
    A.setValue("i", 100);
    std::string val = A.to_string();
    if (val != ideal)
      return 1;
    return 0;
  }
  case PreString_equals: {
    std::string ideal1 = "b\na\nf\ne\nh\ni\n";
    std::string ideal2 =
        "heat\neradicate\nbolstering\nabjuration\nfeather\ninsignia\n";
    std::string ideal3 = "heat\neradicate\nabjuration\nabbreviation\n"
                         "bolstering\nfeather\ngodsend\njustify\n"
                         "insignia\nquizzical\nleaflet\nsolemnly\n";
    A.setValue("a", 1);
    A.setValue("b", 5);
    A.setValue("e", 10);
    A.setValue("h", 15);
    A.setValue("f", 20);
    A.setValue("i", 100);

    if (A.pre_string() != ideal1)
      return 1;

    B.setValue("insignia", 100);
    B.setValue("feather", 20);
    B.setValue("heat", 15);
    B.setValue("eradicate", 10);
    B.setValue("bolstering", 5);
    B.setValue("abjuration", 1);

    if (B.pre_string() != ideal2)
      return 2;

    B.setValue("justify", 100);
    B.setValue("leaflet", 20);
    B.setValue("godsend", 15);
    B.setValue("quizzical", 10);
    B.setValue("solemnly", 5);
    B.setValue("abbreviation", 1);

    if (B.pre_string() != ideal3)
      return 3;
    return 0;
  }
  case Contains_value: {
    if (A.contains("d"))
      return 1;

    A.setValue("d", 1);
    if (!A.contains("d"))
      return 2;

    A.remove("d");
    if (A.contains("d"))
      return 1;
    return 0;
  }
  }
  return 255;
}

void segfault_handler(int signal) { // everyone knows what this is
  testStatus = 255;
  longjmp(test_crash, 1);
}

void exit_attempt_handler(void) { // only I decide when you are done
  if (disable_exit_handler)
    return; // allow this to be disabled
  testStatus = 255;
  longjmp(test_crash, 2);
}

void abrupt_termination_handler(int signal) { // program killed externally
  testStatus = 255;
  longjmp(test_crash, 3);
}

int main(int argc, char **argv) {
  if (argc > 2 || (argc == 2 && strcmp(argv[1], "-v") != 0)) {
    printf("Usage: %s [-v]", (argc > 0 ? argv[0] : "./ModelDictionaryTest"));
    exit(1);
  }

  printf("\n"); // more spacing
  if (argc == 2)
    printf("\n"); // consistency in verbose mode

  testsPassed = 0;
  disable_exit_handler = 0;
  atexit(exit_attempt_handler);
  signal(SIGSEGV, segfault_handler);
  // signal(SIGTERM, abrupt_termination_handler); // dangerous
  // signal(SIGINT, abrupt_termination_handler);
  // signal(SIGFPE, abrupt_termination_handler);
  // signal(SIGABRT, abrupt_termination_handler);
  for (uint8_t i = FIRST_TEST; i < NUM_TESTS; i++) {
    Dictionary A, B;
    uint8_t fail_type;
    try {

      testStatus = runTest(&A, &B, i);
      fail_type = setjmp(test_crash);
    } catch (...) {
      testStatus = 255;
      fail_type = 3;
    }
    if (argc == 2) { // it's verbose mode
      cout << "Test " + testName(i) + ": "
           << (testStatus == 0 ? GREEN "PASSED" NC : RED "FAILED" NC);
      if (testStatus == 255) {
        cout << ": due to a " RED
             << (fail_type == 1
                     ? "segfault"
                     : (fail_type == 2
                            ? "program exit"
                            : (fail_type == 3 ? "exception being thrown"
                                              : "program interruption")))
             << NC << endl;
        cout << "\nWARNING: Program will now stop running tests\n" << endl;
        break;
      } else if (testStatus != 0) {
        cout << ": test " CYAN << testStatus << NC << endl;
      } else {
        cout << endl;
      }
    }
    if (testStatus == 0) {
      testsPassed++;
    }
  }

  disable_exit_handler = 1;

  uint8_t totalScore = (testsPassed / 2) * 5 + (testsPassed % 2) * 3;

  if (argc == 2 && testStatus != 255)
    cout << "\nYou passed " << unsigned(testsPassed) << " out of " << NUM_TESTS
         << " tests" << endl;
  else if (testStatus == 255) {
    totalScore = CHARITY; // charity points
    if (argc == 2)
      cout << RED "Receiving charity points because your program crashes" NC
           << endl;
  }
  cout << "You will receive " << unsigned(totalScore) << " out of " << MAXSCORE
       << " possible points on the Dictionary Functionality Test\n"
       << endl;
  exit(0);
  return 0;
}
