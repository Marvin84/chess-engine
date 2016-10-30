//
//  main.c
//  ChessEngine
//
//  Created by Tina Raissi on 31/03/14.
//  Copyright (c) 2014 Tina Raissi. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ChessEngine.h"



void printBoard ();
int readNotation (char *moveNotation);
int readIndex (int indexType);
int controlNotation (char *moveNotation);
void victoryMessage(int victory);


int main(int argc, const char * argv[])
{
    int origIndex, destIndex, origX, origY, destX, destY;
    bool legalMove = false;
    short unsigned int victory = VC_NO_VICTORY;
    
    initialize();
    printBoard();
    
    
    while(!victory){
        
        while (!legalMove){
            origIndex = readIndex(ORIGINE);
            destIndex = readIndex(DESTINATION);
            transf_index2rel(origIndex, &origX, &origY);
            transf_index2rel(destIndex, &destX, &destY);
            if (isMoveLegal (origX, origY, destX, destY)){
                executeMove(origX, origY, destX, destY);
                printBoard();
                victory = checkForVictory();
                victoryMessage(victory);
                setColor();
            }
            
            else printf("Illegal move, let's try again!\n");
        }
       
       
    }
    
    printf("END\n");
    
        return 0;
        
    }
    


void printBoard (){
    
    printf("  ----------------\n");
    
    int x,y, absX, absY;
    for (y = 8; y > 0; y--) {
        putchar('1' + (y - 1));
        for(x = 1; x < 9; x++) {
            putchar('|');
            if (getCurrentPlayer() == PT_BLACK){
                absY = 9 - y;
            }
            else { absY = y;}
            absX = x;
            
            switch (getSquareContent(absX, absY)) {
                case PC_PAWN_WHITE:
                    putchar('W');
                    break;
                case PC_PAWN_BLACK:
                    putchar('B');
                    break;
                case PC_EMPTY:
                    putchar('.');
                    break;
                case PC_QUEEN_WHITE:
                    putchar('Q');
                    break;
                case PC_QUEEN_BLACK:
                    putchar('Q');
                    break;
                default:
                    break;
            }
        }
        putchar('|');
        putchar('\n');
    }
    printf("  ----------------\n");
    printf("  A B C D E F G H\n");
    
    return;
}

int readNotation (char *moveNotation){
    
    int x, y;
    
    if (strchr(moveNotation, 'a'))
        x = 1;
    else if (strchr(moveNotation, 'b'))
        x = 2;
    else if (strchr(moveNotation, 'c'))
        x = 3;
    else if (strchr(moveNotation, 'd'))
        x = 4;
    else if (strchr(moveNotation, 'e'))
        x = 5;
    else if (strchr(moveNotation, 'f'))
        x = 6;
    else if (strchr(moveNotation, 'g'))
        x  = 7;
    else
        x = 8;
    
    if (strchr(moveNotation, '1'))
        y = 1;
    else if (strchr(moveNotation, '2'))
        y = 2;
    else if (strchr(moveNotation, '3'))
        y = 3;
    else if (strchr(moveNotation, '4'))
        y = 4;
    else if (strchr(moveNotation, '5'))
        y = 5;
    else if (strchr(moveNotation, '6'))
        y = 6;
    else if (strchr(moveNotation, '7'))
        y = 7;
    else
        y = 8;
    
    
    return transf_abs2array (x, y);
    
}


int readIndex (int indexType) {
    
    int x, y, index = ERROR;
    char moveNotation[3];
    
    
    while (index < 0){
        
        if (indexType == ORIGINE)
            printf("enter the piece you want to move: \n");
        else
            printf("enter where you want to move to: \n");
        
        scanf("%s", moveNotation);
        index = readNotation (moveNotation, getCurrentPlayer());
        transf_index2rel(index, &x, &y);
        
        if (!controlNotation(moveNotation)){
            printf("Invalid notation!\n");
            index = ERROR;
        }
        if (indexType == ORIGINE){
            if(getSquareContent(x,y) == PC_EMPTY){
                printf("The square is empty!\n");
                index = ERROR;}
            else if (getSquareContent(x,y) != getCurrentPlayer()+1){
                printf("You are moving the opposite color!\n");
                index = ERROR;
            }
        }
       
    }
    
    return index;
}



int controlNotation (char* moveNotation){
    
    int letter, number;
    letter = moveNotation[0];
    number = moveNotation[1];
    if ((letter >= 97 && letter <= 104) && (number >= 49 && number <= 57) ){
        return true;
    }
    else return false;
}



void victoryMessage(victory){
    
    switch(victory){
            
        case VC_DRAW:
            printf("Draw!\n");
            break;
        case VC_VICTORY:
            if (getCurrentPlayer() == PT_WHITE)
                printf("White wins!\n");
            else
                printf("Black wins!\n");
            break;
        case VC_NO_VICTORY:
            printf("The game continues\n");
            if (getCurrentPlayer() == PT_WHITE)
                printf("Now it's the turn of Black!\n");
            else
                printf("Now it's the turn of White!\n");
            break;
            default:
            break;
 
    }
    
    return;
    
}

