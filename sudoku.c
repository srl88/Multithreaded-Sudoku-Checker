#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/**
 *Global variables
 **/
FILE *f;
int const  ROW=9;
int const COLUMN=9;
int const totalNum=81;
int CRITICAL_FLAG=0;

/**
 * Error handeling
 **/
#define SUCCESS 0
#define FAILURE -1

/**
 *Structure that holds the initial parameters where the thread would start
 *checking for the answer (column and row) as well as the board. This also 
 *includes a flag that would prevent thread collision in the critical section
 *while checking for the specific sub-squares
 **/
typedef struct {

  //Initial Row
  int row;
  //Initial Column
  int col;
  //Pointer to the Board
  int **board;
  //flag for critical section
  int flagThread;

} parameterBoard;


/**
 *Check if a rows contains all 9 digits.
 *@param *paramBoard containing pointer to the parameter
 *@return 1 if true otherwise 0
 **/
void * checkRows(void * paramBoard){
  parameterBoard *p = (parameterBoard*) paramBoard;
  int row = p->row;
  int col = p->col;
  int i, j;
  for (i = row; i< ROW; i++){
    int rflag[10]={0};//flag in case a number is added twice.
    for (j=col; j< COLUMN; j++){
      int number = p->board[i][j];
      // printf("Number %d, row: %d, column: %d \n", number, i, j);
      if(rflag[number]!=0){return (void*) 0;}
      else{rflag[number]=1;}
    }
  }
  return (void*)1;
}

/**
 *Checks if each columns has from 1-9 digits
 *@param *paramBoard , pointer to the parameter
 *@retun 1 if true, 0 otherwise
 **/
void * checkColumn(void * paramBoard){
  parameterBoard *p = (parameterBoard*) paramBoard;
  int row = p->row;
  int col = p->col;
  int i, j;
  for (i = col; i< COLUMN; i++){
    int rflag[9]={0};
    for (j=row; j< ROW; j++){
      int number = p->board[j][i];
      if(rflag[number-1]!=0){return (void *)0;}
      else{rflag[number-1]=1;}
    }
  }
  return (void*) 1;
}

/**
 * Checks the sub-quares (3x3) and makes sure they contain all number 1-9
 *@param *paramBoard conating the pointer to the parameterBoard
 *@return 1 if true, 0 otherwise
 */
void * checkSquares(void* paramBoard){

  parameterBoard *check = (parameterBoard*) paramBoard;

  while (CRITICAL_FLAG!= check->flagThread){}

  parameterBoard *p = (parameterBoard*) paramBoard;
  int row = p->row;
  int col = p->col;
  int rflag[9]={0};
  int i, j;
  for (i = row; i< row+3; i++){                                                                                                                                               
    for (j=col; j< col+3; j++){
      int number = p->board[i][j];
      if(rflag[number-1]!=0){CRITICAL_FLAG++; return (void*)0;}
      else{rflag[number-1]=1;}
    }
  }
  CRITICAL_FLAG++;
  return (void*)1;
}

int main (int argc, char** argv){
  // Variables for the input
  int rowIndex=0;
  int columnIndex=0;
  int inputCount=0;
  int **board; // 2D array for ints
  int c; // stores the read character 
  int i , j; // for loop indexs

  //Allocates Memory for input
  board = calloc(ROW, sizeof(int*));  
  for (i=0; i<ROW; i++){
    board[i]=calloc(COLUMN, sizeof(int));
  } 
  
  //Reads file, if not present it outputs stderror message
  f = fopen(argv[1], "r");
  if(f== NULL) {
    perror("Error in opening the file.");
    return(FAILURE);
  }
  
  //Reads each character and does format check
  while (((c = fgetc(f)))!=EOF){
    if ((c>48)&&(c<=57)&&(inputCount<totalNum)){
      int num = c -'0';
      board[rowIndex][columnIndex] = num;
      columnIndex++;
      inputCount++;
      if (columnIndex==9){columnIndex=0; rowIndex++;}
    }
    else if (c>=0&&c<=32){} // ASCII control characters
    else {perror("Error in format"); return (FAILURE);} // if not an integer or a separator it will print error
  }
  //prints the board
  printf("This is the board: \n");
  for (i=0; i<9; i++){
    for(j=0; j<9; j++){
      printf("%d ", board[i][j]);
    }
    printf("\n");
  }

  /**
   *All parameterBoard iniziliaze here. row and columns check for colums the other ones check their corresponding square
   * Critical Flag is set to 0 but changed to another  number for those threads that enter the critical setction.
   */
  parameterBoard *paramRow = (parameterBoard*) malloc(sizeof(parameterBoard));
  paramRow->row =0;
  paramRow->col=0;
  paramRow->board = board;
  paramRow->flagThread = 0; 
  parameterBoard *paramColumn= (parameterBoard*) malloc(sizeof(parameterBoard));
  paramColumn->row =0;
  paramColumn->col=0;
  paramColumn->board = board;
  paramColumn->flagThread = 0;
  parameterBoard *param1= (parameterBoard*) malloc(sizeof(parameterBoard));
  param1->row =0;
  param1->col=0;
  param1->board = board;
  param1->flagThread = 0;
  parameterBoard *param2= (parameterBoard*) malloc(sizeof(parameterBoard));
  param2->row =0;
  param2->col=3;
  param2->board = board;
  param2->flagThread = 1;
  parameterBoard *param3= (parameterBoard*) malloc(sizeof(parameterBoard));
  param3->row =0;
  param3->col=6;
  param3->board = board;
  param3->flagThread = 2;
  parameterBoard *param4= (parameterBoard*) malloc(sizeof(parameterBoard));
  param4->row =3;
  param4->col=0;
  param4->board = board;
  param4->flagThread = 3;
  parameterBoard *param5= (parameterBoard*) malloc(sizeof(parameterBoard));
  param5->row =6;
  param5->col=0;
  param5->board = board;
  param5->flagThread = 4;
  parameterBoard *param6= (parameterBoard*) malloc(sizeof(parameterBoard));
  param6->row =3;
  param6->col=3;
  param6->board = board;
  param6->flagThread = 5;
  parameterBoard *param7= (parameterBoard*) malloc(sizeof(parameterBoard));
  param7->row =6;
  param7->col=6;
  param7->board = board;
  param7->flagThread = 6;
  parameterBoard *param8= (parameterBoard*) malloc(sizeof(parameterBoard));
  param8->row =3;
  param8->col=6;
  param8->board = board;
  param8->flagThread = 7;
  parameterBoard *param9= (parameterBoard*) malloc(sizeof(parameterBoard));
  param9->row =6;
  param9->col=3;
  param9->board = board;
  param9->flagThread = 8;
  
  // here we store the parameterBoard
  parameterBoard *param[]={paramRow, paramColumn, param1, param2, param3, param4, param5, param6, param7, param8, param9};
  // here we store the results for each thread 0 = row, 1= column, 2 = square1..... 11=square9; 
  void *results[11];
  // here we store the threads
  pthread_t threads[11];
  for (i=0;i<11;i++){
    if (i==0){ pthread_create(&threads[i], NULL, checkRows, (void*)param[i]); }
    else if ( i==1) {pthread_create(&threads[i], NULL, checkColumn, (void*)param[i]);}
    else{pthread_create(&threads[i], NULL, checkSquares, (void*)param[i]);}
  }
  for (i=0; i<11; i++){
    pthread_join(threads[i], &results[i]);
  }
  for (i=0; i<11; i++){
    if ((int)results[i]!=1){printf("ERROR\n"); return(FAILURE);}
  }
  printf("Success!!\n");
  return(SUCCESS);
}
