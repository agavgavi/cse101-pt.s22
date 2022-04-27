#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"

#define FIRST_TEST Empty_length
#define MAXSCORE 10
#define CHARITY 0

#define RED "\033[0;31m"
#define CYAN "\033[0;36m"
#define GREEN "\033[0;32m"
#define NC "\033[0m"

static uint8_t testsPassed;
static volatile sig_atomic_t testStatus;
static uint8_t disable_exit_handler;
jmp_buf test_crash;

enum Test_e {
  Empty_length = 0,
  Append_length,
  Prepend_length,
  InsertAfter_length,
  InsertBefore_length,
  DeleteFront_length,
  DeleteBack_length,
  Delete_length,

  EmptyList_index,
  MoveFront_index,
  MoveBack_index,
  MoveNext_index,
  MovePrev_index,
  Append_index,
  Prepend_index,
  InsertAfter_index,
  InsertBefore_index,
  DeleteFront_index,
  DeleteBack_index,
  Delete_index,

  Empty_clear,
  NonEmpty_clear,

  Set_get,
  Set_front,
  NonEmpty_front,
  Set_back,
  NonEmpty_back,

  NUM_TESTS,
};

char *testName(int test) {
  if (test == Empty_length)
    return "Empty_length";
  if (test == Append_length)
    return "Append_length";
  if (test == Prepend_length)
    return "Prepend_length";
  if (test == InsertAfter_length)
    return "InsertAfter_length";
  if (test == InsertBefore_length)
    return "InsertBefore_length";
  if (test == DeleteFront_length)
    return "DeleteFront_length";
  if (test == DeleteBack_length)
    return "DeleteBack_length";
  if (test == Delete_length)
    return "Delete_length";

  if (test == EmptyList_index)
    return "EmptyList_index";
  if (test == MoveFront_index)
    return "MoveFront_index";
  if (test == MoveBack_index)
    return "MoveBack_index";
  if (test == MoveNext_index)
    return "MoveNext_index";
  if (test == MovePrev_index)
    return "MovePrev_index";
  if (test == Append_index)
    return "Append_index";
  if (test == Prepend_index)
    return "Prepend_index";
  if (test == InsertAfter_index)
    return "InsertAfter_index";
  if (test == InsertBefore_index)
    return "InsertBefore_index";
  if (test == DeleteFront_index)
    return "DeleteFront_index";
  if (test == DeleteBack_index)
    return "DeleteBack_index";
  if (test == Delete_index)
    return "Delete_index";

  if (test == Empty_clear)
    return "Empty_clear";
  if (test == NonEmpty_clear)
    return "NonEmpty_clear";

  if (test == Set_get)
    return "Set_get";
  if (test == Set_front)
    return "Set_front";
  if (test == NonEmpty_front)
    return "NonEmpty_front";
  if (test == Set_back)
    return "Set_back";
  if (test == NonEmpty_back)
    return "NonEmpty_back";

  return "";
}

int *newData(int data) {
  int *N = malloc(sizeof(int));
  *N = data;
  return N;
}

