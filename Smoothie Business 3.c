/* COP 3502C Assignment 3
This program is written by: Abigail Royce */
//time ./main < in1.txt

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

// struct which contains the information of a store
typedef struct {
  int x; //the x cordinate fo the store
  int y; //the y cordinate of the store 
  char name[21]; //the name of the store 
} Store;

// global variables for array of stores and array of indexes of stores with min distance between them
int minTotalDistance[16]; //array to store the stores with min distance between them
Store stores[16]; //array to store details of stores
double minDistanceSum = DBL_MAX; //variable initialized to DBL_MAX to store minimum total distance between stores

// function prototypes
double actualTotalDistance(int i, int j);
void permute(int n, int step, int paired[16], int currentStores[16], double currentDistanceSum);

int main() {
  int c; //variable to store the number of test cases 
  scanf("%d", &c); //get the test cases from input

  while (c--) { //loop through test cases
    int n; //variable to store n to calculate number of stores
    scanf("%d", &n); //get n from input to caculate stores

    for (int i = 0; i < 2 * n; i++) { //read the x and y cordinates and the name of all the 2n stores
      scanf("%d %d %s", &stores[i].x, &stores[i].y, stores[i].name);
    }

    int used[16] = {0}; //array to store the current stores that are being used
    int currentPairStores[16] = {0}; //array to store the current pairs of stores
    minDistanceSum = DBL_MAX; //initialize minDistanceSum

    permute(n, 0, used, currentPairStores, 0.0); //carry out permutations to find the perfect pair

    printf("%.3lf\n", minDistanceSum); //print the minimum distance found between stores for a particular test case
    for (int i = 0; i < 2 * n; i += 2) { //get indexes of stores with min distance between them
      int from = minTotalDistance[i];
      int to = minTotalDistance[i + 1];
      printf("(%s, %s, %.3lf)\n", stores[from].name, stores[to].name, actualTotalDistance(from, to)); //print names of stores with minimum distance between them and the distance
    }
  }

  return 0;
}

// optimized permute function with early termination
void permute(int n, int step, int paired[16], int currentStores[16], double currentDistanceSum) {
  if (step == 2 * n) { //proceed to next step if pairs have been made
    if (currentDistanceSum < minDistanceSum) { // check if the distance between current stores is less than another 2 stores before
      minDistanceSum = currentDistanceSum; //set the minimum distance 
      memcpy(minTotalDistance, currentStores, sizeof(int) * 2 * n); //save the information of these stores and forget the previous one
    }
    return;
  }

  // Implement early termination based on a threshold
  if (currentDistanceSum >= minDistanceSum) {
    return;  // No need to continue if current path is already worse
  }

  // Continue with permutations
  for (int i = 0; i < 2 * n; i++) {
    if (!paired[i]) {
      paired[i] = 1; //record the store i that is being used currently
      currentStores[step] = i; //record the index of current store i used currently

      for (int j = i + 1; j < 2 * n; j++) { //pair the store i with unused store j
        if (!paired[j]) {
          paired[j] = 1; //record the store j that is being used currently
          currentStores[step + 1] = j; //record the index of current store j used currently

          double distance = actualTotalDistance(i, j); //calculate distance between i and j stores
          permute(n, step + 2, paired, currentStores, currentDistanceSum + distance); //use the new parameters to find permutations for next step

          paired[j] = 0; //change the j after getting all the permutations
        }
      }

      paired[i] = 0; //change the i after getting all the permutations
      break; // Early termination after the first valid pair (like in your first snippet)
    }
  }
}


// function to find the distance between two stores
double actualTotalDistance(int i, int j) {
  int dx = stores[i].x - stores[j].x; //calculate distance between x cordinates of stores
  int dy = stores[i].y - stores[j].y; //fidn the distance between y cordinates of stores
  return sqrt(dx * dx + dy * dy); //return the distance between stores by sqaring the distances between x and y cordinates of stores and adding them and taking their square root
}

