#include "student.h"
#include "user_lib.h"
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_N 1000
#define MAX_L 10000

int channel_range = 65535;
int max_expansion = 15;

static int message[MAX_N];
static int N;
static int encoded_message[MAX_L];
static int L;
static int output_message[MAX_N];
static int O;

inline void my_assert(int e) {
  if (!e)
    abort();
};

void send(int a) {
  if (L == MAX_L) {
    printf("Encoded message too long.\n");
    exit(0);
  }
  encoded_message[L] = a;
  L++;
}

void output(int b) {
  if (O == N)
    O++;
  if (O > N)
    return;
  output_message[O] = b;
  O++;
}

static void sort_message(int d) {
  int i, j, b, bi, t;
  for (i = 0; i < L - 1; i++) {
    bi = i;
    b = encoded_message[i];
    for (j = i + 1; j < L; j++)
      if (((d == 0) && (encoded_message[j] < b)) ||
          ((d == 1) && (encoded_message[j] > b))) {
        b = encoded_message[j];
        bi = j;
      }
    t = encoded_message[i];
    encoded_message[i] = encoded_message[bi];
    encoded_message[bi] = t;
  }
}

static void random_shuffle() {
  int i, t, p;
  for (i = 0; i < L - 1; i++) {
    p = rand() % (L - i);
    t = encoded_message[i];
    encoded_message[i] = encoded_message[i + p];
    encoded_message[i + p] = t;
  }
}

static void shuffle(int method) {
  if (method == 0)
    sort_message(0);
  else if (method == 1)
    sort_message(1);
  else
    random_shuffle();
}

static void check_encoded_message() {
  int i;
  if (L > max_expansion * N) {
    printf("Encoded message too long.\n");
    exit(0);
  }
  for (i = 0; i < L; i++)
    if ((encoded_message[i] < 0) || (encoded_message[i] > channel_range)) {
      printf("Bad encoded integer.\n");
      exit(0);
    }
}

static int check_output() {
  int i;

  if (O != N)
    return 0;
  for (i = 0; i < N; i++)
    if (message[i] != output_message[i])
      return 0;
  return 1;
}

void count_bird(int isReport) {

  int i, tt, t, p, r;

  int count = 0;
  int original = 0;
  int pass = true;
  clock_t timeUse;

  timeUse = clock();

  scanf("%d", &tt);
  scanf("%d %d", &max_expansion, &channel_range);

  scanf("%d", &r);
  srand(r);

  for (t = 0; t < tt; t++) {
    scanf("%d", &N);
    for (i = 0; i < N; i++)
      scanf("%d", &message[i]);

    original += N;

    L = 0;
    encode(N, message);

    count += L;

    check_encoded_message();

    scanf("%d", &p);
    shuffle(p);

    O = 0;
    decode(N, L, encoded_message);

    if (!check_output()) {
      pass = false;
    }
  }

  timeUse = clock() - timeUse;

  if(pass) {
    if (isReport) {
      printf("[uses %d parrots]", count);
    } else {
      printf("Correct. ");
      printf("Bird use : %d / %d (%.3f) [%f s]\n", count, original,
             count / (float)original, ((float)timeUse) / CLOCKS_PER_SEC);
    }
  } else {
    if (isReport) {
      printf("-");
    } else {
      printf("Incorrect [%f s]\n", ((float)timeUse) / CLOCKS_PER_SEC);
    }
  }


}

int main() {

  const char *countBird = std::getenv("COUNT_BIRD");
  const char *verbose = std::getenv("VERBOSE");
  const char *report = std::getenv("REPORT");

  if (countBird != NULL || report != NULL) {
    count_bird(report != NULL);
    return 0;
  }

  int i, t;
  double ratio = 0;

  // read input
  printf("Enter message size: ");
  my_assert(1 == scanf("%d", &N));
  printf("Enter message: ");
  for (i = 0; i < N; i++)
    my_assert(1 == scanf("%d", &message[i]));

  printf("\n");

  // let the user do the encoding
  L = 0;
  encode(N, message);

  if (verbose != NULL) {
    printf("Message : [ ");
    for (int i = 0; i < N; i++)
      printf("%d ", message[i]);
    printf("]\n");
  }

  // calculate ratio of message size
  if ((double)L / N > ratio) {
    ratio = ((double)L) / N;
  }

  // test the decoding by different shuffle 5 times, the first one is nut
  // shuffled
  for (t = 0; t < 5; t++) {

    check_encoded_message();

    if (t > 0)
      random_shuffle();

    // NN = 0;
    O = 0;
    decode(N, L, encoded_message);

    if (!check_output()) {
      printf("--> run %d : Incorrect.\n", t);
      printf("Size -> Result : %d, Expect : %d\n", O, N);
    } else {
      printf("--> run %d : Correct.\n", t);
    }

    if (verbose != NULL) {
      printf("Output  : [ ");
      for (int i = 0; i < N; i++)
        printf("%d ", output_message[i]);
      printf("]\n");
    }

    printf("\n");
  }
  printf("Input size: %d\nMessage size: %d\nRatio = %3.3f\n\n", N, L, ratio);

  return 0;
}