uint8_t runTest(List *pA, int test) {
  List A = *pA;

  switch (test) {
  case Empty_length: {
    if (length(A) != 0)
      return 1;
    return 0;
  }
  case Append_length: {
    append(A, newData(1));
    append(A, newData(2));
    append(A, newData(3));
    append(A, newData(5));
    if (length(A) != 4)
      return 1;
    return 0;
  }
  case Prepend_length: {
    prepend(A, newData(6));
    prepend(A, newData(4));
    prepend(A, newData(2));
    prepend(A, newData(1));
    if (length(A) != 4)
      return 1;
    return 0;
  }
  case InsertAfter_length: {
    append(A, newData(1));
    append(A, newData(2));
    append(A, newData(3));
    append(A, newData(5));
    moveFront(A);
    insertAfter(A, newData(12));
    if (length(A) != 5)
      return 1;
    return 0;
  }
  case InsertBefore_length: {
    prepend(A, newData(76));
    prepend(A, newData(4));
    prepend(A, newData(3));
    prepend(A, newData(1));
    moveFront(A);
    insertBefore(A, newData(100));
    if (length(A) != 5)
      return 1;
    return 0;
  }
  case DeleteFront_length: {
    prepend(A, newData(76));
    prepend(A, newData(4));
    deleteFront(A);
    prepend(A, newData(3));
    prepend(A, newData(1));
    moveFront(A);
    insertBefore(A, newData(115));
    deleteFront(A);
    if (length(A) != 3)
      return 1;
    return 0;
  }
  case DeleteBack_length: {
    append(A, newData(1));
    deleteBack(A);
    append(A, newData(2));
    append(A, newData(3));
    append(A, newData(5));
    moveFront(A);
    insertAfter(A, newData(12));
    deleteBack(A);
    if (length(A) != 3)
      return 1;
    return 0;
  }
  case Delete_length: {
    append(A, newData(1));
    append(A, newData(2));
    moveFront(A);
    delete (A);
    append(A, newData(3));
    append(A, newData(5));
    moveFront(A);
    insertAfter(A, newData(12));
    delete (A);
    if (length(A) != 3)
      return 1;
    return 0;
  }
  case EmptyList_index: {
    if (index(A) != -1)
      return 1;
    return 0;
  }
  case MoveFront_index: {
    append(A, newData(1));
    append(A, newData(5));
    append(A, newData(16));
    append(A, newData(176));
    append(A, newData(3214));
    moveFront(A);
    if (index(A) != 0)
      return 1;
    return 0;
  }
  case MoveBack_index: {
    append(A, newData(1));
    append(A, newData(5));
    append(A, newData(16));
    append(A, newData(176));
    append(A, newData(3214));
    moveBack(A);
    if (index(A) != 4)
      return 1;
    return 0;
  }
  case MoveNext_index: {
    append(A, newData(1));
    append(A, newData(5));
    append(A, newData(16));
    append(A, newData(176));
    append(A, newData(3214));
    moveFront(A);
    moveNext(A);
    moveNext(A);
    if (index(A) != 2)
      return 1;
    moveNext(A);
    moveNext(A);
    moveNext(A);
    if (index(A) != -1)
      return 2;
    return 0;
  }
  case MovePrev_index: {
    append(A, newData(1));
    append(A, newData(5));
    append(A, newData(3214));
    moveBack(A);
    movePrev(A);
    if (index(A) != 1)
      return 1;
    movePrev(A);
    movePrev(A);
    if (index(A) != -1)
      return 2;
    return 0;
  }
  case Append_index: {
    append(A, newData(1));
    append(A, newData(5));
    append(A, newData(7));
    moveBack(A);
    append(A, newData(45));
    append(A, newData(51));
    append(A, newData(3214));
    if (index(A) != 2)
      return 1;
    moveBack(A);
    movePrev(A);
    movePrev(A);
    if (index(A) != 3)
      return 2;
    moveFront(A);
    movePrev(A);
    if (index(A) != -1)
      return 3;
    return 0;
  }
  case Prepend_index: {
    prepend(A, newData(1));
    prepend(A, newData(5));
    prepend(A, newData(7));
    moveFront(A);
    prepend(A, newData(45));
    prepend(A, newData(51));
    prepend(A, newData(3214));
    prepend(A, newData(314));
    prepend(A, newData(324));
    if (index(A) != 5)
      return 1;
    moveBack(A);
    movePrev(A);
    prepend(A, newData(234));
    movePrev(A);
    if (index(A) != 6)
      return 2;
    moveFront(A);
    movePrev(A);
    if (index(A) != -1)
      return 3;
    return 0;
  }
  case InsertAfter_index: {
    append(A, newData(5));
    append(A, newData(6));
    append(A, newData(4));
    append(A, newData(33));
    append(A, newData(2));
    append(A, newData(1));
    moveBack(A);
    insertAfter(A, newData(75));
    moveNext(A);
    if (index(A) != 6)
      return 1;
    insertAfter(A, newData(345));
    moveBack(A);
    if (index(A) != 7)
      return 2;
    return 0;
  }
  case InsertBefore_index: {
    prepend(A, newData(34));
    prepend(A, newData(4));
    prepend(A, newData(354));
    prepend(A, newData(3674));
    moveBack(A);
    insertBefore(A, newData(435));
    if (index(A) != 4)
      return 1;
    prepend(A, newData(324));
    prepend(A, newData(33464));
    prepend(A, newData(3498));
    moveFront(A);
    insertBefore(A, newData(67));
    if (index(A) != 1)
      return 2;
    return 0;
  }
  case DeleteFront_index: {
    prepend(A, newData(5));
    prepend(A, newData(65));
    prepend(A, newData(43));
    prepend(A, newData(2));
    prepend(A, newData(8));
    prepend(A, newData(1));
    moveFront(A);
    deleteFront(A);
    if (index(A) != -1)
      return 1;
    moveBack(A);
    deleteFront(A);
    if (index(A) != 3)
      return 2;
    return 0;
  }
  case DeleteBack_index: {
    prepend(A, newData(5));
    prepend(A, newData(65));
    prepend(A, newData(43));
    prepend(A, newData(2));
    prepend(A, newData(8));
    prepend(A, newData(1));
    moveBack(A);
    deleteBack(A);
    if (index(A) != -1)
      return 1;
    moveFront(A);
    deleteBack(A);
    moveNext(A);
    if (index(A) != 1)
      return 2;
    return 0;
  }
  case Delete_index: {
    prepend(A, newData(5));
    prepend(A, newData(65));
    prepend(A, newData(43));
    moveBack(A);
    delete (A);
    if (index(A) != -1)
      return 1;
    prepend(A, newData(2));
    prepend(A, newData(8));
    prepend(A, newData(1));
    moveBack(A);
    if (index(A) != 4)
      return 2;
    delete (A);
    moveBack(A);
    if (index(A) != 3)
      return 3;
    moveFront(A);
    delete (A);
    moveFront(A);
    if (index(A) != 0)
      return 4;
    delete (A);
    if (index(A) != -1)
      return 5;
    return 0;
  }
  case Empty_clear: {
    clear(A);
    if (index(A) != -1 || length(A) != 0)
      return 1;
    return 0;
  }
  case NonEmpty_clear: {
    append(A, newData(1));
    prepend(A, newData(2));
    moveFront(A);
    clear(A);
    if (index(A) != -1 || length(A) != 0)
      return 1;
    return 0;
  }
  case Set_get: {
    append(A, newData(1));
    prepend(A, newData(2));
    deleteFront(A);
    moveBack(A);
    if (*(int *)get(A) != 1)
      return 1;
    return 0;
  }
  case Set_front: {
    append(A, newData(1));
    prepend(A, newData(5));
    moveBack(A);
    if (*(int *)front(A) != 5)
      return 1;
    return 0;
  }
  case NonEmpty_front: {
    prepend(A, newData(5));
    append(A, newData(7));
    prepend(A, newData(2));
    moveFront(A);
    insertBefore(A, newData(43));
    deleteFront(A);
    delete (A);
    if (*(int *)front(A) != 5)
      return 1;
    return 0;
  }
  case Set_back: {
    prepend(A, newData(1));
    append(A, newData(5));
    moveFront(A);
    if (*(int *)back(A) != 5)
      return 1;
    return 0;
  }
  case NonEmpty_back: {
    append(A, newData(5));
    prepend(A, newData(7));
    append(A, newData(2));
    moveBack(A);
    insertAfter(A, newData(43));
    deleteBack(A);
    delete (A);
    if (*(int *)back(A) != 5)
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
    printf("Usage: %s [-v]", (argc > 0 ? argv[0] : "./ListTest"));
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
    List A = newList();
    testStatus = runTest(&A, i);
    freeList(&A);
    uint8_t fail_type = setjmp(test_crash);
    if (argc == 2) { // it's verbose mode
      printf("Test %s: %s", testName(i),
             testStatus == 0 ? GREEN "PASSED" NC : RED "FAILED" NC);
      if (testStatus == 255) {
        printf(": due to a " RED "%s" NC "\n", fail_type == 1 ? "segfault"
                                               : fail_type == 2
                                                   ? "program exit"
                                                   : "program interruption");
        printf(RED "\nWARNING: Program will now stop running tests\n\n" NC);
        break;
      } else if (testStatus != 0) {
        printf(": test" CYAN " %d\n" NC, testStatus);
      } else {
        printf("\n");
      }
    }
    if (testStatus == 0) {
      testsPassed++;
    }
  }

  disable_exit_handler = 1;

  uint8_t totalScore = (MAXSCORE - NUM_TESTS / 3) + testsPassed / 3;

  if (argc == 2 && testStatus != 255)
    printf("\nYou passed %d out of %d tests\n", testsPassed, NUM_TESTS);
  else if (testStatus == 255) {
    totalScore = CHARITY; // charity points
    if (argc == 2)
      printf(RED "Receiving charity points because your program crashes\n" NC);
  }
  printf("\nYou will receive %d out of %d possible points on the ListTests\n\n",
         totalScore, MAXSCORE);
  exit(0);
  return 0;
}
