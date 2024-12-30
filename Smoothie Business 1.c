/* COP 3502C Assignment 1
This program is written by: Abigail Royce */
#include <stdlib.h>
#include <stdio.h>

typedef struct item { //stores one component of a smoothie recipe
    int portions; //stores the volume of an ingredient
    int itemNo; // stores the ingredient numbe
} item;

typedef struct recipe { //stores the variables needed for one smoothie 
    int totalPortions; //stores the sum of portions of each ingredient 
    int itemCount; //stores the number of different ingredients 
    item* itemList; //dynamically allocated with each ingredient from a smoothie
} recipe;

//Function protoypes 
recipe** readAllRecipes(int *numRecipes); //fucntion to read recipes
double* calculateOrder(int ingredientCount, int numSmoothies, recipe** smoothieList); //function to calculate the order 
recipe* readRecipe(); // fucntion to read one recipe
void printOrder(char** ingredientNames, double* amtOfEachItem, int numIngredients); //function to print the contents in order
char** readIngredients(int *numIngredients); //function reads ingredients from the input
void freeIngredients(char** ingredientList, int numIngredients); //fucntion to free ingredients stored
void freeRecipes(recipe** allRecipes, int numRecipes); //function to free recipes 

// Main function
int main() {
int numRecipes, numStore, numIngredients; //declares variable for recipes, stores and ingredient information
char** ingredientNames = readIngredients(&numIngredients); //calls the function to reads ingredients from input
recipe** smoothieList = readAllRecipes(&numRecipes); //calls function that reads recipes
scanf("%d", &numStore);
for (int i = 0; i < numStore; i++) { //to access each store
    printf("List of items for store %d:\n", i + 1);
    int numSmoothies; //declare variable for smoothies for current store
    scanf("%d", &numSmoothies);
    double* amtOfEachItem = calculateOrder(numIngredients, numSmoothies, smoothieList); //call function to calculate order of store
    printOrder(ingredientNames, amtOfEachItem, numIngredients); //call function to print details of store
    free(amtOfEachItem); //free dynamically allocated memory for each ingredient
    printf("\n"); //print line after each store
}
freeIngredients(ingredientNames, numIngredients); //frees ingredients
freeRecipes(smoothieList, numRecipes); //frees the recipes
return 0;
}

//Fucntion to read the recipes
recipe** readAllRecipes(int *numRecipes) { 
scanf("%d", numRecipes); //reads recipes from input
recipe** smoothieList = (recipe**)malloc(*numRecipes * sizeof(recipe*)); //dynamically allocates memory for smoothielist
    for (int i = 0; i < *numRecipes; i++) {
        smoothieList[i] = readRecipe(); //stores recipes in smoothielist
    }
return smoothieList;
}

//Function to calculate the order of each store
double* calculateOrder(int ingredientCount, int numSmoothies, recipe** smoothieList) { 
double* amtOfEachItem = (double*)calloc(ingredientCount, sizeof(double)); //dynamically allocates memory for each item
for (int i = 0; i < numSmoothies; i++) { //loops through the smoothies
    int indexSmoothie, weightSold; //declares variables to store the index of the smoothie and the amount sold
    scanf("%d %d", &indexSmoothie, &weightSold);
    recipe* rec = smoothieList[indexSmoothie]; //gets recipe of smoothie
    for (int j = 0; j < rec->itemCount; j++) { //loops through each of the items
        int ingredientIndex = rec->itemList[j].itemNo; //gets position of ingredient
        amtOfEachItem[ingredientIndex] += weightSold*((double)rec->itemList[j].portions / rec->totalPortions);
    }
}
return amtOfEachItem;
}

//Fucntion to read one recipe
recipe* readRecipe() {
recipe* newRecipe = (recipe*)malloc(sizeof(recipe)); //dynamically allocates memory for a new smoothie
scanf("%d", &(newRecipe->itemCount));
newRecipe->itemList = (item*)malloc(newRecipe->itemCount * sizeof(item)); //dynamically allocates memory for the ingredients in that smoothie
newRecipe->totalPortions = 0;
for (int i = 0; i < newRecipe->itemCount; i++) {
    scanf("%d %d", &(newRecipe->itemList[i].itemNo), &(newRecipe->itemList[i].portions));
    newRecipe->totalPortions += newRecipe->itemList[i].portions; //updates the portion of that recipe
}
return newRecipe;
}

//Function to print the contents in order
void printOrder(char** ingredientNames, double* amtOfEachItem, int numIngredients) {
for (int i = 0; i < numIngredients; i++) { //loops through ingredients
    if (amtOfEachItem[i] > 0) {
        printf("%s %.6f\n", ingredientNames[i], amtOfEachItem[i]);
    }
}
}

//Function that reads ingredients from the input
char** readIngredients(int *numIngredients) {
scanf("%d", numIngredients); //reads ingredients from input
char **ingredients = (char **)malloc(*numIngredients * sizeof(char *)); //dynamically allocates memory for ingredients
for (int i = 0; i < *numIngredients; i++) {
    ingredients[i] = (char *)malloc(21 * sizeof(char)); //dynamically allocates memory for name
    scanf("%s", ingredients[i]);
}
return ingredients;
}

//Fucntion to free the ingredients stored
void freeIngredients(char** ingredientList, int numIngredients) {
for (int i = 0; i < numIngredients; i++) {
    free(ingredientList[i]); //frees memory dynamically allocated for each ingredient
}
free(ingredientList);
}

//Function to free the recipes created
void freeRecipes(recipe** allRecipes, int numRecipes) {
for (int i = 0; i < numRecipes; i++) {
    free(allRecipes[i]->itemList); //frees memory dynamically allocated for items in smoothies
    free(allRecipes[i]); //frees memory dynamically allocated for all recipes 
}
free(allRecipes); //frees memory dynamically allocated for recipe pointer
}
