/* COP 3502C Programming Assignment 4
This program is written by: Abigail Royce */
#include <stdio.h>
#include <stdlib.h>

int globalXCoordinate; // global variable to store the x coordinate for distance calculations
int globalYCoordinate; // global variable to store the y coordinate for distance calculations

typedef struct { //structure to hold point with x and y coordinates
  int x;  // x coordinate of the point
  int y;  // y coordinate of the point
} point;  

void readData(point **points, point **queries, int *x, int *y, int *n, int *s, int *t); // function to read data
int compareTo(const void *ptrPt1, const void *ptrPt2); // function to compare two points based on their distance from a global point
void mergeSort(point arr[], int l, int r, int t); // function to sort array using merge sort with insertion sort
void merge(point arr[], int l, int m, int r); // function to merge two sorted halves an array
void sortPoints(point arr[], int n, int t); // wrapper function to sort the array using mergeSort
void insertionSort(point arr[], int l, int r); //function to sort elements in order
int binarySearch(point arr[], int l, int r, point x); // function to check for an element

int main() {
  int x; // stores the x-coordinate of the reference point
  int y; // stores the y-coordinate of the reference point
  int n; // stores the number of points to store
  int s; // stores the number of query points
  int t; // stores the threshold value
  point *points, *queries; // pointers to arrays of points and query points
  
  readData(&points, &queries, &x, &y, &n, &s, &t); // read data from input
  globalYCoordinate = y; // set the global y-coordinate
  globalXCoordinate = x; // set the global x-coordinate

  sortPoints(points, n, t); // sort the points array using merge sort with insertion sort for small sections

for (int i = 0; i < n; i++) {
  printf("%d %d\n", points[i].x, points[i].y); // print each point after sorting
}

for (int i = 0; i < s; i++) {
  int index = binarySearch(points, 0, n - 1, queries[i]); // search query points in the sorted array
    if (index != -1) {  
      printf("%d %d found at rank %d\n", queries[i].x, queries[i].y, index + 1); // print the rank if found
    } else {
      printf("%d %d not found\n", queries[i].x, queries[i].y); // print not found
    }
}

  free(points); // free memory allocated for points array
  free(queries); // free memory allocated for queries array

  return 0; 
}

void readData(point **points, point **queries, int *x, int *y, int *n, int *s, int *t) { // function to read data
scanf("%d %d %d %d %d", x, y, n, s, t); // read the x, y coordinates, number of points, queries, threshold

*points = (point *)malloc(*n * sizeof(point)); // allocate memory for points based on number of points
*queries = (point *)malloc(*s * sizeof(point)); // allocate memory for queries based on number of queries

for (int i = 0; i < *n; i++) { 
  scanf("%d %d", &((*points)[i].x), &((*points)[i].y)); // read x and y coordinates for each point and store in points array
}
for (int i = 0; i < *s; i++) { 
  scanf("%d %d", &((*queries)[i].x), &((*queries)[i].y)); // read x and y coordinates for each query and store in queries array
}
}

int compareTo(const void *ptrPt1, const void *ptrPt2) { // function to compare two points based on their distance from a global point
  point *pt1 = (point *)ptrPt1; // cast pointer to point type for the first argument
  point *pt2 = (point *)ptrPt2; // cast pointer to point type for the second argument

  int dist1 = (pt1->x - globalXCoordinate) * (pt1->x - globalXCoordinate) + (pt1->y - globalYCoordinate) * (pt1->y - globalYCoordinate); // calculate the distance of pt1 from the global coordinates


  int dist2 = (pt2->x - globalXCoordinate) * (pt2->x - globalXCoordinate) + (pt2->y - globalYCoordinate) * (pt2->y - globalYCoordinate); // calculate the distance of pt2 from the global coordinates

  if (dist1 < dist2) {
    return -1; // return -1 if pt1 is closer to the global point than pt2
  }
  if (dist1 > dist2) {
    return 1; // return 1 if pt1 is farther from the global point than pt2
  }

  // for same distances, compare the x coordinates of pt1 and pt2
  if (pt1->x < pt2->x) {
    return -1; // return -1 if pt1's x coordinate is less than pt2's
  }
  if (pt1->x > pt2->x) {
    return 1; // return 1 if pt1's x coordinate is greater than pt2's
  }

  // for same x coordinates, compare the y coordinates of pt1 and pt2
  if (pt1->y < pt2->y) {
    return -1; // return -1 if pt1's y coordinate is less than pt2's
  }
  if (pt1->y > pt2->y) {
    return 1; // return 1 if pt1's y coordinate is greater than pt2's
  }

  // if both coordinates are the same, return 0 
  return 0;
}

