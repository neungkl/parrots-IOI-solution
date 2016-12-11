#include "student.h"
#include <cstdio>

int everCalcEncode = 0;

void encode(int N, int M[]) {

  struct BigInteger {
    const int LENGTH = 20;
    const long long NUM_SIZE = 10000000000000000LL;

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

    BigInteger &operator+=(const BigInteger &other) {

      long long overflow = 0;
      int mhid = hid > other.hid ? hid : other.hid;
      for (int i = 0; i < mhid; i++) {
        long long sum = overflow;
        if (i < hid)
          sum += n[i];
        if (i < other.hid)
          sum += other.n[i];
        overflow = sum / NUM_SIZE;
        n[i] = sum % NUM_SIZE;
      }

      if (overflow > 0) {
        n[mhid++] = overflow;
      }
      hid = mhid;

      return *this;
    }

    BigInteger &operator-=(const BigInteger &other) {

      for (int i = 0; i < hid; i++) {
        n[i] -= other.n[i];
        if (n[i] < 0) {
          n[i] += NUM_SIZE;
          n[i + 1]--;
        }
      }

      while (n[hid - 1] == 0 && hid > 1) {
        hid--;
      }

      return *this;
    }

    BigInteger pow(int p) {
      BigInteger k(n[0]);
      BigInteger biCache(0);
      for (int i = 0; i < p; i++) {
        biCache = k;
        biCache += k;
        k = biCache;
      }
      return k;
    }

    void shift(int k) {

      int mod = 1 << k;
      int left = 0;

      for (int i = hid - 1; i >= 0; i--) {
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
          printf(" %017lld", n[i]);
      }
      printf(":%d", hid);
      printf(end);
    }
  };

  BigInteger dp[512][128];

  const int N_MAX = 128;

  if (!everCalcEncode) {

    BigInteger biOne(1);

    for (int i = 0; i < N_MAX * 3; i++) {
      for (int j = 0; j < N_MAX; j++) {
        dp[i][j] = 0;
      }
    }

    for (int i = 0; i < N_MAX * 3; i++) {
      for (int j = 0; j < N_MAX; j++) {
        if (i == 0) {
          dp[i][j] = j;
        } else if (i == 1 && j == 0) {
          dp[i][0] = dp[i - 1][N_MAX - 1];
          dp[i][0] += biOne;
        } else if (j == 0) {
          dp[i][0] = dp[i - 1][0];
          dp[i][0] += dp[i - 1][N_MAX - 1];
          dp[i][0] -= dp[i - 2][0];
        } else {
          dp[i][j] = dp[i - 1][j];
          dp[i][j] += dp[i][j - 1];
          dp[i][j] -= dp[i - 1][0];
        }
      }
    }

    // printf("========");
    // for(int i=30; i<31; i++) {
    //   for(int j=0; j<10; j++) {
    //     dp[i][j].print(" ");
    //   }
    //   printf("\n");
    // }

    everCalcEncode = 1;
  }

  BigInteger num[2];
  num[0] = 0;
  num[1] = 0;
  const int K = 4;
  for (int i = 0, p = 0; i < N; i++, p++) {
    BigInteger tmp(M[i] >> (8 - K));
    num[0] += tmp.pow(i * K);
    tmp = (M[i] & ((1 << (8 - K)) - 1));
    num[1] += tmp.pow(i * (8 - K));
  }

  // printf("0 : "); num[0].print("\n");
  // printf("1 : "); num[1].print("\n");

  for (int i = 0; i < 2; i++) {
    int r, c;
    r = 0, c = 0;

    while (dp[r][c] <= num[i]) {
      if (++c >= N_MAX) {
        c = 0;
        ++r;
      }
    }
    if (--c < 0) {
      c = N_MAX - 1;
      --r;
    }

    num[i] -= dp[r][c];
    send(c | (i << 7));

    for (--r; r >= 0; --r) {
      num[i] += dp[r][0];
      while (num[i] < dp[r][c])
        c--;

      send(c | (i << 7));
      num[i] -= dp[r][c];
    }
  }
}

//------------------- DO NOT REMOVE NOR EDIT THESE 3 LINES -----------------
//------------------- above is encode, below is decode ---------------------
//--------------------cut here----------------------------------------------

#include <algorithm>
using namespace std;

