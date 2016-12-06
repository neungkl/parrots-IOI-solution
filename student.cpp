#include "student.h"
#include <cstdio>

struct BigInteger {
  const int LENGTH = 20;
  const long long NUM_SIZE = 1000000000000000LL;

  long long n[20];
  int hid;

  BigInteger(int num) {
    for (int i = LENGTH - 1; i > 0; i--)
      n[i] = 0;
    n[0] = num;
    hid = 1;
  }

  BigInteger() { BigInteger(0); }

  BigInteger &operator=(int &other) {
    n[0] = other;
    hid = 1;
    return *this;
  }

  bool operator<=(const BigInteger &other) {
    if (hid > other.hid)
      return false;
    if (hid < other.hid)
      return true;
    for (int i = hid - 1; i >= 0; i--) {
      if (n[i] > other.n[i])
        return false;
      if (n[i] < other.n[i])
        return true;
    }
    return true;
  }

  bool operator<(const BigInteger &other) {
    if (hid > other.hid)
      return false;
    if (hid < other.hid)
      return true;
    for (int i = hid - 1; i >= 0; i--) {
      if (n[i] > other.n[i])
        return false;
      if (n[i] < other.n[i])
        return true;
    }
    return false;
  }

  BigInteger &operator=(const BigInteger &other) {
    for (int i = 0; i < other.hid; i++) {
      n[i] = other.n[i];
    }
    hid = other.hid;
    return *this;
  }

  BigInteger &operator+(const BigInteger &other) {
    BigInteger *ret = new BigInteger(0);

    long long overflow = 0;
    int mhid = hid > other.hid ? hid : other.hid;
    for (int i = 0; i < mhid; i++) {
      ret->n[i] = overflow;
      if(i < hid) ret->n[i] = ret->n[i] + n[i];
      if(i < other.hid) ret->n[i] = ret->n[i] + other.n[i];
      overflow = ret->n[i] / NUM_SIZE;
      ret->n[i] %= NUM_SIZE;
    }

    if (overflow > 0) {
      ret->n[mhid++] = overflow;
    }
    ret->hid = mhid;

    return *ret;
  }

  BigInteger &operator-(const BigInteger &other) {
    BigInteger *ret = new BigInteger(0);

    int mhid = hid;

    for (int i = 0; i < mhid; i++) {
      ret->n[i] += this->n[i] - other.n[i];
      if (ret->n[i] < 0) {
        ret->n[i] += NUM_SIZE;
        ret->n[i + 1]--;
      }
    }

    while (ret->n[mhid - 1] == 0 && mhid > 1) {
      mhid--;
    }

    ret->hid = mhid;

    return *ret;
  }

  BigInteger pow(int p) {
    BigInteger k(n[0]);
    for (int i = 0; i < p; i++) {
      k = k + k;
    }
    return k;
  }

  BigInteger shift8() {

    int mod = 1<<8;
    int left = 0;

    for(int i = hid - 1; i>=0; i--) {
      n[i] += left * NUM_SIZE;
      left = n[i] % mod;
      n[i] /= mod;
    }

    while (n[hid - 1] == 0 && hid > 1) {
      hid--;
    }
  }

  void print(char *end) {
    for (int i = hid - 1; i >= 0; i--) {
      if (i == hid - 1)
        printf("%lld", n[i]);
      else
        printf(" %015lld", n[i]);
    }
    printf(":%d", hid);
    printf(end);
  }
};

BigInteger dp[512][256];

void encode(int N, int M[]) {

  const int N_MAX = 256;

  for (int i = 0; i < N_MAX * 2; i++) {
    for (int j = 0; j < N_MAX; j++) {
      if (i == 0) {
        dp[i][j] = j;
      } else if (i == 1 && j == 0) {
        dp[i][0] = dp[i - 1][N_MAX - 1] + 1;
      } else if (j == 0) {
        dp[i][0] = (dp[i - 1][0] - dp[i - 2][0]) + dp[i - 1][N_MAX - 1];
      } else {
        dp[i][j] = (dp[i - 1][j] - dp[i - 1][0]) + dp[i][j - 1];
      }
    }
  }

  BigInteger num(0);
  for (int i = 0, p = 0; i < N; i++, p++) {
    BigInteger num2(M[i]);
    num = num + num2.pow(i * 8);
  }

  int r, c;
  r = 0, c = 0;
  while (dp[r][c] <= num) {
    if (++c >= N_MAX) {
      c = 0;
      ++r;
    }
  }
  if (--c < 0) {
    c = N_MAX - 1;
    --r;
  }

  // num.print("\n");
  num = num - dp[r][c];
  // printf("%d %d\n", r, c);
  send(c);
  for (--r; r >= 0; --r) {
    // printf("======#======\n");
    // num.print("\n");
    // dp[r][0].print("\n");
    num = num + dp[r][0];
    // num.print("\n");
    while (num < dp[r][c])
      c--;

    // printf("======\n");
    // printf("%d %d\n", num < dp[r][c], num < dp[r][c+1]);
    // num.print("\n");
    // dp[r][c].print("\n");
    // dp[r][c+1].print("\n");

    // printf("%d\n", c);
    send(c);
    num = num - dp[r][c];

    // num.print("\n");
  }
  // num.print("haha");
}

//------------------- DO NOT REMOVE NOR EDIT THESE 3 LINES -----------------
//------------------- above is encode, below is decode ---------------------
//--------------------cut here----------------------------------------------

#include <algorithm>
using namespace std;

void decode(int N, int L, int X[]) {

  const int N_MAX = 256;

  for (int i = 0; i < N_MAX * 2; i++) {
    for (int j = 0; j < N_MAX; j++) {
      if (i == 0) {
        dp[i][j] = j;
      } else if (i == 1 && j == 0) {
        dp[i][0] = dp[i - 1][N_MAX - 1] + 1;
      } else if (j == 0) {
        dp[i][0] = (dp[i - 1][0] - dp[i - 2][0]) + dp[i - 1][N_MAX - 1];
      } else {
        dp[i][j] = (dp[i - 1][j] - dp[i - 1][0]) + dp[i][j - 1];
      }
    }
  }

  sort(X, X+L);
  reverse(X, X+L);

  BigInteger num = 0;
  int r = L - 1;
  for (int i = 0; i < L; i++, r--) {
    if (i == 0) {
      num = num + dp[r][X[i]];
    } else {
      num = num + dp[r][X[i]] - dp[r][0];
    }
  }

  // num.print("\n");

  for(int i=0; i<N; i++) {
    output(num.n[0] & 0xff);
    num.shift8();
  }
}
