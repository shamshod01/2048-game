#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>


#define SIZE 4
void setBufferedInput(bool);
void getColor(uint8_t value, char *color, size_t length);
void drawBoard(uint8_t board[SIZE][SIZE]);
void initBoard(uint8_t board[SIZE][SIZE]);
void addRandom(uint8_t board[SIZE][SIZE]);
bool slideUp(uint8_t arr[SIZE][SIZE]);
bool slideDown(uint8_t arr[SIZE][SIZE]);
bool slideLeft(uint8_t arr[SIZE][SIZE]);
bool slideRight(uint8_t arr[SIZE][SIZE]);



int main(void) {

  uint8_t board[SIZE][SIZE] = {0,};
  
  srand(time(NULL));
  initBoard(board);
  char k;
  setBufferedInput(false);
  bool slided = false;
  
  while(true) {
    slided = false;
    k = getchar();
  
    switch(k) {
			case 97:	
			case 68:	
       slided = slideLeft(board); break;
			case 100:	
			case 67:	
       slided = slideRight(board); break;
			case 119:	
			case 65:	
       slided = slideUp(board); break;
			case 115:	
			case 66:
       slided = slideDown(board); break;
			default: slided = false;
		}
    if(slided){
    drawBoard(board);
    usleep(100000);
    addRandom(board);
    drawBoard(board);
    }
    
  }
  
 setBufferedInput(true);
 
  return 0;
}



void getColor(uint8_t value, char *color, size_t length) {
	uint8_t bgColors[] = {8,230,223,216,215,203,202,69,39,10,11,12};
  uint8_t fontColors[] = {255,232,232,255,255,255,255,255,0,0,0,0};
  
	snprintf(color,length,"\033[38;5;%d;48;5;%dm",fontColors[value],bgColors[value]);
}


void drawBoard(uint8_t board[SIZE][SIZE]) {
	uint8_t x,y;
	char color[40];
	printf("\033[H"); //clear screen.


	for (y=0;y<SIZE;y++) {
    //header of block
		for (x=0;x<SIZE;x++) {
			getColor(board[y][x],color,40);
			printf("%s",color);
			printf("         "); //size of space is important
		}
		printf("\n");
    //the content of block
		for (x=0;x<SIZE;x++) {
			getColor(board[y][x],color,40);
			printf("%s",color);
			if (board[y][x]!=0) {
        uint8_t t = 9; //widht of the block
        if(board[y][x]<4){ //change number of spaces depend on value
          t = 9-1;
        } 
        else if (board[y][x]<7) {
          t = 9 - 2;
        } else if(board[y][x]<10) {
          t = 9-3;
        } else {
          t = 9 - 4;
        }
		
				printf("%*s%u%*s",t-t/2,"",1<<board[y][x],t/2,"");
			} else {
				printf("    *    ");
			}
		}
		printf("\n");
    //the footer of block
		for (x=0;x<SIZE;x++) {
			getColor(board[y][x],color,50);
			printf("%s",color);
			printf("         ");
		}
		printf("\n");
	}
  
	printf("\n");
}



void initBoard(uint8_t board[SIZE][SIZE]) {
	addRandom(board);
	addRandom(board);
	drawBoard(board);
}

int randomSpot()
{
	return rand() % SIZE;
}

void addRandom(uint8_t board[SIZE][SIZE]) {

  while(true){
    int x = randomSpot();
    int y = randomSpot();

    if(board[x][y]==0) {
      board[x][y] = (rand()%10)/9+1;
      return;
    }
  }
}


void setBufferedInput(bool enable) {
	static bool enabled = true;
	static struct termios old;
	struct termios new;

	if (enable && !enabled) {
		// restore the former settings
		tcsetattr(STDIN_FILENO,TCSANOW,&old);
		// set the new state
		enabled = true;
	} else if (!enable && enabled) {
		// get the terminal settings for standard input
		tcgetattr(STDIN_FILENO,&new);
		// we want to keep the old setting to restore them at the end
		old = new;
		// disable canonical mode (buffered i/o) and local echo
		new.c_lflag &=(~ICANON & ~ECHO);
		// set the new settings immediately
		tcsetattr(STDIN_FILENO,TCSANOW,&new);
		// set the new state
		enabled = false;
	}
}


bool slideRight(uint8_t arr[SIZE][SIZE]){
  bool slided = false;
  for (int x = 0; x< SIZE; x++){
    int stop = SIZE; //add until this index
    for(int k = 0; k<SIZE; k++){
      for(int y=SIZE-1; y>0; y--){
        if(arr[x][y-1] != 0 ){
          if(arr[x][y]==0){
            arr[x][y] = arr[x][y-1];
            arr[x][y-1] = 0;
            slided = true;
          }
         
          if(arr[x][y-1] == arr[x][y] && y<stop){
            arr[x][y]++;
            arr[x][y-1] = 0; 
            slided = true;
            stop = y;//change stop index
            
          }
          
          
        }
      //end of y
      }
      //end of k
    }
    //end of x
  }
  return slided;
}
bool slideLeft(uint8_t arr[SIZE][SIZE]){
  bool slided = false;
  for (int x = 0; x< SIZE; x++){
    //for each array
    int stop = 0;
    for(int k = 0; k<SIZE; k++){
      //check zeros
      for(int y=1; y<SIZE; y++){
        //for each element in inner array
        if(arr[x][y] != 0 ){ //if target has value
          if(arr[x][y-1]==0){ //if next number is zero, interchange value
            arr[x][y-1] = arr[x][y];
            arr[x][y] = 0;
            slided = true;
          }
        
        
          else if(arr[x][y-1] == arr[x][y] && y > stop){ //if next value is same then add
            arr[x][y-1]++;
            arr[x][y] = 0; 
            slided = true;
            stop = y;
         
          }
        
        }
      //end of y
      }
      //end of k
    }
    //end of x
  }
  return slided;
}

bool slideUp(uint8_t arr[SIZE][SIZE]){
  bool slided = false;

  for (int y = 0; y< SIZE; y++){
    int stop = 0;
    //for each array
    for(int k = 0; k<SIZE; k++){
      //check zeros
      for(int x=1; x<SIZE; x++){
        //for each element in inner array
        if(arr[x][y] != 0 ){ //if target has value
          if(arr[x-1][y]==0){ //if next number is zero, interchange value
            arr[x-1][y] = arr[x][y];
            arr[x][y] = 0;
            slided = true; // slided
          }
          
          if(arr[x-1][y] == arr[x][y] && x > stop){ //if next value is same then add
            arr[x-1][y]++;
            arr[x][y] = 0; 
            slided = true; //slided
            stop = x;
            
           }          
        }
      //end of y
      }
      //end of k
    }
    //end of x
  }
  return slided;
}

bool slideDown(uint8_t arr[SIZE][SIZE]){
  bool slided = false;
  for (int y = 0; y< SIZE; y++){
    int stop = SIZE;
    for(int k = 0; k<SIZE; k++){
      for(int x=SIZE-1; x>0; x--){
        if(arr[x-1][y] != 0 ){
          if(arr[x][y]==0){
            arr[x][y] = arr[x-1][y];
            arr[x-1][y] = 0;
            slided = true;
          }
          else if(arr[x-1][y] == arr[x][y] && x<stop){
            arr[x][y]++;
            arr[x-1][y] = 0;
            slided = true;
            stop= x; 
          }
      
        }
      //end of y
      }
      //end of k
    }
    //end of x
  }
  return slided;
}
