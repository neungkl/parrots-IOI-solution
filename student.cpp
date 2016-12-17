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

  BigInteger dp[512][256];

  const int N_MAX = 256;

  BigInteger biOne(1);

  if (!everCalcEncode) {

    for (int i = 0; i < N_MAX * 3 / 2; i++) {
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

    everCalcEncode = 1;
  }

  int rangeMin = 256;
  int rangeMax = 0;

  for (int i = 0; i < N; i++) {
    if (M[i] < rangeMin)
      rangeMin = M[i];
    if (M[i] > rangeMax)
      rangeMax = M[i];
  }

  if (rangeMin == 0 && rangeMax == 0) {
    return;
  }
  if(rangeMin == 255 && rangeMax == 255) {
    send(0);
    return ;
  }
  if (rangeMin >= 0 && rangeMax <= 1 && N == 8) {
    int data = 0;
    for (int i = 0; i < N; i++) {
      data |= ((M[i] & 1) << i);
    }
    send(data);
    return;
  }

  int input[64];

  for(int i=0; i<N; i++) {
    input[i] = M[i];
  }

  if(N%2 == 0) {
    for(int i=N-1,j=0; i>=0; i--,j++) {
      input[j] = M[i];
    }
  }

  BigInteger num(0);
  for (int i = 0, p = 0; i < N; i++, p++) {
    BigInteger num2(input[i]);
    num += num2.pow(i * 8);
  }

  // num.print("\n");

  int K = 0;

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

  num -= dp[r][c];
  send(c);
  K++;
  for (--r; r >= 0; --r) {
    num += dp[r][0];
    while (num < dp[r][c])
      c--;

    send(c);
    K++;
    num -= dp[r][c];
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

  BigInteger dp[512][256];

  const int N_MAX = 256;

  BigInteger biOne(1);

  if (!everCalcDecode) {

    for (int i = 0; i < N_MAX * 3 / 2; i++) {
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

    everCalcDecode = 1;
  }

  if (L == 0) {
    for (int i = 0; i < N; i++) {
      output(0);
    }
    return;
  } else if (L == 1 && X[0] == 0) {
    for (int i = 0; i < N; i++) {
      output(255);
    }
    return ;
  } else if (L == 1 && N == 8) {
    for (int i = 0; i < 8; i++) {
      output(X[0] & 1);
      X[0] >>= 1;
    }
    return;
  }

  sort(X, X + L);
  reverse(X, X + L);

  BigInteger num(0);

  if (L != 0) {
    int r = L - 1;
    for (int i = 0; i < L; i++, r--) {
      if (i == 0) {
        num += dp[r][X[i]];
      } else {
        num += dp[r][X[i]];
        num -= dp[r][0];
      }
    }
  }

  BigInteger num2 = num;

  int message[64];

  for(int i=0; i<N; i++) {
    message[i] = num2.n[0] & 0xff;
    num2.shift(8);
  }

  if(N%2 == 0) {
    for(int i=N-1; i>=0; i--) {
      output(message[i]);
    }
  } else {
    for(int i=0; i<N; i++) {
      output(message[i]);
    }
  }
}
