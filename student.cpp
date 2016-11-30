#include "student.h"
void encode(int N, int M[]) {
  //this simply send the message as it is
  //this is definitely wrong
  for (int i = 0;i < N;i++) {
    send(M[i]);
  }
}

//------------------- DO NOT REMOVE NOR EDIT THESE 3 LINES -----------------
//------------------- above is encode, below is decode ---------------------
//--------------------cut here----------------------------------------------

void decode(int N, int L, int X[]) {
  //this simply output the message as it is
  //this is definitely wrong
  for (int i = 0;i < N;i++) {
    output(X[i]);
  }
}
