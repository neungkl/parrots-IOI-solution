#include "student.h"
#include <cstdio>

typedef long long llint;

void encode(int N, int M[]) {

  const int N_MAX = 16;
  long long dp[N_MAX * 2][N_MAX];

  for (int i = 0; i < N_MAX * 2; i++) {
    for (int j = 0; j < N_MAX; j++) {
      if (i == 0) {
        dp[i][j] = j;
      } else if (i == 1 && j == 0) {
        dp[i][0] = 1 + dp[i - 1][N_MAX - 1];
      } else if (j == 0) {
        dp[i][0] = (dp[i - 1][0] - dp[i - 2][0]) + dp[i - 1][N_MAX - 1];
      } else {
        dp[i][j] = dp[i][j - 1] + (dp[i - 1][j] - dp[i - 1][0]);
      }
    }
  }

  llint num = 0;
  for (int i = 0, p = 0; i < N; i += 4, p++) {
    llint n1 = i < N ? M[i] : 0;
    llint n2 = i + 1 < N ? M[i + 1] : 0;
    llint n3 = i + 2 < N ? M[i + 2] : 0;
    llint n4 = i + 3 < N ? M[i + 3] : 0;
    num = n1 | (n2 << 8) | (n3 << 16) | (n4 << 24);

    //printf("==> endcode %lld\n",num);

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
    // printf("%d\n", c);
    send((p << 4) | c);
    for (--r; r >= 0; --r) {
      num += dp[r][0];
      while (dp[r][c] > num)
        c--;
      // printf("%d\n", c);
      send((p << 4) | c);
      num -= dp[r][c];
    }
  }
}

//------------------- DO NOT REMOVE NOR EDIT THESE 3 LINES -----------------
//------------------- above is encode, below is decode ---------------------
//--------------------cut here----------------------------------------------

#include <vector>
#include <algorithm>
using namespace std;

void decode(int N, int L, int X[]) {

  const int N_MAX = 16;
  long long dp[N_MAX * 2][N_MAX];

  for (int i = 0; i < N_MAX * 2; i++) {
    for (int j = 0; j < N_MAX; j++) {
      if (i == 0) {
        dp[i][j] = j;
      } else if (i == 1 && j == 0) {
        dp[i][0] = 1 + dp[i - 1][N_MAX - 1];
      } else if (j == 0) {
        dp[i][0] = (dp[i - 1][0] - dp[i - 2][0]) + dp[i - 1][N_MAX - 1];
      } else {
        dp[i][j] = dp[i][j - 1] + (dp[i - 1][j] - dp[i - 1][0]);
      }
    }
  }

  vector<int> box[16];

  for (int i = 0; i < L; i++) {
    box[(X[i] >> 4) & 0xf].push_back(X[i] & 0xf);
  }

  for (int i = 0, p = 0; i < N; i += 4, p++) {
    sort(box[p].begin(), box[p].end());
    reverse(box[p].begin(), box[p].end());

    long long num = 0;
    int r = box[p].size() - 1;
    for (int i = 0; i < box[p].size(); i++, r--) {
      if (i == 0) {
        num += dp[r][box[p][i]];
      } else {
        num += dp[r][box[p][i]] - dp[r][0];
      }
    }

    //printf("==> decode %lld\n", num);

    if (i < N)
      output(num & 0xff);
    if (i + 1 < N)
      output((num >> 8) & 0xff);
    if (i + 2 < N)
      output((num >> 16) & 0xff);
    if (i + 3 < N)
      output((num >> 24) & 0xff);
  }
}