void mergeSort(point arr[], int l, int r, int t) { // function to sort array using merge sort with insertion sort
if (l < r) {  // check if the starting index is less than the ending index
  if ((r - l + 1) <= t) {  // if the number of elements is less than or equal to the threshold
    insertionSort(arr, l, r);  // use insertion sort for small sections of the array
  } else {  
    int m = l + (r - l) / 2;  // calculate the middle 
    mergeSort(arr, l, m, t);  // sort the left half of the array
    mergeSort(arr, m + 1, r, t);  // sort the right half of the array
    merge(arr, l, m, r);  // merge the two sorted halves
  }
}
}

void merge(point arr[], int l, int m, int r) { // function to merge two sorted halves an array
int n1 = m - l + 1; // elements in the left half
int n2 = r - m; // elements in the right half

point *L = (point *)malloc(n1 * sizeof(point)); // allocate memory for the left half
point *R = (point *)malloc(n2 * sizeof(point)); // allocate memory for the right half

for (int i = 0; i < n1; i++) {
  L[i] = arr[l + i]; // copy data to left half
}
for (int j = 0; j < n2; j++) {
  R[j] = arr[m + 1 + j]; // copy data to right half
}

int i = 0, j = 0, k = l; // pointers for left, right, and merged arrays
  while (i < n1 && j < n2) { // while there are elements in both halves
    if (compareTo(&L[i], &R[j]) <= 0) { // if element in left half is less or equal
      arr[k] = L[i]; // copy element from left half final array
      i++; // 
    } else {
      arr[k] = R[j]; // copy element from right half to merged array
      j++; 
    }
      k++; // move to next position in merged array
    }

  while (i < n1) { // copy remaining elements from left half to final array
    arr[k] = L[i]; 
    i++; 
    k++; 
  }

  while (j < n2) { // copy remaining elements from right half to final array
    arr[k] = R[j]; 
    j++; 
    k++; 
  }

  free(L); // free memory allocated for left side
  free(R); // free memory allocated for right sied
}

void sortPoints(point arr[], int n, int t) { // wrapper function to sort the array using mergeSort
  mergeSort(arr, 0, n - 1, t);  // call mergeSort to sort the array from index 0 to n-1 with threshold t
}

void insertionSort(point arr[], int l, int r) { //function to sort elements in order
for (int i = l + 1; i <= r; i++) {  // loop from the second to the last element 
  point imp = arr[i];  // store the current element to be inserted
  int j = i - 1;  // set j before the current element
  while (j >= l && compareTo(&arr[j], &imp) > 0) {  // move elements that are greater than imp one position ahead
    arr[j + 1] = arr[j];  // shift element to the right
    j--;  
  }
  arr[j + 1] = imp;  // place element into the correct place
}
}

int binarySearch(point arr[], int l, int r, point x) {  // function to check for an element
while (l <= r) {  // continue searching while the left is less than or equal to the right index
  int m = l + (r - l) / 2;  // find the middle index
  int compare = compareTo(&arr[m], &x);  // compare the middle element with the element x

  if (compare == 0) return m;  // if the middle element is equal to x, return its index
    if (compare < 0) l = m + 1;  // if the middle element is less than x, search in the right side
      else r = m - 1;  // if the middle element is greater than x, search in the left side
}
  return -1;  // if x is not there, then return -1 to indicate it is not there
}
