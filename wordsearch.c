#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void printSearchPath(int** path);
char* toUppercase(const char* word);
int returnDigits(int x);
void clearSearchPath(int** searchPath);
int wasPrevSearch(int *visited, int row, int col);
void clearPreviouslySearched(int *visited);
int dfs(char** board, char* word, int* visited, int wordIndex, int row, int col, int** searchPath, int* found);
void searchPuzzle(char** arr, char* word);
int bSize;

// Main function, DO NOT MODIFY 	
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);
    
    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));

    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) {
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j) {
            fscanf(fptr, "%c ", *(block + i) + j);            
        }
        fscanf(fptr, "%c \n", *(block + i) + j);
    }
    fclose(fptr);

    printf("Enter the word to search: ");
    scanf("%s", word);
    
    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);
    
    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word);
    
    return 0;
}

void printPuzzle(char** arr) {
	// This function will print out the complete puzzle grid (arr). 
    // It must produce the output in the SAME format as the samples 
    // in the instructions.
    // Your implementation here...
    for(int i=0; i<bSize; i++) {
		for(int j=0; j<bSize; j++) {
			printf("%c ", *(*(arr + i) + j)); 
		}
		printf("\n");
	}
	printf("\n");
}

void printSearchPath(int** path) {
    for(int i=0; i<bSize; i++) {
        for(int j=0; j<bSize; j++) {
            printf("%d\t", *(*(path+i)+j));
        }
        printf("\n");
    }
}

char* toUppercase(const char* word) {
    char* rString = (char*)malloc(strlen(word) + 1); // Allocate memory for return string
    strcpy(rString, word); // Copy over word 
    for(int i = 0; rString[i] != '\0'; i++) {
        if (rString[i] >= 'a' && rString[i] <= 'z') {
            // Convert lowercase to uppercase by subtracting ASCII difference
            rString[i] -= ('a' - 'A');
        }
    }
    return rString;
}

int returnDigits(int x) {
    int count = 0;
    while(x > 0) {
        x /= 10;
        count ++;
    }
    return count;
}

void clearSearchPath(int** searchPath) {
    for (int i = 0; i < bSize; ++i) {
        for (int j = 0; j < bSize; ++j) {
            *(*(searchPath + i) + j) = 0;  // Set all elements to 0
        }
    }
}

int wasPrevSearch(int *visited, int row, int col) {
    if(*(visited) == row && *(visited + 1) == col) {
        return 1;
    }
    return 0;
}

int dfs(char** board, char* word, int* visited, int wordIndex, int row, int col, int** searchPath, int* found) {
    if (*found) {
        return 1; // Return 1 if a correct path is already found
    }
    if (wordIndex == strlen(word)) {
        *found = 1; // Set found to 1 to indicate a correct path is found
        return 1;
    }
    if (row < 0 || col < 0 || row >= bSize || col >= bSize ||
        *(*(board + row) + col) != *(word + wordIndex) ||
        wasPrevSearch(visited, row, col)) {
        return 0; // Return 0 to indicate an incorrect path
    }

    //update previously searched
    *(visited) = row;
    *(visited+1) = col;

    // Recursively explore all possible directions
    int result = (
        dfs(board, word, visited, wordIndex + 1, row, col - 1, searchPath, found) ||    // Check Left
        dfs(board, word, visited, wordIndex + 1, row, col + 1, searchPath, found) ||    // Check right
        dfs(board, word, visited, wordIndex + 1, row - 1, col, searchPath, found) ||    // Check up
        dfs(board, word, visited, wordIndex + 1, row + 1, col, searchPath, found) ||    // Check down
        dfs(board, word, visited, wordIndex + 1, row - 1, col - 1, searchPath, found) ||  // Check diag-up-left
        dfs(board, word, visited, wordIndex + 1, row - 1, col + 1, searchPath, found) ||  // Check diag-up-right
        dfs(board, word, visited, wordIndex + 1, row + 1, col - 1, searchPath, found) ||  // Check diag-down-left
        dfs(board, word, visited, wordIndex + 1, row + 1, col + 1, searchPath, found)     // Check diag-down-right
    );

    // If the current position is on the correct path, update the searchPath
    if (result) {
       *(*(searchPath + row) + col) = (*(*(searchPath + row) + col) * 10) + (wordIndex + 1);
    }

    return result;
}

void searchPuzzle(char** arr, char* word) {
    int found = 0; // create flag for if correct path is found
    int* prevVisit = (int*)malloc(2 * sizeof(int));
    // Set arbitrary position so first no collision on first check
    *(prevVisit) = -1;
    *(prevVisit + 1) = -1; 

    int** searchPath = (int**)malloc(bSize * sizeof(int*)); // Allocate space for search path
    for (int i = 0; i < bSize; ++i) {
        *(searchPath + i) = (int*)malloc(bSize * sizeof(int));
        for (int j = 0; j < bSize; ++j) {
            *(*(searchPath + i) + j) = 0; // Set each element to 0
        }
    }

    for (int i = 0; i < bSize && !found; i++) {
        for (int j = 0; j < bSize && !found; j++) {
            if (dfs(arr, toUppercase(word), prevVisit, 0, i, j, searchPath, &found)) { // Call dfs on each Char on Board
                found = 1;
            }
            if (found == 0) {
                *(prevVisit) = -1;
                *(prevVisit + 1) = -1;
                clearSearchPath(searchPath); // Clear previously searched before next search
            }
        }
    }
    free(prevVisit); // Free dynamically allocated memory

    if (found) {
        printf("Word found!\n");
        printf("Printing the search path:\n");
        printSearchPath(searchPath);
    } else {
        printf("Word not found!\n");
    }
    // Free dynamically allocated memory for searchPath
    for (int i = 0; i < bSize; ++i) {
        free(*(searchPath + i));
    }
    free(searchPath);
}