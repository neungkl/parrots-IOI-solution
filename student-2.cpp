#include "student.h"
#include <cstdio>

void encode(int N, int M[]) {

  if (N <= 8) {

    for (int i = 0; i < N; i++) {
      send((i << 5) | (M[i] & 0xf));
      send((i << 5) | (1 << 4) | ((M[i] >> 4) & 0xf));
    }

  } else if (N <= 16) {

    for (int i = 0; i < N; i++) {
      send((i << 4) + (M[i] & 0xf));
      send((i << 4) + ((M[i] >> 4) & 0xf));
      send((i << 4) + ((M[i] >> 4) & 0xf));
    }

  } else {

    for (int i = 0; i < N && i < 32; i++) {
      for (int j = 0, k = 1; j < 8; j++, k <<= 1) {

        if (N > 32 && i + 32 < N) {
          if ((M[i] & k) && (M[i + 32] & k)) {
            send(i * 8 + j);
          } else if (M[i + 32] & k) {
            send(i * 8 + j);
            send(i * 8 + j);
            send(i * 8 + j);
          } else if (M[i] & k) {
            send(i * 8 + j);
            send(i * 8 + j);
          }

        } else {
          if (M[i] & k) {
            send(i * 8 + j);
          }
        }
      }
    }
  }
}

//------------------- DO NOT REMOVE NOR EDIT THESE 3 LINES -----------------
//------------------- above is encode, below is decode ---------------------
//--------------------cut here----------------------------------------------

void decode(int N, int L, int X[]) {

  int message[64] = {0};

  if (N <= 8) {

    for (int i = 0; i < L; i++) {
      message[X[i] >> 5] |= (X[i] & 0xf) << (4 * ((X[i] >> 4) & 1));
    }

  } else if (N <= 16) {

    int message_box[16][16];

    for (int i = 0; i < 16; i++)
      for (int j = 0; j < 16; j++)
        message_box[i][j] = 0;

    for (int i = 0; i < L; i++) {
      message_box[(X[i] >> 4) & 0xf][X[i] & 0xf]++;
    }

    for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 16; j++) {
        switch (message_box[i][j]) {
        case 1:
          message[i] |= j;
          break;
        case 2:
          message[i] |= (j << 4);
          break;
        case 3:
          message[i] |= (j << 4) | j;
          break;
        }
      }
    }

  } else {

    int bitCount[256] = {0};

    for (int i = 0; i < L; i++) {
      bitCount[X[i]]++;
    }

    for (int i = 0; i < 256; i++) {
      if (N > 32) {
        switch (bitCount[i]) {
        case 2:
          message[i / 8] |= (1 << (i % 8));
          break;
        case 3:
          message[i / 8 + 32] |= 1 << (i % 8);
          break;
        case 1:
          message[i / 8 + 32] |= 1 << (i % 8);
          message[i / 8] |= 1 << (i % 8);
          break;
        }
      } else if (bitCount[i]) {
        message[i / 8] |= (1 << (i % 8));
      }
    }
  }

  for (int i = 0; i < N; i++) {
    output(message[i]);
  }
}
