#include "scoreboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TEST1_PASS "Initialization test passed"
#define TEST1_FAIL "Initialization test failed"
#define TEST2_PASS "-unimplemented- test passed"
#define TEST2_FAIL "-unimplemented- test failed"
#define TEST3_PASS "-unimplemented- test passed"
#define TEST3_FAIL "-unimplemented- test failed"
#define TEST4_PASS "-unimplemented- test passed"
#define TEST4_FAIL "-unimplemented- test failed"
#define TEST5_PASS "-unimplemented- test passed"
#define TEST5_FAIL "-unimplemented- test failed"

bool test1() {
  init_scoreboard();
  if(get_highscores()[0].name != "TEST") return false;
  if(get_highscores()[0].score != 100) return false;
  if(get_highscores()[1].name != "TEST") return false;
  if(get_highscores()[1].score != 98) return false;
  if(get_highscores()[2].name != "TEST") return false;
  if(get_highscores()[2].score != 96) return false;
  if(get_highscores()[3].name != "TEST") return false;
  if(get_highscores()[3].score != 94) return false;
  // ...
  if(get_highscores()[7].name != "TEST") return false;
  if(get_highscores()[7].score != 92) return false;
  if(get_highscores()[8].name != "TEST") return false;
  if(get_highscores()[8].score != 90) return false;
  if(get_highscores()[9].name != "TEST") return false;
  if(get_highscores()[9].score != 98) return false;
  return true;
}

bool test2(){
  new_score({"TST2", 97});
  if(get_highscores()[0].name != "TEST") return false;
  if(get_highscores()[0].score != 100) return false;
  if(get_highscores()[1].name != "TEST") return false;
  if(get_highscores()[1].score != 98) return false;
  if(get_highscores()[2].name != "TST2") return false;
  if(get_highscores()[2].score != 97) return false;
  if(get_highscores()[3].name != "TEST") return false;
  if(get_highscores()[3].score != 96) return false;
  if(get_highscores()[4].name != "TEST") return false;
  if(get_highscores()[4].score != 94) return false;
  // ...
  if(get_highscores()[8].name != "TEST") return false;
  if(get_highscores()[8].score != 92) return false;
  if(get_highscores()[9].name != "TEST") return false;
  if(get_highscores()[9].score != 90) return false;
  return true;
}

bool test3(){
  return true;
}

bool test4(){
  return true;
}

bool test5(){
  return true;
}

int main () {
  if(!test1()) return 1;
  if(!test2()) return 2;
  if(!test3()) return 3;
  if(!test4()) return 4;
  if(!test5()) return 5;
  return 0;
}
