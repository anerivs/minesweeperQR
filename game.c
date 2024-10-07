#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

//fixed sizes for the board size and number of bombs
#define SIZE 10
#define BOMBS 20

char grid[SIZE][SIZE]; //game board
int bomb_loc[SIZE][SIZE]; //bomb placements, 1 represents bomb present
char visible[SIZE][SIZE]; //visible state for each cell
int bomb_counts[SIZE][SIZE]; //bomb counts for each cell
int dig = 0; //tracks the number of digs
int flags = 0; //tracks the number of flags placed

//method to initialize the game board
    //set up grid & visible, bombs are set to 0
void init_game(){
    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            grid[i][j] = '*'; //uncharted land
            visible[i][j] = '*'; //visible board state
            bomb_loc[i][j] = 0; //no bomb
            bomb_counts[i][j] = 0; //initialize bomb count to 0
        }
    }

    //randomly place the bombs
    int placed = 0; //running number of placed bombs
    while(placed < BOMBS){
        int x = rand() % SIZE;
        int y = rand() % SIZE;
        if(bomb_loc[x][y] == 0){
            bomb_loc[x][y] = 1;
            placed++;
        }
    }

    //calculate bomb counts around each cell
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (bomb_loc[i][j] == 0) { //only calculate for non-bomb cells
                int count = 0;
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        int ni = i + di;
                        int nj = j + dj;
                        if (ni >= 0 && ni < SIZE && nj >= 0 && nj < SIZE) {
                            count += bomb_loc[ni][nj];
                        }
                    }
                }
                bomb_counts[i][j] = count; //store bomb count
            }
        }
    }
}

//printing the grid
    //shows bombs, visible state, and the current position of the cursor
void print_grid(int pointX, int pointY){
    for(int i=0; i<SIZE; i++){
        for(int j=0; j<SIZE; j++){
            if (i == pointX && j == pointY) {
                printf("!"); //current cursor location marked with !
            } else if (visible[i][j] == '_') {
                if (bomb_counts[i][j] > 0) {
                    printf("%d", bomb_counts[i][j]); //print bomb count if available
                } else {
                    printf("_"); //empty cell with no surrounding bombs
                }
            } else {
                printf("%c", visible[i][j]); //print current visible state
            }
        }
        printf("\n");
    }

    //print a horizontal separator line below the game
    printf("-----------------------------\n");
}

//method to handle user input
    //processes player movements and actions
//method to handle user input
void handle_input(int * pointX, int* pointY){
    char inp = _getch();

    switch(inp){
        case 'w'://up
            if(*pointX > 0){ //decrease X to move up
                (*pointX)--; 
            }
            break;
       case 'a'://left
            if(*pointY > 0){ //decrease Y to move left
                (*pointY)--;
            }
            break;
        case 's'://down
            if(*pointX < SIZE-1){ //increase X to move down
                (*pointX)++;
            }
            break;
       case 'd'://right
            if(*pointY < SIZE-1){ //increase Y to move right
                (*pointY)++;
            }     
            break;
        case 'D'://dig
            if(dig == 0){
                //reveal the first move and surrounding cells
                visible[*pointX][*pointY] = '_';
                dig++;
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        int ni = *pointX + di;
                        int nj = *pointY + dj;
                        if (ni >= 0 && ni < SIZE && nj >= 0 && nj < SIZE) {
                            visible[ni][nj] = '_'; //reveal surrounding cells
                        }
                    }
                }
            } else {
                if(bomb_loc[*pointX][*pointY] == 1){//if we hit a bomb
                    visible[*pointX][*pointY] = 'X';
                    printf("you lose\n");//player loses & we exit the program
                    exit(0);
                } else { //reveal normal cell
                    visible[*pointX][*pointY] = '_';
                }
            }
            break;
        case 'f'://flag
            if (visible[*pointX][*pointY] == '*') {
                visible[*pointX][*pointY] = '@'; //mark as flagged
                flags++;  
                if(flags == BOMBS){ //check if all bombs are flagged
                    int correct_flags = 1;
                    for (int i = 0; i < SIZE; i++) {
                        for (int j = 0; j < SIZE; j++) {
                            if (visible[i][j] == '@' && bomb_loc[i][j] != 1) {
                                correct_flags = 0; //found an incorrect flag
                            }
                        }
                    }
                    if (correct_flags) {
                        printf("you won\n");
                        exit(0);
                    }
                }
            }
            break;
        case 'E': //need to exit
            exit(0);
            break;
        default: break;
    }        
}


//main method
    //trigger method to handle keypress
    //end state
int main(){
    int pointX=0, pointY=0; //user position
    init_game();
    printf("w=up, a=left, s=down, d=right, D=dig, f=flag, E=exit\n");
    while(1){
        print_grid(pointX, pointY); //print the grid and cursor
        handle_input(&pointX, &pointY); //handle input
    }
    return 0;
}
