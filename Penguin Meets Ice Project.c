#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int size = 8;

int penguinRow;
int penguinCol;
int dotRow;
int dotCol;

void initializeGame();
void displayBoard();
void movePenguin(char direction);
int isDotReached();
int min(int a, int b);

int main() {
  char direction;

  initializeGame();

  while (!isDotReached()) {
    displayBoard();

    printf(
        "Enter direction (N North, W West, S South, E East): ");
    scanf(" %c", &direction);

    movePenguin(direction);
  }

  displayBoard();
  printf("Your Penguin has reached the ice!\n");

  return 0;
}

void initializeGame() {
  srand(time(NULL));

  penguinRow = rand() % size;
  penguinCol = rand() % size;

  dotRow = rand() % size;
  dotCol = rand() % size;
}

void displayBoard() {
  int i, j;
  printf("\n  ");
  for (i = 0; i < size; i++) {
    printf("%d ", i + 1);
  }
  printf("\n");
  for (i = 0; i < size; i++) {
    printf("%d ", i + 1);
    for (j = 0; j < size; j++) {
      if (i == penguinRow && j == penguinCol) {
        printf("P ");
      } else if (i == dotRow && j == dotCol) {
        printf("* ");
      } else {
        printf(". ");
      }
    }
    printf("\n");
  }
}

void movePenguin(char direction) {
  switch (direction) {
  case 'N':
    if (penguinRow > 0) {
      penguinRow--;
    }
    break;
  case 'S':
    if (penguinRow < size - 1) {
      penguinRow++;
    }
    break;
  case 'E':
    if (penguinCol < size - 1) {
      penguinCol++;
    }
    break;
  case 'W':
    if (penguinCol > 0) {
      penguinCol--;
    }
    break;
  default:
    printf("Wrong direction!\n");
    break;
  }
}

int isDotReached() { return (penguinRow == dotRow && penguinCol == dotCol); }
