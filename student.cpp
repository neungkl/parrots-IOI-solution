#include "student.h"
#include <cstdio>
#include <vector>
using namespace std;

#define duplicate
#ifdef duplicate

const long long NUM_SIZE = 10000000000000000LL;

struct BigInteger {

  long long n[16];
  int hid;

  BigInteger(int num) {
    n[0] = num;
    hid = 1;
  }

  BigInteger() { BigInteger(0); }
};

void reset(BigInteger &b) {
  b.n[0] = 0;
  b.hid = 1;
}

void assign(BigInteger &b, int other) {
  b.n[0] = other;
  b.hid = 1;
}

void assign(BigInteger &b, const BigInteger &other) {
  for (int i = 0; i < other.hid; i++) {
    b.n[i] = other.n[i];
  }
  b.hid = other.hid;
}

bool isLessThanEqual(BigInteger &b, const BigInteger &other) {
  if (b.hid > other.hid)
    return false;
  if (b.hid < other.hid)
    return true;
  for (int i = b.hid - 1; i >= 0; i--) {
    if (b.n[i] > other.n[i])
      return false;
    if (b.n[i] < other.n[i])
      return true;
  }
  return true;
}

bool isLessThan(BigInteger &b, const BigInteger &other) {
  if (b.hid > other.hid)
    return false;
  if (b.hid < other.hid)
    return true;
  for (int i = b.hid - 1; i >= 0; i--) {
    if (b.n[i] > other.n[i])
      return false;
    if (b.n[i] < other.n[i])
      return true;
  }
  return false;
}

void add(BigInteger &b, const BigInteger &other) {
  long long overflow = 0;
  int mhid = b.hid > other.hid ? b.hid : other.hid;
  for (int i = 0; i < mhid; i++) {
    long long sum = overflow;
    if (i < b.hid)
      sum += b.n[i];
    if (i < other.hid)
      sum += other.n[i];
    overflow = sum / NUM_SIZE;
    b.n[i] = sum % NUM_SIZE;
  }

  if (overflow > 0) {
    b.n[mhid++] = overflow;
  }
  b.hid = mhid;
}

void subtract(BigInteger &b, const BigInteger &other) {
  for (int i = 0; i < b.hid; i++) {
    b.n[i] -= other.n[i];
    if (b.n[i] < 0) {
      b.n[i] += NUM_SIZE;
      b.n[i + 1]--;
    }
  }

  while (b.n[b.hid - 1] == 0 && b.hid > 1) {
    b.hid--;
  }
}

void pow(BigInteger &b, int p) {
  for (int i = 0; i < p; i++) {
    add(b, b);
  }
}

void shift(BigInteger &b, int k) {

  int mod = 1 << k;
  int left = 0;

  for (int i = b.hid - 1; i >= 0; i--) {
    b.n[i] += left * NUM_SIZE;
    left = b.n[i] % mod;
    b.n[i] /= mod;
  }

  while (b.n[b.hid - 1] == 0 && b.hid > 1) {
    b.hid--;
  }
}

void print(BigInteger &b, char end[]) {
  for (int i = b.hid - 1; i >= 0; i--) {
    if (i == b.hid - 1)
      printf("%lld", b.n[i]);
    else
      printf(" %017lld", b.n[i]);
  }
  printf(":%d", b.hid);
  printf(end);
}

BigInteger dp[512][256];
bool everCalc = false;

#endif

void encode(int N, int M[]) {

  const int N_MAX = 256;

  BigInteger biOne(1);

  if(!everCalc) {

    for (int i = 0; i < N_MAX * 3 / 2; i++) {
      for (int j = 0; j < N_MAX; j++) {
        reset(dp[i][j]);
      }
    }

    for (int i = 0; i < N_MAX * 3 / 2; i++) {
      for (int j = 0; j < N_MAX; j++) {
        if (i == 0) {
          assign(dp[i][j], j);
        } else if (i == 1 && j == 0) {
          add(dp[i][0], dp[i - 1][N_MAX - 1]);
          add(dp[i][0], biOne);
        } else if (j == 0) {
          add(dp[i][0], dp[i - 1][0]);
          add(dp[i][0], dp[i - 1][N_MAX - 1]);
          subtract(dp[i][0], dp[i - 2][0]);
        } else {
          add(dp[i][j], dp[i - 1][j]);
          add(dp[i][j], dp[i][j - 1]);
          subtract(dp[i][j], dp[i - 1][0]);
        }
      }
    }

    everCalc = 1;
  }

  // for(int i=0; i<4; i++) {
  //   for(int j=0; j<4; j++) {
  //     print(dp[i][j], ""); printf(" ");
  //   }
  //   printf("\n");
  // }

  // return ;

  BigInteger num(0);
  for (int i = 0, p = 0; i < N; i++, p++) {
    BigInteger m(M[i]);
    pow(m, i * 8);
    add(num, m);
  }

  // print(num, "\n");

  int r, c;
  r = 0, c = 0;
  while (isLessThanEqual(dp[r][c], num)) {
    if (++c >= N_MAX) {
      c = 0;
      ++r;
    }
  }
  if (--c < 0) {
    c = N_MAX - 1;
    --r;
  }

  subtract(num, dp[r][c]);
  send(c);
  for (--r; r >= 0; --r) {
    add(num, dp[r][0]);
    while (isLessThan(num, dp[r][c]))
      c--;

    send(c);
    subtract(num, dp[r][c]);
  }
}

//------------------- DO NOT REMOVE NOR EDIT THESE 3 LINES -----------------
//------------------- above is encode, below is decode ---------------------
//--------------------cut here----------------------------------------------

#include "student.h"
#include <algorithm>
using namespace std;

#ifndef duplicate


#endif

void decode(int N, int L, int X[]) {

  const int N_MAX = 256;

  BigInteger biOne(1);

  if(!everCalc) {

    for (int i = 0; i < N_MAX * 3 / 2; i++) {
      for (int j = 0; j < N_MAX; j++) {
        assign(dp[i][j], 0);
      }
    }

    for (int i = 0; i < N_MAX * 3 / 2; i++) {
      for (int j = 0; j < N_MAX; j++) {
        if (i == 0) {
          assign(dp[i][j], j);
        } else if (i == 1 && j == 0) {
          add(dp[i][0], dp[i - 1][N_MAX - 1]);
          add(dp[i][0], biOne);
        } else if (j == 0) {
          add(dp[i][0], dp[i - 1][0]);
          add(dp[i][0], dp[i - 1][N_MAX - 1]);
          subtract(dp[i][0], dp[i - 2][0]);
        } else {
          add(dp[i][j], dp[i - 1][j]);
          add(dp[i][j], dp[i][j - 1]);
          subtract(dp[i][j], dp[i - 1][0]);
        }
      }
    }

    everCalc = true;
  }

  // for(int i=0; i<4; i++) {
  //   for(int j=0; j<4; j++) {
  //     print(dp[i][j], ""); printf(" ");
  //   }
  //   printf("\n");
  // }

  // return ;

  sort(X, X + L);
  reverse(X, X + L);

  BigInteger num(0);
  int r = L - 1;
  for (int i = 0; i < L; i++, r--) {
    if (i == 0) {
      add(num, dp[r][X[i]]);
    } else {
      add(num, dp[r][X[i]]);
      subtract(num, dp[r][0]);
    }
  }

  // print(num, "\n");

  for (int i = 0; i < N; i++) {
    output(num.n[0] & 0xff);
    shift(num, 8);
  }
}
