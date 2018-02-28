#include "scoreboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool test1() {
  init_scoreboard();
  if(get_highscores()[0].name != "DBRO") return false;
  if(get_highscores()[0].score != 9999999999) return false;
  if(get_highscores()[1].name != "OS K") return false;
  if(get_highscores()[1].score != 1234500) return false;
  if(get_highscores()[2].name != "HU S") return false;
  if(get_highscores()[2].score != 1234500) return false;
  if(get_highscores()[3].name != "TEST") return false;
  if(get_highscores()[3].score != 1000000) return false;
  // ...
  if(get_highscores()[7].name != "AAAA") return false;
  if(get_highscores()[7].score != 300000) return false;
  if(get_highscores()[8].name != "BBBB") return false;
  if(get_highscores()[8].score != 200000) return false;
  if(get_highscores()[9].name != "CCCC") return false;
  if(get_highscores()[9].score != 100000) return false;
  return true;
}

bool test2(){
  return true;
}

bool test3(){
  return true;
}

bool test4(){
  return true;
}

bool test6(){
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
