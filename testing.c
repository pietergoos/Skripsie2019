#include <stdio.h>
#include <stdlib.h>
#include "testing.h"

int leds[16][5];
int currTime;
char str[20];

int main()
{
    //INITIALIZATION
    currTime = 0;
    altCol();
    //altLED(rand() % 5, rand() % 16);

    printArr();

    int r = 0;
    int c = 0;

    while(1){
        r = 0;
        c = 0;
        scanf("%[^\n]%*c", str); 

        switch(str[0]){
            case 'i':
                timeIncrem();
                printArr();
                break;
            case 'l':
                sscanf(str, "l %d,%d", &r, &c);
                altLED(r,c);
                printArr();
                break;
            case 'v':
                printArr();
                break;
            case 'c':
                altCol();
                printArr();
                break;
            default:
                printf("Invalid Command"); 
        }        
    }
    
    return 0;
}

void timeIncrem(){
    altCol();
    currTime++;
    if (currTime == 16){
        currTime = 0;
    }
    altCol();
}

void altCol(){
    for(int i = 0; i<5; i++){
        if (leds[currTime][i] == 0){
            leds[currTime][i] = 1;
        }else{
            leds[currTime][i] = 0;
        }
    }
}

void altLED(int row, int col){
    if (leds[col][row] == 0){
        leds[col][row] = 1;
    }else{
        leds[col][row] = 0;
    }
}

void printArr(){
    for(int i = 0; i<5; i++){
        for(int j = 0; j<16; j++){
            printf("%d", leds[j][i]);
        }
        printf("\n");
    }
}