int everCalcDecode = 0;

void decode(int N, int L, int X[]) {

  struct BigInteger {
    const int LENGTH = 20;
    const long long NUM_SIZE = 10000000000000000LL;

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

    BigInteger &operator+=(const BigInteger &other) {

      long long overflow = 0;
      int mhid = hid > other.hid ? hid : other.hid;
      for (int i = 0; i < mhid; i++) {
        long long sum = overflow;
        if (i < hid)
          sum += n[i];
        if (i < other.hid)
          sum += other.n[i];
        overflow = sum / NUM_SIZE;
        n[i] = sum % NUM_SIZE;
      }

      if (overflow > 0) {
        n[mhid++] = overflow;
      }
      hid = mhid;

      return *this;
    }

    BigInteger &operator-=(const BigInteger &other) {

      for (int i = 0; i < hid; i++) {
        n[i] -= other.n[i];
        if (n[i] < 0) {
          n[i] += NUM_SIZE;
          n[i + 1]--;
        }
      }

      while (n[hid - 1] == 0 && hid > 1) {
        hid--;
      }

      return *this;
    }

    BigInteger pow(int p) {
      BigInteger k(n[0]);
      BigInteger biCache(0);
      for (int i = 0; i < p; i++) {
        biCache = k;
        biCache += k;
        k = biCache;
      }
      return k;
    }

    void shift(int k) {

      int mod = 1 << k;
      int left = 0;

      for (int i = hid - 1; i >= 0; i--) {
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
          printf(" %017lld", n[i]);
      }
      printf(":%d", hid);
      printf(end);
    }
  };

  BigInteger dp[512][128];

  const int N_MAX = 128;

  if (!everCalcDecode) {

    BigInteger biOne(1);

    for (int i = 0; i < N_MAX * 3; i++) {
      for (int j = 0; j < N_MAX; j++) {
        dp[i][j] = 0;
      }
    }

    for (int i = 0; i < N_MAX * 3; i++) {
      for (int j = 0; j < N_MAX; j++) {
        if (i == 0) {
          dp[i][j] = j;
        } else if (i == 1 && j == 0) {
          dp[i][0] = dp[i - 1][N_MAX - 1];
          dp[i][0] += biOne;
        } else if (j == 0) {
          dp[i][0] = dp[i - 1][0];
          dp[i][0] += dp[i - 1][N_MAX - 1];
          dp[i][0] -= dp[i - 2][0];
        } else {
          dp[i][j] = dp[i - 1][j];
          dp[i][j] += dp[i][j - 1];
          dp[i][j] -= dp[i - 1][0];
        }
      }
    }

    // printf("========");
    // for(int i=30; i<31; i++) {
    //   for(int j=0; j<10; j++) {
    //     dp[i][j].print(" ");
    //   }
    //   printf("\n");
    // }

    everCalcDecode = 1;
  }

  int D[2][1000];
  int DL[2] = {0};

  for (int i = 0; i < L; i++) {
    int d = X[i] >> 7;
    D[d][DL[d]++] = X[i] & 0b01111111;
  }

  sort(D[0], D[0] + DL[0]);
  reverse(D[0], D[0] + DL[0]);
  sort(D[1], D[1] + DL[1]);
  reverse(D[1], D[1] + DL[1]);

  BigInteger num[2];

  num[0] = 0;
  num[1] = 0;

  for (int d = 0; d < 2; d++) {
    int r = DL[d] - 1;
    for (int i = 0; i < DL[d]; i++, r--) {
      if (i == 0) {
        num[d] += dp[r][D[d][i]];
      } else {
        num[d] += dp[r][D[d][i]];
        num[d] -= dp[r][0];
      }
    }
  }

  // num[0].print("\n");
  // num[1].print("\n");

  int message[64] = {0};

  const int K = 4;
  for(int d=0; d<2; d++) {
    for (int i = 0; i < N; i++) {
      if(d == 0) {
        message[i] |= (num[d].n[0] & ((1 << K) - 1)) << (8 - K);
      } else {
        message[i] |= (num[d].n[0] & ((1 << (8 - K)) - 1));
      }
      num[d].shift(d == 0 ? K : 8 - K);
    }
  }

  for(int i=0; i<N; i++) {
    output(message[i]);
  }
}
