///FileName: main.c
///Author: Trevor Powers
///
///This is a c version the cocs project. The goal is to outline logic, and to
///later emulate this logic in mips
///
///In this file Input is handled, once input has ended program flow is passed to
///game.c, which will handle game logic. After the game has ended program flow
///will come back to this file, which will handle exit procedures
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>


/**
*This function prints the banner for the start of the simulation
**/
void printBanner(){
      printf("*****************************************\n");
      printf("**     Cellular Automata Simulator     **\n");
      printf("*****************************************\n");
      printf("\n");

}



/**
*This function takes a decimal number and converts it to binary rep
**/
char *convert2(int a, char *buffer, int buf_size){
      buffer += (buf_size -1);

      for(int i = 7; i >= 0; i--){
            *buffer-- = (a & 1) + '0';

            a >>= 1;
      }
      return buffer;
}



//This prints the border for the simulation
void printBorder(int row){
      int curr, counter;
      curr = 1;
      counter = 1;

      printf("   ");    //3 space buffer

      while(curr <= row){
            if(counter == 5){
                  printf("+");
                  curr ++;
                  counter ++;
            }
            else if(counter == 10){
                  printf("0");
                  curr ++;
                  counter = 1;
            }
            else{
                  printf("-");
                  curr ++;
                  counter ++;
            }
      }
      printf("\n");

}



//prints the current generation
void printRow(int currgen[], int row, int gen){
      printf(" %d ", gen);

      for(int i = 0; i < row; i++){
            if(currgen[i] == 0){
                  printf(".");
            }
            else{
                  printf("A");
            }
      }

      printf("\n");
}



//This function performs the simulation
//
//@param    current     array that is the current generation
//@param    next        array that will have the next gen put in it
//@param    row         int that is the length of a row
//@param    rules       array that is the rule to compare to
////////////////////////////////////////////////////////////////////////////////
void rowsim(int current[], int next[], int row, char rule[]){

      int bit;    //which bit to look at
      bit = 0;

      for(int i = 0; i < row; i++){
            //I first need to get the number bit to look at in the rule by
            //looking at the left neighbor, the cell itself, and the right
            //neighbor. Edges wrap. ie the left neighbor of 0 is row-1.

            //look at the left
            if(i == 0){
                  bit = current[row-1] * 4;
            }
            else{
                  bit = current[i-1] * 4;
            }

            //look at current
            bit += current[i] * 2;

            //look at right. although the times one is kinda pointless I am
            //leaving it in in order to make the what I am doing clearer
            if(i == (row-1) ){
                  bit += current[0] * 1;
            }
            else{
                  bit += current[i + 1] * 1;
            }

            //write the value for next
            if(rule[7-bit] == '1'){
                  next[i] = 1;
            }
            else{
                  next[i] = 0;
            }
            bit = 0;
      }

      //at this point current[] should hold the correct next generation for the
      //simulation

      //TODO: implement generation counting, it would be done by comparing
      //current to next, and if a cell is alive in both, change its value.

      //will now copy next into current.
      for(int i = 0; i < row; i++){
            current[i] = next[i];
      }
}



//This function handels the simulation itself
//in this function we will first print the border, and then the initial row
//then we will perfrom the simulation, and then print the row again until
//generation == numOfGens, new line prints will always be handeld, inside of the
//print functions.
//
//@param    numOfGens   the number of generations to simulate
//@param    currgen     the array that is the current generation
//@param    rule        binary char array of the rule
//@param    row         row length (needed for output)
//@return               void
////////////////////////////////////////////////////////////////////////////////
void runsim(int numOfGens, int currgen[], char rule[], int row){
      int generation;   //this is the generation counter
      generation = 0;

      int nextgen [row];      //used to place the next gen into, so
                              //a comparison can be made to count how long
                              //a cell has been alive

      //print the initial generation
      printBorder(row);
      printRow(currgen, row, generation);
      generation ++;

      while(generation <= numOfGens){
            rowsim(currgen, nextgen, row, rule);
            printRow(currgen, row, generation);
            generation ++;
      }

      //all generations should have been simulated. Now print out the the bottom
      //border and exit
      printBorder(row);

}



int main(int argc, char const *argv[]) {
      (void) argc;
      (void) argv;
      int rule, gen, row, buffer;
      rule = 0;
      gen = 0;
      row = 0;
      buffer = 9;

      char bin[buffer];
      bin[buffer-1] = '\0';


      //print the banner
      printBanner();

      //scan in rule number, number of generations, row number
      scanf("%d", &rule);
      scanf("%d", &gen);
      scanf("%d", &row);

      //check to see that rule is in the allowed range
      if(rule < 0 || rule > 255){
            fprintf(stderr, "Invalid rule number, cell-auto-sim terminating");
            exit(1);
      }

      //check to see that gen is allowed
      if(gen < 0 || gen > 25){
            fprintf(stderr, "Invalid generation number, cell-auto-sim terminating");
            exit(1);
      }


      //check to see if row value is valid
      if(row < 0 || row > 70){
            fprintf(stderr, "Invalid  row size, cell-auto-sim terminating");
            exit(1);
      }

      //get the binary version of the rule
      convert2(rule, bin, buffer-1);

      //read in the values for the initial start
      int currgen [row];

      int val;
      for(int i = 0; i < row; i++){
            scanf("%d", &val);

            //error checking for row input
            if(val != 1 && val != 0){
                  fprintf(stderr, "Illegal input value, cell-auto-sim terminating\n");
                  exit(1);
            }

            currgen [i] = val;
      }

      //output rule
      printf("Rule %d (%s)\n", rule, bin);
      printf("currgen: ");
      for(int i = 0; i < row; i++){
            printf("%d", currgen[i]);
      }
      printf("\n\n\n");

      runsim(gen, currgen, bin, row);

      exit(0);
}
